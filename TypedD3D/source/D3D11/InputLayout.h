#pragma once
#include "DeviceChild.h"


namespace TypedD3D::Internal
{
    namespace D3D11
    {
        namespace InputLayout
        {
        }
    }

    template<>
    struct Traits<ID3D11InputLayout>
    {
        using value_type = ID3D11InputLayout;
        using pointer = ID3D11InputLayout*;
        using const_pointer = const ID3D11InputLayout*;
        using reference = ID3D11InputLayout&;
        using const_reference = const ID3D11InputLayout&;

        template<class DerivedSelf>
        class Interface : public D3D11::DeviceChild::Interface<DerivedSelf>
        {

        };
    };
}