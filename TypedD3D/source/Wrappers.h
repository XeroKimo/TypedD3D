#pragma once
#include "Helpers/COMHelpers.h"

namespace TypedD3D
{
    namespace Internal
    {
        template<class DirectXClass, auto EnumTypeConstraint = 0>
        class InterfaceWrapper;

        template<class>
        struct is_interface_wrapper : std::false_type {};

        template<class DirectXClass, auto EnumTypeConstraint>
        struct is_interface_wrapper<InterfaceWrapper<DirectXClass, EnumTypeConstraint>> : std::true_type {};
    }

    template<class IUnknownTy>
    using Wrapper = Internal::InterfaceWrapper<IUnknownTy>;

    template<class DerivedTy, class DirectXClass, auto EnumTypeConstraint>
        requires std::is_base_of_v<IUnknown, DerivedTy>
    Internal::InterfaceWrapper<DerivedTy, EnumTypeConstraint> Cast(Internal::InterfaceWrapper<DirectXClass, EnumTypeConstraint> value)
    {
        return Internal::InterfaceWrapper<DerivedTy, EnumTypeConstraint>(Helpers::COM::Cast<DerivedTy>(value.GetComPtr()));
    }    
    
    template<class DerivedTy, class DirectXClass, auto EnumTypeConstraint>
        requires Internal::is_interface_wrapper<DerivedTy>::value &&
            std::is_base_of_v<IUnknown, typename DerivedTy::underlying_type> &&
            (Internal::InterfaceWrapper<DirectXClass, EnumTypeConstraint>::tag_value == DerivedTy::tag_value)
    DerivedTy Cast(Internal::InterfaceWrapper<DirectXClass, EnumTypeConstraint> value)
    {
        return DerivedTy(Helpers::COM::Cast<typename DerivedTy::underlying_type>(value.GetComPtr()));
    }
}