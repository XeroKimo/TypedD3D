module;

#include <memory>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <span>
#include <d3d12.h>
#include <cassert>

export module TypedDXGI:SwapChain;
import TypedD3D.Shared;
import TypedD3D11;
import TypedD3D12;
import :DXGIObject;

template<class Ty>
concept Resource = std::derived_from<Ty, ID3D11Resource> || std::derived_from<Ty, ID3D12Resource>;

namespace TypedD3D::DXGI
{
}

namespace TypedD3D
{
	template<>
	struct Trait<Untagged<IDXGISwapChain>>
	{
		using inner_type = IDXGISwapChain;

		using inner_tag = IDXGISwapChain;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;
		template<class Derived>
		class Interface : public InterfaceBase<Untagged<Derived>>
		{
		public:
			void Present(UINT SyncInterval, UINT Flags)
			{
				ThrowIfFailed(Self().Present(SyncInterval, Flags));
			}

			template<Resource Ty>
			Wrapper<Ty> GetBuffer(UINT buffer)
			{
				return ForwardFunction<Wrapper<Ty>>(&inner_type::GetBuffer, Self(), buffer);
			}

			void SetFullscreenState(BOOL Fullscreen, IDXGIOutput* optTarget)
			{
				ThrowIfFailed(Self().SetFullscreenState(Fullscreen, optTarget));
			}

			std::pair<BOOL, Wrapper<IDXGIOutput>> GetFullscreenState()
			{
				std::pair<BOOL, Wrapper<IDXGIOutput>> state;
				Self().GetFullscreenState(&state.first, OutPtr{ state.second });
				return state;
			}

			DXGI_SWAP_CHAIN_DESC GetDesc()
			{
				DXGI_SWAP_CHAIN_DESC desc;
				Self().GetDesc(&desc);
				return desc;
			}

			void ResizeBuffers(
				UINT BufferCount,
				UINT Width,
				UINT Height,
				DXGI_FORMAT NewFormat,
				UINT SwapChainFlags)
			{
				ThrowIfFailed(Self().ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags));
			}

			void ResizeTarget(const DXGI_MODE_DESC& pNewTargetParameters)
			{
				ThrowIfFailed(Self().ResizeTarget(&pNewTargetParameters));
			}

			Wrapper<IDXGIOutput> GetContainingOutput()
			{
				return ForwardFunction<IDXGIOutput>(&inner_type::GetContainingOutput, Self());
			}

			DXGI_FRAME_STATISTICS GetFrameStatistics()
			{
				DXGI_FRAME_STATISTICS stats;
				Self().GetFrameStatistics(&stats);
				return stats;
			}

