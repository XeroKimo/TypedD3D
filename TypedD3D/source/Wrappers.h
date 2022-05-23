#pragma once
#include "Helpers/COMHelpers.h"

namespace TypedD3D
{
    namespace Internal
    {
        template<class IUnknownTy, auto EnumTypeConstraint = 0>
        struct InterfaceMapper;

        template<class DirectXClass, auto EnumTypeConstraint = 0>
        class InterfaceWrapper;
    }

    template<class IUnknownTy>
    using Wrapper = typename Internal::InterfaceMapper<IUnknownTy>::type;

    template<class DerivedTy, class DirectXClass, auto EnumTypeConstraint>
        requires std::is_base_of_v<IUnknown, DirectXClass>
    Internal::InterfaceWrapper<DerivedTy, EnumTypeConstraint> Cast(Internal::InterfaceWrapper<DirectXClass, EnumTypeConstraint> value)
    {
        return Internal::InterfaceWrapper<DerivedTy, EnumTypeConstraint>(Helpers::COM::Cast<DerivedTy>(value.GetComPtr()));
    }
}