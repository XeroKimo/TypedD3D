//*********************************************************
//
// Copyright (c) 2022 Renzy Alarcon
// Licensed under the MIT License (MIT).
//
//*********************************************************

#pragma once
#include "COMHelpers.h"
#include "DXGIHelpers.h"
#include "CommonHelpers.h"
#include "expected.hpp"
#include "span_tuple.h"
#include <functional>
#include <type_traits>
#include <d3d12.h>
#include <chrono>
#include <span>
#include <assert.h>
#include <algorithm>
#include <optional>

namespace TypedD3D::Helpers::D3D12
{
    template<std::derived_from<ID3D12Device> Device = ID3D12Device>
    tl::expected<Microsoft::WRL::ComPtr<Device>, HRESULT> CreateDevice(D3D_FEATURE_LEVEL minFeatureLevel, IDXGIAdapter* optAdapter = nullptr)
    {
        using Microsoft::WRL::ComPtr;

        static_assert(std::derived_from<Device, ID3D12Device>, "This function requires it's type to inherit from ID3D12Device");
        return Helpers::COM::IIDToObjectForwardFunction<Device>(&D3D12CreateDevice, optAdapter, minFeatureLevel);
    }

    //Creates a open command list
    template<class CommandList = ID3D12GraphicsCommandList>
    tl::expected<Microsoft::WRL::ComPtr<CommandList>, HRESULT> CreateCommandList(ID3D12Device& device, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator& allocator, UINT nodeMask = 0, ID3D12PipelineState* optInitialPipeline = nullptr)
    {
        static_assert(std::derived_from<CommandList, ID3D12CommandList>, "This function requires it's type to inherit from ID3D12CommandList");
        return Helpers::COM::IIDToObjectForwardFunction<CommandList>(&ID3D12Device::CreateCommandList, device, nodeMask, type, &allocator, optInitialPipeline);
    }

