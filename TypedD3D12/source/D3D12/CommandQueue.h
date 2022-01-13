#pragma once
#include "CommandList.h"
#include <memory>

namespace TypedD3D::D3D12::CommandQueue
{
    namespace Internal
    {
        template<D3D12_COMMAND_LIST_TYPE Type>
        struct command_queue_tag : Meta::command_list_type_tag<Type>
        {
            using list_type = CommandList::Internal::CommandList<Type, ID3D12GraphicsCommandList>;
        };

        template<D3D12_COMMAND_LIST_TYPE Type>
        class CommandQueue : public ComWrapper<ID3D12CommandQueue>
        {
        public:
            using tag = command_queue_tag<Type>;
            using list_type = typename tag::list_type;

        public:
            void UpdateTileMappings(
                ID3D12Resource* pResource,
                UINT NumResourceRegions,
                const D3D12_TILED_RESOURCE_COORDINATE* pResourceRegionStartCoordinates,
                const D3D12_TILE_REGION_SIZE* pResourceRegionSizes,
                ID3D12Heap* pHeap,
                UINT NumRanges,
                const D3D12_TILE_RANGE_FLAGS* pRangeFlags,
                const UINT* pHeapRangeStartOffsets,
                const UINT* pRangeTileCounts,
                D3D12_TILE_MAPPING_FLAGS Flags) { }

            void CopyTileMappings(
                ID3D12Resource* pDstResource,
                const D3D12_TILED_RESOURCE_COORDINATE* pDstRegionStartCoordinate,
                ID3D12Resource* pSrcResource,
                const D3D12_TILED_RESOURCE_COORDINATE* pSrcRegionStartCoordinate,
                const D3D12_TILE_REGION_SIZE* pRegionSize,
                D3D12_TILE_MAPPING_FLAGS Flags) { }

            template<size_t Extents>
            void ExecuteCommandLists(
                std::span<list_type*, Extents> commandLists)
            {
                std::array<ID3D12CommandList*, Extents> submitList;

                for(size_t i = 0; i < commandLists.size(); i++)
                {
                    submitList[i] = commandLists[i]->Get();
                }

                Get()->ExecuteCommandLists(static_cast<UINT>(commandLists.size()), submitList.data());
            }

            template<>
            void ExecuteCommandLists<std::dynamic_extent>(
                std::span<list_type*> commandLists)
            {
                std::unique_ptr<ID3D12CommandList[]> submitList = std::make_unique<ID3D12CommandList[]>(commandLists.size());

                for(size_t i = 0; i < commandLists.size(); i++)
                {
                    submitList[i] = commandLists[i]->Get();
                }

                Get()->ExecuteCommandLists(static_cast<UINT>(commandLists.size()), submitList.get());
            }

            void SetMarker(
                UINT Metadata,
                const void* pData,
                UINT Size) { }

            void BeginEvent(
                UINT Metadata,
                const void* pData,
                UINT Size) { }

            void EndEvent(void) { }

            HRESULT Signal(
                ID3D12Fence* pFence,
                UINT64 Value) { }

            HRESULT Wait(
                ID3D12Fence* pFence,
                UINT64 Value) { }

            HRESULT GetTimestampFrequency(
                UINT64* pFrequency) { }

            HRESULT GetClockCalibration(
                UINT64* pGpuTimestamp,
                UINT64* pCpuTimestamp) { }

            D3D12_COMMAND_QUEUE_DESC GetDesc(void) { }
        };
    };

    using Direct = Internal::CommandQueue<D3D12_COMMAND_LIST_TYPE_DIRECT>;
    using Bundle = Internal::CommandQueue<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
    using Compute = Internal::CommandQueue<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
    using Copy = Internal::CommandQueue<D3D12_COMMAND_LIST_TYPE_COPY>;
}