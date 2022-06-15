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

    template<std::derived_from<IUnknown> OtherTy, class Ty>
    Wrapper<OtherTy> Cast(Ty& other) noexcept
    {
        return Wrapper<OtherTy>(Internal::Cast<OtherTy, typename Wrapper<OtherTy>::interface_type>(other));
    }

    template<std::derived_from<IUnknown> OtherTy, class Ty>
    Wrapper<OtherTy> Cast(Ty&& other) noexcept
    {
        return Wrapper<OtherTy>(Internal::Cast<OtherTy, typename Wrapper<OtherTy>::interface_type>(std::move(other)));
    }
}