    //Creates a closed command list
    template<class CommandList = ID3D12GraphicsCommandList>
    tl::expected<Microsoft::WRL::ComPtr<CommandList>, HRESULT> CreateCommandList(ID3D12Device4& device, D3D12_COMMAND_LIST_TYPE type, D3D12_COMMAND_LIST_FLAGS flags, UINT nodeMask = 0)
    {
        static_assert(std::is_base_of_v<ID3D12CommandList, CommandList>, "This function requires it's type to inherit from ID3D12CommandList");
        return Helpers::COM::IIDToObjectForwardFunction<CommandList>(&ID3D12Device4::CreateCommandList1, device, nodeMask, type, flags);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, HRESULT> CreateCommandAllocator(ID3D12Device& device, D3D12_COMMAND_LIST_TYPE type)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12CommandAllocator>(&ID3D12Device::CreateCommandAllocator, device, type);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12PipelineState>, HRESULT> CreateGraphicsPipelineState(ID3D12Device& device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12PipelineState>(&ID3D12Device::CreateGraphicsPipelineState, device, &desc);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12PipelineState>, HRESULT> CreateComputePipelineState(ID3D12Device& device, const D3D12_COMPUTE_PIPELINE_STATE_DESC& desc)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12PipelineState>(&ID3D12Device::CreateComputePipelineState, device, &desc);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12CommandQueue>, HRESULT> CreateCommandQueue(ID3D12Device& device, const D3D12_COMMAND_QUEUE_DESC& desc)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12CommandQueue>(&ID3D12Device::CreateCommandQueue, device, &desc);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12Fence>, HRESULT> CreateFence(ID3D12Device& device, D3D12_FENCE_FLAGS flags, UINT64 initialValue = 0)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12Fence>(&ID3D12Device::CreateFence, device, initialValue, flags);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>, HRESULT> CreateDescriptorHeap(ID3D12Device& device, const D3D12_DESCRIPTOR_HEAP_DESC& descriptorHeapDesc)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12DescriptorHeap>(&ID3D12Device::CreateDescriptorHeap, device, &descriptorHeapDesc);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12RootSignature>, HRESULT> CreateRootSignature(ID3D12Device& device, UINT nodeMask, const void* pBlobWithRootSignature, SIZE_T blobLengthInBytes)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12RootSignature>(&ID3D12Device::CreateRootSignature, device, nodeMask, pBlobWithRootSignature, blobLengthInBytes);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12QueryHeap>, HRESULT> CreateQueryHeap(ID3D12Device& device, const D3D12_QUERY_HEAP_DESC& desc)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12QueryHeap>(&ID3D12Device::CreateQueryHeap, device, &desc);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12CommandSignature>, HRESULT> CreateCommandSignature(ID3D12Device& device,
        const D3D12_COMMAND_SIGNATURE_DESC& pDesc,
        ID3D12RootSignature* optRootSignature)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12CommandSignature>(&ID3D12Device::CreateCommandSignature, device, &pDesc, optRootSignature);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12Resource>, HRESULT> CreateCommittedResource(ID3D12Device& device,
        const D3D12_HEAP_PROPERTIES& pHeapProperties,
        D3D12_HEAP_FLAGS HeapFlags,
        const D3D12_RESOURCE_DESC& pDesc,
        D3D12_RESOURCE_STATES InitialResourceState,
        const D3D12_CLEAR_VALUE* optOptimizedClearValue)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12Resource>(&ID3D12Device::CreateCommittedResource, device, &pHeapProperties, HeapFlags, &pDesc, InitialResourceState, optOptimizedClearValue);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12Heap>, HRESULT> CreateHeap(
        ID3D12Device& device,
        const D3D12_HEAP_DESC& pDesc)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12Heap>(&ID3D12Device::CreateHeap, device, &pDesc);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12Resource>, HRESULT> CreatePlacedResource(ID3D12Device& device,
        ID3D12Heap& pHeap,
        UINT64 HeapOffset,
        const D3D12_RESOURCE_DESC& pDesc,
        D3D12_RESOURCE_STATES InitialState,
        const D3D12_CLEAR_VALUE* optOptimizedClearValue)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12Resource>(&ID3D12Device::CreatePlacedResource, device, &pHeap, HeapOffset, &pDesc, InitialState, optOptimizedClearValue);
    }

    inline tl::expected<Microsoft::WRL::ComPtr<ID3D12Resource>, HRESULT> CreateReservedResource(ID3D12Device& device,
        const D3D12_RESOURCE_DESC& pDesc,
        D3D12_RESOURCE_STATES InitialState,
        const D3D12_CLEAR_VALUE* optOptimizedClearValue)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12Resource>(&ID3D12Device::CreateReservedResource, device, &pDesc, InitialState, optOptimizedClearValue);
    }

    template<class Resource = ID3D12Resource>
    std::vector<Microsoft::WRL::ComPtr<Resource>> CreateSwapChainRenderTargets(ID3D12Device& device, IDXGISwapChain1& swapChain, ID3D12DescriptorHeap& descriptor)
    {
        UINT rtvOffset = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE handle = descriptor.GetCPUDescriptorHandleForHeapStart();
        DXGI_SWAP_CHAIN_DESC1 desc = Helpers::Common::GetDescription(swapChain);

        std::vector<Microsoft::WRL::ComPtr<Resource>> buffers(desc.BufferCount);
        for(UINT i = 0; i < desc.BufferCount; i++)
        {
            buffers[i] = Helpers::DXGI::SwapChain::GetBuffer<Resource>(swapChain, i).value();

            device.CreateRenderTargetView(buffers[i].Get(), nullptr, handle);
            handle.ptr += rtvOffset;
        }

        return buffers;
    }

    /// <summary>
    /// Tells a given command queue to update the fence value to the currentFenceValue + 1 when it is done executing it's command lists
    /// </summary>
    /// <param name="commandQueue">The command queue to signal</param>
    /// <param name="fence">The fence that will be signaled</param>
    /// <param name="currentFenceValue"></param>
    /// <returns>The next fence value one waits for</returns>
    UINT64 SignalFenceGPU(ID3D12CommandQueue& commandQueue, ID3D12Fence& fence, UINT64 currentFenceValue);

