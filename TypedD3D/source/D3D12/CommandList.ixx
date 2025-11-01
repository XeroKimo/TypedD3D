//Commandlist API restrictions https://docs.microsoft.com/en-us/windows/win32/direct3d12/recording-command-lists-and-bundles#command-list-api-restrictions

module;
#include <memory>
#include <d3d12.h>
#include <array>
#include <span>
#include "gsl/pointers"
#include <assert.h>
#include <optional>

#pragma warning (push)
#pragma warning (disable : 4584)

export module TypedD3D12:CommandList;
import :CommandAllocator;
import :DescriptorHeap;
import :DeviceChild;
import :D3D12Object;
import TypedD3D.Shared;
import :Wrappers;
import :PipelineState;

namespace TypedD3D::D3D12
{
	template<template<class> class Tag>
	concept CommandListEnabledTag = SameTagAs<Tag, DirectTag>
		|| SameTagAs<Tag, ComputeTag>
		|| SameTagAs<Tag, CopyTag>
		|| SameTagAs<Tag, BundleTag>
		|| SameTagAs<Tag, RenderPassTag>
		|| SameTagAs<Tag, Untagged>;

	template<template<class> class Tag, template<class> class... Compare>
	concept DisableFunction = !(SameTagAs<Tag, Compare> || ...);

	export class WriteBufferImmediateData
	{
		UINT count;
		D3D12_WRITEBUFFERIMMEDIATE_PARAMETER* parameters;
		D3D12_WRITEBUFFERIMMEDIATE_MODE* modes = nullptr;

	public:
		template<UINT Count>
		WriteBufferImmediateData(std::span<D3D12_WRITEBUFFERIMMEDIATE_PARAMETER, Count> parameters, std::optional<std::span<D3D12_WRITEBUFFERIMMEDIATE_MODE, Count>> modes) :
			count{ Count },
			parameters{ parameters.data() },
			modes{ modes.value_or({}).data() }
		{

		}

		WriteBufferImmediateData(UINT count, D3D12_WRITEBUFFERIMMEDIATE_PARAMETER* parameters, D3D12_WRITEBUFFERIMMEDIATE_MODE* optionalModes) :
			count{ count },
			parameters{ parameters },
			modes{ optionalModes }
		{

		}

	public:
		UINT GetCount() const noexcept { return count; }
		const D3D12_WRITEBUFFERIMMEDIATE_PARAMETER* GetParameters() const noexcept { return parameters; }
		const D3D12_WRITEBUFFERIMMEDIATE_MODE* GetModes() const noexcept { return modes; }
	};
};

namespace TypedD3D
{
	template<template<class> class Tag, std::derived_from<ID3D12GraphicsCommandList8> Ty>
		requires D3D12::CommandListEnabledTag<Tag>
	struct Trait<Tag<Ty>>
	{
		using inner_type = Ty;

		using inner_tag = Ty;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		using Interface = Ty*;
	};

	template<template<class> class Tag>
		requires D3D12::CommandListEnabledTag<Tag>
	struct Trait<Tag<ID3D12CommandList>>
	{
		using inner_type = ID3D12CommandList;

		using inner_tag = ID3D12CommandList;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		struct Interface : D3D12::DeviceChildTraits::Interface<Tag<Derived>>
		{
			D3D12_COMMAND_LIST_TYPE GetType() { return Self().GetType(); }
		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};

	template<template<class> class Tag>
		requires D3D12::CommandListEnabledTag<Tag>
	struct Trait<Tag<ID3D12GraphicsCommandList>>
	{
		using inner_type = ID3D12GraphicsCommandList;

