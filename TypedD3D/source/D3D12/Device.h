#pragma once
#include "../Helpers/D3D12Helpers.h"
#include "../Helpers/COMHelpers.h"
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


    struct MetaCommandParameterInfo
    {
        UINT totalStructureSizeInBytes;
        UINT parameterCount;
        std::vector<D3D12_META_COMMAND_PARAMETER_DESC> parameterDescs;
    };

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
                    return Utils::Unexpected(commandQueue.GetError());

                return queue_type(commandQueue.GetValue());
            }

            template<D3D12_COMMAND_LIST_TYPE Type>
            Utils::Expected<CommandAllocator::Internal::CommandAllocator<Type>, HRESULT> CreateCommandAllocator()
            {
                using allocator_type = CommandAllocator::Internal::CommandAllocator<Type>;
                auto commandAllocator = Helpers::D3D12::CreateCommandAllocator(InternalGetDevice(), Type);

                if(!commandAllocator)
                    return Utils::Unexpected(commandAllocator.GetError());

                return allocator_type(commandAllocator.GetValue());
            }

            Utils::Expected<PipelineState::Graphics, HRESULT> CreateGraphicsPipelineState(
                const D3D12_GRAPHICS_PIPELINE_STATE_DESC& pDesc)
            {
                auto pipelineState = Helpers::D3D12::CreateGraphicsPipelineState(InternalGetDevice(), pDesc);

                if(!pipelineState)
                    return Utils::Unexpected(pipelineState.GetError());

                return PipelineState::Graphics(pipelineState.GetValue());
            }

            Utils::Expected<PipelineState::Compute, HRESULT> CreateComputePipelineState(
                const D3D12_COMPUTE_PIPELINE_STATE_DESC& pDesc)
            {
                auto pipelineState = Helpers::D3D12::CreateComputePipelineState(InternalGetDevice(), pDesc);

                if(!pipelineState)
                    return Utils::Unexpected(pipelineState.GetError());

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
                    return Utils::Unexpected(commandList.GetError());

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



        template<class WrapperTy>
        class DeviceInterface<WrapperTy, ID3D12Device1> : public DeviceInterface<WrapperTy, ID3D12Device>
        {
        private:
            using type = ID3D12Device1;

        public:
            Utils::Expected<ComPtr<ID3D12PipelineLibrary>, HRESULT> CreatePipelineLibrary(
                const void* pLibraryBlob,
                SIZE_T BlobLength)
            {
                return Helpers::COM::IIDToObjectForwardFunction<ID3D12PipelineLibrary>(ID3D12Device1::CreatePipelineLibrary, InternalGetDevice(), pLibraryBlob, BlobLength);
            }

            HRESULT SetEventOnMultipleFenceCompletion(
                std::span<const ID3D12Fence*> fences,
                std::span<const UINT64> fenceValues,
                D3D12_MULTIPLE_FENCE_WAIT_FLAGS Flags,
                HANDLE hEvent)
            {
                assert(fences.size() == fenceValues.size());

                return InternalGetDevice().SetEventOnMultipleFenceCompletion(
                    fences.data(), 
                    fenceValues.data(), 
                    static_cast<UINT>(fences.size()), 
                    Flags, 
                    hEvent);
            }

            HRESULT SetResidencyPriority(
                std::span<ID3D12Pageable*> ppObjects,
                std::span<const D3D12_RESIDENCY_PRIORITY> priorities)
            {
                assert(ppObjects.size() == priorities.size());
                return InternalGetDevice().SetResidencyPriority(
                    static_cast<UINT>(ppObjects.size()),
                    ppObjects.data(),
                    priorities.data());
            }

        private:
            type& InternalGetDevice() { return *static_cast<WrapperTy&>(*this).Get(); }
        };



        template<class WrapperTy>
        class DeviceInterface<WrapperTy, ID3D12Device2> : public DeviceInterface<WrapperTy, ID3D12Device1>
        {
        private:
            using type = ID3D12Device2;

        public:
            Utils::Expected<ComPtr<ID3D12PipelineState>, HRESULT> CreatePipelineState(
                const D3D12_PIPELINE_STATE_STREAM_DESC pDesc)
            {
                return Helpers::COM::IIDToObjectForwardFunction<ID3D12PipelineState>(&ID3D12Device2::CreatePipelineState, InternalGetDevice(), &pDesc);
            }

        private:
            type& InternalGetDevice() { return *static_cast<WrapperTy&>(*this).Get(); }
        };



        template<class WrapperTy>
        class DeviceInterface<WrapperTy, ID3D12Device3> : public DeviceInterface<WrapperTy, ID3D12Device2>
        {
        private:
            using type = ID3D12Device3;

        public:
            Utils::Expected<ComPtr<ID3D12Heap>, HRESULT> OpenExistingHeapFromAddress(
                const void* pAddress)
            {
                return Helpers::COM::IIDToObjectForwardFunction<ID3D12Heap>(&ID3D12Device3::OpenExistingHeapFromAddress, InternalGetDevice(), pAddress);
            }

            HRESULT OpenExistingHeapFromFileMapping(
                HANDLE hFileMapping)
            {
                return Helpers::COM::IIDToObjectForwardFunction<ID3D12Heap>(&ID3D12Device3::OpenExistingHeapFromFileMapping, InternalGetDevice(), hFileMapping);
            }

            HRESULT EnqueueMakeResident(
                D3D12_RESIDENCY_FLAGS Flags,
                std::span<ID3D12Pageable*> ppObjects,
                ID3D12Fence& pFenceToSignal,
                UINT64 FenceValueToSignal)
            {
                return InternalGetDevice().EnqueueMakeResident(
                    Flags,
                    static_cast<UINT>(ppObjects.size()),
                    ppObjects.data(),
                    &pFenceToSignal, FenceValueToSignal);
            }

        private:
            type& InternalGetDevice() { return *static_cast<WrapperTy&>(*this).Get(); }
        };



        template<class WrapperTy>
        class DeviceInterface<WrapperTy, ID3D12Device4> : public DeviceInterface<WrapperTy, ID3D12Device3>
        {
        private:
            using type = ID3D12Device4;

        public:
            template<D3D12_COMMAND_LIST_TYPE type>
            Utils::Expected<CommandList::CommandList_t<type, ID3D12GraphicsCommandList>, HRESULT> CreateCommandList1(
                UINT nodeMask,
                D3D12_COMMAND_LIST_FLAGS flags)
            {
                Utils::Expected<ComPtr<ID3D12GraphicsCommandList>, HRESULT> cl = Helpers::D3D12::CreateCommandList(InternalGetDevice(), type, flags, nodeMask);

                if(!cl.HasValue())
                    return Utils::Unexpected(cl.GetError());

                return CommandList::CommandList_t<type, ID3D12GraphicsCommandList>(cl.GetValue());
            }

            Utils::Expected<ComPtr<ID3D12ProtectedResourceSession>, HRESULT> CreateProtectedResourceSession(
                const D3D12_PROTECTED_RESOURCE_SESSION_DESC& pDesc)
            {
                return Helpers::COM::IIDToObjectForwardFunction<ID3D12ProtectedResourceSession>(&ID3D12Device4::CreateProtectedResourceSession, InternalGetDevice(), &pDesc);
            }

            Utils::Expected<ComPtr<ID3D12Resource>, HRESULT> CreateCommittedResource1(
                const D3D12_HEAP_PROPERTIES& pHeapProperties,
                D3D12_HEAP_FLAGS HeapFlags,
                const D3D12_RESOURCE_DESC& pDesc,
                D3D12_RESOURCE_STATES InitialResourceState,
                const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                ID3D12ProtectedResourceSession* pProtectedSession)
            {
                return Helpers::COM::IIDToObjectForwardFunction<ID3D12Resource>(
                    &ID3D12Device4::CreateCommittedResource1, 
                    InternalGetDevice(), 
                    &pHeapProperties, 
                    HeapFlags, 
                    &pDesc, 
                    InitialResourceState, 
                    pOptimizedClearValue, 
                    pProtectedSession);
            }

            Utils::Expected<ComPtr<ID3D12Heap>, HRESULT> CreateHeap1(
                const D3D12_HEAP_DESC& pDesc,
                ID3D12ProtectedResourceSession* pProtectedSession)
            {
                return Helpers::COM::IIDToObjectForwardFunction<ID3D12Heap>(
                    &ID3D12Device4::CreateHeap1,
                    InternalGetDevice(),
                    &pDesc,
                    pProtectedSession);
            }

            Utils::Expected<ComPtr<ID3D12Resource>, HRESULT> CreateReservedResource1(
                const D3D12_RESOURCE_DESC& pDesc,
                D3D12_RESOURCE_STATES InitialState,
                const D3D12_CLEAR_VALUE* pOptimizedClearValue,
                ID3D12ProtectedResourceSession* pProtectedSession)
            {
                return Helpers::COM::IIDToObjectForwardFunction<ID3D12Heap>(
                    &ID3D12Device4::CreateReservedResource1,
                    InternalGetDevice(),
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
                InternalGetDevice().GetResourceAllocationInfo1(visibleMask, static_cast<UINT>(pResourceDescs.size()), pResourceDescs.data(), pResourceAllocationInfo1.data());
                return pResourceAllocationInfo1;
            }


        private:
            type& InternalGetDevice() { return *static_cast<WrapperTy&>(*this).Get(); }
        };

        template<class WrapperTy>
        class DeviceInterface<WrapperTy, ID3D12Device5> : public DeviceInterface<WrapperTy, ID3D12Device4>
        {
        private:
            using type = ID3D12Device5;

        public:
            HRESULT CreateLifetimeTracker(
                ID3D12LifetimeOwner& pOwner,
                REFIID riid,
                void** ppvTracker)
            {
                return InternalGetDevice().CreateLifetimeTracker(&pOwner, riid, ppvTracker);
            }

            void RemoveDevice()
            {
                InternalGetDevice().RemoveDevice();
            }

            UINT GetNumMetaCommands()
            {
                UINT pNumMetaCommands = 0;
                InternalGetDevice().EnumerateMetaCommands(&pNumMetaCommands, nullptr);
                return pNumMetaCommands;
            }

            Utils::Expected<std::vector<D3D12_META_COMMAND_DESC>, HRESULT> EnumerateMetaCommands(
                UINT pNumMetaCommands)
            {
                std::vector<D3D12_META_COMMAND_DESC> pDescs(pNumMetaCommands);
                HRESULT result = InternalGetDevice().EnumerateMetaCommands(&pNumMetaCommands, pDescs.data());
                if(FAILED(result))
                    return Utils::Unexpected(result);
                return pDescs;
            }

            UINT GetNumMetaCommandParameters(
                REFGUID CommandId,
                D3D12_META_COMMAND_PARAMETER_STAGE Stage)
            {
                UINT pNumMetaCommandParams = 0;
                InternalGetDevice().EnumerateMetaCommandParameters(CommandId, Stage, nullptr, &pNumMetaCommandParams, nullptr);
                return pNumMetaCommandParams;
            }

            Utils::Expected<MetaCommandParameterInfo, HRESULT> STDMETHODCALLTYPE EnumerateMetaCommandParameters(
                _In_  REFGUID CommandId,
                _In_  D3D12_META_COMMAND_PARAMETER_STAGE Stage,
                UINT parameterCount)
            {
                MetaCommandParameterInfo info{};

                info.parameterCount = parameterCount;
                info.parameterDescs.resize(parameterCount);

                HRESULT result = InternalGetDevice().EnumerateMetaCommandParameters(
                    CommandId,
                    Stage,
                    &info.totalStructureSizeInBytes,
                    &info.parameterCount,
                    info.parameterDescs.data());

                if(FAILED(result))
                    return Utils::Unexpected(result);
                return info;
            }

            Utils::Expected<ComPtr<ID3D12MetaCommand>, HRESULT> CreateMetaCommand(
                REFGUID CommandId,
                UINT NodeMask,
                const void* pCreationParametersData,
                SIZE_T CreationParametersDataSizeInBytes)
            {
                return Helpers::COM::IIDToObjectForwardFunction<ID3D12MetaCommand>(
                    &ID3D12Device5::CreateMetaCommand,
                    InternalGetDevice(),
                    CommandId,
                    NodeMask,
                    pCreationParametersData,
                    CreationParametersDataSizeInBytes);
            }             
            
            template<class CreationParamStruct>
            Utils::Expected<ComPtr<ID3D12MetaCommand>, HRESULT> CreateMetaCommand(
                REFGUID CommandId,
                UINT NodeMask,
                const CreationParamStruct& pCreationParametersData)
            {
                return CreateMetaCommand(CommandId, NodeMask, &pCreationParametersData, sizeof(CreationParamStruct));
            }           
            
            Utils::Expected<ComPtr<ID3D12MetaCommand>, HRESULT> CreateMetaCommand(
                REFGUID CommandId,
                UINT NodeMask)
            {
                return CreateMetaCommand(CommandId, NodeMask, nullptr, 0);
            }

            Utils::Expected<ComPtr<ID3D12StateObject>, HRESULT> CreateStateObject(
                const D3D12_STATE_OBJECT_DESC& pDesc)
            {
                return Helpers::COM::IIDToObjectForwardFunction<ID3D12StateObject>(
                    &ID3D12Device5::CreateStateObject,
                    InternalGetDevice(),
                    &pDesc);
            }

            D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO GetRaytracingAccelerationStructurePrebuildInfo(
                const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& pDesc)
            {
                D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuildInfo;
                InternalGetDevice().GetRaytracingAccelerationStructurePrebuildInfo(&pDesc, &prebuildInfo);
                return prebuildInfo;
            }

            D3D12_DRIVER_MATCHING_IDENTIFIER_STATUS CheckDriverMatchingIdentifier(
                D3D12_SERIALIZED_DATA_TYPE SerializedDataType,
                const D3D12_SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER& pIdentifierToCheck)
            {
                return InternalGetDevice().CheckDriverMatchingIdentifier(
                    SerializedDataType,
                    &pIdentifierToCheck);
            }



        private:
            type& InternalGetDevice() { return *static_cast<WrapperTy&>(*this).Get(); }
        };



        template<class Ty>
        class Device : public ComWrapper<Ty>, private DeviceInterface<Device<Ty>, Ty>
        {
            template<class WrapperTy2, class Ty2>
            friend class DeviceInterface;

        public:
            using ComWrapper<Ty>::ComWrapper;
            using value_type = Ty;

        public:
            template<class DerivedDeviceTy>
            Device(const Device<DerivedDeviceTy>& other) :
                ComWrapper<Ty>::ComWrapper(other.GetComPtr())
            {

            }

        public:
            DeviceInterface<Device<Ty>, Ty>* GetInterface() { return this; }
            DeviceInterface<Device<Ty>, Ty>* operator->() { return this; }

            template<class Ty2>
            Ty2 As()
            {
                return Ty2(Helpers::COM::Cast<typename Ty2::value_type>(ComWrapper<Ty>::GetComPtr()));
            }
        };
    }

    using Device = Internal::Device<ID3D12Device>;
    using Device1 = Internal::Device<ID3D12Device1>;
    using Device2 = Internal::Device<ID3D12Device2>;
    using Device3 = Internal::Device<ID3D12Device3>;
    using Device4 = Internal::Device<ID3D12Device4>;
    using Device5 = Internal::Device<ID3D12Device5>;

    template<class DeviceTy = Device>
    Utils::Expected<DeviceTy, HRESULT> CreateDevice(D3D_FEATURE_LEVEL minimumFeatureLevel, IDXGIAdapter* optAdapter = nullptr)
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