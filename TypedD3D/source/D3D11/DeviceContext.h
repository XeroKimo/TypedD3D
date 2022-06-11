#pragma once
#include "DeviceChild.h"
#include "expected.hpp"
#include "span_tuple.h"
#include <d3d11_4.h>
#include <tuple>
#include <optional>
#include <array>
#include <vector>
#include <utility>

namespace TypedD3D::D3D11
{
    struct IAGetVertexBufferData
    {
        std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> buffers;
        UINT* strides;
        UINT* offsets;
    };

    struct IAGetIndexBufferData
    {
        Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
        DXGI_FORMAT format;
        UINT offset;
    };

    struct OMGetRenderTargetsAndUnorderedAccessViewsData
    {                    
        std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> renderTargetViews;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
        std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> unorderedAccessViews;
    };

    struct OMGetBlendStateData
    {
        Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
        std::array<FLOAT, 4> blendFactor;
        UINT sampleMask;
    };
}

namespace TypedD3D::Internal
{
    namespace D3D11
    {
        template<class Ty>
        using DeviceContext_t = InterfaceWrapper<Ty>;
        namespace DeviceContext
        {
            template<class WrapperTy, class DeviceContextTy>
            class Interface;

            template<class WrapperTy>
            class Interface<WrapperTy, ID3D11DeviceContext> : public DeviceChild::Interface<WrapperTy>
            {
            private:
                using device_context_ty = ID3D11DeviceContext;
                using wrapper_type = WrapperTy;
            public:
            public:
                void VSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<ID3D11Buffer*> ppConstantBuffers)
                {
                    InternalGet().VSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
                }

                void PSSetShaderResources(
                    UINT StartSlot,
                    std::span<ID3D11ShaderResourceView*> ppShaderResourceViews)
                {
                    InternalGet().PSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
                }

                void PSSetShader(
                    Wrapper<ID3D11PixelShader> optPixelShader,
                    std::span<ID3D11ClassInstance*> optClassInstancess)
                {
                    InternalGet().PSSetShader(optPixelShader.Get(), static_cast<UINT>(optClassInstancess.size()), optClassInstancess.data());
                }

                void PSSetSamplers(
                    UINT StartSlot,
                    std::span<ID3D11SamplerState*> ppSamplers)
                {
                    InternalGet().PSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
                }

                void VSSetShader(
                    Wrapper<ID3D11VertexShader> optVertexShader,
                    std::span<ID3D11ClassInstance*> optClassInstancess)
                {
                    InternalGet().VSSetShader(optVertexShader.Get(), static_cast<UINT>(optClassInstancess.size()), optClassInstancess.data());
                }

                void DrawIndexed(
                    UINT IndexCount,
                    UINT StartIndexLocation,
                    INT BaseVertexLocation)
                {
                    InternalGet().DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
                }

                void Draw(
                    UINT VertexCount,
                    UINT StartVertexLocation)
                {
                    InternalGet().Draw(VertexCount, StartVertexLocation);
                }

                tl::expected<D3D11_MAPPED_SUBRESOURCE, HRESULT> Map(
                    ID3D11Resource& pResource,
                    UINT Subresource,
                    D3D11_MAP MapType,
                    UINT MapFlags)
                {
                    D3D11_MAPPED_SUBRESOURCE subresource;

                    if(HRESULT result = InternalGet().Map(&pResource, Subresource, MapType, MapFlags, &subresource); FAILED(result))
                    {
                        return tl::unexpected(result);
                    }
                    return subresource;
                }

                void Unmap(
                    ID3D11Resource& pResource,
                    UINT Subresource)
                {
                    InternalGet().Unmap(&pResource, Subresource);
                }

                void PSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<ID3D11Buffer*> ppConstantBuffers)
                {
                    InternalGet().PSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
                }

                void IASetInputLayout(
                    ID3D11InputLayout* pInputLayout)
                {
                    InternalGet().IASetInputLayout(pInputLayout);
                }

                using Stride = UINT;
                using Offset = UINT;