		using inner_tag = ID3D12GraphicsCommandList;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Tag<ID3D12CommandList>, Derived>
		{
			void BeginEvent(
				UINT Metadata,
				const void* pData,
				UINT Size)
			{
				Self().BeginEvent(Metadata, pData, Size);
			}

			void BeginQuery(
				gsl::not_null<WrapperView<ID3D12QueryHeap>> pQueryHeap,
				D3D12_QUERY_TYPE Type,
				UINT Index) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag>
			{
				Self().BeginQuery(pQueryHeap.get().Get(), Type, Index);
			}

			void ClearDepthStencilView(
				DSV<D3D12_CPU_DESCRIPTOR_HANDLE> DepthStencilView,
				D3D12_CLEAR_FLAGS ClearFlags,
				FLOAT Depth,
				UINT8 Stencil,
				std::span<const D3D12_RECT> rects) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().ClearDepthStencilView(DepthStencilView.Raw(), ClearFlags, Depth, Stencil, static_cast<UINT>(rects.size()), rects.data());
			}

			void ClearRenderTargetView(
				RTV<D3D12_CPU_DESCRIPTOR_HANDLE> RenderTargetView,
				std::span<const float, 4> colorRGBA,
				std::span<const D3D12_RECT> rects) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().ClearRenderTargetView(RenderTargetView.Raw(), colorRGBA.data(), static_cast<UINT>(rects.size()), rects.data());
			}

			void ClearRenderTargetView(
				RTV<D3D12_CPU_DESCRIPTOR_HANDLE> RenderTargetView,
				const std::array<float, 4>& colorRGBA) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag, RenderPassTag>
			{
				ClearRenderTargetView(RenderTargetView, colorRGBA, {});
			}
			void ClearState(WrapperView<ID3D12PipelineState> pPipelineState) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().ClearState(pPipelineState.Get());
			}

