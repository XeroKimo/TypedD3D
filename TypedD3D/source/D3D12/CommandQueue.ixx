module;

#include <memory>
#include <array>
#include <span>
#include <d3d12.h>

export module TypedD3D12:CommandQueue;
import :CommandList;
import :CommandAllocator;
import :Wrappers;
import TypedD3D.Shared;

namespace TypedD3D::D3D12
{
    struct ClockCalibrationData
    {
        UINT64 gpuTimestamp;
        UINT64 cpuTimestamp;
    };
}

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        export template<D3D12_COMMAND_LIST_TYPE Type>
        using CommandQueue_t = IUnknownWrapper<ID3D12CommandQueue, GetTraitTagType<CommandListTypeToTraitTag<Type>>>;

        namespace CommandQueue
        {
            struct Traits
            {
                using value_type = ID3D12CommandQueue;
                using pointer = ID3D12CommandQueue*;
                using const_pointer = const ID3D12CommandQueue*;
                using reference = ID3D12CommandQueue&;
                using const_reference = const ID3D12CommandQueue&;

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

                    void CopyTileMappings(
                        ID3D12Resource& pDstResource,
                        const D3D12_TILED_RESOURCE_COORDINATE& pDstRegionStartCoordinate,
                        ID3D12Resource& pSrcResource,
                        const D3D12_TILED_RESOURCE_COORDINATE& pSrcRegionStartCoordinate,
                        const D3D12_TILE_REGION_SIZE& pRegionSize,
                        D3D12_TILE_MAPPING_FLAGS Flags)
                    {
                        Get().CopyTileMappings(
                            &pDstResource,
                            &pDstRegionStartCoordinate,
                            &pSrcResource,
                            &pSrcRegionStartCoordinate,
                            &pRegionSize,
                            Flags);
                    }

                    void EndEvent() { Get().EndEvent(); }

                    TypedD3D::D3D12::ClockCalibrationData GetClockCalibration()
                    {
                        TypedD3D::D3D12::ClockCalibrationData timeStamps;
                        Get().GetClockCalibration(&timeStamps.gpuTimestamp, &timeStamps.cpuTimestamp);
                        return timeStamps;
                    }

                    D3D12_COMMAND_QUEUE_DESC GetDesc() { return Get().GetDesc(); }

                    UINT64 GetTimestampFrequency()
                    {
                        UINT64 frequency = 0;
                        Get().GetTimestampFrequency(&frequency);
                        return frequency;
                    }

                    void SetMarker(
                        UINT Metadata,
                        const void* pData,
                        UINT Size)
                    {
                        Get().SetMarker(Metadata, pData, Size);
                    }

                    HRESULT Signal(
                        ID3D12Fence& Fence,
                        UINT64 Value)
                    {
                        return Get().Signal(&Fence, Value);
                    }

                    HRESULT Reset() { Get().Reset(); }

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
                        Get().UpdateTileMappings(
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

                    HRESULT Wait(
                        ID3D12Fence& Fence,
                        UINT64 Value)
                    {
                        return Get().Wait(&Fence, Value);
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };
        }
    }

}

namespace TypedD3D
{
    template<D3D12TraitTags Type>
    struct CommandQueueTraits
    {
        static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Internal::D3D12::TraitTagToCommandListType<Type>;

        using value_type = ID3D12CommandQueue;
        using pointer = ID3D12CommandQueue*;
        using const_pointer = const ID3D12CommandQueue*;
        using reference = ID3D12CommandQueue&;
        using const_reference = const ID3D12CommandQueue&;

        using list_value_type = D3D12::CommandList_t<ID3D12CommandList, command_list_value>;
        using allocator_value_type = D3D12::CommandAllocator_t<command_list_value>;

        template<class DerivedSelf>
        class Interface : public Internal::D3D12::CommandQueue::Traits::Interface<DerivedSelf>
        {
        private:
            using derived_self = DerivedSelf;

        public:
            template<size_t Extents>
            void ExecuteCommandLists(
                std::span<list_value_type, Extents> commandLists)
            {
                if constexpr(Extents == std::dynamic_extent)
                {
                    std::unique_ptr<ID3D12CommandList* []> submitList = std::make_unique<ID3D12CommandList * []>(commandLists.size());

                    for(size_t i = 0; i < commandLists.size(); i++)
                    {
                        submitList[i] = commandLists[i].Get();
                    }

                    Get().ExecuteCommandLists(static_cast<UINT>(commandLists.size()), submitList.get());
                }
                else
                {
                    std::array<ID3D12CommandList*, Extents> submitList;

                    for(size_t i = 0; i < commandLists.size(); i++)
                    {
                        submitList[i] = commandLists[i].Get();
                    }

                    Get().ExecuteCommandLists(static_cast<UINT>(commandLists.size()), submitList.data());
                }
            }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };



    template<std::derived_from<ID3D12CommandQueue> Ty, D3D12TraitTags Tag>
        requires (Tag == D3D12TraitTags::Direct) ||
    (Tag == D3D12TraitTags::Compute) ||
        (Tag == D3D12TraitTags::Copy) ||
        (Tag == D3D12TraitTags::Bundle)
        struct D3D12TaggedTraits<Ty, Tag> : CommandQueueTraits<Tag>
    {
        static constexpr D3D12TraitTags tag_value = Tag;
    };
}

namespace TypedD3D::D3D12
{
    export template<D3D12_COMMAND_LIST_TYPE Type>
    using CommandQueue_t = Internal::D3D12::CommandQueue_t<Type>;

    namespace CommandQueue
    {
        export using Direct = CommandQueue_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
        export using Bundle = CommandQueue_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
        export using Compute = CommandQueue_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
        export using Copy = CommandQueue_t<D3D12_COMMAND_LIST_TYPE_COPY>;
    }
}