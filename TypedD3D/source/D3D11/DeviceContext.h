#pragma once
#include "DeviceChild.h"
#include "expected.hpp"
#include "span_tuple.h"
#include "gsl/pointers"
#include "ResourceViews.h"
#include "States.h"
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
        std::vector<Wrapper<ID3D11Buffer>> buffers;
        std::vector<UINT> strides;
        std::vector<UINT> offsets;
    };

    struct IAGetIndexBufferData
    {
        Wrapper<ID3D11Buffer> buffer;
        DXGI_FORMAT format;
        UINT offset;
    };

    struct OMGetRenderTargetsAndUnorderedAccessViewsData
    {
        std::vector<Wrapper<ID3D11RenderTargetView>> renderTargetViews;
        Wrapper<ID3D11DepthStencilView> depthStencilView;
        std::vector<Wrapper<ID3D11UnorderedAccessView>> unorderedAccessViews;
    };

    struct OMGetBlendStateData
    {
        Wrapper<ID3D11BlendState> blendState;
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
                void VSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> rawBuffers = std::make_unique<ID3D11Buffer* []>(ppConstantBuffers.size());
                    for(size_t i = 0; i < ppConstantBuffers.size(); i++)
                    {
                        rawBuffers[i] = ppConstantBuffers[i].Get();
                    }
                    InternalGet().VSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
                }

                void PSSetShaderResources(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> rawBuffers = std::make_unique<ID3D11ShaderResourceView* []>(ppShaderResourceViews.size());
                    for(size_t i = 0; i < ppShaderResourceViews.size(); i++)
                    {
                        rawBuffers[i] = ppShaderResourceViews[i].Get();
                    }
                    InternalGet().PSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
                }

                void PSSetShader(
                    Wrapper<ID3D11PixelShader> optPixelShader,
                    std::span<ID3D11ClassInstance*> optClassInstances)
                {
                    InternalGet().PSSetShader(optPixelShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
                }

                void PSSetSamplers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11SamplerState>> ppSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> rawSamplers = std::make_unique<ID3D11SamplerState* []>(ppSamplers.size());
                    for(size_t i = 0; i < ppSamplers.size(); i++)
                    {
                        rawSamplers[i] = ppSamplers[i].Get();
                    }
                    InternalGet().PSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
                }

                void VSSetShader(
                    Wrapper<ID3D11VertexShader> optVertexShader,
                    std::span<ID3D11ClassInstance*> optClassInstances)
                {
                    InternalGet().VSSetShader(optVertexShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
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
                    gsl::not_null<Wrapper<ID3D11Resource>> pResource,
                    UINT Subresource,
                    D3D11_MAP MapType,
                    UINT MapFlags)
                {
                    D3D11_MAPPED_SUBRESOURCE subresource;

                    if(HRESULT result = InternalGet().Map(pResource.get().Get(), Subresource, MapType, MapFlags, &subresource); FAILED(result))
                    {
                        return tl::unexpected(result);
                    }
                    return subresource;
                }

                void Unmap(
                    gsl::not_null<Wrapper<ID3D11Resource>> pResource,
                    UINT Subresource)
                {
                    InternalGet().Unmap(pResource.get().Get(), Subresource);
                }

                void PSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> rawBuffers = std::make_unique<ID3D11Buffer* []>(ppConstantBuffers.size());
                    for(size_t i = 0; i < ppConstantBuffers.size(); i++)
                    {
                        rawBuffers[i] = ppConstantBuffers[i].Get();
                    }
                    InternalGet().PSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
                }

                void IASetInputLayout(
                    Wrapper<ID3D11InputLayout> pInputLayout)
                {
                    InternalGet().IASetInputLayout(pInputLayout.Get());
                }

                using Stride = UINT;
                using Offset = UINT;

                void IASetVertexBuffers(
                    UINT StartSlot,
                    xk::span_tuple<Wrapper<ID3D11Buffer>, std::dynamic_extent, const Stride, const Offset> vertexBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> rawBuffers = std::make_unique<ID3D11Buffer* []>(vertexBuffers.size());
                    for(size_t i = 0; i < vertexBuffers.size(); i++)
                    {
                        rawBuffers[i] = get<0>(vertexBuffers)[i].Get();
                    }
                    InternalGet().IASetVertexBuffers(StartSlot, static_cast<UINT>(vertexBuffers.size()), rawBuffers.get(), vertexBuffers.data<1>(), vertexBuffers.data<2>());
                }

                void IASetIndexBuffer(
                    Wrapper<ID3D11Buffer> pIndexBuffer,
                    DXGI_FORMAT Format,
                    UINT Offset)
                {
                    InternalGet().IASetIndexBuffer(pIndexBuffer.Get(), Format, Offset);
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
                    std::span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> rawBuffers = std::make_unique<ID3D11Buffer* []>(ppConstantBuffers.size());
                    for(size_t i = 0; i < ppConstantBuffers.size(); i++)
                    {
                        rawBuffers[i] = ppConstantBuffers[i].Get();
                    }
                    InternalGet().GSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
                }

                void GSSetShader(
                    Wrapper<ID3D11GeometryShader> optGeometryShader,
                    std::span<ID3D11ClassInstance*> optClassInstances)
                {
                    InternalGet().GSSetShader(optGeometryShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
                }

                void IASetPrimitiveTopology(
                    D3D11_PRIMITIVE_TOPOLOGY Topology)
                {
                    InternalGet().IASetPrimitiveTopology(Topology);
                }

                void VSSetShaderResources(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> rawBuffers = std::make_unique<ID3D11ShaderResourceView* []>(ppShaderResourceViews.size());
                    for(size_t i = 0; i < ppShaderResourceViews.size(); i++)
                    {
                        rawBuffers[i] = ppShaderResourceViews[i].Get();
                    }
                    InternalGet().VSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
                }

                void VSSetSamplers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11SamplerState>> ppSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> rawSamplers = std::make_unique<ID3D11SamplerState* []>(ppSamplers.size());
                    for(size_t i = 0; i < ppSamplers.size(); i++)
                    {
                        rawSamplers[i] = ppSamplers[i].Get();
                    }
                    InternalGet().VSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
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
                    std::span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> rawBuffers = std::make_unique<ID3D11ShaderResourceView* []>(ppShaderResourceViews.size());
                    for(size_t i = 0; i < ppShaderResourceViews.size(); i++)
                    {
                        rawBuffers[i] = ppShaderResourceViews[i].Get();
                    }
                    InternalGet().GSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
                }

                void GSSetSamplers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11SamplerState>> ppSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> rawSamplers = std::make_unique<ID3D11SamplerState* []>(ppSamplers.size());
                    for(size_t i = 0; i < ppSamplers.size(); i++)
                    {
                        rawSamplers[i] = ppSamplers[i].Get();
                    }
                    InternalGet().GSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
                }

                void OMSetRenderTargets(
                    std::span<Wrapper<ID3D11RenderTargetView>> ppRenderTargetViews,
                    Wrapper<ID3D11DepthStencilView> optDepthStencilView)
                {
                    std::unique_ptr<ID3D11RenderTargetView* []> rawBuffers = std::make_unique<ID3D11RenderTargetView* []>(ppRenderTargetViews.size());
                    for(size_t i = 0; i < ppRenderTargetViews.size(); i++)
                    {
                        rawBuffers[i] = ppRenderTargetViews[i].Get();
                    }
                    InternalGet().OMSetRenderTargets(static_cast<UINT>(ppRenderTargetViews.size()), rawBuffers.get(), optDepthStencilView.Get());
                }

                void OMSetRenderTargetsAndUnorderedAccessViews(
                    std::span<Wrapper<ID3D11RenderTargetView>> ppRenderTargetViews,
                    Wrapper<ID3D11DepthStencilView> optDepthStencilView,
                    UINT UAVStartSlot,
                    std::span<ID3D11UnorderedAccessView*> ppUnorderedAccessViews,
                    const UINT* pUAVInitialCounts)
                {
                    std::unique_ptr<ID3D11RenderTargetView* []> rawBuffers = std::make_unique<ID3D11RenderTargetView* []>(ppRenderTargetViews.size());
                    for(size_t i = 0; i < ppRenderTargetViews.size(); i++)
                    {
                        rawBuffers[i] = ppRenderTargetViews[i].Get();
                    }

                    InternalGet().OMSetRenderTargetsAndUnorderedAccessViews(
                        static_cast<UINT>(ppRenderTargetViews.size()),
                        rawBuffers.get(),
                        optDepthStencilView.Get(),
                        UAVStartSlot,
                        static_cast<UINT>(ppUnorderedAccessViews.size()),
                        ppUnorderedAccessViews.data(),
                        pUAVInitialCounts);
                }

                void OMSetBlendState(
                    Wrapper<ID3D11BlendState> optBlendState,
                    std::optional<std::span<const FLOAT, 4>> BlendFactor,
                    UINT SampleMask)
                {
                    if(BlendFactor.has_value())
                        InternalGet().OMSetBlendState(optBlendState.Get(), BlendFactor.value().data(), SampleMask);
                    else
                        InternalGet().OMSetBlendState(optBlendState.Get(), nullptr, SampleMask);
                }

                void OMSetDepthStencilState(
                    Wrapper<ID3D11DepthStencilState> optDepthStencilState,
                    UINT StencilRef)
                {
                    InternalGet().OMSetDepthStencilState(optDepthStencilState.Get(), StencilRef);
                }

                void SOSetTargets(
                    xk::span_tuple<Wrapper<ID3D11Buffer>, std::dynamic_extent, const Offset> ppSOTargets)
                {
                    std::unique_ptr<ID3D11Buffer* []> rawBuffers = std::make_unique<ID3D11Buffer* []>(ppSOTargets.size());
                    for(size_t i = 0; i < ppSOTargets.size(); i++)
                    {
                        rawBuffers[i] = get<0>(ppSOTargets)[i].Get();
                    }
                    InternalGet().SOSetTargets(static_cast<UINT>(ppSOTargets.size()), rawBuffers.get(), ppSOTargets.data<1>());
                }

                void DrawAuto()
                {
                    InternalGet().DrawAuto();
                }

                void DrawIndexedInstancedIndirect(
                    gsl::not_null<Wrapper<ID3D11Buffer>> pBufferForArgs,
                    UINT AlignedByteOffsetForArgs)
                {
                    InternalGet().DrawIndexedInstancedIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
                }

                void DrawInstancedIndirect(
                    gsl::not_null<Wrapper<ID3D11Buffer>> pBufferForArgs,
                    UINT AlignedByteOffsetForArgs)
                {
                    InternalGet().DrawInstancedIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
                }

                void Dispatch(
                    UINT ThreadGroupCountX,
                    UINT ThreadGroupCountY,
                    UINT ThreadGroupCountZ)
                {
                    InternalGet().Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
                }

                void DispatchIndirect(
                    gsl::not_null<Wrapper<ID3D11Buffer>> pBufferForArgs,
                    UINT AlignedByteOffsetForArgs)
                {
                    InternalGet().DispatchIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
                }

                void RSSetState(
                    Wrapper<ID3D11RasterizerState> optRasterizerState)
                {
                    InternalGet().RSSetState(optRasterizerState.Get());
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
                    gsl::not_null<Wrapper<ID3D11Resource>> pDstResource,
                    UINT DstSubresource,
                    UINT DstX,
                    UINT DstY,
                    UINT DstZ,
                    gsl::not_null<Wrapper<ID3D11Resource>>  pSrcResource,
                    UINT SrcSubresource,
                    const D3D11_BOX* pSrcBox)
                {
                    InternalGet().CopySubresourceRegion(
                        pDstResource.get().Get(),
                        DstSubresource,
                        DstX,
                        DstY,
                        DstZ,
                        pSrcResource.get().Get(),
                        SrcSubresource,
                        pSrcBox);
                }

                void CopyResource(
                    gsl::not_null<Wrapper<ID3D11Resource>>  pDstResource,
                    gsl::not_null<Wrapper<ID3D11Resource>>  pSrcResource)
                {
                    InternalGet().CopyResource(
                        pDstResource.get().Get(),
                        pSrcResource.get().Get());
                }

                void UpdateSubresource(
                    gsl::not_null<Wrapper<ID3D11Resource>> pDstResource,
                    UINT DstSubresource,
                    const D3D11_BOX* pDstBox,
                    const void* pSrcData,
                    UINT SrcRowPitch,
                    UINT SrcDepthPitch)
                {
                    InternalGet().UpdateSubresource(
                        pDstResource.get().Get(),
                        DstSubresource,
                        pDstBox,
                        pSrcData,
                        SrcRowPitch,
                        SrcDepthPitch);
                }

                void CopyStructureCount(
                    gsl::not_null<Wrapper<ID3D11Buffer>> pDstBuffer,
                    UINT DstAlignedByteOffset,
                    gsl::not_null<Wrapper<ID3D11UnorderedAccessView>> pSrcView)
                {
                    InternalGet().CopyStructureCount(pDstBuffer.get().Get(), DstAlignedByteOffset, pSrcView.get().Get());
                }

                void ClearRenderTargetView(
                    gsl::not_null<Wrapper<ID3D11RenderTargetView>> pRenderTargetView,
                    std::span<const FLOAT, 4> ColorRGBA)
                {
                    InternalGet().ClearRenderTargetView(pRenderTargetView.get().Get(), ColorRGBA.data());
                }

                void ClearUnorderedAccessViewUint(
                    gsl::not_null<Wrapper<ID3D11UnorderedAccessView>> pUnorderedAccessView,
                    std::span<const UINT, 4> Values)
                {
                    InternalGet().ClearUnorderedAccessViewUint(pUnorderedAccessView.get().Get(), Values.data());
                }

                void ClearUnorderedAccessViewFloat(
                    gsl::not_null<Wrapper<ID3D11UnorderedAccessView>> pUnorderedAccessView,
                    std::span<const FLOAT, 4> Values)
                {
                    InternalGet().ClearUnorderedAccessViewFloat(pUnorderedAccessView.get().Get(), Values.data());
                }

                void ClearDepthStencilView(
                    gsl::not_null<Wrapper<ID3D11DepthStencilView>> pDepthStencilView,
                    UINT ClearFlags,
                    FLOAT Depth,
                    UINT8 Stencil)
                {
                    InternalGet().ClearDepthStencilView(pDepthStencilView.get().Get(), ClearFlags, Depth, Stencil);
                }

                void GenerateMips(
                    gsl::not_null<Wrapper<ID3D11ShaderResourceView>> pShaderResourceView)
                {
                    InternalGet().GenerateMips(pShaderResourceView.get().Get());
                }

                void SetResourceMinLOD(
                    gsl::not_null<Wrapper<ID3D11Resource>> pResource,
                    FLOAT MinLOD)
                {
                    InternalGet().SetResourceMinLOD(pResource.get().Get(), MinLOD);
                }

                FLOAT GetResourceMinLOD(
                    gsl::not_null<Wrapper<ID3D11Resource>> pResource)
                {
                    return InternalGet().GetResourceMinLOD(pResource.get().Get());
                }

                void ResolveSubresource(
                    gsl::not_null<Wrapper<ID3D11Resource>> pDstResource,
                    UINT DstSubresource,
                    gsl::not_null<Wrapper<ID3D11Resource>> pSrcResource,
                    UINT SrcSubresource,
                    DXGI_FORMAT Format)
                {
                    InternalGet().ResolveSubresource(pDstResource.get().Get(), DstSubresource, pSrcResource.get().Get(), SrcSubresource, Format);
                }

                void ExecuteCommandList(
                    ID3D11CommandList& pCommandList,
                    BOOL RestoreContextState)
                {
                    InternalGet().ExecuteCommandList(&pCommandList, RestoreContextState);
                }

                void HSSetShaderResources(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> rawBuffers = std::make_unique<ID3D11ShaderResourceView* []>(ppShaderResourceViews.size());
                    for(size_t i = 0; i < ppShaderResourceViews.size(); i++)
                    {
                        rawBuffers[i] = ppShaderResourceViews[i].Get();
                    }
                    InternalGet().HSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
                }

                void HSSetShader(
                    Wrapper<ID3D11HullShader> optHullShader,
                    std::span<ID3D11ClassInstance*> optClassInstances)
                {
                    InternalGet().HSSetShader(optHullShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
                }

                void HSSetSamplers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11SamplerState>> ppSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> rawSamplers = std::make_unique<ID3D11SamplerState* []>(ppSamplers.size());
                    for(size_t i = 0; i < ppSamplers.size(); i++)
                    {
                        rawSamplers[i] = ppSamplers[i].Get();
                    }
                    InternalGet().HSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
                }

                void HSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> rawBuffers = std::make_unique<ID3D11Buffer* []>(ppConstantBuffers.size());
                    for(size_t i = 0; i < ppConstantBuffers.size(); i++)
                    {
                        rawBuffers[i] = ppConstantBuffers[i].Get();
                    }
                    InternalGet().HSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
                }

                void DSSetShaderResources(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> rawBuffers = std::make_unique<ID3D11ShaderResourceView* []>(ppShaderResourceViews.size());
                    for(size_t i = 0; i < ppShaderResourceViews.size(); i++)
                    {
                        rawBuffers[i] = ppShaderResourceViews[i].Get();
                    }
                    InternalGet().DSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
                }

                void DSSetShader(
                    Wrapper<ID3D11DomainShader> optDomainShader,
                    std::span<ID3D11ClassInstance*> optClassInstances)
                {
                    InternalGet().DSSetShader(optDomainShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
                }

                void DSSetSamplers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11SamplerState>> ppSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> rawSamplers = std::make_unique<ID3D11SamplerState* []>(ppSamplers.size());
                    for(size_t i = 0; i < ppSamplers.size(); i++)
                    {
                        rawSamplers[i] = ppSamplers[i].Get();
                    }
                    InternalGet().DSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
                }

                void DSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> rawBuffers = std::make_unique<ID3D11Buffer* []>(ppConstantBuffers.size());
                    for(size_t i = 0; i < ppConstantBuffers.size(); i++)
                    {
                        rawBuffers[i] = ppConstantBuffers[i].Get();
                    }
                    InternalGet().DSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
                }

                void CSSetShaderResources(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> rawBuffers = std::make_unique<ID3D11ShaderResourceView* []>(ppShaderResourceViews.size());
                    for(size_t i = 0; i < ppShaderResourceViews.size(); i++)
                    {
                        rawBuffers[i] = ppShaderResourceViews[i].Get();
                    }
                    InternalGet().CSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
                }

                void CSSetUnorderedAccessViews(
                    UINT StartSlot,
                    xk::span_tuple<Wrapper<ID3D11UnorderedAccessView>, std::dynamic_extent, const UINT> ppUnorderedAccessViews)
                {
                    std::unique_ptr<ID3D11UnorderedAccessView* []> rawBuffers = std::make_unique<ID3D11UnorderedAccessView* []>(ppUnorderedAccessViews.size());
                    for(size_t i = 0; i < ppUnorderedAccessViews.size(); i++)
                    {
                        rawBuffers[i] = get<0>(ppUnorderedAccessViews[i]).Get();
                    }
                    InternalGet().CSSetUnorderedAccessViews(StartSlot, static_cast<UINT>(ppUnorderedAccessViews.size()), rawBuffers.get(), ppUnorderedAccessViews.data<1>());
                }

                void CSSetShader(
                    Wrapper<ID3D11ComputeShader> optComputeShader,
                    std::span<ID3D11ClassInstance*> optClassInstances)
                {
                    InternalGet().CSSetShader(optComputeShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
                }

                void CSSetSamplers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11SamplerState>> ppSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> rawSamplers = std::make_unique<ID3D11SamplerState* []>(ppSamplers.size());
                    for(size_t i = 0; i < ppSamplers.size(); i++)
                    {
                        rawSamplers[i] = ppSamplers[i].Get();
                    }
                    InternalGet().CSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
                }

                void CSSetConstantBuffers(
                    UINT StartSlot,
                    std::span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> rawBuffers = std::make_unique<ID3D11Buffer* []>(ppConstantBuffers.size());
                    for(size_t i = 0; i < ppConstantBuffers.size(); i++)
                    {
                        rawBuffers[i] = ppConstantBuffers[i].Get();
                    }
                    InternalGet().CSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
                }

                std::vector<Wrapper<ID3D11Buffer>> VSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().VSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
                        temp.Attach(tempBuffers[i]);
                        buffers[i] = std::move(temp);
                    }

                    return buffers;
                }

                std::vector<Wrapper<ID3D11ShaderResourceView>> PSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().PSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> temp;
                        temp.Attach(tempViews[i]);
                        views[i] = std::move(temp);
                    }

                    return views;
                }

                std::pair<Wrapper<ID3D11PixelShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> PSGetShader()
                {
                    std::pair<Wrapper<ID3D11PixelShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;
                    Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;

                    InternalGet().PSGetShader(&shader, &instances, &numInstances);
                    output.first = std::move(shader);

                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                std::vector<Wrapper<ID3D11SamplerState>> PSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().PSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11SamplerState> temp;
                        temp.Attach(tempSamplers[i]);
                        samplers[i] = std::move(temp);
                    }

                    return samplers;
                }

                std::pair<Wrapper<ID3D11VertexShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> VSGetShader()
                {
                    std::pair<Wrapper<ID3D11VertexShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;
                    Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;

                    InternalGet().VSGetShader(&shader, &instances, &numInstances);
                    output.first = std::move(shader);
                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                std::vector<Wrapper<ID3D11Buffer>> PSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().PSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
                        temp.Attach(tempBuffers[i]);
                        buffers[i] = std::move(temp);
                    }

                    return buffers;
                }

                Wrapper<ID3D11InputLayout> IAGetInputLayout()
                {
                    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
                    InternalGet().IAGetInputLayout(&layout);
                    return Wrapper<ID3D11InputLayout>(std::move(layout));
                }

                TypedD3D::D3D11::IAGetVertexBufferData IAGetVertexBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    std::unique_ptr<UINT[]> tempStrides = std::make_unique<UINT[]>(NumBuffers);
                    std::unique_ptr<UINT[]> tempOffsets = std::make_unique<UINT[]>(NumBuffers);
                    InternalGet().IAGetVertexBuffers(StartSlot, NumBuffers, tempBuffers.get(), tempStrides.get(), tempOffsets.get());

                    TypedD3D::D3D11::IAGetVertexBufferData output;
                    output.buffers.resize(NumBuffers);
                    output.strides.resize(NumBuffers);
                    output.offsets.resize(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
                        temp.Attach(tempBuffers[i]);
                        output.buffers[i] = std::move(temp);
                    }
                    std::copy(&tempStrides[0], &tempStrides[NumBuffers], output.strides.begin());
                    std::copy(&tempOffsets[0], &tempOffsets[NumBuffers], output.offsets.begin());

                    return output;
                }

                TypedD3D::D3D11::IAGetIndexBufferData IAGetIndexBuffer()
                {
                    TypedD3D::D3D11::IAGetIndexBufferData output;
                    Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
                    InternalGet().IAGetIndexBuffer(&temp, &output.format, &output.offset);
                    output.buffer = std::move(temp);
                    return output;
                }

                std::vector<Wrapper<ID3D11Buffer>> GSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().GSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
                        temp.Attach(tempBuffers[i]);
                        buffers[i] = std::move(temp);
                    }

                    return buffers;
                }

                std::pair<Wrapper<ID3D11GeometryShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> GSGetShader()
                {
                    std::pair<Wrapper<ID3D11GeometryShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;
                    Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader;

                    InternalGet().GSGetShader(&shader, &instances, &numInstances);
                    output.first = std::move(shader);
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

                std::vector<Wrapper<ID3D11ShaderResourceView>> VSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().VSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> temp;
                        temp.Attach(tempViews[i]);
                        views[i] = std::move(temp);
                    }

                    return views;
                }

                std::vector<Wrapper<ID3D11SamplerState>> VSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().VSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11SamplerState> temp;
                        temp.Attach(tempSamplers[i]);
                        samplers[i] = std::move(temp);
                    }

                    return samplers;
                }

                std::pair<Microsoft::WRL::ComPtr<ID3D11Predicate>, BOOL> GetPredication()
                {
                    std::pair<Microsoft::WRL::ComPtr<ID3D11Predicate>, BOOL> output;
                    InternalGet().GetPredication(&output.first, &output.second);
                    return output;
                }

                std::vector<Wrapper<ID3D11ShaderResourceView>> GSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().GSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> temp;
                        temp.Attach(tempViews[i]);
                        views[i] = std::move(temp);
                    }

                    return views;
                }
                
                std::vector<Wrapper<ID3D11SamplerState>> GSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().GSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11SamplerState> temp;
                        temp.Attach(tempSamplers[i]);
                        samplers[i] = std::move(temp);
                    }

                    return samplers;
                }

                std::pair<std::vector<Wrapper<ID3D11RenderTargetView>>, Wrapper<ID3D11DepthStencilView>>  OMGetRenderTargets(
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11RenderTargetView* []> tempViews = std::make_unique<ID3D11RenderTargetView* []>(NumViews);
                    std::pair<std::vector<Wrapper<ID3D11RenderTargetView>>, Wrapper<ID3D11DepthStencilView>> output;
                    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> tempDS;
                    InternalGet().OMGetRenderTargets(NumViews, tempViews.get(), &tempDS);

                    output.second = std::move(tempDS);

                    for(UINT i = 0; i < NumViews; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> tempRTV;
                        tempRTV.Attach(tempViews[i]);
                        output.first.push_back();
                        output.first.back() =std::move(tempRTV);
                    }

                    return output;
                }

                TypedD3D::D3D11::OMGetRenderTargetsAndUnorderedAccessViewsData OMGetRenderTargetsAndUnorderedAccessViews(
                    UINT NumRTVs,
                    UINT UAVStartSlot,
                    UINT NumUAVs)
                {
                    std::unique_ptr<ID3D11RenderTargetView* []> tempRTVs = std::make_unique<ID3D11RenderTargetView* []>(NumRTVs);
                    std::unique_ptr<ID3D11UnorderedAccessView* []> tempUAVs = std::make_unique<ID3D11UnorderedAccessView* []>(NumUAVs);

                    TypedD3D::D3D11::OMGetRenderTargetsAndUnorderedAccessViewsData data;

                    data.renderTargetViews.resize(NumRTVs);
                    data.unorderedAccessViews.resize(NumUAVs);

                    InternalGet().OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, tempRTVs.get(), &data.depthStencilView, UAVStartSlot, NumUAVs, tempUAVs.get());

                    for(UINT i = 0; i < NumRTVs; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv;
                        rtv.Attach(tempRTVs[i]);
                        data.renderTargetViews[i] = std::move(rtv);
                    }
                    for(UINT i = 0; i < NumUAVs; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> uav;
                        uav.Attach(tempUAVs[i]);
                        data.unorderedAccessViews[i] = std::move(uav);
                    }
                    return data;
                }

                TypedD3D::D3D11::OMGetBlendStateData OMGetBlendState()
                {
                    TypedD3D::D3D11::OMGetBlendStateData data;
                    Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
                    InternalGet().OMGetBlendState(&blendState, data.blendFactor.data(), &data.sampleMask);
                    data.blendState = std::move(blendState);
                    return data;
                }

                std::pair<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, UINT> OMGetDepthStencilState()
                {
                    std::pair<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, UINT> state;
                    InternalGet().OMGetDepthStencilState(&state.first, &state.second);
                    return state;

                }

                std::vector<Wrapper<ID3D11Buffer>> SOGetTargets(UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().SOGetTargets(NumBuffers, tempBuffers.get());

                    std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
                        temp.Attach(tempBuffers[i]);
                        buffers[i] = std::move(temp);
                    }

                    return buffers;
                }

                Wrapper<ID3D11RasterizerState> RSGetState()
                {
                    Microsoft::WRL::ComPtr<ID3D11RasterizerState> state;
                    InternalGet().RSGetState(&state);
                    return Wrapper<ID3D11RasterizerState>(std::move(state));
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

                std::vector<Wrapper<ID3D11ShaderResourceView>> HSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().HSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> temp;
                        temp.Attach(tempViews[i]);
                        views[i] = std::move(temp);
                    }

                    return views;
                }

                std::pair<Wrapper<ID3D11HullShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> HSGetShader()
                {
                    std::pair<Wrapper<ID3D11HullShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;
                    Microsoft::WRL::ComPtr<ID3D11HullShader> shader;

                    InternalGet().HSGetShader(&shader, &instances, &numInstances);
                    output.first = std::move(shader);
                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                std::vector<Wrapper<ID3D11SamplerState>> HSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().HSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11SamplerState> temp;
                        temp.Attach(tempSamplers[i]);
                        samplers[i] = std::move(temp);
                    }

                    return samplers;
                }

                std::vector<Wrapper<ID3D11Buffer>> HSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().HSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
                        temp.Attach(tempBuffers[i]);
                        buffers[i] = std::move(temp);
                    }

                    return buffers;
                }

                std::vector<Wrapper<ID3D11ShaderResourceView>> DSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().DSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> temp;
                        temp.Attach(tempViews[i]);
                        views[i] = std::move(temp);
                    }

                    return views;
                }

                std::pair<Wrapper<ID3D11DomainShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> DSGetShader()
                {
                    std::pair<Wrapper<ID3D11DomainShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;
                    Microsoft::WRL::ComPtr<ID3D11DomainShader> shader;

                    InternalGet().DSGetShader(&shader, &instances, &numInstances);
                    output.first = std::move(shader);
                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                std::vector<Wrapper<ID3D11SamplerState>> DSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().DSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11SamplerState> temp;
                        temp.Attach(tempSamplers[i]);
                        samplers[i] = std::move(temp);
                    }

                    return samplers;
                }

                std::vector<Wrapper<ID3D11Buffer>> DSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().DSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
                        temp.Attach(tempBuffers[i]);
                        buffers[i] = std::move(temp);
                    }

                    return buffers;
                }

                std::vector<Wrapper<ID3D11ShaderResourceView>> CSGetShaderResources(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView* []>(NumViews);
                    InternalGet().CSGetShaderResources(0, NumViews, tempViews.get());

                    std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> temp;
                        temp.Attach(tempViews[i]);
                        views[i] = std::move(temp);
                    }

                    return views;
                }

                std::vector<Wrapper<ID3D11UnorderedAccessView>> CSGetUnorderedAccessViews(
                    UINT StartSlot,
                    UINT NumViews)
                {
                    std::unique_ptr<ID3D11UnorderedAccessView* []> tempViews = std::make_unique<ID3D11UnorderedAccessView* []>(NumViews);
                    InternalGet().CSGetUnorderedAccessViews(0, NumViews, tempViews.get());

                    std::vector<Wrapper<ID3D11UnorderedAccessView>> views(NumViews);
                    for(UINT i = 0; i < NumViews; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> temp;
                        temp.Attach(tempViews[i]);
                        views[i] = std::move(temp);
                    }

                    return views;
                }

                std::pair<Wrapper<ID3D11ComputeShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> CSGetShader()
                {
                    std::pair<Wrapper<ID3D11ComputeShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                    ID3D11ClassInstance* instances;
                    UINT numInstances;
                    Microsoft::WRL::ComPtr<ID3D11ComputeShader> shader;

                    InternalGet().CSGetShader(&shader, &instances, &numInstances);
                    output.first = std::move(shader);
                    for(UINT i = 0; i < numInstances; i++)
                    {
                        output.second.push_back();
                        output.second.back().Attach(instances[i]);
                    }

                    return output;
                }

                std::vector<Wrapper<ID3D11SamplerState>> CSGetSamplers(
                    UINT StartSlot,
                    UINT NumSamplers)
                {
                    std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState* []>(NumSamplers);
                    InternalGet().CSGetSamplers(0, NumSamplers, tempSamplers.get());

                    std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers);
                    for(UINT i = 0; i < NumSamplers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11SamplerState> temp;
                        temp.Attach(tempSamplers[i]);
                        samplers[i] = std::move(temp);
                    }

                    return samplers;
                }

                std::vector<Wrapper<ID3D11Buffer>> CSGetConstantBuffers(
                    UINT StartSlot,
                    UINT NumBuffers)
                {
                    std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                    InternalGet().CSGetConstantBuffers(0, NumBuffers, tempBuffers.get());

                    std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                    for(UINT i = 0; i < NumBuffers; i++)
                    {
                        Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
                        temp.Attach(tempBuffers[i]);
                        buffers[i] = std::move(temp);
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