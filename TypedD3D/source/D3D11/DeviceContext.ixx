module;

#include "gsl/pointers"
#include <d3d11_4.h>
#include <tuple>
#include <optional>
#include <array>
#include <vector>
#include <utility>

#include <span>
#include <cassert>

export module TypedD3D11:DeviceContext;
import :Constants;
import :DeviceChild;
import :States;
import :Resources;
import :Shaders;
import :InputLayout;
import :ResourceViews;

namespace TypedD3D::D3D11
{
	export struct IAGetVertexBufferData
	{
		Vector<Wrapper<ID3D11Buffer>> buffers;
		std::vector<UINT> strides;
		std::vector<UINT> offsets;
	};

	export struct IAGetIndexBufferData
	{
		Wrapper<ID3D11Buffer> buffer;
		DXGI_FORMAT format;
		UINT offset;
	};

	export struct OMGetRenderTargetsAndUnorderedAccessViewsData
	{
		Vector<Wrapper<ID3D11RenderTargetView>> renderTargetViews;
		Wrapper<ID3D11DepthStencilView> depthStencilView;
		Vector<Wrapper<ID3D11UnorderedAccessView>> unorderedAccessViews;
	};

	export struct OMGetBlendStateData
	{
		Wrapper<ID3D11BlendState> blendState;
		std::array<FLOAT, 4> blendFactor;
		UINT sampleMask;
	};

	template<class Ty, class Ty2, size_t Extent>
	concept BoundedRandomAccessContainer = std::ranges::contiguous_range<Ty> && std::same_as<typename Ty::value_type, Ty2> && (std::extent_v<Ty> == Extent);

	export class IASetVertexBuffersData
	{
		UINT count;
		ID3D11Buffer* const* bufferView;
		const UINT* strideView;
		const UINT* offsetView;
	public:
		template<size_t Count>
		IASetVertexBuffersData(std::span<ID3D11Buffer*, Count> buffers, std::span<UINT, Count> strides, std::span<UINT, Count> offsets) :
			count{ buffers.size() },
			bufferView{ buffers.data() },
			strideView{ strides.data() },
			offsetView{ offsets.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == strides.size() && buffers.size() == offsets.size());
			}
		}

		template<size_t Count>
		IASetVertexBuffersData(Span<const Wrapper<ID3D11Buffer>, Count> buffers, std::span<UINT, Count> strides, std::span<UINT, Count> offsets) :
			count{ static_cast<UINT>(buffers.size()) },
			bufferView{ buffers.data() },
			strideView{ strides.data() },
			offsetView{ offsets.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == strides.size() && buffers.size() == offsets.size());
			}
		}

		IASetVertexBuffersData(Span<const Wrapper<ID3D11Buffer>> buffers, std::span<UINT> strides, std::span<UINT> offsets) :
			count{ static_cast<UINT>(buffers.size()) },
			bufferView{ buffers.data() },
			strideView{ strides.data() },
			offsetView{ offsets.data() }
		{
			assert(buffers.size() == strides.size() && buffers.size() == offsets.size());
		}

		template<size_t Count>
		IASetVertexBuffersData(Span<const WrapperView<ID3D11Buffer>, Count> buffers, std::span<UINT, Count> strides, std::span<UINT, Count> offsets) :
			count{ static_cast<UINT>(buffers.size()) },
			bufferView{ buffers.data() },
			strideView{ strides.data() },
			offsetView{ offsets.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == strides.size() && buffers.size() == offsets.size());
			}
		}

		IASetVertexBuffersData(Span<const WrapperView<ID3D11Buffer>> buffers, std::span<UINT> strides, std::span<UINT> offsets) :
			count{ static_cast<UINT>(buffers.size()) },
			bufferView{ buffers.data() },
			strideView{ strides.data() },
			offsetView{ offsets.data() }
		{
			assert(buffers.size() == strides.size() && buffers.size() == offsets.size());
		}

		IASetVertexBuffersData(const IAGetVertexBufferData& data) :
			count{ static_cast<UINT>(data.buffers.size()) },
			bufferView{ data.buffers.data() },
			strideView{ data.strides.data() },
			offsetView{ data.offsets.data() }
		{
		}

		//template<size_t Count, BoundedRandomAccessContainer<ID3D11Buffer*, Count> Ty1, BoundedRandomAccessContainer<UINT, Count> Ty2, BoundedRandomAccessContainer<UINT, Count> Ty3>
		//IASetVertexBuffersData(Ty1& buffers, Ty2& strides, Ty3& offsets) :
		//	count{ buffers.size() },
		//	bufferView{ buffers.data() },
		//	strideView{ strides.data() },
		//	offsetView{ offsets.data() }
		//{
		//	//if constexpr(Count == std::dynamic_extent)
		//	//{
		//	//	assert(buffers.size() == strides.size() && buffers.size() == offsets.size());
		//	//}
		//}

		IASetVertexBuffersData(UINT count, ID3D11Buffer* const* buffers, const UINT* strides, const UINT* offsets) :
			count{ count },
			bufferView{ buffers },
			strideView{ strides },
			offsetView{ offsets }
		{

		}

	public:
		UINT GetCount() const noexcept { return count; }
		ID3D11Buffer* const* GetBuffers() const noexcept { return bufferView; }
		const UINT* GetStrides() const noexcept { return strideView; }
		const UINT* GetOffsets() const noexcept { return offsetView; }
	};

	export class SOSetTargetsData
	{
		UINT count;
		ID3D11Buffer* const* bufferView;
		const UINT* offsetView;
	public:
		template<size_t Count>
		SOSetTargetsData(std::span<ID3D11Buffer* const, Count> buffers, std::span<UINT, Count> offsets) :
			count{ static_cast<UINT>(buffers.size()) },
			bufferView{ buffers.data() },
			offsetView{ offsets.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == offsets.size());
			}
		}

		template<size_t Count>
		SOSetTargetsData(Span<const Wrapper<ID3D11Buffer>, Count> buffers, std::span<UINT, Count> offsets) :
			count{ static_cast<UINT>(buffers.size()) },
			bufferView{ buffers.data() },
			offsetView{ offsets.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == offsets.size());
			}
		}

		template<size_t Count>
		SOSetTargetsData(Span<const WrapperView<ID3D11Buffer>, Count> buffers, std::span<UINT, Count> offsets) :
			count{ static_cast<UINT>(buffers.size()) },
			bufferView{ buffers.data() },
			offsetView{ offsets.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == offsets.size());
			}
		}

		SOSetTargetsData(UINT count, ID3D11Buffer* const* buffers, const UINT* offsets) :
			count{ count },
			bufferView{ buffers },
			offsetView{ offsets }
		{
		}

	public:
		UINT GetCount() const noexcept { return count; }
		ID3D11Buffer* const* GetBuffers() const noexcept { return bufferView; }
		const UINT* GetOffsets() const noexcept { return offsetView; }
	};

	export class CSSetUnorderedAccessViewsData
	{
		UINT count;
		ID3D11UnorderedAccessView* const* bufferView;
		const UINT* pUAVInitialCounts;
	public:
		template<UINT Count>
		CSSetUnorderedAccessViewsData(std::span<ID3D11UnorderedAccessView*, Count> buffers, std::span<UINT, Count> pUAVInitialCounts) :
			count{ buffers.size() },
			bufferView{ buffers.data() },
			pUAVInitialCounts{ pUAVInitialCounts.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == pUAVInitialCounts.size());
			}
		}

		template<UINT Count>
		CSSetUnorderedAccessViewsData(Span<const Wrapper<ID3D11UnorderedAccessView>, Count> buffers, std::span<UINT, Count> pUAVInitialCounts) :
			count{ buffers.size() },
			bufferView{ buffers.data() },
			pUAVInitialCounts{ pUAVInitialCounts.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == pUAVInitialCounts.size());
			}
		}

		template<UINT Count>
		CSSetUnorderedAccessViewsData(Span<const WrapperView<ID3D11UnorderedAccessView>, Count> buffers, std::span<UINT, Count> pUAVInitialCounts) :
			count{ buffers.size() },
			bufferView{ buffers.data() },
			pUAVInitialCounts{ pUAVInitialCounts.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == pUAVInitialCounts.size());
			}
		}

		CSSetUnorderedAccessViewsData(UINT count, ID3D11UnorderedAccessView* const* buffers, const UINT* pUAVInitialCounts) :
			count{ count },
			bufferView{ buffers },
			pUAVInitialCounts{ pUAVInitialCounts }
		{
		}

	public:
		UINT GetCount() const noexcept { return count; }
		ID3D11UnorderedAccessView* const* GetBuffers() const noexcept { return bufferView; }
		const UINT* GetInitialCounts() const noexcept { return pUAVInitialCounts; }
	};
}

