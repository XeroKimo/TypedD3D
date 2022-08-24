#pragma once
#include "source/Helpers/D3D12Helpers.h"
#include "source/Helpers/COMHelpers.h"
#include "source/Internal/IUnknownWrapper.h"
#include "source/Internal/D3D12/Meta.h"
#include "source/D3D12Wrappers.h"
#include "CommandList.h"
#include "CommandAllocator.h"
#include "CommandQueue.h"
#include "PipelineState.h"
#include "DescriptorHeap.h"
#include "span_tuple.h"
#include "source/Types.h"

#include <d3d12.h>
#include <wrl/client.h>

namespace TypedD3D::D3D12
{
    struct MetaCommandParameterInfo
    {
        UINT totalStructureSizeInBytes;
        UINT parameterCount;
        std::vector<D3D12_META_COMMAND_PARAMETER_DESC> parameterDescs;
    };
}

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        template<std::derived_from<ID3D12Device> DeviceTy>
        using Device_t = IUnknownWrapper<DeviceTy>;

        namespace Device
        {
            template<class Ty>
            struct Traits;

            template<>
            struct Traits<ID3D12Device>
            {
                using value_type = ID3D12Device;
                using pointer = ID3D12Device*;
                using const_pointer = const ID3D12Device*;
                using reference = ID3D12Device&;
                using const_reference = const ID3D12Device&;

                template<class DerivedSelf>
                class Interface
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    UINT GetNodeCount()
                    {
                        return Get().GetNodeCount();
                    }

                    template<D3D12_COMMAND_LIST_TYPE Type>
                    CommandQueue_t<Type> CreateCommandQueue(
                        D3D12_COMMAND_QUEUE_PRIORITY priority,
                        D3D12_COMMAND_QUEUE_FLAGS flags,
                        UINT nodeMask)
                    {
                        using queue_type = TypedD3D::D3D12::CommandQueue_t<Type>;

                        D3D12_COMMAND_QUEUE_DESC desc
                        {
                            .Type = Type,
                            .Priority = priority,
                            .Flags = flags,
                            .NodeMask = nodeMask
                        };

                        return Helpers::D3D12::CreateCommandQueue(Get(), desc);
                    }

                    template<D3D12_COMMAND_LIST_TYPE Type>
                    CommandAllocator_t<Type> CreateCommandAllocator()
                    {
                        using allocator_type = TypedD3D::D3D12::CommandAllocator_t<Type>;

                        return Helpers::D3D12::CreateCommandAllocator(Get(), Type);
                    }

                    Graphics<ID3D12PipelineState> CreateGraphicsPipelineState(
                        const D3D12_GRAPHICS_PIPELINE_STATE_DESC& pDesc)
                    {
                        return Helpers::D3D12::CreateGraphicsPipelineState(Get(), pDesc);
                    }

                    Compute<ID3D12PipelineState> CreateComputePipelineState(
                        const D3D12_COMPUTE_PIPELINE_STATE_DESC& pDesc)
                    {
                        return Helpers::D3D12::CreateComputePipelineState(Get(), pDesc);
                    }

                    template<D3D12_COMMAND_LIST_TYPE Type>
                    CommandList_t<ID3D12GraphicsCommandList, Type> CreateCommandList(
                        CommandAllocator_t<Type> pCommandAllocator,
                        UINT nodeMask = 0,
                        ID3D12PipelineState* optInitialState = nullptr)
                    {
                        using command_list_type = TypedD3D::D3D12::CommandList_t<ID3D12GraphicsCommandList, Type>;
                        return Helpers::D3D12::CreateCommandList<ID3D12GraphicsCommandList>(Get(), Type, *pCommandAllocator.Get(), nodeMask, optInitialState);
                    }

                    template<D3D12_FEATURE Feature>
                    typename Meta::DeviceFeatureToType<Feature>::type CheckFeatureSupport()
                    {
                        using feature_t = typename Meta::DeviceFeatureToType<Feature>::type;
                        feature_t feature{};

                        ThrowIfFailed(Get().CheckFeatureSupport(Feature, &feature, sizeof(feature_t)));

                        return feature;
                    }

                    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlag>
                    DescriptorHeap_t<Type, HeapFlag> CreateDescriptorHeap(
                        UINT NumDescriptors,
                        UINT NodeMask)
                    {
                        using DescriptorHeap_t = DescriptorHeap_t<Type, HeapFlag>;

                        D3D12_DESCRIPTOR_HEAP_DESC desc
                        {
                            .Type = Type,
                            .NumDescriptors = NumDescriptors,
                            .Flags = HeapFlag,
                            .NodeMask = NodeMask
                        };

                        return Helpers::D3D12::CreateDescriptorHeap(Get(), desc);
                    }

                    UINT GetDescriptorHandleIncrementSize(
                        D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType)
                    {
                        return Get().GetDescriptorHandleIncrementSize(DescriptorHeapType);
                    }

                    Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature(
                        UINT nodeMask,
                        const void* pBlobWithRootSignature,
                        SIZE_T blobLengthInBytes)
                    {
                        return Helpers::D3D12::CreateRootSignature(Get(), nodeMask, pBlobWithRootSignature, blobLengthInBytes);
                    }

                    template<D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
                    void CreateConstantBufferView(
                        const D3D12_CONSTANT_BUFFER_VIEW_DESC& pDesc,
                        CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, HeapFlags> DestDescriptor)
                    {
                        Get().CreateConstantBufferView(pDesc, DestDescriptor.Get());
                    }


                    template<D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
                    void CreateShaderResourceView(
                        ID3D12Resource& pResource,
                        const D3D12_SHADER_RESOURCE_VIEW_DESC* optDesc,
                        CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, HeapFlags> DestDescriptor)
                    {
                        Get().CreateShaderResourceView(&pResource, optDesc, DestDescriptor.Get());
                    }

                    template<D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
                    void CreateNullShaderResourceView(
                        const D3D12_SHADER_RESOURCE_VIEW_DESC& pDesc,
                        CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, HeapFlags> DestDescriptor)
                    {
                        Get().CreateShaderResourceView(nullptr, &pDesc, DestDescriptor.Get());
                    }

                    template<D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
                    void CreateUnorderedAccessView(
                        ID3D12Resource& pResource,
                        ID3D12Resource& pCounterResource,
                        const D3D12_UNORDERED_ACCESS_VIEW_DESC& pDesc,
                        CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, HeapFlags> DestDescriptor)
                    {
                        Get().CreateUnorderedAccessView(&pResource, &pCounterResource, &pDesc, DestDescriptor.Get());
                    }

                    void CreateRenderTargetView(
                        ID3D12Resource& Resource,
                        const D3D12_RENDER_TARGET_VIEW_DESC* optDesc,
                        RTV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
                    {
                        Get().CreateRenderTargetView(&Resource, optDesc, DestDescriptor.Get());
                    }

                    void CreateNullRenderTargetView(
                        const D3D12_RENDER_TARGET_VIEW_DESC& Desc,
                        RTV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
                    {
                        Get().CreateRenderTargetView(nullptr, &Desc, DestDescriptor.Get());
                    }

                    void CreateDepthStencilView(
                        ID3D12Resource& Resource,
                        const D3D12_DEPTH_STENCIL_VIEW_DESC* optDesc,
                        DSV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
                    {
                        Get().CreateDepthStencilView(&Resource, optDesc, DestDescriptor.Get());
                    }

                    void CreateNullDepthStencilView(
                        const D3D12_DEPTH_STENCIL_VIEW_DESC& optDesc,
                        DSV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
                    {
                        Get().CreateDepthStencilView(nullptr, optDesc, DestDescriptor.Get());
                    }

                    template<D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
                    void CreateSampler(
                        const D3D12_SAMPLER_DESC& pDesc,
                        CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, HeapFlags> DestDescriptor)
                    {
                        Get().CreateSampler(&pDesc, DestDescriptor.Get());
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
                        Get().CopyDescriptors(
                            NumDestDescriptorRanges,
                            pDestDescriptorRangeStarts,
                            pDestDescriptorRangeSizes,
                            NumSrcDescriptorRanges,
                            pSrcDescriptorRangeStarts,
                            pSrcDescriptorRangeSizes,
                            DescriptorHeapsType);
                    }

                    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS DestinationFlag>
                    void CopyDescriptorsSimple(
                        UINT NumDescriptors,
                        DescriptorHeap_t<Type, DestinationFlag>::CPU_DESCRIPTOR_HANDLE DestDescriptorRangeStart,
                        DescriptorHeap_t<Type, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>::CPU_DESCRIPTOR_HANDLE SrcDescriptorRangeStart)
                    {
                        Get().CopyDescriptorsSimple(NumDescriptors, DestDescriptorRangeStart.Get(), SrcDescriptorRangeStart.Get(), Type);
                    }

                    D3D12_RESOURCE_ALLOCATION_INFO GetResourceAllocationInfo(
                        UINT visibleMask,
                        const std::span<D3D12_RESOURCE_DESC> resourceDescs)
                    {
                        return Get().GetResourceAllocationInfo(
                            visibleMask,
                            static_cast<UINT>(resourceDescs.size()),
                            resourceDescs.data());
                    }

                    D3D12_HEAP_PROPERTIES GetCustomHeapProperties(
                        UINT nodeMask,
                        D3D12_HEAP_TYPE heapType)
                    {
                        return Get().GetCustomHeapProperties(nodeMask, heapType);
                    }

                    Microsoft::WRL::ComPtr<ID3D12Resource> CreateCommittedResource(
                        const D3D12_HEAP_PROPERTIES& pHeapProperties,
                        D3D12_HEAP_FLAGS HeapFlags,
                        const D3D12_RESOURCE_DESC& pDesc,
                        D3D12_RESOURCE_STATES InitialResourceState,
                        const D3D12_CLEAR_VALUE* optOptimizedClearValue)
                    {
                        return Helpers::D3D12::CreateCommittedResource(
                            Get(),
                            pHeapProperties,
                            HeapFlags,
                            pDesc,
                            InitialResourceState,
                            optOptimizedClearValue);
                    }

                    Microsoft::WRL::ComPtr<ID3D12Heap> CreateHeap(
                        const D3D12_HEAP_DESC& pDesc)
                    {
                        return Helpers::D3D12::CreateHeap(
                            Get(),
                            pDesc);
                    }

                    Microsoft::WRL::ComPtr<ID3D12Resource> CreatePlacedResource(
                        ID3D12Heap& pHeap,
                        UINT64 HeapOffset,
                        const D3D12_RESOURCE_DESC& pDesc,
                        D3D12_RESOURCE_STATES InitialState,
                        const D3D12_CLEAR_VALUE* optOptimizedClearValue)
                    {
                        return Helpers::D3D12::CreatePlacedResource(
                            Get(),
                            pHeap,
                            HeapOffset,
                            pDesc,
                            InitialState,
                            optOptimizedClearValue);
                    }

                    Microsoft::WRL::ComPtr<ID3D12Resource> CreateReservedResource(
                        const D3D12_RESOURCE_DESC& pDesc,
                        D3D12_RESOURCE_STATES InitialState,
                        const D3D12_CLEAR_VALUE* optOptimizedClearValue)
                    {
                        return Helpers::D3D12::CreateReservedResource(
                            Get(),
                            pDesc,
                            InitialState,
                            optOptimizedClearValue);
                    }

                    HANDLE CreateSharedHandle(
                        ID3D12DeviceChild& pObject,
                        const SECURITY_ATTRIBUTES* optAttributes,
                        DWORD Access,
                        LPCWSTR Name)
                    {
                        HANDLE handle;

                        ThrowIfFailed(Get().CreateSharedHandle(
                            &pObject,
                            optAttributes,
                            Access,
                            Name,
                            &handle));

                        return handle;
                    }

                      //TODO: Figure out how this works to update to a more modern API
                    void OpenSharedHandle(
                        HANDLE NTHandle,
                        const IID& riid,
                        void** ppvObj)
                    {
                        ThrowIfFailed(Get().OpenSharedHandle(
                            NTHandle,
                            riid,
                            ppvObj));
                    }

                    HANDLE OpenSharedHandleByName(
                        LPCWSTR Name,
                        DWORD Access)
                    {
                        HANDLE handle;

                        ThrowIfFailed(Get().OpenSharedHandleByName(
                            Name,
                            Access,
                            &handle));

                        return handle;
                    }

                    void MakeResident(
                        const std::span<ID3D12Pageable*> objects)
                    {
                        ThrowIfFailed(Get().MakeResident(
                            static_cast<UINT>(objects.size()),
                            objects.data()));
                    }

                    void Evict(
                        const std::span<ID3D12Pageable*> objects)
                    {
                        ThrowIfFailed(Get().Evict(
                            static_cast<UINT>(objects.size()),
                            objects.data()));
                    }

                    Microsoft::WRL::ComPtr<ID3D12Fence> CreateFence(
                        UINT64 InitialValue,
                        D3D12_FENCE_FLAGS Flags)
                    {
                        return Helpers::D3D12::CreateFence(Get(), Flags, InitialValue);
                    }

                    HRESULT GetDeviceRemovedReason()
                    {
                        return Get().GetDeviceRemovedReason();
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
                        Get().GetCopyableFootprints(
                            &pResourceDesc,
                            FirstSubresource,
                            NumSubresources,
                            BaseOffset,
                            optOutLayouts,
                            optOutNumRows,
                            optOutRowSizeInBytes,
                            optOutTotalBytes);
                    }

                    Microsoft::WRL::ComPtr<ID3D12QueryHeap> CreateQueryHeap(
                        const D3D12_QUERY_HEAP_DESC& pDesc)
                    {
                        return Helpers::D3D12::CreateQueryHeap(Get(), pDesc);
                    }

                    void SetStablePowerState(
                        BOOL Enable)
                    {
                        ThrowIfFailed(Get().SetStablePowerState(Enable));
                    }

                    Microsoft::WRL::ComPtr<ID3D12CommandSignature> CreateCommandSignature(
                        const D3D12_COMMAND_SIGNATURE_DESC& pDesc,
                        ID3D12RootSignature* optRootSignature)
                    {
                        return Helpers::D3D12::CreateCommandSignature(Get(), pDesc, optRootSignature);
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
                        Get().GetResourceTiling(
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
                        return Get().GetAdapterLuid();
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct Traits<ID3D12Device1>
            {
                using value_type = ID3D12Device1;
                using pointer = ID3D12Device1*;
                using const_pointer = const ID3D12Device1*;
                using reference = ID3D12Device1&;
                using const_reference = const ID3D12Device1&;

                template<class DerivedSelf>
                class Interface : public Traits<ID3D12Device>::Interface<DerivedSelf>
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    Microsoft::WRL::ComPtr<ID3D12PipelineLibrary> CreatePipelineLibrary(
                        const void* pLibraryBlob,
                        SIZE_T BlobLength)
                    {
                        return Helpers::COM::IIDToObjectForwardFunction<ID3D12PipelineLibrary>(ID3D12Device1::CreatePipelineLibrary, Get(), pLibraryBlob, BlobLength);
                    }

                    void SetEventOnMultipleFenceCompletion(
                        xk::span_tuple<const ID3D12Fence*, std::dynamic_extent, const FenceValue> fences,
                        D3D12_MULTIPLE_FENCE_WAIT_FLAGS Flags,
                        HANDLE hEvent)
                    {
                        std::unique_ptr<std::underlying_type_t<FenceValue>[]> fenceValues = std::make_unique< std::underlying_type_t<FenceValue>[]>(fences.size());
                        std::memcpy(fenceValues.get(), fences.data<1>(), fences.size_bytes<1>());

                        ThrowIfFailed(Get().SetEventOnMultipleFenceCompletion(
                            fences.data<0>(),
                            fenceValues.get(),
                            static_cast<UINT>(fences.size()),
                            Flags,
                            hEvent));
                    }

                    void SetResidencyPriority(
                        std::span<ID3D12Pageable*> ppObjects,
                        std::span<const D3D12_RESIDENCY_PRIORITY> priorities)
                    {
                        assert(ppObjects.size() == priorities.size());
                        ThrowIfFailed(Get().SetResidencyPriority(
                            static_cast<UINT>(ppObjects.size()),
                            ppObjects.data(),
                            priorities.data()));
                    }


                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct Traits<ID3D12Device2>
            {
                using value_type = ID3D12Device2;
                using pointer = ID3D12Device2*;
                using const_pointer = const ID3D12Device2*;
                using reference = ID3D12Device2&;
                using const_reference = const ID3D12Device2&;

                template<class DerivedSelf>
                class Interface : public Traits<ID3D12Device1>::Interface<DerivedSelf>
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    Microsoft::WRL::ComPtr<ID3D12PipelineState> CreatePipelineState(
                        const D3D12_PIPELINE_STATE_STREAM_DESC pDesc)
                    {
                        return Helpers::COM::IIDToObjectForwardFunction<ID3D12PipelineState>(&ID3D12Device2::CreatePipelineState, Get(), &pDesc);
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct Traits<ID3D12Device3>
            {
                using value_type = ID3D12Device3;
                using pointer = ID3D12Device3*;
                using const_pointer = const ID3D12Device3*;
                using reference = ID3D12Device3&;
                using const_reference = const ID3D12Device3&;

                template<class DerivedSelf>
                class Interface : public Traits<ID3D12Device2>::Interface<DerivedSelf>
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    Microsoft::WRL::ComPtr<ID3D12Heap> OpenExistingHeapFromAddress(
                        const void* pAddress)
                    {
                        return Helpers::COM::IIDToObjectForwardFunction<ID3D12Heap>(&ID3D12Device3::OpenExistingHeapFromAddress, Get(), pAddress);
                    }

                    void OpenExistingHeapFromFileMapping(
                        HANDLE hFileMapping)
                    {
                        ThrowIfFailed(Helpers::COM::IIDToObjectForwardFunction<ID3D12Heap>(&ID3D12Device3::OpenExistingHeapFromFileMapping, Get(), hFileMapping));
                    }

                    void EnqueueMakeResident(
                        D3D12_RESIDENCY_FLAGS Flags,
                        std::span<ID3D12Pageable*> ppObjects,
                        ID3D12Fence& pFenceToSignal,
                        UINT64 FenceValueToSignal)
                    {
                        ThrowIfFailed(Get().EnqueueMakeResident(
                            Flags,
                            static_cast<UINT>(ppObjects.size()),
                            ppObjects.data(),
                            &pFenceToSignal, FenceValueToSignal));
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct Traits<ID3D12Device4>
            {
                using value_type = ID3D12Device4;
                using pointer = ID3D12Device4*;
                using const_pointer = const ID3D12Device4*;
                using reference = ID3D12Device4&;
                using const_reference = const ID3D12Device4&;

                template<class DerivedSelf>
                class Interface : public Traits<ID3D12Device3>::Interface<DerivedSelf>
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    template<D3D12_COMMAND_LIST_TYPE Type>
                    TypedD3D::D3D12::CommandList_t<ID3D12GraphicsCommandList, Type> CreateCommandList1(
                        UINT nodeMask,
                        D3D12_COMMAND_LIST_FLAGS flags)
                    {
                        return TypedD3D::D3D12::CommandList_t<ID3D12GraphicsCommandList, Type>(Helpers::D3D12::CreateCommandList(Get(), Type, flags, nodeMask));
                    }

                    Microsoft::WRL::ComPtr<ID3D12ProtectedResourceSession> CreateProtectedResourceSession(
                        const D3D12_PROTECTED_RESOURCE_SESSION_DESC& pDesc)
                    {
                        return Helpers::COM::IIDToObjectForwardFunction<ID3D12ProtectedResourceSession>(&ID3D12Device4::CreateProtectedResourceSession, Get(), &pDesc);
                    }

                    Microsoft::WRL::ComPtr<ID3D12Resource> CreateCommittedResource1(
                        const D3D12_HEAP_PROPERTIES& pHeapProperties,
                        D3D12_HEAP_FLAGS HeapFlags,
                        const D3D12_RESOURCE_DESC& pDesc,
                        D3D12_RESOURCE_STATES InitialResourceState,
                        const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                        ID3D12ProtectedResourceSession* pProtectedSession)
                    {
                        return Helpers::COM::IIDToObjectForwardFunction<ID3D12Resource>(
                            &ID3D12Device4::CreateCommittedResource1,
                            Get(),
                            &pHeapProperties,
                            HeapFlags,
                            &pDesc,
                            InitialResourceState,
                            pOptimizedClearValue,
                            pProtectedSession);
                    }

                    Microsoft::WRL::ComPtr<ID3D12Heap> CreateHeap1(
                        const D3D12_HEAP_DESC& pDesc,
                        ID3D12ProtectedResourceSession* pProtectedSession)
                    {
                        return Helpers::COM::IIDToObjectForwardFunction<ID3D12Heap>(
                            &ID3D12Device4::CreateHeap1,
                            Get(),
                            &pDesc,
                            pProtectedSession);
                    }

                    Microsoft::WRL::ComPtr<ID3D12Resource> CreateReservedResource1(
                        const D3D12_RESOURCE_DESC& pDesc,
                        D3D12_RESOURCE_STATES InitialState,
                        const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                        ID3D12ProtectedResourceSession* pProtectedSession)
                    {
                        return Helpers::COM::IIDToObjectForwardFunction<ID3D12Heap>(
                            &ID3D12Device4::CreateReservedResource1,
                            Get(),
                            &pDesc,
                            InitialState,
                            pOptimizedClearValue,
                            pProtectedSession);
                    }

                    std::vector<D3D12_RESOURCE_ALLOCATION_INFO1> GetResourceAllocationInfo1(
                        UINT visibleMask,
                        std::span<const D3D12_RESOURCE_DESC> pResourceDescs)
                    {
                        std::vector<D3D12_RESOURCE_ALLOCATION_INFO1> pResourceAllocationInfo1(pResourceDescs.size());
                        Get().GetResourceAllocationInfo1(visibleMask, static_cast<UINT>(pResourceDescs.size()), pResourceDescs.data(), pResourceAllocationInfo1.data());
                        return pResourceAllocationInfo1;
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct Traits<ID3D12Device5>
            {
                using value_type = ID3D12Device5;
                using pointer = ID3D12Device5*;
                using const_pointer = const ID3D12Device5*;
                using reference = ID3D12Device5&;
                using const_reference = const ID3D12Device5&;

                template<class DerivedSelf>
                class Interface : public Traits<ID3D12Device4>::Interface<DerivedSelf>
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                //TODO: Figure out how this works to update to a more modern API
                    void CreateLifetimeTracker(
                        ID3D12LifetimeOwner& pOwner,
                        REFIID riid,
                        void** ppvTracker)
                    {
                        ThrowIfFailed(Get().CreateLifetimeTracker(&pOwner, riid, ppvTracker));
                    }

                    void RemoveDevice()
                    {
                        Get().RemoveDevice();
                    }

                    UINT GetNumMetaCommands()
                    {
                        UINT pNumMetaCommands = 0;
                        Get().EnumerateMetaCommands(&pNumMetaCommands, nullptr);
                        return pNumMetaCommands;
                    }

                    std::vector<D3D12_META_COMMAND_DESC> EnumerateMetaCommands(
                        UINT pNumMetaCommands)
                    {
                        std::vector<D3D12_META_COMMAND_DESC> pDescs(pNumMetaCommands);
                        ThrowIfFailed(Get().EnumerateMetaCommands(&pNumMetaCommands, pDescs.data()));
                        return pDescs;
                    }

                    UINT GetNumMetaCommandParameters(
                        REFGUID CommandId,
                        D3D12_META_COMMAND_PARAMETER_STAGE Stage)
                    {
                        UINT pNumMetaCommandParams = 0;
                        Get().EnumerateMetaCommandParameters(CommandId, Stage, nullptr, &pNumMetaCommandParams, nullptr);
                        return pNumMetaCommandParams;
                    }

                    TypedD3D::D3D12::MetaCommandParameterInfo STDMETHODCALLTYPE EnumerateMetaCommandParameters(
                        _In_  REFGUID CommandId,
                        _In_  D3D12_META_COMMAND_PARAMETER_STAGE Stage,
                        UINT parameterCount)
                    {
                        TypedD3D::D3D12::MetaCommandParameterInfo info{};

                        info.parameterCount = parameterCount;
                        info.parameterDescs.resize(parameterCount);

                        ThrowIfFailed(Get().EnumerateMetaCommandParameters(
                            CommandId,
                            Stage,
                            &info.totalStructureSizeInBytes,
                            &info.parameterCount,
                            info.parameterDescs.data()));
                        return info;
                    }

                    Microsoft::WRL::ComPtr<ID3D12MetaCommand> CreateMetaCommand(
                        REFGUID CommandId,
                        UINT NodeMask,
                        const void* pCreationParametersData,
                        SIZE_T CreationParametersDataSizeInBytes)
                    {
                        return Helpers::COM::IIDToObjectForwardFunction<ID3D12MetaCommand>(
                            &ID3D12Device5::CreateMetaCommand,
                            Get(),
                            CommandId,
                            NodeMask,
                            pCreationParametersData,
                            CreationParametersDataSizeInBytes);
                    }

                    template<class CreationParamStruct>
                    Microsoft::WRL::ComPtr<ID3D12MetaCommand> CreateMetaCommand(
                        REFGUID CommandId,
                        UINT NodeMask,
                        const CreationParamStruct& pCreationParametersData)
                    {
                        return CreateMetaCommand(CommandId, NodeMask, &pCreationParametersData, sizeof(CreationParamStruct));
                    }

                    Microsoft::WRL::ComPtr<ID3D12MetaCommand> CreateMetaCommand(
                        REFGUID CommandId,
                        UINT NodeMask)
                    {
                        return CreateMetaCommand(CommandId, NodeMask, nullptr, 0);
                    }

                    Microsoft::WRL::ComPtr<ID3D12StateObject> CreateStateObject(
                        const D3D12_STATE_OBJECT_DESC& pDesc)
                    {
                        return Helpers::COM::IIDToObjectForwardFunction<ID3D12StateObject>(
                            &ID3D12Device5::CreateStateObject,
                            Get(),
                            &pDesc);
                    }

                    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO GetRaytracingAccelerationStructurePrebuildInfo(
                        const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& pDesc)
                    {
                        D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuildInfo;
                        Get().GetRaytracingAccelerationStructurePrebuildInfo(&pDesc, &prebuildInfo);
                        return prebuildInfo;
                    }

                    D3D12_DRIVER_MATCHING_IDENTIFIER_STATUS CheckDriverMatchingIdentifier(
                        D3D12_SERIALIZED_DATA_TYPE SerializedDataType,
                        const D3D12_SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER& pIdentifierToCheck)
                    {
                        return Get().CheckDriverMatchingIdentifier(
                            SerializedDataType,
                            &pIdentifierToCheck);
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };
        }
    }


    template<std::derived_from<ID3D12Device> Ty>
    struct Traits<Ty>
    {
        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using const_reference = const Ty&;

        template<class DerivedSelf>
        using Interface = typename D3D12::Device::Traits<Ty>::template Interface<DerivedSelf>;
    };
}

namespace TypedD3D::D3D12
{
    template<std::derived_from<ID3D12Device> Ty>
    using Device_t = Internal::D3D12::Device_t<Ty>;

    using Device = Device_t<ID3D12Device>;
    using Device1 = Device_t<ID3D12Device1>;
    using Device2 = Device_t<ID3D12Device2>;
    using Device3 = Device_t<ID3D12Device3>;
    using Device4 = Device_t<ID3D12Device4>;
    using Device5 = Device_t<ID3D12Device5>;

    template<class DeviceTy = Device>
    auto CreateDevice(D3D_FEATURE_LEVEL minimumFeatureLevel, IDXGIAdapter* optAdapter = nullptr)
    {
        if constexpr(std::is_base_of_v<ID3D12Device, DeviceTy>)
        {
            return Helpers::D3D12::CreateDevice<DeviceTy>(minimumFeatureLevel, optAdapter);
        }
        else
        {
            return Helpers::D3D12::CreateDevice<typename DeviceTy::value_type>(minimumFeatureLevel, optAdapter);
        }
    }
}