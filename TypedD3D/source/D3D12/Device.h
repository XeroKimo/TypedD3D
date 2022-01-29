#pragma once
#include "../Helpers/D3D12Helpers.h"
#include "../Utils.h"
#include "ComWrapper.h"
#include "CommandList.h"
#include "CommandAllocator.h"
#include "CommandQueue.h"
#include "PipelineState.h"
#include "DescriptorHeap.h"
#include "Meta.h"

#include <d3d12.h>
#include <wrl/client.h>

namespace TypedD3D::D3D12
{
    using Microsoft::WRL::ComPtr;

    namespace Internal
    {
        template<class Ty>
        class Device;

        template<class WrapperTy, class Ty>
        class DeviceInterface;

        template<class WrapperTy>
        class DeviceInterface<WrapperTy, ID3D12Device>
        {
        private:
            using type = ID3D12Device;

        public:
            UINT GetNodeCount()
            {
                return InternalGetDevice().GetNodeCount();
            }

            template<D3D12_COMMAND_LIST_TYPE Type>
            Utils::Expected<CommandQueue::Internal::CommandQueue<Type>, HRESULT> CreateCommandQueue(
                D3D12_COMMAND_QUEUE_PRIORITY priority,
                D3D12_COMMAND_QUEUE_FLAGS flags,
                UINT nodeMask)
            {
                using queue_type = CommandQueue::Internal::CommandQueue<Type>;

                D3D12_COMMAND_QUEUE_DESC desc
                {
                    .Type = Type,
                    .Priority = priority,
                    .Flags = flags,
                    .NodeMask = nodeMask
                };

                auto commandQueue = Helpers::D3D12::CreateCommandQueue(InternalGetDevice(), desc);

                if(!commandQueue)
                    Utils::Unexpected(commandQueue.GetError());

                return queue_type(commandQueue.GetValue());
            }

            template<D3D12_COMMAND_LIST_TYPE Type>
            Utils::Expected<CommandAllocator::Internal::CommandAllocator<Type>, HRESULT> CreateCommandAllocator()
            {
                using allocator_type = CommandAllocator::Internal::CommandAllocator<Type>;
                auto commandAllocator = Helpers::D3D12::CreateCommandAllocator(InternalGetDevice(), Type);

                if(!commandAllocator)
                    Utils::Unexpected(commandAllocator.GetError());

                return allocator_type(commandAllocator.GetValue());
            }

            Utils::Expected<PipelineState::Graphics, HRESULT> CreateGraphicsPipelineState(
                const D3D12_GRAPHICS_PIPELINE_STATE_DESC& pDesc)
            {
                auto pipelineState = Helpers::D3D12::CreateGraphicsPipelineState(InternalGetDevice(), pDesc);

                if(!pipelineState)
                    Utils::Unexpected(pipelineState.GetError());

                return PipelineState::Graphics(pipelineState.GetValue());
            }

            Utils::Expected<PipelineState::Compute, HRESULT> CreateComputePipelineState(
                const D3D12_COMPUTE_PIPELINE_STATE_DESC& pDesc)
            {
                auto pipelineState = Helpers::D3D12::CreateComputePipelineState(InternalGetDevice(), pDesc);

                if(!pipelineState)
                    Utils::Unexpected(pipelineState.GetError());

                return PipelineState::Compute(pipelineState.GetValue());
            }

            template<D3D12_COMMAND_LIST_TYPE Type>
            Utils::Expected<CommandList::CommandList_t<Type, ID3D12GraphicsCommandList>, HRESULT> CreateCommandList(
                CommandAllocator::CommandAllocator_t<Type> pCommandAllocator,
                UINT nodeMask = 0,
                ID3D12PipelineState* optInitialState = nullptr)
            {
                using command_list_type = CommandList::CommandList_t<Type, ID3D12GraphicsCommandList>;
                auto commandList = Helpers::D3D12::CreateCommandList<ID3D12GraphicsCommandList>(InternalGetDevice(), command_list_type::value, *pCommandAllocator.Get(), nodeMask, optInitialState);

                if(!commandList)
                    Utils::Unexpected(commandList.GetError());

                return command_list_type(commandList.GetValue());
            }

            template<D3D12_FEATURE Feature>
            Utils::Expected<typename Meta::DeviceFeatureToType<Feature>::type, HRESULT> CheckFeatureSupport()
            {
                using feature_t = typename Meta::DeviceFeatureToType<Feature>::type;
                feature_t feature{};

                HRESULT hr = InternalGetDevice().CheckFeatureSupport(Feature, &feature, sizeof(feature_t));

                if(FAILED(hr))
                    return Utils::Unexpected(hr);

                return feature;
            }