    /// <summary>
    /// Updates the fence's current value to a given value.
    /// </summary>
    /// <param name="fence"></param>
    /// <param name="fenceValue"></param>
    void SignalFenceCPU(ID3D12Fence& fence, UINT64 fenceValue);

    inline void ResetFence(ID3D12Fence& fence) { fence.Signal(0); }

    void FlushCommandQueue(ID3D12CommandQueue& commandQueue, ID3D12Fence& fence, UINT64& currentFenceValue, HANDLE waitEvent = nullptr);

    constexpr std::chrono::milliseconds waitForCompletion = (std::chrono::milliseconds::max)();

    /// <summary>
    /// Stalls the calling CPU thread by putting it to sleep until the given fence has been signaled to a given value by another thread or the GPU
    /// </summary>
    /// <param name="fence">Fence that the GPU will signal </param>
    /// <param name="fenceValue">The value we're waiting to be signaled</param>
    /// <param name="waitEvent">The syncing primitive given by windows, passing a nullptr will cause the fence to wait until the fence has been signaled to given value</param>
    /// <param name="waitInterval">How long the thread will stall for to check for completion. If the default value (equivalent to std::chrono::milliseconds::max()) is passed, the thread will be asleep until the fence has been signaled to the given value. If waitEvent is nullptr, this param is unused</param>
    void StallCPUThread(ID3D12Fence& fence, UINT64 fenceValue, HANDLE waitEvent = nullptr, std::chrono::milliseconds waitInterval = waitForCompletion);

    /// <summary>
    /// Stalls a command queue until the given fence has been signaled to a given value by another command queue or the GPU
    /// </summary>
    /// <param name="commandQueue"> Command queue that will be stalled</param>
    /// <param name="fence"> The fence that will be signaled </param>
    /// <param name="fenceValue"> The value we're waiting for before </param>
    void StallCommandQueue(ID3D12CommandQueue& commandQueue, ID3D12Fence& fence, UINT64 fenceValue);

    /// <summary>
    /// Resets the passed in command list and allocator before sending it to the invocable followed by closing the command list and returning it
    /// </summary>
    /// <param name="commandList">A closed command list to reset</param>
    /// <param name="commandAllocator">A command allocator that requires resetting</param>
    /// <param name="func">An invocable that takes in a command list pointer to do work with it</param>
    /// <param name="pipeline">An optional pipeline to start the command list with</param>
    /// <returns>The passed in command list in a closed state</returns>
    template<std::derived_from<ID3D12GraphicsCommandList> ListTy, std::invocable<ListTy*> Func>
    ID3D12CommandList* Record(ListTy& commandList, ID3D12CommandAllocator& commandAllocator, Func&& func, ID3D12PipelineState* pipeline = nullptr)
    {
        commandAllocator.Reset();
        commandList.Reset(&commandAllocator, pipeline);
        func(&commandList);
        commandList.Close();
        return static_cast<ID3D12CommandList*>(&commandList);
    }

    /// <summary>
    /// Executes the function with the passed in command list and closes it before returning
    /// </summary>
    /// <param name="commandList">An already resetted command list</param>
    /// <param name="func">An invocable that takes in a command list pointer to do work with it</param>
    /// <returns>The passed in command list in a closed state</returns>
    template<std::derived_from<ID3D12GraphicsCommandList> ListTy, std::invocable<ListTy*> Func>
    ID3D12CommandList* Record(ListTy& commandList, Func&& func)
    {
        func(&commandList);
        commandList.Close();
        return static_cast<ID3D12CommandList*>(&commandList);
    }

    template<std::derived_from<ID3D12GraphicsCommandList> ListTy, std::invocable<ListTy*> Func>
    void ResourceBarrier(ListTy& commandList, std::span<D3D12_RESOURCE_BARRIER> beginBarriers, std::span<D3D12_RESOURCE_BARRIER> endBarriers, Func&& func)
    {
        commandList.ResourceBarrier(static_cast<UINT>(beginBarriers.size()), beginBarriers.data());

        func(&commandList);

        commandList.ResourceBarrier(static_cast<UINT>(endBarriers.size()), endBarriers.data());
    }