			void ClearUnorderedAccessViewFloat(
				CBV_SRV_UAV<D3D12_GPU_DESCRIPTOR_HANDLE> ViewGPUHandleInCurrentHeap,
				CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE> ViewCPUHandle,
				gsl::not_null<WrapperView<ID3D12Resource>> pResource,
				std::span<const float, 4> values,
				std::span<const D3D12_RECT> rects) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().ClearUnorderedAccessViewFloat(ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource.get().Get(), values.data(), static_cast<UINT>(rects.size()), rects.data());
			}

			void ClearUnorderedAccessViewUint(
				CBV_SRV_UAV<D3D12_GPU_DESCRIPTOR_HANDLE> ViewGPUHandleInCurrentHeap,
				CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE> ViewCPUHandle,
				gsl::not_null<WrapperView<ID3D12Resource>> pResource,
				std::span<const UINT, 4> values,
				std::span<const D3D12_RECT> rects) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().ClearUnorderedAccessViewUint(ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource.get().Get(), values.data(), static_cast<UINT>(rects.size()), rects.data());
			}

			HRESULT Close()
			{
				return Self().Close();
			}

			void CopyBufferRegion(
				gsl::not_null<WrapperView<ID3D12Resource>> pDstBuffer,
				UINT64 DstOffset,
				gsl::not_null<WrapperView<ID3D12Resource>> pSrcBuffer,
				UINT64 SrcOffset,
				UINT64 NumBytes) requires D3D12::DisableFunction<Tag, BundleTag, RenderPassTag>
			{
				Self().CopyBufferRegion(
					pDstBuffer.get().Get(),
					DstOffset,
					pSrcBuffer.get().Get(),
					SrcOffset,
					NumBytes);
			}

			void CopyResource(
				gsl::not_null<WrapperView<ID3D12Resource>> pDstResource,
				gsl::not_null<WrapperView<ID3D12Resource>> pSrcResource) requires D3D12::DisableFunction<Tag, BundleTag, RenderPassTag>
			{
				Self().CopyResource(pDstResource.get().Get(), pSrcResource.get().Get());
			}

			void CopyTextureRegion(
				const D3D12_TEXTURE_COPY_LOCATION& pDst,
				UINT DstX,
				UINT DstY,
				UINT DstZ,
				const D3D12_TEXTURE_COPY_LOCATION& pSrc,
				const D3D12_BOX* pOptSrcBox) requires D3D12::DisableFunction<Tag, BundleTag, RenderPassTag>
			{
				Self().CopyTextureRegion(
					&pDst,
					DstX,
					DstY,
					DstZ,
					&pSrc,
					pOptSrcBox);
			}

			void CopyTiles(
				gsl::not_null<WrapperView<ID3D12Resource>> pTiledResource,
				const D3D12_TILED_RESOURCE_COORDINATE& pTileRegionStartCoordinate,
				const D3D12_TILE_REGION_SIZE& pTileRegionSize,
				gsl::not_null<WrapperView<ID3D12Resource>> pBuffer,
				UINT64 BufferStartOffsetInBytes,
				D3D12_TILE_COPY_FLAGS Flags) requires D3D12::DisableFunction<Tag, BundleTag, RenderPassTag>
			{
				Self().CopyTiles(
					pTiledResource.get().Get(),
					&pTileRegionStartCoordinate,
					&pTileRegionSize,
					pBuffer.get().Get(),
					BufferStartOffsetInBytes,
					Flags);
			}

			void DiscardResource(
				gsl::not_null<WrapperView<ID3D12Resource>> pResource,
				const D3D12_DISCARD_REGION* pOptRegion) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().DiscardResource(pResource.get().Get(), pOptRegion);
			}

			void Dispatch(
				UINT ThreadGroupCountX,
				UINT ThreadGroupCountY,
				UINT ThreadGroupCountZ) requires D3D12::DisableFunction<Tag, CopyTag, RenderPassTag>
			{
				Self().Dispatch(
					ThreadGroupCountX,
					ThreadGroupCountY,
					ThreadGroupCountZ);
			}

			void DrawIndexedInstanced(
				UINT IndexCountPerInstance,
				UINT InstanceCount,
				UINT StartIndexLocation,
				INT BaseVertexLocation,
				UINT StartInstanceLocation) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().DrawIndexedInstanced(
					IndexCountPerInstance,
					InstanceCount,
					StartIndexLocation,
					BaseVertexLocation,
					StartInstanceLocation);
			}

			void DrawInstanced(
				UINT VertexCountPerInstance,
				UINT InstanceCount,
				UINT StartVertexLocation,
				UINT StartInstanceLocation) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
			}

			void EndEvent()
			{
				Self().EndEvent();
			}

			void EndQuery(
				gsl::not_null<WrapperView<ID3D12QueryHeap>> pQueryHeap,
				D3D12_QUERY_TYPE Type,
				UINT Index) requires D3D12::DisableFunction<Tag, BundleTag>
			{
				Self().EndQuery(pQueryHeap.get().Get(), Type, Index);
			}

			template<template<class> class OtherTag = BundleTag>
				requires SameTagAs<OtherTag, BundleTag>
			void ExecuteBundle(gsl::not_null<WeakWrapper<OtherTag<ID3D12GraphicsCommandList>>> commandList) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag>
			{
				static_assert(IUnknownTrait<BundleTag<ID3D12GraphicsCommandList>>);
				Self().ExecuteBundle(commandList.get().Get());
			}

			void ExecuteIndirect(
				gsl::not_null<WrapperView<ID3D12CommandSignature>> commandSignature,
				UINT maxCommandCount,
				gsl::not_null<WrapperView<ID3D12Resource>> argumentBuffer,
				UINT64 argumentBufferOffset,
				WrapperView<ID3D12Resource> optCountBuffer = nullptr,
				UINT64 optCountBufferOffset = 0) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				Self().ExecuteIndirect(commandSignature.get().Get(), maxCommandCount, argumentBuffer.get().Get(), argumentBufferOffset, optCountBuffer.Get(), optCountBufferOffset);
			}

			void IASetIndexBuffer(
				const D3D12_INDEX_BUFFER_VIEW* pView) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().IASetIndexBuffer(pView);
			}

			void IASetPrimitiveTopology(
				D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().IASetPrimitiveTopology(PrimitiveTopology);
			}

			void IASetVertexBuffers(
				UINT StartSlot,
				std::span<const D3D12_VERTEX_BUFFER_VIEW> views) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().IASetVertexBuffers(StartSlot, static_cast<UINT>(views.size()), views.data());
			}

			void OMSetBlendFactor(std::span<const float, 4> BlendFactor) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().OMSetBlendFactor(BlendFactor.data());
			}

			void OMSetRenderTargets(
				Span<const RTV<D3D12_CPU_DESCRIPTOR_HANDLE>> pRenderTargetDescriptors,
				BOOL RTsSingleHandleToDescriptorRange,
				const DSV<D3D12_CPU_DESCRIPTOR_HANDLE>* pDepthStencilDescriptor) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag, RenderPassTag>
			{
				const D3D12_CPU_DESCRIPTOR_HANDLE* depthStencil = (pDepthStencilDescriptor) ? &pDepthStencilDescriptor->Raw() : nullptr;

				Self().OMSetRenderTargets(static_cast<UINT>(pRenderTargetDescriptors.size()), pRenderTargetDescriptors.data(), RTsSingleHandleToDescriptorRange, depthStencil);
			}

			void OMSetStencilRef(UINT StencilRef) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().OMSetStencilRef(StencilRef);
			}

			HRESULT Reset(gsl::not_null<WeakWrapper<Tag<ID3D12CommandAllocator>>> allocator,
				WrapperView<ID3D12PipelineState> optPipelineState)
			{
				return Self().Reset(allocator.get().Get(), optPipelineState.Get());
			}

			void ResolveQueryData(
				gsl::not_null<WrapperView<ID3D12QueryHeap>> pQueryHeap,
				D3D12_QUERY_TYPE Type,
				UINT StartIndex,
				UINT NumQueries,
				gsl::not_null<WrapperView<ID3D12Resource>> pDestinationBuffer,
				UINT64 AlignedDestinationBufferOffset) requires D3D12::DisableFunction<Tag, BundleTag, RenderPassTag>
			{
				Self().ResolveQueryData(pQueryHeap.get().Get(), Type, StartIndex, NumQueries, pDestinationBuffer.get().Get(), AlignedDestinationBufferOffset);
			}

			void ResolveSubresource(
				gsl::not_null<WrapperView<ID3D12Resource>> pDstResource,
				UINT DstSubresource,
				gsl::not_null<WrapperView<ID3D12Resource>> pSrcResource,
				UINT SrcSubresource,
				DXGI_FORMAT Format) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().ResolveSubresource(
					pDstResource.get().Get(),
					DstSubresource,
					pSrcResource.get().Get(),
					SrcSubresource,
					Format);
			}

			void ResourceBarrier(std::span<const D3D12_RESOURCE_BARRIER> Barriers) requires D3D12::DisableFunction<Tag, BundleTag>
			{
				Self().ResourceBarrier(static_cast<UINT>(Barriers.size()), Barriers.data());
			}

			void RSSetScissorRects(
				std::span<D3D12_RECT> rects) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag>
			{
				Self().RSSetScissorRects(static_cast<UINT>(rects.size()), rects.data());
			}

			void RSSetViewports(
				std::span<D3D12_VIEWPORT> viewports) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag>
			{
				Self().RSSetViewports(static_cast<UINT>(viewports.size()), viewports.data());
			}

			void SetComputeRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				Self().SetComputeRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
			}

			void SetComputeRoot32BitConstants(
				UINT RootParameterIndex,
				UINT Num32BitValuesToSet,
				const void* pSrcData,
				UINT DestOffsetIn32BitValues) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				Self().SetComputeRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
			}

			void SetComputeRootConstantBufferView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				Self().SetComputeRootConstantBufferView(RootParameterIndex, BufferLocation);
			}

			void SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				Self().SetComputeRootDescriptorTable(RootParameterIndex, BaseDescriptor);
			}

			void SetComputeRootShaderResourceView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				Self().SetComputeRootShaderResourceView(RootParameterIndex, BufferLocation);
			}

			void SetComputeRootSignature(WrapperView<ID3D12RootSignature> pRootSignature) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				Self().SetComputeRootSignature(pRootSignature.Get());
			}

			void SetComputeRootUnorderedAccessView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				Self().SetComputeRootUnorderedAccessView(RootParameterIndex, BufferLocation);
			}

			void SetDescriptorHeaps(ShaderVisible<CBV_SRV_UAV<ID3D12DescriptorHeap>> descriptorHeap) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
				Self().SetDescriptorHeaps(1, heaps);
			}

			void SetDescriptorHeaps(ShaderVisible<Sampler<ID3D12DescriptorHeap>> descriptorHeap) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
				Self().SetDescriptorHeaps(1, heaps);
			}

			void SetDescriptorHeaps(ShaderVisible<CBV_SRV_UAV<ID3D12DescriptorHeap>> cbv_srv_uavHeap, ShaderVisible<Sampler<ID3D12DescriptorHeap>> samplerHeap) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				ID3D12DescriptorHeap* heaps[] = { cbv_srv_uavHeap.Get(), samplerHeap.Get() };
				Self().SetDescriptorHeaps(2, heaps);
			}

			void SetGraphicsRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().SetGraphicsRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
			}

			void SetGraphicsRoot32BitConstants(
				UINT RootParameterIndex,
				UINT Num32BitValuesToSet,
				const void* pSrcData,
				UINT DestOffsetIn32BitValues) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().SetGraphicsRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
			}

			void SetGraphicsRootConstantBufferView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().SetGraphicsRootConstantBufferView(RootParameterIndex, BufferLocation);
			}

			void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().SetGraphicsRootDescriptorTable(RootParameterIndex, BaseDescriptor);
			}

			void SetGraphicsRootShaderResourceView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().SetGraphicsRootShaderResourceView(RootParameterIndex, BufferLocation);
			}

			void SetGraphicsRootSignature(WrapperView<ID3D12RootSignature> pRootSignature) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().SetGraphicsRootSignature(pRootSignature.Get());
			}

			void SetGraphicsRootUnorderedAccessView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().SetGraphicsRootUnorderedAccessView(RootParameterIndex, BufferLocation);
			}

			void SetMarker(
				UINT Metadata,
				const void* pData,
				UINT Size)
			{
				Self().SetMarker(Metadata, pData, Size);
			}

			void SetPipelineState(WrapperView<ID3D12PipelineState> pPipelineState) requires D3D12::DisableFunction<Tag, CopyTag>
			{
				Self().SetPipelineState(pPipelineState.Get());
			}

			void SetPredication(
				WrapperView<ID3D12Resource> pBuffer,
				UINT64 AlignedBufferOffset,
				D3D12_PREDICATION_OP Operation) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag>
			{
				Self().SetPredication(pBuffer.Get(), AlignedBufferOffset, Operation);
			}

			void SOSetTargets(
				UINT StartSlot,
				std::span<const D3D12_STREAM_OUTPUT_BUFFER_VIEW> views) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag>
			{
				Self().SOSetTargets(StartSlot, static_cast<UINT>(views.size()), views.data());
			}

		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};

	template<template<class> class Tag>
		requires D3D12::CommandListEnabledTag<Tag>
	struct Trait<Tag<ID3D12GraphicsCommandList1>>
	{
		using inner_type = ID3D12GraphicsCommandList1;

		using inner_tag = ID3D12GraphicsCommandList1;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Tag<ID3D12GraphicsCommandList>, Derived>
		{
			void AtomicCopyBufferUINT(
				gsl::not_null<WrapperView<ID3D12Resource>> pDstBuffer,
				UINT64 DstOffset,
				gsl::not_null<WrapperView<ID3D12Resource>> pSrcBuffer,
				UINT64 SrcOffset,
				UINT Dependencies,
				Span<const WrapperView<ID3D12Resource>> ppDependentResources,
				const D3D12_SUBRESOURCE_RANGE_UINT64* pDependentSubresourceRanges) requires D3D12::DisableFunction<Tag, BundleTag, RenderPassTag>
			{
				Self().AtomicCopyBufferUINT(
					pDstBuffer.get().Get(),
					DstOffset,
					pSrcBuffer.get().Get(),
					SrcOffset,
					Dependencies,
					ppDependentResources.data(),
					pDependentSubresourceRanges);
			}

			void AtomicCopyBufferUINT64(
				gsl::not_null<WrapperView<ID3D12Resource>> pDstBuffer,
				UINT64 DstOffset,
				gsl::not_null<WrapperView<ID3D12Resource>> pSrcBuffer,
				UINT64 SrcOffset,
				UINT Dependencies,
				Span<const WrapperView<ID3D12Resource>> ppDependentResources,
				const D3D12_SUBRESOURCE_RANGE_UINT64* pDependentSubresourceRanges) requires D3D12::DisableFunction<Tag, BundleTag, RenderPassTag>
			{
				Self().AtomicCopyBufferUINT64(
					pDstBuffer.get().Get(),
					DstOffset,
					pSrcBuffer.get().Get(),
					SrcOffset,
					Dependencies,
					ppDependentResources.data(),
					pDependentSubresourceRanges);
			}

			void OMSetDepthBounds(
				FLOAT Min,
				FLOAT Max) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().OMSetDepthBounds(Min, Max);
			}

			void ResolveSubresourceRegion(
				gsl::not_null<WrapperView<ID3D12Resource>> pDstResource,
				UINT DstSubresource,
				UINT DstX,
				UINT DstY,
				gsl::not_null<WrapperView<ID3D12Resource>> pSrcResource,
				UINT SrcSubresource,
				D3D12_RECT* pSrcRect,
				DXGI_FORMAT Format,
				D3D12_RESOLVE_MODE ResolveMode) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().ResolveSubresourceRegion(
					pDstResource.get().Get(),
					DstSubresource,
					DstX,
					DstY,
					pSrcResource.get().Get(),
					SrcSubresource,
					pSrcRect,
					Format,
					ResolveMode
				);
			}

			void SetSamplePositions(
				UINT NumSamplesPerPixel,
				UINT NumPixels,
				D3D12_SAMPLE_POSITION* pSamplePositions) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().SetSamplePositions(
					NumSamplesPerPixel,
					NumPixels,
					pSamplePositions);
			}

			void SetViewInstanceMask(
				UINT Mask) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().SetViewInstanceMask(Mask);
			}
		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};

	template<template<class> class Tag>
		requires D3D12::CommandListEnabledTag<Tag>
	struct Trait<Tag<ID3D12GraphicsCommandList2>>
	{
		using inner_type = ID3D12GraphicsCommandList2;

		using inner_tag = ID3D12GraphicsCommandList2;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Tag<ID3D12GraphicsCommandList1>, Derived>
		{
			void WriteBufferImmediate(D3D12::WriteBufferImmediateData data)
			{
				Self().WriteBufferImmediate(data.GetCount(), data.GetParameters(), data.GetModes());
			}

			void WriteBufferImmediate(D3D12_WRITEBUFFERIMMEDIATE_PARAMETER parameters, std::optional<D3D12_WRITEBUFFERIMMEDIATE_MODE> modes)
			{
				Self().WriteBufferImmediate({ 1, &parameters, modes ? std::optional{ std::span{1, &modes.value()} } : std::optional{ std::nullopt } });
			}
		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};

	template<template<class> class Tag>
		requires D3D12::CommandListEnabledTag<Tag>
	struct Trait<Tag<ID3D12GraphicsCommandList3>>
	{
		using inner_type = ID3D12GraphicsCommandList3;

		using inner_tag = ID3D12GraphicsCommandList3;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Tag<ID3D12GraphicsCommandList2>, Derived>
		{
			void SetProtectedResourceSession(
				WrapperView<ID3D12ProtectedResourceSession> pProtectedResourceSession)
			{
				Self().SetProtectedResourceSession(pProtectedResourceSession.Get());
			}
		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};

	template<template<class> class Tag>
		requires D3D12::CommandListEnabledTag<Tag>
	struct Trait<Tag<ID3D12GraphicsCommandList4>>
	{
		using inner_type = ID3D12GraphicsCommandList4;

		using inner_tag = ID3D12GraphicsCommandList4;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Tag<ID3D12GraphicsCommandList3>, Derived>
		{
			template<template<class> class Wrapper = RenderPass>
				requires std::same_as<Wrapper<ID3D12GraphicsCommandList4>, RenderPass<ID3D12GraphicsCommandList4>>
			Wrapper<ID3D12GraphicsCommandList4> BeginRenderPass(
				std::span<const D3D12_RENDER_PASS_RENDER_TARGET_DESC> renderTargets,
				const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* pDepthStencil,
				D3D12_RENDER_PASS_FLAGS Flags) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().BeginRenderPass(renderTargets.size(), renderTargets.data(), pDepthStencil, Flags);
				return { &Self() };
			}

			void BuildRaytracingAccelerationStructure(
				const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& pDesc,
				std::span<const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC> pPostbuildInfoDescs) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().BuildRaytracingAccelerationStructure(
					&pDesc,
					static_cast<UINT>(pPostbuildInfoDescs.size()),
					pPostbuildInfoDescs.data());
			}

			void CopyRaytracingAccelerationStructure(
				D3D12_GPU_VIRTUAL_ADDRESS DestAccelerationStructureData,
				D3D12_GPU_VIRTUAL_ADDRESS SourceAccelerationStructureData,
				D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE Mode) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().CopyRaytracingAccelerationStructure(
					DestAccelerationStructureData,
					SourceAccelerationStructureData,
					Mode);
			}

			void DispatchRays(
				const D3D12_DISPATCH_RAYS_DESC& pDesc) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag>
			{
				Self().DispatchRays(&pDesc);
			}

			void EmitRaytracingAccelerationStructurePostbuildInfo(
				const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC& pDesc,
				std::span<const D3D12_GPU_VIRTUAL_ADDRESS> pSourceAccelerationStructureData) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().EmitRaytracingAccelerationStructurePostbuildInfo(
					&pDesc,
					static_cast<UINT>(pSourceAccelerationStructureData.size()),
					pSourceAccelerationStructureData.data());
			}

			void EndRenderPass() requires D3D12::DisableFunction<Tag, RenderPassTag>
			{
				Self().EndRenderPass();
			}

			void InitializeMetaCommand(
				gsl::not_null<WrapperView<ID3D12MetaCommand>> pMetaCommand,
				const void* pInitializationParametersData,
				SIZE_T InitializationParametersDataSizeInBytes) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().InitializeMetaCommand(pMetaCommand.get().Get(), pInitializationParametersData, InitializationParametersDataSizeInBytes);
			}

			void InitializeMetaCommand(
				gsl::not_null<WrapperView<ID3D12MetaCommand>> pMetaCommand) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				InitializeMetaCommand(pMetaCommand, nullptr, 0);
			}

			template<class T>
			void InitializeMetaCommand(
				gsl::not_null<WrapperView<ID3D12MetaCommand>> pMetaCommand,
				const T& pInitializationParametersData) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				InitializeMetaCommand(pMetaCommand, &pInitializationParametersData, sizeof(T));
			}

			void ExecuteMetaCommand(
				gsl::not_null<WrapperView<ID3D12MetaCommand>> pMetaCommand,
				const void* pExecutionParametersData,
				SIZE_T ExecutionParametersDataSizeInBytes) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				Self().ExecuteMetaCommand(pMetaCommand.get().Get(), pExecutionParametersData, ExecutionParametersDataSizeInBytes);
			}

			void ExecuteMetaCommand(
				gsl::not_null<WrapperView<ID3D12MetaCommand>> pMetaCommand) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				ExecuteMetaCommand(pMetaCommand, nullptr, 0);
			}

			template<class T>
			void ExecuteMetaCommand(
				gsl::not_null<WrapperView<ID3D12MetaCommand>> pMetaCommand,
				const T& pExecutionParametersData) requires D3D12::DisableFunction<Tag, CopyTag, BundleTag, RenderPassTag>
			{
				ExecuteMetaCommand(pMetaCommand, &pExecutionParametersData, sizeof(T));
			}

			void SetPipelineState1(
				WrapperView<ID3D12StateObject> pStateObject) requires D3D12::DisableFunction<Tag, CopyTag, RenderPassTag>
			{
				Self().SetPipelineState1(pStateObject.Get());
			}
		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};

	template<template<class> class Tag>
		requires D3D12::CommandListEnabledTag<Tag>
	struct Trait<Tag<ID3D12GraphicsCommandList5>>
	{
		using inner_type = ID3D12GraphicsCommandList5;

		using inner_tag = ID3D12GraphicsCommandList5;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Tag<ID3D12GraphicsCommandList4>, Derived>
		{
			void RSSetShadingRate(
				D3D12_SHADING_RATE baseShadingRate,
				const D3D12_SHADING_RATE_COMBINER* combiners) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().RSSetShadingRate(
					baseShadingRate,
					combiners);
			}

			void RSSetShadingRateImage(
				WrapperView<ID3D12Resource> shadingRateImage) requires D3D12::DisableFunction<Tag, ComputeTag, CopyTag>
			{
				Self().RSSetShadingRateImage(
					shadingRateImage.Get());
			}

		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};

	template<template<class> class Tag>
		requires D3D12::CommandListEnabledTag<Tag>
	struct Trait<Tag<ID3D12GraphicsCommandList6>>
	{
		using inner_type = ID3D12GraphicsCommandList6;

		using inner_tag = ID3D12GraphicsCommandList6;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Tag<ID3D12GraphicsCommandList5>, Derived>
		{
			//At the time of writing, the document doesn't display the restriction, but I'm guessing based on the fact
			//that Dispatch related functions can't be called on Copy and Render Pass command lists
			void DispatchMesh(
				UINT ThreadGroupCountX,
				UINT ThreadGroupCountY,
				UINT ThreadGroupCountZ) requires D3D12::DisableFunction<Tag, CopyTag, RenderPassTag>
			{
				Self().DispatchMesh(
					ThreadGroupCountX,
					ThreadGroupCountY,
					ThreadGroupCountZ);
			}

		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};

	template<template<class> class Tag>
		requires D3D12::CommandListEnabledTag<Tag>
	struct Trait<Tag<ID3D12GraphicsCommandList7>>
	{
		using inner_type = ID3D12GraphicsCommandList7;

		using inner_tag = ID3D12GraphicsCommandList7;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		struct Interface : TraitInterface<Tag<ID3D12GraphicsCommandList6>, Derived>
		{
			//At the time of writing, the document doesn't display the restriction, but I'm guessing based on the fact
			//that ResourceBarrier can't be called on Bundles
			void Barrier(
				std::span<const D3D12_BARRIER_GROUP> pBarrierGroups) requires D3D12::DisableFunction<Tag, BundleTag>
			{
				Self().Barrier(static_cast<UINT32>(pBarrierGroups.size()), pBarrierGroups.data());
			}

			//At the time of writing, the document doesn't display the restriction, but I'm guessing based on the fact
			//that ResourceBarrier can't be called on Bundles
			template<std::size_t Size>
			void Barrier(
				std::array<const D3D12_BARRIER_GROUP, Size> pBarrierGroups) requires D3D12::DisableFunction<Tag, BundleTag>
			{
				Barrier(std::span{ pBarrierGroups });
			}

		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};
}
