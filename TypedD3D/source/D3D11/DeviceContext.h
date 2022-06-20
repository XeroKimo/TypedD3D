#pragma once
#include "DeviceChild.h"
#include "expected.hpp"
#include "span_tuple.h"
#include "gsl/pointers"
#include "ResourceViews.h"
#include "States.h"
#include "Constants.h"
#include "source/Types.h"
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
        using DeviceContext_t = IUnknownWrapper<Ty>;

        namespace DeviceContext
        {
            template<class WrapperTy, class DeviceContextTy>
            class Interface;

            template<class Ty>
            struct Traits;

            template<>
            struct Traits<ID3D11DeviceContext>
            {
                using value_type = ID3D11DeviceContext;
                using pointer = ID3D11DeviceContext*;
                using const_pointer = const ID3D11DeviceContext*;
                using reference = ID3D11DeviceContext&;
                using const_reference = const ID3D11DeviceContext&;

                template<class DerivedSelf>
                class Interface : public DeviceChild::Interface<DerivedSelf>
                {
                private:
                    using derived_self = DerivedSelf;

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
                        Get().VSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
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
                        Get().PSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
                    }

                    void PSSetShader(
                        Wrapper<ID3D11PixelShader> optPixelShader,
                        std::span<ID3D11ClassInstance*> optClassInstances)
                    {
                        Get().PSSetShader(optPixelShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
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
                        Get().PSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
                    }

                    void VSSetShader(
                        Wrapper<ID3D11VertexShader> optVertexShader,
                        std::span<ID3D11ClassInstance*> optClassInstances)
                    {
                        Get().VSSetShader(optVertexShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
                    }

                    void DrawIndexed(
                        UINT IndexCount,
                        UINT StartIndexLocation,
                        INT BaseVertexLocation)
                    {
                        Get().DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
                    }

                    void Draw(
                        UINT VertexCount,
                        UINT StartVertexLocation)
                    {
                        Get().Draw(VertexCount, StartVertexLocation);
                    }

                    tl::expected<D3D11_MAPPED_SUBRESOURCE, HRESULT> Map(
                        gsl::not_null<Wrapper<ID3D11Resource>> pResource,
                        UINT Subresource,
                        D3D11_MAP MapType,
                        UINT MapFlags)
                    {
                        D3D11_MAPPED_SUBRESOURCE subresource;

                        if(HRESULT result = Get().Map(pResource.get().Get(), Subresource, MapType, MapFlags, &subresource); FAILED(result))
                        {
                            return tl::unexpected(result);
                        }
                        return subresource;
                    }

                    void Unmap(
                        gsl::not_null<Wrapper<ID3D11Resource>> pResource,
                        UINT Subresource)
                    {
                        Get().Unmap(pResource.get().Get(), Subresource);
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
                        Get().PSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
                    }

                    void IASetInputLayout(
                        Wrapper<ID3D11InputLayout> pInputLayout)
                    {
                        Get().IASetInputLayout(pInputLayout.Get());
                    }

                    void IASetVertexBuffers(
                        UINT StartSlot,
                        xk::span_tuple<Wrapper<ID3D11Buffer>, std::dynamic_extent, const Stride, const Offset> vertexBuffers)
                    {
                        std::unique_ptr<ID3D11Buffer* []> rawBuffers = std::make_unique<ID3D11Buffer* []>(vertexBuffers.size());
                        for(size_t i = 0; i < vertexBuffers.size(); i++)
                        {
                            rawBuffers[i] = get<0>(vertexBuffers)[i].Get();
                        }

                        std::unique_ptr<std::underlying_type_t<Stride>[]> strides = std::make_unique< std::underlying_type_t<Stride>[]>(vertexBuffers.size());
                        std::memcpy(strides.get(), vertexBuffers.data<1>(), vertexBuffers.size_bytes<1>());

                        std::unique_ptr<std::underlying_type_t<Offset>[]> offsets = std::make_unique< std::underlying_type_t<Offset>[]>(vertexBuffers.size());
                        std::memcpy(offsets.get(), vertexBuffers.data<2>(), vertexBuffers.size_bytes<2>());
                        Get().IASetVertexBuffers(
                            StartSlot, 
                            static_cast<UINT>(vertexBuffers.size()), 
                            rawBuffers.get(),
                            strides.get(),
                            offsets.get());
                    }

                    void IASetIndexBuffer(
                        Wrapper<ID3D11Buffer> pIndexBuffer,
                        DXGI_FORMAT Format,
                        UINT Offset)
                    {
                        Get().IASetIndexBuffer(pIndexBuffer.Get(), Format, Offset);
                    }

                    void DrawIndexedInstanced(
                        UINT IndexCountPerInstance,
                        UINT InstanceCount,
                        UINT StartIndexLocation,
                        INT BaseVertexLocation,
                        UINT StartInstanceLocation)
                    {
                        Get().DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
                    }

                    void DrawInstanced(
                        UINT VertexCountPerInstance,
                        UINT InstanceCount,
                        UINT StartVertexLocation,
                        UINT StartInstanceLocation)
                    {
                        Get().DrawIndexedInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
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
                        Get().GSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
                    }

                    void GSSetShader(
                        Wrapper<ID3D11GeometryShader> optGeometryShader,
                        std::span<ID3D11ClassInstance*> optClassInstances)
                    {
                        Get().GSSetShader(optGeometryShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
                    }

                    void IASetPrimitiveTopology(
                        D3D11_PRIMITIVE_TOPOLOGY Topology)
                    {
                        Get().IASetPrimitiveTopology(Topology);
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
                        Get().VSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
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
                        Get().VSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
                    }

                    void Begin(
                        ID3D11Asynchronous& pAsync)
                    {
                        Get().Begin(&pAsync);
                    }

                    void End(
                        ID3D11Asynchronous& pAsync)
                    {
                        Get().End(&pAsync);
                    }

                    tl::expected<void*, HRESULT> GetData(
                        ID3D11Asynchronous& pAsync,
                        UINT GetDataFlags)
                    {
                        void* data;

                        if(HRESULT result = Get().GetData(&pAsync, data, pAsync.GetDataSize(), GetDataFlags); FAILED(result))
                            return tl::unexpected(result);

                        return data;
                    }

                    void SetPredication(
                        ID3D11Predicate* pPredicate,
                        BOOL PredicateValue)
                    {
                        Get().SetPredication(pPredicate, PredicateValue);
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
                        Get().GSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
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
                        Get().GSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
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
                        Get().OMSetRenderTargets(static_cast<UINT>(ppRenderTargetViews.size()), rawBuffers.get(), optDepthStencilView.Get());
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

                        Get().OMSetRenderTargetsAndUnorderedAccessViews(
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
                            Get().OMSetBlendState(optBlendState.Get(), BlendFactor.value().data(), SampleMask);
                        else
                            Get().OMSetBlendState(optBlendState.Get(), nullptr, SampleMask);
                    }

                    void OMSetDepthStencilState(
                        Wrapper<ID3D11DepthStencilState> optDepthStencilState,
                        UINT StencilRef)
                    {
                        Get().OMSetDepthStencilState(optDepthStencilState.Get(), StencilRef);
                    }

                    void SOSetTargets(
                        xk::span_tuple<Wrapper<ID3D11Buffer>, std::dynamic_extent, const Offset> ppSOTargets)
                    {
                        std::unique_ptr<ID3D11Buffer* []> rawBuffers = std::make_unique<ID3D11Buffer* []>(ppSOTargets.size());
                        for(size_t i = 0; i < ppSOTargets.size(); i++)
                        {
                            rawBuffers[i] = get<0>(ppSOTargets)[i].Get();
                        }
                        Get().SOSetTargets(static_cast<UINT>(ppSOTargets.size()), rawBuffers.get(), ppSOTargets.data<1>());
                    }

                    void DrawAuto()
                    {
                        Get().DrawAuto();
                    }

                    void DrawIndexedInstancedIndirect(
                        gsl::not_null<Wrapper<ID3D11Buffer>> pBufferForArgs,
                        UINT AlignedByteOffsetForArgs)
                    {
                        Get().DrawIndexedInstancedIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
                    }

                    void DrawInstancedIndirect(
                        gsl::not_null<Wrapper<ID3D11Buffer>> pBufferForArgs,
                        UINT AlignedByteOffsetForArgs)
                    {
                        Get().DrawInstancedIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
                    }

                    void Dispatch(
                        UINT ThreadGroupCountX,
                        UINT ThreadGroupCountY,
                        UINT ThreadGroupCountZ)
                    {
                        Get().Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
                    }

                    void DispatchIndirect(
                        gsl::not_null<Wrapper<ID3D11Buffer>> pBufferForArgs,
                        UINT AlignedByteOffsetForArgs)
                    {
                        Get().DispatchIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
                    }

                    void RSSetState(
                        Wrapper<ID3D11RasterizerState> optRasterizerState)
                    {
                        Get().RSSetState(optRasterizerState.Get());
                    }

                    void RSSetViewports(
                        std::span<const D3D11_VIEWPORT> pViewports)
                    {
                        Get().RSSetViewports(static_cast<UINT>(pViewports.size()), pViewports.data());
                    }

                    void RSSetScissorRects(
                        std::span<const D3D11_RECT> pRects)
                    {
                        Get().RSSetScissorRects(static_cast<UINT>(pRects.size()), pRects.data());
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
                        Get().CopySubresourceRegion(
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
                        Get().CopyResource(
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
                        Get().UpdateSubresource(
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
                        Get().CopyStructureCount(pDstBuffer.get().Get(), DstAlignedByteOffset, pSrcView.get().Get());
                    }

                    void ClearRenderTargetView(
                        gsl::not_null<Wrapper<ID3D11RenderTargetView>> pRenderTargetView,
                        std::span<const FLOAT, 4> ColorRGBA)
                    {
                        Get().ClearRenderTargetView(pRenderTargetView.get().Get(), ColorRGBA.data());
                    }

                    void ClearUnorderedAccessViewUint(
                        gsl::not_null<Wrapper<ID3D11UnorderedAccessView>> pUnorderedAccessView,
                        std::span<const UINT, 4> Values)
                    {
                        Get().ClearUnorderedAccessViewUint(pUnorderedAccessView.get().Get(), Values.data());
                    }

                    void ClearUnorderedAccessViewFloat(
                        gsl::not_null<Wrapper<ID3D11UnorderedAccessView>> pUnorderedAccessView,
                        std::span<const FLOAT, 4> Values)
                    {
                        Get().ClearUnorderedAccessViewFloat(pUnorderedAccessView.get().Get(), Values.data());
                    }

                    void ClearDepthStencilView(
                        gsl::not_null<Wrapper<ID3D11DepthStencilView>> pDepthStencilView,
                        UINT ClearFlags,
                        FLOAT Depth,
                        UINT8 Stencil)
                    {
                        Get().ClearDepthStencilView(pDepthStencilView.get().Get(), ClearFlags, Depth, Stencil);
                    }

                    void GenerateMips(
                        gsl::not_null<Wrapper<ID3D11ShaderResourceView>> pShaderResourceView)
                    {
                        Get().GenerateMips(pShaderResourceView.get().Get());
                    }

                    void SetResourceMinLOD(
                        gsl::not_null<Wrapper<ID3D11Resource>> pResource,
                        FLOAT MinLOD)
                    {
                        Get().SetResourceMinLOD(pResource.get().Get(), MinLOD);
                    }

                    FLOAT GetResourceMinLOD(
                        gsl::not_null<Wrapper<ID3D11Resource>> pResource)
                    {
                        return Get().GetResourceMinLOD(pResource.get().Get());
                    }

                    void ResolveSubresource(
                        gsl::not_null<Wrapper<ID3D11Resource>> pDstResource,
                        UINT DstSubresource,
                        gsl::not_null<Wrapper<ID3D11Resource>> pSrcResource,
                        UINT SrcSubresource,
                        DXGI_FORMAT Format)
                    {
                        Get().ResolveSubresource(pDstResource.get().Get(), DstSubresource, pSrcResource.get().Get(), SrcSubresource, Format);
                    }

                    void ExecuteCommandList(
                        ID3D11CommandList& pCommandList,
                        BOOL RestoreContextState)
                    {
                        Get().ExecuteCommandList(&pCommandList, RestoreContextState);
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
                        Get().HSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
                    }

                    void HSSetShader(
                        Wrapper<ID3D11HullShader> optHullShader,
                        std::span<ID3D11ClassInstance*> optClassInstances)
                    {
                        Get().HSSetShader(optHullShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
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
                        Get().HSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
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
                        Get().HSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
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
                        Get().DSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
                    }

                    void DSSetShader(
                        Wrapper<ID3D11DomainShader> optDomainShader,
                        std::span<ID3D11ClassInstance*> optClassInstances)
                    {
                        Get().DSSetShader(optDomainShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
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
                        Get().DSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
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
                        Get().DSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
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
                        Get().CSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), rawBuffers.get());
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
                        Get().CSSetUnorderedAccessViews(StartSlot, static_cast<UINT>(ppUnorderedAccessViews.size()), rawBuffers.get(), ppUnorderedAccessViews.data<1>());
                    }

                    void CSSetShader(
                        Wrapper<ID3D11ComputeShader> optComputeShader,
                        std::span<ID3D11ClassInstance*> optClassInstances)
                    {
                        Get().CSSetShader(optComputeShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
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
                        Get().CSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), rawSamplers.get());
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
                        Get().CSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), rawBuffers.get());
                    }

                    std::vector<Wrapper<ID3D11Buffer>> VSGetConstantBuffers(
                        UINT StartSlot,
                        UINT NumBuffers)
                    {
                        std::unique_ptr<ID3D11Buffer*[]> tempBuffers = std::make_unique<ID3D11Buffer*[]>(NumBuffers);
                        Get().VSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

                        std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                        for(UINT i = 0; i < NumBuffers; i++)
                        {
                            buffers[i].Attach(tempBuffers[i]);
                        }

                        return buffers;
                    }

                    std::vector<Wrapper<ID3D11ShaderResourceView>> PSGetShaderResources(
                        UINT StartSlot,
                        UINT NumViews)
                    {
                        std::unique_ptr<ID3D11ShaderResourceView*[]> tempViews = std::make_unique<ID3D11ShaderResourceView*[]>(NumViews);
                        Get().PSGetShaderResources(StartSlot, NumViews, tempViews.get());

                        std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                        for(UINT i = 0; i < NumViews; i++)
                        {
                            views[i].Attach(tempViews[i]);
                        }

                        return views;
                    }

                    std::pair<Wrapper<ID3D11PixelShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> PSGetShader(UINT numInstances = 0)
                    {
                        std::pair<Wrapper<ID3D11PixelShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                        std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
                        Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;

                        Get().PSGetShader(&shader, tempInstances.data(), &numInstances);
                        output.first.Attach(shader.Detach());
                        output.second.resize(numInstances);
                        for(UINT i = 0; i < numInstances; i++)
                        {
                            output.second[i].Attach(tempInstances[i]);
                        }

                        return output;
                    }

                    std::vector<Wrapper<ID3D11SamplerState>> PSGetSamplers(
                        UINT StartSlot,
                        UINT NumSamplers)
                    {
                        std::unique_ptr<ID3D11SamplerState*[]> tempSamplers = std::make_unique<ID3D11SamplerState*[]>(NumSamplers - StartSlot);
                        Get().PSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

                        std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
                        for(UINT i = 0; i < NumSamplers; i++)
                        {
                            samplers[i].Attach(tempSamplers[i]);
                        }

                        return samplers;
                    }

                    std::pair<Wrapper<ID3D11VertexShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> VSGetShader(UINT numInstances = 0)
                    {
                        std::pair<Wrapper<ID3D11VertexShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                        std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
                        Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;

                        Get().VSGetShader(&shader, tempInstances.data(), &numInstances);
                        output.first.Attach(shader.Detach());
                        output.second.resize(numInstances);
                        for(UINT i = 0; i < numInstances; i++)
                        {
                            output.second[i].Attach(tempInstances[i]);
                        }

                        return output;
                    }

                    std::vector<Wrapper<ID3D11Buffer>> PSGetConstantBuffers(
                        UINT StartSlot,
                        UINT NumBuffers)
                    {
                        std::unique_ptr<ID3D11Buffer*[]> tempBuffers = std::make_unique<ID3D11Buffer*[]>(NumBuffers);
                        Get().PSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

                        std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                        for(UINT i = 0; i < NumBuffers; i++)
                        {
                            buffers[i].Attach(tempBuffers[i]);
                        }

                        return buffers;
                    }

                    Wrapper<ID3D11InputLayout> IAGetInputLayout()
                    {
                        Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
                        Get().IAGetInputLayout(&layout);
                        return Wrapper<ID3D11InputLayout>(std::move(layout));
                    }

                    TypedD3D::D3D11::IAGetVertexBufferData IAGetVertexBuffers(
                        UINT StartSlot,
                        UINT NumBuffers)
                    {
                        std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                        std::unique_ptr<UINT[]> tempStrides = std::make_unique<UINT[]>(NumBuffers);
                        std::unique_ptr<UINT[]> tempOffsets = std::make_unique<UINT[]>(NumBuffers);
                        Get().IAGetVertexBuffers(StartSlot, NumBuffers, tempBuffers.get(), tempStrides.get(), tempOffsets.get());

                        TypedD3D::D3D11::IAGetVertexBufferData output;
                        output.buffers.resize(NumBuffers);
                        output.strides.resize(NumBuffers);
                        output.offsets.resize(NumBuffers);
                        for(UINT i = 0; i < NumBuffers; i++)
                        {
                            output.buffers[i].Attach(tempBuffers[i]);
                        }
                        std::copy(&tempStrides[0], &tempStrides[NumBuffers], output.strides.begin());
                        std::copy(&tempOffsets[0], &tempOffsets[NumBuffers], output.offsets.begin());

                        return output;
                    }

                    TypedD3D::D3D11::IAGetIndexBufferData IAGetIndexBuffer()
                    {
                        TypedD3D::D3D11::IAGetIndexBufferData output;
                        Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
                        Get().IAGetIndexBuffer(&temp, &output.format, &output.offset);
                        output.buffer.Attach(temp.Detach());
                        return output;
                    }

                    std::vector<Wrapper<ID3D11Buffer>> GSGetConstantBuffers(
                        UINT StartSlot,
                        UINT NumBuffers)
                    {
                        std::unique_ptr<ID3D11Buffer*[]> tempBuffers = std::make_unique<ID3D11Buffer*[]>(NumBuffers);
                        Get().GSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

                        std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                        for(UINT i = 0; i < NumBuffers; i++)
                        {
                            buffers[i].Attach(tempBuffers[i]);
                        }

                        return buffers;
                    }

                    std::pair<Wrapper<ID3D11GeometryShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> GSGetShader(UINT numInstances = 0)
                    {
                        std::pair<Wrapper<ID3D11GeometryShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                        std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
                        Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader;

                        Get().GSGetShader(&shader, tempInstances.data(), &numInstances);
                        output.first.Attach(shader.Detach());
                        output.second.resize(numInstances);
                        for(UINT i = 0; i < numInstances; i++)
                        {
                            output.second[i].Attach(tempInstances[i]);
                        }

                        return output;
                    }

                    D3D11_PRIMITIVE_TOPOLOGY IAGetPrimitiveTopology()
                    {
                        D3D11_PRIMITIVE_TOPOLOGY topology;
                        Get().IAGetPrimitiveTopology(&topology);
                        return topology;
                    }

                    std::vector<Wrapper<ID3D11ShaderResourceView>> VSGetShaderResources(
                        UINT StartSlot,
                        UINT NumViews)
                    {
                        std::unique_ptr<ID3D11ShaderResourceView*[]> tempViews = std::make_unique<ID3D11ShaderResourceView*[]>(NumViews);
                        Get().VSGetShaderResources(StartSlot, NumViews, tempViews.get());

                        std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                        for(UINT i = 0; i < NumViews; i++)
                        {
                            views[i].Attach(tempViews[i]);
                        }

                        return views;
                    }

                    std::vector<Wrapper<ID3D11SamplerState>> VSGetSamplers(
                        UINT StartSlot,
                        UINT NumSamplers)
                    {
                        std::unique_ptr<ID3D11SamplerState*[]> tempSamplers = std::make_unique<ID3D11SamplerState*[]>(NumSamplers - StartSlot);
                        Get().VSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

                        std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
                        for(UINT i = 0; i < NumSamplers; i++)
                        {
                            samplers[i].Attach(tempSamplers[i]);
                        }

                        return samplers;
                    }

                    std::pair<Microsoft::WRL::ComPtr<ID3D11Predicate>, BOOL> GetPredication()
                    {
                        std::pair<Microsoft::WRL::ComPtr<ID3D11Predicate>, BOOL> output;
                        Get().GetPredication(&output.first, &output.second);
                        return output;
                    }

                    std::vector<Wrapper<ID3D11ShaderResourceView>> GSGetShaderResources(
                        UINT StartSlot,
                        UINT NumViews)
                    {
                        std::unique_ptr<ID3D11ShaderResourceView*[]> tempViews = std::make_unique<ID3D11ShaderResourceView*[]>(NumViews);
                        Get().GSGetShaderResources(StartSlot, NumViews, tempViews.get());

                        std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                        for(UINT i = 0; i < NumViews; i++)
                        {
                            views[i].Attach(tempViews[i]);
                        }

                        return views;
                    }

                    std::vector<Wrapper<ID3D11SamplerState>> GSGetSamplers(
                        UINT StartSlot,
                        UINT NumSamplers)
                    {
                        std::unique_ptr<ID3D11SamplerState*[]> tempSamplers = std::make_unique<ID3D11SamplerState*[]>(NumSamplers - StartSlot);
                        Get().GSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

                        std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
                        for(UINT i = 0; i < NumSamplers; i++)
                        {
                            samplers[i].Attach(tempSamplers[i]);
                        }

                        return samplers;
                    }

                    std::pair<std::vector<Wrapper<ID3D11RenderTargetView>>, Wrapper<ID3D11DepthStencilView>>  OMGetRenderTargets(
                        UINT NumViews)
                    {
                        std::unique_ptr<ID3D11RenderTargetView* []> tempViews = std::make_unique<ID3D11RenderTargetView* []>(NumViews);
                        std::pair<std::vector<Wrapper<ID3D11RenderTargetView>>, Wrapper<ID3D11DepthStencilView>> output;
                        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> tempDS;
                        Get().OMGetRenderTargets(NumViews, tempViews.get(), &tempDS);

                        output.second = std::move(tempDS);

                        for(UINT i = 0; i < NumViews; i++)
                        {
                            Microsoft::WRL::ComPtr<ID3D11RenderTargetView> tempRTV;
                            tempRTV.Attach(tempViews[i]);
                            output.first.push_back();
                            output.first.back() = std::move(tempRTV);
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

                        Get().OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, tempRTVs.get(), &data.depthStencilView, UAVStartSlot, NumUAVs, tempUAVs.get());

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
                        Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
                        Get().OMGetBlendState(&blendState, data.blendFactor.data(), &data.sampleMask);
                        data.blendState.Attach(blendState.Detach());
                        return data;
                    }

                    std::pair<Wrapper<ID3D11DepthStencilState>, UINT> OMGetDepthStencilState()
                    {
                        std::pair<Wrapper<ID3D11DepthStencilState>, UINT> state;
                        ID3D11DepthStencilState* temp;
                        Get().OMGetDepthStencilState(&temp, &state.second);
                        state.first.Attach(temp);
                        return state;

                    }

                    std::vector<Wrapper<ID3D11Buffer>> SOGetTargets(UINT NumBuffers)
                    {
                        std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer* []>(NumBuffers);
                        Get().SOGetTargets(NumBuffers, tempBuffers.get());

                        std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                        for(UINT i = 0; i < NumBuffers; i++)
                        {
                            buffers[i].Attach(tempBuffers[i]);
                        }

                        return buffers;
                    }

                    Wrapper<ID3D11RasterizerState> RSGetState()
                    {
                        Microsoft::WRL::ComPtr<ID3D11RasterizerState> state;
                        Get().RSGetState(&state);
                        return Wrapper<ID3D11RasterizerState>(std::move(state));
                    }

                    std::vector<D3D11_VIEWPORT> RSGetViewports()
                    {
                        UINT numViewports;
                        Get().RSGetViewports(&numViewports, nullptr);

                        std::vector<D3D11_VIEWPORT> output(numViewports);
                        Get().RSGetViewports(&numViewports, output.data());

                        return output;
                    }

                    std::vector<D3D11_RECT> RSGetScissorRects()
                    {
                        UINT numRects;
                        Get().RSGetScissorRects(&numRects, nullptr);

                        std::vector<D3D11_RECT> output(numRects);
                        Get().RSGetScissorRects(&numRects, output.data());

                        return output;
                    }

                    std::vector<Wrapper<ID3D11ShaderResourceView>> HSGetShaderResources(
                        UINT StartSlot,
                        UINT NumViews)
                    {
                        std::unique_ptr<ID3D11ShaderResourceView*[]> tempViews = std::make_unique<ID3D11ShaderResourceView*[]>(NumViews);
                        Get().HSGetShaderResources(StartSlot, NumViews, tempViews.get());

                        std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                        for(UINT i = 0; i < NumViews; i++)
                        {
                            views[i].Attach(tempViews[i]);
                        }

                        return views;
                    }

                    std::pair<Wrapper<ID3D11HullShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> HSGetShader(UINT numInstances = 0)
                    {
                        std::pair<Wrapper<ID3D11HullShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                        std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
                        Microsoft::WRL::ComPtr<ID3D11HullShader> shader;

                        Get().HSGetShader(&shader, tempInstances.data(), &numInstances);
                        output.first.Attach(shader.Detach());
                        output.second.resize(numInstances);
                        for(UINT i = 0; i < numInstances; i++)
                        {
                            output.second[i].Attach(tempInstances[i]);
                        }

                        return output;
                    }

                    std::vector<Wrapper<ID3D11SamplerState>> HSGetSamplers(
                        UINT StartSlot,
                        UINT NumSamplers)
                    {
                        std::unique_ptr<ID3D11SamplerState*[]> tempSamplers = std::make_unique<ID3D11SamplerState*[]>(NumSamplers - StartSlot);
                        Get().HSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

                        std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
                        for(UINT i = 0; i < NumSamplers; i++)
                        {
                            samplers[i].Attach(tempSamplers[i]);
                        }

                        return samplers;
                    }

                    std::vector<Wrapper<ID3D11Buffer>> HSGetConstantBuffers(
                        UINT StartSlot,
                        UINT NumBuffers)
                    {
                        std::unique_ptr<ID3D11Buffer*[]> tempBuffers = std::make_unique<ID3D11Buffer*[]>(NumBuffers);
                        Get().HSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

                        std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                        for(UINT i = 0; i < NumBuffers; i++)
                        {
                            buffers[i].Attach(tempBuffers[i]);
                        }

                        return buffers;
                    }

                    std::vector<Wrapper<ID3D11ShaderResourceView>> DSGetShaderResources(
                        UINT StartSlot,
                        UINT NumViews)
                    {
                        std::unique_ptr<ID3D11ShaderResourceView*[]> tempViews = std::make_unique<ID3D11ShaderResourceView*[]>(NumViews);
                        Get().DSGetShaderResources(StartSlot, NumViews, tempViews.get());

                        std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                        for(UINT i = 0; i < NumViews; i++)
                        {
                            views[i].Attach(tempViews[i]);
                        }

                        return views;
                    }

                    std::pair<Wrapper<ID3D11DomainShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> DSGetShader(UINT numInstances = 0)
                    {
                        std::pair<Wrapper<ID3D11DomainShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                        std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
                        Microsoft::WRL::ComPtr<ID3D11DomainShader> shader;

                        Get().DSGetShader(&shader, tempInstances.data(), &numInstances);
                        output.first.Attach(shader.Detach());
                        output.second.resize(numInstances);
                        for(UINT i = 0; i < numInstances; i++)
                        {
                            output.second[i].Attach(tempInstances[i]);
                        }

                        return output;
                    }

                    std::vector<Wrapper<ID3D11SamplerState>> DSGetSamplers(
                        UINT StartSlot,
                        UINT NumSamplers)
                    {
                        std::unique_ptr<ID3D11SamplerState*[]> tempSamplers = std::make_unique<ID3D11SamplerState*[]>(NumSamplers - StartSlot);
                        Get().DSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

                        std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
                        for(UINT i = 0; i < NumSamplers; i++)
                        {
                            samplers[i].Attach(tempSamplers[i]);
                        }

                        return samplers;
                    }

                    std::vector<Wrapper<ID3D11Buffer>> DSGetConstantBuffers(
                        UINT StartSlot,
                        UINT NumBuffers)
                    {
                        std::unique_ptr<ID3D11Buffer*[]> tempBuffers = std::make_unique<ID3D11Buffer*[]>(NumBuffers);
                        Get().DSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

                        std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                        for(UINT i = 0; i < NumBuffers; i++)
                        {
                            buffers[i].Attach(tempBuffers[i]);
                        }

                        return buffers;
                    }

                    std::vector<Wrapper<ID3D11ShaderResourceView>> CSGetShaderResources(
                        UINT StartSlot,
                        UINT NumViews)
                    {
                        std::unique_ptr<ID3D11ShaderResourceView*[]> tempViews = std::make_unique<ID3D11ShaderResourceView*[]>(NumViews);
                        Get().CSGetShaderResources(StartSlot, NumViews, tempViews.get());

                        std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
                        for(UINT i = 0; i < NumViews; i++)
                        {
                            views[i].Attach(tempViews[i]);
                        }

                        return views;
                    }

                    std::vector<Wrapper<ID3D11UnorderedAccessView>> CSGetUnorderedAccessViews(
                        UINT StartSlot,
                        UINT NumViews)
                    {
                        std::unique_ptr<ID3D11UnorderedAccessView* []> tempViews = std::make_unique<ID3D11UnorderedAccessView* []>(NumViews);
                        Get().CSGetUnorderedAccessViews(StartSlot, NumViews, tempViews.get());

                        std::vector<Wrapper<ID3D11UnorderedAccessView>> views(NumViews);
                        for(UINT i = 0; i < NumViews; i++)
                        {
                            views[i].Attach(tempViews[i]);
                        }

                        return views;
                    }

                    std::pair<Wrapper<ID3D11ComputeShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> CSGetShader(UINT numInstances = 0)
                    {
                        std::pair<Wrapper<ID3D11ComputeShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
                        std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
                        Microsoft::WRL::ComPtr<ID3D11ComputeShader> shader;

                        Get().CSGetShader(&shader, tempInstances.data(), &numInstances);
                        output.first.Attach(shader.Detach());
                        output.second.resize(numInstances);
                        for(UINT i = 0; i < numInstances; i++)
                        {
                            output.second[i].Attach(tempInstances[i]);
                        }

                        return output;
                    }

                    std::vector<Wrapper<ID3D11SamplerState>> CSGetSamplers(
                        UINT StartSlot,
                        UINT NumSamplers)
                    {
                        std::unique_ptr<ID3D11SamplerState*[]> tempSamplers = std::make_unique<ID3D11SamplerState*[]>(NumSamplers - StartSlot);
                        Get().CSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

                        std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
                        for(UINT i = 0; i < NumSamplers; i++)
                        {
                            samplers[i].Attach(tempSamplers[i]);
                        }

                        return samplers;
                    }

                    std::vector<Wrapper<ID3D11Buffer>> CSGetConstantBuffers(
                        UINT StartSlot,
                        UINT NumBuffers)
                    {
                        std::unique_ptr<ID3D11Buffer*[]> tempBuffers = std::make_unique<ID3D11Buffer*[]>(NumBuffers);
                        Get().CSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

                        std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
                        for(UINT i = 0; i < NumBuffers; i++)
                        {
                            buffers[i].Attach(tempBuffers[i]);
                        }

                        return buffers;
                    }

                    void ClearState()
                    {
                        return Get().ClearState();
                    }

                    void Flush()
                    {
                        return Get().Flush();
                    }

                    D3D11_DEVICE_CONTEXT_TYPE GetType()
                    {
                        return Get().GetType();
                    }

                    UINT GetContextFlags()
                    {
                        return Get().GetContextFlags();
                    }

                    tl::expected<Microsoft::WRL::ComPtr<ID3D11CommandList>, HRESULT> FinishCommandList(BOOL RestoreDeferredContextState)
                    {
                        Microsoft::WRL::ComPtr<ID3D11CommandList> commandList;

                        if(HRESULT result = Get().FinishCommandList(RestoreDeferredContextState, &commandList); FAILED(result))
                            return tl::unexpected(result);

                        return commandList;
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };
        }
    }

    template<std::derived_from<ID3D11DeviceContext> Ty>
    struct Traits<Ty>
    {
        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using const_reference = const Ty&;

        template<class DerivedSelf>
        using Interface = typename D3D11::DeviceContext::Traits<Ty>::template Interface<DerivedSelf>;
    };
}

namespace TypedD3D::D3D11
{
    template<std::derived_from<ID3D11DeviceContext> DeviceContextTy>
    using DeviceContext_t = Internal::D3D11::DeviceContext_t<DeviceContextTy>;

    using DeviceContext = DeviceContext_t<ID3D11DeviceContext>;
}