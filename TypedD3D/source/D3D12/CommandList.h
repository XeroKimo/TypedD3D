//*********************************************************
//
// Copyright (c) 2022 Renzy Alarcon
// Licensed under the MIT License (MIT).
//
//*********************************************************
#pragma once
#include "CommandAllocator.h"
#include "DescriptorHeap.h"
#include "source/Internal/D3D12/Meta.h"
#include "source/Internal/IUnknownWrapper.h"
#include "source/Helpers/COMHelpers.h"
#include "source/D3D12Wrappers.h"
#include "span_tuple.h"
#include <memory>
#include <d3d12.h>
#include <array>
#include <span>
#include <wrl/client.h>
#include <assert.h>
#include <optional>

#pragma warning (push)
#pragma warning (disable : 4584)

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        namespace CommandList
        {
            enum class TypeEnum
            {
                Direct,
                Compute,
                Copy,
                Bundle,
                RenderPass
            };

            template<D3D12_COMMAND_LIST_TYPE>
            constexpr TypeEnum ToInternalType;

            template<>
            constexpr TypeEnum ToInternalType<D3D12_COMMAND_LIST_TYPE_DIRECT> = TypeEnum::Direct;

            template<>
            constexpr TypeEnum ToInternalType<D3D12_COMMAND_LIST_TYPE_COMPUTE> = TypeEnum::Compute;

            template<>
            constexpr TypeEnum ToInternalType<D3D12_COMMAND_LIST_TYPE_COPY> = TypeEnum::Copy;

            template<>
            constexpr TypeEnum ToInternalType<D3D12_COMMAND_LIST_TYPE_BUNDLE> = TypeEnum::Bundle;

            template<TypeEnum>
            constexpr D3D12_COMMAND_LIST_TYPE ToStandardType;

            template<>
            constexpr D3D12_COMMAND_LIST_TYPE ToStandardType<TypeEnum::Direct> = D3D12_COMMAND_LIST_TYPE_DIRECT;

            template<>
            constexpr D3D12_COMMAND_LIST_TYPE ToStandardType<TypeEnum::Compute> = D3D12_COMMAND_LIST_TYPE_COMPUTE;

            template<>
            constexpr D3D12_COMMAND_LIST_TYPE ToStandardType<TypeEnum::Copy> = D3D12_COMMAND_LIST_TYPE_COPY;

            template<>
            constexpr D3D12_COMMAND_LIST_TYPE ToStandardType<TypeEnum::Bundle> = D3D12_COMMAND_LIST_TYPE_BUNDLE;
        }

        template<std::derived_from<ID3D12CommandList> Ty, D3D12_COMMAND_LIST_TYPE Type>
        using CommandList_t = IUnknownWrapper<Ty, CommandList::ToInternalType<Type>>;

        template<std::derived_from<ID3D12CommandList> Ty>
        using RenderPass_t = IUnknownWrapper<Ty, CommandList::TypeEnum::RenderPass>;

        namespace CommandList
        {
            //Commandlist API restrictions https://docs.microsoft.com/en-us/windows/win32/direct3d12/recording-command-lists-and-bundles#command-list-api-restrictions

            template<class Ty>
            struct TraitsImpl;

            template<>
            struct TraitsImpl<ID3D12CommandList>
            {
                using value_type = ID3D12CommandList;
                using pointer = ID3D12CommandList*;
                using const_pointer = const ID3D12CommandList*;
                using reference = ID3D12CommandList&;
                using const_reference = const ID3D12CommandList&;

                template<class DerivedSelf>
                class Interface
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    D3D12_COMMAND_LIST_TYPE GetType() { return Get().GetType(); }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };

                template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
                class InterfaceTagged : public Interface<DerivedSelf>
                {
                private:
                    static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
                    using derived_self = DerivedSelf;
                    using allocator_value_type = D3D12::CommandAllocator_t<Type>;

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList>
            {
                using value_type = ID3D12GraphicsCommandList;
                using pointer = ID3D12GraphicsCommandList*;
                using const_pointer = const ID3D12GraphicsCommandList*;
                using reference = ID3D12GraphicsCommandList&;
                using const_reference = const ID3D12GraphicsCommandList&;

                template<class DerivedSelf>
                class Interface
                {
                private:
                    using derived_self = DerivedSelf;

                public:

                    void BeginEvent(
                        UINT Metadata,
                        const void* pData,
                        UINT Size)
                    {
                        Get().BeginEvent(Metadata, pData, Size);
                    }

                    void BeginQuery(
                        ID3D12QueryHeap& pQueryHeap,
                        D3D12_QUERY_TYPE Type,
                        UINT Index)
                    {
                        Get().BeginQuery(&pQueryHeap, Type, Index);
                    }

                    void ClearDepthStencilView(
                        DSV<D3D12_CPU_DESCRIPTOR_HANDLE> DepthStencilView,
                        D3D12_CLEAR_FLAGS ClearFlags,
                        FLOAT Depth,
                        UINT8 Stencil,
                        std::span<const D3D12_RECT> rects)
                    {
                        Get().ClearDepthStencilView(DepthStencilView, ClearFlags, Depth, Stencil, static_cast<UINT>(rects.size()), rects.data());
                    }

                    void ClearRenderTargetView(
                        RTV<D3D12_CPU_DESCRIPTOR_HANDLE> RenderTargetView,
                        std::span<const float, 4> colorRGBA,
                        std::span<const D3D12_RECT> rects)
                    {
                        Get().ClearRenderTargetView(RenderTargetView.Get(), colorRGBA.data(), static_cast<UINT>(rects.size()), rects.data());
                    }

                    void ClearState(ID3D12PipelineState* pPipelineState)
                    {
                        Get().ClearState(pPipelineState);
                    }

                    void ClearUnorderedAccessViewFloat(
                        CBV_SRV_UAV<D3D12_GPU_DESCRIPTOR_HANDLE> ViewGPUHandleInCurrentHeap,
                        CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE> ViewCPUHandle,
                        ID3D12Resource& pResource,
                        std::span<const float, 4> values,
                        std::span<const D3D12_RECT> rects)
                    {
                        Get().ClearUnorderedAccessViewFloat(ViewGPUHandleInCurrentHeap, ViewCPUHandle, &pResource, values.data(), static_cast<UINT>(rects.size()), rects.data());
                    }

                    void ClearUnorderedAccessViewUint(
                        CBV_SRV_UAV<D3D12_GPU_DESCRIPTOR_HANDLE> ViewGPUHandleInCurrentHeap,
                        CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE> ViewCPUHandle,
                        ID3D12Resource& pResource,
                        std::span<const UINT, 4> values,
                        std::span<const D3D12_RECT> rects)
                    {
                        Get().ClearUnorderedAccessViewUint(ViewGPUHandleInCurrentHeap, ViewCPUHandle, &pResource, values.data(), static_cast<UINT>(rects.size()), rects.data());
                    }

                    HRESULT Close()
                    {
                        return Get().Close();
                    }

                    void CopyBufferRegion(
                        ID3D12Resource& pDstBuffer,
                        UINT64 DstOffset,
                        ID3D12Resource& pSrcBuffer,
                        UINT64 SrcOffset,
                        UINT64 NumBytes)
                    {
                        Get().CopyBufferRegion(
                            &pDstBuffer,
                            DstOffset,
                            &pSrcBuffer,
                            SrcOffset,
                            NumBytes);
                    }

                    void CopyResource(
                        ID3D12Resource& pDstResource,
                        ID3D12Resource& pSrcResource)
                    {
                        Get().CopyResource(&pDstResource, &pSrcResource);
                    }

                    void CopyTextureRegion(
                        const D3D12_TEXTURE_COPY_LOCATION& pDst,
                        UINT DstX,
                        UINT DstY,
                        UINT DstZ,
                        const D3D12_TEXTURE_COPY_LOCATION& pSrc,
                        const D3D12_BOX* pSrcBox)
                    {
                        Get().CopyTextureRegion(
                            &pDst,
                            DstX,
                            DstY,
                            DstZ,
                            &pSrc,
                            pSrcBox);
                    }

                    void CopyTiles(
                        ID3D12Resource& pTiledResource,
                        const D3D12_TILED_RESOURCE_COORDINATE& pTileRegionStartCoordinate,
                        const D3D12_TILE_REGION_SIZE& pTileRegionSize,
                        ID3D12Resource& pBuffer,
                        UINT64 BufferStartOffsetInBytes,
                        D3D12_TILE_COPY_FLAGS Flags)
                    {
                        Get().CopyTiles(
                            &pTiledResource,
                            &pTileRegionStartCoordinate,
                            &pTileRegionSize,
                            &pBuffer,
                            BufferStartOffsetInBytes,
                            Flags);
                    }

                    void DiscardResource(
                        ID3D12Resource& pResource,
                        const D3D12_DISCARD_REGION* pRegion)
                    {
                        Get().DiscardResource(&pResource, pRegion);
                    }

                    void Dispatch(
                        UINT ThreadGroupCountX,
                        UINT ThreadGroupCountY,
                        UINT ThreadGroupCountZ)
                    {
                        Get().Dispatch(
                            ThreadGroupCountX,
                            ThreadGroupCountY,
                            ThreadGroupCountZ);
                    }

                    void DrawIndexedInstanced(
                        UINT IndexCountPerInstance,
                        UINT InstanceCount,
                        UINT StartIndexLocation,
                        INT BaseVertexLocation,
                        UINT StartInstanceLocation)
                    {
                        Get().DrawIndexedInstanced(
                            IndexCountPerInstance,
                            InstanceCount,
                            StartIndexLocation,
                            BaseVertexLocation,
                            StartInstanceLocation);
                    }

                    void DrawInstanced(
                        UINT VertexCountPerInstance,
                        UINT InstanceCount,
                        UINT StartVertexLocation,
                        UINT StartInstanceLocation)
                    {
                        Get().DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
                    }

                    void EndEvent()
                    {
                        Get().EndEvent();
                    }

                    void EndQuery(
                        ID3D12QueryHeap& pQueryHeap,
                        D3D12_QUERY_TYPE Type,
                        UINT Index)
                    {
                        Get().EndQuery(&pQueryHeap, Type, Index);
                    }

                    void ExecuteBundle(CommandList_t<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_BUNDLE> pCommandList);

                    void ExecuteIndirect(
                        ID3D12CommandSignature& commandSignature,
                        UINT maxCommandCount,
                        ID3D12Resource& argumentBuffer,
                        UINT64 argumentBufferOffset,
                        ID3D12Resource* optCountBuffer = nullptr,
                        UINT64 optCountBufferOffset = 0)
                    {
                        Get().ExecuteIndirect(&commandSignature, maxCommandCount, &argumentBuffer, argumentBufferOffset, optCountBuffer, optCountBufferOffset);
                    }

                    void IASetIndexBuffer(
                        const D3D12_INDEX_BUFFER_VIEW* pView)
                    {
                        Get().IASetIndexBuffer(pView);
                    }

                    void IASetPrimitiveTopology(
                        D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology)
                    {
                        Get().IASetPrimitiveTopology(PrimitiveTopology);
                    }

                    void IASetVertexBuffers(
                        UINT StartSlot,
                        std::span<const D3D12_VERTEX_BUFFER_VIEW> views)
                    {
                        Get().IASetVertexBuffers(StartSlot, static_cast<UINT>(views.size()), views.data());
                    }

                    void OMSetBlendFactor(std::span<const float, 4> BlendFactor)
                    {
                        Get().OMSetBlendFactor(BlendFactor.data());
                    }

                    void OMSetRenderTargets(
                        std::span<const RTV<D3D12_CPU_DESCRIPTOR_HANDLE>> pRenderTargetDescriptors,
                        BOOL RTsSingleHandleToDescriptorRange,
                        const DSV<D3D12_CPU_DESCRIPTOR_HANDLE>* pDepthStencilDescriptor)
                    {
                        std::unique_ptr<D3D12_CPU_DESCRIPTOR_HANDLE[]> renderTargets = std::make_unique<D3D12_CPU_DESCRIPTOR_HANDLE[]>(pRenderTargetDescriptors.size());

                        for(size_t i = 0; i < pRenderTargetDescriptors.size(); i++)
                        {
                            renderTargets[i] = pRenderTargetDescriptors[i].Get();
                        }

                        const D3D12_CPU_DESCRIPTOR_HANDLE* depthStencil = (pDepthStencilDescriptor) ? &pDepthStencilDescriptor->Get() : nullptr;

                        Get().OMSetRenderTargets(static_cast<UINT>(pRenderTargetDescriptors.size()), renderTargets.get(), RTsSingleHandleToDescriptorRange, depthStencil);
                    }

                    void OMSetStencilRef(UINT StencilRef)
                    {
                        Get().OMSetStencilRef(StencilRef);
                    }

                    void ResolveQueryData(
                        ID3D12QueryHeap& pQueryHeap,
                        D3D12_QUERY_TYPE Type,
                        UINT StartIndex,
                        UINT NumQueries,
                        ID3D12Resource& pDestinationBuffer,
                        UINT64 AlignedDestinationBufferOffset)
                    {
                        Get().ResolveQueryData(&pQueryHeap, Type, StartIndex, NumQueries, &pDestinationBuffer, AlignedDestinationBufferOffset);
                    }

                    void ResolveSubresource(
                        ID3D12Resource& pDstResource,
                        UINT DstSubresource,
                        ID3D12Resource& pSrcResource,
                        UINT SrcSubresource,
                        DXGI_FORMAT Format)
                    {
                        Get().ResolveSubresource(
                            &pDstResource,
                            DstSubresource,
                            &pSrcResource,
                            SrcSubresource,
                            Format);
                    }

                    void ResourceBarrier(std::span<const D3D12_RESOURCE_BARRIER> Barriers)
                    {
                        Get().ResourceBarrier(static_cast<UINT>(Barriers.size()), Barriers.data());
                    }

                    void RSSetScissorRects(
                        std::span<D3D12_RECT> rects)
                    {
                        Get().RSSetScissorRects(static_cast<UINT>(rects.size()), rects.data());
                    }

                    void RSSetViewports(
                        std::span<D3D12_VIEWPORT> viewports)
                    {
                        Get().RSSetViewports(static_cast<UINT>(viewports.size()), viewports.data());
                    }

                    void SetComputeRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
                    {
                        Get().SetComputeRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
                    }

                    void SetComputeRoot32BitConstants(
                        UINT RootParameterIndex,
                        UINT Num32BitValuesToSet,
                        const void* pSrcData,
                        UINT DestOffsetIn32BitValues)
                    {
                        Get().SetComputeRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
                    }

                    void SetComputeRootConstantBufferView(
                        UINT RootParameterIndex,
                        D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                    {
                        Get().SetComputeRootConstantBufferView(RootParameterIndex, BufferLocation);
                    }

                    void SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
                    {
                        Get().SetComputeRootDescriptorTable(RootParameterIndex, BaseDescriptor);
                    }

                    void SetComputeRootShaderResourceView(
                        UINT RootParameterIndex,
                        D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                    {
                        Get().SetComputeRootShaderResourceView(RootParameterIndex, BufferLocation);
                    }

                    void SetComputeRootSignature(ID3D12RootSignature* pRootSignature)
                    {
                        Get().SetComputeRootSignature(pRootSignature);
                    }

                    void SetComputeRootUnorderedAccessView(
                        UINT RootParameterIndex,
                        D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                    {
                        Get().SetComputeRootUnorderedAccessView(RootParameterIndex, BufferLocation);
                    }

                    void SetDescriptorHeaps(ShaderVisible<CBV_SRV_UAV<ID3D12DescriptorHeap>> descriptorHeap)
                    {
                        ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
                        Get().SetDescriptorHeaps(1, heaps);
                    }

                    void SetDescriptorHeaps(ShaderVisible<Sampler<ID3D12DescriptorHeap>> descriptorHeap)
                    {
                        ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
                        Get().SetDescriptorHeaps(1, heaps);
                    }

                    void SetDescriptorHeaps(ShaderVisible<CBV_SRV_UAV<ID3D12DescriptorHeap>> cbv_srv_uavHeap, ShaderVisible<Sampler<ID3D12DescriptorHeap>> samplerHeap)
                    {
                        ID3D12DescriptorHeap* heaps[] = { cbv_srv_uavHeap.Get(), samplerHeap.Get() };
                        Get().SetDescriptorHeaps(2, heaps);
                    }

                    void SetGraphicsRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
                    {
                        Get().SetGraphicsRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
                    }

                    void SetGraphicsRoot32BitConstants(
                        UINT RootParameterIndex,
                        UINT Num32BitValuesToSet,
                        const void* pSrcData,
                        UINT DestOffsetIn32BitValues)
                    {
                        Get().SetGraphicsRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
                    }

                    void SetGraphicsRootConstantBufferView(
                        UINT RootParameterIndex,
                        D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                    {
                        Get().SetGraphicsRootConstantBufferView(RootParameterIndex, BufferLocation);
                    }

                    void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
                    {
                        Get().SetGraphicsRootDescriptorTable(RootParameterIndex, BaseDescriptor);
                    }

                    void SetGraphicsRootShaderResourceView(
                        UINT RootParameterIndex,
                        D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                    {
                        Get().SetGraphicsRootShaderResourceView(RootParameterIndex, BufferLocation);
                    }

                    void SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature)
                    {
                        Get().SetGraphicsRootSignature(pRootSignature);
                    }

                    void SetGraphicsRootUnorderedAccessView(
                        UINT RootParameterIndex,
                        D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                    {
                        Get().SetGraphicsRootUnorderedAccessView(RootParameterIndex, BufferLocation);
                    }

                    void SetMarker(
                        UINT Metadata,
                        const void* pData,
                        UINT Size)
                    {
                        Get().SetMarker(Metadata, pData, Size);
                    }

                    void SetPipelineState(ID3D12PipelineState* pPipelineState)
                    {
                        Get().SetPipelineState(pPipelineState);
                    }

                    void SetPredication(
                        ID3D12Resource* pBuffer,
                        UINT64 AlignedBufferOffset,
                        D3D12_PREDICATION_OP Operation)
                    {
                        Get().SetPredication(pBuffer, AlignedBufferOffset, Operation);
                    }

                    void SOSetTargets(
                        UINT StartSlot,
                        std::span<const D3D12_STREAM_OUTPUT_BUFFER_VIEW> views)
                    {
                        Get().SOSetTargets(StartSlot, static_cast<UINT>(views.size()), views.data());
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };

                template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
                class InterfaceTagged : public Interface<DerivedSelf>
                {
                private:
                    static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
                    using derived_self = DerivedSelf;
                    using allocator_value_type = D3D12::CommandAllocator_t<Type>;

                public:
                    HRESULT Reset(
                        allocator_value_type pAllocator,
                        ID3D12PipelineState* pInitialState)
                    {
                        return Get().Reset(pAllocator.Get(), pInitialState);
                    }

                  ////Alphabetical order
                  //using Base::BeginEvent;
                  //using Base::BeginQuery;
                  //using Base::ClearDepthStencilView;
                  //using Base::ClearRenderTargetView;
                  //using Base::ClearState;
                  //using Base::ClearUnorderedAccessViewFloat;
                  //using Base::ClearUnorderedAccessViewUint;
                  //using Base::Close;
                  //using Base::CopyBufferRegion;
                  //using Base::CopyResource;
                  //using Base::CopyTextureRegion;
                  //using Base::CopyTiles;
                  //using Base::DiscardResource;
                  //using Base::Dispatch;
                  //using Base::DrawIndexedInstanced;
                  //using Base::DrawInstanced;
                  //using Base::EndEvent;
                  //using Base::EndQuery;
                  //using Base::ExecuteBundle;
                  //using Base::ExecuteIndirect;
                  //using Base::IASetIndexBuffer;
                  //using Base::IASetPrimitiveTopology;
                  //using Base::IASetVertexBuffers;
                  //using Base::OMSetBlendFactor;
                  //using Base::OMSetRenderTargets;
                  //using Base::OMSetStencilRef;
                  //using Base::Reset;
                  //using Base::ResolveQueryData;
                  //using Base::ResolveSubresource;
                  //using Base::ResourceBarrier;
                  //using Base::RSSetScissorRects;
                  //using Base::RSSetViewports;
                  //using Base::SetComputeRoot32BitConstant;
                  //using Base::SetComputeRoot32BitConstants;
                  //using Base::SetComputeRootConstantBufferView;
                  //using Base::SetComputeRootDescriptorTable;
                  //using Base::SetComputeRootShaderResourceView;
                  //using Base::SetComputeRootSignature;
                  //using Base::SetComputeRootUnorderedAccessView;
                  //using Base::SetDescriptorHeaps;
                  //using Base::SetGraphicsRoot32BitConstant;
                  //using Base::SetGraphicsRoot32BitConstants;
                  //using Base::SetGraphicsRootConstantBufferView;
                  //using Base::SetGraphicsRootDescriptorTable;
                  //using Base::SetGraphicsRootShaderResourceView;
                  //using Base::SetGraphicsRootSignature;
                  //using Base::SetGraphicsRootUnorderedAccessView;
                  //using Base::SetMarker;
                  //using Base::SetPipelineState;
                  //using Base::SetPredication;
                  //using Base::SOSetTargets;

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList1>
            {
                using value_type = ID3D12GraphicsCommandList1;
                using pointer = ID3D12GraphicsCommandList1*;
                using const_pointer = const ID3D12GraphicsCommandList1*;
                using reference = ID3D12GraphicsCommandList1&;
                using const_reference = const ID3D12GraphicsCommandList1&;

                template<class DerivedSelf>
                class Interface
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    void AtomicCopyBufferUINT(
                        ID3D12Resource& pDstBuffer,
                        UINT64 DstOffset,
                        ID3D12Resource& pSrcBuffer,
                        UINT64 SrcOffset,
                        UINT Dependencies,
                        ID3D12Resource* const* ppDependentResources,
                        const D3D12_SUBRESOURCE_RANGE_UINT64* pDependentSubresourceRanges)
                    {
                        Get().AtomicCopyBufferUINT(
                            &pDstBuffer,
                            DstOffset,
                            &pSrcBuffer,
                            SrcOffset,
                            Dependencies,
                            ppDependentResources,
                            pDependentSubresourceRanges);
                    }

                    void AtomicCopyBufferUINT64(
                        ID3D12Resource& pDstBuffer,
                        UINT64 DstOffset,
                        ID3D12Resource& pSrcBuffer,
                        UINT64 SrcOffset,
                        UINT Dependencies,
                        ID3D12Resource* const* ppDependentResources,
                        const D3D12_SUBRESOURCE_RANGE_UINT64* pDependentSubresourceRanges)
                    {
                        Get().AtomicCopyBufferUINT64(
                            &pDstBuffer,
                            DstOffset,
                            &pSrcBuffer,
                            SrcOffset,
                            Dependencies,
                            ppDependentResources,
                            pDependentSubresourceRanges);
                    }

                    void OMSetDepthBounds(
                        FLOAT Min,
                        FLOAT Max)
                    {
                        Get().OMSetDepthBounds(Min, Max);
                    }

                    void ResolveSubresourceRegion(
                        ID3D12Resource& pDstResource,
                        UINT DstSubresource,
                        UINT DstX,
                        UINT DstY,
                        ID3D12Resource& pSrcResource,
                        UINT SrcSubresource,
                        D3D12_RECT* pSrcRect,
                        DXGI_FORMAT Format,
                        D3D12_RESOLVE_MODE ResolveMode)
                    {
                        Get().ResolveSubresourceRegion(
                            &pDstResource,
                            DstSubresource,
                            DstX,
                            DstY,
                            &pSrcResource,
                            SrcSubresource,
                            pSrcRect,
                            Format,
                            ResolveMode
                        );
                    }

                    void SetSamplePositions(
                        UINT NumSamplesPerPixel,
                        UINT NumPixels,
                        D3D12_SAMPLE_POSITION* pSamplePositions)
                    {
                        Get().SetSamplePositions(
                            NumSamplesPerPixel,
                            NumPixels,
                            pSamplePositions);
                    }

                    void SetViewInstanceMask(
                        UINT Mask)
                    {
                        Get().SetViewInstanceMask(Mask);
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };

                template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
                class InterfaceTagged : public Interface<DerivedSelf>
                {
                private:
                    static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
                    using derived_self = DerivedSelf;
                    using allocator_value_type = D3D12::CommandAllocator_t<Type>;

                public:
                    ////Alphabetical order
                    //using Base::AtomicCopyBufferUINT;
                    //using Base::AtomicCopyBufferUINT64;
                    //using Base::OMSetDepthBounds;
                    //using Base::ResolveSubresourceRegion;
                    //using Base::SetSamplePositions;
                    //using Base::SetViewInstanceMask;     

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList2>
            {
                using value_type = ID3D12GraphicsCommandList2;
                using pointer = ID3D12GraphicsCommandList2*;
                using const_pointer = const ID3D12GraphicsCommandList2*;
                using reference = ID3D12GraphicsCommandList2&;
                using const_reference = const ID3D12GraphicsCommandList2&;

                template<class DerivedSelf>
                class Interface
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    void WriteBufferImmediate(
                        xk::span_tuple<D3D12_WRITEBUFFERIMMEDIATE_PARAMETER, std::dynamic_extent, xk::optional<D3D12_WRITEBUFFERIMMEDIATE_MODE>> pParams)
                    {
                        Get().WriteBufferImmediate(static_cast<UINT>(pParams.size()), pParams.data<0>(), pParams.data<1>());
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };

                template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
                class InterfaceTagged : Interface<DerivedSelf>
                {
                private:
                    static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
                    using derived_self = DerivedSelf;
                    using allocator_value_type = D3D12::CommandAllocator_t<Type>;

                public:
                    ////Alphabetical order
                    //using Base::WriteBufferImmediate;
                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList3>
            {
                using value_type = ID3D12GraphicsCommandList3;
                using pointer = ID3D12GraphicsCommandList3*;
                using const_pointer = const ID3D12GraphicsCommandList3*;
                using reference = ID3D12GraphicsCommandList3&;
                using const_reference = const ID3D12GraphicsCommandList3&;

                template<class DerivedSelf>
                class Interface
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    void SetProtectedResourceSession(
                        ID3D12ProtectedResourceSession* pProtectedResourceSession)
                    {
                        Get().SetProtectedResourceSession(pProtectedResourceSession);
                    }

                    //In code order                                                          //Alphabetical order
                    //using Base::SetProtectedResourceSession;                               using Base::SetProtectedResourceSession;
                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };

                template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
                class InterfaceTagged : Interface<DerivedSelf>
                {
                private:
                    static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
                    using derived_self = DerivedSelf;
                    using allocator_value_type = D3D12::CommandAllocator_t<Type>;

                public:
                    //In code order                                                          //Alphabetical order
                    //using Base::SetProtectedResourceSession;                               using Base::SetProtectedResourceSession;

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList4>
            {
                using value_type = ID3D12GraphicsCommandList4;
                using pointer = ID3D12GraphicsCommandList4*;
                using const_pointer = const ID3D12GraphicsCommandList4*;
                using reference = ID3D12GraphicsCommandList4&;
                using const_reference = const ID3D12GraphicsCommandList4&;

                template<class DerivedSelf>
                class Interface
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    RenderPass_t<ID3D12GraphicsCommandList4> BeginRenderPass(
                        std::span<const D3D12_RENDER_PASS_RENDER_TARGET_DESC> renderTargets,
                        const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* pDepthStencil,
                        D3D12_RENDER_PASS_FLAGS Flags);

                    void BuildRaytracingAccelerationStructure(
                        const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& pDesc,
                        std::span<const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC> pPostbuildInfoDescs)
                    {
                        Get().BuildRaytracingAccelerationStructure(
                            &pDesc,
                            static_cast<UINT>(pPostbuildInfoDescs.size()),
                            pPostbuildInfoDescs.data());
                    }

                    void CopyRaytracingAccelerationStructure(
                        D3D12_GPU_VIRTUAL_ADDRESS DestAccelerationStructureData,
                        D3D12_GPU_VIRTUAL_ADDRESS SourceAccelerationStructureData,
                        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE Mode)
                    {
                        Get().CopyRaytracingAccelerationStructure(
                            DestAccelerationStructureData,
                            SourceAccelerationStructureData,
                            Mode);
                    }

                    void DispatchRays(
                        const D3D12_DISPATCH_RAYS_DESC& pDesc)
                    {
                        Get().DispatchRays(&pDesc);
                    }

                    void EmitRaytracingAccelerationStructurePostbuildInfo(
                        const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC& pDesc,
                        std::span<const D3D12_GPU_VIRTUAL_ADDRESS> pSourceAccelerationStructureData)
                    {
                        Get().EmitRaytracingAccelerationStructurePostbuildInfo(
                            &pDesc,
                            static_cast<UINT>(pSourceAccelerationStructureData.size()),
                            pSourceAccelerationStructureData.data());
                    }

                    void EndRenderPass()
                    {
                        Get().EndRenderPass();
                    }

                    void InitializeMetaCommand(
                        ID3D12MetaCommand& pMetaCommand,
                        const void* pInitializationParametersData,
                        SIZE_T InitializationParametersDataSizeInBytes)
                    {
                        Get().InitializeMetaCommand(&pMetaCommand, pInitializationParametersData, InitializationParametersDataSizeInBytes);
                    }

                    void ExecuteMetaCommand(
                        ID3D12MetaCommand& pMetaCommand,
                        const void* pExecutionParametersData,
                        SIZE_T ExecutionParametersDataSizeInBytes)
                    {
                        Get().ExecuteMetaCommand(&pMetaCommand, pExecutionParametersData, ExecutionParametersDataSizeInBytes);
                    }

                    void ExecuteMetaCommand(
                        ID3D12MetaCommand& pMetaCommand)
                    {
                        ExecuteMetaCommand(pMetaCommand, nullptr, 0);
                    }

                    template<class T>
                    void ExecuteMetaCommand(
                        ID3D12MetaCommand& pMetaCommand,
                        const T& pExecutionParametersData)
                    {
                        ExecuteMetaCommand(pMetaCommand, &pExecutionParametersData, sizeof(T));
                    }

                    void InitializeMetaCommand(
                        ID3D12MetaCommand& pMetaCommand)
                    {
                        InitializeMetaCommand(pMetaCommand, nullptr, 0);
                    }

                    template<class T>
                    void InitializeMetaCommand(
                        ID3D12MetaCommand& pMetaCommand,
                        const T& pInitializationParametersData)
                    {
                        InitializeMetaCommand(pMetaCommand, &pInitializationParametersData, sizeof(T));
                    }

                    void SetPipelineState1(
                        ID3D12StateObject* pStateObject)
                    {
                        Get().SetPipelineState1(pStateObject);
                    }
                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };

                template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
                class InterfaceTagged : public Interface<DerivedSelf>
                {
                private:
                    static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
                    using derived_self = DerivedSelf;
                    using allocator_value_type = D3D12::CommandAllocator_t<Type>;

                public:
                    ////Alphabetical order
                    //using Base::BeginRenderPass;
                    //using Base::BuildRaytracingAccelerationStructure;
                    //using Base::CopyRaytracingAccelerationStructure;
                    //using Base::DispatchRays;
                    //using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
                    //using Base::EndRenderPass;
                    //using Base::ExecuteMetaCommand;
                    //using Base::InitializeMetaCommand;
                    //using Base::SetPipelineState1;

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList5>
            {
                using value_type = ID3D12GraphicsCommandList5;
                using pointer = ID3D12GraphicsCommandList5*;
                using const_pointer = const ID3D12GraphicsCommandList5*;
                using reference = ID3D12GraphicsCommandList5&;
                using const_reference = const ID3D12GraphicsCommandList5&;

                template<class DerivedSelf>
                class Interface
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    void RSSetShadingRate(
                        D3D12_SHADING_RATE baseShadingRate,
                        const D3D12_SHADING_RATE_COMBINER* combiners)
                    {
                        Get().RSSetShadingRate(
                            baseShadingRate,
                            combiners);
                    }

                    void RSSetShadingRateImage(
                        ID3D12Resource* shadingRateImage)
                    {
                        Get().RSSetShadingRateImage(
                            shadingRateImage);
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };

                template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
                class InterfaceTagged : public Interface<DerivedSelf>
                {
                private:
                    static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
                    using derived_self = DerivedSelf;
                    using allocator_value_type = D3D12::CommandAllocator_t<Type>;

                public:
                    ////Alphabetical order
                    //using Base::RSSetShadingRate;
                    //using Base::RSSetShadingRateImage;
                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };


            template<class Ty, D3D12_COMMAND_LIST_TYPE Type>
            struct Traits;

            template<D3D12_COMMAND_LIST_TYPE Type>
            struct Traits<ID3D12CommandList, Type>
            {
                using value_type = ID3D12CommandList;
                using pointer = ID3D12CommandList*;
                using const_pointer = const ID3D12CommandList*;
                using reference = ID3D12CommandList&;
                using const_reference = const ID3D12CommandList&;


                template<class DerivedSelf>
                class Interface : public TraitsImpl<ID3D12CommandList>::InterfaceTagged<DerivedSelf, Type>
                {

                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_DIRECT>
            {
                using value_type = ID3D12GraphicsCommandList;
                using pointer = ID3D12GraphicsCommandList*;
                using const_pointer = const ID3D12GraphicsCommandList*;
                using reference = ID3D12GraphicsCommandList&;
                using const_reference = const ID3D12GraphicsCommandList&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12CommandList, D3D12_COMMAND_LIST_TYPE_DIRECT>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
                {

                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList1, D3D12_COMMAND_LIST_TYPE_DIRECT>
            {
                using value_type = ID3D12GraphicsCommandList1;
                using pointer = ID3D12GraphicsCommandList1*;
                using const_pointer = const ID3D12GraphicsCommandList1*;
                using reference = ID3D12GraphicsCommandList1&;
                using const_reference = const ID3D12GraphicsCommandList1&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_DIRECT>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList1>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
                {

                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList2, D3D12_COMMAND_LIST_TYPE_DIRECT>
            {
                using value_type = ID3D12GraphicsCommandList2;
                using pointer = ID3D12GraphicsCommandList2*;
                using const_pointer = const ID3D12GraphicsCommandList2*;
                using reference = ID3D12GraphicsCommandList2&;
                using const_reference = const ID3D12GraphicsCommandList2&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList1, D3D12_COMMAND_LIST_TYPE_DIRECT>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList2>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
                {

                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList3, D3D12_COMMAND_LIST_TYPE_DIRECT>
            {
                using value_type = ID3D12GraphicsCommandList3;
                using pointer = ID3D12GraphicsCommandList3*;
                using const_pointer = const ID3D12GraphicsCommandList3*;
                using reference = ID3D12GraphicsCommandList3&;
                using const_reference = const ID3D12GraphicsCommandList3&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList2, D3D12_COMMAND_LIST_TYPE_DIRECT>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList3>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
                {

                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList4, D3D12_COMMAND_LIST_TYPE_DIRECT>
            {
                using value_type = ID3D12GraphicsCommandList4;
                using pointer = ID3D12GraphicsCommandList4*;
                using const_pointer = const ID3D12GraphicsCommandList4*;
                using reference = ID3D12GraphicsCommandList4&;
                using const_reference = const ID3D12GraphicsCommandList4&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList3, D3D12_COMMAND_LIST_TYPE_DIRECT>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList4>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
                {

                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList5, D3D12_COMMAND_LIST_TYPE_DIRECT>
            {
                using value_type = ID3D12GraphicsCommandList5;
                using pointer = ID3D12GraphicsCommandList5*;
                using const_pointer = const ID3D12GraphicsCommandList5*;
                using reference = ID3D12GraphicsCommandList5&;
                using const_reference = const ID3D12GraphicsCommandList5&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList4, D3D12_COMMAND_LIST_TYPE_DIRECT>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList5>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
                {

                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_COMPUTE>
            {
                using value_type = ID3D12GraphicsCommandList;
                using pointer = ID3D12GraphicsCommandList*;
                using const_pointer = const ID3D12GraphicsCommandList*;
                using reference = ID3D12GraphicsCommandList&;
                using const_reference = const ID3D12GraphicsCommandList&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12CommandList, D3D12_COMMAND_LIST_TYPE_COMPUTE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
                private:
                    using Base::BeginEvent;
                    using Base::BeginQuery;
                    using Base::ClearDepthStencilView;
                    using Base::ClearRenderTargetView;
                    //using Base::ClearState;
                    //using Base::ClearUnorderedAccessViewFloat;
                    //using Base::ClearUnorderedAccessViewUint;
                    //using Base::Close;
                    //using Base::CopyBufferRegion;
                    //using Base::CopyResource;
                    //using Base::CopyTextureRegion;
                    //using Base::CopyTiles;
                    //using Base::DiscardResource;
                    //using Base::Dispatch;
                    using Base::DrawIndexedInstanced;
                    using Base::DrawInstanced;
                    using Base::EndEvent;
                    //using Base::EndQuery;
                    using Base::ExecuteBundle;
                    //using Base::ExecuteIndirect;
                    using Base::IASetIndexBuffer;
                    using Base::IASetPrimitiveTopology;
                    using Base::IASetVertexBuffers;
                    using Base::OMSetBlendFactor;
                    using Base::OMSetRenderTargets;
                    using Base::OMSetStencilRef;
                    //using Base::Reset;
                    //using Base::ResolveQueryData;
                    using Base::ResolveSubresource;
                    //using Base::ResourceBarrier;
                    //using Base::RSSetScissorRects;
                    //using Base::RSSetViewports;
                    //using Base::SetComputeRoot32BitConstant;
                    //using Base::SetComputeRoot32BitConstants;
                    //using Base::SetComputeRootConstantBufferView;
                    //using Base::SetComputeRootDescriptorTable;
                    //using Base::SetComputeRootShaderResourceView;
                    //using Base::SetComputeRootSignature;
                    //using Base::SetComputeRootUnorderedAccessView;
                    //using Base::SetDescriptorHeaps;
                    using Base::SetGraphicsRoot32BitConstant;
                    using Base::SetGraphicsRoot32BitConstants;
                    using Base::SetGraphicsRootConstantBufferView;
                    using Base::SetGraphicsRootDescriptorTable;
                    using Base::SetGraphicsRootShaderResourceView;
                    using Base::SetGraphicsRootSignature;
                    using Base::SetGraphicsRootUnorderedAccessView;
                    using Base::SetMarker;
                    //using Base::SetPipelineState;
                    //using Base::SetPredication;
                    using Base::SOSetTargets;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList1, D3D12_COMMAND_LIST_TYPE_COMPUTE>
            {
                using value_type = ID3D12GraphicsCommandList1;
                using pointer = ID3D12GraphicsCommandList1*;
                using const_pointer = const ID3D12GraphicsCommandList1*;
                using reference = ID3D12GraphicsCommandList1&;
                using const_reference = const ID3D12GraphicsCommandList1&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_COMPUTE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList1>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList1>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;

                private:
                    //using Base::AtomicCopyBufferUINT;
                    //using Base::AtomicCopyBufferUINT64;
                    using Base::OMSetDepthBounds;
                    using Base::ResolveSubresourceRegion;
                    using Base::SetSamplePositions;
                    using Base::SetViewInstanceMask;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList2, D3D12_COMMAND_LIST_TYPE_COMPUTE>
            {
                using value_type = ID3D12GraphicsCommandList2;
                using pointer = ID3D12GraphicsCommandList2*;
                using const_pointer = const ID3D12GraphicsCommandList2*;
                using reference = ID3D12GraphicsCommandList2&;
                using const_reference = const ID3D12GraphicsCommandList2&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList1, D3D12_COMMAND_LIST_TYPE_COMPUTE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList2>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList2>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;

                private:
                    //using Base::WriteBufferImmediate;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList3, D3D12_COMMAND_LIST_TYPE_COMPUTE>
            {
                using value_type = ID3D12GraphicsCommandList3;
                using pointer = ID3D12GraphicsCommandList3*;
                using const_pointer = const ID3D12GraphicsCommandList3*;
                using reference = ID3D12GraphicsCommandList3&;
                using const_reference = const ID3D12GraphicsCommandList3&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList2, D3D12_COMMAND_LIST_TYPE_COMPUTE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList3>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList3>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;

                private:
                    //using Base::SetProtectedResourceSession;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList4, D3D12_COMMAND_LIST_TYPE_COMPUTE>
            {
                using value_type = ID3D12GraphicsCommandList4;
                using pointer = ID3D12GraphicsCommandList4*;
                using const_pointer = const ID3D12GraphicsCommandList4*;
                using reference = ID3D12GraphicsCommandList4&;
                using const_reference = const ID3D12GraphicsCommandList4&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList3, D3D12_COMMAND_LIST_TYPE_COMPUTE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList4>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList4>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
                private:
                    using Base::BeginRenderPass;
                    using Base::BuildRaytracingAccelerationStructure;
                    using Base::CopyRaytracingAccelerationStructure;
                    //using Base::DispatchRays;
                    using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
                    using Base::EndRenderPass;
                    using Base::ExecuteMetaCommand;
                    using Base::InitializeMetaCommand;
                    //using Base::SetPipelineState1;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList5, D3D12_COMMAND_LIST_TYPE_COMPUTE>
            {
                using value_type = ID3D12GraphicsCommandList5;
                using pointer = ID3D12GraphicsCommandList5*;
                using const_pointer = const ID3D12GraphicsCommandList5*;
                using reference = ID3D12GraphicsCommandList5&;
                using const_reference = const ID3D12GraphicsCommandList5&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList4, D3D12_COMMAND_LIST_TYPE_COMPUTE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList5>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList5>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
                private:
                    using Base::RSSetShadingRate;
                    using Base::RSSetShadingRateImage;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_COPY>
            {
                using value_type = ID3D12GraphicsCommandList;
                using pointer = ID3D12GraphicsCommandList*;
                using const_pointer = const ID3D12GraphicsCommandList*;
                using reference = ID3D12GraphicsCommandList&;
                using const_reference = const ID3D12GraphicsCommandList&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12CommandList, D3D12_COMMAND_LIST_TYPE_COPY>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;

                private:
                    using Base::BeginEvent;
                    using Base::BeginQuery;
                    using Base::ClearDepthStencilView;
                    using Base::ClearRenderTargetView;
                    using Base::ClearState;
                    using Base::ClearUnorderedAccessViewFloat;
                    using Base::ClearUnorderedAccessViewUint;
                    //using Base::Close;
                    //using Base::CopyBufferRegion;
                    //using Base::CopyResource;
                    //using Base::CopyTextureRegion;
                    //using Base::CopyTiles;
                    //using Base::DiscardResource;
                    //using Base::Dispatch;
                    using Base::DrawIndexedInstanced;
                    using Base::DrawInstanced;
                    using Base::EndEvent;
                    //using Base::EndQuery;
                    using Base::ExecuteBundle;
                    using Base::ExecuteIndirect;
                    using Base::IASetIndexBuffer;
                    using Base::IASetPrimitiveTopology;
                    using Base::IASetVertexBuffers;
                    using Base::OMSetBlendFactor;
                    using Base::OMSetRenderTargets;
                    using Base::OMSetStencilRef;
                    //using Base::Reset;
                    //using Base::ResolveQueryData;
                    using Base::ResolveSubresource;
                    //using Base::ResourceBarrier;
                    using Base::RSSetScissorRects;
                    using Base::RSSetViewports;
                    using Base::SetComputeRoot32BitConstant;
                    using Base::SetComputeRoot32BitConstants;
                    using Base::SetComputeRootConstantBufferView;
                    using Base::SetComputeRootDescriptorTable;
                    using Base::SetComputeRootShaderResourceView;
                    using Base::SetComputeRootSignature;
                    using Base::SetComputeRootUnorderedAccessView;
                    using Base::SetDescriptorHeaps;
                    using Base::SetGraphicsRoot32BitConstant;
                    using Base::SetGraphicsRoot32BitConstants;
                    using Base::SetGraphicsRootConstantBufferView;
                    using Base::SetGraphicsRootDescriptorTable;
                    using Base::SetGraphicsRootShaderResourceView;
                    using Base::SetGraphicsRootSignature;
                    using Base::SetGraphicsRootUnorderedAccessView;
                    using Base::SetMarker;
                    using Base::SetPipelineState;
                    using Base::SetPredication;
                    using Base::SOSetTargets;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList1, D3D12_COMMAND_LIST_TYPE_COPY>
            {
                using value_type = ID3D12GraphicsCommandList1;
                using pointer = ID3D12GraphicsCommandList1*;
                using const_pointer = const ID3D12GraphicsCommandList1*;
                using reference = ID3D12GraphicsCommandList1&;
                using const_reference = const ID3D12GraphicsCommandList1&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_COPY>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList1>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList1>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;

                private:
                    //using Base::AtomicCopyBufferUINT;
                    //using Base::AtomicCopyBufferUINT64;
                    using Base::OMSetDepthBounds;
                    using Base::ResolveSubresourceRegion;
                    using Base::SetSamplePositions;
                    using Base::SetViewInstanceMask;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList2, D3D12_COMMAND_LIST_TYPE_COPY>
            {
                using value_type = ID3D12GraphicsCommandList2;
                using pointer = ID3D12GraphicsCommandList2*;
                using const_pointer = const ID3D12GraphicsCommandList2*;
                using reference = ID3D12GraphicsCommandList2&;
                using const_reference = const ID3D12GraphicsCommandList2&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList1, D3D12_COMMAND_LIST_TYPE_COPY>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList2>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList2>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;

                private:
                    //using Base::WriteBufferImmediate;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList3, D3D12_COMMAND_LIST_TYPE_COPY>
            {
                using value_type = ID3D12GraphicsCommandList3;
                using pointer = ID3D12GraphicsCommandList3*;
                using const_pointer = const ID3D12GraphicsCommandList3*;
                using reference = ID3D12GraphicsCommandList3&;
                using const_reference = const ID3D12GraphicsCommandList3&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList2, D3D12_COMMAND_LIST_TYPE_COPY>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList3>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList3>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;

                private:
                    //using Base::SetProtectedResourceSession;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList4, D3D12_COMMAND_LIST_TYPE_COPY>
            {
                using value_type = ID3D12GraphicsCommandList4;
                using pointer = ID3D12GraphicsCommandList4*;
                using const_pointer = const ID3D12GraphicsCommandList4*;
                using reference = ID3D12GraphicsCommandList4&;
                using const_reference = const ID3D12GraphicsCommandList4&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList3, D3D12_COMMAND_LIST_TYPE_COPY>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList4>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList4>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;

                private:
                    using Base::BeginRenderPass;
                    using Base::BuildRaytracingAccelerationStructure;
                    using Base::CopyRaytracingAccelerationStructure;
                    //using Base::DispatchRays;
                    using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
                    using Base::EndRenderPass;
                    using Base::ExecuteMetaCommand;
                    using Base::InitializeMetaCommand;
                    //using Base::SetPipelineState1;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList5, D3D12_COMMAND_LIST_TYPE_COPY>
            {
                using value_type = ID3D12GraphicsCommandList5;
                using pointer = ID3D12GraphicsCommandList5*;
                using const_pointer = const ID3D12GraphicsCommandList5*;
                using reference = ID3D12GraphicsCommandList5&;
                using const_reference = const ID3D12GraphicsCommandList5&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList4, D3D12_COMMAND_LIST_TYPE_COPY>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList5>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList5>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;
                private:
                    using Base::RSSetShadingRate;
                    using Base::RSSetShadingRateImage;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_BUNDLE>
            {
                using value_type = ID3D12GraphicsCommandList;
                using pointer = ID3D12GraphicsCommandList*;
                using const_pointer = const ID3D12GraphicsCommandList*;
                using reference = ID3D12GraphicsCommandList&;
                using const_reference = const ID3D12GraphicsCommandList&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12CommandList, D3D12_COMMAND_LIST_TYPE_BUNDLE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;

                private:
                    using Base::BeginEvent;
                    using Base::BeginQuery;
                    using Base::ClearDepthStencilView;
                    using Base::ClearRenderTargetView;
                    using Base::ClearState;
                    using Base::ClearUnorderedAccessViewFloat;
                    using Base::ClearUnorderedAccessViewUint;
                    //using Base::Close;
                    using Base::CopyBufferRegion;
                    using Base::CopyResource;
                    using Base::CopyTextureRegion;
                    using Base::CopyTiles;
                    using Base::DiscardResource;
                    //using Base::Dispatch;
                    //using Base::DrawIndexedInstanced;
                    //using Base::DrawInstanced;
                    using Base::EndEvent;
                    using Base::EndQuery;
                    using Base::ExecuteBundle;
                    //using Base::ExecuteIndirect;
                    //using Base::IASetIndexBuffer;
                    //using Base::IASetPrimitiveTopology;
                    //using Base::IASetVertexBuffers;
                    //using Base::OMSetBlendFactor;
                    using Base::OMSetRenderTargets;
                    //using Base::OMSetStencilRef;
                    //using Base::Reset;
                    using Base::ResolveQueryData;
                    using Base::ResolveSubresource;
                    using Base::ResourceBarrier;
                    using Base::RSSetScissorRects;
                    using Base::RSSetViewports;
                    //using Base::SetComputeRoot32BitConstant;
                    //using Base::SetComputeRoot32BitConstants;
                    //using Base::SetComputeRootConstantBufferView;
                    //using Base::SetComputeRootDescriptorTable;
                    //using Base::SetComputeRootShaderResourceView;
                    //using Base::SetComputeRootSignature;
                    //using Base::SetComputeRootUnorderedAccessView;
                    //using Base::SetDescriptorHeaps;
                    //using Base::SetGraphicsRoot32BitConstant;
                    //using Base::SetGraphicsRoot32BitConstants;
                    //using Base::SetGraphicsRootConstantBufferView;
                    //using Base::SetGraphicsRootDescriptorTable;
                    //using Base::SetGraphicsRootShaderResourceView;
                    //using Base::SetGraphicsRootSignature;
                    //using Base::SetGraphicsRootUnorderedAccessView;
                    //using Base::SetMarker;
                    //using Base::SetPipelineState;
                    using Base::SetPredication;
                    using Base::SOSetTargets;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList1, D3D12_COMMAND_LIST_TYPE_BUNDLE>
            {
                using value_type = ID3D12GraphicsCommandList1;
                using pointer = ID3D12GraphicsCommandList1*;
                using const_pointer = const ID3D12GraphicsCommandList1*;
                using reference = ID3D12GraphicsCommandList1&;
                using const_reference = const ID3D12GraphicsCommandList1&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_BUNDLE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList1>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList1>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;

                private:
                    using Base::AtomicCopyBufferUINT;
                    using Base::AtomicCopyBufferUINT64;
                    //using Base::OMSetDepthBounds;
                    using Base::ResolveSubresourceRegion;
                    //using Base::SetSamplePositions;
                    //using Base::SetViewInstanceMask;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList2, D3D12_COMMAND_LIST_TYPE_BUNDLE>
            {
                using value_type = ID3D12GraphicsCommandList2;
                using pointer = ID3D12GraphicsCommandList2*;
                using const_pointer = const ID3D12GraphicsCommandList2*;
                using reference = ID3D12GraphicsCommandList2&;
                using const_reference = const ID3D12GraphicsCommandList2&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList1, D3D12_COMMAND_LIST_TYPE_BUNDLE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList2>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList2>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;

                private:
                    //using Base::WriteBufferImmediate;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList3, D3D12_COMMAND_LIST_TYPE_BUNDLE>
            {
                using value_type = ID3D12GraphicsCommandList3;
                using pointer = ID3D12GraphicsCommandList3*;
                using const_pointer = const ID3D12GraphicsCommandList3*;
                using reference = ID3D12GraphicsCommandList3&;
                using const_reference = const ID3D12GraphicsCommandList3&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList2, D3D12_COMMAND_LIST_TYPE_BUNDLE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList3>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList3>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;

                private:
                    using Base::SetProtectedResourceSession;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList4, D3D12_COMMAND_LIST_TYPE_BUNDLE>
            {
                using value_type = ID3D12GraphicsCommandList4;
                using pointer = ID3D12GraphicsCommandList4*;
                using const_pointer = const ID3D12GraphicsCommandList4*;
                using reference = ID3D12GraphicsCommandList4&;
                using const_reference = const ID3D12GraphicsCommandList4&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList3, D3D12_COMMAND_LIST_TYPE_BUNDLE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList4>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList4>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;

                private:
                    using Base::BeginRenderPass;
                    //using Base::BuildRaytracingAccelerationStructure;
                    //using Base::CopyRaytracingAccelerationStructure;
                    //using Base::DispatchRays;
                    //using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
                    using Base::EndRenderPass;
                    //using Base::ExecuteMetaCommand;
                    //using Base::InitializeMetaCommand;
                    //using Base::SetPipelineState1;
                };
            };

            template<>
            struct Traits<ID3D12GraphicsCommandList5, D3D12_COMMAND_LIST_TYPE_BUNDLE>
            {
                using value_type = ID3D12GraphicsCommandList5;
                using pointer = ID3D12GraphicsCommandList5*;
                using const_pointer = const ID3D12GraphicsCommandList5*;
                using reference = ID3D12GraphicsCommandList5&;
                using const_reference = const ID3D12GraphicsCommandList5&;


                template<class DerivedSelf>
                class Interface : public Traits<ID3D12GraphicsCommandList4, D3D12_COMMAND_LIST_TYPE_BUNDLE>::Interface<DerivedSelf>,
                    public TraitsImpl<ID3D12GraphicsCommandList5>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
                {
                    using Base = TraitsImpl<ID3D12GraphicsCommandList5>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
                private:
                    //using Base::RSSetShadingRate;
                    //using Base::RSSetShadingRateImage;
                };
            };
        }


        namespace RenderPass
        {
            template<class Ty>
            struct TraitsImpl;

            template<>
            struct TraitsImpl<ID3D12CommandList>
            {
                using value_type = ID3D12CommandList;
                using pointer = ID3D12CommandList*;
                using const_pointer = const ID3D12CommandList*;
                using reference = ID3D12CommandList&;
                using const_reference = const ID3D12CommandList&;

                template<class DerivedSelf>
                class Interface : public CommandList::TraitsImpl<ID3D12CommandList>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
                {

                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList>
            {
                using value_type = ID3D12GraphicsCommandList;
                using pointer = ID3D12GraphicsCommandList*;
                using const_pointer = const ID3D12GraphicsCommandList*;
                using reference = ID3D12GraphicsCommandList&;
                using const_reference = const ID3D12GraphicsCommandList&;

                template<class DerivedSelf>
                class Interface : public CommandList::TraitsImpl<ID3D12GraphicsCommandList>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>,
                    public TraitsImpl<ID3D12CommandList>::Interface<DerivedSelf>
                {
                private:
                    using Base = CommandList::TraitsImpl<ID3D12GraphicsCommandList>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;

                private:
                    using Base::BeginEvent;
                    //using Base::BeginQuery;
                    using Base::ClearDepthStencilView;
                    using Base::ClearRenderTargetView;
                    using Base::ClearState;
                    using Base::ClearUnorderedAccessViewFloat;
                    using Base::ClearUnorderedAccessViewUint;
                    //using Base::Close;
                    using Base::CopyBufferRegion;
                    using Base::CopyResource;
                    using Base::CopyTextureRegion;
                    using Base::CopyTiles;
                    using Base::DiscardResource;
                    using Base::Dispatch;
                    //using Base::DrawIndexedInstanced;
                    //using Base::DrawInstanced;
                    //using Base::EndEvent;
                    //using Base::EndQuery;
                    //using Base::ExecuteBundle;
                    //using Base::ExecuteIndirect;
                    //using Base::IASetIndexBuffer;
                    //using Base::IASetPrimitiveTopology;
                    //using Base::IASetVertexBuffers;
                    //using Base::OMSetBlendFactor;
                    using Base::OMSetRenderTargets;
                    //using Base::OMSetStencilRef;
                    //using Base::Reset;
                    using Base::ResolveQueryData;
                    using Base::ResolveSubresource;
                    //using Base::ResourceBarrier;
                    //using Base::RSSetScissorRects;
                    //using Base::RSSetViewports;
                    //using Base::SetComputeRoot32BitConstant;
                    //using Base::SetComputeRoot32BitConstants;
                    //using Base::SetComputeRootConstantBufferView;
                    //using Base::SetComputeRootDescriptorTable;
                    //using Base::SetComputeRootShaderResourceView;
                    //using Base::SetComputeRootSignature;
                    //using Base::SetComputeRootUnorderedAccessView;
                    //using Base::SetDescriptorHeaps;
                    //using Base::SetGraphicsRoot32BitConstant;
                    //using Base::SetGraphicsRoot32BitConstants;
                    //using Base::SetGraphicsRootConstantBufferView;
                    //using Base::SetGraphicsRootDescriptorTable;
                    //using Base::SetGraphicsRootShaderResourceView;
                    //using Base::SetGraphicsRootSignature;
                    //using Base::SetGraphicsRootUnorderedAccessView;
                    //using Base::SetMarker;
                    //using Base::SetPipelineState;
                    //using Base::SetPredication;
                    //using Base::SOSetTargets;
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList1>
            {
                using value_type = ID3D12GraphicsCommandList1;
                using pointer = ID3D12GraphicsCommandList1*;
                using const_pointer = const ID3D12GraphicsCommandList1*;
                using reference = ID3D12GraphicsCommandList1&;
                using const_reference = const ID3D12GraphicsCommandList1&;

                template<class DerivedSelf>
                class Interface : public CommandList::TraitsImpl<ID3D12GraphicsCommandList1>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>,
                    public TraitsImpl<ID3D12GraphicsCommandList>::Interface<DerivedSelf>
                {
                    using Base = CommandList::TraitsImpl<ID3D12GraphicsCommandList1>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;

                private:
                    using Base::AtomicCopyBufferUINT;
                    using Base::AtomicCopyBufferUINT64;
                    //using Base::OMSetDepthBounds;
                    using Base::ResolveSubresourceRegion;
                    //using Base::SetSamplePositions;
                    //using Base::SetViewInstanceMask;
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList2>
            {
                using value_type = ID3D12GraphicsCommandList2;
                using pointer = ID3D12GraphicsCommandList2*;
                using const_pointer = const ID3D12GraphicsCommandList2*;
                using reference = ID3D12GraphicsCommandList2&;
                using const_reference = const ID3D12GraphicsCommandList2&;

                template<class DerivedSelf>
                class Interface : public CommandList::TraitsImpl<ID3D12GraphicsCommandList2>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>,
                    public TraitsImpl<ID3D12GraphicsCommandList1>::Interface<DerivedSelf>
                {
                    using Base = CommandList::TraitsImpl<ID3D12GraphicsCommandList2>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;

                private:
                    //using Base::WriteBufferImmediate;
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList3>
            {
                using value_type = ID3D12GraphicsCommandList3;
                using pointer = ID3D12GraphicsCommandList3*;
                using const_pointer = const ID3D12GraphicsCommandList3*;
                using reference = ID3D12GraphicsCommandList3&;
                using const_reference = const ID3D12GraphicsCommandList3&;

                template<class DerivedSelf>
                class Interface : public CommandList::TraitsImpl<ID3D12GraphicsCommandList3>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>,
                    public TraitsImpl<ID3D12GraphicsCommandList2>::Interface<DerivedSelf>
                {

                    using Base = CommandList::TraitsImpl<ID3D12GraphicsCommandList3>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList4>
            {
                using value_type = ID3D12GraphicsCommandList4;
                using pointer = ID3D12GraphicsCommandList4*;
                using const_pointer = const ID3D12GraphicsCommandList4*;
                using reference = ID3D12GraphicsCommandList4&;
                using const_reference = const ID3D12GraphicsCommandList4&;

                template<class DerivedSelf>
                class Interface : public CommandList::TraitsImpl<ID3D12GraphicsCommandList4>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>,
                    public TraitsImpl<ID3D12GraphicsCommandList3>::Interface<DerivedSelf>
                {
                    using Base = CommandList::TraitsImpl<ID3D12GraphicsCommandList4>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;

                private:
                    using Base::BeginRenderPass;
                    using Base::BuildRaytracingAccelerationStructure;
                    using Base::CopyRaytracingAccelerationStructure;
                    using Base::DispatchRays;
                    using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
                    //using Base::EndRenderPass;
                    using Base::ExecuteMetaCommand;
                    using Base::InitializeMetaCommand;
                    using Base::SetPipelineState1;
                };
            };

            template<>
            struct TraitsImpl<ID3D12GraphicsCommandList5>
            {
                using value_type = ID3D12GraphicsCommandList5;
                using pointer = ID3D12GraphicsCommandList5*;
                using const_pointer = const ID3D12GraphicsCommandList5*;
                using reference = ID3D12GraphicsCommandList5&;
                using const_reference = const ID3D12GraphicsCommandList5&;

                template<class DerivedSelf>
                class Interface : public CommandList::TraitsImpl<ID3D12GraphicsCommandList5>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>,
                    public TraitsImpl<ID3D12GraphicsCommandList4>::Interface<DerivedSelf>
                {
                    using Base = CommandList::TraitsImpl<ID3D12GraphicsCommandList5>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;
                private:
                    //using Base::RSSetShadingRate;
                    //using Base::RSSetShadingRateImage;
                };
            };
        }
    }

    template<std::derived_from<ID3D12CommandList> Ty, D3D12::CommandList::TypeEnum Type>
    struct Traits<Ty, Type>
    {
        static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = D3D12::CommandList::ToStandardType<Type>;

        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using const_reference = const Ty&;

        template<class DerivedSelf>
        using Interface = typename D3D12::CommandList::Traits<Ty, command_list_value>::template Interface<DerivedSelf>;
    };

    template<std::derived_from<ID3D12CommandList> Ty>
    struct Traits<Ty, D3D12::CommandList::TypeEnum::RenderPass>
    {
        static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = D3D12_COMMAND_LIST_TYPE_DIRECT;

        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using const_reference = const Ty&;

        template<class DerivedSelf>
        using Interface = typename D3D12::RenderPass::TraitsImpl<Ty>::template Interface<DerivedSelf>;
    };

    template<std::derived_from<ID3D12CommandList> Ty>
    struct DirectMapper<Ty>
    {
        using type = D3D12::CommandList_t<Ty, D3D12_COMMAND_LIST_TYPE_DIRECT>;
    };

    template<std::derived_from<ID3D12CommandList> Ty>
    struct ComputeMapper<Ty>
    {
        using type = D3D12::CommandList_t<Ty, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
    };

    template<std::derived_from<ID3D12CommandList> Ty>
    struct CopyMapper<Ty>
    {
        using type = D3D12::CommandList_t<Ty, D3D12_COMMAND_LIST_TYPE_COPY>;
    };

    template<std::derived_from<ID3D12CommandList> Ty>
    struct BundleMapper<Ty>
    {
        using type = D3D12::CommandList_t<Ty, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
    };

}

namespace TypedD3D::D3D12
{
    using Microsoft::WRL::ComPtr;

    template<std::derived_from<ID3D12CommandList> Ty, D3D12_COMMAND_LIST_TYPE Type>
    using CommandList_t = Internal::D3D12::CommandList_t<Ty, Type>;

    template<std::derived_from<ID3D12CommandList> Ty>
    using RenderPass_t = Internal::D3D12::RenderPass_t<Ty>;

    using RenderPass4 = RenderPass_t<ID3D12GraphicsCommandList4>;
    using RenderPass5 = RenderPass_t<ID3D12GraphicsCommandList5>;
    namespace CommandList
    {
        template<class ListTy>
        using Direct_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_DIRECT>;

        using DirectBase = Direct_t<ID3D12CommandList>;
        using Direct = Direct_t<ID3D12GraphicsCommandList>;
        using Direct1 = Direct_t<ID3D12GraphicsCommandList1>;
        using Direct2 = Direct_t<ID3D12GraphicsCommandList2>;
        using Direct3 = Direct_t<ID3D12GraphicsCommandList3>;
        using Direct4 = Direct_t<ID3D12GraphicsCommandList4>;
        using Direct5 = Direct_t<ID3D12GraphicsCommandList5>;
        //using Direct6 = Internal::Direct<ID3D12GraphicsCommandList6>;

        template<class ListTy>
        using Bundle_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_BUNDLE>;

        using Bundle = Bundle_t<ID3D12GraphicsCommandList>;
        using Bundle1 = Bundle_t<ID3D12GraphicsCommandList1>;
        using Bundle2 = Bundle_t<ID3D12GraphicsCommandList2>;
        using Bundle3 = Bundle_t<ID3D12GraphicsCommandList3>;
        using Bundle4 = Bundle_t<ID3D12GraphicsCommandList4>;
        using Bundle5 = Bundle_t<ID3D12GraphicsCommandList5>;
        //using Bundle6 = Internal::Bundle<ID3D12GraphicsCommandList6>;

        template<class ListTy>
        using Compute_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_COMPUTE>;

        using ComputeBase = Direct_t<ID3D12CommandList>;
        using Compute = Compute_t<ID3D12GraphicsCommandList>;
        using Compute1 = Compute_t<ID3D12GraphicsCommandList1>;
        using Compute2 = Compute_t<ID3D12GraphicsCommandList2>;
        using Compute3 = Compute_t<ID3D12GraphicsCommandList3>;
        using Compute4 = Compute_t<ID3D12GraphicsCommandList4>;
        using Compute5 = Compute_t<ID3D12GraphicsCommandList5>;
        //using Compute6 = Internal::Compute<ID3D12GraphicsCommandList6>;

        template<class ListTy>
        using Copy_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_COPY>;

        using CopyBase = Direct_t<ID3D12CommandList>;
        using Copy = Copy_t<ID3D12GraphicsCommandList>;
        using Copy1 = Copy_t<ID3D12GraphicsCommandList1>;
        using Copy2 = Copy_t<ID3D12GraphicsCommandList2>;
        using Copy3 = Copy_t<ID3D12GraphicsCommandList3>;
        using Copy4 = Copy_t<ID3D12GraphicsCommandList4>;
        using Copy5 = Copy_t<ID3D12GraphicsCommandList5>;
        //using Copy6 = Internal::Copy<ID3D12GraphicsCommandList6>;

    }
};

namespace TypedD3D::Internal::D3D12::CommandList
{
    template<class DerivedSelf>
    void TraitsImpl<ID3D12GraphicsCommandList>::Interface<DerivedSelf>::ExecuteBundle(CommandList_t<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_BUNDLE> pCommandList)
    {
        Get().ExecuteBundle(pCommandList.Get());
    }

    template<class DerivedSelf>
    RenderPass_t<ID3D12GraphicsCommandList4> TraitsImpl<ID3D12GraphicsCommandList4>::Interface<DerivedSelf>::BeginRenderPass(std::span<const D3D12_RENDER_PASS_RENDER_TARGET_DESC> renderTargets, const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* pDepthStencil, D3D12_RENDER_PASS_FLAGS Flags)
    {
        Get().BeginRenderPass(static_cast<UINT>(renderTargets.size()), renderTargets.data(), pDepthStencil, Flags);
        return RenderPass_t<ID3D12GraphicsCommandList4>(&Get());
    }
}

#pragma warning(pop)