namespace TypedD3D
{
	template<>
	struct UntaggedTraits<ID3D11DeviceContext>
	{
		using value_type = ID3D11DeviceContext;
		using pointer = ID3D11DeviceContext*;
		using const_pointer = const ID3D11DeviceContext*;
		using reference = ID3D11DeviceContext&;
		using const_reference = const ID3D11DeviceContext&;

		template<class Derived>
		class Interface : public UntaggedTraits<ID3D11DeviceChild>::Interface<Derived>
		{
		public:
			D3D11_MAPPED_SUBRESOURCE Map(
				gsl::not_null<WrapperView<ID3D11Resource>> pResource,
				UINT Subresource,
				D3D11_MAP MapType,
				UINT MapFlags)
			{
				D3D11_MAPPED_SUBRESOURCE subresource;

				ThrowIfFailed(Self().Map(pResource.get().Get(), Subresource, MapType, MapFlags, &subresource));
				return subresource;
			}

			void Unmap(
				gsl::not_null<WrapperView<ID3D11Resource>> pResource,
				UINT Subresource)
			{
				Self().Unmap(pResource.get().Get(), Subresource);
			}

			void ClearState()
			{
				return Self().ClearState();
			}

			void Flush()
			{
				return Self().Flush();
			}

			D3D11_DEVICE_CONTEXT_TYPE GetType()
			{
				return Self().GetType();
			}

			UINT GetContextFlags()
			{
				return Self().GetContextFlags();
			}

			void ExecuteCommandList(
				ID3D11CommandList& pCommandList,
				BOOL RestoreContextState)
			{
				Self().ExecuteCommandList(&pCommandList, RestoreContextState);
			}

			Wrapper<ID3D11CommandList> FinishCommandList(BOOL RestoreDeferredContextState)
			{
				Wrapper<ID3D11CommandList> commandList;
				ThrowIfFailed(Self().FinishCommandList(RestoreDeferredContextState, OutPtr{ commandList }));
				return commandList;
			}

			void Begin(
				ID3D11Asynchronous& pAsync)
			{
				Self().Begin(&pAsync);
			}

			void End(
				ID3D11Asynchronous& pAsync)
			{
				Self().End(&pAsync);
			}

			void* GetData(
				ID3D11Asynchronous& pAsync,
				UINT GetDataFlags)
			{
				void* data = nullptr;

				ThrowIfFailed(Self().GetData(&pAsync, data, pAsync.GetDataSize(), GetDataFlags));

				return data;
			}

			void SetPredication(
				WrapperView<ID3D11Predicate> pPredicate,
				BOOL PredicateValue)
			{
				Self().SetPredication(pPredicate.Get(), PredicateValue);
			}

			std::pair<Wrapper<ID3D11Predicate>, BOOL> GetPredication()
			{
				std::pair<Wrapper<ID3D11Predicate>, BOOL> output;
				Self().GetPredication(OutPtr{ output.first }, &output.second);
				return output;
			}

			void DrawIndexed(
				UINT IndexCount,
				UINT StartIndexLocation,
				INT BaseVertexLocation)
			{
				Self().DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
			}

			void Draw(
				UINT VertexCount,
				UINT StartVertexLocation)
			{
				Self().Draw(VertexCount, StartVertexLocation);
			}

			void DrawIndexedInstanced(
				UINT IndexCountPerInstance,
				UINT InstanceCount,
				UINT StartIndexLocation,
				INT BaseVertexLocation,
				UINT StartInstanceLocation)
			{
				Self().DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
			}

