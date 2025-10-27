module;
#include <d3d11_4.h>

export module TypedD3D11:InputLayout;
import :DeviceChild;

import TypedD3D.Shared;

namespace TypedD3D
{
    template<>
    struct UntaggedTraits<ID3D11InputLayout>
    {
        using value_type = ID3D11InputLayout;
        using pointer = ID3D11InputLayout*;
        using const_pointer = const ID3D11InputLayout*;
        using reference = ID3D11InputLayout&;
        using const_reference = const ID3D11InputLayout&;
        using inner_type = ID3D11InputLayout;

        template<class Derived>
        class Interface : public UntaggedTraits<ID3D11DeviceChild>::Interface<Derived>
        {

        };
    };
}