    /// <summary>
    /// Resets the passed in command list and allocator before sending it to the invocable followed by closing the command list and executing it
    /// </summary>
    /// <param name="commandQueue">A command queue which will execute the command list</param>
    /// <param name="commandList">A closed command list to reset</param>
    /// <param name="commandAllocator">A command allocator that requires resetting</param>
    /// <param name="func">An invocable that takes in a command list pointer to do work with it</param>
    /// <param name="pipeline">An optional pipeline to start the command list with</param>
    template<std::derived_from<ID3D12GraphicsCommandList> ListTy, std::invocable<ListTy*> Func>
    void RecordAndExecute(ListTy& commandList, ID3D12CommandAllocator& commandAllocator, ID3D12CommandQueue& commandQueue, Func&& func, ID3D12PipelineState* pipeline = nullptr)
    {
        std::array<ID3D12CommandList*, 1> lists{ Record(commandList, commandAllocator, std::forward<Func>(func), pipeline) };
        commandQueue.ExecuteCommandLists(static_cast<UINT>(lists.size()), lists.data());
    }

    /// <summary>
    /// Executes the function with the passed in command list and closes it before returning
    /// </summary>
    /// <param name="commandQueue">A command queue which will execute the command list</param>
    /// <param name="commandList">An already resetted command list</param>
    /// <param name="func">An invocable that takes in a command list pointer to do work with it</param>
    template<std::derived_from<ID3D12GraphicsCommandList> ListTy, std::invocable<ListTy*> Func>
    void RecordAndExecute(ListTy& commandList, ID3D12CommandQueue& commandQueue, Func&& func)
    {
        std::array<ID3D12CommandList*, 1> lists{ Record(commandList, std::forward<Func>(func)) };
        commandQueue.ExecuteCommandLists(static_cast<UINT>(lists.size()), lists.data());
    }

    /// <summary>
    /// Calls the invocable and then have the command queue signal the fence when work is done
    /// </summary>
    /// <param name="commandQueue"></param>
    /// <param name="fence"></param>
    /// <param name="fenceValue"></param>
    /// <param name="f"></param>
    template<std::invocable<ID3D12CommandQueue*> Func>
    void GPUWork(ID3D12CommandQueue& commandQueue, ID3D12Fence& fence, UINT64& fenceValue, Func&& func)
    {
        func(&commandQueue);
        fenceValue = SignalFenceGPU(commandQueue, fence, fenceValue);
    }

    /// <summary>
    /// Calls the invocable and then have the CPU signal the fence
    /// </summary>
    /// <param name="fence"></param>
    /// <param name="fenceValue"></param>
    /// <param name="f"></param>
    template<std::invocable Func>
    void CPUWork(ID3D12Fence& fence, UINT64 fenceValue, Func&& func)
    {
        func();
        SignalFenceCPU(fence, fenceValue);
    }

    /// <summary>
    /// Has a CPU thread wait for a fence to reach a fence value before doing some work
    /// </summary>
    /// <param name="fence"></param>
    /// <param name="fenceValue"></param>
    /// <param name="f"></param>
    /// <param name="waitEvent"></param>
    /// <param name="waitInterval"></param>
    template<std::invocable Func>
    void CPUWaitAndThen(ID3D12Fence& fence, UINT64 fenceValue, Func&& func, HANDLE waitEvent = nullptr, std::chrono::milliseconds waitInterval = waitForCompletion)
    {
        StallCPUThread(fence, fenceValue, waitEvent, waitInterval);
        func();
    }

    /// <summary>
    /// Has a GPU queue wait for a fence value before executing the next work
    /// </summary>
    /// <param name="commandQueue"></param>
    /// <param name="fence"></param>
    /// <param name="fenceValue"></param>
    /// <param name="f"></param>
    template<std::invocable<ID3D12CommandQueue*> Func>
    void GPUWaitAndThen(ID3D12CommandQueue& commandQueue, ID3D12Fence& fence, UINT64 fenceValue, Func&& func)
    {
        commandQueue.Wait(&fence, fenceValue);
        func(&commandQueue);
    }