			void DrawInstanced(
				UINT VertexCountPerInstance,
				UINT InstanceCount,
				UINT StartVertexLocation,
				UINT StartInstanceLocation)
			{
				Self().DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
			}

			void DrawAuto()
			{
				Self().DrawAuto();
			}

			void DrawIndexedInstancedIndirect(
				gsl::not_null<WrapperView<ID3D11Buffer>> pBufferForArgs,
				UINT AlignedByteOffsetForArgs)
			{
				Self().DrawIndexedInstancedIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
			}

			void DrawInstancedIndirect(
				gsl::not_null<WrapperView<ID3D11Buffer>> pBufferForArgs,
				UINT AlignedByteOffsetForArgs)
			{
				Self().DrawInstancedIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
			}

			void Dispatch(
				UINT ThreadGroupCountX,
				UINT ThreadGroupCountY,
				UINT ThreadGroupCountZ)
			{
				Self().Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
			}

			void DispatchIndirect(
				gsl::not_null<WrapperView<ID3D11Buffer>> pBufferForArgs,
				UINT AlignedByteOffsetForArgs)
			{
				Self().DispatchIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
			}

			void GenerateMips(
				gsl::not_null<WrapperView<ID3D11ShaderResourceView>> pShaderResourceView)
			{
				Self().GenerateMips(pShaderResourceView.get().Get());
			}

			void SetResourceMinLOD(
				gsl::not_null<WrapperView<ID3D11Resource>> pResource,
				FLOAT MinLOD)
			{
				Self().SetResourceMinLOD(pResource.get().Get(), MinLOD);
			}

			FLOAT GetResourceMinLOD(
				gsl::not_null<WrapperView<ID3D11Resource>> pResource)
			{
				return Self().GetResourceMinLOD(pResource.get().Get());
			}

			void ResolveSubresource(
				gsl::not_null<WrapperView<ID3D11Resource>> pDstResource,
				UINT DstSubresource,
				gsl::not_null<WrapperView<ID3D11Resource>> pSrcResource,
				UINT SrcSubresource,
				DXGI_FORMAT Format)
			{
				Self().ResolveSubresource(pDstResource.get().Get(), DstSubresource, pSrcResource.get().Get(), SrcSubresource, Format);
			}

			void CopySubresourceRegion(
				gsl::not_null<WrapperView<ID3D11Resource>> pDstResource,
				UINT DstSubresource,
				UINT DstX,
				UINT DstY,
				UINT DstZ,
				gsl::not_null<WrapperView<ID3D11Resource>>  pSrcResource,
				UINT SrcSubresource,
				const D3D11_BOX* pSrcBox)
			{
				Self().CopySubresourceRegion(
					pDstResource.get().Get(),
					DstSubresource,
					DstX,
					DstY,
					DstZ,
					pSrcResource.get().Get(),
					SrcSubresource,
					pSrcBox);
			}

			void CopyResource(
				gsl::not_null<WrapperView<ID3D11Resource>>  pDstResource,
				gsl::not_null<WrapperView<ID3D11Resource>>  pSrcResource)
			{
				Self().CopyResource(
					pDstResource.get().Get(),
					pSrcResource.get().Get());
			}

			void UpdateSubresource(
				gsl::not_null<WrapperView<ID3D11Resource>> pDstResource,
				UINT DstSubresource,
				const D3D11_BOX* pDstBox,
				const void* pSrcData,
				UINT SrcRowPitch,
				UINT SrcDepthPitch)
			{
				Self().UpdateSubresource(
					pDstResource.get().Get(),
					DstSubresource,
					pDstBox,
					pSrcData,
					SrcRowPitch,
					SrcDepthPitch);
			}

			void CopyStructureCount(
				gsl::not_null<WrapperView<ID3D11Buffer>> pDstBuffer,
				UINT DstAlignedByteOffset,
				gsl::not_null<WrapperView<ID3D11UnorderedAccessView>> pSrcView)
			{
				Self().CopyStructureCount(pDstBuffer.get().Get(), DstAlignedByteOffset, pSrcView.get().Get());
			}

			void ClearRenderTargetView(
				gsl::not_null<WrapperView<ID3D11RenderTargetView>> pRenderTargetView,
				std::span<const FLOAT, 4> ColorRGBA)
			{
				Self().ClearRenderTargetView(pRenderTargetView.get().Get(), ColorRGBA.data());
			}

			void ClearRenderTargetView(
				gsl::not_null<WrapperView<ID3D11RenderTargetView>> pRenderTargetView,
				std::array<const FLOAT, 4> ColorRGBA)
			{
				ClearRenderTargetView(pRenderTargetView, std::span{ ColorRGBA });
			}

			void ClearUnorderedAccessViewUint(
				gsl::not_null<WrapperView<ID3D11UnorderedAccessView>> pUnorderedAccessView,
				std::span<const UINT, 4> Values)
			{
				Self().ClearUnorderedAccessViewUint(pUnorderedAccessView.get().Get(), Values.data());
			}

			void ClearUnorderedAccessViewUint(
				gsl::not_null<WrapperView<ID3D11UnorderedAccessView>> pUnorderedAccessView,
				std::array<const UINT, 4> Values)
			{
				ClearUnorderedAccessViewUint(pUnorderedAccessView, std::span{ Values });
			}

			void ClearUnorderedAccessViewFloat(
				gsl::not_null<WrapperView<ID3D11UnorderedAccessView>> pUnorderedAccessView,
				std::span<const FLOAT, 4> Values)
			{
				Self().ClearUnorderedAccessViewFloat(pUnorderedAccessView.get().Get(), Values.data());
			}

			void ClearUnorderedAccessViewFloat(
				gsl::not_null<WrapperView<ID3D11UnorderedAccessView>> pUnorderedAccessView,
				std::array<const FLOAT, 4> Values)
			{
				ClearUnorderedAccessViewFloat(pUnorderedAccessView, std::span{ Values });
			}

			void ClearDepthStencilView(
				gsl::not_null<WrapperView<ID3D11DepthStencilView>> pDepthStencilView,
				UINT ClearFlags,
				FLOAT Depth,
				UINT8 Stencil)
			{
				Self().ClearDepthStencilView(pDepthStencilView.get().Get(), ClearFlags, Depth, Stencil);
			}

