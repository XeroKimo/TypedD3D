#pragma once
#include "Helpers/COMHelpers.h"

namespace TypedD3D
{
    namespace Internal
    {
        template<class Ty, auto... Tags>
        struct Traits;

        template<class Ty, auto... Tags>
        class IUnknownWrapper : public Internal::InterfaceWrapper<Ty, typename Internal::Traits<Ty, Tags...>::Interface>
        {
        private:
            using Base = Internal::InterfaceWrapper<Ty, typename Internal::Traits<Ty, Tags...>::Interface>;

        public:
            using traits_type = Internal::Traits<Ty, Tags...>;

        public:
            using Base::Base;
            using Base::operator=;
        };

        template<class Ty>
        struct WrapperMapper;

        template<std::derived_from<IUnknown> Ty>
        struct WrapperMapper<Ty>
        {
            using type = IUnknownWrapper<Ty>;
        };
    }

    template<class Ty>
    using Wrapper = Internal::WrapperMapper<Ty>::type;

    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
    Internal::IUnknownWrapper<DerivedTy, Tags...> Cast(Internal::IUnknownWrapper<Ty, Tags...>& other) noexcept
    {
        return Internal::IUnknownWrapper<DerivedTy, Tags...>(Internal::Cast<DerivedTy, typename Internal::IUnknownWrapper<DerivedTy, Tags...>::interface_type>(other));
    }

    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
    Internal::IUnknownWrapper<DerivedTy, Tags...> Cast(Internal::IUnknownWrapper<Ty, Tags...>&& other) noexcept
    {
        return Internal::IUnknownWrapper<DerivedTy, Tags...>(Internal::Cast<DerivedTy, typename Internal::IUnknownWrapper<DerivedTy, Tags...>::interface_type>(std::move(other)));
    }
}