    class FrameData
    {
    private:
        //The command queue associated with the current frame's swap chain
        ID3D12CommandQueue& m_commandQueue;

        //The fence the command queue will use to signal
        ID3D12Fence& m_fence;

        using FenceValue_t = UINT64;
        //The resources that will be used as frame buffers and the fence values
        xk::span_tuple<ID3D12Resource*, std::dynamic_extent, FenceValue_t> m_frameBuffers;

        //Equivalent to the frame with the highest fence value
        //Suggested to use a seperate UINT64 object that can persist between Frame() calls
        FenceValue_t& m_highestFenceValue;

        //The current back buffer index
        UINT& m_backBufferIndex;

        D3D12_CPU_DESCRIPTOR_HANDLE m_backBufferRTVHandle;

    public:
        template<std::derived_from<IDXGISwapChain> SwapChainTy, std::invocable<FrameData&> Func>
        FrameData(ID3D12CommandQueue& commandQueue,
            ID3D12Fence& fence,
            xk::span_tuple<ID3D12Resource*, std::dynamic_extent, FenceValue_t> frameBuffers,
            UINT64& highestFenceValue,
            UINT& backBufferIndex,
            D3D12_CPU_DESCRIPTOR_HANDLE backBufferRTVHandle,
            SwapChainTy& swapChain,
            UINT syncInterval,
            UINT presentFlags,
            HANDLE waitEvent,
            std::chrono::milliseconds waitInterval,
            Func&& func) :
            m_commandQueue(commandQueue),
            m_fence(fence),
            m_frameBuffers(frameBuffers),
            m_highestFenceValue(highestFenceValue),
            m_backBufferIndex(backBufferIndex),
            m_backBufferRTVHandle(backBufferRTVHandle)
        {
            CPUWaitAndThen(fence, GetCurrentFrameFenceValue(), [&, this]()
            {
                GetCurrentFrameFenceValue() = highestFenceValue;

                func(*this);

                GetCurrentFrameFenceValue() = highestFenceValue = SignalFenceGPU(commandQueue, fence, GetCurrentFrameFenceValue());
                swapChain.Present(syncInterval, presentFlags);
                backBufferIndex = (backBufferIndex + 1) % frameBuffers.size();

            }, waitEvent, waitInterval);
        }

    public:
        ID3D12CommandQueue& GetCommandQueue() const { return m_commandQueue; }
        ID3D12Fence& GetFence() const { return m_fence; }

        ID3D12Resource& GetCurrentFrameBuffer() const { return *get<ID3D12Resource*>(m_frameBuffers)[GetCurrentFrameIndex()]; }
        ID3D12Resource& GetPreviousFrameBuffer()const { return *get<ID3D12Resource*>(m_frameBuffers)[GetPreviousFrameIndex()]; }

        FenceValue_t& GetCurrentFrameFenceValue() const { return get<FenceValue_t>(m_frameBuffers)[GetCurrentFrameIndex()]; }
        FenceValue_t& GetPreviousFrameFenceValue() const { return get<FenceValue_t>(m_frameBuffers)[GetPreviousFrameIndex()]; }

        size_t GetBufferCount() const { return m_frameBuffers.size(); }

        UINT GetCurrentFrameIndex() const { return m_backBufferIndex; }
        UINT GetPreviousFrameIndex() const { return (std::min)(m_backBufferIndex - 1, static_cast<UINT>(m_frameBuffers.size() - 1)); }

        D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentFrameBufferHandle() const { return m_backBufferRTVHandle; }
    };

    template<std::derived_from<IDXGISwapChain> SwapChainTy, std::invocable<FrameData&> Func>
    void Frame(SwapChainTy& swapChain,
        ID3D12CommandQueue& commandQueue,
        ID3D12Fence& fence,
        xk::span_tuple<ID3D12Resource*, std::dynamic_extent, UINT64, D3D12_CPU_DESCRIPTOR_HANDLE> frameBuffers,
        UINT64& highestFenceValue,
        UINT& backBufferIndex,
        Func&& func,
        UINT syncInterval = 0,
        UINT presentFlags = 0,
        HANDLE waitEvent = nullptr,
        std::chrono::milliseconds waitInterval = waitForCompletion)
    {
        FrameData(commandQueue, 
            fence,
            xk::span_tuple<ID3D12Resource*, std::dynamic_extent, UINT64>(get<0>(frameBuffers).begin(), get<0>(frameBuffers).end(), get<1>(frameBuffers).begin()),
            highestFenceValue, 
            backBufferIndex, 
            get<2>(frameBuffers)[backBufferIndex],
            swapChain, 
            syncInterval, 
            presentFlags, 
            waitEvent, 
            waitInterval, 
            std::forward<Func>(func));
    }

