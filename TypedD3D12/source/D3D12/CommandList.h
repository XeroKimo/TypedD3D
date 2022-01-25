//*********************************************************
//
// Copyright (c) 2022 Renzy Alarcon
// Licensed under the MIT License (MIT).
//
//*********************************************************
#pragma once
#include "CommandAllocator.h"
#include "DescriptorHeap.h"
#include "Meta.h"
#include <d3d12.h>
#include <array>
#include <span>
#include <wrl/client.h>
#include <assert.h>
#include <optional>

#pragma warning (push)
#pragma warning (disable : 4584)
namespace TypedD3D::D3D12::CommandList
{
    using Microsoft::WRL::ComPtr;
    namespace Internal
    {
        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type, class ListTy>
        class ListInterface;

        //Can find all API restrictions here https://docs.microsoft.com/en-us/windows/win32/direct3d12/recording-command-lists-and-bundles#command-list-api-restrictions
        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type, class ListTy>
        class PublicListInterface;

        template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
        class CommandList;

        template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
        struct command_list_trait : Meta::command_list_type_tag<Type>
        {
            using trait_value_type = command_list_trait;

            using list_value_type = ListTy;
            using allocator_value_type = CommandAllocator::CommandAllocator_t<Type>;

            template<class WrapperTy>
            using interface_type = PublicListInterface<WrapperTy, Type, ListTy>;
        };

        template<class ListTy>
        using bundle_command_list_trait = command_list_trait<D3D12_COMMAND_LIST_TYPE_BUNDLE, ListTy>;

        template<class ListTy>
        using direct_command_list_trait = command_list_trait<D3D12_COMMAND_LIST_TYPE_DIRECT, ListTy>;

        template<class ListTy>
        using copy_command_list_trait = command_list_trait<D3D12_COMMAND_LIST_TYPE_COPY, ListTy>;

        template<class ListTy>
        using compute_command_list_trait = command_list_trait<D3D12_COMMAND_LIST_TYPE_COMPUTE, ListTy>;

        template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
        using interface_type = typename command_list_trait<Type, ListTy>::template interface_type<CommandList<Type, ListTy>>;

        template<class ListTy>
        using Bundle = CommandList<D3D12_COMMAND_LIST_TYPE_BUNDLE, ListTy>;

        template<class ListTy>
        using Direct = CommandList<D3D12_COMMAND_LIST_TYPE_DIRECT, ListTy>;

        template<class ListTy>
        using Copy = CommandList<D3D12_COMMAND_LIST_TYPE_COPY, ListTy>;

        template<class ListTy>
        using Compute = CommandList<D3D12_COMMAND_LIST_TYPE_COMPUTE, ListTy>;

        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
        class ListInterface<WrapperTy, Type, ID3D12GraphicsCommandList>
        {
        private:
            using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList>;
            using list_value_type = typename trait_value_type::list_value_type;
            using allocator_value_type = typename trait_value_type::allocator_value_type;
            using wrapper_type = WrapperTy;

        public:
            HRESULT Close()
            {
                return InternalCommandList().Close();
            }

            HRESULT Reset(
                allocator_value_type pAllocator,
                ID3D12PipelineState* pInitialState)
            {
                return InternalCommandList().Reset(pAllocator.Get(), pInitialState);
            }

            void ClearState(
                ID3D12PipelineState* pPipelineState)
            {
                InternalCommandList().ClearState(pPipelineState);
            }

            void DrawInstanced(
                UINT VertexCountPerInstance,
                UINT InstanceCount,
                UINT StartVertexLocation,
                UINT StartInstanceLocation)
            {
                InternalCommandList().DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
            }

            void DrawIndexedInstanced(
                UINT IndexCountPerInstance,
                UINT InstanceCount,
                UINT StartIndexLocation,
                INT BaseVertexLocation,
                UINT StartInstanceLocation)
            {
                InternalCommandList().DrawIndexedInstanced(
                    IndexCountPerInstance,
                    InstanceCount,
                    StartIndexLocation,
                    BaseVertexLocation,
                    StartInstanceLocation);
            }

            void Dispatch(
                UINT ThreadGroupCountX,
                UINT ThreadGroupCountY,
                UINT ThreadGroupCountZ)
            {
                InternalCommandList().Dispatch(
                    ThreadGroupCountX,
                    ThreadGroupCountY,
                    ThreadGroupCountZ);
            }

