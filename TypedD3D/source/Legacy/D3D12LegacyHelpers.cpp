//*********************************************************
//
// Copyright (c) 2022 Renzy Alarcon
// Licensed under the MIT License (MIT).
//
//*********************************************************
module;

#include <functional>
#include <type_traits>
#include <d3d12.h>
#include <chrono>
#include <span>
#include <assert.h>
#include <algorithm>
#include <optional>

module TypedD3D.Legacy.D3D12Helpers;

namespace TypedD3D::Helpers::D3D12
{
    UINT64 SignalFenceGPU(ID3D12CommandQueue& commandQueue, ID3D12Fence& fence, UINT64 currentFenceValue)
    {
        UINT64 waitValue = currentFenceValue + 1;
        commandQueue.Signal(&fence, waitValue);
        return waitValue;
    }

    void SignalFenceCPU(ID3D12Fence& fence, UINT64 fenceValue)
    {
        fence.Signal(fenceValue);
    }

    void FlushCommandQueue(ID3D12CommandQueue& commandQueue, ID3D12Fence& fence, UINT64& currentFenceValue, HANDLE waitEvent)
    {
        currentFenceValue = SignalFenceGPU(commandQueue, fence, currentFenceValue);
        StallCPUThread(fence, currentFenceValue, waitEvent);
    }

    void StallCPUThread(ID3D12Fence& fence, UINT64 fenceValue, HANDLE waitEvent, std::chrono::milliseconds waitInterval)
    {
        if(fence.GetCompletedValue() < fenceValue)
        {
            HRESULT hr = fence.SetEventOnCompletion(fenceValue, waitEvent);
            if(FAILED(hr) && hr == E_OUTOFMEMORY)
                throw std::runtime_error("Out of memory");

            if(waitEvent != nullptr)
            {
                while(WaitForSingleObject(waitEvent, static_cast<DWORD>(waitInterval.count())))
                {


                }
            }
        }
    }

    void StallCommandQueue(ID3D12CommandQueue& commandQueue, ID3D12Fence& fence, UINT64 fenceValue)
    {
        commandQueue.Wait(&fence, fenceValue);
    }

        D3D12_RESOURCE_BARRIER TransitionBarrier(ID3D12Resource& resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, D3D12_RESOURCE_BARRIER_FLAGS flags, UINT subresource)
        {
            return D3D12_RESOURCE_BARRIER
            {
                .Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
                .Flags = flags,
                .Transition = D3D12_RESOURCE_TRANSITION_BARRIER
                {
                    .pResource = &resource,
                    .Subresource = subresource,
                    .StateBefore = before,
                    .StateAfter = after
                }
            };
        }
        D3D12_RESOURCE_BARRIER AliasingBarrier(ID3D12Resource* optResourceBefore, ID3D12Resource* optResourceAfter, D3D12_RESOURCE_BARRIER_FLAGS flags)
        {
            return D3D12_RESOURCE_BARRIER
            {
                .Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING,
                .Flags = flags,
                .Aliasing = D3D12_RESOURCE_ALIASING_BARRIER
                {
                    .pResourceBefore = optResourceBefore,
                    .pResourceAfter = optResourceAfter
                }
            };
        }
        D3D12_RESOURCE_BARRIER UAVBarrier(ID3D12Resource& resource, D3D12_RESOURCE_BARRIER_FLAGS flags)
        {
            return D3D12_RESOURCE_BARRIER
            {
                .Type = D3D12_RESOURCE_BARRIER_TYPE_UAV,
                .Flags = flags,
                .UAV = D3D12_RESOURCE_UAV_BARRIER
                {
                    .pResource = &resource
                }
            };
        }
}