			UINT GetLastPresentCount()
			{
				UINT LastPresentCount;
				Self().GetLastPresentCount(&LastPresentCount);
				return LastPresentCount;
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<IDXGISwapChain1>>
	{
		using inner_type = IDXGISwapChain1;

		using inner_tag = IDXGISwapChain1;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;
		template<class Derived>
		class Interface : public Trait<Untagged<IDXGISwapChain>>::Interface<Derived>
		{
		public:
			DXGI_SWAP_CHAIN_DESC1 GetDesc1()
			{
				DXGI_SWAP_CHAIN_DESC1 desc;
				Self().GetDesc1(&desc);
				return desc;
			}

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC GetFullscreenDesc()
			{
				DXGI_SWAP_CHAIN_FULLSCREEN_DESC desc;
				HRESULT result = Self().GetFullscreenDesc(&desc);
				return desc;
			}

			HWND GetHwnd()
			{
				HWND hwnd;
				Self().GetHwnd(&hwnd);
				return hwnd;
			}

			void GetCoreWindow(REFIID refiid, void** ppUnk)
			{
				ThrowIfFailed(Self().GetCoreWindow(refiid, ppUnk));
			}

			void Present1(UINT SyncInterval, UINT PresentFlags, const DXGI_PRESENT_PARAMETERS& pPresentParameters)
			{
				ThrowIfFailed(Self().Present1(SyncInterval, PresentFlags, &pPresentParameters));
			}

			BOOL IsTemporaryMonoSupported()
			{
				return Self().IsTemporaryMonoSupported();
			}

			Wrapper<IDXGIOutput> GetRestrictToOutput()
			{
				return ForwardFunction<IDXGIOutput>(&inner_type::GetRestrictToOutput, Self()).value();
			}

			void SetBackgroundColor(DXGI_RGBA pColor)
			{
				return ThrowIfFailed(Self().SetBackgroundColor(&pColor));
			}

			DXGI_RGBA GetBackgroundColor(DXGI_RGBA* pColor)
			{
				DXGI_RGBA color;
				ThrowIfFailed(Self().GetBackgroundColor(&color));
				return color;
			}

			void SetRotation(DXGI_MODE_ROTATION Rotation)
			{
				ThrowIfFailed(Self().SetRotation(Rotation));
			}

			DXGI_MODE_ROTATION GetRotation()
			{
				DXGI_MODE_ROTATION rotation;
				ThrowIfFailed(Self().GetRotation(rotation));
				return rotation;
			}
		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	export struct SourceSize
	{
		UINT Width;
		UINT Height;
	};

	template<>
	struct Trait<Untagged<IDXGISwapChain2>>
	{
		using inner_type = IDXGISwapChain2;

		using inner_tag = IDXGISwapChain2;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;
		template<class Derived>
		class Interface : public Trait<Untagged<IDXGISwapChain1>>::Interface<Derived>
		{
		private:
			using derived_self = Derived;

		public:
			HRESULT SetSourceSize(SourceSize Size)
			{
				return Self().SetSourceSize(Size.Width, Size.Height);
			}

			SourceSize GetSourceSize()
			{
				SourceSize size;
				ThrowIfFailed(Self().GetSourceSize(&size.Width, &size.Height));
				return size;
			}

			void SetMaximumFrameLatency(UINT MaxLatency)
			{
				ThrowIfFailed(Self().SetMaximumFrameLatency(&MaxLatency));
			}

			UINT STDMETHODCALLTYPE GetMaximumFrameLatency()
			{
				UINT latency;
				ThrowIfFailed(Self().GetMaximumFrameLatency(&latency));
				return latency;
			}

			HANDLE GetFrameLatencyWaitableObject()
			{
				return Self().GetFrameLatencyWaitableObject();
			}

			void SetMatrixTransform(const DXGI_MATRIX_3X2_F& pMatrix)
			{
				ThrowIfFailed(Self().SetMatrixTransform(&pMatrix));
			}

			DXGI_MATRIX_3X2_F STDMETHODCALLTYPE GetMatrixTransform()
			{
				DXGI_MATRIX_3X2_F matrix;
				ThrowIfFailed(Self().GetMatrixTransform(&matrix));
				return matrix;
			}
		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};									   
	};

	template<>
	struct Trait<Untagged<IDXGISwapChain3>>
	{
		using inner_type = IDXGISwapChain3;

		using inner_tag = IDXGISwapChain3;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;


		template<class NewInner>
		using trait_template = Untagged<NewInner>;
		template<class Derived>
		class Interface : public Trait<Untagged<IDXGISwapChain2>>::Interface<Derived>
		{
		private:
			using derived_self = Derived;

		public:
			UINT GetCurrentBackBufferIndex()
			{
				return Self().GetCurrentBackBufferIndex();
			}

			DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG CheckColorSpaceSupport(DXGI_COLOR_SPACE_TYPE ColorSpace)
			{
				UINT colorSupport;
				Self().CheckColorSpaceSupport(ColorSpace, &colorSupport);
				return static_cast<DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG>(colorSupport);
			}

			void SetColorSpace1(DXGI_COLOR_SPACE_TYPE ColorSpace)
			{
				ThrowIfFailed(Self().SetColorSpace1(ColorSpace));
			}

			template<std::derived_from<ID3D12CommandQueue> QueueTy>
			void ResizeBuffers1(
				UINT BufferCount,
				UINT Width,
				UINT Height,
				DXGI_FORMAT Format,
				UINT SwapChainFlags,
				std::span<UINT> pCreationNodeMask,
				Span<const TypedD3D::DirectView<QueueTy>> ppPresentQueue)
			{
				assert(BufferCount == pCreationNodeMask.size() && BufferCount == ppPresentQueue.size());
				ThrowIfFailed(Self().ResizeBuffers1(BufferCount, Width, Height, Format, SwapChainFlags, pCreationNodeMask.data(), ppPresentQueue.data()));
			}
		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};
}
