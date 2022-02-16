#pragma once
#include "CommandList.h"
#include <memory>
#include <array>

namespace TypedD3D::D3D12::CommandQueue
{
    namespace Internal
    {
        struct GPU_TIMESTAMP
        {
            UINT64 value;
        };
        struct CPU_TIMESTAMP
        {
            UINT64 value;
        };

        template<D3D12_COMMAND_LIST_TYPE Type>
        struct command_queue_tag : Meta::command_list_type_tag<Type>
        {
            using list_type = CommandList::Internal::CommandList<Type, ID3D12GraphicsCommandList>;
        };

        template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
        class QueueInterface
        {
            using tag = command_queue_tag<Type>;
            using list_type = typename tag::list_type;
            static constexpr D3D12_COMMAND_LIST_TYPE value = Type;

        public:
            void UpdateTileMappings(
                ID3D12Resource& pResource,
                UINT NumResourceRegions,
                const D3D12_TILED_RESOURCE_COORDINATE* pResourceRegionStartCoordinates,
                const D3D12_TILE_REGION_SIZE* pResourceRegionSizes,
                ID3D12Heap* pHeap,
                UINT NumRanges,
                const D3D12_TILE_RANGE_FLAGS* pRangeFlags,
                const UINT* pHeapRangeStartOffsets,
                const UINT* pRangeTileCounts,
                D3D12_TILE_MAPPING_FLAGS Flags)
            {
                InternalGet()->UpdateTileMappings(
                    &pResource,
                    NumResourceRegions,
                    pResourceRegionStartCoordinates,
                    pResourceRegionSizes,
                    pHeap,
                    NumRanges,
                    pRangeFlags,
                    pHeapRangeStartOffsets,
                    pRangeTileCounts,
                    Flags);
            }

            void CopyTileMappings(
                ID3D12Resource& pDstResource,
                const D3D12_TILED_RESOURCE_COORDINATE& pDstRegionStartCoordinate,
                ID3D12Resource& pSrcResource,
                const D3D12_TILED_RESOURCE_COORDINATE& pSrcRegionStartCoordinate,
                const D3D12_TILE_REGION_SIZE& pRegionSize,
                D3D12_TILE_MAPPING_FLAGS Flags)
            {
                InternalGet()->CopyTileMappings(
                    &pDstResource,
                    &pDstRegionStartCoordinate,
                    &pSrcResource,
                    &pSrcRegionStartCoordinate,
                    &pRegionSize,
                    Flags);
            }

            template<size_t Extents>
            void ExecuteCommandLists(
                std::span<list_type, Extents> commandLists)
            {
                if constexpr(Extents == std::dynamic_extent)
                {
                    std::unique_ptr<ID3D12CommandList* []> submitList = std::make_unique<ID3D12CommandList* []>(commandLists.size());

                    for(size_t i = 0; i < commandLists.size(); i++)
                    {
                        submitList[i] = commandLists[i].Get();
                    }

                    InternalGet()->ExecuteCommandLists(static_cast<UINT>(commandLists.size()), submitList.get());
                }
                else
                {
                    std::array<ID3D12CommandList*, Extents> submitList;

                    for(size_t i = 0; i < commandLists.size(); i++)
                    {
                        submitList[i] = commandLists[i].Get();
                    }

                    InternalGet()->ExecuteCommandLists(static_cast<UINT>(commandLists.size()), submitList.data());
                }
            }

            void SetMarker(
                UINT Metadata,
                const void* pData,
                UINT Size)
            {
                InternalGet()->SetMarker(Metadata, pData, Size);
            }

            void BeginEvent(
                UINT Metadata,
                const void* pData,
                UINT Size)
            {
                InternalGet()->BeginEvent(Metadata, pData, Size);
            }

            void EndEvent() { InternalGet()->EndEvent(); }

            HRESULT Signal(
                ID3D12Fence& Fence,
                UINT64 Value)
            {
                return InternalGet()->Signal(&Fence, Value);
            }

            HRESULT Wait(
                ID3D12Fence& Fence,
                UINT64 Value)
            {
                return InternalGet()->Wait(&Fence, Value);
            }

            UINT64 GetTimestampFrequency()
            {
                UINT64 frequency = 0;
                InternalGet()->GetTimestampFrequency(&frequency);
                return frequency;
            }

            std::pair<GPU_TIMESTAMP, CPU_TIMESTAMP> GetClockCalibration()
            {
                std::pair<GPU_TIMESTAMP, CPU_TIMESTAMP> timeStamps;
                InternalGet()->GetClockCalibration(&timeStamps.first.value, &timeStamps.second.value);
                return timeStamps;
            }

            D3D12_COMMAND_QUEUE_DESC GetDesc() { return InternalGet()->GetDesc(); }

        private:
            ID3D12CommandQueue* InternalGet() { return static_cast<WrapperTy&>(*this).Get(); }
        };
        

        template<D3D12_COMMAND_LIST_TYPE Type>
        class CommandQueue : public ComWrapper<ID3D12CommandQueue>, private QueueInterface<CommandQueue<Type>, Type>
        {
            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type2>
            friend class QueueInterface;

        public:
            using tag = command_queue_tag<Type>;
            using list_type = typename tag::list_type;
            static constexpr D3D12_COMMAND_LIST_TYPE value = Type;

        public:
            using ComWrapper<ID3D12CommandQueue>::ComWrapper;

        public:
            QueueInterface<CommandQueue<Type>, Type>* GetInterrface() { return this; }
            QueueInterface<CommandQueue<Type>, Type>* operator->() { return this; }
        };
    };

    template<D3D12_COMMAND_LIST_TYPE Type>
    using CommandQueue_t = Internal::CommandQueue<Type>;

    using Direct = Internal::CommandQueue<D3D12_COMMAND_LIST_TYPE_DIRECT>;
    using Bundle = Internal::CommandQueue<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
    using Compute = Internal::CommandQueue<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
    using Copy = Internal::CommandQueue<D3D12_COMMAND_LIST_TYPE_COPY>;
}