			void SOSetTargets(D3D11::SOSetTargetsData ppSOTargets)
			{
				Self().SOSetTargets(ppSOTargets.GetCount(), ppSOTargets.GetBuffers(), ppSOTargets.GetOffsets());
			}

			Vector<Wrapper<ID3D11Buffer>> SOGetTargets(UINT NumBuffers)
			{
				Vector<Wrapper<ID3D11Buffer>> buffers;
				buffers.resize(NumBuffers);
				Self().SOGetTargets(NumBuffers, buffers.data());

				return buffers;
			}

			void IASetInputLayout(
				WrapperView<ID3D11InputLayout> pInputLayout)
			{
				Self().IASetInputLayout(pInputLayout.Get());
			}

			void IASetVertexBuffers(
				UINT StartSlot,
				WrapperView<ID3D11Buffer> buffer,
				UINT stride,
				UINT offset)
			{
				ID3D11Buffer* temp = buffer.Get();
				IASetVertexBuffers(
					StartSlot,
					{ 1, &temp, &stride, &offset });
			}

			void IASetVertexBuffers(
				UINT StartSlot,
				D3D11::IASetVertexBuffersData data)
			{
				Self().IASetVertexBuffers(
					StartSlot,
					data.GetCount(),
					data.GetBuffers(),
					data.GetStrides(),
					data.GetOffsets());
			}

			void IASetIndexBuffer(
				WrapperView<ID3D11Buffer> pIndexBuffer,
				DXGI_FORMAT Format,
				UINT Offset)
			{
				Self().IASetIndexBuffer(pIndexBuffer.Get(), Format, Offset);
			}

			void IASetPrimitiveTopology(
				D3D11_PRIMITIVE_TOPOLOGY Topology)
			{
				Self().IASetPrimitiveTopology(Topology);
			}

			Wrapper<ID3D11InputLayout> IAGetInputLayout()
			{
				Wrapper<ID3D11InputLayout> layout;
				Self().IAGetInputLayout(OutPtr{ layout });
				return layout;
			}

			D3D11::IAGetVertexBufferData IAGetVertexBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				D3D11::IAGetVertexBufferData output;
				output.buffers.resize(NumBuffers);
				output.strides.resize(NumBuffers);
				output.offsets.resize(NumBuffers);
				Self().IAGetVertexBuffers(StartSlot, NumBuffers, output.buffers.data(), output.strides.data(), output.offsets.data());

				return output;
			}

			D3D11::IAGetIndexBufferData IAGetIndexBuffer()
			{
				D3D11::IAGetIndexBufferData output;
				Self().IAGetIndexBuffer(OutPtr{ output.buffer }, &output.format, &output.offset);
				return output;
			}

			D3D11_PRIMITIVE_TOPOLOGY IAGetPrimitiveTopology()
			{
				D3D11_PRIMITIVE_TOPOLOGY topology;
				Self().IAGetPrimitiveTopology(&topology);
				return topology;
			}

			void RSSetState(
				WrapperView<ID3D11RasterizerState> optRasterizerState)
			{
				Self().RSSetState(optRasterizerState.Get());
			}

			void RSSetViewports(
				std::span<const D3D11_VIEWPORT> pViewports)
			{
				Self().RSSetViewports(static_cast<UINT>(pViewports.size()), pViewports.data());
			}

			void RSSetViewports(
				D3D11_VIEWPORT pViewports)
			{
				RSSetViewports(std::span{ &pViewports, 1 });
			}

			void RSSetScissorRects(
				std::span<const D3D11_RECT> pRects)
			{
				Self().RSSetScissorRects(static_cast<UINT>(pRects.size()), pRects.data());
			}

			void RSSetScissorRects(
				D3D11_RECT pRects)
			{
				RSSetScissorRects(std::span{ &pRects, 1 });
			}

			Wrapper<ID3D11RasterizerState> RSGetState()
			{
				Wrapper<ID3D11RasterizerState> state;
				Self().RSGetState(OutPtr{ state });
				return state;
			}

			std::vector<D3D11_VIEWPORT> RSGetViewports()
			{
				UINT numViewports = 0;
				Self().RSGetViewports(&numViewports, nullptr);

				std::vector<D3D11_VIEWPORT> output(numViewports);
				Self().RSGetViewports(&numViewports, output.data());

				return output;
			}

			std::vector<D3D11_RECT> RSGetScissorRects()
			{
				UINT numRects = 0;
				Self().RSGetScissorRects(&numRects, nullptr);

				std::vector<D3D11_RECT> output(numRects);
				Self().RSGetScissorRects(&numRects, output.data());

				return output;
			}

			void OMSetRenderTargets(
				Span<const WrapperView<ID3D11RenderTargetView>> optRenderTargetViews,
				WrapperView<ID3D11DepthStencilView> optDepthStencilView)
			{
				Self().OMSetRenderTargets(static_cast<UINT>(optRenderTargetViews.size()), optRenderTargetViews.data(), optDepthStencilView.Get());
			}

			void OMSetRenderTargets(
				WrapperView<ID3D11RenderTargetView> optRenderTargetViews,
				WrapperView<ID3D11DepthStencilView> optDepthStencilView)
			{
				auto temp = optRenderTargetViews.Get();
				OMSetRenderTargets(Span<const WrapperView<ID3D11RenderTargetView>>{ &temp, temp ? size_t{ 1 } : size_t{ 0 } }, optDepthStencilView);
			}

			void OMSetRenderTargetsAndUnorderedAccessViews(
				Span<const WrapperView<ID3D11RenderTargetView>> optRenderTargetViews,
				WrapperView<ID3D11DepthStencilView> optDepthStencilView,
				UINT UAVStartSlot,
				Span<const WrapperView<ID3D11UnorderedAccessView>> ppUnorderedAccessViews,
				const UINT* pUAVInitialCounts)
			{
				Self().OMSetRenderTargetsAndUnorderedAccessViews(
					static_cast<UINT>(optRenderTargetViews.size()),
					optRenderTargetViews.data(),
					optDepthStencilView.Get(),
					UAVStartSlot,
					static_cast<UINT>(ppUnorderedAccessViews.size()),
					ppUnorderedAccessViews.data(),
					pUAVInitialCounts);
			}

