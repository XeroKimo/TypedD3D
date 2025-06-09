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

	struct CommonCommandQueueTraits
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

			ClockCalibrationData GetClockCalibration()
			{
				ClockCalibrationData timeStamps;
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

	template<D3D12_COMMAND_LIST_TYPE Type>
	struct CommandQueueTraits
	{
		static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;

		using value_type = ID3D12CommandQueue;
		using pointer = ID3D12CommandQueue*;
		using const_pointer = const ID3D12CommandQueue*;
		using reference = ID3D12CommandQueue&;
		using const_reference = const ID3D12CommandQueue&;

		using list_value_type = CommandList_t<ID3D12CommandList, command_list_value>;
		using allocator_value_type = CommandAllocator_t<command_list_value>;

		template<class DerivedSelf>
		class Interface : public CommonCommandQueueTraits::Interface<DerivedSelf>
		{
		private:
			using derived_self = DerivedSelf;

		public:
			template<std::convertible_to<list_value_type> ListTy, size_t Extents>
			void ExecuteCommandLists(
				Span<ListTy, Extents> commandLists)
			{
				Get().ExecuteCommandLists(static_cast<UINT>(commandLists.size()), commandLists.data());
			}

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	template<>
	struct DirectTraits<ID3D12CommandQueue> : CommandQueueTraits<TraitToCommandListType<DirectTraits>> {};

	template<>
	struct ComputeTraits<ID3D12CommandQueue> : CommandQueueTraits<TraitToCommandListType<ComputeTraits>> {};

	template<>
	struct CopyTraits<ID3D12CommandQueue> : CommandQueueTraits<TraitToCommandListType<CopyTraits>> {};

	template<>
	struct BundleTraits<ID3D12CommandQueue> : CommandQueueTraits<TraitToCommandListType<BundleTraits>> {};

	template<D3D12_COMMAND_LIST_TYPE Type>
	using D3D12CommandQueue_t = IUnknownWrapper<ID3D12CommandQueue, CommandListTypeToTrait<Type>::template type>;

	namespace Aliases
	{
		export using D3D12DirectCommandQueue = D3D12CommandQueue_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
		export using D3D12BundleCommandQueue = D3D12CommandQueue_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
		export using D3D12ComputeCommandQueue = D3D12CommandQueue_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
		export using D3D12CopyCommandQueue = D3D12CommandQueue_t<D3D12_COMMAND_LIST_TYPE_COPY>;
	}
}