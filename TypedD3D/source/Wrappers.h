#pragma once

#include <concepts>
#include <wrl/client.h>
import TypedD3D.Shared;
namespace TypedD3D
{
    namespace Internal
    {
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
}