            template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
            Utils::Expected<DescriptorHeap::DescriptorHeap_t<Type>, HRESULT> CreateDescriptorHeap(
                UINT NumDescriptors,
                D3D12_DESCRIPTOR_HEAP_FLAGS Flags,
                UINT NodeMask)
            {
                using DescriptorHeap_t = DescriptorHeap::DescriptorHeap_t<Type>;

                D3D12_DESCRIPTOR_HEAP_DESC desc
                {
                    .Type = Type,
                    .NumDescriptors = NumDescriptors,
                    .Flags = Flags,
                    .NodeMask = NodeMask
                };

                auto descriptorHeap = Helpers::D3D12::CreateDescriptorHeap(InternalGetDevice(), desc);

                if(!descriptorHeap)
                    return Utils::Unexpected(descriptorHeap.GetError());

                return DescriptorHeap_t(descriptorHeap.GetValue());
            }

            UINT GetDescriptorHandleIncrementSize(
                D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType)
            {
                return InternalGetDevice().GetDescriptorHandleIncrementSize(DescriptorHeapType);
            }

            Utils::Expected<ComPtr<ID3D12RootSignature>, HRESULT> CreateRootSignature(
                UINT nodeMask,
                const void* pBlobWithRootSignature,
                SIZE_T blobLengthInBytes)
            {
                return Helpers::D3D12::CreateRootSignature(InternalGetDevice(), nodeMask, pBlobWithRootSignature, blobLengthInBytes);
            }

            void CreateConstantBufferView(
                const D3D12_CONSTANT_BUFFER_VIEW_DESC& pDesc,
                DescriptorHandle::CPU_CBV_SRV_UAV DestDescriptor)
            {
                InternalGetDevice().CreateConstantBufferView(pDesc, DestDescriptor.Data());
            }


            void CreateShaderResourceView(
                ID3D12Resource& pResource,
                const D3D12_SHADER_RESOURCE_VIEW_DESC* optDesc,
                DescriptorHandle::CPU_CBV_SRV_UAV DestDescriptor)
            {
                InternalGetDevice().CreateShaderResourceView(&pResource, optDesc, DestDescriptor.Data());
            }

            void CreateNullShaderResourceView(
                const D3D12_SHADER_RESOURCE_VIEW_DESC& pDesc,
                DescriptorHandle::CPU_CBV_SRV_UAV DestDescriptor)
            {
                InternalGetDevice().CreateShaderResourceView(nullptr, &pDesc, DestDescriptor.Data());
            }

            void CreateUnorderedAccessView(
                ID3D12Resource& pResource,
                ID3D12Resource& pCounterResource,
                const D3D12_UNORDERED_ACCESS_VIEW_DESC& pDesc,
                DescriptorHandle::CPU_CBV_SRV_UAV DestDescriptor)
            {
                InternalGetDevice().CreateUnorderedAccessView(&pResource, &pCounterResource, &pDesc, DestDescriptor.Data());
            }

            void CreateRenderTargetView(
                ID3D12Resource& Resource,
                const D3D12_RENDER_TARGET_VIEW_DESC* optDesc,
                DescriptorHandle::CPU_RTV DestDescriptor)
            {
                InternalGetDevice().CreateRenderTargetView(&Resource, optDesc, DestDescriptor.Data());
            }

            void CreateNullRenderTargetView(
                const D3D12_RENDER_TARGET_VIEW_DESC& Desc,
                DescriptorHandle::CPU_RTV DestDescriptor)
            {
                InternalGetDevice().CreateRenderTargetView(nullptr, &Desc, DestDescriptor.Data());
            }

            void CreateDepthStencilView(
                ID3D12Resource& Resource,
                const D3D12_DEPTH_STENCIL_VIEW_DESC* optDesc,
                DescriptorHandle::CPU_DSV DestDescriptor)
            {
                InternalGetDevice().CreateDepthStencilView(&Resource, optDesc, DestDescriptor.Data());
            }

            void CreateNullDepthStencilView(
                const D3D12_DEPTH_STENCIL_VIEW_DESC& optDesc,
                DescriptorHandle::CPU_DSV DestDescriptor)
            {
                InternalGetDevice().CreateDepthStencilView(nullptr, optDesc, DestDescriptor.Data());
            }

            void CreateSampler(
                const D3D12_SAMPLER_DESC& pDesc,
                DescriptorHandle::CPU_SAMPLER DestDescriptor)
            {
                InternalGetDevice().CreateSampler(&pDesc, DestDescriptor.Data());
            }


