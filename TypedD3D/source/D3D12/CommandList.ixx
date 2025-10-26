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
//import :DeviceChild;
import TypedD3D.Shared;
import :Wrappers;

namespace TypedD3D::D3D12
{
	template<std::derived_from<ID3D12CommandList> Ty, D3D12_COMMAND_LIST_TYPE Type>
	struct CommandListTrait;

	template<D3D12_COMMAND_LIST_TYPE Type>
	struct CommandListTrait<ID3D12CommandList, Type>
	{
		template<class Derived>
		struct Interface : InterfaceBase<CommandListTypeToTrait<Type, Derived>>
		{
			D3D12_COMMAND_LIST_TYPE GetType() { return Self().GetType(); }
		private:
			using InterfaceBase<CommandListTypeToTrait<Type, Derived>>::Self;
			using InterfaceBase<CommandListTypeToTrait<Type, Derived>>::ToDerived;
		};
	};

	template<D3D12_COMMAND_LIST_TYPE Type>
	struct CommandListTrait<ID3D12GraphicsCommandList, Type>
	{
		template<class Derived>
		struct Interface : InterfaceBase<CommandListTypeToTrait<Type, Derived>>
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
				UINT Index)
			{
				Self().BeginQuery(pQueryHeap.get().Get(), Type, Index);
			}

			void ClearDepthStencilView(
				DSV<D3D12_CPU_DESCRIPTOR_HANDLE> DepthStencilView,
				D3D12_CLEAR_FLAGS ClearFlags,
				FLOAT Depth,
				UINT8 Stencil,
				std::span<const D3D12_RECT> rects)
			{
				Self().ClearDepthStencilView(DepthStencilView.Raw(), ClearFlags, Depth, Stencil, static_cast<UINT>(rects.size()), rects.data());
			}

			void ClearRenderTargetView(
				RTV<D3D12_CPU_DESCRIPTOR_HANDLE> RenderTargetView,
				std::span<const float, 4> colorRGBA,
				std::span<const D3D12_RECT> rects)
			{
				Self().ClearRenderTargetView(RenderTargetView.Raw(), colorRGBA.data(), static_cast<UINT>(rects.size()), rects.data());
			}

			void ClearRenderTargetView(
				RTV<D3D12_CPU_DESCRIPTOR_HANDLE> RenderTargetView,
				const std::array<float, 4>& colorRGBA)
			{
				ClearRenderTargetView(RenderTargetView, colorRGBA, {});
			}

			void ClearState(GraphicsView<ID3D12PipelineState> pPipelineState)
			{
				Self().ClearState(pPipelineState.Get());
			}

			void ClearState(ComputeView<ID3D12PipelineState> pPipelineState)
			{
				Self().ClearState(pPipelineState.Get());
			}

			void ClearUnorderedAccessViewFloat(
				CBV_SRV_UAV<::D3D12_GPU_DESCRIPTOR_HANDLE> ViewGPUHandleInCurrentHeap,
				CBV_SRV_UAV<::D3D12_CPU_DESCRIPTOR_HANDLE> ViewCPUHandle,
				gsl::not_null<WrapperView<ID3D12Resource>> pResource,
				std::span<const float, 4> values,
				std::span<const D3D12_RECT> rects)
			{
				Self().ClearUnorderedAccessViewFloat(ViewGPUHandleInCurrentHeap, ViewCPUHandle, pResource.get().Get(), values.data(), static_cast<UINT>(rects.size()), rects.data());
			}

			void ClearUnorderedAccessViewUint(
				CBV_SRV_UAV<::D3D12_GPU_DESCRIPTOR_HANDLE> ViewGPUHandleInCurrentHeap,
				CBV_SRV_UAV<::D3D12_CPU_DESCRIPTOR_HANDLE> ViewCPUHandle,
				gsl::not_null<WrapperView<ID3D12Resource>> pResource,
				std::span<const UINT, 4> values,
				std::span<const D3D12_RECT> rects)
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
				UINT64 NumBytes)
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
				gsl::not_null<WrapperView<ID3D12Resource>> pSrcResource)
			{
				Self().CopyResource(pDstResource.get().Get(), &pSrcResource.get().Get());
			}

			void CopyTextureRegion(
				const D3D12_TEXTURE_COPY_LOCATION& pDst,
				UINT DstX,
				UINT DstY,
				UINT DstZ,
				const D3D12_TEXTURE_COPY_LOCATION& pSrc,
				const D3D12_BOX* pOptSrcBox)
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
				D3D12_TILE_COPY_FLAGS Flags)
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
				const D3D12_DISCARD_REGION* pOptRegion)
			{
				Self().DiscardResource(pResource.get().Get(), pOptRegion);
			}

			void Dispatch(
				UINT ThreadGroupCountX,
				UINT ThreadGroupCountY,
				UINT ThreadGroupCountZ)
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
				UINT StartInstanceLocation)
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
				UINT StartInstanceLocation)
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
				UINT Index)
			{
				Self().EndQuery(pQueryHeap.get().Get(), Type, Index);
			}

			void ExecuteBundle(gsl::not_null<WeakWrapper<CommandListTypeToTrait<Type, ID3D12GraphicsCommandList>>> commandList)
			{
				Self().ExecuteBundle(commandList.get().Get());
			}

			void ExecuteIndirect(
				gsl::not_null<WrapperView<ID3D12CommandSignature>> commandSignature,
				UINT maxCommandCount,
				gsl::not_null<WrapperView<ID3D12Resource>> argumentBuffer,
				UINT64 argumentBufferOffset,
				WrapperView<ID3D12Resource> optCountBuffer = nullptr,
				UINT64 optCountBufferOffset = 0)
			{
				Self().ExecuteIndirect(commandSignature.get().Get(), maxCommandCount, argumentBuffer.get().Get(), argumentBufferOffset, optCountBuffer.Get(), optCountBufferOffset);
			}

			void IASetIndexBuffer(
				const D3D12_INDEX_BUFFER_VIEW* pView)
			{
				Self().IASetIndexBuffer(pView);
			}
			
			void IASetPrimitiveTopology(
				D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology)
			{
				Self().IASetPrimitiveTopology(PrimitiveTopology);
			}
			
			void IASetVertexBuffers(
				UINT StartSlot,
				std::span<const D3D12_VERTEX_BUFFER_VIEW> views)
			{
				Self().IASetVertexBuffers(StartSlot, static_cast<UINT>(views.size()), views.data());
			}
			
			void OMSetBlendFactor(std::span<const float, 4> BlendFactor)
			{
				Self().OMSetBlendFactor(BlendFactor.data());
			}
			
			//void OMSetRenderTargets(
			//	Span<const RTV<::D3D12_CPU_DESCRIPTOR_HANDLE>> pRenderTargetDescriptors,
			//	BOOL RTsSingleHandleToDescriptorRange,
			//	const DSV<::D3D12_CPU_DESCRIPTOR_HANDLE>* pDepthStencilDescriptor)
			//{
			//	const ::D3D12_CPU_DESCRIPTOR_HANDLE* depthStencil = (pDepthStencilDescriptor) ? &pDepthStencilDescriptor->Raw() : nullptr;
			//
			//	Self().OMSetRenderTargets(static_cast<UINT>(pRenderTargetDescriptors.size()), pRenderTargetDescriptors.data(), RTsSingleHandleToDescriptorRange, depthStencil);
			//}
			 
			void OMSetRenderTargets(
				std::span<const RTV<::D3D12_CPU_DESCRIPTOR_HANDLE>> pRenderTargetDescriptors,
				BOOL RTsSingleHandleToDescriptorRange,
				const DSV<::D3D12_CPU_DESCRIPTOR_HANDLE>* pDepthStencilDescriptor)
			{
				std::unique_ptr<::D3D12_CPU_DESCRIPTOR_HANDLE[]> renderTargets = std::make_unique<::D3D12_CPU_DESCRIPTOR_HANDLE[]>(pRenderTargetDescriptors.size());

				for(size_t i = 0; i < pRenderTargetDescriptors.size(); i++)
				{
					renderTargets[i] = pRenderTargetDescriptors[i].Raw();
				}

				const ::D3D12_CPU_DESCRIPTOR_HANDLE* depthStencil = (pDepthStencilDescriptor) ? &pDepthStencilDescriptor->Raw() : nullptr;

				Self().OMSetRenderTargets(static_cast<UINT>(pRenderTargetDescriptors.size()), renderTargets.get(), RTsSingleHandleToDescriptorRange, depthStencil);
			}
			
			void OMSetStencilRef(UINT StencilRef)
			{
				Self().OMSetStencilRef(StencilRef);
			}

			void RSSetScissorRects(
				std::span<D3D12_RECT> rects)
			{
				Self().RSSetScissorRects(static_cast<UINT>(rects.size()), rects.data());
			}

			void RSSetViewports(
				std::span<D3D12_VIEWPORT> viewports)
			{
				Self().RSSetViewports(static_cast<UINT>(viewports.size()), viewports.data());
			}

			HRESULT Reset(gsl::not_null<WeakWrapper<CommandListTypeToTrait<Type, ID3D12CommandAllocator>>> allocator,
				ID3D12PipelineState* optPipelineState)
			{
				return Self().Reset(allocator.get().Get(), optPipelineState);
			}
			
			void ResolveQueryData(
				gsl::not_null<WrapperView<ID3D12QueryHeap>> pQueryHeap,
				D3D12_QUERY_TYPE Type,
				UINT StartIndex,
				UINT NumQueries,
				ID3D12Resource& pDestinationBuffer,
				UINT64 AlignedDestinationBufferOffset)
			{
				Self().ResolveQueryData(pQueryHeap.get().Get(), Type, StartIndex, NumQueries, &pDestinationBuffer, AlignedDestinationBufferOffset);
			}
			
			void ResolveSubresource(
				gsl::not_null<WrapperView<ID3D12Resource>> pDstResource,
				UINT DstSubresource,
				gsl::not_null<WrapperView<ID3D12Resource>> pSrcResource,
				UINT SrcSubresource,
				DXGI_FORMAT Format)
			{
				Self().ResolveSubresource(
					pDstResource.get().Get(),
					DstSubresource,
					pSrcResource.get().Get(),
					SrcSubresource,
					Format);
			}
			
			void ResourceBarrier(std::span<const D3D12_RESOURCE_BARRIER> Barriers)
			{
				Self().ResourceBarrier(static_cast<UINT>(Barriers.size()), Barriers.data());
			}
			
			void SOSetTargets(
				UINT StartSlot,
				std::span<const D3D12_STREAM_OUTPUT_BUFFER_VIEW> views)
			{
				Self().SOSetTargets(StartSlot, static_cast<UINT>(views.size()), views.data());
			}
			
			void SetComputeRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
			{
				Self().SetComputeRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
			}
			
			void SetComputeRoot32BitConstants(
				UINT RootParameterIndex,
				UINT Num32BitValuesToSet,
				const void* pSrcData,
				UINT DestOffsetIn32BitValues)
			{
				Self().SetComputeRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
			}
			
			void SetComputeRootConstantBufferView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
			{
				Self().SetComputeRootConstantBufferView(RootParameterIndex, BufferLocation);
			}
			
			void SetComputeRootDescriptorTable(UINT RootParameterIndex, ::D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
			{
				Self().SetComputeRootDescriptorTable(RootParameterIndex, BaseDescriptor);
			}
			
			void SetComputeRootShaderResourceView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
			{
				Self().SetComputeRootShaderResourceView(RootParameterIndex, BufferLocation);
			}

			void SetComputeRootSignature(ID3D12RootSignature* pRootSignature)
			{
				Self().SetComputeRootSignature(pRootSignature);
			}
			
			void SetComputeRootUnorderedAccessView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
			{
				Self().SetComputeRootUnorderedAccessView(RootParameterIndex, BufferLocation);
			}
			
			void SetDescriptorHeaps(ShaderVisible<CBV_SRV_UAV<ID3D12DescriptorHeap>> descriptorHeap)
			{
				ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
				Self().SetDescriptorHeaps(1, heaps);
			}
			
			void SetDescriptorHeaps(ShaderVisible<Sampler<ID3D12DescriptorHeap>> descriptorHeap)
			{
				ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
				Self().SetDescriptorHeaps(1, heaps);
			}
			
			void SetDescriptorHeaps(ShaderVisible<CBV_SRV_UAV<ID3D12DescriptorHeap>> cbv_srv_uavHeap, ShaderVisible<Sampler<ID3D12DescriptorHeap>> samplerHeap)
			{
				ID3D12DescriptorHeap* heaps[] = { cbv_srv_uavHeap.Get(), samplerHeap.Get() };
				Self().SetDescriptorHeaps(2, heaps);
			}
			
			void SetGraphicsRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
			{
				Self().SetGraphicsRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
			}
			
			void SetGraphicsRoot32BitConstants(
				UINT RootParameterIndex,
				UINT Num32BitValuesToSet,
				const void* pSrcData,
				UINT DestOffsetIn32BitValues)
			{
				Self().SetGraphicsRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
			}
			
			void SetGraphicsRootConstantBufferView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
			{
				Self().SetGraphicsRootConstantBufferView(RootParameterIndex, BufferLocation);
			}
			
			void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, ::D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
			{
				Self().SetGraphicsRootDescriptorTable(RootParameterIndex, BaseDescriptor);
			}
			
			void SetGraphicsRootShaderResourceView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
			{
				Self().SetGraphicsRootShaderResourceView(RootParameterIndex, BufferLocation);
			}
			
			void SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature)
			{
				Self().SetGraphicsRootSignature(pRootSignature);
			}
			
			void SetGraphicsRootUnorderedAccessView(
				UINT RootParameterIndex,
				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
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
			
			void SetPipelineState(GraphicsView<ID3D12PipelineState> pPipelineState)
			{
				Self().SetPipelineState(pPipelineState.Get());
			}
			
			void SetPipelineState(ComputeView<ID3D12PipelineState> pPipelineState)
			{
				Self().SetPipelineState(pPipelineState.Get());
			}
			
			void SetPredication(
				WrapperView<ID3D12Resource> pBuffer,
				UINT64 AlignedBufferOffset,
				D3D12_PREDICATION_OP Operation)
			{
				Self().SetPredication(pBuffer.Get(), AlignedBufferOffset, Operation);
			}
		private:
			using InterfaceBase<CommandListTypeToTrait<Type, Derived>>::Self;
			using InterfaceBase<CommandListTypeToTrait<Type, Derived>>::ToDerived;
		};
	};
};

