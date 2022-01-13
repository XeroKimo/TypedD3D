//*********************************************************
//
// Copyright (c) 2022 Renzy Alarcon
// Licensed under the MIT License (MIT).
//
//*********************************************************
#pragma once
#include "CommandAllocator.h"
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
        template<class WrapperTy, class ListTy, template<class ListTy> class Tag>
        class ListInterface;

        template<class WrapperTy, class ListTy, class Type_Tag>
        class PublicListInterface;

        template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
        class CommandList;

        template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
        struct command_list_tag : Meta::command_list_type_tag<Type>
        {
            using tag = command_list_tag<Type, ListTy>;
            using type_tag = Meta::command_list_type_tag<Type>;

            using list_type = ListTy;
            using allocator_type = CommandAllocator::Internal::CommandAllocator<Type>;
            
            template<class WrapperTy>
            using interface_type = PublicListInterface<WrapperTy, ListTy, type_tag>;
        };

        template<class ListTy>
        using bundle_command_list_tag = command_list_tag<D3D12_COMMAND_LIST_TYPE_BUNDLE, ListTy>;

        template<class ListTy>
        using direct_command_list_tag = command_list_tag<D3D12_COMMAND_LIST_TYPE_DIRECT, ListTy>;

        template<class ListTy>
        using copy_command_list_tag = command_list_tag<D3D12_COMMAND_LIST_TYPE_COPY, ListTy>;

        template<class ListTy>
        using compute_command_list_tag = command_list_tag<D3D12_COMMAND_LIST_TYPE_COMPUTE, ListTy>;

        template<class Tag, class WrapperTy>
        using interface_type = typename Tag::template interface_type<WrapperTy>;

        template<class ListTy>
        using Bundle = CommandList<D3D12_COMMAND_LIST_TYPE_BUNDLE, ListTy>;

        template<class ListTy>
        using Direct = CommandList<D3D12_COMMAND_LIST_TYPE_DIRECT, ListTy>;

        template<class ListTy>
        using Copy = CommandList<D3D12_COMMAND_LIST_TYPE_COPY, ListTy>;

        template<class ListTy>
        using Compute = CommandList<D3D12_COMMAND_LIST_TYPE_COMPUTE, ListTy>;

        template<class WrapperTy, template<class ListTy> class Tag>
        class ListInterface<WrapperTy, ID3D12GraphicsCommandList, Tag>
        {
        private:
            using list_type = ID3D12GraphicsCommandList;
            using tag_type = Tag<list_type>;
            using allocator_type = typename tag_type::allocator_type;

        public:
            HRESULT Close()
            {
                return InternalCommandList().Close();
            }

            HRESULT Reset(
                allocator_type pAllocator,
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
                ID3D12Resource* pDstResource,
                ID3D12Resource* pSrcResource)
            {
                InternalCommandList().CopyResource(pDstResource, pSrcResource);
            }

            void CopyTiles(
                ID3D12Resource* pTiledResource,
                const D3D12_TILED_RESOURCE_COORDINATE* pTileRegionStartCoordinate,
                const D3D12_TILE_REGION_SIZE* pTileRegionSize,
                ID3D12Resource* pBuffer,
                UINT64 BufferStartOffsetInBytes,
                D3D12_TILE_COPY_FLAGS Flags)
            {
                InternalCommandList().CopyTiles(
                    pTiledResource,
                    pTileRegionStartCoordinate,
                    pTileRegionSize,
                    pBuffer,
                    BufferStartOffsetInBytes,
                    Flags);
            }

            void ResolveSubresource(
                ID3D12Resource* pDstResource,
                UINT DstSubresource,
                ID3D12Resource* pSrcResource,
                UINT SrcSubresource,
                DXGI_FORMAT Format)
            {
                InternalCommandList().ResolveSubresource(
                    pDstResource,
                    DstSubresource,
                    pSrcResource,
                    SrcSubresource,
                    Format);
            }

            void IASetPrimitiveTopology(
                D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology)
            {
                InternalCommandList().IASetPrimitiveTopology(PrimitiveTopology);
            }

            void RSSetViewports(
                UINT NumViewports,
                const D3D12_VIEWPORT* pViewports)
            {
                InternalCommandList().RSSetViewports(NumViewports, pViewports);
            }

            void RSSetScissorRects(
                UINT NumRects,
                const D3D12_RECT* pRects)
            {
                InternalCommandList().RSSetScissorRects(NumRects, pRects);
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

            void SetDescriptorHeaps(const std::span<ID3D12DescriptorHeap*> descriptorHeaps)
            {
                InternalCommandList().SetDescriptorHeaps(static_cast<UINT>(descriptorHeaps.size()), descriptorHeaps.data());
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
                ID3D12Resource* pResource,
                const D3D12_DISCARD_REGION* pRegion)
            {
                InternalCommandList().DiscardResource(pResource, pRegion);
            }

            void BeginQuery(
                ID3D12QueryHeap* pQueryHeap,
                D3D12_QUERY_TYPE Type,
                UINT Index)
            {
                InternalCommandList().BeginQuery(pQueryHeap, Type, Index);
            }

            void EndQuery(
                ID3D12QueryHeap* pQueryHeap,
                D3D12_QUERY_TYPE Type,
                UINT Index)
            {
                InternalCommandList().EndQuery(pQueryHeap, Type, Index);
            }

            void ResolveQueryData(
                ID3D12QueryHeap* pQueryHeap,
                D3D12_QUERY_TYPE Type,
                UINT StartIndex,
                UINT NumQueries,
                ID3D12Resource* pDestinationBuffer,
                UINT64 AlignedDestinationBufferOffset)
            {
                InternalCommandList().ResolveQueryData(pQueryHeap, Type, StartIndex, NumQueries, pDestinationBuffer, AlignedDestinationBufferOffset);
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

        private:
            list_type& InternalCommandList() { return *static_cast<WrapperTy&>(*this).Get(); }
        };


        template<class WrapperTy>
        class PublicListInterface<WrapperTy, ID3D12GraphicsCommandList, Meta::bundle_command_list_type_tag> : private ListInterface<WrapperTy, ID3D12GraphicsCommandList, bundle_command_list_tag>
        //template<class WrapperTy>
        //class BundleInterface<WrapperTy, ID3D12GraphicsCommandList> : private ListInterface<WrapperTy, bundle_command_list_tag, ID3D12GraphicsCommandList>
        {
            using Base = ListInterface<WrapperTy, ID3D12GraphicsCommandList, bundle_command_list_tag>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;

        public:
            using Base::Close;
            using Base::Reset;
            using Base::DrawInstanced;
            using Base::DrawIndexedInstanced;
            using Base::Dispatch;
            using Base::IASetPrimitiveTopology;
            using Base::OMSetBlendFactor;
            using Base::OMSetStencilRef;
            using Base::SetPipelineState;
            using Base::SetDescriptorHeaps;
            using Base::SetComputeRootSignature;
            using Base::SetGraphicsRootSignature;
            using Base::SetComputeRootDescriptorTable;
            using Base::SetGraphicsRootDescriptorTable;
            using Base::SetComputeRoot32BitConstant;
            using Base::SetGraphicsRoot32BitConstant;
            using Base::SetComputeRoot32BitConstants;
            using Base::SetGraphicsRoot32BitConstants;
            using Base::SetComputeRootConstantBufferView;
            using Base::SetGraphicsRootConstantBufferView;
            using Base::SetComputeRootShaderResourceView;
            using Base::SetGraphicsRootShaderResourceView;
            using Base::SetComputeRootUnorderedAccessView;
            using Base::SetGraphicsRootUnorderedAccessView;
            using Base::IASetIndexBuffer;
            using Base::IASetVertexBuffers;
            using Base::ResolveQueryData;
            using Base::ExecuteIndirect;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, ID3D12GraphicsCommandList, Meta::copy_command_list_type_tag> : private ListInterface<WrapperTy, ID3D12GraphicsCommandList, copy_command_list_tag>
        {
            using Base = ListInterface<WrapperTy, ID3D12GraphicsCommandList, copy_command_list_tag>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, ID3D12GraphicsCommandList, Meta::compute_command_list_type_tag> : private ListInterface<WrapperTy, ID3D12GraphicsCommandList, compute_command_list_tag>
        {
            using Base = ListInterface<WrapperTy, ID3D12GraphicsCommandList, compute_command_list_tag>;

            //Enables casting to WrapperTy as WrapperTy would not know it inherits from ListInterface
            friend Base;
        };

        template<class WrapperTy>
        class PublicListInterface<WrapperTy, ID3D12GraphicsCommandList, Meta::direct_command_list_type_tag> : public ListInterface<WrapperTy, ID3D12GraphicsCommandList, direct_command_list_tag>
        {
            using Base = ListInterface<WrapperTy, ID3D12GraphicsCommandList, direct_command_list_tag>;

        };

        template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
        class CommandList : public ComWrapper<ListTy>, public command_list_tag<Type, ListTy>::template interface_type<CommandList<Type, ListTy>>
        {
        public:
            using tag = command_list_tag<Type, ListTy>;
            using list_type = typename tag::list_type;
            using allocator_type = typename tag::allocator_type;
            static constexpr D3D12_COMMAND_LIST_TYPE list_enum_type = tag::type;
        };

        template<class WrapperTy, template<class ListTy> class Tag>
        void ListInterface<WrapperTy, ID3D12GraphicsCommandList, Tag>::ExecuteBundle(Bundle<ID3D12GraphicsCommandList> pCommandList)
        {
            InternalCommandList().ExecuteBundle(pCommandList.Get());
        }
    }

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