                void IASetVertexBuffers(
                    UINT StartSlot,
                    xk::span_tuple<ID3D11Buffer*, std::dynamic_extent, const Stride, const Offset> vertexBuffers)
                {
                    InternalGet().IASetVertexBuffers(StartSlot, static_cast<UINT>(vertexBuffers.size()), vertexBuffers.data<0>(), vertexBuffers.data<1>(), vertexBuffers.data<2>());
                }

                void IASetIndexBuffer(
                    ID3D11Buffer* pIndexBuffer,
                    DXGI_FORMAT Format,
                    UINT Offset)
                {
                    InternalGet().IASetIndexBuffer(pIndexBuffer, Format, Offset);
                }

                void DrawIndexedInstanced(
                    UINT IndexCountPerInstance,
                    UINT InstanceCount,
                    UINT StartIndexLocation,
                    INT BaseVertexLocation,
                    UINT StartInstanceLocation)
                {
                    InternalGet().DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
                }

                void DrawInstanced(
                    UINT VertexCountPerInstance,
                    UINT InstanceCount,
                    UINT StartVertexLocation,
                    UINT StartInstanceLocation)
                {
                    InternalGet().DrawIndexedInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
                }

                void GSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<ID3D11Buffer*> ppConstantBuffers)
                {
                    InternalGet().GSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
                }

                void GSSetShader(
                    Wrapper<ID3D11GeometryShader> optGeometryShader,
                    std::span<ID3D11ClassInstance*> optClassInstancess)
                {
                    InternalGet().GSSetShader(optGeometryShader.Get(), static_cast<UINT>(optClassInstancess.size()), optClassInstancess.data());
                }

                void IASetPrimitiveTopology(
                    D3D11_PRIMITIVE_TOPOLOGY Topology)
                {
                    InternalGet().IASetPrimitiveTopology(Topology);
                }

                void VSSetShaderResources(
                    UINT StartSlot,
                    std::span<ID3D11ShaderResourceView*> ppShaderResourceViews)
                {
                    InternalGet().VSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
                }

                void VSSetSamplers(
                    UINT StartSlot,
                    std::span<ID3D11SamplerState*> ppSamplers)
                {
                    InternalGet().VSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
                }

                void Begin(
                    ID3D11Asynchronous& pAsync)
                {
                    InternalGet().Begin(&pAsync);
                }

                void End(
                    ID3D11Asynchronous& pAsync)
                {
                    InternalGet().End(&pAsync);
                }

                tl::expected<void*, HRESULT> GetData(
                    ID3D11Asynchronous& pAsync,
                    UINT GetDataFlags)
                {
                    void* data;

                    if(HRESULT result = InternalGet().GetData(&pAsync, data, pAsync.GetDataSize(), GetDataFlags); FAILED(result))
                        return tl::unexpected(result);

                    return data;
                }

                void SetPredication(
                    ID3D11Predicate* pPredicate,
                    BOOL PredicateValue)
                {
                    InternalGet().SetPredication(pPredicate, PredicateValue);
                }

                void GSSetShaderResources(
                    UINT StartSlot,
                    std::span<ID3D11ShaderResourceView*> ppShaderResourceViews)
                {
                    InternalGet().GSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
                }

                void GSSetSamplers(
                    UINT StartSlot,
                    std::span<ID3D11SamplerState*> ppSamplers)
                {
                    InternalGet().GSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
                }

                void OMSetRenderTargets(
                    std::span<ID3D11RenderTargetView*> ppRenderTargetViews,
                    ID3D11DepthStencilView* optDepthStencilView)
                {
                    InternalGet().OMSetRenderTargets(static_cast<UINT>(ppRenderTargetViews.size()), ppRenderTargetViews.data(), optDepthStencilView);
                }

                void OMSetRenderTargetsAndUnorderedAccessViews(
                    std::span<ID3D11RenderTargetView*> ppRenderTargetViews,
                    ID3D11DepthStencilView* optDepthStencilView,
                    UINT UAVStartSlot,
                    std::span<ID3D11UnorderedAccessView*> ppUnorderedAccessViews,
                    const UINT* pUAVInitialCounts)
                {
                    InternalGet().OMSetRenderTargetsAndUnorderedAccessViews(
                        static_cast<UINT>(ppRenderTargetViews.size()),
                        ppRenderTargetViews.data(),
                        optDepthStencilView,
                        UAVStartSlot,
                        static_cast<UINT>(ppUnorderedAccessViews.size()),
                        ppUnorderedAccessViews.data(),
                        pUAVInitialCounts);
                }

