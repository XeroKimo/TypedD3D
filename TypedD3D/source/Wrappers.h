#pragma once
#include "Helpers/COMHelpers.h"

namespace TypedD3D
{
    namespace Internal
    {
        template<class DirectXClass, auto... Tags>
        class InterfaceWrapper;

        template<class>
        struct is_interface_wrapper : std::false_type {};

        template<class DirectXClass, auto... Tags>
        struct is_interface_wrapper<InterfaceWrapper<DirectXClass, Tags...>> : std::true_type {};
    }

    template<class IUnknownTy, auto... ExtraTags>
    using Wrapper = Internal::InterfaceWrapper<IUnknownTy, ExtraTags...>;

    template<class DerivedTy, class DirectXClass, auto... Tags>
        requires std::derived_from<DerivedTy, IUnknown>
    Internal::InterfaceWrapper<DerivedTy, Tags...> Cast(Internal::InterfaceWrapper<DirectXClass, Tags...> value)
    {
        return Internal::InterfaceWrapper<DerivedTy, Tags...>(Helpers::COM::Cast<DerivedTy>(value.GetComPtr()));
    }    
    
    template<class DerivedTy, class DirectXClass, auto... Tags>
        requires Internal::is_interface_wrapper<DerivedTy>::value &&
            std::derived_from<typename DerivedTy::underlying_type, IUnknown> &&
            (Internal::InterfaceWrapper<DirectXClass, Tags...>::tag_value == DerivedTy::tag_value)
    DerivedTy Cast(Internal::InterfaceWrapper<DirectXClass, Tags...> value)
    {
        return DerivedTy(Helpers::COM::Cast<typename DerivedTy::underlying_type>(value.GetComPtr()));
    }
}