            void CopyBufferRegion(
                ID3D12Resource* pDstBuffer,
                UINT64 DstOffset,
                ID3D12Resource* pSrcBuffer,
                UINT64 SrcOffset,
                UINT64 NumBytes)
            {
                InternalCommandList().CopyBufferRegion(
                    pDstBuffer,
                    DstOffset,
                    pSrcBuffer,
                    SrcOffset,
                    NumBytes);
            }

            void CopyTextureRegion(
                const D3D12_TEXTURE_COPY_LOCATION* pDst,
                UINT DstX,
                UINT DstY,
                UINT DstZ,
                const D3D12_TEXTURE_COPY_LOCATION* pSrc,
                const D3D12_BOX* pSrcBox)
            {
                InternalCommandList().CopyTextureRegion(
                    pDst,
                    DstX,
                    DstY,
                    DstZ,
                    pSrc,
                    pSrcBox);
            }

            void CopyResource(
                ID3D12Resource& pDstResource,
                ID3D12Resource& pSrcResource)
            {
                InternalCommandList().CopyResource(&pDstResource, &pSrcResource);
            }

            void CopyTiles(
                ID3D12Resource& pTiledResource,
                const D3D12_TILED_RESOURCE_COORDINATE& pTileRegionStartCoordinate,
                const D3D12_TILE_REGION_SIZE& pTileRegionSize,
                ID3D12Resource& pBuffer,
                UINT64 BufferStartOffsetInBytes,
                D3D12_TILE_COPY_FLAGS Flags)
            {
                InternalCommandList().CopyTiles(
                    &pTiledResource,
                    &pTileRegionStartCoordinate,
                    &pTileRegionSize,
                    &pBuffer,
                    BufferStartOffsetInBytes,
                    Flags);
            }

            void ResolveSubresource(
                ID3D12Resource& pDstResource,
                UINT DstSubresource,
                ID3D12Resource& pSrcResource,
                UINT SrcSubresource,
                DXGI_FORMAT Format)
            {
                InternalCommandList().ResolveSubresource(
                    &pDstResource,
                    DstSubresource,
                    &pSrcResource,
                    SrcSubresource,
                    Format);
            }

            void IASetPrimitiveTopology(
                D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology)
            {
                InternalCommandList().IASetPrimitiveTopology(PrimitiveTopology);
            }

            void RSSetViewports(
                std::span<D3D12_VIEWPORT> viewports)
            {
                InternalCommandList().RSSetViewports(static_cast<UINT>(viewports.size()), viewports.data());
            }

            void RSSetScissorRects(
                std::span<D3D12_VIEWPORT> rects)
            {
                InternalCommandList().RSSetScissorRects(static_cast<UINT>(rects.size()), rects.data());
            }

            void OMSetBlendFactor(const std::array<float, 4>& BlendFactor)
            {
                InternalCommandList().OMSetBlendFactor(BlendFactor.data());
            }

            void OMSetStencilRef(UINT StencilRef)
            {
                InternalCommandList().OMSetStencilRef(StencilRef);
            }

            void SetPipelineState(ID3D12PipelineState* pPipelineState)
            {
                InternalCommandList().SetPipelineState(pPipelineState);
            }

            void ResourceBarrier(const std::span<D3D12_RESOURCE_BARRIER> Barriers)
            {
                InternalCommandList().ResourceBarrier(static_cast<UINT>(Barriers.size()), Barriers.data());
            }

            void ExecuteBundle(Bundle<ID3D12GraphicsCommandList> pCommandList);

            void SetDescriptorHeaps(D3D12::DescriptorHeap::CBV_SRV_UAV descriptorHeap)
            {
                ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
                InternalCommandList().SetDescriptorHeaps(1, heaps);
            }

            void SetDescriptorHeaps(D3D12::DescriptorHeap::Sampler descriptorHeap)
            {
                ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
                InternalCommandList().SetDescriptorHeaps(1, heaps);
            }

            void SetDescriptorHeaps(D3D12::DescriptorHeap::CBV_SRV_UAV cbv_srv_uavHeap, D3D12::DescriptorHeap::Sampler samplerHeap)
            {
                ID3D12DescriptorHeap* heaps[] = { cbv_srv_uavHeap.Get(), samplerHeap.Get() };
                InternalCommandList().SetDescriptorHeaps(2, heaps);
            }

            void SetComputeRootSignature(ID3D12RootSignature* pRootSignature)
            {
                InternalCommandList().SetComputeRootSignature(pRootSignature);
            }

