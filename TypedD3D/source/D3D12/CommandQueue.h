#pragma once
#include "CommandList.h"
#include "../Internal/ComWrapper.h"
#include "../D3D12Wrappers.h"
#include <memory>
#include <array>

namespace TypedD3D::D3D12
{
    struct GPU_TIMESTAMP
    {
        UINT64 value;
    };
    struct CPU_TIMESTAMP
    {
        UINT64 value;
    };
}

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        template<TypeTag Type>
        using CommandQueue_t = InterfaceWrapper<ID3D12CommandQueue, Type>;
        namespace CommandQueue
        {
            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
            class Interface
            {
            private:
                using list_type = TypedD3D::D3D12::CommandList_t<ID3D12CommandList, Type>;

            public:
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

                std::pair<TypedD3D::D3D12::GPU_TIMESTAMP, TypedD3D::D3D12::CPU_TIMESTAMP> GetClockCalibration()
                {
                    std::pair<TypedD3D::D3D12::GPU_TIMESTAMP, TypedD3D::D3D12::CPU_TIMESTAMP> timeStamps;
                    InternalGet()->GetClockCalibration(&timeStamps.first.value, &timeStamps.second.value);
                    return timeStamps;
                }

                D3D12_COMMAND_QUEUE_DESC GetDesc() { return InternalGet()->GetDesc(); }

            private:
                ID3D12CommandQueue* InternalGet() { return static_cast<WrapperTy&>(*this).Get(); }
            };
        }
    }

    template<class DirectXClass, TypeTag Type>
        requires std::is_base_of_v<ID3D12CommandQueue, DirectXClass> && Is_Command_List_Type<Type>
    class InterfaceWrapper<DirectXClass, Type> : public ComWrapper<DirectXClass>, private D3D12::CommandQueue::Interface<InterfaceWrapper<DirectXClass, Type>, listType<Type>>
    {
    private:
        using Interface = D3D12::CommandQueue::Interface<InterfaceWrapper<DirectXClass, Type>, listType<Type>>;
        friend Interface;

    public:
        static constexpr TypeTag tag_value = Type;
        using underlying_type = DirectXClass;

    public:
        using ComWrapper<DirectXClass>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}

namespace TypedD3D::D3D12
{
    template<D3D12_COMMAND_LIST_TYPE Type>
    using CommandQueue_t = TypedD3D::Internal::D3D12::CommandQueue_t<TypedD3D::Internal::tagValue<Type>>;

    namespace CommandQueue
    {
        using Direct = CommandQueue_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
        using Bundle = CommandQueue_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
        using Compute = CommandQueue_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
        using Copy = CommandQueue_t<D3D12_COMMAND_LIST_TYPE_COPY>;
    }
}