                void OMSetBlendState(
                    ID3D11BlendState* optBlendState,
                    std::optional<std::span<const FLOAT, 4>> BlendFactor,
                    UINT SampleMask)
                {
                    if(BlendFactor.has_value())
                        InternalGet().OMSetBlendState(optBlendState, BlendFactor.value().data(), SampleMask);
                    else
                        InternalGet().OMSetBlendState(optBlendState, nullptr, SampleMask);
                }

                void OMSetDepthStencilState(
                    ID3D11DepthStencilState* optDepthStencilState,
                    UINT StencilRef)
                {
                    InternalGet().OMSetDepthStencilState(optDepthStencilState, StencilRef);
                }

                void SOSetTargets(
                    xk::span_tuple<ID3D11Buffer*, std::dynamic_extent, const Offset> ppSOTargets)
                {
                    InternalGet().SOSetTargets(static_cast<UINT>(ppSOTargets.size()), ppSOTargets.data<0>(), ppSOTargets.data<1>());
                }

                void DrawAuto()
                {
                    InternalGet().DrawAuto();
                }

                void DrawIndexedInstancedIndirect(
                    ID3D11Buffer& pBufferForArgs,
                    UINT AlignedByteOffsetForArgs)
                {
                    InternalGet().DrawIndexedInstancedIndirect(&pBufferForArgs, AlignedByteOffsetForArgs);
                }

                void DrawInstancedIndirect(
                    ID3D11Buffer& pBufferForArgs,
                    UINT AlignedByteOffsetForArgs)
                {
                    InternalGet().DrawInstancedIndirect(&pBufferForArgs, AlignedByteOffsetForArgs);
                }

                void Dispatch(
                    UINT ThreadGroupCountX,
                    UINT ThreadGroupCountY,
                    UINT ThreadGroupCountZ)
                {
                    InternalGet().Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
                }

                void DispatchIndirect(
                    ID3D11Buffer& pBufferForArgs,
                    UINT AlignedByteOffsetForArgs)
                {
                    InternalGet().DispatchIndirect(&pBufferForArgs, AlignedByteOffsetForArgs);
                }

                void RSSetState(
                    ID3D11RasterizerState* optRasterizerState)
                {
                    InternalGet().RSSetState(optRasterizerState);
                }

                void RSSetViewports(
                    std::span<const D3D11_VIEWPORT> pViewports)
                {
                    InternalGet().RSSetViewports(static_cast<UINT>(pViewports.size()), pViewports.data());
                }

                void RSSetScissorRects(
                    std::span<const D3D11_RECT> pRects)
                {
                    InternalGet().RSSetScissorRects(static_cast<UINT>(pRects.size()), pRects.data());
                }

                void CopySubresourceRegion(
                    ID3D11Resource& pDstResource,
                    UINT DstSubresource,
                    UINT DstX,
                    UINT DstY,
                    UINT DstZ,
                    ID3D11Resource& pSrcResource,
                    UINT SrcSubresource,
                    const D3D11_BOX* pSrcBox)
                {
                    InternalGet().CopySubresourceRegion(
                        pDstResource,
                        DstSubresource,
                        DstX,
                        DstY,
                        DstZ,
                        pSrcResource,
                        SrcSubresource,
                        pSrcBox);
                }

                void CopyResource(
                    ID3D11Resource& pDstResource,
                    ID3D11Resource& pSrcResource)
                {
                    InternalGet().CopyResource(
                        pDstResource,
                        pSrcResource);
                }

                void UpdateSubresource(
                    ID3D11Resource& pDstResource,
                    UINT DstSubresource,
                    const D3D11_BOX* pDstBox,
                    const void* pSrcData,
                    UINT SrcRowPitch,
                    UINT SrcDepthPitch)
                {
                    InternalGet().UpdateSubresource(
                        pDstResource,
                        DstSubresource,
                        pDstBox,
                        pSrcData,
                        SrcRowPitch,
                        SrcDepthPitch);
                }

