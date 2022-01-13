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
#include "../Utils.h"
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
    template<class Device = ID3D12Device>
    Utils::Expected<Microsoft::WRL::ComPtr<Device>, HRESULT> CreateDevice(D3D_FEATURE_LEVEL minFeatureLevel, IDXGIAdapter* optAdapter = nullptr)
    {
        using Microsoft::WRL::ComPtr;

        Utils::Expected<ComPtr<ID3D12Device>, HRESULT> device = Helpers::COM::IIDToObjectForwardFunction<ID3D12Device>(&D3D12CreateDevice, optAdapter, minFeatureLevel);

        if(!device)
            return Utils::Unexpected(device.GetError());

        if constexpr(std::is_same_v<Device, ID3D12Device>)
            return device.GetValue();
        else
            return COM::Cast<Device>(device.GetValue());
    }

    //Creates a open command list
    template<class CommandList = ID3D12GraphicsCommandList>
    Utils::Expected<Microsoft::WRL::ComPtr<CommandList>, HRESULT> CreateCommandList(ID3D12Device& device,  D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator& allocator, UINT nodeMask = 0, ID3D12PipelineState* optInitialPipeline = nullptr)
    {
        static_assert(std::is_base_of_v<ID3D12CommandList, CommandList>, "This function requires it's type to inherit from ID3D12CommandList");

        if constexpr(std::is_same_v<CommandList, ID3D12CommandList> || std::is_same_v<CommandList, ID3D12GraphicsCommandList>)
        {
            return Helpers::COM::IIDToObjectForwardFunction<CommandList>(&ID3D12Device::CreateCommandList, device, nodeMask, type, &allocator, optInitialPipeline);
        }
        else
        {
            auto commandList = Helpers::COM::IIDToObjectForwardFunction<ID3D12GraphicsCommandList>(&ID3D12Device::CreateCommandList, device, nodeMask, type, &allocator, optInitialPipeline);

            if(!commandList)
                return Utils::Unexpected(commandList.GetError());

            return COM::Cast<CommandList>(commandList.GetValue());
        }
    }
    
    //Creates a closed command list
    template<class CommandList = ID3D12GraphicsCommandList>
    Utils::Expected<Microsoft::WRL::ComPtr<CommandList>, HRESULT> CreateCommandList(ID3D12Device4& device,  D3D12_COMMAND_LIST_TYPE type, D3D12_COMMAND_LIST_FLAGS flags, UINT nodeMask = 0)
    {
        static_assert(std::is_base_of_v<ID3D12CommandList, CommandList>, "This function requires it's type to inherit from ID3D12CommandList");

        if constexpr(std::is_same_v<CommandList, ID3D12CommandList> || std::is_same_v<CommandList, ID3D12GraphicsCommandList>)
        {
            return Helpers::COM::IIDToObjectForwardFunction<CommandList>(&ID3D12Device4::CreateCommandList1, device, nodeMask, type, flags);
        }
        else
        {
            auto commandList = Helpers::COM::IIDToObjectForwardFunction<ID3D12GraphicsCommandList>(&ID3D12Device4::CreateCommandList1, device, nodeMask, type, flags);

            if(!commandList)
                return Utils::Unexpected(commandList.GetError());

            return COM::Cast<CommandList>(commandList.GetValue());
        }
    }

    inline Utils::Expected<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, HRESULT> CreateCommandAllocator(ID3D12Device& device, D3D12_COMMAND_LIST_TYPE type)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12CommandAllocator>(&ID3D12Device::CreateCommandAllocator, device, type);
    }

    inline Utils::Expected<Microsoft::WRL::ComPtr<ID3D12CommandQueue>, HRESULT> CreateCommandQueue(ID3D12Device& device, const D3D12_COMMAND_QUEUE_DESC& desc)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12CommandQueue>(&ID3D12Device::CreateCommandQueue, device, &desc);
    }

    inline Utils::Expected<Microsoft::WRL::ComPtr<ID3D12Fence>, HRESULT> CreateFence(ID3D12Device& device, D3D12_FENCE_FLAGS flags, UINT64 initialValue = 0)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12Fence>(&ID3D12Device::CreateFence, device, initialValue, flags);
    }

    inline Utils::Expected<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>, HRESULT> CreateDescriptorHeap(ID3D12Device& device, const D3D12_DESCRIPTOR_HEAP_DESC& descriptorHeapDesc)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12DescriptorHeap>(&ID3D12Device::CreateDescriptorHeap, device, &descriptorHeapDesc);
    }

    inline Utils::Expected<Microsoft::WRL::ComPtr<ID3D12RootSignature>, HRESULT> CreateRootSignature(ID3D12Device& device, UINT nodeMask, const void* pBlobWithRootSignature, SIZE_T blobLengthInBytes)
    {
        return Helpers::COM::IIDToObjectForwardFunction<ID3D12RootSignature>(&ID3D12Device::CreateRootSignature, device, nodeMask, pBlobWithRootSignature, blobLengthInBytes);
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
            buffers[i] = Helpers::DXGI::SwapChain::GetBuffer<Resource>(swapChain, i).GetValue();

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

    /// <summary>
    /// Resets the fence value to 0
    /// </summary>
    /// <param name="fence"></param>
    inline void ResetFence(ID3D12Fence& fence) { fence.Signal(0); }

    void FlushCommandQueue(ID3D12CommandQueue& commandQueue, ID3D12Fence& fence, UINT64& currentFenceValue, HANDLE waitEvent = nullptr);

#pragma push_macro("max")
#undef max

    constexpr std::chrono::milliseconds waitForCompletion = std::chrono::milliseconds::max();

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

    //Helper function to resize swap chain back buffers, should be called when chainging states
    template<class Resource = ID3D12Resource>
    std::vector<Microsoft::WRL::ComPtr<Resource>> ResizeSwapChain(ID3D12Device& device, IDXGISwapChain1& swapChain, ID3D12DescriptorHeap& descriptor, std::span<ID3D12CommandQueue*> commandQueues, std::span<ID3D12Fence*> fences, std::span<HANDLE> fenceHandles, std::span<UINT64*> fenceValues, UINT width, UINT height, std::optional<UINT> newBufferCount = std::nullopt)
    {
        assert(commandQueues.size() == fences.size());
        assert(fences.size() == fenceHandles.size());
        assert(fences.size() == fenceValues.size());

        //Flushes all command queues to make sure none is currently using swap chain buffers
        for(size_t i = 0; i < commandQueues.size(); i++)
        {
            FlushCommandQueue(*commandQueues[i], *fences[i], *fenceValues[i], fenceHandles[i]);
        }

        DXGI_SWAP_CHAIN_DESC1 desc = Common::GetDescription(swapChain);
        width = std::max<UINT>(1, width);
        height = std::max<UINT>(1, height);

        swapChain.ResizeBuffers(newBufferCount.value_or(desc.BufferCount), width, height, desc.Format, desc.Flags);

        return CreateSwapChainRenderTargets<Resource>(device, swapChain, descriptor);
    }

    //Helper function for chainging swap chain's fullscreen state
    template<class Resource = ID3D12Resource>
    std::vector<Microsoft::WRL::ComPtr<Resource>> SetSwapChainFullscreen(ID3D12Device& device, IDXGISwapChain1& swapChain, ID3D12DescriptorHeap& descriptor, std::span<ID3D12CommandQueue*> commandQueues, std::span<ID3D12Fence*> fences, std::span<HANDLE> fenceHandles, std::span<UINT64*> fenceValues, bool setFullScreenState, std::optional<UINT> newBufferCount = std::nullopt)
    {
        assert(commandQueues.size() == fences.size());
        assert(fences.size() == fenceHandles.size());
        assert(fences.size() == fenceValues.size());

        //Flushes all command queues to make sure none is currently using swap chain buffers
        for(size_t i = 0; i < commandQueues.size(); i++)
        {
            FlushCommandQueue(*commandQueues[i], *fences[i], *fenceValues[i], fenceHandles[i]);
        }

        swapChain.SetFullscreenState(setFullScreenState, nullptr);

        DXGI_SWAP_CHAIN_DESC1 desc = Common::GetDescription(swapChain);
        swapChain.ResizeBuffers(newBufferCount.value_or(desc.BufferCount), desc.Width, desc.Height, desc.Format, desc.Flags);

        return CreateSwapChainRenderTargets<Resource>(device, swapChain, descriptor);
    }
#pragma pop_macro("max")

    namespace ResourceBarrier
    {
        D3D12_RESOURCE_BARRIER Transition(ID3D12Resource& resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE, UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
        D3D12_RESOURCE_BARRIER Aliasing(ID3D12Resource* optResourceBefore, ID3D12Resource* optResourceAfter, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);
        D3D12_RESOURCE_BARRIER UAV(ID3D12Resource& resource, D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);
    }
}