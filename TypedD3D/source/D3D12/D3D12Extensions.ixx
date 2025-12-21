module;

#include <d3d12.h>
#include <utility>
#include <cassert>

export module TypedD3D12:Extensions;
import TypedD3D.Shared;
import :Device;
import :DescriptorHeap;
import :Resource;
import :CommandQueue;
import :Wrappers;
import :Barrier;
import std;

namespace TypedD3D::D3D12::Extensions
{
	export class Fence
	{
		Wrapper<ID3D12Fence> fence;
		UINT64 lastGPUSignaledValue = 0;

	public:
		Fence(WrapperView<ID3D12Device> device, D3D12_FENCE_FLAGS flags = D3D12_FENCE_FLAG_NONE, UINT64 startingValue = 0) :
			fence{ device->CreateFence(startingValue, flags) }
		{

		}

		UINT64 SignalGPU(WrapperView<ID3D12CommandQueue> commandQueue)
		{
			commandQueue->Signal(fence, ++lastGPUSignaledValue);
			return lastGPUSignaledValue;
		}

		void SignalCPU(UINT64 value)
		{
			ThrowIfFailed(fence->Signal(value));
			if(value > lastGPUSignaledValue)
				lastGPUSignaledValue = value;
		}

		void WaitGPU(UINT64 value, WrapperView<ID3D12CommandQueue> commandQueue)
		{
			commandQueue->Wait(fence, value);
		}

		DWORD WaitCPU(UINT64 value, HANDLE eventHandle, DWORD milliseconds = INFINITE)
		{
			if(fence->GetCompletedValue() >= value)
				return 0;

			return WaitForSingleObject(eventHandle, milliseconds);
		}

		void SetEventOnCompletion(UINT64 value, HANDLE eventHandle)
		{
			ThrowIfFailed(fence->SetEventOnCompletion(value, eventHandle));
		}

		DWORD SetEventOnCompletionAndWaitCPU(UINT64 value, HANDLE eventHandle, DWORD milliseconds = INFINITE)
		{
			SetEventOnCompletion(value, eventHandle);
			return WaitCPU(value, eventHandle, milliseconds);
		}

		void Reset()
		{
			fence->Signal(0);
			lastGPUSignaledValue = 0;
		}

		void Flush()
		{
			SetEventOnCompletion(lastGPUSignaledValue, nullptr);
		}

		UINT64 GetCompletedValue() const noexcept
		{
			return fence->GetCompletedValue();
		}

		UINT64 GetLastGPUSignaledValue() const noexcept
		{
			return lastGPUSignaledValue;
		}

		WrapperView<ID3D12Fence> Get() const { return fence; }
	};

	export class CommandQueue;

	template<template<class> class Tag>
		requires CommandQueueEnabledTag<Tag>
	class CommandQueueImpl
	{
		Fence fence;
		StrongWrapper<Tag<ID3D12CommandQueue>> commandQueue;

	public:
		CommandQueueImpl(WrapperView<ID3D12Device> device, UINT nodeMask, D3D12_COMMAND_QUEUE_PRIORITY priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL, D3D12_COMMAND_QUEUE_FLAGS queueFlags = D3D12_COMMAND_QUEUE_FLAG_NONE, D3D12_FENCE_FLAGS fenceFlags = D3D12_FENCE_FLAG_NONE) :
			fence{ device, fenceFlags },
			commandQueue{ device->CreateCommandQueue<D3D12::CommandListTraitToType<Tag>>(priority, queueFlags, nodeMask) }
		{

		}

		~CommandQueueImpl()
		{
			Flush();
		}

	public:
		void BeginEvent(
			UINT Metadata,
			const void* pData,
			UINT Size)
		{
			commandQueue->BeginEvent(Metadata, pData, Size);
		}

		void CopyTileMappings(
			gsl::not_null<WrapperView<ID3D12Resource>> pDstResource,
			const D3D12_TILED_RESOURCE_COORDINATE& pDstRegionStartCoordinate,
			gsl::not_null<WrapperView<ID3D12Resource>> pSrcResource,
			const D3D12_TILED_RESOURCE_COORDINATE& pSrcRegionStartCoordinate,
			const D3D12_TILE_REGION_SIZE& pRegionSize,
			D3D12_TILE_MAPPING_FLAGS Flags)
		{
			commandQueue->CopyTileMappings(
				pDstResource,
				pDstRegionStartCoordinate,
				pSrcResource,
				pSrcRegionStartCoordinate,
				pRegionSize,
				Flags);
		}