            void SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature)
            {
                InternalCommandList().SetGraphicsRootSignature(pRootSignature);
            }

            void SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
            {
                InternalCommandList().SetComputeRootDescriptorTable(RootParameterIndex, BaseDescriptor);
            }

            void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
            {
                InternalCommandList().SetGraphicsRootDescriptorTable(RootParameterIndex, BaseDescriptor);
            }

            void SetComputeRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
            {
                InternalCommandList().SetComputeRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
            }

            void SetGraphicsRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
            {
                InternalCommandList().SetGraphicsRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
            }

            void SetComputeRoot32BitConstants(
                UINT RootParameterIndex,
                UINT Num32BitValuesToSet,
                const void* pSrcData,
                UINT DestOffsetIn32BitValues)
            {
                InternalCommandList().SetComputeRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
            }

            void SetGraphicsRoot32BitConstants(
                UINT RootParameterIndex,
                UINT Num32BitValuesToSet,
                const void* pSrcData,
                UINT DestOffsetIn32BitValues)
            {
                InternalCommandList().SetGraphicsRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
            }

            void SetComputeRootConstantBufferView(
                UINT RootParameterIndex,
                D3D12::DescriptorHandle::GPU_CBV_SRV_UAV BufferLocation)
            {
                InternalCommandList().SetComputeRootConstantBufferView(RootParameterIndex, BufferLocation.Data());
            }

            void SetGraphicsRootConstantBufferView(
                UINT RootParameterIndex,
                D3D12::DescriptorHandle::GPU_CBV_SRV_UAV BufferLocation)
            {
                InternalCommandList().SetGraphicsRootConstantBufferView(RootParameterIndex, BufferLocation.Data());
            }

            void SetComputeRootShaderResourceView(
                UINT RootParameterIndex,
                D3D12::DescriptorHandle::GPU_CBV_SRV_UAV BufferLocation)
            {
                InternalCommandList().SetComputeRootShaderResourceView(RootParameterIndex, BufferLocation.Data());
            }

            void SetGraphicsRootShaderResourceView(
                UINT RootParameterIndex,
                D3D12::DescriptorHandle::GPU_CBV_SRV_UAV BufferLocation)
            {
                InternalCommandList().SetGraphicsRootShaderResourceView(RootParameterIndex, BufferLocation.Data());
            }

            void SetComputeRootUnorderedAccessView(
                UINT RootParameterIndex,
                D3D12::DescriptorHandle::GPU_CBV_SRV_UAV BufferLocation)
            {
                InternalCommandList().SetComputeRootUnorderedAccessView(RootParameterIndex, BufferLocation.Data());
            }

            void SetGraphicsRootUnorderedAccessView(
                UINT RootParameterIndex,
                D3D12::DescriptorHandle::GPU_CBV_SRV_UAV BufferLocation)
            {
                InternalCommandList().SetGraphicsRootUnorderedAccessView(RootParameterIndex, BufferLocation.Data());
            }

            void IASetIndexBuffer(
                const D3D12_INDEX_BUFFER_VIEW* pView)
            {
                InternalCommandList().IASetIndexBuffer(pView);
            }

            void IASetVertexBuffers(
                UINT StartSlot,
                UINT NumViews,
                const D3D12_VERTEX_BUFFER_VIEW* pViews)
            {
                InternalCommandList().IASetVertexBuffers(StartSlot, NumViews, pViews);
            }

            void SOSetTargets(
                UINT StartSlot,
                UINT NumViews,
                const D3D12_STREAM_OUTPUT_BUFFER_VIEW* pViews)
            {
                InternalCommandList().SOSetTargets(StartSlot, NumViews, pViews);
            }

            void OMSetRenderTargets(
                const std::span<D3D12_CPU_DESCRIPTOR_HANDLE> pRenderTargetDescriptors,
                BOOL RTsSingleHandleToDescriptorRange,
                const D3D12_CPU_DESCRIPTOR_HANDLE* pDepthStencilDescriptor)
            {
                InternalCommandList().OMSetRenderTargets(static_cast<UINT>(pRenderTargetDescriptors.size()), pRenderTargetDescriptors.data(), RTsSingleHandleToDescriptorRange, pDepthStencilDescriptor);
            }