            void CopyDescriptors(
                UINT NumDestDescriptorRanges,
                const D3D12_CPU_DESCRIPTOR_HANDLE* pDestDescriptorRangeStarts,
                const UINT* pDestDescriptorRangeSizes,
                UINT NumSrcDescriptorRanges,
                const D3D12_CPU_DESCRIPTOR_HANDLE* pSrcDescriptorRangeStarts,
                const UINT* pSrcDescriptorRangeSizes,
                D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType)
            {
                InternalGetDevice().CopyDescriptors(
                    NumDestDescriptorRanges,
                    pDestDescriptorRangeStarts,
                    pDestDescriptorRangeSizes,
                    NumSrcDescriptorRanges,
                    pSrcDescriptorRangeStarts,
                    pSrcDescriptorRangeSizes,
                    DescriptorHeapsType);
            }

            template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
            void CopyDescriptorsSimple(
                UINT NumDescriptors,
                DescriptorHandle::CPU_t<Type> DestDescriptorRangeStart,
                DescriptorHandle::CPU_t<Type> SrcDescriptorRangeStart)
            {
                InternalGetDevice().CopyDescriptorsSimple(NumDescriptors, DestDescriptorRangeStart.Data(), SrcDescriptorRangeStart.Data(), Type);
            }

            D3D12_RESOURCE_ALLOCATION_INFO GetResourceAllocationInfo(
                UINT visibleMask,
                const std::span<D3D12_RESOURCE_DESC> resourceDescs)
            {
                return InternalGetDevice().GetResourceAllocationInfo(
                    visibleMask, 
                    static_cast<UINT>(resourceDescs.size()), 
                    resourceDescs.data());
            }

            D3D12_HEAP_PROPERTIES GetCustomHeapProperties(
                UINT nodeMask,
                D3D12_HEAP_TYPE heapType)
            {
                return InternalGetDevice().GetCustomHeapProperties(nodeMask, heapType);
            }

            Utils::Expected<ComPtr<ID3D12Resource>, HRESULT> CreateCommittedResource(
                const D3D12_HEAP_PROPERTIES& pHeapProperties,
                D3D12_HEAP_FLAGS HeapFlags,
                const D3D12_RESOURCE_DESC& pDesc,
                D3D12_RESOURCE_STATES InitialResourceState,
                const D3D12_CLEAR_VALUE* optOptimizedClearValue)
            {
                return Helpers::D3D12::CreateCommittedResource(
                    InternalGetDevice(),
                    pHeapProperties,
                    HeapFlags,
                    pDesc,
                    InitialResourceState,
                    optOptimizedClearValue);
            }

            Utils::Expected<ComPtr<ID3D12Heap>, HRESULT> CreateHeap(
                const D3D12_HEAP_DESC& pDesc)
            {
                return Helpers::D3D12::CreateHeap(
                    InternalGetDevice(),
                    pDesc);
            }

            Utils::Expected<ComPtr<ID3D12Resource>, HRESULT> CreatePlacedResource(
                ID3D12Heap& pHeap,
                UINT64 HeapOffset,
                const D3D12_RESOURCE_DESC& pDesc,
                D3D12_RESOURCE_STATES InitialState,
                const D3D12_CLEAR_VALUE* optOptimizedClearValue)
            {
                return Helpers::D3D12::CreatePlacedResource(
                    InternalGetDevice(),
                    pHeap,
                    HeapOffset,
                    pDesc,
                    InitialState,
                    optOptimizedClearValue);
            }

            Utils::Expected<ComPtr<ID3D12Resource>, HRESULT> CreateReservedResource(
                const D3D12_RESOURCE_DESC& pDesc,
                D3D12_RESOURCE_STATES InitialState,
                const D3D12_CLEAR_VALUE* optOptimizedClearValue)
            {
                return Helpers::D3D12::CreateReservedResource(
                    InternalGetDevice(),
                    pDesc,
                    InitialState,
                    optOptimizedClearValue);
            }

            Utils::Expected<HANDLE, HRESULT> CreateSharedHandle(
                ID3D12DeviceChild& pObject,
                const SECURITY_ATTRIBUTES* optAttributes,
                DWORD Access,
                LPCWSTR Name)
            {
                HANDLE handle;

                HRESULT result = InternalGetDevice().CreateSharedHandle(
                    &pObject,
                    optAttributes,
                    Access,
                    Name,
                    &handle);

                if(FAILED(result))
                    return Utils::Unexpected(result);

                return handle;
            }

            HRESULT OpenSharedHandle(
                HANDLE NTHandle,
                const IID& riid,
                void** ppvObj)
            {
                return InternalGetDevice().OpenSharedHandle(
                    NTHandle,
                    riid,
                    ppvObj);
            }

            Utils::Expected<HANDLE, HRESULT> OpenSharedHandleByName(
                LPCWSTR Name,
                DWORD Access)
            {
                HANDLE handle;

                HRESULT result = InternalGetDevice().OpenSharedHandleByName(
                    Name,
                    Access,
                    &handle);

                if(FAILED(result))
                    return Utils::Unexpected(result);

                return handle;
            }