                void CopyStructureCount(
                    ID3D11Buffer& pDstBuffer,
                    UINT DstAlignedByteOffset,
                    ID3D11UnorderedAccessView& pSrcView)
                {
                    InternalGet().CopyStructureCount(pDstBuffer, DstAlignedByteOffset, pSrcView);
                }

                void ClearRenderTargetView(
                    ID3D11RenderTargetView& pRenderTargetView,
                    std::span<const FLOAT, 4> ColorRGBA)
                {
                    InternalGet().ClearRenderTargetView(&pRenderTargetView, ColorRGBA.data());
                }

                void ClearUnorderedAccessViewUint(
                    ID3D11UnorderedAccessView& pUnorderedAccessView,
                    std::span<const UINT, 4> Values)
                {
                    InternalGet().ClearUnorderedAccessViewUint(&pUnorderedAccessView, Values.data());
                }

                void ClearUnorderedAccessViewFloat(
                    ID3D11UnorderedAccessView& pUnorderedAccessView,
                    std::span<const FLOAT, 4> Values)
                {
                    InternalGet().ClearUnorderedAccessViewFloat(&pUnorderedAccessView, Values.data());
                }

                void ClearDepthStencilView(
                    ID3D11DepthStencilView& pDepthStencilView,
                    UINT ClearFlags,
                    FLOAT Depth,
                    UINT8 Stencil)
                {
                    InternalGet().ClearDepthStencilView(&pDepthStencilView, ClearFlags, Depth, Stencil);
                }

                void GenerateMips(
                    ID3D11ShaderResourceView& pShaderResourceView)
                {
                    InternalGet().GenerateMips(&pShaderResourceView);
                }

                void SetResourceMinLOD(
                    ID3D11Resource& pResource,
                    FLOAT MinLOD)
                {
                    InternalGet().SetResourceMinLOD(&pResource, MinLOD);
                }

                FLOAT GetResourceMinLOD(
                    ID3D11Resource& pResource)
                {
                    return InternalGet().GetResourceMinLOD(&pResource);
                }

                void ResolveSubresource(
                    ID3D11Resource& pDstResource,
                    UINT DstSubresource,
                    ID3D11Resource& pSrcResource,
                    UINT SrcSubresource,
                    DXGI_FORMAT Format)
                {
                    InternalGet().ResolveSubresource(&pDstResource, DstSubresource, &pSrcResource, SrcSubresource, Format);
                }

                void ExecuteCommandList(
                    ID3D11CommandList& pCommandList,
                    BOOL RestoreContextState)
                {
                    InternalGet().ExecuteCommandList(&pCommandList, RestoreContextState);
                }

                void HSSetShaderResources(
                    UINT StartSlot,
                    std::span<ID3D11ShaderResourceView*> ppShaderResourceViews)
                {
                    InternalGet().HSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
                }

                void HSSetShader(
                    Wrapper<ID3D11HullShader> optHullShader,
                    std::span<ID3D11ClassInstance*> optClassInstancess)
                {
                    InternalGet().HSSetShader(optHullShader.Get(), static_cast<UINT>(optClassInstancess.size()), optClassInstancess.data());
                }

                void HSSetSamplers(
                    UINT StartSlot,
                    std::span<ID3D11SamplerState*> ppSamplers)
                {
                    InternalGet().HSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
                }

