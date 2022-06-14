#pragma once
#include "Helpers/COMHelpers.h"

namespace TypedD3D
{
    namespace Internal
    {
        template<class Ty>
        struct WrapperMapper;
    }

    template<class Ty>
    using Wrapper = Internal::WrapperMapper<Ty>::type;

    template<std::derived_from<IUnknown> OtherTy, std::derived_from<IUnknown> Ty>
    Wrapper<OtherTy> Cast(const Wrapper<Ty>& other) noexcept
    {
        return Internal::Cast<Wrapper<OtherTy>, typename Wrapper<OtherTy>::interface_type>(other);
    }

    template<std::derived_from<IUnknown> OtherTy, std::derived_from<IUnknown> Ty>
    Wrapper<OtherTy> Cast(Wrapper<Ty>&& other) noexcept
    {
        return Internal::Cast<Wrapper<OtherTy>, typename Wrapper<OtherTy>::interface_type>(std::move(other));
    }
}