namespace TypedD3D
{
	template<std::derived_from<ID3D12CommandList> Ty>
	struct DirectTraits<Ty>
	{
		template<class Derived>
		using Interface = Ty*;
	};

	template<std::derived_from<ID3D12CommandList> Ty>
	struct ComputeTraits<Ty>
	{
		template<class Derived>
		using Interface = Ty*;
	};

	template<std::derived_from<ID3D12CommandList> Ty>
	struct CopyTraits<Ty>
	{
		template<class Derived>
		using Interface = Ty*;
	};

	template<std::derived_from<ID3D12CommandList> Ty>
	struct BundleTraits<Ty>
	{
		template<class Derived>
		using Interface = Ty*;
	};

	template<std::derived_from<ID3D12CommandList> Ty>
	struct RenderPassTraits<Ty>
	{
		template<class Derived>
		using Interface = Ty*;
	};

	template<>
	struct DirectTraits<ID3D12GraphicsCommandList>
	{
		template<class Derived>
		class Interface : public D3D12::CommandListTrait<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_DIRECT>::Interface<Derived>
		{
		};
	};

	template<>
	struct ComputeTraits<ID3D12GraphicsCommandList>
	{
		template<class Derived>
		class Interface : public D3D12::CommandListTrait<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_COMPUTE>::Interface<Derived>
		{
		};
	};

	template<>
	struct CopyTraits<ID3D12GraphicsCommandList>
	{
		template<class Derived>
		class Interface : public D3D12::CommandListTrait<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_COPY>::Interface<Derived>
		{
		};
	};

	template<>
	struct BundleTraits<ID3D12GraphicsCommandList>
	{
		template<class Derived>
		class Interface : public D3D12::CommandListTrait<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_BUNDLE>::Interface<Derived>
		{
		};
	};
}


