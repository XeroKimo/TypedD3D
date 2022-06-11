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

        namespace Shader
        {
            template<class WrapperTy>
            class Interface : public DeviceChild::Interface<WrapperTy>
            {

            };
        }

    }

    template<D3D11::ShaderConcept ShaderTy>
    class InterfaceWrapper<ShaderTy> : public ComWrapper<ShaderTy>, private D3D11::Shader::Interface<InterfaceWrapper<ShaderTy>>
    {
    private:
        using Interface = D3D11::Shader::Interface<InterfaceWrapper<ShaderTy>>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = ShaderTy;

    public:
        using ComWrapper<ShaderTy>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}