		void EndEvent() { commandQueue->EndEvent(); }

		D3D12::ClockCalibrationData GetClockCalibration()
		{
			return commandQueue->GetClockCalibration();
		}

		D3D12_COMMAND_QUEUE_DESC GetDesc() { return commandQueue->GetDesc(); }

		UINT64 GetTimestampFrequency()
		{
			return commandQueue->GetTimestampFrequency();
		}

		void SetMarker(
			UINT Metadata,
			const void* pData,
			UINT Size)
		{
			commandQueue->SetMarker(Metadata, pData, Size);
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
			commandQueue->UpdateTileMappings(
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
			gsl::not_null<WrapperView<ID3D12Fence>> Fence,
			UINT64 Value)
		{
			return commandQueue->Wait(Fence, Value);
		}

		void ExecuteCommandLists(Span<WeakWrapper<Tag<ID3D12CommandList>>> commandLists)
		{
			commandQueue->ExecuteCommandLists(commandLists);
		}

		void ExecuteCommandLists(WeakWrapper<Tag<ID3D12CommandList>> commandLists)
		{
			Array temp{ commandLists };
			commandQueue->ExecuteCommandLists(temp);
		}

		void Flush()
		{
			SignalGPU();
			fence.Flush();
		}

		void Reset()
		{
			Flush();
			fence.Reset();
		}

		UINT64 SignalGPU()
		{
			return fence.SignalGPU(commandQueue);
		}

		UINT64 SignalGPU(WrapperView<ID3D12CommandQueue> commandQueue)
		{
			return fence.SignalGPU(commandQueue);
		}

		void SignalCPU(UINT64 value)
		{
			return fence.SignalCPU(value);
		}

		void WaitGPU(UINT64 value, WrapperView<ID3D12CommandQueue> commandQueue)
		{
			return fence.WaitGPU(value, commandQueue);
		}

		DWORD WaitCPU(UINT64 value, HANDLE eventHandle, DWORD milliseconds = INFINITE)
		{
			return fence.WaitCPU(value, eventHandle, milliseconds);
		}

		void SetEventOnCompletion(UINT64 value, HANDLE eventHandle)
		{
			return fence.SetEventOnCompletion(value, eventHandle);
		}

		DWORD SetEventOnCompletionAndWaitCPU(UINT64 value, HANDLE eventHandle, DWORD milliseconds = INFINITE)
		{
			return fence.SetEventOnCompletionAndWaitCPU(value, eventHandle, milliseconds);
		}

		UINT64 GetCompletedValue() const noexcept
		{
			return fence.GetCompletedValue();
		}

		UINT64 GetLastGPUSignaledValue() const noexcept
		{
			return fence.GetLastGPUSignaledValue();
		}


		WeakWrapper<Tag<ID3D12CommandQueue>> Get() const { return commandQueue; }
	};

	export template<class Ty>
	class FreeListAllocator;

	export enum class DescriptorHeapIndex : UINT {};

	export struct DescriptorHeapIndexArray
	{
		DescriptorHeapIndex start;
		UINT size;

		DescriptorHeapIndex operator[](UINT i)
		{
			return DescriptorHeapIndex{ std::to_underlying(start) + i };
		}
	};

	template<template<class> class Outer, class Inner>
		requires D3D12::DescriptorHeapEnabledTag<Outer<Inner>>&& std::same_as<InnerType<Inner>, ID3D12DescriptorHeap>
	class FreeListAllocator<StrongWrapper<Outer<Inner>>>
	{
		using CPUDescriptorHandle = decltype(std::declval<StrongWrapper<Outer<Inner>>>()->GetCPUDescriptorHandleForHeapStart());
		using GPUDescriptorHandle = decltype(std::declval<StrongWrapper<Outer<Inner>>>()->GetGPUDescriptorHandleForHeapStart());

		struct FreeRange
		{
			DescriptorHeapIndex begin;
			DescriptorHeapIndex end;
		};
	
		StrongWrapper<Outer<Inner>> heap;
		UINT incrementSize;
		UINT allocatorSize;
		CPUDescriptorHandle CPUHandleStart;
		GPUDescriptorHandle GPUHandleStart;
		std::forward_list<FreeRange> freeList;