            void ClearDepthStencilView(
                D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView,
                D3D12_CLEAR_FLAGS ClearFlags,
                FLOAT Depth,
                UINT8 Stencil,
                const std::span<D3D12_RECT> rects)
            {
                InternalCommandList().ClearDepthStencilView(DepthStencilView, ClearFlags, Depth, Stencil, static_cast<UINT>(rects.size()), rects.data());
            }

            void ClearRenderTargetView(
                D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView,
                const std::array<float, 4>& colorRGBA,
                std::span<D3D12_RECT> rects)
            {
                InternalCommandList().ClearRenderTargetView(RenderTargetView, colorRGBA.data(), static_cast<UINT>(rects.size()), rects.data());
            }

            void ClearUnorderedAccessViewUint(
                D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
                D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
                ID3D12Resource* pResource,
                const std::array<UINT, 4>& values,
                std::span<D3D12_RECT> rects)
            {
                InternalCommandList().ClearUnorderedAccessViewUint(ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource, values.data(), static_cast<UINT>(rects.size()), rects.data());
            }

            void ClearUnorderedAccessViewFloat(
                D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
                D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
                ID3D12Resource* pResource,
                const std::array<float, 4>& values,
                std::span<D3D12_RECT> rects)
            {
                InternalCommandList().ClearUnorderedAccessViewFloat(ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource, values.data(), static_cast<UINT>(rects.size()), rects.data());
            }

            void DiscardResource(
                ID3D12Resource& pResource,
                const D3D12_DISCARD_REGION* pRegion)
            {
                InternalCommandList().DiscardResource(&pResource, pRegion);
            }

            void BeginQuery(
                ID3D12QueryHeap& pQueryHeap,
                D3D12_QUERY_TYPE Type,
                UINT Index)
            {
                InternalCommandList().BeginQuery(&pQueryHeap, Type, Index);
            }

            void EndQuery(
                ID3D12QueryHeap& pQueryHeap,
                D3D12_QUERY_TYPE Type,
                UINT Index)
            {
                InternalCommandList().EndQuery(&pQueryHeap, Type, Index);
            }

            void ResolveQueryData(
                ID3D12QueryHeap& pQueryHeap,
                D3D12_QUERY_TYPE Type,
                UINT StartIndex,
                UINT NumQueries,
                ID3D12Resource& pDestinationBuffer,
                UINT64 AlignedDestinationBufferOffset)
            {
                InternalCommandList().ResolveQueryData(&pQueryHeap, Type, StartIndex, NumQueries, &pDestinationBuffer, AlignedDestinationBufferOffset);
            }

            void SetPredication(
                ID3D12Resource* pBuffer,
                UINT64 AlignedBufferOffset,
                D3D12_PREDICATION_OP Operation)
            {
                InternalCommandList().SetPredication(pBuffer, AlignedBufferOffset, Operation);
            }

            void SetMarker(
                UINT Metadata,
                const void* pData,
                UINT Size)
            {
                InternalCommandList().SetMarker(Metadata, pData, Size);
            }

            void BeginEvent(
                UINT Metadata,
                const void* pData,
                UINT Size)
            {
                InternalCommandList().BeginEvent(Metadata, pData, Size);
            }

            void EndEvent()
            {
                InternalCommandList().EndEvent();
            }

            void ExecuteIndirect(
                ID3D12CommandSignature& commandSignature,
                UINT maxCommandCount,
                ID3D12Resource& argumentBuffer,
                UINT64 argumentBufferOffset,
                ID3D12Resource* optCountBuffer = nullptr,
                UINT64 optCountBufferOffset = 0)
            {
                InternalCommandList().ExecuteIndirect(&commandSignature, maxCommandCount, &argumentBuffer, argumentBufferOffset, optCountBuffer, optCountBufferOffset);
            }

