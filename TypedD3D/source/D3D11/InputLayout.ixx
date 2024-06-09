module;
#include <d3d11_4.h>
#include "source/Wrappers.h"
export module TypedD3D11:InputLayout;
import :DeviceChild;

import TypedD3D.Shared;


namespace TypedD3D::Internal
{
    namespace D3D11
    {
        namespace InputLayout
        {
        }
    }
}

namespace TypedD3D
{
    template<>
    struct Traits<ID3D11InputLayout>
    {
        using value_type = ID3D11InputLayout;
        using pointer = ID3D11InputLayout*;
        using const_pointer = const ID3D11InputLayout*;
        using reference = ID3D11InputLayout&;
        using const_reference = const ID3D11InputLayout&;

        template<class DerivedSelf>
        class Interface : public Internal::D3D11::DeviceChild::Interface<DerivedSelf>
        {

        };
    };
}