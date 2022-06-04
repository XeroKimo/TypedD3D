#pragma once
#include "../Wrappers.h"
#include "../Internal/ComWrapper.h"
#include "../Helpers/COMHelpers.h"
#include <d3d11_4.h>
#include <span>

namespace TypedD3D::Internal
{
    namespace D3D11
    {
        template<class Ty>
        using Device_t = InterfaceWrapper<Ty>;

        namespace Device
        {
            template<class WrapperTy, class Ty>
            class Interface;

            template<class WrapperTy>
            class Interface<WrapperTy, ID3D11Device>
            {
            private:
                using device_type = ID3D11Device;
                using wrapper_type = WrapperTy;
            public:
                tl::expected<Microsoft::WRL::ComPtr<ID3D11Buffer>, HRESULT> CreateBuffer(
                    const D3D11_BUFFER_DESC& pDesc,
                    const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11Buffer>(&device_type::CreateBuffer, InternalGet(), &pDesc, optInitialData);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11Texture1D>, HRESULT>  CreateTexture1D(
                    const D3D11_TEXTURE1D_DESC& pDesc,
                    const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11Texture1D>(&device_type::CreateTexture1D, InternalGet(), &pDesc, optInitialData);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11Texture2D>, HRESULT>  CreateTexture2D(
                    const D3D11_TEXTURE2D_DESC& pDesc,
                    const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11Texture2D>(&device_type::CreateTexture2D, InternalGet(), &pDesc, optInitialData);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11Texture3D>, HRESULT>  CreateTexture3D(
                    const D3D11_TEXTURE3D_DESC& pDesc,
                    const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11Texture1D>(&device_type::CreateTexture3D, InternalGet(), &pDesc, optInitialData);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, HRESULT> CreateShaderResourceView(
                    ID3D11Resource& pResource,
                    const D3D11_SHADER_RESOURCE_VIEW_DESC* optDesc = nullptr)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11ShaderResourceView>(&device_type::CreateShaderResourceView, InternalGet(), &pResource, optDesc);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>, HRESULT> CreateUnorderedAccessView(
                    ID3D11Resource& pResource,
                    const D3D11_UNORDERED_ACCESS_VIEW_DESC* optDesc = nullptr)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11UnorderedAccessView>(&device_type::CreateUnorderedAccessView, InternalGet(), &pResource, optDesc);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>, HRESULT> CreateRenderTargetView(
                    ID3D11Resource& pResource,
                    const D3D11_RENDER_TARGET_VIEW_DESC* optDesc = nullptr)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11RenderTargetView>(&device_type::CreateRenderTargetView, InternalGet(), &pResource, optDesc);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>, HRESULT> CreateDepthStencilView(
                    ID3D11Resource& pResource,
                    const D3D11_DEPTH_STENCIL_VIEW_DESC* optDesc = nullptr)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11DepthStencilView>(&device_type::CreateDepthStencilView, InternalGet(), &pResource, optDesc);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11InputLayout>, HRESULT> CreateInputLayout(
                    std::span<D3D11_INPUT_ELEMENT_DESC> inputElementDescs,
                    ID3DBlob& pShaderBytecodeWithInputSignature)
                {
                    return CreateInputLayout(inputElementDescs, pShaderBytecodeWithInputSignature.GetBufferPointer(), pShaderBytecodeWithInputSignature.GetBufferSize());
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11InputLayout>, HRESULT> CreateInputLayout(
                    std::span<D3D11_INPUT_ELEMENT_DESC> inputElementDescs,
                    const void* pShaderBytecodeWithInputSignature,
                    SIZE_T BytecodeLength)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11InputLayout>(&device_type::CreateInputLayout, InternalGet(), inputElementDescs.data(), inputElementDescs.size(), pShaderBytecodeWithInputSignature, BytecodeLength);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11VertexShader>, HRESULT> CreateVertexShader(
                    ID3DBlob& pShaderBytecode,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return CreateVertexShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11VertexShader>, HRESULT> CreateVertexShader(
                    const void* pShaderBytecode,
                    SIZE_T BytecodeLength,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11VertexShader>(&device_type::CreateVertexShader, InternalGet(), pShaderBytecode.data(), BytecodeLength.size(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11GeometryShader>, HRESULT> CreateGeometryShader(
                    ID3DBlob& pShaderBytecode,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return CreateVertexShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11GeometryShader>, HRESULT> CreateGeometryShader(
                    const void* pShaderBytecode,
                    SIZE_T BytecodeLength,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11GeometryShader>(&device_type::CreateGeometryShader, InternalGet(), pShaderBytecode.data(), BytecodeLength.size(), pClassLinkage);
                }

                virtual HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(
                    /* [annotation] */
                    _In_reads_(BytecodeLength)  const void* pShaderBytecode,
                    /* [annotation] */
                    _In_  SIZE_T BytecodeLength,
                    /* [annotation] */
                    _In_reads_opt_(NumEntries)  const D3D11_SO_DECLARATION_ENTRY* pSODeclaration,
                    /* [annotation] */
                    _In_range_(0, D3D11_SO_STREAM_COUNT* D3D11_SO_OUTPUT_COMPONENT_COUNT)  UINT NumEntries,
                    /* [annotation] */
                    _In_reads_opt_(NumStrides)  const UINT* pBufferStrides,
                    /* [annotation] */
                    _In_range_(0, D3D11_SO_BUFFER_SLOT_COUNT)  UINT NumStrides,
                    /* [annotation] */
                    _In_  UINT RasterizedStream,
                    /* [annotation] */
                    _In_opt_  ID3D11ClassLinkage* pClassLinkage,
                    /* [annotation] */
                    _COM_Outptr_opt_  ID3D11GeometryShader** ppGeometryShader) = 0;

                tl::expected<Microsoft::WRL::ComPtr<ID3D11PixelShader>, HRESULT> CreatePixelShader(
                    ID3DBlob& pShaderBytecode,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return CreatePixelShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11PixelShader>, HRESULT> CreatePixelShader(
                    const void* pShaderBytecode,
                    SIZE_T BytecodeLength,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11PixelShader>(&device_type::CreatePixelShader, InternalGet(), pShaderBytecode.data(), BytecodeLength.size(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11HullShader>, HRESULT> CreateHullShader(
                    ID3DBlob& pShaderBytecode,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return CreateHullShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11HullShader>, HRESULT> CreateHullShader(
                    const void* pShaderBytecode,
                    SIZE_T BytecodeLength,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11HullShader>(&device_type::CreateHullShader, InternalGet(), pShaderBytecode.data(), BytecodeLength.size(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11DomainShader>, HRESULT> CreateDomainShader(
                    ID3DBlob& pShaderBytecode,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return CreateDomainShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11DomainShader>, HRESULT> CreateDomainShader(
                    const void* pShaderBytecode,
                    SIZE_T BytecodeLength,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11DomainShader>(&device_type::CreateDomainShader, InternalGet(), pShaderBytecode.data(), BytecodeLength.size(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11ComputeShader>, HRESULT> CreateComputeShader(
                    ID3DBlob& pShaderBytecode,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return CreateComputeShader(pShaderBytecode.GetBufferPointer(), pShaderBytecode.GetBufferSize(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11ComputeShader>, HRESULT> CreateComputeShader(
                    const void* pShaderBytecode,
                    SIZE_T BytecodeLength,
                    ID3D11ClassLinkage* pClassLinkage)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11ComputeShader>(&device_type::CreateComputeShader, InternalGet(), pShaderBytecode.data(), BytecodeLength.size(), pClassLinkage);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11ClassLinkage>, HRESULT> CreateClassLinkage()
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11ClassLinkage>(&device_type::CreateClassLinkage, InternalGet());
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11BlendState>, HRESULT> CreateBlendState(
                    const D3D11_BLEND_DESC& pBlendStateDesc)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11BlendState>(&device_type::CreateBlendState, InternalGet(), &pBlendStateDesc);
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, HRESULT> CreateDepthStencilState(
                    const D3D11_DEPTH_STENCIL_DESC& pDepthStencilDesc)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<ID3D11DepthStencilState>(&device_type::CreateDepthStencilState, InternalGet(), &pDepthStencilDesc);
                }

                virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState(
                    /* [annotation] */
                    _In_  const D3D11_RASTERIZER_DESC* pRasterizerDesc,
                    /* [annotation] */
                    _COM_Outptr_opt_  ID3D11RasterizerState** ppRasterizerState) = 0;

                virtual HRESULT STDMETHODCALLTYPE CreateSamplerState(
                    /* [annotation] */
                    _In_  const D3D11_SAMPLER_DESC* pSamplerDesc,
                    /* [annotation] */
                    _COM_Outptr_opt_  ID3D11SamplerState** ppSamplerState) = 0;

                virtual HRESULT STDMETHODCALLTYPE CreateQuery(
                    /* [annotation] */
                    _In_  const D3D11_QUERY_DESC* pQueryDesc,
                    /* [annotation] */
                    _COM_Outptr_opt_  ID3D11Query** ppQuery) = 0;

                virtual HRESULT STDMETHODCALLTYPE CreatePredicate(
                    /* [annotation] */
                    _In_  const D3D11_QUERY_DESC* pPredicateDesc,
                    /* [annotation] */
                    _COM_Outptr_opt_  ID3D11Predicate** ppPredicate) = 0;

                virtual HRESULT STDMETHODCALLTYPE CreateCounter(
                    /* [annotation] */
                    _In_  const D3D11_COUNTER_DESC* pCounterDesc,
                    /* [annotation] */
                    _COM_Outptr_opt_  ID3D11Counter** ppCounter) = 0;

                virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext(
                    UINT ContextFlags,
                    /* [annotation] */
                    _COM_Outptr_opt_  ID3D11DeviceContext** ppDeferredContext) = 0;

                virtual HRESULT STDMETHODCALLTYPE OpenSharedResource(
                    /* [annotation] */
                    _In_  HANDLE hResource,
                    /* [annotation] */
                    _In_  REFIID ReturnedInterface,
                    /* [annotation] */
                    _COM_Outptr_opt_  void** ppResource) = 0;

                virtual HRESULT STDMETHODCALLTYPE CheckFormatSupport(
                    /* [annotation] */
                    _In_  DXGI_FORMAT Format,
                    /* [annotation] */
                    _Out_  UINT* pFormatSupport) = 0;

                virtual HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(
                    /* [annotation] */
                    _In_  DXGI_FORMAT Format,
                    /* [annotation] */
                    _In_  UINT SampleCount,
                    /* [annotation] */
                    _Out_  UINT* pNumQualityLevels) = 0;

                virtual void STDMETHODCALLTYPE CheckCounterInfo(
                    /* [annotation] */
                    _Out_  D3D11_COUNTER_INFO* pCounterInfo) = 0;

                virtual HRESULT STDMETHODCALLTYPE CheckCounter(
                    /* [annotation] */
                    _In_  const D3D11_COUNTER_DESC* pDesc,
                    /* [annotation] */
                    _Out_  D3D11_COUNTER_TYPE* pType,
                    /* [annotation] */
                    _Out_  UINT* pActiveCounters,
                    /* [annotation] */
                    _Out_writes_opt_(*pNameLength)  LPSTR szName,
                    /* [annotation] */
                    _Inout_opt_  UINT* pNameLength,
                    /* [annotation] */
                    _Out_writes_opt_(*pUnitsLength)  LPSTR szUnits,
                    /* [annotation] */
                    _Inout_opt_  UINT* pUnitsLength,
                    /* [annotation] */
                    _Out_writes_opt_(*pDescriptionLength)  LPSTR szDescription,
                    /* [annotation] */
                    _Inout_opt_  UINT* pDescriptionLength) = 0;

                virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(
                    D3D11_FEATURE Feature,
                    /* [annotation] */
                    _Out_writes_bytes_(FeatureSupportDataSize)  void* pFeatureSupportData,
                    UINT FeatureSupportDataSize) = 0;

                virtual HRESULT STDMETHODCALLTYPE GetPrivateData(
                    /* [annotation] */
                    _In_  REFGUID guid,
                    /* [annotation] */
                    _Inout_  UINT* pDataSize,
                    /* [annotation] */
                    _Out_writes_bytes_opt_(*pDataSize)  void* pData) = 0;

                virtual HRESULT STDMETHODCALLTYPE SetPrivateData(
                    /* [annotation] */
                    _In_  REFGUID guid,
                    /* [annotation] */
                    _In_  UINT DataSize,
                    /* [annotation] */
                    _In_reads_bytes_opt_(DataSize)  const void* pData) = 0;

                virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(
                    /* [annotation] */
                    _In_  REFGUID guid,
                    /* [annotation] */
                    _In_opt_  const IUnknown* pData) = 0;

                virtual D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel(void) = 0;

                virtual UINT STDMETHODCALLTYPE GetCreationFlags(void) = 0;

                virtual HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason(void) = 0;

                virtual void STDMETHODCALLTYPE GetImmediateContext(
                    /* [annotation] */
                    _Outptr_  ID3D11DeviceContext** ppImmediateContext) = 0;

                virtual HRESULT STDMETHODCALLTYPE SetExceptionMode(
                    UINT RaiseFlags) = 0;

                virtual UINT STDMETHODCALLTYPE GetExceptionMode(void) = 0;

            private:

                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                device_type& InternalGet() { return *ToDerived().Get(); }
            };
        }
    }

    template<std::derived_from<ID3D11Device> DeviceTy>
    class InterfaceWrapper<DeviceTy> : public ComWrapper<DeviceTy>, private D3D11::Device::Interface<InterfaceWrapper<DeviceTy>, DeviceTy>
    {
    private:
        using Interface = D3D11::Device::Interface<InterfaceWrapper<DeviceTy>, DeviceTy>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = DeviceTy;

    public:
        using ComWrapper<DeviceTy>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}