            //In code order                                                          //Alphabetical order
            //using Base::Close;                                                     using Base::BeginEvent;
            //using Base::Reset;                                                     using Base::BeginQuery;
            //using Base::ClearState;                                                using Base::ClearDepthStencilView;
            //using Base::DrawInstanced;                                             using Base::ClearRenderTargetView;
            //using Base::DrawIndexedInstanced;                                      using Base::ClearState;
            //using Base::Dispatch;                                                  using Base::ClearUnorderedAccessViewFloat;
            //using Base::CopyBufferRegion;                                          using Base::ClearUnorderedAccessViewUint;
            //using Base::CopyTextureRegion;                                         using Base::Close;
            //using Base::CopyResource;                                              using Base::CopyBufferRegion;
            //using Base::CopyTiles;                                                 using Base::CopyResource;
            //using Base::ResolveSubresource;                                        using Base::CopyTextureRegion;
            //using Base::IASetPrimitiveTopology;                                    using Base::CopyTiles;
            //using Base::RSSetViewports;                                            using Base::DiscardResource;
            //using Base::RSSetScissorRects;                                         using Base::Dispatch;
            //using Base::OMSetBlendFactor;                                          using Base::DrawIndexedInstanced;
            //using Base::OMSetStencilRef;                                           using Base::DrawInstanced;
            //using Base::SetPipelineState;                                          using Base::EndEvent;
            //using Base::ResourceBarrier;                                           using Base::EndQuery;
            //using Base::ExecuteBundle;                                             using Base::ExecuteBundle;
            //using Base::SetDescriptorHeaps;                                        using Base::ExecuteIndirect;
            //using Base::SetComputeRootSignature;                                   using Base::IASetIndexBuffer;
            //using Base::SetGraphicsRootSignature;                                  using Base::IASetPrimitiveTopology;
            //using Base::SetComputeRootDescriptorTable;                             using Base::IASetVertexBuffers;
            //using Base::SetGraphicsRootDescriptorTable;                            using Base::OMSetBlendFactor;
            //using Base::SetComputeRoot32BitConstant;                               using Base::OMSetRenderTargets;
            //using Base::SetGraphicsRoot32BitConstant;                              using Base::OMSetStencilRef;
            //using Base::SetComputeRoot32BitConstants;                              using Base::Reset;
            //using Base::SetGraphicsRoot32BitConstants;                             using Base::ResolveQueryData;
            //using Base::SetComputeRootConstantBufferView;                          using Base::ResolveSubresource;
            //using Base::SetGraphicsRootConstantBufferView;                         using Base::ResourceBarrier;
            //using Base::SetComputeRootShaderResourceView;                          using Base::RSSetScissorRects;
            //using Base::SetGraphicsRootShaderResourceView;                         using Base::RSSetViewports;
            //using Base::SetComputeRootUnorderedAccessView;                         using Base::SetComputeRoot32BitConstant;
            //using Base::SetGraphicsRootUnorderedAccessView;                        using Base::SetComputeRoot32BitConstants;
            //using Base::IASetIndexBuffer;                                          using Base::SetComputeRootConstantBufferView;
            //using Base::IASetVertexBuffers;                                        using Base::SetComputeRootDescriptorTable;
            //using Base::SOSetTargets;                                              using Base::SetComputeRootShaderResourceView;
            //using Base::OMSetRenderTargets;                                        using Base::SetComputeRootSignature;
            //using Base::ClearDepthStencilView;                                     using Base::SetComputeRootUnorderedAccessView;
            //using Base::ClearRenderTargetView;                                     using Base::SetDescriptorHeaps;
            //using Base::ClearUnorderedAccessViewUint;                              using Base::SetGraphicsRoot32BitConstant;
            //using Base::ClearUnorderedAccessViewFloat;                             using Base::SetGraphicsRoot32BitConstants;
            //using Base::DiscardResource;                                           using Base::SetGraphicsRootConstantBufferView;
            //using Base::BeginQuery;                                                using Base::SetGraphicsRootDescriptorTable;
            //using Base::EndQuery;                                                  using Base::SetGraphicsRootShaderResourceView;
            //using Base::ResolveQueryData;                                          using Base::SetGraphicsRootSignature;
            //using Base::SetPredication;                                            using Base::SetGraphicsRootUnorderedAccessView;
            //using Base::SetMarker;                                                 using Base::SetMarker;
            //using Base::BeginEvent;                                                using Base::SetPipelineState;
            //using Base::EndEvent;                                                  using Base::SetPredication;
            //using Base::ExecuteIndirect;                                           using Base::SOSetTargets;