    /// <summary>
    /// 
    /// </summary>
    /// <typeparam name="SwapChainTy"></typeparam>
    /// <typeparam name="Func"></typeparam>
    /// <param name="swapChain">The swap chain we will call Present with</param>
    /// <param name="commandQueue">The command queue associated with the swap chain</param>
    /// <param name="fence">The fence the associated command queue will signal</param>
    /// <param name="frameBuffers">The buffers of the swap chain and the fence values associated to a frame</param>
    /// <param name="frameBufferRTVStartHandle">A RTV Descriptor Heap handle to the first buffer, assumes other buffers are layed out sequentially in the heap</param>
    /// <param name="rtvOffset">ID3D12Device.GetDescriptorHandleIncrementSize() of type RTV</param>
    /// <param name="highestFenceValue">A external fence value that will always have the same value as the frame that currently has the highest fence value</param>
    /// <param name="backBufferIndex">The current back buffer</param>
    /// <param name="func"></param>
    /// <param name="syncInterval"></param>
    /// <param name="presentFlags"></param>
    /// <param name="waitEvent">The syncronization primitive for the fence</param>
    /// <param name="waitInterval"></param>
    template<std::derived_from<IDXGISwapChain> SwapChainTy, std::invocable<FrameData&> Func>
    void Frame(SwapChainTy& swapChain,
        ID3D12CommandQueue& commandQueue,
        ID3D12Fence& fence,
        xk::span_tuple<ID3D12Resource*, std::dynamic_extent, UINT64> frameBuffers,
        D3D12_CPU_DESCRIPTOR_HANDLE frameBufferRTVStartHandle, 
        UINT64 rtvOffset,
        UINT64& highestFenceValue,
        UINT& backBufferIndex,
        Func&& func,
        UINT syncInterval = 0,
        UINT presentFlags = 0,
        HANDLE waitEvent = nullptr,
        std::chrono::milliseconds waitInterval = waitForCompletion)
    {
        frameBufferRTVStartHandle.ptr += rtvOffset * backBufferIndex;
        FrameData(commandQueue,
            fence,
            xk::span_tuple<ID3D12Resource*, std::dynamic_extent, UINT64>(get<0>(frameBuffers).begin(), get<0>(frameBuffers).end(), get<1>(frameBuffers).begin()),
            highestFenceValue,
            backBufferIndex,
            frameBufferRTVStartHandle,
            swapChain,
            syncInterval,
            presentFlags,
            waitEvent,
            waitInterval,
            std::forward<Func>(func));
    }

    D3D12_RESOURCE_BARRIER TransitionBarrier(ID3D12Resource& resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE, UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
    D3D12_RESOURCE_BARRIER AliasingBarrier(ID3D12Resource* optResourceBefore, ID3D12Resource* optResourceAfter, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);
    D3D12_RESOURCE_BARRIER UAVBarrier(ID3D12Resource& resource, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);

    template<class DebugTy = ID3D12Debug>
    tl::expected<Microsoft::WRL::ComPtr<DebugTy>, HRESULT> GetDebugInterface()
    {
        if constexpr(std::same_as<DebugTy, ID3D12Debug>)
            return Helpers::COM::IIDToObjectForwardFunction<ID3D12Debug>(&D3D12GetDebugInterface);
        else
        {
            auto debug = GetDebugInterface<ID3D12Debug>();
            if(!debug.has_value())
                return tl::unexpected(debug.error());

            return Helpers::COM::Cast<DebugTy>(debug.value());
        }
    }
}