			void OMSetBlendState(
				WrapperView<ID3D11BlendState> optBlendState,
				std::optional<std::span<const FLOAT, 4>> BlendFactor,
				UINT SampleMask)
			{
				if(BlendFactor.has_value())
					Self().OMSetBlendState(optBlendState.Get(), BlendFactor.value().data(), SampleMask);
				else
					Self().OMSetBlendState(optBlendState.Get(), nullptr, SampleMask);
			}

			void OMSetDepthStencilState(
				WrapperView<ID3D11DepthStencilState> optDepthStencilState,
				UINT StencilRef)
			{
				Self().OMSetDepthStencilState(optDepthStencilState.Get(), StencilRef);
			}

			std::pair<Vector<Wrapper<ID3D11RenderTargetView>>, Wrapper<ID3D11DepthStencilView>>  OMGetRenderTargets(
				UINT NumViews)
			{
				std::pair<Vector<Wrapper<ID3D11RenderTargetView>>, Wrapper<ID3D11DepthStencilView>> output;
				output.first.resize(NumViews);
				Self().OMGetRenderTargets(NumViews, output.first.data(), OutPtr{ output.second });

				return output;
			}

			D3D11::OMGetRenderTargetsAndUnorderedAccessViewsData OMGetRenderTargetsAndUnorderedAccessViews(
				UINT NumRTVs,
				UINT UAVStartSlot,
				UINT NumUAVs)
			{
				D3D11::OMGetRenderTargetsAndUnorderedAccessViewsData data;

				data.renderTargetViews.resize(NumRTVs);
				data.unorderedAccessViews.resize(NumUAVs);

				Self().OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, data.renderTargetViews.data(), OutPtr{ data.depthStencilView }, UAVStartSlot, NumUAVs, data.unorderedAccessViews.data());
				return data;
			}

			D3D11::OMGetBlendStateData OMGetBlendState()
			{
				D3D11::OMGetBlendStateData data;
				Self().OMGetBlendState(OutPtr{ data.blendState }, data.blendFactor.data(), &data.sampleMask);
				return data;
			}

			std::pair<Wrapper<ID3D11DepthStencilState>, UINT> OMGetDepthStencilState()
			{
				std::pair<Wrapper<ID3D11DepthStencilState>, UINT> state;
				Self().OMGetDepthStencilState(OutPtr{ state.first }, &state.second);
				return state;
			}

			void VSSetShader(
				WrapperView<ID3D11VertexShader> optVertexShader,
				Span<const WrapperView<ID3D11ClassInstance>> optClassInstances)
			{
				Self().VSSetShader(optVertexShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void VSSetShader(
				WrapperView<ID3D11VertexShader> optVertexShader,
				WrapperView<ID3D11ClassInstance> optClassInstances)
			{
				Array temp{ optClassInstances };
				VSSetShader(optVertexShader.Get(), optClassInstances ? Span<const WrapperView<ID3D11ClassInstance>>{ temp } : Span<const WrapperView<ID3D11ClassInstance>>{});
			}

			void VSSetConstantBuffers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11Buffer>> ppConstantBuffers)
			{
				Self().VSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void VSSetConstantBuffers(
				UINT StartSlot,
				WrapperView<ID3D11Buffer> ppConstantBuffers)
			{
				auto temp = ppConstantBuffers.Get();
				VSSetConstantBuffers(StartSlot, Span<const WrapperView<ID3D11Buffer>>{ &temp, 1 });
			}

			void VSSetShaderResources(
				UINT StartSlot,
				Span<const WrapperView<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Self().VSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void VSSetShaderResources(
				UINT StartSlot,
				WrapperView<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				auto temp = ppShaderResourceViews.Get();
				VSSetShaderResources(StartSlot, Span<const WrapperView<ID3D11ShaderResourceView>>{ &temp, 1 });
			}

			void VSSetSamplers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11SamplerState>> ppSamplers)
			{
				Self().VSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void VSSetSamplers(
				UINT StartSlot,
				WrapperView<ID3D11SamplerState> ppSamplers)
			{
				auto temp = ppSamplers.Get();
				VSSetSamplers(StartSlot, Span<const WrapperView<ID3D11SamplerState>>{ &temp, 1 });
			}

			Vector<Wrapper<ID3D11Buffer>> VSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				Vector<Wrapper<ID3D11Buffer>> buffers;
				buffers.resize(NumBuffers);
				Self().VSGetConstantBuffers(StartSlot, NumBuffers, buffers.data());
				return buffers;
			}

			Vector<Wrapper<ID3D11ShaderResourceView>> VSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				Vector<Wrapper<ID3D11ShaderResourceView>> views;
				views.resize(NumViews);
				Self().VSGetShaderResources(StartSlot, NumViews, views.data());

				return views;
			}

			Vector<Wrapper<ID3D11SamplerState>> VSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				Vector<Wrapper<ID3D11SamplerState>> samplers;
				samplers.resize(NumSamplers);
				Self().VSGetSamplers(StartSlot, NumSamplers, samplers.data());
				return samplers;
			}

			std::pair<Wrapper<ID3D11VertexShader>, Vector<Wrapper<ID3D11ClassInstance>>> VSGetShader(UINT& numInstances)
			{
				std::pair<Wrapper<ID3D11VertexShader>, Vector<Wrapper<ID3D11ClassInstance>>> output;
				output.second.resize(numInstances);
				Self().VSGetShader(OutPtr{ output.first }, output.second.data(), &numInstances);
				return output;
			}

			std::pair<Wrapper<ID3D11VertexShader>, UINT> VSGetShader(Span<Wrapper<ID3D11ClassInstance>> classIntances)
			{
				std::pair<Wrapper<ID3D11VertexShader>, UINT> output;
				output.second = classIntances.size();
				Self().VSGetShader(OutPtr{ output.first }, classIntances.data(), &output.second);
				return output;
			}