        private:
            list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList> : private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
            using Base::Close;
            using Base::Dispatch;
            using Base::DrawIndexedInstanced;
            using Base::DrawInstanced;
            using Base::ExecuteIndirect;
            using Base::IASetIndexBuffer;
            using Base::IASetPrimitiveTopology;
            using Base::IASetVertexBuffers;
            using Base::OMSetBlendFactor;
            using Base::OMSetStencilRef;
            using Base::Reset;
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
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList> : private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
            using Base::Close;
            using Base::CopyBufferRegion;
            using Base::CopyResource;
            using Base::CopyTextureRegion;
            using Base::CopyTiles;
            using Base::DiscardResource;
            using Base::Dispatch;
            using Base::DrawIndexedInstanced;
            using Base::DrawInstanced;
            using Base::EndQuery;
            using Base::Reset;
            using Base::ResolveQueryData;
            using Base::ResourceBarrier;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList> : private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;
        public:
            using Base::ClearState;
            using Base::ClearUnorderedAccessViewFloat;
            using Base::ClearUnorderedAccessViewUint;
            using Base::Close;
            using Base::CopyBufferRegion;
            using Base::CopyResource;
            using Base::CopyTextureRegion;
            using Base::CopyTiles;
            using Base::DiscardResource;
            using Base::Dispatch;
            using Base::EndQuery;
            using Base::ExecuteIndirect;
            using Base::Reset;
            using Base::ResolveQueryData;
            using Base::ResourceBarrier;
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
            using Base::SetPipelineState;
            using Base::SetPredication;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList> : public ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>;

        };

        template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
        class CommandList : public ComWrapper<ListTy>, public interface_type<Type, ListTy>
        {
        public:
            using trait_value_type = command_list_trait<Type, ListTy>;
            using list_value_type = typename trait_value_type::list_value_type;
            using allocator_value_type = typename trait_value_type::allocator_value_type;
            static constexpr D3D12_COMMAND_LIST_TYPE value = trait_value_type::value;

        public:
            using ComWrapper<ListTy>::ComWrapper;
        };

        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE type>
        void ListInterface<WrapperTy, type, ID3D12GraphicsCommandList>::ExecuteBundle(Bundle<ID3D12GraphicsCommandList> pCommandList)
        {
            InternalCommandList().ExecuteBundle(pCommandList.Get());
        }
    }

    template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
    using CommandList_t = Internal::CommandList<Type, ListTy>;

    using Direct = Internal::Direct<ID3D12GraphicsCommandList>;
    //using Direct1 = Internal::Direct<ID3D12GraphicsCommandList1>;
    //using Direct2 = Internal::Direct<ID3D12GraphicsCommandList2>;
    //using Direct3 = Internal::Direct<ID3D12GraphicsCommandList3>;
    //using Direct4 = Internal::Direct<ID3D12GraphicsCommandList4>;
    //using Direct5 = Internal::Direct<ID3D12GraphicsCommandList5>;
    //using Direct6 = Internal::Direct<ID3D12GraphicsCommandList6>;

    using Bundle = Internal::Bundle<ID3D12GraphicsCommandList>;
    //using Bundle1 = Internal::Bundle<ID3D12GraphicsCommandList1>;
    //using Bundle2 = Internal::Bundle<ID3D12GraphicsCommandList2>;
    //using Bundle3 = Internal::Bundle<ID3D12GraphicsCommandList3>;
    //using Bundle4 = Internal::Bundle<ID3D12GraphicsCommandList4>;
    //using Bundle5 = Internal::Bundle<ID3D12GraphicsCommandList5>;
    //using Bundle6 = Internal::Bundle<ID3D12GraphicsCommandList6>;

    using Compute = Internal::Compute<ID3D12GraphicsCommandList>;
    //using Compute1 = Internal::Compute<ID3D12GraphicsCommandList1>;
    //using Compute2 = Internal::Compute<ID3D12GraphicsCommandList2>;
    //using Compute3 = Internal::Compute<ID3D12GraphicsCommandList3>;
    //using Compute4 = Internal::Compute<ID3D12GraphicsCommandList4>;
    //using Compute5 = Internal::Compute<ID3D12GraphicsCommandList5>;
    //using Compute6 = Internal::Compute<ID3D12GraphicsCommandList6>;

    using Copy = Internal::Copy<ID3D12GraphicsCommandList>;
    //using Copy1 = Internal::Copy<ID3D12GraphicsCommandList1>;
    //using Copy2 = Internal::Copy<ID3D12GraphicsCommandList2>;
    //using Copy3 = Internal::Copy<ID3D12GraphicsCommandList3>;
    //using Copy4 = Internal::Copy<ID3D12GraphicsCommandList4>;
    //using Copy5 = Internal::Copy<ID3D12GraphicsCommandList5>;
    //using Copy6 = Internal::Copy<ID3D12GraphicsCommandList6>;
};

#pragma warning(pop)
