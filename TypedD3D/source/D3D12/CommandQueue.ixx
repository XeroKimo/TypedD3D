module;

#include <memory>
#include <array>
#include <span>
#include <d3d12.h>
#include <gsl/pointers>

export module TypedD3D12:CommandQueue;
import :CommandList;
import :CommandAllocator;
import :Wrappers;
import :Resource;
import TypedD3D.Shared;

namespace TypedD3D::D3D12
{
	export struct ClockCalibrationData
	{
		UINT64 gpuTimestamp;
		UINT64 cpuTimestamp;
	};

	template<template<class> class Tag>
	concept CommandQueueEnabledTag = SameTagAs<Tag, DirectTag>
		|| SameTagAs<Tag, ComputeTag>
		|| SameTagAs<Tag, CopyTag>;
}

namespace TypedD3D
{

	template<template<class> class Tag>
		requires D3D12::CommandQueueEnabledTag<Tag>
	struct Trait<Tag<ID3D12CommandQueue>>
	{
		using inner_type = ID3D12CommandQueue;

		using inner_tag = ID3D12CommandQueue;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		class Interface : public InterfaceBase<Tag<Derived>>
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
				gsl::not_null<WrapperView<ID3D12Resource>> pDstResource,
				const D3D12_TILED_RESOURCE_COORDINATE& pDstRegionStartCoordinate,
				gsl::not_null<WrapperView<ID3D12Resource>> pSrcResource,
				const D3D12_TILED_RESOURCE_COORDINATE& pSrcRegionStartCoordinate,
				const D3D12_TILE_REGION_SIZE& pRegionSize,
				D3D12_TILE_MAPPING_FLAGS Flags)
			{
				Self().CopyTileMappings(
					&pDstResource.get().Get(),
					&pDstRegionStartCoordinate,
					&pSrcResource.get().Get(),
					&pSrcRegionStartCoordinate,
					&pRegionSize,
					Flags);
			}

			void EndEvent() { Self().EndEvent(); }

			D3D12::ClockCalibrationData GetClockCalibration()
			{
				D3D12::ClockCalibrationData timeStamps;
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
				gsl::not_null<WrapperView<ID3D12Resource>> pResource,
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
					pResource.get().Get(),
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

			void ExecuteCommandLists(Span<WeakWrapper<Tag<ID3D12CommandList>>> commandLists)
			{
				Self().ExecuteCommandLists(static_cast<UINT>(commandLists.size()), commandLists.data());
			}

		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};

}