			void HSSetShaderResources(
				UINT StartSlot,
				Span<const WrapperView<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Self().HSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void HSSetShaderResources(
				UINT StartSlot,
				WrapperView<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				auto temp = ppShaderResourceViews.Get();
				HSSetShaderResources(StartSlot, Span<const WrapperView<ID3D11ShaderResourceView>>{ &temp, 1 });
			}

			void HSSetShader(
				WrapperView<ID3D11HullShader> optHullShader,
				Span<const WrapperView<ID3D11ClassInstance>> optClassInstances)
			{
				Self().HSSetShader(optHullShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void HSSetShader(
				WrapperView<ID3D11HullShader> optHullShader,
				WrapperView<ID3D11ClassInstance> optClassInstances)
			{
				Array temp{ optClassInstances };
				HSSetShader(optHullShader.Get(), optClassInstances ? Span<const WrapperView<ID3D11ClassInstance>>{ temp } : Span<const WrapperView<ID3D11ClassInstance>>{});
			}

			void HSSetSamplers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11SamplerState>> ppSamplers)
			{
				Self().HSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void HSSetSamplers(
				UINT StartSlot,
				WrapperView<ID3D11SamplerState> ppSamplers)
			{
				auto temp = ppSamplers.Get();
				HSSetSamplers(StartSlot, Span<const WrapperView<ID3D11SamplerState>>{ &temp, 1 });
			}

			void HSSetConstantBuffers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11Buffer>> ppConstantBuffers)
			{
				Self().HSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void HSSetConstantBuffers(
				UINT StartSlot,
				WrapperView<ID3D11Buffer> ppConstantBuffers)
			{
				auto temp = ppConstantBuffers.Get();
				HSSetConstantBuffers(StartSlot, Span<const WrapperView<ID3D11Buffer>>{ &temp, 1 });
			}

			Vector<Wrapper<ID3D11ShaderResourceView>> HSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				Vector<Wrapper<ID3D11ShaderResourceView>> views;
				views.resize(NumViews);
				Self().HSGetShaderResources(StartSlot, NumViews, views.data());
				return views;
			}

			std::pair<Wrapper<ID3D11HullShader>, Vector<Wrapper<ID3D11ClassInstance>>> HSGetShader(UINT& numInstances)
			{
				std::pair<Wrapper<ID3D11HullShader>, Vector<Wrapper<ID3D11ClassInstance>>> output;
				output.second.resize(numInstances);
				Self().HSGetShader(OutPtr{ output.first }, output.second.data(), &numInstances);
				return output;
			}

			std::pair<Wrapper<ID3D11HullShader>, UINT> HSGetShader(Span<Wrapper<ID3D11ClassInstance>> classIntances)
			{
				std::pair<Wrapper<ID3D11HullShader>, UINT> output;
				output.second = classIntances.size();
				Self().HSGetShader(OutPtr{ output.first }, classIntances.data(), &output.second);
				return output;
			}

			Vector<Wrapper<ID3D11SamplerState>> HSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				Vector<Wrapper<ID3D11SamplerState>> samplers;
				samplers.resize(NumSamplers);
				Self().HSGetSamplers(StartSlot, NumSamplers, samplers.data());
				return samplers;
			}

			Vector<Wrapper<ID3D11Buffer>> HSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				Vector<Wrapper<ID3D11Buffer>> buffers;
				buffers.resize(NumBuffers);
				Self().HSGetConstantBuffers(StartSlot, NumBuffers, buffers.data());
				return buffers;
			}

			void DSSetShaderResources(
				UINT StartSlot,
				Span<const WrapperView<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Self().DSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void DSSetShaderResources(
				UINT StartSlot,
				WrapperView<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				auto temp = ppShaderResourceViews.Get();
				DSSetShaderResources(StartSlot, Span<const WrapperView<ID3D11ShaderResourceView>>{ &temp, 1 });
			}

			void DSSetShader(
				WrapperView<ID3D11DomainShader> optDomainShader,
				Span<const WrapperView<ID3D11ClassInstance>> optClassInstances)
			{
				Self().DSSetShader(optDomainShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void DSSetShader(
				WrapperView<ID3D11DomainShader> optDomainShader,
				WrapperView<ID3D11ClassInstance> optClassInstances)
			{
				Array temp{ optClassInstances };
				DSSetShader(optDomainShader.Get(), optClassInstances ? Span<const WrapperView<ID3D11ClassInstance>>{ temp } : Span<const WrapperView<ID3D11ClassInstance>>{});
			}

			void DSSetSamplers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11SamplerState>> ppSamplers)
			{
				Self().DSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void DSSetSamplers(
				UINT StartSlot,
				WrapperView<ID3D11SamplerState> ppSamplers)
			{
				auto temp = ppSamplers.Get();
				DSSetSamplers(StartSlot, Span<const WrapperView<ID3D11SamplerState>>{ &temp, 1 });
			}

			void DSSetConstantBuffers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11Buffer>> ppConstantBuffers)
			{
				Self().DSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void DSSetConstantBuffers(
				UINT StartSlot,
				WrapperView<ID3D11Buffer> ppConstantBuffers)
			{
				auto temp = ppConstantBuffers.Get();
				DSSetConstantBuffers(StartSlot, Span<const WrapperView<ID3D11Buffer>>{ &temp, 1 });
			}

			Vector<Wrapper<ID3D11ShaderResourceView>> DSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				Vector<Wrapper<ID3D11ShaderResourceView>> views;
				views.resize(NumViews);
				Self().DSGetShaderResources(StartSlot, NumViews, views.data());
				return views;
			}

			std::pair<Wrapper<ID3D11DomainShader>, Vector<Wrapper<ID3D11ClassInstance>>> DSGetShader(UINT& numInstances)
			{
				std::pair<Wrapper<ID3D11DomainShader>, Vector<Wrapper<ID3D11ClassInstance>>> output;
				output.second.resize(numInstances);
				Self().DSGetShader(OutPtr{ output.first }, output.second.data(), &numInstances);
				return output;
			}

			std::pair<Wrapper<ID3D11DomainShader>, UINT> DSGetShader(Span<Wrapper<ID3D11ClassInstance>> classIntances)
			{
				std::pair<Wrapper<ID3D11DomainShader>, UINT> output;
				output.second = classIntances.size();
				Self().DSGetShader(OutPtr{ output.first }, classIntances.data(), &output.second);
				return output;
			}

			Vector<Wrapper<ID3D11SamplerState>> DSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				Vector<Wrapper<ID3D11SamplerState>> samplers;
				samplers.resize(NumSamplers);
				Self().DSGetSamplers(StartSlot, NumSamplers, samplers.data());

				return samplers;
			}

			Vector<Wrapper<ID3D11Buffer>> DSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				Vector<Wrapper<ID3D11Buffer>> buffers;
				buffers.resize(NumBuffers);
				Self().DSGetConstantBuffers(StartSlot, NumBuffers, buffers.data());
				return buffers;
			}

