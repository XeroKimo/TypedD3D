#pragma once
#include "DeviceChild.h"

namespace TypedD3D::Internal
{
    namespace D3D11
    {
        template<class Ty>
        concept ShaderConcept =
            std::same_as<Ty, ID3D11VertexShader> ||
            std::same_as<Ty, ID3D11DomainShader> ||
            std::same_as<Ty, ID3D11HullShader> ||
            std::same_as<Ty, ID3D11GeometryShader> ||
            std::same_as<Ty, ID3D11PixelShader> ||
            std::same_as<Ty, ID3D11ComputeShader>;
    }

    template<D3D11::ShaderConcept Ty>
    struct Traits<Ty>
    {
        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using const_reference = const Ty&;

        template<class DerivedSelf>
        class Interface : public D3D11::DeviceChild::Interface<DerivedSelf>
        {

        };
    };
}

namespace TypedD3D::D3D11
{
    using VertexShader = Wrapper<ID3D11VertexShader>;
    using DomainShader = Wrapper<ID3D11DomainShader>;
    using HullShader = Wrapper<ID3D11HullShader>;
    using GeometryShader = Wrapper<ID3D11GeometryShader>;
    using PixelShader = Wrapper<ID3D11PixelShader>;
    using ComputeShader = Wrapper<ID3D11ComputeShader>;
}