	public:
		FreeListAllocator(StrongWrapper<Outer<Inner>> heap, UINT incrementSize, UINT startingOffset = 0, std::optional<UINT> allocatorSize = std::nullopt) :
			heap{ std::move(heap) },
			incrementSize{ incrementSize },
			allocatorSize{ allocatorSize.value_or(this->heap->GetDesc().NumDescriptors) }
		{
			if(this->heap->GetDesc().NumDescriptors - startingOffset < this->allocatorSize)
				throw std::bad_alloc();

			CPUHandleStart = this->heap->GetCPUDescriptorHandleForHeapStart().Offset(startingOffset, incrementSize);

			if constexpr(std::same_as<InnerType<Outer<Inner>>, GetTraitInnerType<Outer<Inner>>>)
				GPUHandleStart = {};
			else
				GPUHandleStart = this->heap->GetGPUDescriptorHandleForHeapStart().Offset(startingOffset, incrementSize);
			freeList.push_front(FreeRange{ DescriptorHeapIndex{ 0 }, DescriptorHeapIndex{ this->allocatorSize } });
		}

		FreeListAllocator(const FreeListAllocator&) = delete;
		FreeListAllocator(FreeListAllocator&&) noexcept = default;
		~FreeListAllocator() = default;

		FreeListAllocator& operator=(const FreeListAllocator&) = delete;
		FreeListAllocator& operator=(FreeListAllocator&&) noexcept = default;

		WeakWrapper<Outer<Inner>> GetHeap() const noexcept { return heap; }

		DescriptorHeapIndex Allocate()
		{
			for(FreeRange& range : freeList)
			{
				if(range.begin == range.end)
					continue;

				DescriptorHeapIndex output = range.begin;
				range.begin = DescriptorHeapIndex{ std::to_underlying(range.begin) + 1 };

				return output;
			}

			throw std::bad_alloc();
		}

		void Free(DescriptorHeapIndex index)
		{
			freeList.push_front(FreeRange{ index, DescriptorHeapIndex{ std::to_underlying(index) + 1 } });
		}

		DescriptorHeapIndexArray Allocate(UINT N)
		{
			for(FreeRange& range : freeList)
			{
				if(range.begin == range.end)
					continue;

				if(std::to_underlying(range.end) - std::to_underlying(range.begin) < N)
					continue;

				DescriptorHeapIndexArray array{ range.begin, N };
				range.begin = DescriptorHeapIndex{ std::to_underlying(range.begin) + N };
				return array;
			}

			throw std::bad_alloc();
		}

		void Free(DescriptorHeapIndexArray array)
		{
			freeList.push_front(FreeRange{ array.start, DescriptorHeapIndex{ std::to_underlying(array.start) + array.size } });
		}

		CPUDescriptorHandle GetCPUHandle(DescriptorHeapIndex index)
		{
			return CPUHandleStart.Offset(std::to_underlying(index), incrementSize);
		}

		GPUDescriptorHandle GetGPUHandle(DescriptorHeapIndex index) requires !std::same_as<InnerType<Outer<Inner>>, GetTraitInnerType<Outer<Inner>>>
		{
			return GPUHandleStart.Offset(std::to_underlying(index), incrementSize);
		}

		DescriptorHeapIndex GetIndex(CPUDescriptorHandle handle)
		{
			if(handle.Ptr() < CPUHandleStart.Ptr())
				throw std::range_error{ "Handle out of range" };

			return DescriptorHeapIndex{ static_cast<UINT>((handle.Ptr() - CPUHandleStart.Ptr()) / incrementSize) };
		}

		DescriptorHeapIndex GetIndex(GPUDescriptorHandle handle) requires !std::same_as<InnerType<Outer<Inner>>, GetTraitInnerType<Outer<Inner>>>
		{
			if(handle.Ptr() < GPUHandleStart.Ptr())
				throw std::range_error{ "Handle out of range" };

			return DescriptorHeapIndex{ static_cast<UINT>((handle.Ptr() - GPUHandleStart.Ptr()) / incrementSize) };
		}
	};

	class ConstantBufferCircularAllocator
	{
		struct AllocationFrame
		{
			UINT64 fenceValue;
			SIZE_T size;
		};

		Wrapper<ID3D12Resource> resource;
		std::byte* cpuStartAddress;
		D3D12_GPU_VIRTUAL_ADDRESS gpuStartAddress;
		SIZE_T currentOffset = 0;
		SIZE_T totalAllocated = 0;
		SIZE_T allocatorSize;
		UINT64 freeFenceValue = 0;
		std::vector<AllocationFrame> allocations;
		UINT currentFrame = 0;