            HRESULT MakeResident(
                const std::span<ID3D12Pageable*> objects)
            {
                return InternalGetDevice().MakeResident(
                    static_cast<UINT>(objects.size()),
                    objects.data());
            }

            HRESULT Evict(
                const std::span<ID3D12Pageable*> objects)
            {
                return InternalGetDevice().Evict(
                    static_cast<UINT>(objects.size()),
                    objects.data());
            }

            Utils::Expected<ComPtr<ID3D12Fence>, HRESULT> CreateFence(
                UINT64 InitialValue,
                D3D12_FENCE_FLAGS Flags)
            {
                return Helpers::D3D12::CreateFence(InternalGetDevice(), Flags, InitialValue);
            }

            Utils::Expected<int, HRESULT> GetDeviceRemovedReason()
            {
                return InternalGetDevice().GetDeviceRemovedReason();
            }

            void GetCopyableFootprints(
                const D3D12_RESOURCE_DESC& pResourceDesc,
                UINT FirstSubresource,
                UINT NumSubresources,
                UINT64 BaseOffset,
                D3D12_PLACED_SUBRESOURCE_FOOTPRINT* optOutLayouts,
                UINT* optOutNumRows,
                UINT64* optOutRowSizeInBytes,
                UINT64* optOutTotalBytes)
            {
                InternalGetDevice().GetCopyableFootprints(
                    &pResourceDesc,
                    FirstSubresource,
                    NumSubresources,
                    BaseOffset,
                    optOutLayouts,
                    optOutNumRows,
                    optOutRowSizeInBytes,
                    optOutTotalBytes);
            }

            Utils::Expected<ComPtr<ID3D12QueryHeap>, HRESULT> CreateQueryHeap(
                const D3D12_QUERY_HEAP_DESC& pDesc)
            {
                return Helpers::D3D12::CreateQueryHeap(InternalGetDevice(), pDesc);
            }

            HRESULT SetStablePowerState(
                BOOL Enable)
            {
                return InternalGetDevice().SetStablePowerState(Enable);
            }

            Utils::Expected<ComPtr<ID3D12CommandSignature>, HRESULT> CreateCommandSignature(
                const D3D12_COMMAND_SIGNATURE_DESC& pDesc,
                ID3D12RootSignature* optRootSignature)
            {
                return Helpers::D3D12::CreateCommandSignature(InternalGetDevice(), pDesc, optRootSignature);
            }

            void GetResourceTiling(
                ID3D12Resource& pTiledResource,
                UINT* optOutNumTilesForEntireResource,
                D3D12_PACKED_MIP_INFO* optOutPackedMipDesc,
                D3D12_TILE_SHAPE* optOutStandardTileShapeForNonPackedMips,
                UINT* pNumSubresourceTilings,
                UINT FirstSubresourceTilingToGet,
                D3D12_SUBRESOURCE_TILING& pSubresourceTilingsForNonPackedMips)
            {
                InternalGetDevice().GetResourceTiling(
                    &pTiledResource,
                    optOutNumTilesForEntireResource,
                    optOutPackedMipDesc,
                    optOutStandardTileShapeForNonPackedMips,
                    pNumSubresourceTilings,
                    FirstSubresourceTilingToGet,
                    &pSubresourceTilingsForNonPackedMips);
            }

            LUID GetAdapterLuid()
            {
                return InternalGetDevice().GetAdapterLuid();
            }

        private:
            type& InternalGetDevice() { return *static_cast<WrapperTy&>(*this).Get(); }
        };

        template<class Ty>
        class Device : public ComWrapper<Ty>, public DeviceInterface<Device<Ty>, Ty>
        {
        public:
            using ComWrapper<Ty>::ComWrapper;
            using value_type = Ty;
        };
    }


    using Device = Internal::Device<ID3D12Device>;

    template<class DeviceTy = Device>
    Utils::Expected<Device, HRESULT> CreateDevice(D3D_FEATURE_LEVEL minimumFeatureLevel, IDXGIAdapter* optAdapter = nullptr)
    {
        Utils::Expected<ComPtr<ID3D12Device>, HRESULT> device = Helpers::D3D12::CreateDevice(minimumFeatureLevel, optAdapter);

        if(!device)
            return Utils::Unexpected(device.GetError());

        if constexpr(std::same_as<typename DeviceTy::value_type, ID3D12Device>)
            return DeviceTy(device.GetValue());
        else
        {
            using DerivedDevice = typename DeviceTy::value_type;
            ComPtr<DerivedDevice> derived = Helpers::COM::Cast<DerivedDevice>(device.GetValue());
            return DeviceTy(derived);
        }
    }
}