                void HSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<ID3D11Buffer*> ppConstantBuffers)
                {
                    InternalGet().HSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
                }

                void DSSetShaderResources(
                    UINT StartSlot,
                    std::span<ID3D11ShaderResourceView*> ppShaderResourceViews)
                {
                    InternalGet().DSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
                }

                void DSSetShader(
                    Wrapper<ID3D11DomainShader> optDomainShader,
                    std::span<ID3D11ClassInstance*> optClassInstancess)
                {
                    InternalGet().DSSetShader(optDomainShader.Get(), static_cast<UINT>(optClassInstancess.size()), optClassInstancess.data());
                }

                void DSSetSamplers(
                    UINT StartSlot,
                    std::span<ID3D11SamplerState*> ppSamplers)
                {
                    InternalGet().DSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
                }

                void DSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<ID3D11Buffer*> ppConstantBuffers)
                {
                    InternalGet().DSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
                }

                void CSSetShaderResources(
                    UINT StartSlot,
                    std::span<ID3D11ShaderResourceView*> ppShaderResourceViews)
                {
                    InternalGet().CSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
                }

                void CSSetUnorderedAccessViews(
                    UINT StartSlot,
                    std::span<ID3D11UnorderedAccessView*> ppUnorderedAccessViews,
                    const UINT* pUAVInitialCounts)
                {
                    InternalGet().CSSetUnorderedAccessViews(StartSlot, static_cast<UINT>(ppUnorderedAccessViews.size()), ppUnorderedAccessViews.data(), pUAVInitialCounts);
                }

                void CSSetShader(
                    Wrapper<ID3D11ComputeShader> optComputeShader,
                    std::span<ID3D11ClassInstance*> optClassInstancess)
                {
                    InternalGet().CSSetShader(optComputeShader.Get(), static_cast<UINT>(optClassInstancess.size()), optClassInstancess.data());
                }

                void CSSetSamplers(
                    UINT StartSlot,
                    std::span<ID3D11SamplerState*> ppSamplers)
                {
                    InternalGet().CSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
                }

                void CSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<ID3D11Buffer*> ppConstantBuffers)
                {
                    InternalGet().CSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> VSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().VSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        buffers[i].Attach(tempBuffers[i]);
                    }

                    return buffers;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> PSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().PSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        views[i].Attach(tempViews[i]);
                    }

                    return views;
                }

                std::pair<Microsoft::WRL::ComPtr<ID3D11PixelShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> PSGetShader()
                {
                    std::pair<Microsoft::WRL::ComPtr<ID3D11PixelShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;

                    InternalGet().PSGetShader(&output.first, &instances, &numInstances);

                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> PSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().PSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        samplers[i].Attach(tempSamplers[i]);
                    }

                    return samplers;
                }

                std::pair<Microsoft::WRL::ComPtr<ID3D11VertexShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> VSGetShader()
                {
                    std::pair<Microsoft::WRL::ComPtr<ID3D11VertexShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;

                    InternalGet().VSGetShader(&output.first, &instances, &numInstances);
                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> PSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().PSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        buffers[i].Attach(tempBuffers[i]);
                    }

                    return buffers;
                }

                Microsoft::WRL::ComPtr<ID3D11InputLayout> IAGetInputLayout()
                {
                    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
                    InternalGet().IAGetInputLayout(&layout);
                    return layout;
                }

                TypedD3D::D3D11::IAGetVertexBufferData IAGetVertexBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    TypedD3D::D3D11::IAGetVertexBufferData output;
                    output.buffers.resize(NumBuffers);
                    InternalGet().IAGetVertexBuffers(StartSlot, NumBuffers, tempBuffers.get(), output.strides, output.offsets);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        output.buffers[i].Attach(tempBuffers[i]);
                    }

                    return output;
                }

                TypedD3D::D3D11::IAGetIndexBufferData IAGetIndexBuffer()
                {
                    TypedD3D::D3D11::IAGetIndexBufferData output;

                    InternalGet().IAGetIndexBuffer(&output.buffer, &output.format, &output.offset);
                    return output;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> GSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().GSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        buffers[i].Attach(tempBuffers[i]);
                    }

                    return buffers;
                }

                std::pair<Microsoft::WRL::ComPtr<ID3D11GeometryShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> GSGetShader()
                {
                    std::pair<Microsoft::WRL::ComPtr<ID3D11GeometryShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;

                    InternalGet().GSGetShader(&output.first, &instances, &numInstances);
                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                D3D11_PRIMITIVE_TOPOLOGY IAGetPrimitiveTopology()
                {
                    D3D11_PRIMITIVE_TOPOLOGY topology;
                    InternalGet().IAGetPrimitiveTopology(&topology);
                    return topology;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> VSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().VSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        views[i].Attach(tempViews[i]);
                    }

                    return views;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> VSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().VSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        samplers[i].Attach(tempSamplers[i]);
                    }

                    return samplers;
                }

                std::pair<Microsoft::WRL::ComPtr<ID3D11Predicate>, BOOL> GetPredication()
                {
                    std::pair<Microsoft::WRL::ComPtr<ID3D11Predicate>, BOOL> output;
                    InternalGet().GetPredication(&output.first, &output.second);
                    return output;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> GSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().GSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        views[i].Attach(tempViews[i]);
                    }

                    return views;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> GSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().GSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        samplers[i].Attach(tempSamplers[i]);
                    }

                    return samplers;
                }

                std::pair<std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>>, Microsoft::WRL::ComPtr<ID3D11DepthStencilView>>  OMGetRenderTargets(
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11RenderTargetView* []> tempViews = std::make_unique<ID3D11RenderTargetView* []>(NumViews);
                    std::pair<std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>>, Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> output;
                    InternalGet().OMGetRenderTargets(NumViews, tempViews.get(), &output.second);

                    for(UINT i = 0; i < NumViews; i++)
                    {
                        output.first.push_back();
                        output.first.back().Attach(tempViews[i]);
                    }

                    return output;
                }

                TypedD3D::D3D11::OMGetRenderTargetsAndUnorderedAccessViewsData OMGetRenderTargetsAndUnorderedAccessViews(
                    UINT NumRTVs,
                    UINT UAVStartSlot,
                    UINT NumUAVs)
                {
                    std::unique_ptr<ID3D11RenderTargetView*[]> tempRTVs = std::make_unique<ID3D11RenderTargetView*[]>(NumRTVs);
                    std::unique_ptr<ID3D11UnorderedAccessView*[]> tempUAVs = std::make_unique<ID3D11UnorderedAccessView*[]>(NumUAVs);

                    TypedD3D::D3D11::OMGetRenderTargetsAndUnorderedAccessViewsData data;

                    data.renderTargetViews.resize(NumRTVs);
                    data.unorderedAccessViews.resize(NumUAVs);

                    InternalGet().OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, tempRTVs.get(), &data.depthStencilView, UAVStartSlot, NumUAVs, tempUAVs.get());

                    for(UINT i = 0; i < NumRTVs; i++)
                    {
                        data.renderTargetViews[i].Attach(tempRTVs[i]);
                    }
                    for(UINT i = 0; i < NumUAVs; i++)
                    {
                        data.unorderedAccessViews[i].Attach(tempUAVs[i]);
                    }
                    return data;
                }

                TypedD3D::D3D11::OMGetBlendStateData OMGetBlendState()
                {
                    TypedD3D::D3D11::OMGetBlendStateData data;
                    InternalGet().OMGetBlendState(&data.blendState, data.blendFactor.data(), &data.sampleMask);
                    return data;
                }

                std::pair<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, UINT> OMGetDepthStencilState()
                {
                    std::pair<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, UINT> state;
                    InternalGet().OMGetDepthStencilState(&state.first, &state.second);
                    return state;

                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> SOGetTargets(UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().SOGetTargets(NumBuffers, tempBuffers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        buffers[i].Attach(tempBuffers[i]);
                    }

                    return buffers;
                }

                Microsoft::WRL::ComPtr<ID3D11RasterizerState> RSGetState()
                {
                    Microsoft::WRL::ComPtr<ID3D11RasterizerState> state;
                    InternalGet().RSGetState(&state);
                    return state;
                }

                std::vector<D3D11_VIEWPORT> RSGetViewports()
                {
                    D3D11_VIEWPORT* viewports;
                    UINT numViewports;
                    InternalGet().RSGetViewports(&numViewports, viewports);

                    std::vector<D3D11_VIEWPORT> output(numViewports);
                    for(UINT i = 0; i < numViewports; i++)
                    {
                        output[i] = viewports[i];
                    }

                    return output;
                }

                std::vector<D3D11_RECT> RSGetScissorRects()
                {
                    D3D11_RECT* rects;
                    UINT numRects;
                    InternalGet().RSGetScissorRects(&numRects, rects);

                    std::vector<D3D11_RECT> output(numRects);
                    for(UINT i = 0; i < numRects; i++)
                    {
                        output[i] = rects[i];
                    }

                    return output;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> HSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().HSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        views[i].Attach(tempViews[i]);
                    }

                    return views;
                }

                std::pair<Microsoft::WRL::ComPtr<ID3D11HullShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> HSGetShader()
                {
                    std::pair<Microsoft::WRL::ComPtr<ID3D11HullShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;

                    InternalGet().HSGetShader(&output.first, &instances, &numInstances);
                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> HSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().HSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        samplers[i].Attach(tempSamplers[i]);
                    }

                    return samplers;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> HSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().HSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        buffers[i].Attach(tempBuffers[i]);
                    }

                    return buffers;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> DSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().DSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        views[i].Attach(tempViews[i]);
                    }

                    return views;
                }

                std::pair<Microsoft::WRL::ComPtr<ID3D11DomainShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> DSGetShader()
                {
                    std::pair<Microsoft::WRL::ComPtr<ID3D11DomainShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;

                    InternalGet().DSGetShader(&output.first, &instances, &numInstances);
                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> DSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().DSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        samplers[i].Attach(tempSamplers[i]);
                    }

                    return samplers;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> DSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().DSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        buffers[i].Attach(tempBuffers[i]);
                    }

                    return buffers;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> CSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().CSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        views[i].Attach(tempViews[i]);
                    }

                    return views;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> CSGetUnorderedAccessViews(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11UnorderedAccessView* []> tempViews = std::make_unique<ID3D11UnorderedAccessView* []>(NumViews);
                    InternalGet().CSGetUnorderedAccessViews(0, NumViews, tempViews.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        views[i].Attach(tempViews[i]);
                    }

                    return views;
                }

                std::pair<Microsoft::WRL::ComPtr<ID3D11ComputeShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> CSGetShader()
                {
                    std::pair<Microsoft::WRL::ComPtr<ID3D11ComputeShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;

                    InternalGet().CSGetShader(&output.first, &instances, &numInstances);
                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> CSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().CSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        samplers[i].Attach(tempSamplers[i]);
                    }

                    return samplers;
                }

                std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> CSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().CSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        buffers[i].Attach(tempBuffers[i]);
                    }

                    return buffers;
                }

                void ClearState()
                {
                    return InternalGet().ClearState();
                }

                void Flush()
                {
                    return InternalGet().Flush();
                }

                D3D11_DEVICE_CONTEXT_TYPE GetType()
                {
                    return InternalGet().GetType();
                }

                UINT GetContextFlags()
                {
                    return InternalGet().GetContextFlags();
                }

                tl::expected<Microsoft::WRL::ComPtr<ID3D11CommandList>, HRESULT> FinishCommandList(BOOL RestoreDeferredContextState)
                {
                    Microsoft::WRL::ComPtr<ID3D11CommandList> commandList;

                    if(HRESULT result = InternalGet().FinishCommandList(RestoreDeferredContextState, &commandList); FAILED(result))
                        return tl::unexpected(result);

                    return commandList;
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                device_context_ty& InternalGet() { return *ToDerived().Get(); }
            };
        }
    }

    template<std::derived_from<ID3D11DeviceContext> DeviceContextTy>
    class InterfaceWrapper<DeviceContextTy> : public ComWrapper<DeviceContextTy>, private D3D11::DeviceContext::Interface<InterfaceWrapper<DeviceContextTy>, DeviceContextTy>
    {
    private:
        using Interface = D3D11::DeviceContext::Interface<InterfaceWrapper<DeviceContextTy>, DeviceContextTy>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = DeviceContextTy;

    public:
        using ComWrapper<DeviceContextTy>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}

namespace TypedD3D::D3D11
{
    template<std::derived_from<ID3D11DeviceContext> DeviceContextTy>
    using DeviceContext_t = Internal::D3D11::DeviceContext_t<DeviceContextTy>;

    using DeviceContext = DeviceContext_t<ID3D11DeviceContext>;
}