	public:
		ConstantBufferCircularAllocator(Wrapper<ID3D12Resource> inResource, SIZE_T startOffset = 0, std::optional<SIZE_T> allocatorSize = std::nullopt, UINT maxFrames = 8) :
			resource{ std::move(inResource) }
		{
			assert(resource->GetHeapProperties().first.Type == D3D12_HEAP_TYPE_UPLOAD);
			assert((startOffset & (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) == 0); //Start offset needs to be aligned to the constant buffer placement
			assert(resource->GetDesc().Width - startOffset >= allocatorSize.value_or(resource->GetDesc().Width - startOffset)); //Check for if there's sufficient size
			assert(std::has_single_bit(allocatorSize.value_or(resource->GetDesc().Width - startOffset))); //For speed purposes, allocator size should be a power of 2
			D3D12_RANGE readRange{};
			cpuStartAddress = resource->Map(0, &readRange) + startOffset;
			gpuStartAddress = resource->GetGPUVirtualAddress() + startOffset;
			this->allocatorSize = allocatorSize.value_or(resource->GetDesc().Width - startOffset);

			allocations.resize(maxFrames);
		}

		ConstantBufferCircularAllocator(const ConstantBufferCircularAllocator&) = delete;
		ConstantBufferCircularAllocator(ConstantBufferCircularAllocator&&) noexcept = default;

		~ConstantBufferCircularAllocator()
		{
			resource->Unmap(0, nullptr);
		}

		ConstantBufferCircularAllocator& operator=(const ConstantBufferCircularAllocator&) = delete;
		ConstantBufferCircularAllocator& operator=(ConstantBufferCircularAllocator&&) noexcept = default;

		template<class Ty>
		D3D12_GPU_VIRTUAL_ADDRESS Allocate(const Ty& data)
		{
			return InternalUpload(&data, sizeof(data));
		}

		template<std::ranges::contiguous_range Ty>
		D3D12_GPU_VIRTUAL_ADDRESS Allocate(const Ty& data)
		{
			return InternalUpload(std::data(data), std::size(data) * sizeof(std::ranges::range_value_t<Ty>));
		}

		void Signal(UINT64 fenceValue)
		{
			allocations[currentFrame].fenceValue = fenceValue;
			currentFrame = (currentFrame + 1) % allocations.size();
		}

		void Free(UINT64 fenceValue)
		{
			for(std::size_t i = 0; i < allocations.size(); i++)
			{
				UINT frameToCheck = currentFrame - static_cast<UINT>(i);
				if(frameToCheck >= allocations.size())
					frameToCheck = static_cast<UINT>(allocations.size()) - 1;

				if(allocations[frameToCheck].fenceValue <= freeFenceValue)
					break;

				if(allocations[frameToCheck].fenceValue <= fenceValue)
					totalAllocated -= allocations[frameToCheck].size;
			}

			freeFenceValue = fenceValue;
		}

	private:
		D3D12_GPU_VIRTUAL_ADDRESS InternalUpload(const void* data, SIZE_T size)
		{
			const SIZE_T wrappedOffset = currentOffset & (allocatorSize - 1);
			const SIZE_T remainingSizeUntilWrap = allocatorSize - wrappedOffset;

			const auto [totalUploadSize, gpuAddress, cpuAddress] = [&]
			{
				if(remainingSizeUntilWrap < size)
					return std::tuple{ size + remainingSizeUntilWrap, gpuStartAddress , cpuStartAddress };
				else
					return std::tuple{ size, gpuStartAddress + wrappedOffset, cpuStartAddress + wrappedOffset };
			}();

			if(allocatorSize - totalAllocated < totalUploadSize)
				throw std::bad_alloc();

			std::memcpy(cpuAddress, data, size);
			allocations[currentFrame].size += totalUploadSize;
			currentOffset += totalUploadSize;
			totalAllocated += totalUploadSize;

			return gpuAddress;
		}
	};

