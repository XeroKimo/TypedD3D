module;

#include <d3d12.h>

#include <vector>
#include <memory>
#include <dxgi1_6.h>
#include <cassert>
#include <span>
#include <gsl/pointers>

export module TypedD3D12:Device;
export import TypedD3D.Shared;
export import :CommandList;
export import :CommandQueue;
export import :CommandAllocator;
export import :DescriptorHeap;
export import :PipelineState;
export import :Resource;



namespace TypedD3D::D3D12
{
	struct MetaCommandParameterInfo
	{
		UINT totalStructureSizeInBytes;
		UINT parameterCount;
		std::vector<D3D12_META_COMMAND_PARAMETER_DESC> parameterDescs;
	};

#pragma region Device Feature Map
	template<D3D12_FEATURE Feature>
	struct DeviceFeatureMap;

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_ARCHITECTURE>
	{
		using type = D3D12_FEATURE_DATA_ARCHITECTURE;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_FEATURE_LEVELS>
	{
		using type = D3D12_FEATURE_DATA_FEATURE_LEVELS;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_FORMAT_SUPPORT>
	{
		using type = D3D12_FEATURE_DATA_FORMAT_INFO;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT>
	{
		using type = D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_SHADER_MODEL>
	{
		using type = D3D12_FEATURE_DATA_SHADER_MODEL;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS1>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS1;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_SUPPORT>
	{
		using type = D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_SUPPORT;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_ROOT_SIGNATURE>
	{
		using type = D3D12_FEATURE_DATA_ROOT_SIGNATURE;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_ARCHITECTURE1>
	{
		using type = D3D12_FEATURE_DATA_ARCHITECTURE1;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS2>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS2;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_SHADER_CACHE>
	{
		using type = D3D12_FEATURE_DATA_SHADER_CACHE;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_COMMAND_QUEUE_PRIORITY>
	{
		using type = D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS3>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS3;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_EXISTING_HEAPS>
	{
		using type = D3D12_FEATURE_DATA_EXISTING_HEAPS;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS4>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS4;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_SERIALIZATION>
	{
		using type = D3D12_FEATURE_DATA_SERIALIZATION;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_CROSS_NODE>
	{
		using type = D3D12_FEATURE_DATA_CROSS_NODE;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS5>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS5;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS6>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS6;
	};

	//Requires Windows 11
	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_DISPLAYABLE>
	{
		using type = D3D12_FEATURE_DATA_DISPLAYABLE;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_QUERY_META_COMMAND>
	{
		using type = D3D12_FEATURE_DATA_QUERY_META_COMMAND;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS7>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS7;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_TYPE_COUNT>
	{
		using type = D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_TYPE_COUNT;
	};

	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_TYPES>
	{
		using type = D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_TYPES;
	};

	//Requires Windows 11
	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS8>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS8;
	};

	//Requires Windows 11
	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS9>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS9;
	};

	//Requires Windows 11
	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS10>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS10;
	};

	//Requires Windows 11
	template<>
	struct DeviceFeatureMap<D3D12_FEATURE_D3D12_OPTIONS11>
	{
		using type = D3D12_FEATURE_DATA_D3D12_OPTIONS11;
	};

	class SetEventOnMultipleFenceCompletionData
	{
		UINT count;
		ID3D12Fence* const* fenceViews;
		const UINT64* fenceValues;

	public:
		template<UINT Count>
		SetEventOnMultipleFenceCompletionData(std::span<ID3D12Fence*, Count> fences, std::span<UINT64, Count> fenceValues) :
			count{ Count },
			fenceViews{ fences.data() },
			fenceValues{ fenceValues.data() }
		{

		}

		SetEventOnMultipleFenceCompletionData(UINT count, ID3D12Fence* const* fences, UINT64* fenceValues) :
			count{ count },
			fenceViews{ fences },
			fenceValues{ fenceValues }
		{

		}

	public:
		UINT GetCount() const noexcept { return count; }
		ID3D12Fence* const* GetFences() const noexcept { return fenceViews; }
		const UINT64* GetFenceValues() const noexcept { return fenceValues; }
	};

	class SetResidencyPriorityData
	{
		UINT count;
		ID3D12Pageable* const* pageables;
		const D3D12_RESIDENCY_PRIORITY* priorities;

	public:
		template<UINT Count>
		SetResidencyPriorityData(std::span<ID3D12Pageable*, Count> pageables, std::span<D3D12_RESIDENCY_PRIORITY, Count> priorities) :
			count{ Count },
			pageables{ pageables.data() },
			priorities{ priorities.data() }
		{

		}

		SetResidencyPriorityData(UINT count, ID3D12Pageable* const* pageables, D3D12_RESIDENCY_PRIORITY* priorities) :
			count{ count },
			pageables{ pageables },
			priorities{ priorities }
		{

		}

	public:
		UINT GetCount() const noexcept { return count; }
		ID3D12Pageable* const* GetPageables() const noexcept { return pageables; }
		const D3D12_RESIDENCY_PRIORITY* GetPriorities() const noexcept { return priorities; }
	};
}

namespace TypedD3D
{
	template<>
	struct Trait<Untagged<ID3D12Device>>
	{
		using inner_type = ID3D12Device;

		using inner_tag = ID3D12Device;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : InterfaceBase<Untagged<Derived>>
		{
			UINT GetNodeCount()
			{
				return Self().GetNodeCount();
			}

			template<D3D12_COMMAND_LIST_TYPE Type>
			StrongWrapper<D3D12::CommandListTypeToTrait<Type, ID3D12CommandQueue>> CreateCommandQueue(
				D3D12_COMMAND_QUEUE_PRIORITY priority,
				D3D12_COMMAND_QUEUE_FLAGS flags,
				UINT nodeMask)
			{
				D3D12_COMMAND_QUEUE_DESC desc
				{
					.Type = Type,
					.Priority = priority,
					.Flags = flags,
					.NodeMask = nodeMask
				};

				return ForwardFunction<StrongWrapper<D3D12::CommandListTypeToTrait<Type, ID3D12CommandQueue>>>(&ID3D12Device::CreateCommandQueue, Self(), &desc);
			}

			template<D3D12_COMMAND_LIST_TYPE Type>
			StrongWrapper<D3D12::CommandListTypeToTrait<Type, ID3D12CommandAllocator>> CreateCommandAllocator()
			{
				return ForwardFunction<StrongWrapper<D3D12::CommandListTypeToTrait<Type, ID3D12CommandAllocator>>>(&ID3D12Device::CreateCommandAllocator, Self(), Type);
			}

			Graphics<ID3D12PipelineState> CreateGraphicsPipelineState(
				const D3D12_GRAPHICS_PIPELINE_STATE_DESC& pDesc)
			{
				return ForwardFunction<Graphics<ID3D12PipelineState>>(&ID3D12Device::CreateGraphicsPipelineState, Self(), &pDesc);
			}

			Compute<ID3D12PipelineState> CreateComputePipelineState(
				const D3D12_COMPUTE_PIPELINE_STATE_DESC& pDesc)
			{
				return ForwardFunction<Compute<ID3D12PipelineState>>(&ID3D12Device::CreateComputePipelineState, Self(), &pDesc);
			}

			template<D3D12_COMMAND_LIST_TYPE Type, std::derived_from<ID3D12CommandList> ListTy = ID3D12GraphicsCommandList>
			StrongWrapper<D3D12::CommandListTypeToTrait<Type, ListTy>> CreateCommandList(
				WeakWrapper<D3D12::CommandListTypeToTrait<Type, ID3D12CommandAllocator>> pCommandAllocator,
				UINT nodeMask = 0,
				ID3D12PipelineState* optInitialState = nullptr)
			{
				return ForwardFunction<StrongWrapper<D3D12::CommandListTypeToTrait<Type, ListTy>>>(&ID3D12Device::CreateCommandList, Self(), nodeMask, Type, pCommandAllocator.Get(), optInitialState);
			}

			template<D3D12_FEATURE Feature>
			typename D3D12::DeviceFeatureMap<Feature>::type CheckFeatureSupport()
			{
				using feature_t = typename D3D12::DeviceFeatureMap<Feature>::type;
				feature_t feature{};
				ThrowIfFailed(Self().CheckFeatureSupport(Feature, &feature, sizeof(feature_t)));
				return feature;
			}

			template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlag>
			StrongWrapper<D3D12::DescriptorHeapTypeToTrait<Type, HeapFlag, ID3D12DescriptorHeap>> CreateDescriptorHeap(
				UINT NumDescriptors,
				UINT NodeMask)
			{
				D3D12_DESCRIPTOR_HEAP_DESC desc
				{
					.Type = Type,
					.NumDescriptors = NumDescriptors,
					.Flags = HeapFlag,
					.NodeMask = NodeMask
				};

				return ForwardFunction<StrongWrapper<D3D12::DescriptorHeapTypeToTrait<Type, HeapFlag, ID3D12DescriptorHeap>>>(&ID3D12Device::CreateDescriptorHeap, Self(), &desc);
			}

			UINT GetDescriptorHandleIncrementSize(
				D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapType)
			{
				return Self().GetDescriptorHandleIncrementSize(DescriptorHeapType);
			}

			Wrapper<ID3D12RootSignature> CreateRootSignature(
				UINT nodeMask,
				const void* pBlobWithRootSignature,
				SIZE_T blobLengthInBytes)
			{
				return ForwardFunction<Wrapper<ID3D12RootSignature>>(&ID3D12Device::CreateRootSignature, Self(), nodeMask, pBlobWithRootSignature, blobLengthInBytes);
			}

			void CreateConstantBufferView(
				const D3D12_CONSTANT_BUFFER_VIEW_DESC& pDesc,
				CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
			{
				Self().CreateConstantBufferView(pDesc, DestDescriptor.Raw());
			}

			void CreateConstantBufferView(
				const D3D12_CONSTANT_BUFFER_VIEW_DESC& pDesc,
				ShaderVisible<CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE>> DestDescriptor)
			{
				Self().CreateConstantBufferView(pDesc, DestDescriptor.Raw());
			}

			void CreateShaderResourceView(
				gsl::not_null<WrapperView<ID3D12Resource>> pResource,
				const D3D12_SHADER_RESOURCE_VIEW_DESC* optDesc,
				CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
			{
				Self().CreateShaderResourceView(pResource.get().Get(), optDesc, DestDescriptor.Raw());
			}

			void CreateShaderResourceView(
				gsl::not_null<WrapperView<ID3D12Resource>> pResource,
				const D3D12_SHADER_RESOURCE_VIEW_DESC* optDesc,
				ShaderVisible<CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE>> DestDescriptor)
			{
				Self().CreateShaderResourceView(pResource.get().Get(), optDesc, DestDescriptor.Raw());
			}

			void CreateNullShaderResourceView(
				const D3D12_SHADER_RESOURCE_VIEW_DESC& pDesc,
				CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
			{
				Self().CreateShaderResourceView(nullptr, &pDesc, DestDescriptor.Raw());
			}

			void CreateNullShaderResourceView(
				const D3D12_SHADER_RESOURCE_VIEW_DESC& pDesc,
				ShaderVisible<CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE>> DestDescriptor)
			{
				Self().CreateShaderResourceView(nullptr, &pDesc, DestDescriptor.Raw());
			}

			void CreateUnorderedAccessView(
				gsl::not_null<WrapperView<ID3D12Resource>> pResource,
				gsl::not_null<WrapperView<ID3D12Resource>> pCounterResource,
				const D3D12_UNORDERED_ACCESS_VIEW_DESC& pDesc,
				CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
			{
				Self().CreateUnorderedAccessView(pResource.get().Get(), pCounterResource.get().Get(), &pDesc, DestDescriptor.Raw());
			}

			void CreateUnorderedAccessView(
				gsl::not_null<WrapperView<ID3D12Resource>> pResource,
				gsl::not_null<WrapperView<ID3D12Resource>> pCounterResource,
				const D3D12_UNORDERED_ACCESS_VIEW_DESC& pDesc,
				ShaderVisible<CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE>> DestDescriptor)
			{
				Self().CreateUnorderedAccessView(pResource.get().Get(), pCounterResource.get().Get(), &pDesc, DestDescriptor.Raw());
			}

			void CreateRenderTargetView(
				gsl::not_null<WrapperView<ID3D12Resource>> Resource,
				const D3D12_RENDER_TARGET_VIEW_DESC* optDesc,
				RTV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
			{
				Self().CreateRenderTargetView(Resource.get().Get(), optDesc, DestDescriptor.Raw());
			}

			void CreateNullRenderTargetView(
				const D3D12_RENDER_TARGET_VIEW_DESC& Desc,
				RTV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
			{
				Self().CreateRenderTargetView(nullptr, &Desc, DestDescriptor.Raw());
			}

			void CreateDepthStencilView(
				gsl::not_null<WrapperView<ID3D12Resource>> Resource,
				const D3D12_DEPTH_STENCIL_VIEW_DESC* optDesc,
				DSV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
			{
				Self().CreateDepthStencilView(Resource.get().Get(), optDesc, DestDescriptor.Raw());
			}

			void CreateNullDepthStencilView(
				const D3D12_DEPTH_STENCIL_VIEW_DESC& optDesc,
				DSV<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
			{
				Self().CreateDepthStencilView(nullptr, optDesc, DestDescriptor.Raw());
			}

			void CreateSampler(
				const D3D12_SAMPLER_DESC& pDesc,
				Sampler<D3D12_CPU_DESCRIPTOR_HANDLE> DestDescriptor)
			{
				Self().CreateSampler(&pDesc, DestDescriptor.Raw());
			}

			void CreateSampler(
				const D3D12_SAMPLER_DESC& pDesc,
				ShaderVisible<Sampler<D3D12_CPU_DESCRIPTOR_HANDLE>> DestDescriptor)
			{
				Self().CreateSampler(&pDesc, DestDescriptor.Raw());
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
				Self().CopyDescriptors(
					NumDestDescriptorRanges,
					pDestDescriptorRangeStarts,
					pDestDescriptorRangeSizes,
					NumSrcDescriptorRanges,
					pSrcDescriptorRangeStarts,
					pSrcDescriptorRangeSizes,
					DescriptorHeapsType);
			}

			template<template<class> class Tag>
			void CopyDescriptorsSimple(
				UINT NumDescriptors,
				TypedStruct<Tag<D3D12_CPU_DESCRIPTOR_HANDLE>> DestDescriptorRangeStart,
				TypedStruct<Tag<D3D12_CPU_DESCRIPTOR_HANDLE>> SrcDescriptorRangeStart)
			{
				Self().CopyDescriptorsSimple(NumDescriptors, DestDescriptorRangeStart.Raw(), SrcDescriptorRangeStart.Raw(), D3D12::DescriptorHeapTraitToType<Tag<D3D12_CPU_DESCRIPTOR_HANDLE>>);
			}

			template<template<class> class Tag>
			void CopyDescriptorsSimple(
				UINT NumDescriptors,
				TypedStruct<ShaderVisibleTag<Tag<D3D12_CPU_DESCRIPTOR_HANDLE>>> DestDescriptorRangeStart,
				TypedStruct<Tag<D3D12_CPU_DESCRIPTOR_HANDLE>> SrcDescriptorRangeStart)
			{
				Self().CopyDescriptorsSimple(NumDescriptors, DestDescriptorRangeStart.Raw(), SrcDescriptorRangeStart.Raw(), D3D12::DescriptorHeapTraitToType<Tag<D3D12_CPU_DESCRIPTOR_HANDLE>>);
			}

			D3D12_RESOURCE_ALLOCATION_INFO GetResourceAllocationInfo(
				UINT visibleMask,
				const std::span<D3D12_RESOURCE_DESC> resourceDescs)
			{
				return Self().GetResourceAllocationInfo(
					visibleMask,
					static_cast<UINT>(resourceDescs.size()),
					resourceDescs.data());
			}

			D3D12_HEAP_PROPERTIES GetCustomHeapProperties(
				UINT nodeMask,
				D3D12_HEAP_TYPE heapType)
			{
				return Self().GetCustomHeapProperties(nodeMask, heapType);
			}

			Wrapper<ID3D12Resource> CreateCommittedResource(
				const D3D12_HEAP_PROPERTIES& pHeapProperties,
				D3D12_HEAP_FLAGS HeapFlags,
				const D3D12_RESOURCE_DESC& pDesc,
				D3D12_RESOURCE_STATES InitialResourceState,
				const D3D12_CLEAR_VALUE* optOptimizedClearValue)
			{
				return ForwardFunction<Wrapper<ID3D12Resource>>(&ID3D12Device::CreateCommittedResource, Self(),
					&pHeapProperties,
					HeapFlags,
					&pDesc,
					InitialResourceState,
					optOptimizedClearValue);
			}

			Wrapper<ID3D12Heap> CreateHeap(
				const D3D12_HEAP_DESC& pDesc)
			{
				return ForwardFunction<Wrapper<ID3D12Heap>>(&ID3D12Device::CreateHeap, Self(), &pDesc);
			}

			Wrapper<ID3D12Resource> CreatePlacedResource(
				gsl::not_null<WrapperView<ID3D12Heap>> pHeap,
				UINT64 HeapOffset,
				const D3D12_RESOURCE_DESC& pDesc,
				D3D12_RESOURCE_STATES InitialState,
				const D3D12_CLEAR_VALUE* optOptimizedClearValue)
			{
				return ForwardFunction<Wrapper<ID3D12Resource>>(&ID3D12Device::CreatePlacedResource, Self(),
					pHeap.get().Get(),
					HeapOffset,
					&pDesc,
					InitialState,
					optOptimizedClearValue);
			}

			Wrapper<ID3D12Resource> CreateReservedResource(
				const D3D12_RESOURCE_DESC& pDesc,
				D3D12_RESOURCE_STATES InitialState,
				const D3D12_CLEAR_VALUE* optOptimizedClearValue)
			{
				return ForwardFunction<Wrapper<ID3D12Resource>>(&ID3D12Device::CreateReservedResource, Self(),
					&pDesc,
					InitialState,
					optOptimizedClearValue);
			}

			HANDLE CreateSharedHandle(
				gsl::not_null<ID3D12DeviceChild*> pObject,
				const SECURITY_ATTRIBUTES* optAttributes,
				DWORD Access,
				LPCWSTR Name)
			{
				HANDLE handle;

				ThrowIfFailed(Self().CreateSharedHandle(
					pObject.get(),
					optAttributes,
					Access,
					Name,
					&handle));

				return handle;
			}

			//TODO: Figure out how this works to update to a more modern API
			HRESULT OpenSharedHandle(
				HANDLE NTHandle,
				const IID& riid,
				void** ppvObj)
			{
				return Self().OpenSharedHandle(
					NTHandle,
					riid,
					ppvObj);
			}

			HANDLE OpenSharedHandleByName(
				LPCWSTR Name,
				DWORD Access)
			{
				HANDLE handle;

				ThrowIfFailed(Self().OpenSharedHandleByName(
					Name,
					Access,
					&handle));

				return handle;
			}

			void MakeResident(
				const std::span<ID3D12Pageable*> objects)
			{
				ThrowIfFailed(Self().MakeResident(
					static_cast<UINT>(objects.size()),
					objects.data()));
			}

			void Evict(
				const std::span<ID3D12Pageable*> objects)
			{
				ThrowIfFailed(Self().Evict(
					static_cast<UINT>(objects.size()),
					objects.data()));
			}

			Wrapper<ID3D12Fence> CreateFence(
				UINT64 InitialValue,
				D3D12_FENCE_FLAGS Flags)
			{
				return ForwardFunction<Wrapper<ID3D12Fence>>(&ID3D12Device::CreateFence, Self(), InitialValue, Flags);
			}

			HRESULT GetDeviceRemovedReason()
			{
				return Self().GetDeviceRemovedReason();
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
				Self().GetCopyableFootprints(
					&pResourceDesc,
					FirstSubresource,
					NumSubresources,
					BaseOffset,
					optOutLayouts,
					optOutNumRows,
					optOutRowSizeInBytes,
					optOutTotalBytes);
			}

			Wrapper<ID3D12QueryHeap> CreateQueryHeap(
				const D3D12_QUERY_HEAP_DESC& pDesc)
			{
				return ForwardFunction<Wrapper<ID3D12QueryHeap>>(&ID3D12Device::CreateQueryHeap, Self(), &pDesc);
			}

			void SetStablePowerState(
				BOOL Enable)
			{
				ThrowIfFailed(Self().SetStablePowerState(Enable));
			}

			Wrapper<ID3D12CommandSignature> CreateCommandSignature(
				const D3D12_COMMAND_SIGNATURE_DESC& pDesc,
				ID3D12RootSignature* optRootSignature)
			{
				return ForwardFunction<Wrapper<ID3D12CommandSignature>>(&ID3D12Device::CreateCommandSignature, Self(), &pDesc, optRootSignature);
			}

			void GetResourceTiling(
				gsl::not_null<WrapperView<ID3D12Resource>> pTiledResource,
				UINT* optOutNumTilesForEntireResource,
				D3D12_PACKED_MIP_INFO* optOutPackedMipDesc,
				D3D12_TILE_SHAPE* optOutStandardTileShapeForNonPackedMips,
				UINT* pNumSubresourceTilings,
				UINT FirstSubresourceTilingToGet,
				D3D12_SUBRESOURCE_TILING& pSubresourceTilingsForNonPackedMips)
			{
				Self().GetResourceTiling(
					pTiledResource.get().Get(),
					optOutNumTilesForEntireResource,
					optOutPackedMipDesc,
					optOutStandardTileShapeForNonPackedMips,
					pNumSubresourceTilings,
					FirstSubresourceTilingToGet,
					&pSubresourceTilingsForNonPackedMips);
			}

			LUID GetAdapterLuid()
			{
				return Self().GetAdapterLuid();
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<ID3D12Device1>>
	{
		using inner_type = ID3D12Device1;

		using inner_tag = ID3D12Device1;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Untagged<ID3D12Device>, Derived>
		{
			Wrapper<ID3D12PipelineLibrary> CreatePipelineLibrary(
				const void* pLibraryBlob,
				SIZE_T BlobLength)
			{
				return ForwardFunction<Wrapper<ID3D12PipelineLibrary>>(ID3D12Device1::CreatePipelineLibrary, Self(), pLibraryBlob, BlobLength);
			}

			void SetEventOnMultipleFenceCompletion(
				D3D12::SetEventOnMultipleFenceCompletionData data,
				D3D12_MULTIPLE_FENCE_WAIT_FLAGS Flags,
				HANDLE hEvent)
			{

				ThrowIfFailed(Self().SetEventOnMultipleFenceCompletion(
					data.GetFences(),
					data.GetFenceValues(),
					data.GetCount(),
					Flags,
					hEvent));
			}

			void SetResidencyPriority(D3D12::SetResidencyPriorityData data)
			{
				ThrowIfFailed(Self().SetResidencyPriority(
					data.GetCount(),
					data.GetPageables(),
					data.GetPriorities()));
			}

			void SetResidencyPriority(ID3D12Pageable* page, D3D12_RESIDENCY_PRIORITY priority)
			{
				SetResidencyPriority({ 1, &page, &priority });
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<ID3D12Device2>>
	{
		using inner_type = ID3D12Device2;

		using inner_tag = ID3D12Device2;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Untagged<ID3D12Device1>, Derived>
		{
			using TraitInterface<Untagged<ID3D12Device1>, Derived>::Interface<Derived>::CreateGraphicsPipelineState;
			using TraitInterface<Untagged<ID3D12Device1>, Derived>::Interface<Derived>::CreateComputePipelineState;
			Graphics<ID3D12PipelineState> CreateGraphicsPipelineState(
				const D3D12_PIPELINE_STATE_STREAM_DESC pDesc)
			{
				return ForwardFunction<Graphics<ID3D12PipelineState>>(&ID3D12Device2::CreatePipelineState, Self(), &pDesc);
			}

			Compute<ID3D12PipelineState> CreateComputePipelineState(
				const D3D12_PIPELINE_STATE_STREAM_DESC pDesc)
			{
				return ForwardFunction<Compute<ID3D12PipelineState>>(&ID3D12Device2::CreatePipelineState, Self(), &pDesc);
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<ID3D12Device3>>
	{
		using inner_type = ID3D12Device3;

		using inner_tag = ID3D12Device3;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Untagged<ID3D12Device2>, Derived>
		{
			Wrapper<ID3D12Heap> OpenExistingHeapFromAddress(
				const void* pAddress)
			{
				return ForwardFunction<Wrapper<ID3D12Heap>>(&ID3D12Device3::OpenExistingHeapFromAddress, Self(), pAddress);
			}

			Wrapper<ID3D12Heap> OpenExistingHeapFromFileMapping(
				HANDLE hFileMapping)
			{
				return ForwardFunction<Wrapper<ID3D12Heap>>(&ID3D12Device3::OpenExistingHeapFromFileMapping, Self(), hFileMapping);
			}

			void EnqueueMakeResident(
				D3D12_RESIDENCY_FLAGS Flags,
				std::span<ID3D12Pageable*> ppObjects,
				gsl::not_null<WrapperView<ID3D12Fence>> pFenceToSignal,
				UINT64 FenceValueToSignal)
			{
				ThrowIfFailed(Self().EnqueueMakeResident(
					Flags,
					static_cast<UINT>(ppObjects.size()),
					ppObjects.data(),
					pFenceToSignal.get().Self(), FenceValueToSignal));
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<ID3D12Device4>>
	{
		using inner_type = ID3D12Device4;

		using inner_tag = ID3D12Device4;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Untagged<ID3D12Device3>, Derived>
		{
			template<D3D12_COMMAND_LIST_TYPE Type, std::derived_from<ID3D12CommandList> ListTy = ID3D12GraphicsCommandList>
			StrongWrapper<D3D12::CommandListTypeToTrait<Type, ListTy>> CreateCommandList1(
				UINT nodeMask,
				D3D12_COMMAND_LIST_FLAGS flags)
			{
				return ForwardFunction<StrongWrapper<D3D12::CommandListTypeToTrait<Type, ListTy>>>(&ID3D12Device4::CreateCommandList1, Self(), nodeMask, Type, flags);
			}

			Wrapper<ID3D12ProtectedResourceSession> CreateProtectedResourceSession(
				const D3D12_PROTECTED_RESOURCE_SESSION_DESC& pDesc)
			{
				return ForwardFunction<Wrapper<ID3D12ProtectedResourceSession>>(&ID3D12Device4::CreateProtectedResourceSession, Self(), &pDesc);
			}

			Wrapper<ID3D12Resource> CreateCommittedResource1(
				const D3D12_HEAP_PROPERTIES& pHeapProperties,
				D3D12_HEAP_FLAGS HeapFlags,
				const D3D12_RESOURCE_DESC& pDesc,
				D3D12_RESOURCE_STATES InitialResourceState,
				const D3D12_CLEAR_VALUE* pOptimizedClearValue,
				WrapperView<ID3D12ProtectedResourceSession> pProtectedSession)
			{
				return ForwardFunction<Wrapper<ID3D12Resource>>(
					&ID3D12Device4::CreateCommittedResource1,
					Self(),
					&pHeapProperties,
					HeapFlags,
					&pDesc,
					InitialResourceState,
					pOptimizedClearValue,
					pProtectedSession);
			}

			Wrapper<ID3D12Heap> CreateHeap1(
				const D3D12_HEAP_DESC& pDesc,
				WrapperView<ID3D12ProtectedResourceSession> pProtectedSession)
			{
				return ForwardFunction<Wrapper<ID3D12Heap>>(
					&ID3D12Device4::CreateHeap1,
					Self(),
					&pDesc,
					pProtectedSession);
			}

			Wrapper<ID3D12Resource> CreateReservedResource1(
				const D3D12_RESOURCE_DESC& pDesc,
				D3D12_RESOURCE_STATES InitialState,
				const D3D12_CLEAR_VALUE* pOptimizedClearValue,
				WrapperView<ID3D12ProtectedResourceSession> pProtectedSession)
			{
				return ForwardFunction<Wrapper<ID3D12Heap>>(
					&ID3D12Device4::CreateReservedResource1,
					Self(),
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
				Self().GetResourceAllocationInfo1(visibleMask, static_cast<UINT>(pResourceDescs.size()), pResourceDescs.data(), pResourceAllocationInfo1.data());
				return pResourceAllocationInfo1;
			}

			void GetResourceAllocationInfo1(
				UINT visibleMask,
				std::span<const D3D12_RESOURCE_DESC> pResourceDescs,
				std::span<D3D12_RESOURCE_ALLOCATION_INFO1> pResourceAllocationInfo1)
			{
				assert(pResourceDescs.size() == pResourceAllocationInfo1.size());
				Self().GetResourceAllocationInfo1(visibleMask, static_cast<UINT>(pResourceDescs.size()), pResourceDescs.data(), pResourceAllocationInfo1.data());
				return pResourceAllocationInfo1;
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<ID3D12Device5>>
	{
		using inner_type = ID3D12Device5;

		using inner_tag = ID3D12Device5;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Untagged<ID3D12Device4>, Derived>
		{
			//TODO: Figure out how this works to update to a more modern API
			void CreateLifetimeTracker(
				gsl::not_null<WrapperView<ID3D12LifetimeOwner>> pOwner,
				REFIID riid,
				void** ppvTracker)
			{
				ThrowIfFailed(Self().CreateLifetimeTracker(pOwner.get().Get(), riid, ppvTracker));
			}

			void RemoveDevice()
			{
				Self().RemoveDevice();
			}

			UINT GetNumMetaCommands()
			{
				UINT pNumMetaCommands = 0;
				Self().EnumerateMetaCommands(&pNumMetaCommands, nullptr);
				return pNumMetaCommands;
			}

			std::vector<D3D12_META_COMMAND_DESC> EnumerateMetaCommands(
				UINT pNumMetaCommands)
			{
				std::vector<D3D12_META_COMMAND_DESC> pDescs(pNumMetaCommands);
				ThrowIfFailed(Self().EnumerateMetaCommands(&pNumMetaCommands, pDescs.data()));
				return pDescs;
			}

			UINT GetNumMetaCommandParameters(
				REFGUID CommandId,
				D3D12_META_COMMAND_PARAMETER_STAGE Stage)
			{
				UINT pNumMetaCommandParams = 0;
				Self().EnumerateMetaCommandParameters(CommandId, Stage, nullptr, &pNumMetaCommandParams, nullptr);
				return pNumMetaCommandParams;
			}

			D3D12::MetaCommandParameterInfo STDMETHODCALLTYPE EnumerateMetaCommandParameters(
				_In_  REFGUID CommandId,
				_In_  D3D12_META_COMMAND_PARAMETER_STAGE Stage,
				UINT parameterCount)
			{
				D3D12::MetaCommandParameterInfo info{};

				info.parameterCount = parameterCount;
				info.parameterDescs.resize(parameterCount);

				ThrowIfFailed(Self().EnumerateMetaCommandParameters(
					CommandId,
					Stage,
					&info.totalStructureSizeInBytes,
					&info.parameterCount,
					info.parameterDescs.data()));

				return info;
			}

			Wrapper<ID3D12MetaCommand> CreateMetaCommand(
				REFGUID CommandId,
				UINT NodeMask,
				const void* pCreationParametersData,
				SIZE_T CreationParametersDataSizeInBytes)
			{
				return ForwardFunction<Wrapper<ID3D12MetaCommand>>(
					&ID3D12Device5::CreateMetaCommand,
					Self(),
					CommandId,
					NodeMask,
					pCreationParametersData,
					CreationParametersDataSizeInBytes);
			}

			template<class CreationParamStruct>
			Wrapper<ID3D12MetaCommand> CreateMetaCommand(
				REFGUID CommandId,
				UINT NodeMask,
				const CreationParamStruct& pCreationParametersData)
			{
				return CreateMetaCommand(CommandId, NodeMask, &pCreationParametersData, sizeof(CreationParamStruct));
			}

			Wrapper<ID3D12MetaCommand> CreateMetaCommand(
				REFGUID CommandId,
				UINT NodeMask)
			{
				return CreateMetaCommand(CommandId, NodeMask, nullptr, 0);
			}

			Wrapper<ID3D12StateObject> CreateStateObject(
				const D3D12_STATE_OBJECT_DESC& pDesc)
			{
				return ForwardFunction<Wrapper<ID3D12StateObject>>(
					&ID3D12Device5::CreateStateObject,
					Self(),
					&pDesc);
			}

			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO GetRaytracingAccelerationStructurePrebuildInfo(
				const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& pDesc)
			{
				D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO prebuildInfo;
				Self().GetRaytracingAccelerationStructurePrebuildInfo(&pDesc, &prebuildInfo);
				return prebuildInfo;
			}

			D3D12_DRIVER_MATCHING_IDENTIFIER_STATUS CheckDriverMatchingIdentifier(
				D3D12_SERIALIZED_DATA_TYPE SerializedDataType,
				const D3D12_SERIALIZED_DATA_DRIVER_MATCHING_IDENTIFIER& pIdentifierToCheck)
			{
				return Self().CheckDriverMatchingIdentifier(
					SerializedDataType,
					&pIdentifierToCheck);
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<ID3D12Device6>>
	{
		using inner_type = ID3D12Device6;

		using inner_tag = ID3D12Device6;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Untagged<ID3D12Device5>, Derived>
		{
			//TODO: Figure out how this works to update to a more modern API
			HRESULT SetBackgroundProcessingMode(
				D3D12_BACKGROUND_PROCESSING_MODE Mode,
				D3D12_MEASUREMENTS_ACTION MeasurementsAction,
				_In_opt_  HANDLE hEventToSignalUponCompletion,
				_Out_opt_  BOOL* pbFurtherMeasurementsDesired)
			{
				return Self().SetBackgroundProcessingMode(
					Mode,
					MeasurementsAction,
					hEventToSignalUponCompletion,
					pbFurtherMeasurementsDesired);
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<ID3D12Device7>>
	{
		using inner_type = ID3D12Device7;

		using inner_tag = ID3D12Device7;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Untagged<ID3D12Device6>, Derived>
		{
			//TODO: Figure out how this works to update to a more modern API
			HRESULT AddToStateObject(
				const D3D12_STATE_OBJECT_DESC* pAddition,
				ID3D12StateObject* pStateObjectToGrowFrom,
				REFIID riid,
				_COM_Outptr_  void** ppNewStateObject)
			{
				return Self().AddToStateObject(
					pAddition,
					pStateObjectToGrowFrom,
					riid,
					ppNewStateObject);
			}

			//TODO: Figure out how this works to update to a more modern API
			HRESULT CreateProtectedResourceSession1(
				const D3D12_PROTECTED_RESOURCE_SESSION_DESC1* pDesc,
				REFIID riid,
				void** ppSession)
			{
				return Self().CreateProtectedResourceSession1(
					pDesc,
					riid,
					ppSession);
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<ID3D12Device8>>
	{
		using inner_type = ID3D12Device8;

		using inner_tag = ID3D12Device8;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Untagged<ID3D12Device7>, Derived>
		{
#if defined(_MSC_VER) || !defined(_WIN32)
			D3D12_RESOURCE_ALLOCATION_INFO GetResourceAllocationInfo2(
				UINT visibleMask,
				UINT numResourceDescs,
				const D3D12_RESOURCE_DESC1* pResourceDescs,
				D3D12_RESOURCE_ALLOCATION_INFO1* pResourceAllocationInfo1)
			{
				return Self().GetResourceAllocationInfo2(visibleMask,
					numResourceDescs,
					pResourceDescs,
					pResourceAllocationInfo1);
			}
#else
			D3D12_RESOURCE_ALLOCATION_INFO* STDMETHODCALLTYPE GetResourceAllocationInfo2(
				D3D12_RESOURCE_ALLOCATION_INFO* RetVal,
				UINT visibleMask,
				UINT numResourceDescs,
				const D3D12_RESOURCE_DESC1* pResourceDescs,
				D3D12_RESOURCE_ALLOCATION_INFO1* pResourceAllocationInfo1)
			{
				return Self().GetResourceAllocationInfo2(RetVal,
					visibleMask,
					numResourceDescs,
					pResourceDescs,
					pResourceAllocationInfo1);
			}
#endif

			Wrapper<ID3D12Resource> CreateCommittedResource2(
				const D3D12_HEAP_PROPERTIES& pHeapProperties,
				D3D12_HEAP_FLAGS HeapFlags,
				const D3D12_RESOURCE_DESC1& pDesc,
				D3D12_RESOURCE_STATES InitialResourceState,
				const D3D12_CLEAR_VALUE* pOptimizedClearValue,
				ID3D12ProtectedResourceSession* pProtectedSession)
			{
				return ForwardFunction<Wrapper<ID3D12Resource>>(&inner_type::CreateCommittedResource2, Self(),
					&pHeapProperties,
					HeapFlags,
					&pDesc,
					InitialResourceState,
					pOptimizedClearValue,
					pProtectedSession);
			}

			Wrapper<ID3D12Resource> CreatePlacedResource1(
				gsl::not_null<WrapperView<ID3D12Heap>> pHeap,
				UINT64 HeapOffset,
				const D3D12_RESOURCE_DESC& pDesc,
				D3D12_RESOURCE_STATES InitialState,
				const D3D12_CLEAR_VALUE* pOptimizedClearValue)
			{
				return ForwardFunction<Wrapper<ID3D12Resource>>(&inner_type::CreatePlacedResource1, Self(),
					pHeap.get().Get(),
					HeapOffset,
					&pDesc,
					InitialState,
					pOptimizedClearValue);
			}

			void CreateSamplerFeedbackUnorderedAccessView(
				ID3D12Resource* pTargetedResource,
				ID3D12Resource* pFeedbackResource,
				D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor)
			{
				Self().CreateSamplerFeedbackUnorderedAccessView(
					pTargetedResource,
					pFeedbackResource,
					DestDescriptor);
			}

			void GetCopyableFootprints1(
				const D3D12_RESOURCE_DESC1& pResourceDesc,
				UINT FirstSubresource,
				UINT NumSubresources,
				UINT64 BaseOffset,
				D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
				UINT* pNumRows,
				UINT64* pRowSizeInBytes,
				UINT64* pTotalBytes)
			{
				Self().GetCopyableFootprints1(
					&pResourceDesc,
					FirstSubresource,
					NumSubresources,
					BaseOffset,
					pLayouts,
					pNumRows,
					pRowSizeInBytes,
					pTotalBytes);
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<ID3D12Device9>>
	{
		using inner_type = ID3D12Device9;

		using inner_tag = ID3D12Device9;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Untagged<ID3D12Device8>, Derived>
		{
			Wrapper<ID3D12ShaderCacheSession> CreateShaderCacheSession(
				const D3D12_SHADER_CACHE_SESSION_DESC& pDesc)
			{
				return ForwardFunction<Wrapper<ID3D12ShaderCacheSession>>(&inner_type::CreateShaderCacheSession, Self(), &pDesc);
			}

			HRESULT ShaderCacheControl(
				D3D12_SHADER_CACHE_KIND_FLAGS Kinds,
				D3D12_SHADER_CACHE_CONTROL_FLAGS Control)
			{
				return Self().ShaderCacheControl(Kinds, Control);
			}

			template<D3D12_COMMAND_LIST_TYPE Type>
			Wrapper<D3D12::CommandListTypeToTrait<Type, ID3D12CommandQueue>> CreateCommandQueue1(
				D3D12_COMMAND_QUEUE_PRIORITY priority,
				D3D12_COMMAND_QUEUE_FLAGS flags,
				UINT nodeMask,
				REFIID CreatorID)
			{
				D3D12_COMMAND_QUEUE_DESC desc
				{
					.Type = Type,
					.Priority = priority,
					.Flags = flags,
					.NodeMask = nodeMask
				};

				return ForwardFunction<Wrapper<D3D12::CommandListTypeToTrait<Type, ID3D12CommandQueue>>>(&inner_type::CreateCommandQueue1, Self(), &desc, CreatorID);
			}


		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<ID3D12Device10>>
	{
		using inner_type = ID3D12Device10;

		using inner_tag = ID3D12Device10;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Untagged<ID3D12Device9>, Derived>
		{
			Wrapper<ID3D12Resource2> CreateCommittedResource3(
				const D3D12_HEAP_PROPERTIES& pHeapProperties,
				D3D12_HEAP_FLAGS HeapFlags,
				const D3D12_RESOURCE_DESC1& pDesc,
				D3D12_BARRIER_LAYOUT InitialLayout,
				const D3D12_CLEAR_VALUE* pOptimizedClearValue,
				ID3D12ProtectedResourceSession* pProtectedSession,
				std::span<DXGI_FORMAT> CastableFormats)
			{
				return ForwardFunction<Wrapper<ID3D12Resource2>>(&inner_type::CreateCommittedResource3, Self(),
					&pHeapProperties,
					HeapFlags,
					&pDesc,
					InitialLayout,
					pOptimizedClearValue,
					pProtectedSession,
					static_cast<UINT32>(CastableFormats.size()),
					CastableFormats.data());
			}

			Wrapper<ID3D12Resource2> CreatePlacedResource2(
				gsl::not_null<WrapperView<ID3D12Heap>> pHeap,
				UINT64 HeapOffset,
				const D3D12_RESOURCE_DESC1& pDesc,
				D3D12_BARRIER_LAYOUT InitialLayout,
				const D3D12_CLEAR_VALUE* pOptimizedClearValue,
				std::span<DXGI_FORMAT> CastableFormats)
			{
				return ForwardFunction<Wrapper<ID3D12Resource2>>(&inner_type::CreatePlacedResource2, Self(),
					pHeap.get().Get(),
					HeapOffset,
					pDesc,
					InitialLayout,
					pOptimizedClearValue,
					static_cast<UINT32>(CastableFormats.size()),
					CastableFormats.data());
			}

			Wrapper<ID3D12Resource2> CreateReservedResource2(
				const D3D12_RESOURCE_DESC& pDesc,
				D3D12_BARRIER_LAYOUT InitialLayout,
				const D3D12_CLEAR_VALUE* pOptimizedClearValue,
				ID3D12ProtectedResourceSession* pProtectedSession,
				std::span<DXGI_FORMAT> CastableFormats)
			{
				return ForwardFunction<Wrapper<ID3D12Resource2>>(&inner_type::CreateReservedResource2, Self(),
					pDesc,
					InitialLayout,
					pOptimizedClearValue,
					pProtectedSession,
					static_cast<UINT32>(CastableFormats.size()),
					CastableFormats.data());
			}
		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};
}

namespace TypedD3D::D3D12
{
	export template<class DeviceTy = ID3D12Device>
		Wrapper<DeviceTy> CreateDevice(D3D_FEATURE_LEVEL minimumFeatureLevel, IDXGIAdapter* optAdapter = nullptr)
	{
		return ForwardFunction<Wrapper<DeviceTy>>(&D3D12CreateDevice, optAdapter, minimumFeatureLevel);
	}
}