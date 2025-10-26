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
	export struct ClockCalibrationData
	{
		UINT64 gpuTimestamp;
		UINT64 cpuTimestamp;
	};


	export template<D3D12_COMMAND_LIST_TYPE Type>
		struct CommandQueueTraits
	{
		static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;

		using value_type = ID3D12CommandQueue;
		using pointer = ID3D12CommandQueue*;
		using const_pointer = const ID3D12CommandQueue*;
		using reference = ID3D12CommandQueue&;
		using const_reference = const ID3D12CommandQueue&;

		template<class Derived>
		class Interface : public InterfaceBase<CommandListTypeToTrait<Type, Derived>>
		{
		public:

			void BeginEvent(
				UINT Metadata,
				const void* pData,
				UINT Size)
			{
				Self().BeginEvent(Metadata, pData, Size);
			}

			void CopyTileMappings(
				ID3D12Resource& pDstResource,
				const D3D12_TILED_RESOURCE_COORDINATE& pDstRegionStartCoordinate,
				ID3D12Resource& pSrcResource,
				const D3D12_TILED_RESOURCE_COORDINATE& pSrcRegionStartCoordinate,
				const D3D12_TILE_REGION_SIZE& pRegionSize,
				D3D12_TILE_MAPPING_FLAGS Flags)
			{
				Self().CopyTileMappings(
					&pDstResource,
					&pDstRegionStartCoordinate,
					&pSrcResource,
					&pSrcRegionStartCoordinate,
					&pRegionSize,
					Flags);
			}

			void EndEvent() { Self().EndEvent(); }

			ClockCalibrationData GetClockCalibration()
			{
				ClockCalibrationData timeStamps;
				Self().GetClockCalibration(&timeStamps.gpuTimestamp, &timeStamps.cpuTimestamp);
				return timeStamps;
			}

			D3D12_COMMAND_QUEUE_DESC GetDesc() { return Self().GetDesc(); }

			UINT64 GetTimestampFrequency()
			{
				UINT64 frequency = 0;
				Self().GetTimestampFrequency(&frequency);
				return frequency;
			}

			void SetMarker(
				UINT Metadata,
				const void* pData,
				UINT Size)
			{
				Self().SetMarker(Metadata, pData, Size);
			}

			HRESULT Signal(
				ID3D12Fence& Fence,
				UINT64 Value)
			{
				return Self().Signal(&Fence, Value);
			}

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
				Self().UpdateTileMappings(
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
				return Self().Wait(&Fence, Value);
			}

			void ExecuteCommandLists(Span<WeakWrapper<CommandListTypeToTrait<Type, ID3D12CommandList>>> commandLists)
			{
				Self().ExecuteCommandLists(static_cast<UINT>(commandLists.size()), commandLists.data());
			}

		private:
			using InterfaceBase<CommandListTypeToTrait<Type, Derived>>::Self;
			using InterfaceBase<CommandListTypeToTrait<Type, Derived>>::ToDerived;
		};
	};
}

namespace TypedD3D
{
	template<>
	struct DirectTraits<ID3D12CommandQueue> : D3D12::CommandQueueTraits<D3D12_COMMAND_LIST_TYPE_DIRECT> {};

	template<>
	struct ComputeTraits<ID3D12CommandQueue> : D3D12::CommandQueueTraits<D3D12_COMMAND_LIST_TYPE_COMPUTE> {};

	template<>
	struct CopyTraits<ID3D12CommandQueue> : D3D12::CommandQueueTraits<D3D12_COMMAND_LIST_TYPE_COPY> {};
}