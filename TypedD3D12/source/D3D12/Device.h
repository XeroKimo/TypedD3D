#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include "../Utils.h"
#include "CommandList.h"
#include "CommandAllocator.h"

namespace TypedD3D::D3D12::Device
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
            UINT GetNodeCount();

            Utils::Expected<ComPtr<ID3D12CommandQueue>, HRESULT> CreateCommandQueue(
                const D3D12_COMMAND_QUEUE_DESC* pDesc);

            Utils::Expected<ComPtr<ID3D12CommandAllocator>, HRESULT> CreateCommandAllocator(
                D3D12_COMMAND_LIST_TYPE type);

            Utils::Expected<ComPtr<ID3D12PipelineState>, HRESULT> CreateGraphicsPipelineState(
                const D3D12_GRAPHICS_PIPELINE_STATE_DESC& pDesc);

            Utils::Expected<ComPtr<ID3D12PipelineState>, HRESULT> CreateComputePipelineState(
                const D3D12_COMPUTE_PIPELINE_STATE_DESC& pDesc);

            Utils::Expected<CommandList::Direct, HRESULT> CreateDirectCommandList(
                UINT nodeMask,
                CommandAllocator::Direct pCommandAllocator,
                ID3D12PipelineState* optInitialState);

            Utils::Expected<CommandList::Bundle, HRESULT> CreateBundleCommandList(
                UINT nodeMask,
                CommandAllocator::Bundle pCommandAllocator,
                ID3D12PipelineState* optInitialState);

            Utils::Expected<CommandList::Compute, HRESULT> CreateComputeCommandList(
                UINT nodeMask,
                CommandAllocator::Compute pCommandAllocator,
                ID3D12PipelineState* optInitialState);

            Utils::Expected<CommandList::Copy, HRESULT> CreateCopyCommandList(
                UINT nodeMask,
                CommandAllocator::Copy pCommandAllocator,
                ID3D12PipelineState* optInitialState);

            //TODO: return a Utils::Expected<auto, HRESULT> where auto depends on the D3D12_FEATURE passed in
            template<D3D12_FEATURE Feature>
            auto CheckFeatureSupport(
                D3D12_FEATURE Feature,
                void* pFeatureSupportData,
                UINT FeatureSupportDataSize);

            Utils::Expected<ComPtr<ID3D12DescriptorHeap>, HRESULT> CreateDescriptorHeap(
                const D3D12_DESCRIPTOR_HEAP_DESC* pDescriptorHeapDesc,
                const IID& riid,
                void** ppvHeap);

            UINT GetDescriptorHandleIncrementSize(
                D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType);

            Utils::Expected<ComPtr<ID3D12RootSignature>, HRESULT> CreateRootSignature(
                UINT nodeMask,
                const void* pBlobWithRootSignature,
                SIZE_T blobLengthInBytes);

            void CreateConstantBufferView(
                const D3D12_CONSTANT_BUFFER_VIEW_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

            void CreateShaderResourceView(
                ID3D12Resource* pResource,
                const D3D12_SHADER_RESOURCE_VIEW_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

            void CreateUnorderedAccessView(
                ID3D12Resource* pResource,
                ID3D12Resource* pCounterResource,
                const D3D12_UNORDERED_ACCESS_VIEW_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

            void CreateRenderTargetView(
                ID3D12Resource* pResource,
                const D3D12_RENDER_TARGET_VIEW_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

            void CreateDepthStencilView(
                ID3D12Resource* pResource,
                const D3D12_DEPTH_STENCIL_VIEW_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

            void CreateSampler(
                const D3D12_SAMPLER_DESC* pDesc,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor);

            void CopyDescriptors(
                UINT NumDestDescriptorRanges,
                const D3D12_CPU_DESCRIPTOR_HANDLE* pDestDescriptorRangeStarts,
                const UINT* pDestDescriptorRangeSizes,
                UINT NumSrcDescriptorRanges,
                const D3D12_CPU_DESCRIPTOR_HANDLE* pSrcDescriptorRangeStarts,
                const UINT* pSrcDescriptorRangeSizes,
                D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType);

            void CopyDescriptorsSimple(
                UINT NumDescriptors,
                D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptorRangeStart,
                D3D12_CPU_DESCRIPTOR_HANDLE SrcDescriptorRangeStart,
                D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapsType);

            D3D12_RESOURCE_ALLOCATION_INFO GetResourceAllocationInfo(
                UINT visibleMask,
                UINT numResourceDescs,
                const D3D12_RESOURCE_DESC* pResourceDescs);

            D3D12_HEAP_PROPERTIES GetCustomHeapProperties(
                UINT nodeMask,
                D3D12_HEAP_TYPE heapType);

            Utils::Expected<, HRESULT> CreateCommittedResource(
                const D3D12_HEAP_PROPERTIES* pHeapProperties,
                D3D12_HEAP_FLAGS HeapFlags,
                const D3D12_RESOURCE_DESC* pDesc,
                D3D12_RESOURCE_STATES InitialResourceState,
                const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                const IID& riidResource,
                void** ppvResource);

            Utils::Expected<, HRESULT> CreateHeap(
                const D3D12_HEAP_DESC* pDesc,
                const IID& riid,
                void** ppvHeap);

            Utils::Expected<, HRESULT> CreatePlacedResource(
                ID3D12Heap* pHeap,
                UINT64 HeapOffset,
                const D3D12_RESOURCE_DESC* pDesc,
                D3D12_RESOURCE_STATES InitialState,
                const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                const IID& riid,
                void** ppvResource);

            Utils::Expected<, HRESULT> CreateReservedResource(
                const D3D12_RESOURCE_DESC* pDesc,
                D3D12_RESOURCE_STATES InitialState,
                const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                const IID& riid,
                void** ppvResource);

            Utils::Expected<, HRESULT> CreateSharedHandle(
                ID3D12DeviceChild* pObject,
                const SECURITY_ATTRIBUTES* pAttributes,
                DWORD Access,
                LPCWSTR Name,
                HANDLE* pHandle);

            Utils::Expected<, HRESULT> OpenSharedHandle(
                HANDLE NTHandle,
                const IID& riid,
                void** ppvObj);

            Utils::Expected<, HRESULT> OpenSharedHandleByName(
                LPCWSTR Name,
                DWORD Access,
                HANDLE* pNTHandle);

            Utils::Expected<, HRESULT> MakeResident(
                UINT NumObjects,
                ID3D12Pageable* const* ppObjects);

            Utils::Expected<, HRESULT> Evict(
                UINT NumObjects,
                ID3D12Pageable* const* ppObjects);

            Utils::Expected<, HRESULT> CreateFence(
                UINT64 InitialValue,
                D3D12_FENCE_FLAGS Flags,
                const IID& riid,
                void** ppFence);

            Utils::Expected<, HRESULT> GetDeviceRemovedReason(void);

            void GetCopyableFootprints(
                const D3D12_RESOURCE_DESC* pResourceDesc,
                UINT FirstSubresource,
                UINT NumSubresources,
                UINT64 BaseOffset,
                D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
                UINT* pNumRows,
                UINT64* pRowSizeInBytes,
                UINT64* pTotalBytes);

            Utils::Expected<, HRESULT> CreateQueryHeap(
                const D3D12_QUERY_HEAP_DESC* pDesc,
                const IID& riid,
                void** ppvHeap);

            Utils::Expected<, HRESULT> SetStablePowerState(
                BOOL Enable);

            Utils::Expected<, HRESULT> CreateCommandSignature(
                const D3D12_COMMAND_SIGNATURE_DESC* pDesc,
                ID3D12RootSignature* pRootSignature,
                const IID& riid,
                void** ppvCommandSignature);

            void GetResourceTiling(
                ID3D12Resource* pTiledResource,
                UINT* pNumTilesForEntireResource,
                D3D12_PACKED_MIP_INFO* pPackedMipDesc,
                D3D12_TILE_SHAPE* pStandardTileShapeForNonPackedMips,
                UINT* pNumSubresourceTilings,
                UINT FirstSubresourceTilingToGet,
                D3D12_SUBRESOURCE_TILING* pSubresourceTilingsForNonPackedMips);

            LUID GetAdapterLuid(void);

        private:
            type& InternalGetDevice() { return *static_cast<WrapperTy&>(*this).GetDevice().Get(); }
        };

        template<class Ty>
        class Device : public DeviceInterface<Device<Ty>, Ty>
        {
        private:
            ComPtr<Ty> m_device;

        public:
            Device() = default;
            Device(ComPtr<Ty> device) :
                m_device(device)
            {

            }

        public:
            ComPtr<Ty> GetDevice() const { return m_device; }
        };
    }


    using Device = Internal::Device<ID3D12Device>;


}