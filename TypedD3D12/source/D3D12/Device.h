#pragma once
#include "../Helpers/D3D12Helpers.h"
#include "../Utils.h"
#include "ComWrapper.h"
#include "CommandList.h"
#include "CommandAllocator.h"
#include "CommandQueue.h"
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

            Utils::Expected<ComPtr<ID3D12PipelineState>, HRESULT> CreateGraphicsPipelineState(
                const D3D12_GRAPHICS_PIPELINE_STATE_DESC& pDesc)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<ComPtr<ID3D12PipelineState>, HRESULT> CreateComputePipelineState(
                const D3D12_COMPUTE_PIPELINE_STATE_DESC& pDesc)
            {
                return Utils::Unexpected(HRESULT(0));
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

            Utils::Expected<ComPtr<ID3D12DescriptorHeap>, HRESULT> CreateDescriptorHeap(
                const D3D12_DESCRIPTOR_HEAP_DESC& pDescriptorHeapDesc)
            {
                return Helpers::D3D12::CreateDescriptorHeap(InternalGetDevice(), pDescriptorHeapDesc);
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
                const D3D12_CONSTANT_BUFFER_VIEW_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
            {
            }

            void CreateShaderResourceView(
                ID3D12Resource* pResource,
                const D3D12_SHADER_RESOURCE_VIEW_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
            {
            }

            void CreateUnorderedAccessView(
                ID3D12Resource* pResource,
                ID3D12Resource* pCounterResource,
                const D3D12_UNORDERED_ACCESS_VIEW_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
            {
            }

            void CreateRenderTargetView(
                ID3D12Resource* pResource,
                const D3D12_RENDER_TARGET_VIEW_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
            {
            }

            void CreateDepthStencilView(
                ID3D12Resource* pResource,
                const D3D12_DEPTH_STENCIL_VIEW_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
            {
            }

            void CreateSampler(
                const D3D12_SAMPLER_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
            {
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
            }

            void CopyDescriptorsSimple(
                UINT NumDescriptors,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptorRangeStart,
                D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorRangeStart,
                D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType)
            {
            }

            D3D12_RESOURCE_ALLOCATION_INFO GetResourceAllocationInfo(
                UINT visibleMask,
                UINT numResourceDescs,
                const D3D12_RESOURCE_DESC* pResourceDescs)
            {
                return {};
            }

            D3D12_HEAP_PROPERTIES GetCustomHeapProperties(
                UINT nodeMask,
                D3D12_HEAP_TYPE heapType)
            {
                return {};
            }

            Utils::Expected<ComPtr<ID3D12Resource>, HRESULT> CreateCommittedResource(
                const D3D12_HEAP_PROPERTIES* pHeapProperties,
                D3D12_HEAP_FLAGS HeapFlags,
                const D3D12_RESOURCE_DESC* pDesc,
                D3D12_RESOURCE_STATES InitialResourceState,
                const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                const IID& riidResource,
                void** ppvResource)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<ComPtr<ID3D12Heap>, HRESULT> CreateHeap(
                const D3D12_HEAP_DESC* pDesc,
                const IID& riid,
                void** ppvHeap)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<ComPtr<ID3D12Resource>, HRESULT> CreatePlacedResource(
                ID3D12Heap* pHeap,
                UINT64 HeapOffset,
                const D3D12_RESOURCE_DESC* pDesc,
                D3D12_RESOURCE_STATES InitialState,
                const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                const IID& riid,
                void** ppvResource)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<ComPtr<ID3D12Resource>, HRESULT> CreateReservedResource(
                const D3D12_RESOURCE_DESC* pDesc,
                D3D12_RESOURCE_STATES InitialState,
                const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                const IID& riid,
                void** ppvResource)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<int, HRESULT> CreateSharedHandle(
                ID3D12DeviceChild* pObject,
                const SECURITY_ATTRIBUTES* pAttributes,
                DWORD Access,
                LPCWSTR Name,
                HANDLE* pHandle)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<int, HRESULT> OpenSharedHandle(
                HANDLE NTHandle,
                const IID& riid,
                void** ppvObj)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<int, HRESULT> OpenSharedHandleByName(
                LPCWSTR Name,
                DWORD Access,
                HANDLE* pNTHandle)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            HRESULT MakeResident(
                UINT NumObjects,
                ID3D12Pageable* const* ppObjects)
            {
                return HRESULT(0);
            }

            HRESULT Evict(
                UINT NumObjects,
                ID3D12Pageable* const* ppObjects)

            {
                return HRESULT(0);
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
                const D3D12_RESOURCE_DESC* pResourceDesc,
                UINT FirstSubresource,
                UINT NumSubresources,
                UINT64 BaseOffset,
                D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
                UINT* pNumRows,
                UINT64* pRowSizeInBytes,
                UINT64* pTotalBytes)
            {
            }

            Utils::Expected<ComPtr<ID3D12QueryHeap>, HRESULT> CreateQueryHeap(
                const D3D12_QUERY_HEAP_DESC* pDesc)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            HRESULT SetStablePowerState(
                BOOL Enable)
            {
                return InternalGetDevice().SetStablePowerState(Enable);
            }

            Utils::Expected<ComPtr<ID3D12CommandSignature>, HRESULT> CreateCommandSignature(
                const D3D12_COMMAND_SIGNATURE_DESC* pDesc,
                ID3D12RootSignature* pRootSignature,
                const IID& riid,
                void** ppvCommandSignature)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            void GetResourceTiling(
                ID3D12Resource* pTiledResource,
                UINT* pNumTilesForEntireResource,
                D3D12_PACKED_MIP_INFO* pPackedMipDesc,
                D3D12_TILE_SHAPE* pStandardTileShapeForNonPackedMips,
                UINT* pNumSubresourceTilings,
                UINT FirstSubresourceTilingToGet,
                D3D12_SUBRESOURCE_TILING* pSubresourceTilingsForNonPackedMips)
            {
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

        };
    }


    using Device = Internal::Device<ID3D12Device>;


}