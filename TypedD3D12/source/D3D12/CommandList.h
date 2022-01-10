//*********************************************************
//
// Copyright (c) 2022 Renzy Alarcon
// Licensed under the MIT License (MIT).
//
//*********************************************************
#pragma once
#include <d3d12.h>


#pragma warning (push)
#pragma warning (disable : 4584)
namespace TypedD3D::D3D12::CommandList
{
    namespace Internal
    {
        template<class WrapperTy, class ListTy>
        class BaseInterface
        {
        private:
            using list_type = ListTy;

        public:

            HRESULT Close()
            {
                InternalCommandList().Close();
            }

            HRESULT Reset(
                ID3D12CommandAllocator* pAllocator,
                ID3D12PipelineState* pInitialState)
            {

            }

            void ClearState(
                ID3D12PipelineState* pPipelineState)
            {

            }

            void DrawInstanced(
                UINT VertexCountPerInstance,
                UINT InstanceCount,
                UINT StartVertexLocation,
                UINT StartInstanceLocation)
            {

            }

            void DrawIndexedInstanced(
                UINT IndexCountPerInstance,
                UINT InstanceCount,
                UINT StartIndexLocation,
                INT BaseVertexLocation,
                UINT StartInstanceLocation)
            {

            }

            void Dispatch(
                UINT ThreadGroupCountX,
                UINT ThreadGroupCountY,
                UINT ThreadGroupCountZ)
            {

            }

            void CopyBufferRegion(
                ID3D12Resource* pDstBuffer,
                UINT64 DstOffset,
                ID3D12Resource* pSrcBuffer,
                UINT64 SrcOffset,
                UINT64 NumBytes)
            {

            }

            void CopyTextureRegion(
                const D3D12_TEXTURE_COPY_LOCATION* pDst,
                UINT DstX,
                UINT DstY,
                UINT DstZ,
                const D3D12_TEXTURE_COPY_LOCATION* pSrc,
                const D3D12_BOX* pSrcBox)
            {

            }

            void CopyResource(
                ID3D12Resource* pDstResource,
                ID3D12Resource* pSrcResource)
            {

            }

            void CopyTiles(
                ID3D12Resource* pTiledResource,
                const D3D12_TILED_RESOURCE_COORDINATE* pTileRegionStartCoordinate,
                const D3D12_TILE_REGION_SIZE* pTileRegionSize,
                ID3D12Resource* pBuffer,
                UINT64 BufferStartOffsetInBytes,
                D3D12_TILE_COPY_FLAGS Flags)
            {

            }

            void ResolveSubresource(
                ID3D12Resource* pDstResource,
                UINT DstSubresource,
                ID3D12Resource* pSrcResource,
                UINT SrcSubresource,
                DXGI_FORMAT Format)
            {

            }

            void IASetPrimitiveTopology(
                D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology)
            {

            }

            void RSSetViewports(
                UINT NumViewports,
                const D3D12_VIEWPORT* pViewports)
            {

            }

            void RSSetScissorRects(
                UINT NumRects,
                const D3D12_RECT* pRects)
            {

            }

            void OMSetBlendFactor(const FLOAT BlendFactor[4])
            {

            }

            void OMSetStencilRef(UINT StencilRef)
            {

            }

            void SetPipelineState(ID3D12PipelineState* pPipelineState)
            {

            }

            void ResourceBarrier(UINT NumBarriers, const D3D12_RESOURCE_BARRIER* pBarriers)
            {

            }

            void ExecuteBundle(ID3D12GraphicsCommandList* pCommandList)
            {

            }

            void SetDescriptorHeaps(UINT NumDescriptorHeaps, ID3D12DescriptorHeap* const* ppDescriptorHeaps)
            {

            }

            void SetComputeRootSignature(ID3D12RootSignature* pRootSignature)
            {

            }

            void SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature)
            {

            }

            void SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
            {

            }

            void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
            {

            }

            void SetComputeRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
            {

            }

            void SetGraphicsRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
            {

            }

            void SetComputeRoot32BitConstants(
                UINT RootParameterIndex,
                UINT Num32BitValuesToSet,
                const void* pSrcData,
                UINT DestOffsetIn32BitValues)
            {

            }

            void SetGraphicsRoot32BitConstants(
                UINT RootParameterIndex,
                UINT Num32BitValuesToSet,
                const void* pSrcData,
                UINT DestOffsetIn32BitValues)
            {

            }

            void SetComputeRootConstantBufferView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {

            }

            void SetGraphicsRootConstantBufferView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {

            }

            void SetComputeRootShaderResourceView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {

            }

            void SetGraphicsRootShaderResourceView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {

            }

            void SetComputeRootUnorderedAccessView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {

            }