//namespace TypedD3D::D3D12
//{
//	//Used for meta-programming internally
//	template<std::derived_from<ID3D12CommandList> Ty, template<class> class Trait>
//	using GenericCommandList = IUnknownWrapper<Ty, Trait>;
//
//	//Used for public APIs
//	export template<std::derived_from<ID3D12CommandList> Ty, D3D12_COMMAND_LIST_TYPE Type>
//	using CommandList_t = GenericCommandList<Ty, CommandListTypeToTrait<Type>::template type>;
//	template<std::derived_from<ID3D12CommandList> Ty>
//	using RenderPass_t = GenericCommandList<Ty, RenderPassTraits>;
//	namespace Aliases
//	{
//		template<class ListTy>
//		using Direct_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_DIRECT>;
//
//		export using D3D12DirectCommandListBase = Direct_t<ID3D12CommandList>;
//		export using D3D12DirectCommandList = Direct_t<ID3D12GraphicsCommandList>;
//		export using D3D12DirectCommandList1 = Direct_t<ID3D12GraphicsCommandList1>;
//		export using D3D12DirectCommandList2 = Direct_t<ID3D12GraphicsCommandList2>;
//		export using D3D12DirectCommandList3 = Direct_t<ID3D12GraphicsCommandList3>;
//		export using D3D12DirectCommandList4 = Direct_t<ID3D12GraphicsCommandList4>;
//		export using D3D12DirectCommandList5 = Direct_t<ID3D12GraphicsCommandList5>;
//		export using D3D12DirectCommandList6 = Direct_t<ID3D12GraphicsCommandList6>;
//		export using D3D12DirectCommandList7 = Direct_t<ID3D12GraphicsCommandList7>;
//
//		template<class ListTy>
//		using Bundle_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
//
//		export using D3D12BundleCommandListBase = Bundle_t<ID3D12CommandList>;
//		export using D3D12BundleCommandList = Bundle_t<ID3D12GraphicsCommandList>;
//		export using D3D12BundleCommandList1 = Bundle_t<ID3D12GraphicsCommandList1>;
//		export using D3D12BundleCommandList2 = Bundle_t<ID3D12GraphicsCommandList2>;
//		export using D3D12BundleCommandList3 = Bundle_t<ID3D12GraphicsCommandList3>;
//		export using D3D12BundleCommandList4 = Bundle_t<ID3D12GraphicsCommandList4>;
//		export using D3D12BundleCommandList5 = Bundle_t<ID3D12GraphicsCommandList5>;
//		export using D3D12BundleCommandList6 = Bundle_t<ID3D12GraphicsCommandList6>;
//		export using D3D12BundleCommandList7 = Bundle_t<ID3D12GraphicsCommandList7>;
//
//		template<class ListTy>
//		using Compute_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
//
//		export using D3D12ComputeCommandListBase = Compute_t<ID3D12CommandList>;
//		export using D3D12ComputeCommandList = Compute_t<ID3D12GraphicsCommandList>;
//		export using D3D12ComputeCommandList1 = Compute_t<ID3D12GraphicsCommandList1>;
//		export using D3D12ComputeCommandList2 = Compute_t<ID3D12GraphicsCommandList2>;
//		export using D3D12ComputeCommandList3 = Compute_t<ID3D12GraphicsCommandList3>;
//		export using D3D12ComputeCommandList4 = Compute_t<ID3D12GraphicsCommandList4>;
//		export using D3D12ComputeCommandList5 = Compute_t<ID3D12GraphicsCommandList5>;
//		export using D3D12ComputeCommandList6 = Compute_t<ID3D12GraphicsCommandList6>;
//		export using D3D12ComputeCommandList7 = Compute_t<ID3D12GraphicsCommandList7>;
//
//		export template<class ListTy>
//		using Copy_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_COPY>;
//
//		export using D3D12CopyCommandListBase = Copy_t<ID3D12CommandList>;
//		export using D3D12CopyCommandList = Copy_t<ID3D12GraphicsCommandList>;
//		export using D3D12CopyCommandList1 = Copy_t<ID3D12GraphicsCommandList1>;
//		export using D3D12CopyCommandList2 = Copy_t<ID3D12GraphicsCommandList2>;
//		export using D3D12CopyCommandList3 = Copy_t<ID3D12GraphicsCommandList3>;
//		export using D3D12CopyCommandList4 = Copy_t<ID3D12GraphicsCommandList4>;
//		export using D3D12CopyCommandList5 = Copy_t<ID3D12GraphicsCommandList5>;
//		export using D3D12CopyCommandList6 = Copy_t<ID3D12GraphicsCommandList6>;
//		export using D3D12CopyCommandList7 = Copy_t<ID3D12GraphicsCommandList7>;
//
//
//		export using RenderPass4 = RenderPass_t<ID3D12GraphicsCommandList4>;
//		export using RenderPass5 = RenderPass_t<ID3D12GraphicsCommandList5>;
//		export using RenderPass6 = RenderPass_t<ID3D12GraphicsCommandList6>;
//		export using RenderPass7 = RenderPass_t<ID3D12GraphicsCommandList7>;
//	}
//
//#pragma region Untagged Traits
//	template<class Ty>
//	struct CommandListTraits;
//
//	template<>
//	struct CommandListTraits<ID3D12CommandList>
//	{
//		using value_type = ID3D12CommandList;
//		using pointer = ID3D12CommandList*;
//		using const_pointer = const ID3D12CommandList*;
//		using reference = ID3D12CommandList&;
//		using const_reference = const ID3D12CommandList&;
//
//		template<class DerivedSelf>
//		class Interface : public DeviceChildTraits::Interface<DerivedSelf>
//		{
//		private:
//			using derived_self = DerivedSelf;
//
//		public:
//			D3D12_COMMAND_LIST_TYPE GetType() { return Self().GetType(); }
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//
//		template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
//		class InterfaceTagged : public Interface<DerivedSelf>
//		{
//		private:
//			static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
//			using derived_self = DerivedSelf;
//			using allocator_value_type = CommandAllocator_t<Type>;
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//	};
//
//	template<>
//	struct CommandListTraits<ID3D12GraphicsCommandList>
//	{
//		using base_value_type = ID3D12CommandList;
//		using value_type = ID3D12GraphicsCommandList;
//		using pointer = ID3D12GraphicsCommandList*;
//		using const_pointer = const ID3D12GraphicsCommandList*;
//		using reference = ID3D12GraphicsCommandList&;
//		using const_reference = const ID3D12GraphicsCommandList&;
//
//		template<class DerivedSelf>
//		class Interface
//		{
//		private:
//			using derived_self = DerivedSelf;
//
//		public:
//
//			void BeginEvent(
//				UINT Metadata,
//				const void* pData,
//				UINT Size)
//			{
//				Self().BeginEvent(Metadata, pData, Size);
//			}
//
//			void BeginQuery(
//				ID3D12QueryHeap& pQueryHeap,
//				D3D12_QUERY_TYPE Type,
//				UINT Index)
//			{
//				Self().BeginQuery(&pQueryHeap, Type, Index);
//			}
//
//			void ClearDepthStencilView(
//				DSV<::D3D12_CPU_DESCRIPTOR_HANDLE> DepthStencilView,
//				D3D12_CLEAR_FLAGS ClearFlags,
//				FLOAT Depth,
//				UINT8 Stencil,
//				std::span<const D3D12_RECT> rects)
//			{
//				Self().ClearDepthStencilView(DepthStencilView.Get(), ClearFlags, Depth, Stencil, static_cast<UINT>(rects.size()), rects.data());
//			}
//
//			void ClearRenderTargetView(
//				RTV<::D3D12_CPU_DESCRIPTOR_HANDLE> RenderTargetView,
//				std::span<const float, 4> colorRGBA,
//				std::span<const D3D12_RECT> rects)
//			{
//				Self().ClearRenderTargetView(RenderTargetView.Get(), colorRGBA.data(), static_cast<UINT>(rects.size()), rects.data());
//			}
//
//			void ClearRenderTargetView(
//				RTV<::D3D12_CPU_DESCRIPTOR_HANDLE> RenderTargetView,
//				std::array<const float, 4> colorRGBA)
//			{
//				ClearRenderTargetView(RenderTargetView.Get(), std::span<const float, 4>{ colorRGBA }, std::span<const D3D12_RECT>{});
//			}
//
//			void ClearState(ID3D12PipelineState* pPipelineState)
//			{
//				Self().ClearState(pPipelineState);
//			}
//
//			void ClearUnorderedAccessViewFloat(
//				CBV_SRV_UAV<::D3D12_GPU_DESCRIPTOR_HANDLE> ViewGPUHandleInCurrentHeap,
//				CBV_SRV_UAV<::D3D12_CPU_DESCRIPTOR_HANDLE> ViewCPUHandle,
//				ID3D12Resource& pResource,
//				std::span<const float, 4> values,
//				std::span<const D3D12_RECT> rects)
//			{
//				Self().ClearUnorderedAccessViewFloat(ViewGPUHandleInCurrentHeap, ViewCPUHandle, &pResource, values.data(), static_cast<UINT>(rects.size()), rects.data());
//			}
//
//			void ClearUnorderedAccessViewUint(
//				CBV_SRV_UAV<::D3D12_GPU_DESCRIPTOR_HANDLE> ViewGPUHandleInCurrentHeap,
//				CBV_SRV_UAV<::D3D12_CPU_DESCRIPTOR_HANDLE> ViewCPUHandle,
//				ID3D12Resource& pResource,
//				std::span<const UINT, 4> values,
//				std::span<const D3D12_RECT> rects)
//			{
//				Self().ClearUnorderedAccessViewUint(ViewGPUHandleInCurrentHeap, ViewCPUHandle, &pResource, values.data(), static_cast<UINT>(rects.size()), rects.data());
//			}
//
//			HRESULT Close()
//			{
//				return Self().Close();
//			}
//
//			void CopyBufferRegion(
//				ID3D12Resource& pDstBuffer,
//				UINT64 DstOffset,
//				ID3D12Resource& pSrcBuffer,
//				UINT64 SrcOffset,
//				UINT64 NumBytes)
//			{
//				Self().CopyBufferRegion(
//					&pDstBuffer,
//					DstOffset,
//					&pSrcBuffer,
//					SrcOffset,
//					NumBytes);
//			}
//
//			void CopyResource(
//				ID3D12Resource& pDstResource,
//				ID3D12Resource& pSrcResource)
//			{
//				Self().CopyResource(&pDstResource, &pSrcResource);
//			}
//
//			void CopyTextureRegion(
//				const D3D12_TEXTURE_COPY_LOCATION& pDst,
//				UINT DstX,
//				UINT DstY,
//				UINT DstZ,
//				const D3D12_TEXTURE_COPY_LOCATION& pSrc,
//				const D3D12_BOX* pSrcBox)
//			{
//				Self().CopyTextureRegion(
//					&pDst,
//					DstX,
//					DstY,
//					DstZ,
//					&pSrc,
//					pSrcBox);
//			}
//
//			void CopyTiles(
//				ID3D12Resource& pTiledResource,
//				const D3D12_TILED_RESOURCE_COORDINATE& pTileRegionStartCoordinate,
//				const D3D12_TILE_REGION_SIZE& pTileRegionSize,
//				ID3D12Resource& pBuffer,
//				UINT64 BufferStartOffsetInBytes,
//				D3D12_TILE_COPY_FLAGS Flags)
//			{
//				Self().CopyTiles(
//					&pTiledResource,
//					&pTileRegionStartCoordinate,
//					&pTileRegionSize,
//					&pBuffer,
//					BufferStartOffsetInBytes,
//					Flags);
//			}
//
//			void DiscardResource(
//				ID3D12Resource& pResource,
//				const D3D12_DISCARD_REGION* pRegion)
//			{
//				Self().DiscardResource(&pResource, pRegion);
//			}
//
//			void Dispatch(
//				UINT ThreadGroupCountX,
//				UINT ThreadGroupCountY,
//				UINT ThreadGroupCountZ)
//			{
//				Self().Dispatch(
//					ThreadGroupCountX,
//					ThreadGroupCountY,
//					ThreadGroupCountZ);
//			}
//
//			void DrawIndexedInstanced(
//				UINT IndexCountPerInstance,
//				UINT InstanceCount,
//				UINT StartIndexLocation,
//				INT BaseVertexLocation,
//				UINT StartInstanceLocation)
//			{
//				Self().DrawIndexedInstanced(
//					IndexCountPerInstance,
//					InstanceCount,
//					StartIndexLocation,
//					BaseVertexLocation,
//					StartInstanceLocation);
//			}
//
//			void DrawInstanced(
//				UINT VertexCountPerInstance,
//				UINT InstanceCount,
//				UINT StartVertexLocation,
//				UINT StartInstanceLocation)
//			{
//				Self().DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
//			}
//
//			void EndEvent()
//			{
//				Self().EndEvent();
//			}
//
//			void EndQuery(
//				ID3D12QueryHeap& pQueryHeap,
//				D3D12_QUERY_TYPE Type,
//				UINT Index)
//			{
//				Self().EndQuery(&pQueryHeap, Type, Index);
//			}
//
//			void ExecuteBundle(CommandList_t<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_BUNDLE> pCommandList);
//
//			void ExecuteIndirect(
//				ID3D12CommandSignature& commandSignature,
//				UINT maxCommandCount,
//				ID3D12Resource& argumentBuffer,
//				UINT64 argumentBufferOffset,
//				ID3D12Resource* optCountBuffer = nullptr,
//				UINT64 optCountBufferOffset = 0)
//			{
//				Self().ExecuteIndirect(&commandSignature, maxCommandCount, &argumentBuffer, argumentBufferOffset, optCountBuffer, optCountBufferOffset);
//			}
//
//			void IASetIndexBuffer(
//				const D3D12_INDEX_BUFFER_VIEW* pView)
//			{
//				Self().IASetIndexBuffer(pView);
//			}
//
//			void IASetPrimitiveTopology(
//				D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology)
//			{
//				Self().IASetPrimitiveTopology(PrimitiveTopology);
//			}
//
//			void IASetVertexBuffers(
//				UINT StartSlot,
//				std::span<const D3D12_VERTEX_BUFFER_VIEW> views)
//			{
//				Self().IASetVertexBuffers(StartSlot, static_cast<UINT>(views.size()), views.data());
//			}
//
//			void OMSetBlendFactor(std::span<const float, 4> BlendFactor)
//			{
//				Self().OMSetBlendFactor(BlendFactor.data());
//			}
//
//			void OMSetRenderTargets(
//				std::span<const RTV<::D3D12_CPU_DESCRIPTOR_HANDLE>> pRenderTargetDescriptors,
//				BOOL RTsSingleHandleToDescriptorRange,
//				const DSV<::D3D12_CPU_DESCRIPTOR_HANDLE>* pDepthStencilDescriptor)
//			{
//				std::unique_ptr<::D3D12_CPU_DESCRIPTOR_HANDLE[]> renderTargets = std::make_unique<::D3D12_CPU_DESCRIPTOR_HANDLE[]>(pRenderTargetDescriptors.size());
//
//				for(size_t i = 0; i < pRenderTargetDescriptors.size(); i++)
//				{
//					renderTargets[i] = pRenderTargetDescriptors[i].Get();
//				}
//
//				const ::D3D12_CPU_DESCRIPTOR_HANDLE* depthStencil = (pDepthStencilDescriptor) ? &pDepthStencilDescriptor->Get() : nullptr;
//
//				Self().OMSetRenderTargets(static_cast<UINT>(pRenderTargetDescriptors.size()), renderTargets.get(), RTsSingleHandleToDescriptorRange, depthStencil);
//			}
//
//			void OMSetStencilRef(UINT StencilRef)
//			{
//				Self().OMSetStencilRef(StencilRef);
//			}
//
//			void ResolveQueryData(
//				ID3D12QueryHeap& pQueryHeap,
//				D3D12_QUERY_TYPE Type,
//				UINT StartIndex,
//				UINT NumQueries,
//				ID3D12Resource& pDestinationBuffer,
//				UINT64 AlignedDestinationBufferOffset)
//			{
//				Self().ResolveQueryData(&pQueryHeap, Type, StartIndex, NumQueries, &pDestinationBuffer, AlignedDestinationBufferOffset);
//			}
//
//			void ResolveSubresource(
//				ID3D12Resource& pDstResource,
//				UINT DstSubresource,
//				ID3D12Resource& pSrcResource,
//				UINT SrcSubresource,
//				DXGI_FORMAT Format)
//			{
//				Self().ResolveSubresource(
//					&pDstResource,
//					DstSubresource,
//					&pSrcResource,
//					SrcSubresource,
//					Format);
//			}
//
//			void ResourceBarrier(std::span<const D3D12_RESOURCE_BARRIER> Barriers)
//			{
//				Self().ResourceBarrier(static_cast<UINT>(Barriers.size()), Barriers.data());
//			}
//
//			void RSSetScissorRects(
//				std::span<D3D12_RECT> rects)
//			{
//				Self().RSSetScissorRects(static_cast<UINT>(rects.size()), rects.data());
//			}
//
//			void RSSetViewports(
//				std::span<D3D12_VIEWPORT> viewports)
//			{
//				Self().RSSetViewports(static_cast<UINT>(viewports.size()), viewports.data());
//			}
//
//			void SetComputeRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
//			{
//				Self().SetComputeRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
//			}
//
//			void SetComputeRoot32BitConstants(
//				UINT RootParameterIndex,
//				UINT Num32BitValuesToSet,
//				const void* pSrcData,
//				UINT DestOffsetIn32BitValues)
//			{
//				Self().SetComputeRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
//			}
//
//			void SetComputeRootConstantBufferView(
//				UINT RootParameterIndex,
//				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
//			{
//				Self().SetComputeRootConstantBufferView(RootParameterIndex, BufferLocation);
//			}
//
//			void SetComputeRootDescriptorTable(UINT RootParameterIndex, ::D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
//			{
//				Self().SetComputeRootDescriptorTable(RootParameterIndex, BaseDescriptor);
//			}
//
//			void SetComputeRootShaderResourceView(
//				UINT RootParameterIndex,
//				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
//			{
//				Self().SetComputeRootShaderResourceView(RootParameterIndex, BufferLocation);
//			}
//
//			void SetComputeRootSignature(ID3D12RootSignature* pRootSignature)
//			{
//				Self().SetComputeRootSignature(pRootSignature);
//			}
//
//			void SetComputeRootUnorderedAccessView(
//				UINT RootParameterIndex,
//				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
//			{
//				Self().SetComputeRootUnorderedAccessView(RootParameterIndex, BufferLocation);
//			}
//
//			void SetDescriptorHeaps(ShaderVisible<CBV_SRV_UAV<ID3D12DescriptorHeap>> descriptorHeap)
//			{
//				ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
//				Self().SetDescriptorHeaps(1, heaps);
//			}
//
//			void SetDescriptorHeaps(ShaderVisible<Sampler<ID3D12DescriptorHeap>> descriptorHeap)
//			{
//				ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
//				Self().SetDescriptorHeaps(1, heaps);
//			}
//
//			void SetDescriptorHeaps(ShaderVisible<CBV_SRV_UAV<ID3D12DescriptorHeap>> cbv_srv_uavHeap, ShaderVisible<Sampler<ID3D12DescriptorHeap>> samplerHeap)
//			{
//				ID3D12DescriptorHeap* heaps[] = { cbv_srv_uavHeap.Get(), samplerHeap.Get() };
//				Self().SetDescriptorHeaps(2, heaps);
//			}
//
//			void SetGraphicsRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
//			{
//				Self().SetGraphicsRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
//			}
//
//			void SetGraphicsRoot32BitConstants(
//				UINT RootParameterIndex,
//				UINT Num32BitValuesToSet,
//				const void* pSrcData,
//				UINT DestOffsetIn32BitValues)
//			{
//				Self().SetGraphicsRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
//			}
//
//			void SetGraphicsRootConstantBufferView(
//				UINT RootParameterIndex,
//				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
//			{
//				Self().SetGraphicsRootConstantBufferView(RootParameterIndex, BufferLocation);
//			}
//
//			void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, ::D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
//			{
//				Self().SetGraphicsRootDescriptorTable(RootParameterIndex, BaseDescriptor);
//			}
//
//			void SetGraphicsRootShaderResourceView(
//				UINT RootParameterIndex,
//				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
//			{
//				Self().SetGraphicsRootShaderResourceView(RootParameterIndex, BufferLocation);
//			}
//
//			void SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature)
//			{
//				Self().SetGraphicsRootSignature(pRootSignature);
//			}
//
//			void SetGraphicsRootUnorderedAccessView(
//				UINT RootParameterIndex,
//				D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
//			{
//				Self().SetGraphicsRootUnorderedAccessView(RootParameterIndex, BufferLocation);
//			}
//
//			void SetMarker(
//				UINT Metadata,
//				const void* pData,
//				UINT Size)
//			{
//				Self().SetMarker(Metadata, pData, Size);
//			}
//
//			void SetPipelineState(ID3D12PipelineState* pPipelineState)
//			{
//				Self().SetPipelineState(pPipelineState);
//			}
//
//			void SetPredication(
//				ID3D12Resource* pBuffer,
//				UINT64 AlignedBufferOffset,
//				D3D12_PREDICATION_OP Operation)
//			{
//				Self().SetPredication(pBuffer, AlignedBufferOffset, Operation);
//			}
//
//			void SOSetTargets(
//				UINT StartSlot,
//				std::span<const D3D12_STREAM_OUTPUT_BUFFER_VIEW> views)
//			{
//				Self().SOSetTargets(StartSlot, static_cast<UINT>(views.size()), views.data());
//			}
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//
//		template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
//		class InterfaceTagged : public CommandListTraits<base_value_type>::InterfaceTagged<DerivedSelf, Type>,
//			public Interface<DerivedSelf>
//		{
//		private:
//			static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
//			using derived_self = DerivedSelf;
//			using allocator_value_type = CommandAllocator_t<Type>;
//
//		public:
//			HRESULT Reset(
//				allocator_value_type pAllocator,
//				ID3D12PipelineState* pInitialState)
//			{
//				return Self().Reset(pAllocator.Get(), pInitialState);
//			}
//
//		  ////Alphabetical order
//		  //using Base::BeginEvent;
//		  //using Base::BeginQuery;
//		  //using Base::ClearDepthStencilView;
//		  //using Base::ClearRenderTargetView;
//		  //using Base::ClearState;
//		  //using Base::ClearUnorderedAccessViewFloat;
//		  //using Base::ClearUnorderedAccessViewUint;
//		  //using Base::Close;
//		  //using Base::CopyBufferRegion;
//		  //using Base::CopyResource;
//		  //using Base::CopyTextureRegion;
//		  //using Base::CopyTiles;
//		  //using Base::DiscardResource;
//		  //using Base::Dispatch;
//		  //using Base::DrawIndexedInstanced;
//		  //using Base::DrawInstanced;
//		  //using Base::EndEvent;
//		  //using Base::EndQuery;
//		  //using Base::ExecuteBundle;
//		  //using Base::ExecuteIndirect;
//		  //using Base::IASetIndexBuffer;
//		  //using Base::IASetPrimitiveTopology;
//		  //using Base::IASetVertexBuffers;
//		  //using Base::OMSetBlendFactor;
//		  //using Base::OMSetRenderTargets;
//		  //using Base::OMSetStencilRef;
//		  //using Base::Reset;
//		  //using Base::ResolveQueryData;
//		  //using Base::ResolveSubresource;
//		  //using Base::ResourceBarrier;
//		  //using Base::RSSetScissorRects;
//		  //using Base::RSSetViewports;
//		  //using Base::SetComputeRoot32BitConstant;
//		  //using Base::SetComputeRoot32BitConstants;
//		  //using Base::SetComputeRootConstantBufferView;
//		  //using Base::SetComputeRootDescriptorTable;
//		  //using Base::SetComputeRootShaderResourceView;
//		  //using Base::SetComputeRootSignature;
//		  //using Base::SetComputeRootUnorderedAccessView;
//		  //using Base::SetDescriptorHeaps;
//		  //using Base::SetGraphicsRoot32BitConstant;
//		  //using Base::SetGraphicsRoot32BitConstants;
//		  //using Base::SetGraphicsRootConstantBufferView;
//		  //using Base::SetGraphicsRootDescriptorTable;
//		  //using Base::SetGraphicsRootShaderResourceView;
//		  //using Base::SetGraphicsRootSignature;
//		  //using Base::SetGraphicsRootUnorderedAccessView;
//		  //using Base::SetMarker;
//		  //using Base::SetPipelineState;
//		  //using Base::SetPredication;
//		  //using Base::SOSetTargets;
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//	};
//
//	template<>
//	struct CommandListTraits<ID3D12GraphicsCommandList1>
//	{
//		using base_value_type = ID3D12GraphicsCommandList;
//		using value_type = ID3D12GraphicsCommandList1;
//		using pointer = ID3D12GraphicsCommandList1*;
//		using const_pointer = const ID3D12GraphicsCommandList1*;
//		using reference = ID3D12GraphicsCommandList1&;
//		using const_reference = const ID3D12GraphicsCommandList1&;
//
//		template<class DerivedSelf>
//		class Interface
//		{
//		private:
//			using derived_self = DerivedSelf;
//
//		public:
//			void AtomicCopyBufferUINT(
//				ID3D12Resource& pDstBuffer,
//				UINT64 DstOffset,
//				ID3D12Resource& pSrcBuffer,
//				UINT64 SrcOffset,
//				UINT Dependencies,
//				ID3D12Resource* const* ppDependentResources,
//				const D3D12_SUBRESOURCE_RANGE_UINT64* pDependentSubresourceRanges)
//			{
//				Self().AtomicCopyBufferUINT(
//					&pDstBuffer,
//					DstOffset,
//					&pSrcBuffer,
//					SrcOffset,
//					Dependencies,
//					ppDependentResources,
//					pDependentSubresourceRanges);
//			}
//
//			void AtomicCopyBufferUINT64(
//				ID3D12Resource& pDstBuffer,
//				UINT64 DstOffset,
//				ID3D12Resource& pSrcBuffer,
//				UINT64 SrcOffset,
//				UINT Dependencies,
//				ID3D12Resource* const* ppDependentResources,
//				const D3D12_SUBRESOURCE_RANGE_UINT64* pDependentSubresourceRanges)
//			{
//				Self().AtomicCopyBufferUINT64(
//					&pDstBuffer,
//					DstOffset,
//					&pSrcBuffer,
//					SrcOffset,
//					Dependencies,
//					ppDependentResources,
//					pDependentSubresourceRanges);
//			}
//
//			void OMSetDepthBounds(
//				FLOAT Min,
//				FLOAT Max)
//			{
//				Self().OMSetDepthBounds(Min, Max);
//			}
//
//			void ResolveSubresourceRegion(
//				ID3D12Resource& pDstResource,
//				UINT DstSubresource,
//				UINT DstX,
//				UINT DstY,
//				ID3D12Resource& pSrcResource,
//				UINT SrcSubresource,
//				D3D12_RECT* pSrcRect,
//				DXGI_FORMAT Format,
//				D3D12_RESOLVE_MODE ResolveMode)
//			{
//				Self().ResolveSubresourceRegion(
//					&pDstResource,
//					DstSubresource,
//					DstX,
//					DstY,
//					&pSrcResource,
//					SrcSubresource,
//					pSrcRect,
//					Format,
//					ResolveMode
//				);
//			}
//
//			void SetSamplePositions(
//				UINT NumSamplesPerPixel,
//				UINT NumPixels,
//				D3D12_SAMPLE_POSITION* pSamplePositions)
//			{
//				Self().SetSamplePositions(
//					NumSamplesPerPixel,
//					NumPixels,
//					pSamplePositions);
//			}
//
//			void SetViewInstanceMask(
//				UINT Mask)
//			{
//				Self().SetViewInstanceMask(Mask);
//			}
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//
//		template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
//		class InterfaceTagged : public CommandListTraits<base_value_type>::InterfaceTagged<DerivedSelf, Type>,
//			public Interface<DerivedSelf>
//		{
//		private:
//			static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
//			using derived_self = DerivedSelf;
//			using allocator_value_type = CommandAllocator_t<Type>;
//
//		public:
//			////Alphabetical order
//			//using Base::AtomicCopyBufferUINT;
//			//using Base::AtomicCopyBufferUINT64;
//			//using Base::OMSetDepthBounds;
//			//using Base::ResolveSubresourceRegion;
//			//using Base::SetSamplePositions;
//			//using Base::SetViewInstanceMask;     
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//	};
//
//	export class WriteBufferImmediateData
//	{
//		UINT count;
//		D3D12_WRITEBUFFERIMMEDIATE_PARAMETER* parameters;
//		D3D12_WRITEBUFFERIMMEDIATE_MODE* modes = nullptr;
//
//	public:
//		template<UINT Count>
//		WriteBufferImmediateData(std::span<D3D12_WRITEBUFFERIMMEDIATE_PARAMETER, Count> parameters, std::optional<std::span<D3D12_WRITEBUFFERIMMEDIATE_MODE, Count>> modes) :
//			count{ Count },
//			parameters{ parameters.data() },
//			modes{ modes.value_or({}).data() }
//		{
//
//		}
//
//		WriteBufferImmediateData(UINT count, D3D12_WRITEBUFFERIMMEDIATE_PARAMETER* parameters, D3D12_WRITEBUFFERIMMEDIATE_MODE* optionalModes) :
//			count{ count },
//			parameters{ parameters },
//			modes{ optionalModes }
//		{
//
//		}
//
//	public:
//		UINT GetCount() const noexcept { return count; }
//		const D3D12_WRITEBUFFERIMMEDIATE_PARAMETER* GetParameters() const noexcept { return parameters; }
//		const D3D12_WRITEBUFFERIMMEDIATE_MODE* GetModes() const noexcept { return modes; }
//	};
//
//	template<>
//	struct CommandListTraits<ID3D12GraphicsCommandList2>
//	{
//		using base_value_type = ID3D12GraphicsCommandList1;
//		using value_type = ID3D12GraphicsCommandList2;
//		using pointer = ID3D12GraphicsCommandList2*;
//		using const_pointer = const ID3D12GraphicsCommandList2*;
//		using reference = ID3D12GraphicsCommandList2&;
//		using const_reference = const ID3D12GraphicsCommandList2&;
//
//		template<class DerivedSelf>
//		class Interface
//		{
//		private:
//			using derived_self = DerivedSelf;
//
//		public:
//			void WriteBufferImmediate(WriteBufferImmediateData data)
//			{
//				Self().WriteBufferImmediate(data.GetCount(), data.GetParameters(), data.GetModes());
//			}
//
//			void WriteBufferImmediate(D3D12_WRITEBUFFERIMMEDIATE_PARAMETER parameters, std::optional<D3D12_WRITEBUFFERIMMEDIATE_MODE> modes)
//			{
//				Self().WriteBufferImmediate({ 1, &parameters, modes ? std::optional{ std::span{1, &modes.value()} } : std::optional{ std::nullopt } });
//			}
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//
//		template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
//		class InterfaceTagged : public CommandListTraits<base_value_type>::InterfaceTagged<DerivedSelf, Type>,
//			public Interface<DerivedSelf>
//		{
//		private:
//			static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
//			using derived_self = DerivedSelf;
//			using allocator_value_type = CommandAllocator_t<Type>;
//
//		public:
//			////Alphabetical order
//			//using Base::WriteBufferImmediate;
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//	};
//
//	template<>
//	struct CommandListTraits<ID3D12GraphicsCommandList3>
//	{
//		using base_value_type = ID3D12GraphicsCommandList2;
//		using value_type = ID3D12GraphicsCommandList3;
//		using pointer = ID3D12GraphicsCommandList3*;
//		using const_pointer = const ID3D12GraphicsCommandList3*;
//		using reference = ID3D12GraphicsCommandList3&;
//		using const_reference = const ID3D12GraphicsCommandList3&;
//
//		template<class DerivedSelf>
//		class Interface
//		{
//		private:
//			using derived_self = DerivedSelf;
//
//		public:
//			void SetProtectedResourceSession(
//				ID3D12ProtectedResourceSession* pProtectedResourceSession)
//			{
//				Self().SetProtectedResourceSession(pProtectedResourceSession);
//			}
//
//			//In code order                                                          //Alphabetical order
//			//using Base::SetProtectedResourceSession;                               using Base::SetProtectedResourceSession;
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//
//		template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
//		class InterfaceTagged : public CommandListTraits<base_value_type>::InterfaceTagged<DerivedSelf, Type>,
//			public Interface<DerivedSelf>
//		{
//		private:
//			static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
//			using derived_self = DerivedSelf;
//			using allocator_value_type = CommandAllocator_t<Type>;
//
//		public:
//			//In code order                                                          //Alphabetical order
//			//using Base::SetProtectedResourceSession;                               using Base::SetProtectedResourceSession;
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//	};
//
//	template<>
//	struct CommandListTraits<ID3D12GraphicsCommandList4>
//	{
//		using base_value_type = ID3D12GraphicsCommandList3;
//		using value_type = ID3D12GraphicsCommandList4;
//		using pointer = ID3D12GraphicsCommandList4*;
//		using const_pointer = const ID3D12GraphicsCommandList4*;
//		using reference = ID3D12GraphicsCommandList4&;
//		using const_reference = const ID3D12GraphicsCommandList4&;
//
//		template<class DerivedSelf>
//		class Interface
//		{
//		private:
//			using derived_self = DerivedSelf;
//
//		public:
//			RenderPass_t<ID3D12GraphicsCommandList4> BeginRenderPass(
//				std::span<const D3D12_RENDER_PASS_RENDER_TARGET_DESC> renderTargets,
//				const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* pDepthStencil,
//				D3D12_RENDER_PASS_FLAGS Flags);
//
//			void BuildRaytracingAccelerationStructure(
//				const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& pDesc,
//				std::span<const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC> pPostbuildInfoDescs)
//			{
//				Self().BuildRaytracingAccelerationStructure(
//					&pDesc,
//					static_cast<UINT>(pPostbuildInfoDescs.size()),
//					pPostbuildInfoDescs.data());
//			}
//
//			void CopyRaytracingAccelerationStructure(
//				D3D12_GPU_VIRTUAL_ADDRESS DestAccelerationStructureData,
//				D3D12_GPU_VIRTUAL_ADDRESS SourceAccelerationStructureData,
//				D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE Mode)
//			{
//				Self().CopyRaytracingAccelerationStructure(
//					DestAccelerationStructureData,
//					SourceAccelerationStructureData,
//					Mode);
//			}
//
//			void DispatchRays(
//				const D3D12_DISPATCH_RAYS_DESC& pDesc)
//			{
//				Self().DispatchRays(&pDesc);
//			}
//
//			void EmitRaytracingAccelerationStructurePostbuildInfo(
//				const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC& pDesc,
//				std::span<const D3D12_GPU_VIRTUAL_ADDRESS> pSourceAccelerationStructureData)
//			{
//				Self().EmitRaytracingAccelerationStructurePostbuildInfo(
//					&pDesc,
//					static_cast<UINT>(pSourceAccelerationStructureData.size()),
//					pSourceAccelerationStructureData.data());
//			}
//
//			void EndRenderPass()
//			{
//				Self().EndRenderPass();
//			}
//
//			void InitializeMetaCommand(
//				ID3D12MetaCommand& pMetaCommand,
//				const void* pInitializationParametersData,
//				SIZE_T InitializationParametersDataSizeInBytes)
//			{
//				Self().InitializeMetaCommand(&pMetaCommand, pInitializationParametersData, InitializationParametersDataSizeInBytes);
//			}
//
//			void ExecuteMetaCommand(
//				ID3D12MetaCommand& pMetaCommand,
//				const void* pExecutionParametersData,
//				SIZE_T ExecutionParametersDataSizeInBytes)
//			{
//				Self().ExecuteMetaCommand(&pMetaCommand, pExecutionParametersData, ExecutionParametersDataSizeInBytes);
//			}
//
//			void ExecuteMetaCommand(
//				ID3D12MetaCommand& pMetaCommand)
//			{
//				ExecuteMetaCommand(pMetaCommand, nullptr, 0);
//			}
//
//			template<class T>
//			void ExecuteMetaCommand(
//				ID3D12MetaCommand& pMetaCommand,
//				const T& pExecutionParametersData)
//			{
//				ExecuteMetaCommand(pMetaCommand, &pExecutionParametersData, sizeof(T));
//			}
//
//			void InitializeMetaCommand(
//				ID3D12MetaCommand& pMetaCommand)
//			{
//				InitializeMetaCommand(pMetaCommand, nullptr, 0);
//			}
//
//			template<class T>
//			void InitializeMetaCommand(
//				ID3D12MetaCommand& pMetaCommand,
//				const T& pInitializationParametersData)
//			{
//				InitializeMetaCommand(pMetaCommand, &pInitializationParametersData, sizeof(T));
//			}
//
//			void SetPipelineState1(
//				ID3D12StateObject* pStateObject)
//			{
//				Self().SetPipelineState1(pStateObject);
//			}
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//
//		template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
//		class InterfaceTagged : public CommandListTraits<base_value_type>::InterfaceTagged<DerivedSelf, Type>,
//			public Interface<DerivedSelf>
//		{
//		private:
//			static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
//			using derived_self = DerivedSelf;
//			using allocator_value_type = CommandAllocator_t<Type>;
//
//		public:
//			////Alphabetical order
//			//using Base::BeginRenderPass;
//			//using Base::BuildRaytracingAccelerationStructure;
//			//using Base::CopyRaytracingAccelerationStructure;
//			//using Base::DispatchRays;
//			//using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
//			//using Base::EndRenderPass;
//			//using Base::ExecuteMetaCommand;
//			//using Base::InitializeMetaCommand;
//			//using Base::SetPipelineState1;
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//	};
//
//	template<>
//	struct CommandListTraits<ID3D12GraphicsCommandList5>
//	{
//		using base_value_type = ID3D12GraphicsCommandList4;
//		using value_type = ID3D12GraphicsCommandList5;
//		using pointer = ID3D12GraphicsCommandList5*;
//		using const_pointer = const ID3D12GraphicsCommandList5*;
//		using reference = ID3D12GraphicsCommandList5&;
//		using const_reference = const ID3D12GraphicsCommandList5&;
//
//		template<class DerivedSelf>
//		class Interface
//		{
//		private:
//			using derived_self = DerivedSelf;
//
//		public:
//			void RSSetShadingRate(
//				D3D12_SHADING_RATE baseShadingRate,
//				const D3D12_SHADING_RATE_COMBINER* combiners)
//			{
//				Self().RSSetShadingRate(
//					baseShadingRate,
//					combiners);
//			}
//
//			void RSSetShadingRateImage(
//				ID3D12Resource* shadingRateImage)
//			{
//				Self().RSSetShadingRateImage(
//					shadingRateImage);
//			}
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//
//		template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
//		class InterfaceTagged : public CommandListTraits<base_value_type>::InterfaceTagged<DerivedSelf, Type>,
//			public Interface<DerivedSelf>
//		{
//		private:
//			static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
//			using derived_self = DerivedSelf;
//			using allocator_value_type = CommandAllocator_t<Type>;
//
//		public:
//			////Alphabetical order
//			//using Base::RSSetShadingRate;
//			//using Base::RSSetShadingRateImage;
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//	};
//
//	template<>
//	struct CommandListTraits<ID3D12GraphicsCommandList6>
//	{
//		using base_value_type = ID3D12GraphicsCommandList5;
//		using value_type = ID3D12GraphicsCommandList6;
//		using pointer = ID3D12GraphicsCommandList6*;
//		using const_pointer = const ID3D12GraphicsCommandList6*;
//		using reference = ID3D12GraphicsCommandList6&;
//		using const_reference = const ID3D12GraphicsCommandList6&;
//
//		template<class DerivedSelf>
//		class Interface
//		{
//		private:
//			using derived_self = DerivedSelf;
//
//		public:
//			void DispatchMesh(
//				UINT ThreadGroupCountX,
//				UINT ThreadGroupCountY,
//				UINT ThreadGroupCountZ)
//			{
//				Self().DispatchMesh(
//					ThreadGroupCountX,
//					ThreadGroupCountY,
//					ThreadGroupCountZ);
//			}
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//
//		template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
//		class InterfaceTagged : public CommandListTraits<base_value_type>::InterfaceTagged<DerivedSelf, Type>,
//			public Interface<DerivedSelf>
//		{
//		private:
//			static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
//			using derived_self = DerivedSelf;
//			using allocator_value_type = CommandAllocator_t<Type>;
//
//		public:
//			////Alphabetical order
//			//using Base::DispatchMesh;
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//	};
//
//	template<>
//	struct CommandListTraits<ID3D12GraphicsCommandList7>
//	{
//		using base_value_type = ID3D12GraphicsCommandList6;
//		using value_type = ID3D12GraphicsCommandList7;
//		using pointer = ID3D12GraphicsCommandList7*;
//		using const_pointer = const ID3D12GraphicsCommandList7*;
//		using reference = ID3D12GraphicsCommandList7&;
//		using const_reference = const ID3D12GraphicsCommandList7&;
//
//		template<class DerivedSelf>
//		class Interface
//		{
//		private:
//			using derived_self = DerivedSelf;
//
//		public:
//			void Barrier(
//				std::span<const D3D12_BARRIER_GROUP> pBarrierGroups)
//			{
//				Self().Barrier(static_cast<UINT32>(pBarrierGroups.size()), pBarrierGroups.data());
//			}
//
//			template<std::size_t Size>
//			void Barrier(
//				std::array<const D3D12_BARRIER_GROUP, Size> pBarrierGroups)
//			{
//				Barrier(std::span{ pBarrierGroups });
//			}
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//
//		template<class DerivedSelf, D3D12_COMMAND_LIST_TYPE Type>
//		class InterfaceTagged : public CommandListTraits<base_value_type>::InterfaceTagged<DerivedSelf, Type>,
//			public Interface<DerivedSelf>
//		{
//		private:
//			static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
//			using derived_self = DerivedSelf;
//			using allocator_value_type = CommandAllocator_t<Type>;
//
//		public:
//			////Alphabetical order
//			//using Base::DispatchMesh;
//
//		private:
//			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
//			reference Get() { return *ToDerived().derived_self::Get(); }
//		};
//	};
//#pragma endregion
//
//#pragma region Direct Traits
//	template<>
//	struct DirectTraits<ID3D12CommandList> : CommandListTraits<ID3D12CommandList> {};
//
//	template<>	
//	struct DirectTraits<ID3D12GraphicsCommandList> 
//	{
//		using value_type = ID3D12GraphicsCommandList;
//		using pointer = ID3D12GraphicsCommandList*;
//		using const_pointer = const ID3D12GraphicsCommandList*;
//		using reference = ID3D12GraphicsCommandList&;
//		using const_reference = const ID3D12GraphicsCommandList&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//
//		};
//	};
//
//	template<>
//	struct DirectTraits<ID3D12GraphicsCommandList1>
//	{
//		using value_type = ID3D12GraphicsCommandList1;
//		using pointer = ID3D12GraphicsCommandList1*;
//		using const_pointer = const ID3D12GraphicsCommandList1*;
//		using reference = ID3D12GraphicsCommandList1&;
//		using const_reference = const ID3D12GraphicsCommandList1&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//
//		};
//	};
//
//	template<>
//	struct DirectTraits<ID3D12GraphicsCommandList2>
//	{
//		using value_type = ID3D12GraphicsCommandList2;
//		using pointer = ID3D12GraphicsCommandList2*;
//		using const_pointer = const ID3D12GraphicsCommandList2*;
//		using reference = ID3D12GraphicsCommandList2&;
//		using const_reference = const ID3D12GraphicsCommandList2&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//
//		};
//	};
//
//	template<>
//	struct DirectTraits<ID3D12GraphicsCommandList3>
//	{
//		using value_type = ID3D12GraphicsCommandList3;
//		using pointer = ID3D12GraphicsCommandList3*;
//		using const_pointer = const ID3D12GraphicsCommandList3*;
//		using reference = ID3D12GraphicsCommandList3&;
//		using const_reference = const ID3D12GraphicsCommandList3&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//
//		};
//	};
//
//	template<>
//	struct DirectTraits<ID3D12GraphicsCommandList4>
//	{
//		using value_type = ID3D12GraphicsCommandList4;
//		using pointer = ID3D12GraphicsCommandList4*;
//		using const_pointer = const ID3D12GraphicsCommandList4*;
//		using reference = ID3D12GraphicsCommandList4&;
//		using const_reference = const ID3D12GraphicsCommandList4&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//
//		};
//	};
//
//	template<>
//	struct DirectTraits<ID3D12GraphicsCommandList5>
//	{
//		using value_type = ID3D12GraphicsCommandList5;
//		using pointer = ID3D12GraphicsCommandList5*;
//		using const_pointer = const ID3D12GraphicsCommandList5*;
//		using reference = ID3D12GraphicsCommandList5&;
//		using const_reference = const ID3D12GraphicsCommandList5&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//
//		};
//	};
//
//	template<>
//	struct DirectTraits<ID3D12GraphicsCommandList6>
//	{
//		using value_type = ID3D12GraphicsCommandList6;
//		using pointer = ID3D12GraphicsCommandList6*;
//		using const_pointer = const ID3D12GraphicsCommandList6*;
//		using reference = ID3D12GraphicsCommandList6&;
//		using const_reference = const ID3D12GraphicsCommandList6&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//
//		};
//	};
//
//	template<>
//	struct DirectTraits<ID3D12GraphicsCommandList7>
//	{
//		using value_type = ID3D12GraphicsCommandList7;
//		using pointer = ID3D12GraphicsCommandList7*;
//		using const_pointer = const ID3D12GraphicsCommandList7*;
//		using reference = ID3D12GraphicsCommandList7&;
//		using const_reference = const ID3D12GraphicsCommandList7&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//
//		};
//	};
//#pragma endregion
//
//#pragma region Compute Traits
//	template<>
//	struct ComputeTraits<ID3D12CommandList> : CommandListTraits<ID3D12CommandList> {};
//
//	template<>
//	struct ComputeTraits<ID3D12GraphicsCommandList>
//	{
//		using value_type = ID3D12GraphicsCommandList;
//		using pointer = ID3D12GraphicsCommandList*;
//		using const_pointer = const ID3D12GraphicsCommandList*;
//		using reference = ID3D12GraphicsCommandList&;
//		using const_reference = const ID3D12GraphicsCommandList&;
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
//		private:
//			using Base::BeginEvent;
//			using Base::BeginQuery;
//			using Base::ClearDepthStencilView;
//			using Base::ClearRenderTargetView;
//			//using Base::ClearState;
//			//using Base::ClearUnorderedAccessViewFloat;
//			//using Base::ClearUnorderedAccessViewUint;
//			//using Base::Close;
//			//using Base::CopyBufferRegion;
//			//using Base::CopyResource;
//			//using Base::CopyTextureRegion;
//			//using Base::CopyTiles;
//			//using Base::DiscardResource;
//			//using Base::Dispatch;
//			using Base::DrawIndexedInstanced;
//			using Base::DrawInstanced;
//			using Base::EndEvent;
//			//using Base::EndQuery;
//			using Base::ExecuteBundle;
//			//using Base::ExecuteIndirect;
//			using Base::IASetIndexBuffer;
//			using Base::IASetPrimitiveTopology;
//			using Base::IASetVertexBuffers;
//			using Base::OMSetBlendFactor;
//			using Base::OMSetRenderTargets;
//			using Base::OMSetStencilRef;
//			//using Base::Reset;
//			//using Base::ResolveQueryData;
//			using Base::ResolveSubresource;
//			//using Base::ResourceBarrier;
//			//using Base::RSSetScissorRects;
//			//using Base::RSSetViewports;
//			//using Base::SetComputeRoot32BitConstant;
//			//using Base::SetComputeRoot32BitConstants;
//			//using Base::SetComputeRootConstantBufferView;
//			//using Base::SetComputeRootDescriptorTable;
//			//using Base::SetComputeRootShaderResourceView;
//			//using Base::SetComputeRootSignature;
//			//using Base::SetComputeRootUnorderedAccessView;
//			//using Base::SetDescriptorHeaps;
//			using Base::SetGraphicsRoot32BitConstant;
//			using Base::SetGraphicsRoot32BitConstants;
//			using Base::SetGraphicsRootConstantBufferView;
//			using Base::SetGraphicsRootDescriptorTable;
//			using Base::SetGraphicsRootShaderResourceView;
//			using Base::SetGraphicsRootSignature;
//			using Base::SetGraphicsRootUnorderedAccessView;
//			using Base::SetMarker;
//			//using Base::SetPipelineState;
//			//using Base::SetPredication;
//			using Base::SOSetTargets;
//		};
//	};
//
//	template<>
//	struct ComputeTraits<ID3D12GraphicsCommandList1>
//	{
//		using value_type = ID3D12GraphicsCommandList1;
//		using pointer = ID3D12GraphicsCommandList1*;
//		using const_pointer = const ID3D12GraphicsCommandList1*;
//		using reference = ID3D12GraphicsCommandList1&;
//		using const_reference = const ID3D12GraphicsCommandList1&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
//
//		private:
//			//using Base::AtomicCopyBufferUINT;
//			//using Base::AtomicCopyBufferUINT64;
//			using Base::OMSetDepthBounds;
//			using Base::ResolveSubresourceRegion;
//			using Base::SetSamplePositions;
//			using Base::SetViewInstanceMask;
//		};
//	};
//
//	template<>
//	struct ComputeTraits<ID3D12GraphicsCommandList2>
//	{
//		using value_type = ID3D12GraphicsCommandList2;
//		using pointer = ID3D12GraphicsCommandList2*;
//		using const_pointer = const ID3D12GraphicsCommandList2*;
//		using reference = ID3D12GraphicsCommandList2&;
//		using const_reference = const ID3D12GraphicsCommandList2&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
//
//		private:
//			//using Base::WriteBufferImmediate;
//		};
//	};
//
//	template<>
//	struct ComputeTraits<ID3D12GraphicsCommandList3>
//	{
//		using value_type = ID3D12GraphicsCommandList3;
//		using pointer = ID3D12GraphicsCommandList3*;
//		using const_pointer = const ID3D12GraphicsCommandList3*;
//		using reference = ID3D12GraphicsCommandList3&;
//		using const_reference = const ID3D12GraphicsCommandList3&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
//
//		private:
//			//using Base::SetProtectedResourceSession;
//		};
//	};
//
//	template<>
//	struct ComputeTraits<ID3D12GraphicsCommandList4>
//	{
//		using value_type = ID3D12GraphicsCommandList4;
//		using pointer = ID3D12GraphicsCommandList4*;
//		using const_pointer = const ID3D12GraphicsCommandList4*;
//		using reference = ID3D12GraphicsCommandList4&;
//		using const_reference = const ID3D12GraphicsCommandList4&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
//		private:
//			using Base::BeginRenderPass;
//			using Base::BuildRaytracingAccelerationStructure;
//			using Base::CopyRaytracingAccelerationStructure;
//			//using Base::DispatchRays;
//			using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
//			using Base::EndRenderPass;
//			using Base::ExecuteMetaCommand;
//			using Base::InitializeMetaCommand;
//			//using Base::SetPipelineState1;
//		};
//	};
//
//	template<>
//	struct ComputeTraits<ID3D12GraphicsCommandList5>
//	{
//		using value_type = ID3D12GraphicsCommandList5;
//		using pointer = ID3D12GraphicsCommandList5*;
//		using const_pointer = const ID3D12GraphicsCommandList5*;
//		using reference = ID3D12GraphicsCommandList5&;
//		using const_reference = const ID3D12GraphicsCommandList5&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
//		private:
//			using Base::RSSetShadingRate;
//			using Base::RSSetShadingRateImage;
//		};
//	};
//
//	template<>
//	struct ComputeTraits<ID3D12GraphicsCommandList6>
//	{
//		using value_type = ID3D12GraphicsCommandList6;
//		using pointer = ID3D12GraphicsCommandList6*;
//		using const_pointer = const ID3D12GraphicsCommandList6*;
//		using reference = ID3D12GraphicsCommandList6&;
//		using const_reference = const ID3D12GraphicsCommandList6&;
//
//		//TODO: Figure out what functions are restricted as at the time of writing, is not reflected in the linked API restriction document 
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
//		private:
//			//using Base::DispatchMesh;
//		};
//	};
//
//	template<>
//	struct ComputeTraits<ID3D12GraphicsCommandList7>
//	{
//		using value_type = ID3D12GraphicsCommandList7;
//		using pointer = ID3D12GraphicsCommandList7*;
//		using const_pointer = const ID3D12GraphicsCommandList7*;
//		using reference = ID3D12GraphicsCommandList7&;
//		using const_reference = const ID3D12GraphicsCommandList7&;
//
//		//TODO: Figure out what functions are restricted as at the time of writing, is not reflected in the linked API restriction document 
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COMPUTE>;
//		private:
//			//using Base::Barrier;
//		};
//	};
//#pragma endregion
//
//#pragma region Copy Traits	
//	template<>
//	struct CopyTraits<ID3D12CommandList> : CommandListTraits<ID3D12CommandList> {};
//
//	template<>
//	struct CopyTraits<ID3D12GraphicsCommandList>
//	{
//		using value_type = ID3D12GraphicsCommandList;
//		using pointer = ID3D12GraphicsCommandList*;
//		using const_pointer = const ID3D12GraphicsCommandList*;
//		using reference = ID3D12GraphicsCommandList&;
//		using const_reference = const ID3D12GraphicsCommandList&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;
//
//		private:
//			using Base::BeginEvent;
//			using Base::BeginQuery;
//			using Base::ClearDepthStencilView;
//			using Base::ClearRenderTargetView;
//			using Base::ClearState;
//			using Base::ClearUnorderedAccessViewFloat;
//			using Base::ClearUnorderedAccessViewUint;
//			//using Base::Close;
//			//using Base::CopyBufferRegion;
//			//using Base::CopyResource;
//			//using Base::CopyTextureRegion;
//			//using Base::CopyTiles;
//			//using Base::DiscardResource;
//			//using Base::Dispatch;
//			using Base::DrawIndexedInstanced;
//			using Base::DrawInstanced;
//			using Base::EndEvent;
//			//using Base::EndQuery;
//			using Base::ExecuteBundle;
//			using Base::ExecuteIndirect;
//			using Base::IASetIndexBuffer;
//			using Base::IASetPrimitiveTopology;
//			using Base::IASetVertexBuffers;
//			using Base::OMSetBlendFactor;
//			using Base::OMSetRenderTargets;
//			using Base::OMSetStencilRef;
//			//using Base::Reset;
//			//using Base::ResolveQueryData;
//			using Base::ResolveSubresource;
//			//using Base::ResourceBarrier;
//			using Base::RSSetScissorRects;
//			using Base::RSSetViewports;
//			using Base::SetComputeRoot32BitConstant;
//			using Base::SetComputeRoot32BitConstants;
//			using Base::SetComputeRootConstantBufferView;
//			using Base::SetComputeRootDescriptorTable;
//			using Base::SetComputeRootShaderResourceView;
//			using Base::SetComputeRootSignature;
//			using Base::SetComputeRootUnorderedAccessView;
//			using Base::SetDescriptorHeaps;
//			using Base::SetGraphicsRoot32BitConstant;
//			using Base::SetGraphicsRoot32BitConstants;
//			using Base::SetGraphicsRootConstantBufferView;
//			using Base::SetGraphicsRootDescriptorTable;
//			using Base::SetGraphicsRootShaderResourceView;
//			using Base::SetGraphicsRootSignature;
//			using Base::SetGraphicsRootUnorderedAccessView;
//			using Base::SetMarker;
//			using Base::SetPipelineState;
//			using Base::SetPredication;
//			using Base::SOSetTargets;
//		};
//	};
//
//	template<>
//	struct CopyTraits<ID3D12GraphicsCommandList1>
//	{
//		using value_type = ID3D12GraphicsCommandList1;
//		using pointer = ID3D12GraphicsCommandList1*;
//		using const_pointer = const ID3D12GraphicsCommandList1*;
//		using reference = ID3D12GraphicsCommandList1&;
//		using const_reference = const ID3D12GraphicsCommandList1&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;
//
//		private:
//			//using Base::AtomicCopyBufferUINT;
//			//using Base::AtomicCopyBufferUINT64;
//			using Base::OMSetDepthBounds;
//			using Base::ResolveSubresourceRegion;
//			using Base::SetSamplePositions;
//			using Base::SetViewInstanceMask;
//		};
//	};
//
//	template<>
//	struct CopyTraits<ID3D12GraphicsCommandList2>
//	{
//		using value_type = ID3D12GraphicsCommandList2;
//		using pointer = ID3D12GraphicsCommandList2*;
//		using const_pointer = const ID3D12GraphicsCommandList2*;
//		using reference = ID3D12GraphicsCommandList2&;
//		using const_reference = const ID3D12GraphicsCommandList2&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;
//
//		private:
//			//using Base::WriteBufferImmediate;
//		};
//	};
//
//	template<>
//	struct CopyTraits<ID3D12GraphicsCommandList3>
//	{
//		using value_type = ID3D12GraphicsCommandList3;
//		using pointer = ID3D12GraphicsCommandList3*;
//		using const_pointer = const ID3D12GraphicsCommandList3*;
//		using reference = ID3D12GraphicsCommandList3&;
//		using const_reference = const ID3D12GraphicsCommandList3&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;
//
//		private:
//			//using Base::SetProtectedResourceSession;
//		};
//	};
//
//	template<>
//	struct CopyTraits<ID3D12GraphicsCommandList4>
//	{
//		using value_type = ID3D12GraphicsCommandList4;
//		using pointer = ID3D12GraphicsCommandList4*;
//		using const_pointer = const ID3D12GraphicsCommandList4*;
//		using reference = ID3D12GraphicsCommandList4&;
//		using const_reference = const ID3D12GraphicsCommandList4&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;
//
//		private:
//			using Base::BeginRenderPass;
//			using Base::BuildRaytracingAccelerationStructure;
//			using Base::CopyRaytracingAccelerationStructure;
//			//using Base::DispatchRays;
//			using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
//			using Base::EndRenderPass;
//			using Base::ExecuteMetaCommand;
//			using Base::InitializeMetaCommand;
//			//using Base::SetPipelineState1;
//		};
//	};
//
//	template<>
//	struct CopyTraits<ID3D12GraphicsCommandList5>
//	{
//		using value_type = ID3D12GraphicsCommandList5;
//		using pointer = ID3D12GraphicsCommandList5*;
//		using const_pointer = const ID3D12GraphicsCommandList5*;
//		using reference = ID3D12GraphicsCommandList5&;
//		using const_reference = const ID3D12GraphicsCommandList5&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;
//		private:
//			using Base::RSSetShadingRate;
//			using Base::RSSetShadingRateImage;
//		};
//	};
//
//	//TODO: Figure out what functions are restricted as at the time of writing, is not reflected in the linked API restriction document 
//	template<>
//	struct CopyTraits<ID3D12GraphicsCommandList6>
//	{
//		using value_type = ID3D12GraphicsCommandList6;
//		using pointer = ID3D12GraphicsCommandList6*;
//		using const_pointer = const ID3D12GraphicsCommandList6*;
//		using reference = ID3D12GraphicsCommandList6&;
//		using const_reference = const ID3D12GraphicsCommandList6&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;
//		private:
//			//using Base::DispatchMesh;
//		};
//	};
//
//	//TODO: Figure out what functions are restricted as at the time of writing, is not reflected in the linked API restriction document 
//	template<>
//	struct CopyTraits<ID3D12GraphicsCommandList7>
//	{
//		using value_type = ID3D12GraphicsCommandList7;
//		using pointer = ID3D12GraphicsCommandList7*;
//		using const_pointer = const ID3D12GraphicsCommandList7*;
//		using reference = ID3D12GraphicsCommandList7&;
//		using const_reference = const ID3D12GraphicsCommandList7&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_COPY>;
//		private:
//			//using Base::Barrier;
//		};
//	};
//
//#pragma endregion
//
//#pragma region Bundle Traits
//	template<>
//	struct BundleTraits<ID3D12CommandList> : CommandListTraits<ID3D12CommandList> {};
//
//	template<>
//	struct BundleTraits<ID3D12GraphicsCommandList>
//	{
//		using value_type = ID3D12GraphicsCommandList;
//		using pointer = ID3D12GraphicsCommandList*;
//		using const_pointer = const ID3D12GraphicsCommandList*;
//		using reference = ID3D12GraphicsCommandList&;
//		using const_reference = const ID3D12GraphicsCommandList&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
//
//		private:
//			using Base::BeginEvent;
//			using Base::BeginQuery;
//			using Base::ClearDepthStencilView;
//			using Base::ClearRenderTargetView;
//			using Base::ClearState;
//			using Base::ClearUnorderedAccessViewFloat;
//			using Base::ClearUnorderedAccessViewUint;
//			//using Base::Close;
//			using Base::CopyBufferRegion;
//			using Base::CopyResource;
//			using Base::CopyTextureRegion;
//			using Base::CopyTiles;
//			using Base::DiscardResource;
//			//using Base::Dispatch;
//			//using Base::DrawIndexedInstanced;
//			//using Base::DrawInstanced;
//			using Base::EndEvent;
//			using Base::EndQuery;
//			using Base::ExecuteBundle;
//			//using Base::ExecuteIndirect;
//			//using Base::IASetIndexBuffer;
//			//using Base::IASetPrimitiveTopology;
//			//using Base::IASetVertexBuffers;
//			//using Base::OMSetBlendFactor;
//			using Base::OMSetRenderTargets;
//			//using Base::OMSetStencilRef;
//			//using Base::Reset;
//			using Base::ResolveQueryData;
//			using Base::ResolveSubresource;
//			using Base::ResourceBarrier;
//			using Base::RSSetScissorRects;
//			using Base::RSSetViewports;
//			//using Base::SetComputeRoot32BitConstant;
//			//using Base::SetComputeRoot32BitConstants;
//			//using Base::SetComputeRootConstantBufferView;
//			//using Base::SetComputeRootDescriptorTable;
//			//using Base::SetComputeRootShaderResourceView;
//			//using Base::SetComputeRootSignature;
//			//using Base::SetComputeRootUnorderedAccessView;
//			//using Base::SetDescriptorHeaps;
//			//using Base::SetGraphicsRoot32BitConstant;
//			//using Base::SetGraphicsRoot32BitConstants;
//			//using Base::SetGraphicsRootConstantBufferView;
//			//using Base::SetGraphicsRootDescriptorTable;
//			//using Base::SetGraphicsRootShaderResourceView;
//			//using Base::SetGraphicsRootSignature;
//			//using Base::SetGraphicsRootUnorderedAccessView;
//			//using Base::SetMarker;
//			//using Base::SetPipelineState;
//			using Base::SetPredication;
//			using Base::SOSetTargets;
//		};
//	};
//
//	template<>
//	struct BundleTraits<ID3D12GraphicsCommandList1>
//	{
//		using value_type = ID3D12GraphicsCommandList1;
//		using pointer = ID3D12GraphicsCommandList1*;
//		using const_pointer = const ID3D12GraphicsCommandList1*;
//		using reference = ID3D12GraphicsCommandList1&;
//		using const_reference = const ID3D12GraphicsCommandList1&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
//
//		private:
//			using Base::AtomicCopyBufferUINT;
//			using Base::AtomicCopyBufferUINT64;
//			//using Base::OMSetDepthBounds;
//			using Base::ResolveSubresourceRegion;
//			//using Base::SetSamplePositions;
//			//using Base::SetViewInstanceMask;
//		};
//	};
//
//	template<>
//	struct BundleTraits<ID3D12GraphicsCommandList2>
//	{
//		using value_type = ID3D12GraphicsCommandList2;
//		using pointer = ID3D12GraphicsCommandList2*;
//		using const_pointer = const ID3D12GraphicsCommandList2*;
//		using reference = ID3D12GraphicsCommandList2&;
//		using const_reference = const ID3D12GraphicsCommandList2&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
//
//		private:
//			//using Base::WriteBufferImmediate;
//		};
//	};
//
//	template<>
//	struct BundleTraits<ID3D12GraphicsCommandList3>
//	{
//		using value_type = ID3D12GraphicsCommandList3;
//		using pointer = ID3D12GraphicsCommandList3*;
//		using const_pointer = const ID3D12GraphicsCommandList3*;
//		using reference = ID3D12GraphicsCommandList3&;
//		using const_reference = const ID3D12GraphicsCommandList3&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
//
//		private:
//			using Base::SetProtectedResourceSession;
//		};
//	};
//
//	template<>
//	struct BundleTraits<ID3D12GraphicsCommandList4>
//	{
//		using value_type = ID3D12GraphicsCommandList4;
//		using pointer = ID3D12GraphicsCommandList4*;
//		using const_pointer = const ID3D12GraphicsCommandList4*;
//		using reference = ID3D12GraphicsCommandList4&;
//		using const_reference = const ID3D12GraphicsCommandList4&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
//
//		private:
//			using Base::BeginRenderPass;
//			//using Base::BuildRaytracingAccelerationStructure;
//			//using Base::CopyRaytracingAccelerationStructure;
//			//using Base::DispatchRays;
//			//using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
//			using Base::EndRenderPass;
//			//using Base::ExecuteMetaCommand;
//			//using Base::InitializeMetaCommand;
//			//using Base::SetPipelineState1;
//		};
//	};
//
//	template<>
//	struct BundleTraits<ID3D12GraphicsCommandList5>
//	{
//		using value_type = ID3D12GraphicsCommandList5;
//		using pointer = ID3D12GraphicsCommandList5*;
//		using const_pointer = const ID3D12GraphicsCommandList5*;
//		using reference = ID3D12GraphicsCommandList5&;
//		using const_reference = const ID3D12GraphicsCommandList5&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
//		private:
//			//using Base::RSSetShadingRate;
//			//using Base::RSSetShadingRateImage;
//		};
//	};
//
//	//TODO: Figure out what functions are restricted as at the time of writing, is not reflected in the linked API restriction document 
//	template<>
//	struct BundleTraits<ID3D12GraphicsCommandList6>
//	{
//		using value_type = ID3D12GraphicsCommandList6;
//		using pointer = ID3D12GraphicsCommandList6*;
//		using const_pointer = const ID3D12GraphicsCommandList6*;
//		using reference = ID3D12GraphicsCommandList6&;
//		using const_reference = const ID3D12GraphicsCommandList6&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
//		private:
//			//using Base::DispatchMesh;
//		};
//	};
//
//	//TODO: Figure out what functions are restricted as at the time of writing, is not reflected in the linked API restriction document 
//	template<>
//	struct BundleTraits<ID3D12GraphicsCommandList7>
//	{
//		using value_type = ID3D12GraphicsCommandList7;
//		using pointer = ID3D12GraphicsCommandList7*;
//		using const_pointer = const ID3D12GraphicsCommandList7*;
//		using reference = ID3D12GraphicsCommandList7&;
//		using const_reference = const ID3D12GraphicsCommandList7&;
//
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_BUNDLE>;
//		private:
//			//using Base::Barrier;
//		};
//	};
//#pragma endregion
//
//#pragma region RenderPass Traits
//	template<>
//	struct RenderPassTraits<ID3D12CommandList> : CommandListTraits<ID3D12CommandList> {};
//
//	template<>
//	struct RenderPassTraits<ID3D12GraphicsCommandList>
//	{
//		using value_type = ID3D12GraphicsCommandList;
//		using pointer = ID3D12GraphicsCommandList*;
//		using const_pointer = const ID3D12GraphicsCommandList*;
//		using reference = ID3D12GraphicsCommandList&;
//		using const_reference = const ID3D12GraphicsCommandList&;
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;
//
//		private:
//			using Base::BeginEvent;
//			//using Base::BeginQuery;
//			using Base::ClearDepthStencilView;
//			using Base::ClearRenderTargetView;
//			using Base::ClearState;
//			using Base::ClearUnorderedAccessViewFloat;
//			using Base::ClearUnorderedAccessViewUint;
//			//using Base::Close;
//			using Base::CopyBufferRegion;
//			using Base::CopyResource;
//			using Base::CopyTextureRegion;
//			using Base::CopyTiles;
//			using Base::DiscardResource;
//			using Base::Dispatch;
//			//using Base::DrawIndexedInstanced;
//			//using Base::DrawInstanced;
//			//using Base::EndEvent;
//			//using Base::EndQuery;
//			//using Base::ExecuteBundle;
//			//using Base::ExecuteIndirect;
//			//using Base::IASetIndexBuffer;
//			//using Base::IASetPrimitiveTopology;
//			//using Base::IASetVertexBuffers;
//			//using Base::OMSetBlendFactor;
//			using Base::OMSetRenderTargets;
//			//using Base::OMSetStencilRef;
//			//using Base::Reset;
//			using Base::ResolveQueryData;
//			using Base::ResolveSubresource;
//			//using Base::ResourceBarrier;
//			//using Base::RSSetScissorRects;
//			//using Base::RSSetViewports;
//			//using Base::SetComputeRoot32BitConstant;
//			//using Base::SetComputeRoot32BitConstants;
//			//using Base::SetComputeRootConstantBufferView;
//			//using Base::SetComputeRootDescriptorTable;
//			//using Base::SetComputeRootShaderResourceView;
//			//using Base::SetComputeRootSignature;
//			//using Base::SetComputeRootUnorderedAccessView;
//			//using Base::SetDescriptorHeaps;
//			//using Base::SetGraphicsRoot32BitConstant;
//			//using Base::SetGraphicsRoot32BitConstants;
//			//using Base::SetGraphicsRootConstantBufferView;
//			//using Base::SetGraphicsRootDescriptorTable;
//			//using Base::SetGraphicsRootShaderResourceView;
//			//using Base::SetGraphicsRootSignature;
//			//using Base::SetGraphicsRootUnorderedAccessView;
//			//using Base::SetMarker;
//			//using Base::SetPipelineState;
//			//using Base::SetPredication;
//			//using Base::SOSetTargets;
//		};
//	};
//
//	template<>
//	struct RenderPassTraits<ID3D12GraphicsCommandList1>
//	{
//		using value_type = ID3D12GraphicsCommandList1;
//		using pointer = ID3D12GraphicsCommandList1*;
//		using const_pointer = const ID3D12GraphicsCommandList1*;
//		using reference = ID3D12GraphicsCommandList1&;
//		using const_reference = const ID3D12GraphicsCommandList1&;
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;
//
//		private:
//			using Base::AtomicCopyBufferUINT;
//			using Base::AtomicCopyBufferUINT64;
//			//using Base::OMSetDepthBounds;
//			using Base::ResolveSubresourceRegion;
//			//using Base::SetSamplePositions;
//			//using Base::SetViewInstanceMask;
//		};
//	};
//
//	template<>
//	struct RenderPassTraits<ID3D12GraphicsCommandList2>
//	{
//		using value_type = ID3D12GraphicsCommandList2;
//		using pointer = ID3D12GraphicsCommandList2*;
//		using const_pointer = const ID3D12GraphicsCommandList2*;
//		using reference = ID3D12GraphicsCommandList2&;
//		using const_reference = const ID3D12GraphicsCommandList2&;
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;
//
//		private:
//			//using Base::WriteBufferImmediate;
//		};
//	};
//
//	template<>
//	struct RenderPassTraits<ID3D12GraphicsCommandList3>
//	{
//		using value_type = ID3D12GraphicsCommandList3;
//		using pointer = ID3D12GraphicsCommandList3*;
//		using const_pointer = const ID3D12GraphicsCommandList3*;
//		using reference = ID3D12GraphicsCommandList3&;
//		using const_reference = const ID3D12GraphicsCommandList3&;
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;
//		};
//	};
//
//	template<>
//	struct RenderPassTraits<ID3D12GraphicsCommandList4>
//	{
//		using value_type = ID3D12GraphicsCommandList4;
//		using pointer = ID3D12GraphicsCommandList4*;
//		using const_pointer = const ID3D12GraphicsCommandList4*;
//		using reference = ID3D12GraphicsCommandList4&;
//		using const_reference = const ID3D12GraphicsCommandList4&;
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;
//
//		private:
//			using Base::BeginRenderPass;
//			using Base::BuildRaytracingAccelerationStructure;
//			using Base::CopyRaytracingAccelerationStructure;
//			using Base::DispatchRays;
//			using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
//			//using Base::EndRenderPass;
//			using Base::ExecuteMetaCommand;
//			using Base::InitializeMetaCommand;
//			using Base::SetPipelineState1;
//		};
//	};
//
//	template<>
//	struct RenderPassTraits<ID3D12GraphicsCommandList5>
//	{
//		using value_type = ID3D12GraphicsCommandList5;
//		using pointer = ID3D12GraphicsCommandList5*;
//		using const_pointer = const ID3D12GraphicsCommandList5*;
//		using reference = ID3D12GraphicsCommandList5&;
//		using const_reference = const ID3D12GraphicsCommandList5&;
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;
//		private:
//			//using Base::RSSetShadingRate;
//			//using Base::RSSetShadingRateImage;
//		};
//	};
//
//	//TODO: Figure out what functions are restricted as at the time of writing, is not reflected in the linked API restriction document 
//	template<>
//	struct RenderPassTraits<ID3D12GraphicsCommandList6>
//	{
//		using value_type = ID3D12GraphicsCommandList6;
//		using pointer = ID3D12GraphicsCommandList6*;
//		using const_pointer = const ID3D12GraphicsCommandList6*;
//		using reference = ID3D12GraphicsCommandList6&;
//		using const_reference = const ID3D12GraphicsCommandList6&;
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;
//		private:
//			//using Base::DispatchMesh;
//		};
//	};
//
//	//TODO: Figure out what functions are restricted as at the time of writing, is not reflected in the linked API restriction document 
//	template<>
//	struct RenderPassTraits<ID3D12GraphicsCommandList7>
//	{
//		using value_type = ID3D12GraphicsCommandList7;
//		using pointer = ID3D12GraphicsCommandList7*;
//		using const_pointer = const ID3D12GraphicsCommandList7*;
//		using reference = ID3D12GraphicsCommandList7&;
//		using const_reference = const ID3D12GraphicsCommandList7&;
//
//		template<class DerivedSelf>
//		class Interface : public CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>
//		{
//			using Base = CommandListTraits<value_type>::InterfaceTagged<DerivedSelf, D3D12_COMMAND_LIST_TYPE_DIRECT>;
//		private:
//			//using Base::Barrier;
//		};
//	};
//#pragma endregion
//
//	template<class DerivedSelf>
//	void CommandListTraits<ID3D12GraphicsCommandList>::Interface<DerivedSelf>::ExecuteBundle(CommandList_t<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_BUNDLE> pCommandList)
//	{
//		Self().ExecuteBundle(pCommandList.Get());
//	}
//
//	template<class DerivedSelf>
//	RenderPass_t<ID3D12GraphicsCommandList4> CommandListTraits<ID3D12GraphicsCommandList4>::Interface<DerivedSelf>::BeginRenderPass(std::span<const D3D12_RENDER_PASS_RENDER_TARGET_DESC> renderTargets, const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* pDepthStencil, D3D12_RENDER_PASS_FLAGS Flags)
//	{
//		Self().BeginRenderPass(static_cast<UINT>(renderTargets.size()), renderTargets.data(), pDepthStencil, Flags);
//		return RenderPass_t<ID3D12GraphicsCommandList4>(&Get());
//	}
//}
//
//#pragma warning(pop)
