module;

#include "gsl/pointers"
#include <d3d11_4.h>
#include <tuple>
#include <optional>
#include <array>
#include <vector>
#include <utility>
#include <wrl/client.h>
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
	using Microsoft::WRL::ComPtr;
	export struct IAGetVertexBufferData
	{
		std::vector<Wrapper<ID3D11Buffer>> buffers;
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
		std::vector<Wrapper<ID3D11RenderTargetView>> renderTargetViews;
		Wrapper<ID3D11DepthStencilView> depthStencilView;
		std::vector<Wrapper<ID3D11UnorderedAccessView>> unorderedAccessViews;
	};

	export struct OMGetBlendStateData
	{
		Wrapper<ID3D11BlendState> blendState;
		std::array<FLOAT, 4> blendFactor;
		UINT sampleMask;
	};

	class IASetVertexBufferData 
	{
		UINT count;
		ID3D11Buffer* const* bufferView;
		const UINT* strideView;
		const UINT* offsetView;
	public:
		template<UINT Count>
		IASetVertexBufferData(std::span<ID3D11Buffer*, Count> buffers, std::span<UINT, Count> strides, std::span<UINT, Count> offsets) :
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

		template<UINT Count>
		IASetVertexBufferData(Span<Wrapper<ID3D11Buffer>, Count> buffers, std::span<UINT, Count> strides, std::span<UINT, Count> offsets) :
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

		IASetVertexBufferData(UINT count, ID3D11Buffer*const * buffers, const UINT* strides, const UINT* offsets) :
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

	class SOSetTargetsData
	{
		UINT count;
		ID3D11Buffer* const* bufferView;
		const UINT* offsetView;
	public:
		template<UINT Count>
		SOSetTargetsData(std::span<ID3D11Buffer*, Count> buffers, std::span<UINT, Count> offsets) :
			count{ buffers.size() },
			bufferView{ buffers.data() },
			offsetView{ offsets.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == offsets.size());
			}
		}

		template<UINT Count>
		SOSetTargetsData(Span<Wrapper<ID3D11Buffer>, Count> buffers, std::span<UINT, Count> offsets) :
			count{ buffers.size() },
			bufferView{ buffers.data() },
			offsetView{ offsets.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == offsets.size());
			}
		}

		SOSetTargetsData(UINT count, ID3D11Buffer*const * buffers, const UINT* offsets) :
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

	class CSSetUnorderedAccessViewsData
	{
		UINT count;
		ID3D11Buffer* const* bufferView;
		const UINT* pUAVInitialCounts;
	public:
		template<UINT Count>
		CSSetUnorderedAccessViewsData(std::span<ID3D11Buffer*, Count> buffers, std::span<UINT, Count> pUAVInitialCounts) :
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
		CSSetUnorderedAccessViewsData(Span<Wrapper<ID3D11Buffer>, Count> buffers, std::span<UINT, Count> pUAVInitialCounts) :
			count{ buffers.size() },
			bufferView{ buffers.data() },
			pUAVInitialCounts{ pUAVInitialCounts.data() }
		{
			if constexpr(Count == std::dynamic_extent)
			{
				assert(buffers.size() == pUAVInitialCounts.size());
			}
		}

		CSSetUnorderedAccessViewsData(UINT count, ID3D11Buffer* const* buffers, const UINT* pUAVInitialCounts) :
			count{ count },
			bufferView{ buffers },
			pUAVInitialCounts{ pUAVInitialCounts }
		{
		}

	public:
		UINT GetCount() const noexcept { return count; }
		ID3D11Buffer* const* GetBuffers() const noexcept { return bufferView; }
		const UINT* GetInitialCounts() const noexcept { return pUAVInitialCounts; }
	};

	template<class Ty>
	struct D3D11DeviceContextTraits;

	template<>
	struct D3D11DeviceContextTraits<ID3D11DeviceContext>
	{
		using value_type = ID3D11DeviceContext;
		using pointer = ID3D11DeviceContext*;
		using const_pointer = const ID3D11DeviceContext*;
		using reference = ID3D11DeviceContext&;
		using const_reference = const ID3D11DeviceContext&;

		template<class DerivedSelf>
		class Interface : public DeviceChildInterface<DerivedSelf>
		{
		private:
			using derived_self = DerivedSelf;

		public:
			void VSSetConstantBuffers(
				UINT StartSlot,
				Span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
			{
				Get().VSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void VSSetConstantBuffers(
				UINT StartSlot,
				Wrapper<ID3D11Buffer> ppConstantBuffers)
			{
				VSSetConstantBuffers(StartSlot, Span<Wrapper<ID3D11Buffer>>{ ppConstantBuffers.GetAddressOf(), 1 });
			}

			void PSSetShaderResources(
				UINT StartSlot,
				Span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Get().PSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void PSSetShaderResources(
				UINT StartSlot,
				Wrapper<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				PSSetShaderResources(StartSlot, Span<Wrapper<ID3D11ShaderResourceView>>{ ppShaderResourceViews.GetAddressOf(), 1 });
			}

			void PSSetShader(
				Wrapper<ID3D11PixelShader> optPixelShader,
				std::span<ID3D11ClassInstance*> optClassInstances)
			{
				Get().PSSetShader(optPixelShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void PSSetShader(
				Wrapper<ID3D11PixelShader> optPixelShader,
				ID3D11ClassInstance* optClassInstances)
			{
				PSSetShader(optPixelShader.Get(), std::span{ &optClassInstances, optClassInstances ? size_t{1} : size_t{0} });
			}

			void PSSetSamplers(
				UINT StartSlot,
				Span<Wrapper<ID3D11SamplerState>> ppSamplers)
			{
				Get().PSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void PSSetSamplers(
				UINT StartSlot,
				Wrapper<ID3D11SamplerState> ppSamplers)
			{
				PSSetSamplers(StartSlot, Span<Wrapper<ID3D11SamplerState>>{ ppSamplers.GetAddressOf(), 1 });
			}

			void VSSetShader(
				Wrapper<ID3D11VertexShader> optVertexShader,
				std::span<ID3D11ClassInstance*> optClassInstances)
			{
				Get().VSSetShader(optVertexShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void VSSetShader(
				Wrapper<ID3D11VertexShader> optVertexShader,
				ID3D11ClassInstance* optClassInstances)
			{
				VSSetShader(optVertexShader.Get(), std::span{ &optClassInstances, optClassInstances ? size_t{1} : size_t{0} });
			}

			void DrawIndexed(
				UINT IndexCount,
				UINT StartIndexLocation,
				INT BaseVertexLocation)
			{
				Get().DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
			}

			void Draw(
				UINT VertexCount,
				UINT StartVertexLocation)
			{
				Get().Draw(VertexCount, StartVertexLocation);
			}

			D3D11_MAPPED_SUBRESOURCE Map(
				gsl::not_null<Wrapper<ID3D11Resource>> pResource,
				UINT Subresource,
				D3D11_MAP MapType,
				UINT MapFlags)
			{
				D3D11_MAPPED_SUBRESOURCE subresource;

				ThrowIfFailed(Get().Map(pResource.get().Get(), Subresource, MapType, MapFlags, &subresource));
				return subresource;
			}

			void Unmap(
				gsl::not_null<Wrapper<ID3D11Resource>> pResource,
				UINT Subresource)
			{
				Get().Unmap(pResource.get().Get(), Subresource);
			}

			void PSSetConstantBuffers(
				UINT StartSlot,
				Span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
			{
				Get().PSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void PSSetConstantBuffers(
				UINT StartSlot,
				Wrapper<ID3D11Buffer> ppConstantBuffers)
			{
				PSSetConstantBuffers(StartSlot, Span<Wrapper<ID3D11Buffer>>{ ppConstantBuffers.GetAddressOf(), 1 });
			}

			void IASetInputLayout(
				Wrapper<ID3D11InputLayout> pInputLayout)
			{
				Get().IASetInputLayout(pInputLayout.Get());
			}

			void IASetVertexBuffers(
				UINT StartSlot,
				ID3D11Buffer* buffer,
				UINT stride,
				UINT offset)
			{
				IASetVertexBuffers(
					StartSlot,
					{1, &buffer, &stride, &offset});
			}

			void IASetVertexBuffers(
				UINT StartSlot,
				Wrapper<ID3D11Buffer> buffer,
				UINT stride,
				UINT offset)
			{
				IASetVertexBuffers(
					StartSlot,
					{1, buffer.GetAddressOf(), &stride, &offset});
			}

			void IASetVertexBuffers(
				UINT StartSlot,
				IASetVertexBufferData data)
			{
				Get().IASetVertexBuffers(
					StartSlot,
					data.GetCount(),
					data.GetBuffers(),
					data.GetStrides(),
					data.GetOffsets());
			}

			void IASetIndexBuffer(
				Wrapper<ID3D11Buffer> pIndexBuffer,
				DXGI_FORMAT Format,
				UINT Offset)
			{
				Get().IASetIndexBuffer(pIndexBuffer.Get(), Format, Offset);
			}

			void DrawIndexedInstanced(
				UINT IndexCountPerInstance,
				UINT InstanceCount,
				UINT StartIndexLocation,
				INT BaseVertexLocation,
				UINT StartInstanceLocation)
			{
				Get().DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
			}

			void DrawInstanced(
				UINT VertexCountPerInstance,
				UINT InstanceCount,
				UINT StartVertexLocation,
				UINT StartInstanceLocation)
			{
				Get().DrawIndexedInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
			}

			void GSSetConstantBuffers(
				UINT StartSlot,
				Span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
			{
				Get().GSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void GSSetConstantBuffers(
				UINT StartSlot,
				Wrapper<ID3D11Buffer> ppConstantBuffers)
			{
				GSSetConstantBuffers(StartSlot, Span<Wrapper<ID3D11Buffer>>{ ppConstantBuffers.GetAddressOf(), 1 });
			}

			void GSSetShader(
				Wrapper<ID3D11GeometryShader> optGeometryShader,
				std::span<ID3D11ClassInstance*> optClassInstances)
			{
				Get().GSSetShader(optGeometryShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void GSSetShader(
				Wrapper<ID3D11GeometryShader> optGeometryShader,
				ID3D11ClassInstance* optClassInstances)
			{
				GSSetShader(optGeometryShader.Get(), std::span{ &optClassInstances, optClassInstances ? size_t{1} : size_t{0} });
			}

			void IASetPrimitiveTopology(
				D3D11_PRIMITIVE_TOPOLOGY Topology)
			{
				Get().IASetPrimitiveTopology(Topology);
			}

			void VSSetShaderResources(
				UINT StartSlot,
				Span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Get().VSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void VSSetShaderResources(
				UINT StartSlot,
				Wrapper<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				VSSetShaderResources(StartSlot, Span<Wrapper<ID3D11ShaderResourceView>>{ ppShaderResourceViews.GetAddressOf(), 1 });
			}

			void VSSetSamplers(
				UINT StartSlot,
				Span<Wrapper<ID3D11SamplerState>> ppSamplers)
			{
				Get().VSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void VSSetSamplers(
				UINT StartSlot,
				Wrapper<ID3D11SamplerState> ppSamplers)
			{
				VSSetSamplers(StartSlot, Span<Wrapper<ID3D11SamplerState>>{ ppSamplers.GetAddressOf(), 1 });
			}

			void Begin(
				ID3D11Asynchronous& pAsync)
			{
				Get().Begin(&pAsync);
			}

			void End(
				ID3D11Asynchronous& pAsync)
			{
				Get().End(&pAsync);
			}

			void* GetData(
				ID3D11Asynchronous& pAsync,
				UINT GetDataFlags)
			{
				void* data;

				ThrowIfFailed(Get().GetData(&pAsync, data, pAsync.GetDataSize(), GetDataFlags));

				return data;
			}

			void SetPredication(
				ID3D11Predicate* pPredicate,
				BOOL PredicateValue)
			{
				Get().SetPredication(pPredicate, PredicateValue);
			}

			void GSSetShaderResources(
				UINT StartSlot,
				Span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Get().GSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void GSSetShaderResources(
				UINT StartSlot,
				Wrapper<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				GSSetShaderResources(StartSlot, Span<Wrapper<ID3D11ShaderResourceView>>{ ppShaderResourceViews.GetAddressOf(), 1 });
			}

			void GSSetSamplers(
				UINT StartSlot,
				Span<Wrapper<ID3D11SamplerState>> ppSamplers)
			{
				Get().GSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void GSSetSamplers(
				UINT StartSlot,
				Wrapper<ID3D11SamplerState> ppSamplers)
			{
				GSSetSamplers(StartSlot, Span<Wrapper<ID3D11SamplerState>>{ ppSamplers.GetAddressOf(), 1 });
			}

			void OMSetRenderTargets(
				Span<Wrapper<ID3D11RenderTargetView>> optRenderTargetViews,
				Wrapper<ID3D11DepthStencilView> optDepthStencilView)
			{
				Get().OMSetRenderTargets(static_cast<UINT>(optRenderTargetViews.size()), optRenderTargetViews.data(), optDepthStencilView.Get());
			}

			void OMSetRenderTargets(
				Wrapper<ID3D11RenderTargetView> optRenderTargetViews,
				Wrapper<ID3D11DepthStencilView> optDepthStencilView)
			{
				OMSetRenderTargets(Span<Wrapper<ID3D11RenderTargetView>>{ optRenderTargetViews.GetAddressOf(), optRenderTargetViews.Get() ? size_t{ 1 } : size_t{ 0 } }, optDepthStencilView);
			}

			void OMSetRenderTargetsAndUnorderedAccessViews(
				Span<Wrapper<ID3D11RenderTargetView>> optRenderTargetViews,
				Wrapper<ID3D11DepthStencilView> optDepthStencilView,
				UINT UAVStartSlot,
				Span<Wrapper<ID3D11UnorderedAccessView>> ppUnorderedAccessViews,
				const UINT* pUAVInitialCounts)
			{
				Get().OMSetRenderTargetsAndUnorderedAccessViews(
					static_cast<UINT>(optRenderTargetViews.size()),
					optRenderTargetViews.data(),
					optDepthStencilView.Get(),
					UAVStartSlot,
					static_cast<UINT>(ppUnorderedAccessViews.size()),
					ppUnorderedAccessViews.data(),
					pUAVInitialCounts);
			}

			void OMSetBlendState(
				Wrapper<ID3D11BlendState> optBlendState,
				std::optional<std::span<const FLOAT, 4>> BlendFactor,
				UINT SampleMask)
			{
				if(BlendFactor.has_value())
					Get().OMSetBlendState(optBlendState.Get(), BlendFactor.value().data(), SampleMask);
				else
					Get().OMSetBlendState(optBlendState.Get(), nullptr, SampleMask);
			}

			void OMSetDepthStencilState(
				Wrapper<ID3D11DepthStencilState> optDepthStencilState,
				UINT StencilRef)
			{
				Get().OMSetDepthStencilState(optDepthStencilState.Get(), StencilRef);
			}

			void SOSetTargets(SOSetTargetsData ppSOTargets)
			{
				Get().SOSetTargets(ppSOTargets.GetCount(), ppSOTargets.GetBuffers(), ppSOTargets.GetOffsets());
			}

			void DrawAuto()
			{
				Get().DrawAuto();
			}

			void DrawIndexedInstancedIndirect(
				gsl::not_null<Wrapper<ID3D11Buffer>> pBufferForArgs,
				UINT AlignedByteOffsetForArgs)
			{
				Get().DrawIndexedInstancedIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
			}

			void DrawInstancedIndirect(
				gsl::not_null<Wrapper<ID3D11Buffer>> pBufferForArgs,
				UINT AlignedByteOffsetForArgs)
			{
				Get().DrawInstancedIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
			}

			void Dispatch(
				UINT ThreadGroupCountX,
				UINT ThreadGroupCountY,
				UINT ThreadGroupCountZ)
			{
				Get().Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
			}

			void DispatchIndirect(
				gsl::not_null<Wrapper<ID3D11Buffer>> pBufferForArgs,
				UINT AlignedByteOffsetForArgs)
			{
				Get().DispatchIndirect(pBufferForArgs.get().Get(), AlignedByteOffsetForArgs);
			}

			void RSSetState(
				Wrapper<ID3D11RasterizerState> optRasterizerState)
			{
				Get().RSSetState(optRasterizerState.Get());
			}

			void RSSetViewports(
				std::span<const D3D11_VIEWPORT> pViewports)
			{
				Get().RSSetViewports(static_cast<UINT>(pViewports.size()), pViewports.data());
			}

			void RSSetViewports(
				D3D11_VIEWPORT pViewports)
			{
				RSSetViewports(std::span{ &pViewports, 1 });
			}

			void RSSetScissorRects(
				std::span<const D3D11_RECT> pRects)
			{
				Get().RSSetScissorRects(static_cast<UINT>(pRects.size()), pRects.data());
			}

			void RSSetScissorRects(
				D3D11_RECT pRects)
			{
				RSSetScissorRects(std::span{ &pRects, 1 });
			}

			void CopySubresourceRegion(
				gsl::not_null<Wrapper<ID3D11Resource>> pDstResource,
				UINT DstSubresource,
				UINT DstX,
				UINT DstY,
				UINT DstZ,
				gsl::not_null<Wrapper<ID3D11Resource>>  pSrcResource,
				UINT SrcSubresource,
				const D3D11_BOX* pSrcBox)
			{
				Get().CopySubresourceRegion(
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
				gsl::not_null<Wrapper<ID3D11Resource>>  pDstResource,
				gsl::not_null<Wrapper<ID3D11Resource>>  pSrcResource)
			{
				Get().CopyResource(
					pDstResource.get().Get(),
					pSrcResource.get().Get());
			}

			void UpdateSubresource(
				gsl::not_null<Wrapper<ID3D11Resource>> pDstResource,
				UINT DstSubresource,
				const D3D11_BOX* pDstBox,
				const void* pSrcData,
				UINT SrcRowPitch,
				UINT SrcDepthPitch)
			{
				Get().UpdateSubresource(
					pDstResource.get().Get(),
					DstSubresource,
					pDstBox,
					pSrcData,
					SrcRowPitch,
					SrcDepthPitch);
			}

			void CopyStructureCount(
				gsl::not_null<Wrapper<ID3D11Buffer>> pDstBuffer,
				UINT DstAlignedByteOffset,
				gsl::not_null<Wrapper<ID3D11UnorderedAccessView>> pSrcView)
			{
				Get().CopyStructureCount(pDstBuffer.get().Get(), DstAlignedByteOffset, pSrcView.get().Get());
			}

			void ClearRenderTargetView(
				gsl::not_null<Wrapper<ID3D11RenderTargetView>> pRenderTargetView,
				std::span<const FLOAT, 4> ColorRGBA)
			{
				Get().ClearRenderTargetView(pRenderTargetView.get().Get(), ColorRGBA.data());
			}

			void ClearUnorderedAccessViewUint(
				gsl::not_null<Wrapper<ID3D11UnorderedAccessView>> pUnorderedAccessView,
				std::span<const UINT, 4> Values)
			{
				Get().ClearUnorderedAccessViewUint(pUnorderedAccessView.get().Get(), Values.data());
			}

			void ClearUnorderedAccessViewFloat(
				gsl::not_null<Wrapper<ID3D11UnorderedAccessView>> pUnorderedAccessView,
				std::span<const FLOAT, 4> Values)
			{
				Get().ClearUnorderedAccessViewFloat(pUnorderedAccessView.get().Get(), Values.data());
			}

			void ClearDepthStencilView(
				gsl::not_null<Wrapper<ID3D11DepthStencilView>> pDepthStencilView,
				UINT ClearFlags,
				FLOAT Depth,
				UINT8 Stencil)
			{
				Get().ClearDepthStencilView(pDepthStencilView.get().Get(), ClearFlags, Depth, Stencil);
			}

			void GenerateMips(
				gsl::not_null<Wrapper<ID3D11ShaderResourceView>> pShaderResourceView)
			{
				Get().GenerateMips(pShaderResourceView.get().Get());
			}

			void SetResourceMinLOD(
				gsl::not_null<Wrapper<ID3D11Resource>> pResource,
				FLOAT MinLOD)
			{
				Get().SetResourceMinLOD(pResource.get().Get(), MinLOD);
			}

			FLOAT GetResourceMinLOD(
				gsl::not_null<Wrapper<ID3D11Resource>> pResource)
			{
				return Get().GetResourceMinLOD(pResource.get().Get());
			}

			void ResolveSubresource(
				gsl::not_null<Wrapper<ID3D11Resource>> pDstResource,
				UINT DstSubresource,
				gsl::not_null<Wrapper<ID3D11Resource>> pSrcResource,
				UINT SrcSubresource,
				DXGI_FORMAT Format)
			{
				Get().ResolveSubresource(pDstResource.get().Get(), DstSubresource, pSrcResource.get().Get(), SrcSubresource, Format);
			}

			void ExecuteCommandList(
				ID3D11CommandList& pCommandList,
				BOOL RestoreContextState)
			{
				Get().ExecuteCommandList(&pCommandList, RestoreContextState);
			}

			void HSSetShaderResources(
				UINT StartSlot,
				Span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Get().HSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void HSSetShaderResources(
				UINT StartSlot,
				Wrapper<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				HSSetShaderResources(StartSlot, Span<Wrapper<ID3D11ShaderResourceView>>{ ppShaderResourceViews.GetAddressOf(), 1 });
			}

			void HSSetShader(
				Wrapper<ID3D11HullShader> optHullShader,
				std::span<ID3D11ClassInstance*> optClassInstances)
			{
				Get().HSSetShader(optHullShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void HSSetShader(
				Wrapper<ID3D11HullShader> optHullShader,
				ID3D11ClassInstance* optClassInstances)
			{
				HSSetShader(optHullShader.Get(), std::span{ &optClassInstances, optClassInstances ? size_t{1} : size_t{0} });
			}

			void HSSetSamplers(
				UINT StartSlot,
				Span<Wrapper<ID3D11SamplerState>> ppSamplers)
			{
				Get().HSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void HSSetSamplers(
				UINT StartSlot,
				Wrapper<ID3D11SamplerState> ppSamplers)
			{
				HSSetSamplers(StartSlot, Span<Wrapper<ID3D11SamplerState>>{ ppSamplers.GetAddressOf(), 1 });
			}

			void HSSetConstantBuffers(
				UINT StartSlot,
				Span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
			{
				Get().HSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void HSSetConstantBuffers(
				UINT StartSlot,
				Wrapper<ID3D11Buffer> ppConstantBuffers)
			{
				HSSetConstantBuffers(StartSlot, Span<Wrapper<ID3D11Buffer>>{ ppConstantBuffers.GetAddressOf(), 1 });
			}

			void DSSetShaderResources(
				UINT StartSlot,
				Span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Get().DSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void DSSetShaderResources(
				UINT StartSlot,
				Wrapper<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				DSSetShaderResources(StartSlot, Span<Wrapper<ID3D11ShaderResourceView>>{ ppShaderResourceViews.GetAddressOf(), 1 });
			}

			void DSSetShader(
				Wrapper<ID3D11DomainShader> optDomainShader,
				std::span<ID3D11ClassInstance*> optClassInstances)
			{
				Get().DSSetShader(optDomainShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void DSSetShader(
				Wrapper<ID3D11DomainShader> optDomainShader,
				ID3D11ClassInstance* optClassInstances)
			{
				DSSetShader(optDomainShader.Get(), std::span{ &optClassInstances, optClassInstances ? size_t{1} : size_t{0} });
			}

			void DSSetSamplers(
				UINT StartSlot,
				Span<Wrapper<ID3D11SamplerState>> ppSamplers)
			{
				Get().DSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void DSSetSamplers(
				UINT StartSlot,
				Wrapper<ID3D11SamplerState> ppSamplers)
			{
				DSSetSamplers(StartSlot, Span<Wrapper<ID3D11SamplerState>>{ ppSamplers.GetAddressOf(), 1 });
			}

			void DSSetConstantBuffers(
				UINT StartSlot,
				Span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
			{
				Get().DSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}
			
			void DSSetConstantBuffers(
				UINT StartSlot,
				Wrapper<ID3D11Buffer> ppConstantBuffers)
			{
				DSSetConstantBuffers(StartSlot, Span<Wrapper<ID3D11Buffer>>{ ppConstantBuffers.GetAddressOf(), 1 });
			}

			void CSSetShaderResources(
				UINT StartSlot,
				Span<Wrapper<ID3D11ShaderResourceView>> ppShaderResourceViews)
			{
				Get().CSSetShaderResources(StartSlot, static_cast<UINT>(ppShaderResourceViews.size()), ppShaderResourceViews.data());
			}

			void CSSetShaderResources(
				UINT StartSlot,
				Wrapper<ID3D11ShaderResourceView> ppShaderResourceViews)
			{
				CSSetShaderResources(StartSlot, Span<Wrapper<ID3D11ShaderResourceView>>{ ppShaderResourceViews.GetAddressOf(), 1 });
			}

			void CSSetUnorderedAccessViews(
				UINT StartSlot,
				CSSetUnorderedAccessViewsData data)
			{
				Get().CSSetUnorderedAccessViews(StartSlot, data.GetCount(), data.GetBuffers(), data.GetInitialCounts());
			}

			void CSSetUnorderedAccessViews(
				UINT StartSlot,
				ID3D11Buffer* data,
				UINT initialCounts)
			{
				CSSetUnorderedAccessViews(StartSlot, { 1, &data, &initialCounts });
			}

			void CSSetUnorderedAccessViews(
				UINT StartSlot,
				Wrapper<ID3D11Buffer> data,
				UINT initialCounts)
			{
				CSSetUnorderedAccessViews(StartSlot, { 1, data.GetAddressOf(), &initialCounts });
			}

			void CSSetShader(
				Wrapper<ID3D11ComputeShader> optComputeShader,
				std::span<ID3D11ClassInstance*> optClassInstances)
			{
				Get().CSSetShader(optComputeShader.Get(), optClassInstances.data(), static_cast<UINT>(optClassInstances.size()));
			}

			void CSSetShader(
				Wrapper<ID3D11ComputeShader> optComputeShader,
				ID3D11ClassInstance* optClassInstances)
			{
				CSSetShader(optComputeShader.Get(), std::span{ &optClassInstances, optClassInstances ? size_t{1} : size_t{0} });
			}

			void CSSetSamplers(
				UINT StartSlot,
				Span<Wrapper<ID3D11SamplerState>> ppSamplers)
			{
				Get().CSSetSamplers(StartSlot, static_cast<UINT>(ppSamplers.size()), ppSamplers.data());
			}

			void CSSetSamplers(
				UINT StartSlot,
				Wrapper<ID3D11SamplerState> ppSamplers)
			{
				CSSetSamplers(StartSlot, Span<Wrapper<ID3D11SamplerState>>{ ppSamplers.GetAddressOf(), 1 });
			}

			void CSSetConstantBuffers(
				UINT StartSlot,
				Span<Wrapper<ID3D11Buffer>> ppConstantBuffers)
			{
				Get().CSSetConstantBuffers(StartSlot, static_cast<UINT>(ppConstantBuffers.size()), ppConstantBuffers.data());
			}

			void CSSetConstantBuffers(
				UINT StartSlot,
				Wrapper<ID3D11Buffer> ppConstantBuffers)
			{
				CSSetConstantBuffers(StartSlot, Span<Wrapper<ID3D11Buffer>>{ ppConstantBuffers.GetAddressOf(), 1 });
			}

			std::vector<Wrapper<ID3D11Buffer>> VSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer * []>(NumBuffers);
				Get().VSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

				std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
				for(UINT i = 0; i < NumBuffers; i++)
				{
					buffers[i].Attach(tempBuffers[i]);
				}

				return buffers;
			}

			std::vector<Wrapper<ID3D11ShaderResourceView>> PSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView * []>(NumViews);
				Get().PSGetShaderResources(StartSlot, NumViews, tempViews.get());

				std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
				for(UINT i = 0; i < NumViews; i++)
				{
					views[i].Attach(tempViews[i]);
				}

				return views;
			}

			std::pair<Wrapper<ID3D11PixelShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> PSGetShader(UINT numInstances = 0)
			{
				std::pair<Wrapper<ID3D11PixelShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
				std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
				Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;

				Get().PSGetShader(&shader, tempInstances.data(), &numInstances);
				output.first.Attach(shader.Detach());
				output.second.resize(numInstances);
				for(UINT i = 0; i < numInstances; i++)
				{
					output.second[i].Attach(tempInstances[i]);
				}

				return output;
			}

			std::vector<Wrapper<ID3D11SamplerState>> PSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState * []>(NumSamplers - StartSlot);
				Get().PSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

				std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
				for(UINT i = 0; i < NumSamplers; i++)
				{
					samplers[i].Attach(tempSamplers[i]);
				}

				return samplers;
			}

			std::pair<Wrapper<ID3D11VertexShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> VSGetShader(UINT numInstances = 0)
			{
				std::pair<Wrapper<ID3D11VertexShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
				std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
				Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;

				Get().VSGetShader(&shader, tempInstances.data(), &numInstances);
				output.first.Attach(shader.Detach());
				output.second.resize(numInstances);
				for(UINT i = 0; i < numInstances; i++)
				{
					output.second[i].Attach(tempInstances[i]);
				}

				return output;
			}

			std::vector<Wrapper<ID3D11Buffer>> PSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer * []>(NumBuffers);
				Get().PSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

				std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
				for(UINT i = 0; i < NumBuffers; i++)
				{
					buffers[i].Attach(tempBuffers[i]);
				}

				return buffers;
			}

			Wrapper<ID3D11InputLayout> IAGetInputLayout()
			{
				Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
				Get().IAGetInputLayout(&layout);
				return Wrapper<ID3D11InputLayout>(std::move(layout));
			}

			IAGetVertexBufferData IAGetVertexBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer * []>(NumBuffers);
				std::unique_ptr<UINT[]> tempStrides = std::make_unique<UINT[]>(NumBuffers);
				std::unique_ptr<UINT[]> tempOffsets = std::make_unique<UINT[]>(NumBuffers);
				Get().IAGetVertexBuffers(StartSlot, NumBuffers, tempBuffers.get(), tempStrides.get(), tempOffsets.get());

				IAGetVertexBufferData output;
				output.buffers.resize(NumBuffers);
				output.strides.resize(NumBuffers);
				output.offsets.resize(NumBuffers);
				for(UINT i = 0; i < NumBuffers; i++)
				{
					output.buffers[i].Attach(tempBuffers[i]);
				}
				std::copy(&tempStrides[0], &tempStrides[NumBuffers], output.strides.begin());
				std::copy(&tempOffsets[0], &tempOffsets[NumBuffers], output.offsets.begin());

				return output;
			}

			IAGetIndexBufferData IAGetIndexBuffer()
			{
				IAGetIndexBufferData output;
				Microsoft::WRL::ComPtr<ID3D11Buffer> temp;
				Get().IAGetIndexBuffer(&temp, &output.format, &output.offset);
				output.buffer.Attach(temp.Detach());
				return output;
			}

			std::vector<Wrapper<ID3D11Buffer>> GSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer * []>(NumBuffers);
				Get().GSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

				std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
				for(UINT i = 0; i < NumBuffers; i++)
				{
					buffers[i].Attach(tempBuffers[i]);
				}

				return buffers;
			}

			std::pair<Wrapper<ID3D11GeometryShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> GSGetShader(UINT numInstances = 0)
			{
				std::pair<Wrapper<ID3D11GeometryShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
				std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
				Microsoft::WRL::ComPtr<ID3D11GeometryShader> shader;

				Get().GSGetShader(&shader, tempInstances.data(), &numInstances);
				output.first.Attach(shader.Detach());
				output.second.resize(numInstances);
				for(UINT i = 0; i < numInstances; i++)
				{
					output.second[i].Attach(tempInstances[i]);
				}

				return output;
			}

			D3D11_PRIMITIVE_TOPOLOGY IAGetPrimitiveTopology()
			{
				D3D11_PRIMITIVE_TOPOLOGY topology;
				Get().IAGetPrimitiveTopology(&topology);
				return topology;
			}

			std::vector<Wrapper<ID3D11ShaderResourceView>> VSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView * []>(NumViews);
				Get().VSGetShaderResources(StartSlot, NumViews, tempViews.get());

				std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
				for(UINT i = 0; i < NumViews; i++)
				{
					views[i].Attach(tempViews[i]);
				}

				return views;
			}

			std::vector<Wrapper<ID3D11SamplerState>> VSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState * []>(NumSamplers - StartSlot);
				Get().VSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

				std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
				for(UINT i = 0; i < NumSamplers; i++)
				{
					samplers[i].Attach(tempSamplers[i]);
				}

				return samplers;
			}

			std::pair<Microsoft::WRL::ComPtr<ID3D11Predicate>, BOOL> GetPredication()
			{
				std::pair<Microsoft::WRL::ComPtr<ID3D11Predicate>, BOOL> output;
				Get().GetPredication(&output.first, &output.second);
				return output;
			}

			std::vector<Wrapper<ID3D11ShaderResourceView>> GSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView * []>(NumViews);
				Get().GSGetShaderResources(StartSlot, NumViews, tempViews.get());

				std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
				for(UINT i = 0; i < NumViews; i++)
				{
					views[i].Attach(tempViews[i]);
				}

				return views;
			}

			std::vector<Wrapper<ID3D11SamplerState>> GSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState * []>(NumSamplers - StartSlot);
				Get().GSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

				std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
				for(UINT i = 0; i < NumSamplers; i++)
				{
					samplers[i].Attach(tempSamplers[i]);
				}

				return samplers;
			}

			std::pair<std::vector<Wrapper<ID3D11RenderTargetView>>, Wrapper<ID3D11DepthStencilView>>  OMGetRenderTargets(
				UINT NumViews)
			{
				std::unique_ptr<ID3D11RenderTargetView* []> tempViews = std::make_unique<ID3D11RenderTargetView * []>(NumViews);
				std::pair<std::vector<Wrapper<ID3D11RenderTargetView>>, Wrapper<ID3D11DepthStencilView>> output;
				Microsoft::WRL::ComPtr<ID3D11DepthStencilView> tempDS;
				Get().OMGetRenderTargets(NumViews, tempViews.get(), &tempDS);

				output.second = std::move(tempDS);

				for(UINT i = 0; i < NumViews; i++)
				{
					Microsoft::WRL::ComPtr<ID3D11RenderTargetView> tempRTV;
					tempRTV.Attach(tempViews[i]);
					output.first.push_back();
					output.first.back() = std::move(tempRTV);
				}

				return output;
			}

			OMGetRenderTargetsAndUnorderedAccessViewsData OMGetRenderTargetsAndUnorderedAccessViews(
				UINT NumRTVs,
				UINT UAVStartSlot,
				UINT NumUAVs)
			{
				std::unique_ptr<ID3D11RenderTargetView* []> tempRTVs = std::make_unique<ID3D11RenderTargetView * []>(NumRTVs);
				std::unique_ptr<ID3D11UnorderedAccessView* []> tempUAVs = std::make_unique<ID3D11UnorderedAccessView * []>(NumUAVs);

				OMGetRenderTargetsAndUnorderedAccessViewsData data;

				data.renderTargetViews.resize(NumRTVs);
				data.unorderedAccessViews.resize(NumUAVs);

				Get().OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, tempRTVs.get(), &data.depthStencilView, UAVStartSlot, NumUAVs, tempUAVs.get());

				for(UINT i = 0; i < NumRTVs; i++)
				{
					data.renderTargetViews[i].Attach(tempRTVs[i]);
				}
				for(UINT i = 0; i < NumUAVs; i++)
				{
					data.unorderedAccessViews[i].Attach(tempUAVs[i]);
				}
				return data;
			}

			OMGetBlendStateData OMGetBlendState()
			{
				OMGetBlendStateData data;
				Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;
				Get().OMGetBlendState(&blendState, data.blendFactor.data(), &data.sampleMask);
				data.blendState.Attach(blendState.Detach());
				return data;
			}

			std::pair<Wrapper<ID3D11DepthStencilState>, UINT> OMGetDepthStencilState()
			{
				std::pair<Wrapper<ID3D11DepthStencilState>, UINT> state;
				ID3D11DepthStencilState* temp;
				Get().OMGetDepthStencilState(&temp, &state.second);
				state.first.Attach(temp);
				return state;

			}

			std::vector<Wrapper<ID3D11Buffer>> SOGetTargets(UINT NumBuffers)
			{
				std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer * []>(NumBuffers);
				Get().SOGetTargets(NumBuffers, tempBuffers.get());

				std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
				for(UINT i = 0; i < NumBuffers; i++)
				{
					buffers[i].Attach(tempBuffers[i]);
				}

				return buffers;
			}

			Wrapper<ID3D11RasterizerState> RSGetState()
			{
				Microsoft::WRL::ComPtr<ID3D11RasterizerState> state;
				Get().RSGetState(&state);
				return Wrapper<ID3D11RasterizerState>(std::move(state));
			}

			std::vector<D3D11_VIEWPORT> RSGetViewports()
			{
				UINT numViewports;
				Get().RSGetViewports(&numViewports, nullptr);

				std::vector<D3D11_VIEWPORT> output(numViewports);
				Get().RSGetViewports(&numViewports, output.data());

				return output;
			}

			std::vector<D3D11_RECT> RSGetScissorRects()
			{
				UINT numRects;
				Get().RSGetScissorRects(&numRects, nullptr);

				std::vector<D3D11_RECT> output(numRects);
				Get().RSGetScissorRects(&numRects, output.data());

				return output;
			}

			std::vector<Wrapper<ID3D11ShaderResourceView>> HSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView * []>(NumViews);
				Get().HSGetShaderResources(StartSlot, NumViews, tempViews.get());

				std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
				for(UINT i = 0; i < NumViews; i++)
				{
					views[i].Attach(tempViews[i]);
				}

				return views;
			}

			std::pair<Wrapper<ID3D11HullShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> HSGetShader(UINT numInstances = 0)
			{
				std::pair<Wrapper<ID3D11HullShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
				std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
				Microsoft::WRL::ComPtr<ID3D11HullShader> shader;

				Get().HSGetShader(&shader, tempInstances.data(), &numInstances);
				output.first.Attach(shader.Detach());
				output.second.resize(numInstances);
				for(UINT i = 0; i < numInstances; i++)
				{
					output.second[i].Attach(tempInstances[i]);
				}

				return output;
			}

			std::vector<Wrapper<ID3D11SamplerState>> HSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState * []>(NumSamplers - StartSlot);
				Get().HSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

				std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
				for(UINT i = 0; i < NumSamplers; i++)
				{
					samplers[i].Attach(tempSamplers[i]);
				}

				return samplers;
			}

			std::vector<Wrapper<ID3D11Buffer>> HSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer * []>(NumBuffers);
				Get().HSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

				std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
				for(UINT i = 0; i < NumBuffers; i++)
				{
					buffers[i].Attach(tempBuffers[i]);
				}

				return buffers;
			}

			std::vector<Wrapper<ID3D11ShaderResourceView>> DSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView * []>(NumViews);
				Get().DSGetShaderResources(StartSlot, NumViews, tempViews.get());

				std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
				for(UINT i = 0; i < NumViews; i++)
				{
					views[i].Attach(tempViews[i]);
				}

				return views;
			}

			std::pair<Wrapper<ID3D11DomainShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> DSGetShader(UINT numInstances = 0)
			{
				std::pair<Wrapper<ID3D11DomainShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
				std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
				Microsoft::WRL::ComPtr<ID3D11DomainShader> shader;

				Get().DSGetShader(&shader, tempInstances.data(), &numInstances);
				output.first.Attach(shader.Detach());
				output.second.resize(numInstances);
				for(UINT i = 0; i < numInstances; i++)
				{
					output.second[i].Attach(tempInstances[i]);
				}

				return output;
			}

			std::vector<Wrapper<ID3D11SamplerState>> DSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState * []>(NumSamplers - StartSlot);
				Get().DSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

				std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
				for(UINT i = 0; i < NumSamplers; i++)
				{
					samplers[i].Attach(tempSamplers[i]);
				}

				return samplers;
			}

			std::vector<Wrapper<ID3D11Buffer>> DSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer * []>(NumBuffers);
				Get().DSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

				std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
				for(UINT i = 0; i < NumBuffers; i++)
				{
					buffers[i].Attach(tempBuffers[i]);
				}

				return buffers;
			}

			std::vector<Wrapper<ID3D11ShaderResourceView>> CSGetShaderResources(
				UINT StartSlot,
				UINT NumViews)
			{
				std::unique_ptr<ID3D11ShaderResourceView* []> tempViews = std::make_unique<ID3D11ShaderResourceView * []>(NumViews);
				Get().CSGetShaderResources(StartSlot, NumViews, tempViews.get());

				std::vector<Wrapper<ID3D11ShaderResourceView>> views(NumViews);
				for(UINT i = 0; i < NumViews; i++)
				{
					views[i].Attach(tempViews[i]);
				}

				return views;
			}

			std::vector<Wrapper<ID3D11UnorderedAccessView>> CSGetUnorderedAccessViews(
				UINT StartSlot,
				UINT NumViews)
			{
				std::unique_ptr<ID3D11UnorderedAccessView* []> tempViews = std::make_unique<ID3D11UnorderedAccessView * []>(NumViews);
				Get().CSGetUnorderedAccessViews(StartSlot, NumViews, tempViews.get());

				std::vector<Wrapper<ID3D11UnorderedAccessView>> views(NumViews);
				for(UINT i = 0; i < NumViews; i++)
				{
					views[i].Attach(tempViews[i]);
				}

				return views;
			}

			std::pair<Wrapper<ID3D11ComputeShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> CSGetShader(UINT numInstances = 0)
			{
				std::pair<Wrapper<ID3D11ComputeShader>, std::vector<Microsoft::WRL::ComPtr<ID3D11ClassInstance>>> output;
				std::array<ID3D11ClassInstance*, TypedD3D::D3D11::Constants::Shader::g_maxClassInstances> tempInstances;
				Microsoft::WRL::ComPtr<ID3D11ComputeShader> shader;

				Get().CSGetShader(&shader, tempInstances.data(), &numInstances);
				output.first.Attach(shader.Detach());
				output.second.resize(numInstances);
				for(UINT i = 0; i < numInstances; i++)
				{
					output.second[i].Attach(tempInstances[i]);
				}

				return output;
			}

			std::vector<Wrapper<ID3D11SamplerState>> CSGetSamplers(
				UINT StartSlot,
				UINT NumSamplers)
			{
				std::unique_ptr<ID3D11SamplerState* []> tempSamplers = std::make_unique<ID3D11SamplerState * []>(NumSamplers - StartSlot);
				Get().CSGetSamplers(StartSlot, NumSamplers, tempSamplers.get());

				std::vector<Wrapper<ID3D11SamplerState>> samplers(NumSamplers - StartSlot);
				for(UINT i = 0; i < NumSamplers; i++)
				{
					samplers[i].Attach(tempSamplers[i]);
				}

				return samplers;
			}

			std::vector<Wrapper<ID3D11Buffer>> CSGetConstantBuffers(
				UINT StartSlot,
				UINT NumBuffers)
			{
				std::unique_ptr<ID3D11Buffer* []> tempBuffers = std::make_unique<ID3D11Buffer * []>(NumBuffers);
				Get().CSGetConstantBuffers(StartSlot, NumBuffers, tempBuffers.get());

				std::vector<Wrapper<ID3D11Buffer>> buffers(NumBuffers);
				for(UINT i = 0; i < NumBuffers; i++)
				{
					buffers[i].Attach(tempBuffers[i]);
				}

				return buffers;
			}

			void ClearState()
			{
				return Get().ClearState();
			}

			void Flush()
			{
				return Get().Flush();
			}

			D3D11_DEVICE_CONTEXT_TYPE GetType()
			{
				return Get().GetType();
			}

			UINT GetContextFlags()
			{
				return Get().GetContextFlags();
			}

			ComPtr<ID3D11CommandList> FinishCommandList(BOOL RestoreDeferredContextState)
			{
				ComPtr<ID3D11CommandList> commandList;
				ThrowIfFailed(Get().FinishCommandList(RestoreDeferredContextState, &commandList));
				return commandList;
			}

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	template<std::derived_from<ID3D11DeviceContext> Ty>
	struct UntaggedTraits<Ty> : D3D11DeviceContextTraits<Ty>
	{
	};

	export template<std::derived_from<ID3D11DeviceContext> DeviceContextTy>
	using DeviceContext_t = IUnknownWrapper<DeviceContextTy, UntaggedTraits>;

	namespace Aliases
	{
		export using DeviceContext = DeviceContext_t<ID3D11DeviceContext>;
	}
}