			void GSSetConstantBuffers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11Buffer>> ppConstantBuffers)
			{
				Self().GSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void GSSetConstantBuffers(
				UINT StartSlot,
				WrapperView<ID3D11Buffer> ppConstantBuffers)
			{
				auto temp = ppConstantBuffers.Get();
				GSSetConstantBuffers(StartSlot, Span<const WrapperView<ID3D11Buffer>>{ &temp, 1 });
			}

			void GSSetShader(
				WrapperView<ID3D11GeometryShader> optGeometryShader,
				Span<const WrapperView<ID3D11ClassInstance>> optClassInstances)
			{
				Self().GSSetShader(optGeometryShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void GSSetShader(
				WrapperView<ID3D11GeometryShader> optGeometryShader,
				WrapperView<ID3D11ClassInstance> optClassInstances)
			{
				Array temp{ optClassInstances };
				GSSetShader(optGeometryShader.Get(), optClassInstances ? Span<const WrapperView<ID3D11ClassInstance>>{ temp } : Span<const WrapperView<ID3D11ClassInstance>>{});
			}

			void GSSetShaderResources(
				UINT StartSlot,
				Span<const WrapperView<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Self().GSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void GSSetShaderResources(
				UINT StartSlot,
				WrapperView<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				auto temp = ppShaderResourceViews.Get();
				GSSetShaderResources(StartSlot, Span<const WrapperView<ID3D11ShaderResourceView>>{ &temp, 1 });
			}

			void GSSetSamplers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11SamplerState>> ppSamplers)
			{
				Self().GSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void GSSetSamplers(
				UINT StartSlot,
				WrapperView<ID3D11SamplerState> ppSamplers)
			{
				auto temp = ppSamplers.Get();
				GSSetSamplers(StartSlot, Span<const WrapperView<ID3D11SamplerState>>{ &temp, 1 });
			}

			Vector<Wrapper<ID3D11Buffer>> GSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				Vector<Wrapper<ID3D11Buffer>> buffers;
				buffers.resize(NumBuffers);
				Self().GSGetConstantBuffers(StartSlot, NumBuffers, buffers.data());
				return buffers;
			}

			std::pair<Wrapper<ID3D11GeometryShader>, Vector<Wrapper<ID3D11ClassInstance>>> GSGetShader(UINT& numInstances)
			{
				std::pair<Wrapper<ID3D11GeometryShader>, Vector<Wrapper<ID3D11ClassInstance>>> output;
				output.second.resize(numInstances);
				Self().GSGetShader(OutPtr{ output.first }, output.second.data(), &numInstances);
				return output;
			}

			std::pair<Wrapper<ID3D11GeometryShader>, UINT> GSGetShader(Span<Wrapper<ID3D11ClassInstance>> classIntances)
			{
				std::pair<Wrapper<ID3D11GeometryShader>, UINT> output;
				output.second = classIntances.size();
				Self().GSGetShader(OutPtr{ output.first }, classIntances.data(), &output.second);
				return output;
			}

			Vector<Wrapper<ID3D11ShaderResourceView>> GSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				Vector<Wrapper<ID3D11ShaderResourceView>> views;
				views.resize(NumViews);
				Self().GSGetShaderResources(StartSlot, NumViews, views.data());
				return views;
			}

			Vector<Wrapper<ID3D11SamplerState>> GSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				Vector<Wrapper<ID3D11SamplerState>> samplers;
				samplers.resize(NumSamplers);
				Self().GSGetSamplers(StartSlot, NumSamplers, samplers.data());
				return samplers;
			}

			void PSSetShaderResources(
				UINT StartSlot,
				Span<const WrapperView<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Self().PSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void PSSetShaderResources(
				UINT StartSlot,
				WrapperView<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				auto temp = ppShaderResourceViews.Get();
				PSSetShaderResources(StartSlot, Span<const WrapperView<ID3D11ShaderResourceView>>{ &temp, 1 });
			}

			void PSSetShader(
				WrapperView<ID3D11PixelShader> optPixelShader,
				Span<const WrapperView<ID3D11ClassInstance>> optClassInstances)
			{
				Self().PSSetShader(optPixelShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void PSSetShader(
				WrapperView<ID3D11PixelShader> optPixelShader,
				WrapperView<ID3D11ClassInstance> optClassInstances)
			{
				Array temp{ optClassInstances };
				PSSetShader(optPixelShader.Get(), optClassInstances ? Span<const WrapperView<ID3D11ClassInstance>>{ temp } : Span<const WrapperView<ID3D11ClassInstance>>{});
			}

			void PSSetSamplers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11SamplerState>> ppSamplers)
			{
				Self().PSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void PSSetSamplers(
				UINT StartSlot,
				WrapperView<ID3D11SamplerState> ppSamplers)
			{
				auto temp = ppSamplers.Get();
				PSSetSamplers(StartSlot, Span<const WrapperView<ID3D11SamplerState>>{ &temp, 1 });
			}

			void PSSetConstantBuffers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11Buffer>> ppConstantBuffers)
			{
				Self().PSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void PSSetConstantBuffers(
				UINT StartSlot,
				WrapperView<ID3D11Buffer> ppConstantBuffers)
			{
				auto temp = ppConstantBuffers.Get();
				PSSetConstantBuffers(StartSlot, Span<const WrapperView<ID3D11Buffer>>{ &temp, 1 });
			}

			Vector<Wrapper<ID3D11ShaderResourceView>> PSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				Vector<Wrapper<ID3D11ShaderResourceView>> views;
				views.resize(NumViews);
				Self().PSGetShaderResources(StartSlot, NumViews, views.data());
				return views;
			}

			std::pair<Wrapper<ID3D11PixelShader>, Vector<Wrapper<ID3D11ClassInstance>>> PSGetShader(UINT& numInstances)
			{
				std::pair<Wrapper<ID3D11PixelShader>, Vector<Wrapper<ID3D11ClassInstance>>> output;
				output.second.resize(numInstances);
				Self().PSGetShader(OutPtr{ output.first }, output.second.data(), &numInstances);
				return output;
			}

			std::pair<Wrapper<ID3D11PixelShader>, UINT> PSGetShader(Span<Wrapper<ID3D11ClassInstance>> classIntances)
			{
				std::pair<Wrapper<ID3D11PixelShader>, UINT> output;
				output.second = classIntances.size();
				Self().PSGetShader(OutPtr{ output.first }, classIntances.data(), &output.second);
				return output;
			}

			Vector<Wrapper<ID3D11SamplerState>> PSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				Vector<Wrapper<ID3D11SamplerState>> samplers;
				samplers.resize(NumSamplers);
				Self().PSGetSamplers(StartSlot, NumSamplers, samplers.data());
				return samplers;
			}

			Vector<Wrapper<ID3D11Buffer>> PSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				Vector<Wrapper<ID3D11Buffer>> buffers;
				buffers.resize(NumBuffers);
				Self().PSGetConstantBuffers(StartSlot, NumBuffers, buffers.data());
				return buffers;
			}

			void CSSetShaderResources(
				UINT StartSlot,
				Span<const WrapperView<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Self().CSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void CSSetShaderResources(
				UINT StartSlot,
				WrapperView<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				auto temp = ppShaderResourceViews.Get();
				CSSetShaderResources(StartSlot, Span<const WrapperView<ID3D11ShaderResourceView>>{ &temp, 1 });
			}

			void CSSetUnorderedAccessViews(
				UINT StartSlot,
				D3D11::CSSetUnorderedAccessViewsData data)
			{
				Self().CSSetUnorderedAccessViews(StartSlot, data.GetCount(), data.GetBuffers(), data.GetInitialCounts());
			}

			void CSSetUnorderedAccessViews(
				UINT StartSlot,
				ID3D11UnorderedAccessView* data,
				UINT initialCounts)
			{
				CSSetUnorderedAccessViews(StartSlot, { 1, &data, &initialCounts });
			}

			void CSSetUnorderedAccessViews(
				UINT StartSlot,
				WrapperView<ID3D11UnorderedAccessView> data,
				UINT initialCounts)
			{
				auto temp = data.Get();
				CSSetUnorderedAccessViews(StartSlot, { 1, &temp, &initialCounts });
			}

			void CSSetShader(
				WrapperView<ID3D11ComputeShader> optComputeShader,
				Span<const WrapperView<ID3D11ClassInstance>> optClassInstances)
			{
				Self().CSSetShader(optComputeShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void CSSetShader(
				WrapperView<ID3D11ComputeShader> optComputeShader,
				WrapperView<ID3D11ClassInstance> optClassInstances)
			{
				Array temp{ optClassInstances };
				CSSetShader(optComputeShader.Get(), optClassInstances ? Span<const WrapperView<ID3D11ClassInstance>>{ temp } : Span<const WrapperView<ID3D11ClassInstance>>{});
			}

			void CSSetSamplers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11SamplerState>> ppSamplers)
			{
				Self().CSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void CSSetSamplers(
				UINT StartSlot,
				WrapperView<ID3D11SamplerState> ppSamplers)
			{
				auto temp = ppSamplers.Get();
				CSSetSamplers(StartSlot, Span<const WrapperView<ID3D11SamplerState>>{ &temp, 1 });
			}

			void CSSetConstantBuffers(
				UINT StartSlot,
				Span<const WrapperView<ID3D11Buffer>> ppConstantBuffers)
			{
				Self().CSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void CSSetConstantBuffers(
				UINT StartSlot,
				WrapperView<ID3D11Buffer> ppConstantBuffers)
			{
				auto temp = ppConstantBuffers.Get();
				CSSetConstantBuffers(StartSlot, Span<const WrapperView<ID3D11Buffer>>{ &temp, 1 });
			}

			Vector<Wrapper<ID3D11ShaderResourceView>> CSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				Vector<Wrapper<ID3D11ShaderResourceView>> views;
				views.resize(NumViews);
				Self().CSGetShaderResources(StartSlot, NumViews, views.data());
				return views;
			}

			Vector<Wrapper<ID3D11UnorderedAccessView>> CSGetUnorderedAccessViews(
				UINT StartSlot,
				UINT NumViews)
			{
				Vector<Wrapper<ID3D11UnorderedAccessView>> views;
				views.resize(NumViews);
				Self().CSGetUnorderedAccessViews(StartSlot, NumViews, views.data());
				return views;
			}

			std::pair<Wrapper<ID3D11ComputeShader>, Vector<Wrapper<ID3D11ClassInstance>>> CSGetShader(UINT& numInstances)
			{
				std::pair<Wrapper<ID3D11ComputeShader>, Vector<Wrapper<ID3D11ClassInstance>>> output;
				output.second.resize(numInstances);
				Self().CSGetShader(OutPtr{ output.first }, output.second.data(), &numInstances);
				return output;
			}

			std::pair<Wrapper<ID3D11ComputeShader>, UINT> CSGetShader(Span<Wrapper<ID3D11ClassInstance>> classIntances)
			{
				std::pair<Wrapper<ID3D11ComputeShader>, UINT> output;
				output.second = classIntances.size();
				Self().CSGetShader(OutPtr{ output.first }, classIntances.data(), &output.second);
				return output;
			}

			Vector<Wrapper<ID3D11SamplerState>> CSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				Vector<Wrapper<ID3D11SamplerState>> samplers;
				samplers.resize(NumSamplers);
				Self().CSGetSamplers(StartSlot, NumSamplers, samplers.data());

				return samplers;
			}

			Vector<Wrapper<ID3D11Buffer>> CSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				Vector<Wrapper<ID3D11Buffer>> buffers;
				buffers.resize(NumBuffers);
				Self().CSGetConstantBuffers(StartSlot, NumBuffers, buffers.data());

				return buffers;
			}

		private:
			using InterfaceBase<UntaggedTraits<Derived>>::Self;
			using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
		};
	};
}