#pragma once
#include "../Helpers/D3D12Helpers.h"
#include "../Utils.h"
#include "ComWrapper.h"
#include "CommandList.h"
#include "CommandAllocator.h"
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

            Utils::Expected<ComPtr<ID3D12CommandQueue>, HRESULT> CreateCommandQueue(
                const D3D12_COMMAND_QUEUE_DESC& pDesc)
            {
                return Helpers::D3D12::CreateCommandQueue(InternalGetDevice(), pDesc);
            }

            Utils::Expected<ComPtr<ID3D12CommandAllocator>, HRESULT> CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE type)
            {
                return Helpers::D3D12::CreateCommandAllocator(InternalGetDevice(), type);
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

            Utils::Expected<CommandList::Direct, HRESULT> CreateDirectCommandList(
                CommandAllocator::Direct pCommandAllocator,
                UINT nodeMask = 0,
                ID3D12PipelineState* optInitialState = nullptr)
            {
                auto commandList = Helpers::D3D12::CreateCommandList<ID3D12GraphicsCommandList>(InternalGetDevice(), CommandList::Direct::type, pCommandAllocator.Get().Get(), nodeMask, optInitialState);

                if(!commandList)
                    Utils::Unexpected(commandList.GetError());

                return CommandList::Direct(commandList.GetValue());
            }

            Utils::Expected<CommandList::Bundle, HRESULT> CreateBundleCommandList(
                UINT nodeMask,
                CommandAllocator::Bundle pCommandAllocator,
                ID3D12PipelineState* optInitialState)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<CommandList::Compute, HRESULT> CreateComputeCommandList(
                UINT nodeMask,
                CommandAllocator::Compute pCommandAllocator,
                ID3D12PipelineState* optInitialState)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<CommandList::Copy, HRESULT> CreateCopyCommandList(
                UINT nodeMask,
                CommandAllocator::Copy pCommandAllocator,
                ID3D12PipelineState* optInitialState)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            //TODO: return a Utils::Expected<auto, HRESULT> where auto depends on the D3D12_FEATURE passed in
            template<D3D12_FEATURE Feature>
            auto CheckFeatureSupport()
            {
                using feature_t = typename Meta::DeviceFeatureToType<Feature>::type;
                feature_t feature{};

                HRESULT hr = InternalGetDevice().CheckFeatureSupport(Feature, &feature, sizeof(feature_t));

                if(FAILED(hr))
                    return Utils::Expected<feature_t, HRESULT>(Utils::Unexpected(hr));

                return Utils::Expected<feature_t, HRESULT>(feature);
            }

            Utils::Expected<ComPtr<ID3D12DescriptorHeap>, HRESULT> CreateDescriptorHeap(
                const D3D12_DESCRIPTOR_HEAP_DESC* pDescriptorHeapDesc,
                const IID& riid,
                void** ppvHeap)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            UINT GetDescriptorHandleIncrementSize(
                D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType)
            {
                return {};
            }

            Utils::Expected<ComPtr<ID3D12RootSignature>, HRESULT> CreateRootSignature(
                UINT nodeMask,
                const void* pBlobWithRootSignature,
                SIZE_T blobLengthInBytes)
            {
                return Utils::Unexpected(HRESULT(0));
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

            Utils::Expected<int, HRESULT> MakeResident(
                UINT NumObjects,
                ID3D12Pageable* const* ppObjects)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<int, HRESULT> Evict(
                UINT NumObjects,
                ID3D12Pageable* const* ppObjects)

            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<ComPtr<ID3D12Fence>, HRESULT> CreateFence(
                UINT64 InitialValue,
                D3D12_FENCE_FLAGS Flags,
                const IID& riid,
                void** ppFence)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<int, HRESULT> GetDeviceRemovedReason()
            {
                return Utils::Unexpected(HRESULT(0));
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

            Utils::Expected<int, HRESULT> CreateQueryHeap(
                const D3D12_QUERY_HEAP_DESC* pDesc,
                const IID& riid,
                void** ppvHeap)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<int, HRESULT> SetStablePowerState(
                BOOL Enable)
            {
                return Utils::Unexpected(HRESULT(0));
            }

            Utils::Expected<int, HRESULT> CreateCommandSignature(
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
                return {};
            }

        private:
            type& InternalGetDevice() { return *static_cast<WrapperTy&>(*this).Get().Get(); }
        };

        template<class Ty>
        class Device : public ComWrapper<Ty>, public DeviceInterface<Device<Ty>, Ty>
        {

        };
    }


    using Device = Internal::Device<ID3D12Device>;


}