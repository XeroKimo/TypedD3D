#pragma once
#include "Helpers/COMHelpers.h"

namespace TypedD3D
{
    namespace Internal
    {
        template<class Ty, auto... Tags>
        struct Traits;

        template<class Ty, auto... Tags>
        class Wrapper : public Internal::InterfaceWrapper<Ty, typename Internal::Traits<Ty, Tags...>::Interface>
        {
        private:
            using Base = Internal::InterfaceWrapper<Ty, typename Internal::Traits<Ty, Tags...>::Interface>;

        public:
            using traits_type = Internal::Traits<Ty, Tags...>;

        public:
            using Base::Base;
            using Base::operator=;
        };

        template<class Ty, auto... Tags>
        struct WrapperMapper
        {
            using type = Wrapper<Ty, Tags...>;
        };
    }

    template<class Ty, auto... Tags>
    using Wrapper = Internal::WrapperMapper<Ty, Tags...>::type;

    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
    Wrapper<DerivedTy, Tags...> Cast(Internal::Wrapper<Ty, Tags...>& other) noexcept
    {
        return Wrapper<DerivedTy, Tags...>(Internal::Cast<DerivedTy, typename Wrapper<DerivedTy, Tags...>::interface_type>(other));
    }

    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
    Wrapper<DerivedTy, Tags...> Cast(Internal::Wrapper<Ty, Tags...>&& other) noexcept
    {
        return Wrapper<DerivedTy, Tags...>(Internal::Cast<DerivedTy, typename Wrapper<DerivedTy, Tags...>::interface_type>(std::move(other)));
    }
}