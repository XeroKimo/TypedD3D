#pragma once

namespace TypedD3D
{
    namespace Internal
    {
        template<class IUnknownTy, auto EnumTypeConstraint = 0>
        struct InterfaceMapper;
    }

    template<class IUnknownTy>
    using Wrapper = typename Internal::InterfaceMapper<IUnknownTy>::type;
}