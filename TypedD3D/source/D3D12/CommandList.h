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
#include "../Helpers/COMHelpers.h"
#include <memory>
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
                ID3D12Resource& pDstBuffer,
                UINT64 DstOffset,
                ID3D12Resource& pSrcBuffer,
                UINT64 SrcOffset,
                UINT64 NumBytes)
            {
                InternalCommandList().CopyBufferRegion(
                    &pDstBuffer,
                    DstOffset,
                    &pSrcBuffer,
                    SrcOffset,
                    NumBytes);
            }

            void CopyTextureRegion(
                const D3D12_TEXTURE_COPY_LOCATION& pDst,
                UINT DstX,
                UINT DstY,
                UINT DstZ,
                const D3D12_TEXTURE_COPY_LOCATION& pSrc,
                const D3D12_BOX* pSrcBox)
            {
                InternalCommandList().CopyTextureRegion(
                    &pDst,
                    DstX,
                    DstY,
                    DstZ,
                    &pSrc,
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
                std::span<D3D12_RECT> rects)
            {
                InternalCommandList().RSSetScissorRects(static_cast<UINT>(rects.size()), rects.data());
            }

            void OMSetBlendFactor(std::span<const float, 4> BlendFactor)
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

            void ResourceBarrier(std::span<const D3D12_RESOURCE_BARRIER> Barriers)
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
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {
                InternalCommandList().SetComputeRootConstantBufferView(RootParameterIndex, BufferLocation);
            }

            void SetGraphicsRootConstantBufferView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {
                InternalCommandList().SetGraphicsRootConstantBufferView(RootParameterIndex, BufferLocation);
            }

            void SetComputeRootShaderResourceView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {
                InternalCommandList().SetComputeRootShaderResourceView(RootParameterIndex, BufferLocation);
            }

            void SetGraphicsRootShaderResourceView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {
                InternalCommandList().SetGraphicsRootShaderResourceView(RootParameterIndex, BufferLocation);
            }

            void SetComputeRootUnorderedAccessView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {
                InternalCommandList().SetComputeRootUnorderedAccessView(RootParameterIndex, BufferLocation);
            }

            void SetGraphicsRootUnorderedAccessView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {
                InternalCommandList().SetGraphicsRootUnorderedAccessView(RootParameterIndex, BufferLocation);
            }

            void IASetIndexBuffer(
                const D3D12_INDEX_BUFFER_VIEW* pView)
            {
                InternalCommandList().IASetIndexBuffer(pView);
            }

            void IASetVertexBuffers(
                UINT StartSlot,
                std::span<const D3D12_VERTEX_BUFFER_VIEW> views)
            {
                InternalCommandList().IASetVertexBuffers(StartSlot, static_cast<UINT>(views.size()), views.data());
            }

            void SOSetTargets(
                UINT StartSlot,
                std::span<const D3D12_STREAM_OUTPUT_BUFFER_VIEW> views)
            {
                InternalCommandList().SOSetTargets(StartSlot, static_cast<UINT>(views.size()), views.data());
            }

            void OMSetRenderTargets(
                std::span<const DescriptorHandle::CPU_RTV> pRenderTargetDescriptors,
                BOOL RTsSingleHandleToDescriptorRange,
                const DescriptorHandle::CPU_DSV* pDepthStencilDescriptor)
            {
                std::unique_ptr<D3D12_CPU_DESCRIPTOR_HANDLE[]> renderTargets = std::make_unique<D3D12_CPU_DESCRIPTOR_HANDLE[]>(pRenderTargetDescriptors.size());

                for(size_t i = 0; i < pRenderTargetDescriptors.size(); i++)
                {
                    renderTargets[i] = pRenderTargetDescriptors[i].Data();
                }

                const D3D12_CPU_DESCRIPTOR_HANDLE* depthStencil = (pDepthStencilDescriptor) ? &pDepthStencilDescriptor->Data() : nullptr;

                InternalCommandList().OMSetRenderTargets(static_cast<UINT>(pRenderTargetDescriptors.size()), renderTargets.get(), RTsSingleHandleToDescriptorRange, depthStencil);
            }

            void ClearDepthStencilView(
                DescriptorHandle::CPU_DSV DepthStencilView,
                D3D12_CLEAR_FLAGS ClearFlags,
                FLOAT Depth,
                UINT8 Stencil,
                std::span<const D3D12_RECT> rects)
            {
                InternalCommandList().ClearDepthStencilView(DepthStencilView.Data(), ClearFlags, Depth, Stencil, static_cast<UINT>(rects.size()), rects.data());
            }

            void ClearRenderTargetView(
                DescriptorHandle::CPU_RTV RenderTargetView,
                std::span<const float, 4> colorRGBA,
                std::span<const D3D12_RECT> rects)
            {
                InternalCommandList().ClearRenderTargetView(RenderTargetView.Data(), colorRGBA.data(), static_cast<UINT>(rects.size()), rects.data());
            }

            void ClearUnorderedAccessViewUint(
                DescriptorHandle::GPU_CBV_SRV_UAV ViewGPUHandleInCurrentHeap,
                DescriptorHandle::CPU_CBV_SRV_UAV ViewCPUHandle,
                ID3D12Resource& pResource,
                std::span<const UINT, 4> values,
                std::span<const D3D12_RECT> rects)
            {
                InternalCommandList().ClearUnorderedAccessViewUint(ViewGPUHandleInCurrentHeap.Data(), ViewCPUHandle.Data(), &pResource, values.data(), static_cast<UINT>(rects.size()), rects.data());
            }

            void ClearUnorderedAccessViewFloat(
                DescriptorHandle::GPU_CBV_SRV_UAV ViewGPUHandleInCurrentHeap,
                DescriptorHandle::CPU_CBV_SRV_UAV ViewCPUHandle,
                ID3D12Resource& pResource,
                std::span<const float, 4> values,
                std::span<const D3D12_RECT> rects)
            {
                InternalCommandList().ClearUnorderedAccessViewFloat(ViewGPUHandleInCurrentHeap.Data(), ViewCPUHandle.Data(), &pResource, values.data(), static_cast<UINT>(rects.size()), rects.data());
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
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList>
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
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList>
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
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList>
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
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList> :
            public ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>;

        };



        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
        class ListInterface<WrapperTy, Type, ID3D12GraphicsCommandList1>
        {
        private:
            using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList1>;
            using list_value_type = typename trait_value_type::list_value_type;
            using allocator_value_type = typename trait_value_type::allocator_value_type;
            using wrapper_type = WrapperTy;

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
                InternalCommandList().AtomicCopyBufferUINT(
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
                InternalCommandList().AtomicCopyBufferUINT64(
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
                InternalCommandList().OMSetDepthBounds(Min, Max);
            }

            void SetSamplePositions(
                UINT NumSamplesPerPixel,
                UINT NumPixels,
                D3D12_SAMPLE_POSITION* pSamplePositions)
            {
                InternalCommandList().SetSamplePositions(
                    NumSamplesPerPixel,
                    NumPixels,
                    pSamplePositions);
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
                InternalCommandList().ResolveSubresourceRegion(
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

            void SetViewInstanceMask(
                UINT Mask)
            {
                InternalCommandList().SetViewInstanceMask(Mask);
            }


            //In code order                                                          //Alphabetical order
            //using Base::AtomicCopyBufferUINT;                                      using Base::AtomicCopyBufferUINT;
            //using Base::AtomicCopyBufferUINT64;                                    using Base::AtomicCopyBufferUINT64;
            //using Base::OMSetDepthBounds;                                          using Base::OMSetDepthBounds;
            //using Base::SetSamplePositions;                                        using Base::ResolveSubresourceRegion;
            //using Base::ResolveSubresourceRegion;                                  using Base::SetSamplePositions;
            //using Base::SetViewInstanceMask;                                       using Base::SetViewInstanceMask;     
        private:
            list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList1> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList1>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList1>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
            using Base::OMSetDepthBounds;
            using Base::SetSamplePositions;
            using Base::SetViewInstanceMask;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList1> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList1>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList1>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
            using Base::AtomicCopyBufferUINT;
            using Base::AtomicCopyBufferUINT64;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList1> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList1>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList1>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;
        public:
            using Base::AtomicCopyBufferUINT;
            using Base::AtomicCopyBufferUINT64;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1> :
            public ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1>;
        };



        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
        class ListInterface<WrapperTy, Type, ID3D12GraphicsCommandList2>
        {
        private:
            using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList2>;
            using list_value_type = typename trait_value_type::list_value_type;
            using allocator_value_type = typename trait_value_type::allocator_value_type;
            using wrapper_type = WrapperTy;

        public:
            void WriteBufferImmediate(
                std::span<D3D12_WRITEBUFFERIMMEDIATE_PARAMETER> pParams,
                std::span<D3D12_WRITEBUFFERIMMEDIATE_MODE> pModes = {})
            {
                assert(pParams.size() == pModes.size() || pModes.size() == 0);
                InternalCommandList().WriteBufferImmediate(static_cast<UINT>(pParams.size()), pParams.data(), pModes.data());
            }


            //In code order                                                          //Alphabetical order
            //using Base::WriteBufferImmediate;                                      using Base::WriteBufferImmediate;
        private:
            list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList2> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList2>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList1>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList2>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
            using Base::WriteBufferImmediate;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList2> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList2>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList1>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList2>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
            using Base::WriteBufferImmediate;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList2> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList2>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList1>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList2>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;
        public:
            using Base::WriteBufferImmediate;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2> :
            public ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2>;
        };



        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
        class ListInterface<WrapperTy, Type, ID3D12GraphicsCommandList3>
        {
        private:
            using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList3>;
            using list_value_type = typename trait_value_type::list_value_type;
            using allocator_value_type = typename trait_value_type::allocator_value_type;
            using wrapper_type = WrapperTy;

        public:
            void SetProtectedResourceSession(
                ID3D12ProtectedResourceSession* pProtectedResourceSession)
            {
                InternalCommandList().SetProtectedResourceSession(pProtectedResourceSession);
            }


            //In code order                                                          //Alphabetical order
            //using Base::SetProtectedResourceSession;                               using Base::SetProtectedResourceSession;
        private:
            list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList3> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList3>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList2>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList3>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList3> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList3>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList2>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList3>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
            using Base::SetProtectedResourceSession;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList3> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList3>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList2>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList3>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;
        public:
            using Base::SetProtectedResourceSession;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3> :
            public ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3>;
        };



        template<class WrapperTy, class ListTy>
        class RenderPassInterface;

        template<class ListTy>
        class RenderPass;

        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
        class ListInterface<WrapperTy, Type, ID3D12GraphicsCommandList4>
        {
        private:
            using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList4>;
            using list_value_type = typename trait_value_type::list_value_type;
            using allocator_value_type = typename trait_value_type::allocator_value_type;
            using wrapper_type = WrapperTy;

        public:
            RenderPass<ID3D12GraphicsCommandList4> BeginRenderPass(
                std::span<const D3D12_RENDER_PASS_RENDER_TARGET_DESC> renderTargets,
                const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* pDepthStencil,
                D3D12_RENDER_PASS_FLAGS Flags);

            void EndRenderPass()
            {
                InternalCommandList().EndRenderPass();
            }

            void InitializeMetaCommand(
                ID3D12MetaCommand& pMetaCommand,
                const void* pInitializationParametersData,
                SIZE_T InitializationParametersDataSizeInBytes)
            {
                InternalCommandList().InitializeMetaCommand(&pMetaCommand, pInitializationParametersData, InitializationParametersDataSizeInBytes);
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

            void ExecuteMetaCommand(
                ID3D12MetaCommand& pMetaCommand,
                const void* pExecutionParametersData,
                SIZE_T ExecutionParametersDataSizeInBytes)
            {
                InternalCommandList().ExecuteMetaCommand(&pMetaCommand, pExecutionParametersData, ExecutionParametersDataSizeInBytes);
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

            void BuildRaytracingAccelerationStructure(
                const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& pDesc,
                std::span<const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC> pPostbuildInfoDescs)
            {
                InternalCommandList().BuildRaytracingAccelerationStructure(
                    &pDesc,
                    static_cast<UINT>(pPostbuildInfoDescs.size()),
                    pPostbuildInfoDescs.data());
            }

            void EmitRaytracingAccelerationStructurePostbuildInfo(
                const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC& pDesc,
                std::span<const D3D12_GPU_VIRTUAL_ADDRESS> pSourceAccelerationStructureData)
            {
                InternalCommandList().EmitRaytracingAccelerationStructurePostbuildInfo(
                    &pDesc,
                    static_cast<UINT>(pSourceAccelerationStructureData.size()),
                    pSourceAccelerationStructureData.data());
            }

            void CopyRaytracingAccelerationStructure(
                D3D12_GPU_VIRTUAL_ADDRESS DestAccelerationStructureData,
                D3D12_GPU_VIRTUAL_ADDRESS SourceAccelerationStructureData,
                D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE Mode)
            {
                InternalCommandList().CopyRaytracingAccelerationStructure(
                    DestAccelerationStructureData,
                    SourceAccelerationStructureData,
                    Mode);
            }

            void SetPipelineState1(
                ID3D12StateObject* pStateObject)
            {
                InternalCommandList().SetPipelineState1(pStateObject);
            }

            void DispatchRays(
                const D3D12_DISPATCH_RAYS_DESC& pDesc)
            {
                InternalCommandList().DispatchRays(&pDesc);
            }


            //In code order                                                          //Alphabetical order
            //using Base::BeginRenderPass;                                           using Base::BeginRenderPass;
            //using Base::EndRenderPass;                                             using Base::BuildRaytracingAccelerationStructure;
            //using Base::InitializeMetaCommand;                                     using Base::CopyRaytracingAccelerationStructure;
            //using Base::ExecuteMetaCommand;                                        using Base::DispatchRays;
            //using Base::BuildRaytracingAccelerationStructure;                      using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
            //using Base::EmitRaytracingAccelerationStructurePostbuildInfo;          using Base::EndRenderPass;
            //using Base::CopyRaytracingAccelerationStructure;                       using Base::ExecuteMetaCommand;
            //using Base::SetPipelineState1;                                         using Base::InitializeMetaCommand;
            //using Base::DispatchRays;                                              using Base::SetPipelineState1;
        private:
            list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList4> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList4>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList3>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList4>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
            using Base::DispatchRays;
            using Base::SetPipelineState1;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList4> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList4>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList3>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList4>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList4> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList4>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList3>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList4>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;
        public:
            using Base::BuildRaytracingAccelerationStructure;
            using Base::CopyRaytracingAccelerationStructure;
            using Base::DispatchRays;
            using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
            using Base::ExecuteMetaCommand;
            using Base::InitializeMetaCommand;
            using Base::SetPipelineState1;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4> :
            public ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4>;
        };

        template<class WrapperTy>
        class RenderPassInterface<WrapperTy, ID3D12GraphicsCommandList4> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>,
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1>,
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2>,
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3>,
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4>
        {
            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE type, class ListTy>
            friend class ListInterface;

            using Base =  ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>;
            using Base1 = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1>;
            using Base2 = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2>;
            using Base3 = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3>;
            using Base4 = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4>;

        public:
            using Base::BeginQuery;
            using Base::Close;
            using Base::DrawIndexedInstanced;
            using Base::DrawInstanced;
            using Base::EndEvent;
            using Base::EndQuery;
            using Base::ExecuteBundle;
            using Base::ExecuteIndirect;
            using Base::IASetIndexBuffer;
            using Base::IASetPrimitiveTopology;
            using Base::IASetVertexBuffers;
            using Base::OMSetBlendFactor;
            using Base::OMSetStencilRef;
            using Base::Reset;
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

            using Base1::OMSetDepthBounds;
            using Base1::SetSamplePositions;
            using Base1::SetViewInstanceMask;

            using Base2::WriteBufferImmediate;

            using Base4::EndRenderPass;
        };



        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
        class ListInterface<WrapperTy, Type, ID3D12GraphicsCommandList5>
        {
        private:
            using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList5>;
            using list_value_type = typename trait_value_type::list_value_type;
            using allocator_value_type = typename trait_value_type::allocator_value_type;
            using wrapper_type = WrapperTy;

        public:
            void RSSetShadingRate(
                D3D12_SHADING_RATE baseShadingRate,
                const D3D12_SHADING_RATE_COMBINER* combiners)
            {
                InternalCommandList().RSSetShadingRate(
                    baseShadingRate,
                    combiners);
            }

            void RSSetShadingRateImage(
                ID3D12Resource* shadingRateImage)
            {
                InternalCommandList().RSSetShadingRateImage(
                    shadingRateImage);
            }

        public:

            //In code order                                                          //Alphabetical order
            //using Base::RSSetShadingRate;                                          using Base::RSSetShadingRate;
            //using Base::RSSetShadingRateImage;                                     using Base::RSSetShadingRateImage;
        private:
            list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList5> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList5>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList4>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList5>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
            using Base::RSSetShadingRate;
            using Base::RSSetShadingRateImage;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList5> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList5>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList4>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList5>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList5> :
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList5>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList4>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList5>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;
        public:

        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList5> :
            public ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList5>,
            public PublicListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4>
        {
            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList5>;
        };

        template<class WrapperTy>
        class RenderPassInterface<WrapperTy, ID3D12GraphicsCommandList5> :
            public RenderPassInterface<WrapperTy, ID3D12GraphicsCommandList4>,
            private ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList5>
        {
            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE type, class ListTy>
            friend class ListInterface;

            using Base = ListInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList5>;
        public:
            using Base::RSSetShadingRate;
            using Base::RSSetShadingRateImage;
        };



        template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
        class CommandList : public ComWrapper<ListTy>, private interface_type<Type, ListTy>
        {
        private:
            template<class WrapperT, D3D12_COMMAND_LIST_TYPE Type2, class List>
            friend class ListInterface;

        public:
            using trait_value_type = command_list_trait<Type, ListTy>;
            using list_value_type = typename trait_value_type::list_value_type;
            using allocator_value_type = typename trait_value_type::allocator_value_type;
            static constexpr D3D12_COMMAND_LIST_TYPE value = trait_value_type::value;

        public:
            using ComWrapper<ListTy>::ComWrapper;

        public:
            template<class DerivedListTy>
            CommandList(const CommandList<Type, DerivedListTy>& other) :
                ComWrapper<ListTy>::ComWrapper(other.GetComPtr())
            {

            }

        public:
            interface_type<Type, ListTy>* GetInterface() { return this; }
            interface_type<Type, ListTy>* operator->() { return this; }

            template<class Ty>
            Ty As()
            {
                if constexpr(Ty::value == value)
                {
                    return Ty(Helpers::COM::Cast<typename Ty::list_value_type>(ComWrapper<ListTy>::GetComPtr()));
                }
                else
                {
                    return Ty();
                }
            }
        };

        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE type>
        void ListInterface<WrapperTy, type, ID3D12GraphicsCommandList>::ExecuteBundle(Bundle<ID3D12GraphicsCommandList> pCommandList)
        {
            InternalCommandList().ExecuteBundle(pCommandList.Get());
        }

        template<class ListTy>
        class RenderPass : public ComWrapper<ListTy>, private RenderPassInterface<RenderPass<ListTy>, ListTy>
        {
            template<class WrapperTy, class ListTy2>
            friend class RenderPassInterface;

            template<class WrapperT, D3D12_COMMAND_LIST_TYPE Type2, class List>
            friend class ListInterface;

            using internal_interface_type = RenderPassInterface<RenderPass<ListTy>, ListTy>;

        public:
            using trait_value_type = command_list_trait<D3D12_COMMAND_LIST_TYPE_DIRECT, ListTy>;
            using list_value_type = typename trait_value_type::list_value_type;
            using allocator_value_type = typename trait_value_type::allocator_value_type;
            static constexpr D3D12_COMMAND_LIST_TYPE value = trait_value_type::value;

        public:
            using ComWrapper<ListTy>::ComWrapper;

        public:
            internal_interface_type* GetInterface() { return this; }
            internal_interface_type* operator->() { return this; }

            template<class Ty>
            Ty As()
            {
                if constexpr(Ty::value == value)
                {
                    return Ty(Helpers::COM::Cast<typename Ty::list_value_type>(ComWrapper<ListTy>::GetComPtr()));
                }
                else
                {
                    return Ty();
                }
            }
        };

        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
        RenderPass<ID3D12GraphicsCommandList4> ListInterface<WrapperTy, Type, ID3D12GraphicsCommandList4>::BeginRenderPass(std::span<const D3D12_RENDER_PASS_RENDER_TARGET_DESC> renderTargets, const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* pDepthStencil, D3D12_RENDER_PASS_FLAGS Flags)
        {
            InternalCommandList().BeginRenderPass(static_cast<UINT>(renderTargets.size()), renderTargets.data(), pDepthStencil, Flags);
            return RenderPass(static_cast<wrapper_type&>(*this).GetComPtr());
        }
    }

    template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
    using CommandList_t = Internal::CommandList<Type, ListTy>;

    using Direct = Internal::Direct<ID3D12GraphicsCommandList>;
    using Direct1 = Internal::Direct<ID3D12GraphicsCommandList1>;
    using Direct2 = Internal::Direct<ID3D12GraphicsCommandList2>;
    using Direct3 = Internal::Direct<ID3D12GraphicsCommandList3>;
    using Direct4 = Internal::Direct<ID3D12GraphicsCommandList4>;
    using Direct5 = Internal::Direct<ID3D12GraphicsCommandList5>;
    //using Direct6 = Internal::Direct<ID3D12GraphicsCommandList6>;

    using Bundle = Internal::Bundle<ID3D12GraphicsCommandList>;
    using Bundle1 = Internal::Bundle<ID3D12GraphicsCommandList1>;
    using Bundle2 = Internal::Bundle<ID3D12GraphicsCommandList2>;
    using Bundle3 = Internal::Bundle<ID3D12GraphicsCommandList3>;
    using Bundle4 = Internal::Bundle<ID3D12GraphicsCommandList4>;
    using Bundle5 = Internal::Bundle<ID3D12GraphicsCommandList5>;
    //using Bundle6 = Internal::Bundle<ID3D12GraphicsCommandList6>;

    using Compute = Internal::Compute<ID3D12GraphicsCommandList>;
    using Compute1 = Internal::Compute<ID3D12GraphicsCommandList1>;
    using Compute2 = Internal::Compute<ID3D12GraphicsCommandList2>;
    using Compute3 = Internal::Compute<ID3D12GraphicsCommandList3>;
    using Compute4 = Internal::Compute<ID3D12GraphicsCommandList4>;
    using Compute5 = Internal::Compute<ID3D12GraphicsCommandList5>;
    //using Compute6 = Internal::Compute<ID3D12GraphicsCommandList6>;

    using Copy = Internal::Copy<ID3D12GraphicsCommandList>;
    using Copy1 = Internal::Copy<ID3D12GraphicsCommandList1>;
    using Copy2 = Internal::Copy<ID3D12GraphicsCommandList2>;
    using Copy3 = Internal::Copy<ID3D12GraphicsCommandList3>;
    using Copy4 = Internal::Copy<ID3D12GraphicsCommandList4>;
    using Copy5 = Internal::Copy<ID3D12GraphicsCommandList5>;
    //using Copy6 = Internal::Copy<ID3D12GraphicsCommandList6>;

    using RenderPass = Internal::RenderPass<ID3D12GraphicsCommandList4>;
    using RenderPass1 = Internal::RenderPass<ID3D12GraphicsCommandList5>;
};

#pragma warning(pop)
