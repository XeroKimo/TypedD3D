#pragma once
#include "source/Wrappers.h"
#include "source/Internal/IUnknownWrapper.h"
#include "source/Helpers/COMHelpers.h"
#include "Resources.h"
#include "Shaders.h"
#include "InputLayout.h"
#include "ResourceViews.h"
#include "States.h"
#include "gsl/pointers"
#include <d3d11_4.h>
#include <span>

namespace TypedD3D::Internal
{
    namespace D3D11
    {
        template<class Ty>
        using Device_t = IUnknownWrapper<Ty>;

        namespace Device
        {
            template<D3D11_FEATURE Feature>
            struct FeatureMap;

            template<>
            struct FeatureMap<D3D11_FEATURE_THREADING>
            {
                using type = D3D11_FEATURE_DATA_THREADING;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_DOUBLES>
            {
                using type = D3D11_FEATURE_DATA_DOUBLES;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_FORMAT_SUPPORT>
            {
                using type = D3D11_FEATURE_DATA_FORMAT_SUPPORT;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_FORMAT_SUPPORT2>
            {
                using type = D3D11_FEATURE_DATA_FORMAT_SUPPORT;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS>
            {
                using type = D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D11_OPTIONS>
            {
                using type = D3D11_FEATURE_DATA_D3D11_OPTIONS;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_ARCHITECTURE_INFO>
            {
                using type = D3D11_FEATURE_DATA_ARCHITECTURE_INFO;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D9_OPTIONS>
            {
                using type = D3D11_FEATURE_DATA_D3D9_OPTIONS;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_SHADER_MIN_PRECISION_SUPPORT>
            {
                using type = D3D11_FEATURE_DATA_SHADER_MIN_PRECISION_SUPPORT;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D9_SHADOW_SUPPORT>
            {
                using type = D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D11_OPTIONS1>
            {
                using type = D3D11_FEATURE_DATA_D3D11_OPTIONS1;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D9_SIMPLE_INSTANCING_SUPPORT>
            {
                using type = D3D11_FEATURE_DATA_D3D9_SIMPLE_INSTANCING_SUPPORT;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_MARKER_SUPPORT>
            {
                using type = D3D11_FEATURE_DATA_MARKER_SUPPORT;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D9_OPTIONS1>
            {
                using type = D3D11_FEATURE_DATA_D3D9_OPTIONS1;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D11_OPTIONS2>
            {
                using type = D3D11_FEATURE_DATA_D3D11_OPTIONS2;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D11_OPTIONS3>
            {
                using type = D3D11_FEATURE_DATA_D3D11_OPTIONS3;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT>
            {
                using type = D3D11_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D11_OPTIONS4>
            {
                using type = D3D11_FEATURE_DATA_D3D11_OPTIONS4;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_SHADER_CACHE>
            {
                using type = D3D11_FEATURE_DATA_SHADER_CACHE;
            };

            template<>
            struct FeatureMap<D3D11_FEATURE_D3D11_OPTIONS5>
            {
                using type = D3D11_FEATURE_DATA_D3D11_OPTIONS5;
            };


            template<class Ty>
            struct Traits;

            template<>
            struct Traits<ID3D11Device>
            {
                using value_type = ID3D11Device;
                using pointer = ID3D11Device*;
                using const_pointer = const ID3D11Device*;
                using reference = ID3D11Device&;
                using const_reference = const ID3D11Device&;

                template<class DerivedSelf>
                class Interface
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    tl::expected<Wrapper<ID3D11Buffer>, HRESULT> CreateBuffer(
                        const D3D11_BUFFER_DESC& pDesc,
                        const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11Buffer>(&value_type::CreateBuffer, Get(), &pDesc, optInitialData)
                            .and_then([](auto resource) -> tl::expected<Wrapper<ID3D11Buffer>, HRESULT> { return Wrapper<ID3D11Buffer>(resource); });
                    }

                    tl::expected<Wrapper<ID3D11Texture1D>, HRESULT> CreateTexture1D(
                        const D3D11_TEXTURE1D_DESC& pDesc,
                        const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11Texture1D>(&value_type::CreateTexture1D, Get(), &pDesc, optInitialData)
                            .and_then([](auto resource) -> tl::expected<Wrapper<ID3D11Texture1D>, HRESULT> { return Wrapper<ID3D11Texture1D>(resource); });
                    }

                    tl::expected<Wrapper<ID3D11Texture2D>, HRESULT> CreateTexture2D(
                        const D3D11_TEXTURE2D_DESC& pDesc,
                        const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11Texture2D>(&value_type::CreateTexture2D, Get(), &pDesc, optInitialData)
                            .and_then([](auto resource) -> tl::expected<Wrapper<ID3D11Texture2D>, HRESULT> { return Wrapper<ID3D11Texture2D>(resource); });
                    }

                    tl::expected<Wrapper<ID3D11Texture3D>, HRESULT> CreateTexture3D(
                        const D3D11_TEXTURE3D_DESC& pDesc,
                        const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11Texture3D>(&value_type::CreateTexture3D, Get(), &pDesc, optInitialData)
                            .and_then([](auto resource) -> tl::expected<Wrapper<ID3D11Texture3D>, HRESULT> { return Wrapper<ID3D11Texture3D>(resource); });
                    }

                    tl::expected<Wrapper<ID3D11ShaderResourceView>, HRESULT> CreateShaderResourceView(
                        gsl::not_null<Wrapper<ID3D11Resource>> pResource,
                        const D3D11_SHADER_RESOURCE_VIEW_DESC* optDesc = nullptr)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11ShaderResourceView>(&value_type::CreateShaderResourceView, Get(), pResource.get().Get(), optDesc)
                            .and_then([](auto view) -> tl::expected<Wrapper<ID3D11ShaderResourceView>, HRESULT> { return Wrapper<ID3D11ShaderResourceView>(view); });
                    }

                    tl::expected<Wrapper<ID3D11UnorderedAccessView>, HRESULT> CreateUnorderedAccessView(
                        gsl::not_null<Wrapper<ID3D11Resource>> pResource,
                        const D3D11_UNORDERED_ACCESS_VIEW_DESC* optDesc = nullptr)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11UnorderedAccessView>(&value_type::CreateUnorderedAccessView, Get(), pResource.get().Get(), optDesc)
                            .and_then([](auto view) -> tl::expected<Wrapper<ID3D11UnorderedAccessView>, HRESULT> { return Wrapper<ID3D11UnorderedAccessView>(view); });
                    }

                    tl::expected<Wrapper<ID3D11RenderTargetView>, HRESULT> CreateRenderTargetView(
                        gsl::not_null<Wrapper<ID3D11Resource>> pResource,
                        const D3D11_RENDER_TARGET_VIEW_DESC* optDesc = nullptr)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11RenderTargetView>(&value_type::CreateRenderTargetView, Get(), pResource.get().Get(), optDesc)
                            .and_then([](auto view) -> tl::expected<Wrapper<ID3D11RenderTargetView>, HRESULT> { return Wrapper<ID3D11RenderTargetView>(view); });
                    }

                    tl::expected<Wrapper<ID3D11DepthStencilView>, HRESULT> CreateDepthStencilView(
                        gsl::not_null<Wrapper<ID3D11Resource>> pResource,
                        const D3D11_DEPTH_STENCIL_VIEW_DESC* optDesc = nullptr)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11DepthStencilView>(&value_type::CreateDepthStencilView, Get(), pResource.get().Get(), optDesc)
                            .and_then([](auto view) -> tl::expected<Wrapper<ID3D11DepthStencilView>, HRESULT> { return Wrapper<ID3D11DepthStencilView>(view); });
                    }

                    tl::expected<Wrapper<ID3D11InputLayout>, HRESULT> CreateInputLayout(
                        std::span<const D3D11_INPUT_ELEMENT_DESC> inputElementDescs,
                        ID3DBlob& pShaderBytecodeWithInputSignature)
                    {
                        return CreateInputLayout(inputElementDescs, pShaderBytecodeWithInputSignature.GetBufferPointer(), pShaderBytecodeWithInputSignature.GetBufferSize());
                    }

                    tl::expected<Wrapper<ID3D11InputLayout>, HRESULT> CreateInputLayout(
                        std::span<const D3D11_INPUT_ELEMENT_DESC> inputElementDescs,
                        const void* pShaderBytecodeWithInputSignature,
                        SIZE_T BytecodeLength)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11InputLayout>(&value_type::CreateInputLayout, Get(), inputElementDescs.data(), static_cast<UINT>(inputElementDescs.size()), pShaderBytecodeWithInputSignature, BytecodeLength)
                            .and_then([](auto inputLayout) -> tl::expected<Wrapper<ID3D11InputLayout>, HRESULT> { return Wrapper<ID3D11InputLayout>(inputLayout); });
                    }

                    tl::expected<Wrapper<ID3D11VertexShader>, HRESULT> CreateVertexShader(
                        ID3DBlob& pShaderBytecode,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        return CreateVertexShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                    }

                    tl::expected<Wrapper<ID3D11VertexShader>, HRESULT> CreateVertexShader(
                        const void* pShaderBytecode,
                        SIZE_T BytecodeLength,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        using ShaderTy = Wrapper<ID3D11VertexShader>;
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11VertexShader>(&value_type::CreateVertexShader, Get(), pShaderBytecode, BytecodeLength, pClassLinkage)
                            .and_then([](auto shader) -> tl::expected<ShaderTy, HRESULT> { return ShaderTy(shader); });
                    }

                    tl::expected<Wrapper<ID3D11GeometryShader>, HRESULT> CreateGeometryShader(
                        ID3DBlob& pShaderBytecode,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        return CreateGeometryShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                    }

                    tl::expected<Wrapper<ID3D11GeometryShader>, HRESULT> CreateGeometryShader(
                        const void* pShaderBytecode,
                        SIZE_T BytecodeLength,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        using ShaderTy = Wrapper<ID3D11GeometryShader>;
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11GeometryShader>(&value_type::CreateGeometryShader, Get(), pShaderBytecode, BytecodeLength, pClassLinkage)
                            .and_then([](auto shader) -> tl::expected<ShaderTy, HRESULT> { return ShaderTy(shader); });
                    }

                    tl::expected<Wrapper<ID3D11GeometryShader>, HRESULT> CreateGeometryShaderWithStreamOutput(
                        ID3DBlob& pShaderBytecode,
                        std::span<const D3D11_SO_DECLARATION_ENTRY> pSODeclaration,
                        std::span<const UINT> pBufferStrides,
                        UINT RasterizedStream,
                        ID3D11ClassLinkage* optClassLinkage)
                    {
                        return CreateGeometryShaderWithStreamOutput(
                            pShaderBytecode.GetBufferPointer(),
                            pShaderBytecode.GetBufferSize(),
                            pSODeclaration,
                            pBufferStrides,
                            RasterizedStream,
                            optClassLinkage);
                    }

                    tl::expected<Wrapper<ID3D11GeometryShader>, HRESULT> CreateGeometryShaderWithStreamOutput(
                        const void* pShaderBytecode,
                        SIZE_T BytecodeLength,
                        std::span<const D3D11_SO_DECLARATION_ENTRY> pSODeclaration,
                        std::span<const UINT> pBufferStrides,
                        UINT RasterizedStream,
                        ID3D11ClassLinkage* optClassLinkage)
                    {
                        using ShaderTy = Wrapper<ID3D11GeometryShader>;
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11GeometryShader>(
                            &value_type::CreateGeometryShaderWithStreamOutput,
                            Get(),
                            pShaderBytecode,
                            BytecodeLength,
                            pSODeclaration.data(),
                            pSODeclaration.size(),
                            pBufferStrides.data(),
                            pBufferStrides.size(),
                            RasterizedStream,
                            optClassLinkage)
                            .and_then([](auto shader) -> tl::expected<ShaderTy, HRESULT> { return ShaderTy(shader); });
                    }

                    tl::expected<Wrapper<ID3D11PixelShader>, HRESULT> CreatePixelShader(
                        ID3DBlob& pShaderBytecode,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        return CreatePixelShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                    }

                    tl::expected<Wrapper<ID3D11PixelShader>, HRESULT> CreatePixelShader(
                        const void* pShaderBytecode,
                        SIZE_T BytecodeLength,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        using ShaderTy = Wrapper<ID3D11PixelShader>;
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11PixelShader>(&value_type::CreatePixelShader, Get(), pShaderBytecode, BytecodeLength, pClassLinkage)
                            .and_then([](auto shader) -> tl::expected<ShaderTy, HRESULT> { return ShaderTy(shader); });
                    }

                    tl::expected<Wrapper<ID3D11HullShader>, HRESULT> CreateHullShader(
                        ID3DBlob& pShaderBytecode,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        return CreateHullShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                    }

                    tl::expected<Wrapper<ID3D11HullShader>, HRESULT> CreateHullShader(
                        const void* pShaderBytecode,
                        SIZE_T BytecodeLength,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        using ShaderTy = Wrapper<ID3D11HullShader>;
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11HullShader>(&value_type::CreateHullShader, Get(), pShaderBytecode, BytecodeLength, pClassLinkage)
                            .and_then([](auto shader) -> tl::expected<ShaderTy, HRESULT> { return ShaderTy(shader); });
                    }

                    tl::expected<Wrapper<ID3D11DomainShader>, HRESULT> CreateDomainShader(
                        ID3DBlob& pShaderBytecode,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        return CreateDomainShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                    }

                    tl::expected<Wrapper<ID3D11DomainShader>, HRESULT> CreateDomainShader(
                        const void* pShaderBytecode,
                        SIZE_T BytecodeLength,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        using ShaderTy = Wrapper<ID3D11DomainShader>;
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11DomainShader>(&value_type::CreateDomainShader, Get(), pShaderBytecode, BytecodeLength, pClassLinkage)
                            .and_then([](auto shader) -> tl::expected<ShaderTy, HRESULT> { return ShaderTy(shader); });
                    }

                    tl::expected<Wrapper<ID3D11ComputeShader>, HRESULT> CreateComputeShader(
                        ID3DBlob& pShaderBytecode,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        return CreateComputeShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                    }

                    tl::expected<Wrapper<ID3D11ComputeShader>, HRESULT> CreateComputeShader(
                        const void* pShaderBytecode,
                        SIZE_T BytecodeLength,
                        ID3D11ClassLinkage* pClassLinkage)
                    {
                        using ShaderTy = Wrapper<ID3D11ComputeShader>;
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11ComputeShader>(&value_type::CreateComputeShader, Get(), pShaderBytecode, BytecodeLength, pClassLinkage)
                            .and_then([](auto shader) -> tl::expected<ShaderTy, HRESULT> { return ShaderTy(shader); });
                    }

                    tl::expected<Microsoft::WRL::ComPtr<ID3D11ClassLinkage>, HRESULT> CreateClassLinkage()
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11ClassLinkage>(&value_type::CreateClassLinkage, Get());
                    }

                    tl::expected<Wrapper<ID3D11BlendState>, HRESULT> CreateBlendState(
                        const D3D11_BLEND_DESC& pBlendStateDesc)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11BlendState>(&value_type::CreateBlendState, Get(), &pBlendStateDesc)
                            .and_then([](auto stateObj) -> tl::expected<ID3D11BlendState, HRESULT> { return Wrapper<ID3D11BlendState>(stateObj); });
                    }

                    tl::expected<Wrapper<ID3D11DepthStencilState>, HRESULT> CreateDepthStencilState(
                        const D3D11_DEPTH_STENCIL_DESC& pDepthStencilDesc)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11DepthStencilState>(&value_type::CreateDepthStencilState, Get(), &pDepthStencilDesc)
                            .and_then([](auto stateObj) -> tl::expected<ID3D11DepthStencilState, HRESULT> { return Wrapper<ID3D11DepthStencilState>(stateObj); });
                    }

                    tl::expected<Wrapper<ID3D11RasterizerState>, HRESULT>  CreateRasterizerState(
                        const D3D11_RASTERIZER_DESC& pRasterizerDesc)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11RasterizerState>(&value_type::CreateRasterizerState, Get(), &pRasterizerDesc)
                            .and_then([](auto stateObj) -> tl::expected<ID3D11RasterizerState, HRESULT> { return Wrapper<ID3D11RasterizerState>(stateObj); });
                    }

                    tl::expected<Wrapper<ID3D11SamplerState>, HRESULT> CreateSamplerState(
                        const D3D11_SAMPLER_DESC& pSamplerDesc)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11SamplerState>(&value_type::CreateSamplerState, Get(), &pSamplerDesc)
                            .and_then([](auto stateObj) -> tl::expected<ID3D11SamplerState, HRESULT> { return Wrapper<ID3D11SamplerState>(stateObj); });
                    }

                    tl::expected<Microsoft::WRL::ComPtr<ID3D11Query>, HRESULT> CreateQuery(
                        const D3D11_QUERY_DESC& pQueryDesc)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11Query>(&value_type::CreateQuery, Get(), &pQueryDesc);
                    }

                    tl::expected<Microsoft::WRL::ComPtr<ID3D11Predicate>, HRESULT> CreatePredicate(
                        const D3D11_QUERY_DESC& pPredicateDesc)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11Predicate>(&value_type::CreatePredicate, Get(), &pPredicateDesc);
                    }

                    tl::expected<Microsoft::WRL::ComPtr<ID3D11Counter>, HRESULT> CreateCounter(
                        const D3D11_COUNTER_DESC& pCounterDesc)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<ID3D11Counter>(&value_type::CreateCounter, Get(), &pCounterDesc);
                    }

                    template<std::derived_from<ID3D11DeviceContext> DeviceContextTy = ID3D11DeviceContext>
                    tl::expected<Wrapper<DeviceContextTy>, HRESULT> CreateDeferredContext(
                        UINT ContextFlags)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<DeviceContextTy>(&value_type::CreateDeferredContext, Get(), &ContextFlags)
                            .and_then([](auto deviceContext) -> tl::expected<Wrapper<DeviceContextTy>, HRESULT> { return Wrapper<DeviceContextTy>(deviceContext); });
                    }

                      //TODO: Figure out how this works to update to a more modern API
                    HRESULT OpenSharedResource(
                        HANDLE hResource,
                        REFIID ReturnedInterface,
                        void** ppResource)
                    {
                        return Get().OpenSharedResource(hResource, ReturnedInterface, ppResource);
                    }

                    tl::expected<D3D11_FORMAT_SUPPORT, HRESULT> CheckFormatSupport(
                        DXGI_FORMAT Format)
                    {
                        UINT FormatSupport;
                        if(HRESULT result = Get().CheckFormatSupport(Format, &FormatSupport); FAILED(result))
                            return tl::unexpected(result);

                        return static_cast<D3D11_FORMAT_SUPPORT>(FormatSupport);
                    }

                    HRESULT CheckMultisampleQualityLevels(
                        DXGI_FORMAT Format,
                        UINT SampleCount,
                        UINT* pNumQualityLevels)
                    {
                        return Get().CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
                    }

                    D3D11_COUNTER_INFO CheckCounterInfo()
                    {
                        D3D11_COUNTER_INFO pCounterInfo;
                        Get().CheckCounterInfo(&pCounterInfo);
                        return pCounterInfo;
                    }

                    //TODO: Figure out how this works to update to a more modern API
                    HRESULT CheckCounter(
                        const D3D11_COUNTER_DESC* pDesc,
                        D3D11_COUNTER_TYPE* pType,
                        UINT* pActiveCounters,
                        LPSTR szName,
                        UINT* pNameLength,
                        LPSTR szUnits,
                        UINT* pUnitsLength,
                        LPSTR szDescription,
                        UINT* pDescriptionLength)
                    {
                        return Get().CheckCounter(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
                    }

                    template<D3D11_FEATURE Feature>
                    typename FeatureMap<Feature>::type CheckFeatureSupport()
                    {
                        using feature_type = typename FeatureMap<Feature>::type;
                        feature_type feature;
                        Get().CheckFeatureSupport(Feature, &feature, sizeof(feature_type));
                        return feature;
                    }

                      //TODO: Figure out how this works to update to a more modern API
                    HRESULT GetPrivateData(
                        REFGUID guid,
                        UINT* pDataSize,
                        void* pData)
                    {
                        return Get().GetPrivateData(guid, pDataSize, pData);
                    }

                    //TODO: Figure out how this works to update to a more modern API
                    HRESULT SetPrivateData(
                        REFGUID guid,
                        UINT DataSize,
                        const void* pData)
                    {
                        return Get().SetPrivateData(guid, DataSize, pData);
                    }

                     //TODO: Figure out how this works to update to a more modern API
                    HRESULT SetPrivateDataInterface(
                        REFGUID guid,
                        const IUnknown* pData)
                    {
                        return Get().SetPrivateDataInterface(guid, pData);
                    }

                    D3D_FEATURE_LEVEL GetFeatureLevel()
                    {
                        return Get().GetFeatureLevel();
                    }

                    UINT GetCreationFlags()
                    {
                        return Get().GetCreationFlags();
                    }

                    HRESULT GetDeviceRemovedReason()
                    {
                        return Get().GetDeviceRemovedReason();
                    }

                    template<std::derived_from<ID3D11DeviceContext> DeviceContextTy = ID3D11DeviceContext>
                    Wrapper<DeviceContextTy> GetImmediateContext()
                    {
                        if constexpr(std::same_as<DeviceContextTy, ID3D11DeviceContext>)
                        {
                            return Helpers::COM::UnknownObjectForwardFunction<ID3D11DeviceContext>(&value_type::GetImmediateContext, Get())
                                .map([](auto deviceContext) { return Wrapper<DeviceContextTy>(deviceContext); });
                        }
                        else
                        {
                            return Helpers::COM::UnknownObjectForwardFunction<ID3D11DeviceContext>(&value_type::GetImmediateContext, Get())
                                .map([](auto deviceContext) { return Wrapper<DeviceContextTy>(Helpers::COM::Cast<DeviceContextTy>(deviceContext)); });
                        }
                    }

                    HRESULT SetExceptionMode(
                        UINT RaiseFlags)
                    {
                        return Get().SetExceptionMode(RaiseFlags);
                    }

                    UINT GetExceptionMode()
                    {
                        return Get().GetExceptionMode();
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

        }
    }

    template<std::derived_from<ID3D11Device> Ty>
    struct Traits<Ty>
    {
        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using const_reference = const Ty&;

        template<class DerivedSelf>
        using Interface = typename D3D11::Device::Traits<Ty>::template Interface<DerivedSelf>;
    };
}

namespace TypedD3D::D3D11
{
    template<std::derived_from<ID3D11Device> DeviceTy>
    using Device_t = Internal::D3D11::Device_t<DeviceTy>;

    using Device = Device_t<ID3D11Device>;

    template<class DeviceTy = ID3D11Device, class DeviceContextTy = ID3D11DeviceContext>
    auto CreateDevice(
        IDXGIAdapter* optAdapter,
        D3D_DRIVER_TYPE DriverType,
        HMODULE Software,
        UINT Flags,
        std::span<const D3D_FEATURE_LEVEL> pFeatureLevels,
        UINT SDKVersion)
    {
        if constexpr(!std::derived_from<DeviceTy, ID3D11Device> && !std::derived_from<DeviceContextTy, ID3D11DeviceContext>)
        {
            return CreateDevice<typename DeviceTy::value_type, typename DeviceContextTy::value_type>(
                optAdapter,
                DriverType,
                Software,
                Flags,
                pFeatureLevels,
                SDKVersion);
        }
        else if constexpr(!std::derived_from<DeviceTy, ID3D11Device>)
        {
            return CreateDevice<typename DeviceTy::value_type, DeviceContextTy>(
                optAdapter,
                DriverType,
                Software,
                Flags,
                pFeatureLevels,
                SDKVersion);
        }
        else if constexpr(!std::derived_from<DeviceContextTy, ID3D11DeviceContext>)
        {
            return CreateDevice<DeviceTy, typename DeviceContextTy::value_type>(
                optAdapter,
                DriverType,
                Software,
                Flags,
                pFeatureLevels,
                SDKVersion);
        }
        else
        {
            Microsoft::WRL::ComPtr<ID3D11Device> tempDevice;
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> tempDeviceContext;

            if(HRESULT result = D3D11CreateDevice(
                optAdapter,
                DriverType,
                Software,
                Flags,
                pFeatureLevels.data(),
                static_cast<UINT>(pFeatureLevels.size()),
                SDKVersion,
                &tempDevice,
                nullptr,
                &tempDeviceContext);
                FAILED(result))
                return tl::expected<std::pair<Wrapper<DeviceTy>, Wrapper<DeviceContextTy>>, HRESULT>(tl::unexpected(result));

            std::pair<Wrapper<DeviceTy>, Wrapper<DeviceContextTy>> device;
            device.first = Wrapper<DeviceTy>(Helpers::COM::Cast<DeviceTy>(tempDevice));
            device.second = Wrapper<DeviceContextTy>(Helpers::COM::Cast<DeviceContextTy>(tempDeviceContext));

            return tl::expected<std::pair<Wrapper<DeviceTy>, Wrapper<DeviceContextTy>>, HRESULT>(device);
        }
    }
}