	export template<class Ty>
	void DrawFrame(DirectView<ID3D12GraphicsCommandList7> commandList, 
		WrapperView<ID3D12Resource> renderTargetResource,
		RTV<D3D12_CPU_DESCRIPTOR_HANDLE> renderTarget, 
		std::array<float, 4> clearColor,
		WrapperView<ID3D12Resource> depthStencilResource,
		std::optional<DSV<D3D12_CPU_DESCRIPTOR_HANDLE>> depthStencil,
		D3D12_CLEAR_FLAGS clearFlags,
		FLOAT depthClear,
		UINT8 stencilClear,
		D3D12_RECT rect, 
		D3D12_VIEWPORT viewport,
		std::optional<ShaderVisibleView<CBV_SRV_UAV<ID3D12DescriptorHeap>>> cbvHeap,
		std::optional<ShaderVisibleView<Sampler<ID3D12DescriptorHeap>>> srvHeap,
		Ty func)
	{
		if (cbvHeap && srvHeap)
			commandList->SetDescriptorHeaps(cbvHeap.value(), srvHeap.value());
		else if (cbvHeap)
			commandList->SetDescriptorHeaps(cbvHeap.value());
		else
			commandList->SetDescriptorHeaps(srvHeap.value());

		if(depthStencil)
		{
		
			TypedD3D::Array<TypedD3D::Wrapper<D3D12_TEXTURE_BARRIER>, 2> b
			{
				TypedD3D::Wrapper<D3D12_TEXTURE_BARRIER>::Construct(renderTargetResource)
					.Before(D3D12::BarrierAccessCommon(D3D12_BARRIER_SYNC_DRAW, D3D12_BARRIER_LAYOUT_COMMON))
					.After(D3D12::BarrierAccessRenderTarget(D3D12::RenderTargetSync::RenderTarget)),
				TypedD3D::Wrapper<D3D12_TEXTURE_BARRIER>::Construct(depthStencilResource)
					.Before(D3D12::BarrierAccessCommon(D3D12_BARRIER_SYNC_DRAW, D3D12_BARRIER_LAYOUT_COMMON))
					.After(D3D12::BarrierAccessDepthStencilWrite(D3D12::DepthStencilSync::Draw)),
			};

			std::array group{ D3D12::MakeBarrierGroup(TypedD3D::Span{b}) };

			commandList->Barrier(group);
			commandList->ClearRenderTargetView(renderTarget, clearColor);
			commandList->ClearDepthStencilView(depthStencil.value(), clearFlags, depthClear, stencilClear, {});
			commandList->OMSetRenderTargets(TypedD3D::Span<TypedD3D::RTV<D3D12_CPU_DESCRIPTOR_HANDLE>>{ &renderTarget.Raw(), 1 }, true, &depthStencil.value());
			commandList->RSSetScissorRects({ &rect, 1 });
			commandList->RSSetViewports({ &viewport, 1 });

			func();

			b[0] = b[0].Flip();
			b[1] = b[1].Flip();
			commandList->Barrier(group);
		}
		else
		{
			TypedD3D::Array<TypedD3D::Wrapper<D3D12_TEXTURE_BARRIER>, 1> b
			{
				TypedD3D::Wrapper<D3D12_TEXTURE_BARRIER>::Construct(renderTargetResource)
					.Before(D3D12::BarrierAccessCommon(D3D12_BARRIER_SYNC_DRAW, D3D12_BARRIER_LAYOUT_COMMON))
					.After(D3D12::BarrierAccessRenderTarget(D3D12::RenderTargetSync::RenderTarget))
			};

			std::array group{ D3D12::MakeBarrierGroup(TypedD3D::Span{b}) };
			commandList->Barrier(group);

			commandList->ClearRenderTargetView(renderTarget, clearColor);
			commandList->OMSetRenderTargets(TypedD3D::Span<TypedD3D::RTV<D3D12_CPU_DESCRIPTOR_HANDLE>>{ &renderTarget.Raw(), 1 }, true, nullptr);
			commandList->RSSetScissorRects({ &rect, 1 });
			commandList->RSSetViewports({ &viewport, 1 });

			func();

			b[0] = b[0].Flip();
			commandList->Barrier(group);
		}
	}
}

namespace TypedD3D
{
	template<>
	struct DirectMapper<D3D12::Extensions::CommandQueue>
	{
		using type = D3D12::Extensions::CommandQueueImpl<DirectTag>;
	};

	template<>
	struct ComputeMapper<D3D12::Extensions::CommandQueue>
	{
		using type = D3D12::Extensions::CommandQueueImpl<ComputeTag>;
	};

	template<>
	struct CopyMapper<D3D12::Extensions::CommandQueue>
	{
		using type = D3D12::Extensions::CommandQueueImpl<CopyTag>;
	};
}