            void SetGraphicsRootUnorderedAccessView(
                UINT RootParameterIndex,
                D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
            {

            }

            void IASetIndexBuffer(
                const D3D12_INDEX_BUFFER_VIEW* pView)
            {

            }

            void IASetVertexBuffers(
                UINT StartSlot,
                UINT NumViews,
                const D3D12_VERTEX_BUFFER_VIEW* pViews)
            {

            }

            void SOSetTargets(
                UINT StartSlot,
                UINT NumViews,
                const D3D12_STREAM_OUTPUT_BUFFER_VIEW* pViews)
            {

            }

            void OMSetRenderTargets(
                UINT NumRenderTargetDescriptors,
                const D3D12_CPU_DESCRIPTOR_HANDLE* pRenderTargetDescriptors,
                BOOL RTsSingleHandleToDescriptorRange,
                const D3D12_CPU_DESCRIPTOR_HANDLE* pDepthStencilDescriptor)
            {

            }

            void ClearDepthStencilView(
                D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView,
                D3D12_CLEAR_FLAGS ClearFlags,
                FLOAT Depth,
                UINT8 Stencil,
                UINT NumRects,
                const D3D12_RECT* pRects)
            {

            }

            void ClearRenderTargetView(
                D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView,
                const FLOAT ColorRGBA[4],
                UINT NumRects,
                const D3D12_RECT* pRects)
            {

            }

            void ClearUnorderedAccessViewUint(
                D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
                D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
                ID3D12Resource* pResource,
                const UINT Values[4],
                UINT NumRects,
                const D3D12_RECT* pRects)
            {

            }

            void ClearUnorderedAccessViewFloat(
                D3D12_GPU_DESCRIPTOR_HANDLE ViewGPUHandleInCurrentHeap,
                D3D12_CPU_DESCRIPTOR_HANDLE ViewCPUHandle,
                ID3D12Resource* pResource,
                const FLOAT Values[4],
                UINT NumRects,
                const D3D12_RECT* pRects)
            {

            }

            void DiscardResource(
                ID3D12Resource* pResource,
                const D3D12_DISCARD_REGION* pRegion)
            {

            }

            void BeginQuery(
                ID3D12QueryHeap* pQueryHeap,
                D3D12_QUERY_TYPE Type,
                UINT Index)
            {

            }

            void EndQuery(
                ID3D12QueryHeap* pQueryHeap,
                D3D12_QUERY_TYPE Type,
                UINT Index)
            {

            }

            void ResolveQueryData(
                ID3D12QueryHeap* pQueryHeap,
                D3D12_QUERY_TYPE Type,
                UINT StartIndex,
                UINT NumQueries,
                ID3D12Resource* pDestinationBuffer,
                UINT64 AlignedDestinationBufferOffset)
            {

            }

            void SetPredication(
                ID3D12Resource* pBuffer,
                UINT64 AlignedBufferOffset,
                D3D12_PREDICATION_OP Operation)
            {

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
            list_type& InternalCommandList() { return *static_cast<WrapperTy&>(*this).GetCommandList().Get(); }
        };

        template<class WrapperTy, class ListTy>
        class BundleInterface : private BaseInterface<WrapperTy, ListTy>
        {
            using Base = BaseInterface<WrapperTy, ListTy>;

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

        template<class WrapperTy, class ListTy>
        class CopyInterface : private BaseInterface<WrapperTy, ListTy>
        {

        };

        template<class WrapperTy, class ListTy>
        class ComputeInterface : public CopyInterface<WrapperTy, ListTy>, private BaseInterface<WrapperTy, ListTy>
        {

        };

        template<class WrapperTy, class ListTy>
        class DirectInterface : public BaseInterface<WrapperTy, ListTy>
        {

        };

        template<class ListTy, template<class WrapperTy, class ListTy> class InterfaceTy>
        class CommandList : public InterfaceTy<CommandList<ListTy, InterfaceTy>, ListTy>
        {

        };

        template<class ListTy>
        class Direct : public CommandList<ListTy, DirectInterface>
        {

        };
        template<class ListTy>
        class Bundle : public CommandList<ListTy, BundleInterface>
        {

        };
        template<class ListTy>
        class Compute : public CommandList<ListTy, ComputeInterface>
        {

        };
        template<class ListTy>
        class Copy : public CommandList<ListTy, CopyInterface>
        {

        };
    };

    using Direct = Internal::Direct<ID3D12GraphicsCommandList>;
    using Direct1 = Internal::Direct<ID3D12GraphicsCommandList1>;
    using Direct2 = Internal::Direct<ID3D12GraphicsCommandList2>;
    using Direct3 = Internal::Direct<ID3D12GraphicsCommandList3>;
    using Direct4 = Internal::Direct<ID3D12GraphicsCommandList4>;
    using Direct5 = Internal::Direct<ID3D12GraphicsCommandList5>;
    using Direct6 = Internal::Direct<ID3D12GraphicsCommandList6>;

    using Bundle = Internal::Bundle<ID3D12GraphicsCommandList>;
    using Bundle1 = Internal::Bundle<ID3D12GraphicsCommandList1>;
    using Bundle2 = Internal::Bundle<ID3D12GraphicsCommandList2>;
    using Bundle3 = Internal::Bundle<ID3D12GraphicsCommandList3>;
    using Bundle4 = Internal::Bundle<ID3D12GraphicsCommandList4>;
    using Bundle5 = Internal::Bundle<ID3D12GraphicsCommandList5>;
    using Bundle6 = Internal::Bundle<ID3D12GraphicsCommandList6>;

    using Compute = Internal::Compute<ID3D12GraphicsCommandList>;
    using Compute1 = Internal::Compute<ID3D12GraphicsCommandList1>;
    using Compute2 = Internal::Compute<ID3D12GraphicsCommandList2>;
    using Compute3 = Internal::Compute<ID3D12GraphicsCommandList3>;
    using Compute4 = Internal::Compute<ID3D12GraphicsCommandList4>;
    using Compute5 = Internal::Compute<ID3D12GraphicsCommandList5>;
    using Compute6 = Internal::Compute<ID3D12GraphicsCommandList6>;

    using Copy = Internal::Copy<ID3D12GraphicsCommandList>;
    using Copy1 = Internal::Copy<ID3D12GraphicsCommandList1>;
    using Copy2 = Internal::Copy<ID3D12GraphicsCommandList2>;
    using Copy3 = Internal::Copy<ID3D12GraphicsCommandList3>;
    using Copy4 = Internal::Copy<ID3D12GraphicsCommandList4>;
    using Copy5 = Internal::Copy<ID3D12GraphicsCommandList5>;
    using Copy6 = Internal::Copy<ID3D12GraphicsCommandList6>;
};

#pragma warning(pop)