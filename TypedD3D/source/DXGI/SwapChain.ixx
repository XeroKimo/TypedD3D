module;

#include <memory>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <span>
#include <d3d12.h>
#include <wrl/client.h>
export module TypedDXGI:SwapChain;
struct ID3D12CommandQueue;
import TypedD3D.Shared;
import TypedD3D12;

struct ID3D11Resource;
struct ID3D12Resource;

template<class Ty>
concept Resource = std::derived_from<Ty, ID3D11Resource> || std::derived_from<Ty, ID3D12Resource>;

namespace TypedD3D::DXGI
{
	using Microsoft::WRL::ComPtr;

	template<class Ty>
	using SwapChain_t = IUnknownWrapper<Ty, UntaggedTraits>;

	namespace Aliases
	{
		export using SwapChain = SwapChain_t<IDXGISwapChain>;
		export using SwapChain1 = SwapChain_t<IDXGISwapChain1>;
	}

	template<class Ty>
	struct SwapChainTraits;

	template<>
	struct SwapChainTraits<IDXGISwapChain>
	{
		using value_type = IDXGISwapChain;
		using pointer = IDXGISwapChain*;
		using const_pointer = const IDXGISwapChain*;
		using reference = IDXGISwapChain&;
		using cosnt_reference = const IDXGISwapChain&;

		template<class DerivedSelf>
		class Interface
		{
		private:
			using derived_self = DerivedSelf;

		public:
			void Present(UINT SyncInterval, UINT Flags)
			{
				ThrowIfFailed(Get().Present(SyncInterval, Flags));
			}

			template<Resource Ty>
			Wrapper<Ty> GetBuffer(UINT buffer)
			{
				return IIDToObjectForwardFunction<Ty>(&value_type::GetBuffer, Get(), buffer);
			}

			void SetFullscreenState(BOOL Fullscreen, IDXGIOutput* optTarget)
			{
				ThrowIfFailed(Get().SetFullscreenState(Fullscreen, optTarget));
			}

			std::pair<BOOL, ComPtr<IDXGIOutput>> GetFullscreenState()
			{
				std::pair<BOOL, ComPtr<IDXGIOutput>> state;
				Get().GetFullscreenState(&state.first, &state.second);
				return state;
			}

			DXGI_SWAP_CHAIN_DESC GetDesc()
			{
				DXGI_SWAP_CHAIN_DESC desc;
				Get().GetDesc(&desc);
				return desc;
			}

			void ResizeBuffers(
				UINT BufferCount,
				UINT Width,
				UINT Height,
				DXGI_FORMAT NewFormat,
				UINT SwapChainFlags)
			{
				ThrowIfFailed(Get().ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags));
			}

			void ResizeTarget(const DXGI_MODE_DESC& pNewTargetParameters)
			{
				ThrowIfFailed(Get().ResizeTarget(&pNewTargetParameters));
			}

			ComPtr<IDXGIOutput> GetContainingOutput()
			{
				return UnknownObjectForwardFunction<IDXGIOutput>(&value_type::GetContainingOutput, Get()).value();
			}

			DXGI_FRAME_STATISTICS GetFrameStatistics()
			{
				DXGI_FRAME_STATISTICS stats;
				Get().GetFrameStatistics(stats);
				return stats;
			}

			UINT GetLastPresentCount()
			{
				UINT LastPresentCount;
				Get().GetLastPresentCount(&LastPresentCount);
				return LastPresentCount;
			}

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	template<>
	struct SwapChainTraits<IDXGISwapChain1>
	{
		using value_type = IDXGISwapChain1;
		using pointer = IDXGISwapChain1*;
		using const_pointer = const IDXGISwapChain1*;
		using reference = IDXGISwapChain1&;
		using cosnt_reference = const IDXGISwapChain1&;

		template<class DerivedSelf>
		class Interface : public SwapChainTraits<IDXGISwapChain>::Interface<DerivedSelf>
		{
		private:
			using derived_self = DerivedSelf;
		public:
			DXGI_SWAP_CHAIN_DESC1 GetDesc1()
			{
				DXGI_SWAP_CHAIN_DESC1 desc;
				Get().GetDesc1(&desc);
				return desc;
			}

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC GetFullscreenDesc()
			{
				DXGI_SWAP_CHAIN_FULLSCREEN_DESC desc;
				HRESULT result = Get().GetFullscreenDesc(&desc);
				return desc;
			}

			HWND GetHwnd()
			{
				HWND hwnd;
				Get().GetHwnd(&hwnd);
				return hwnd;
			}

			void GetCoreWindow(REFIID refiid, void** ppUnk)
			{
				ThrowIfFailed(Get().GetCoreWindow(refiid, ppUnk));
			}

			void Present1(UINT SyncInterval, UINT PresentFlags, const DXGI_PRESENT_PARAMETERS& pPresentParameters)
			{
				ThrowIfFailed(Get().Present1(SyncInterval, PresentFlags, &pPresentParameters));
			}

			BOOL IsTemporaryMonoSupported()
			{
				return Get().IsTemporaryMonoSupported();
			}

			ComPtr<IDXGIOutput> GetRestrictToOutput()
			{
				return UnknownObjectForwardFunction<IDXGIOutput>(&value_type::GetRestrictToOutput, Get()).value();
			}

			void SetBackgroundColor(DXGI_RGBA pColor)
			{
				return ThrowIfFailed(Get().SetBackgroundColor(&pColor));
			}

			DXGI_RGBA GetBackgroundColor(DXGI_RGBA* pColor)
			{
				DXGI_RGBA color;
				ThrowIfFailed(Get().GetBackgroundColor(&color));
				return color;
			}

			void SetRotation(DXGI_MODE_ROTATION Rotation)
			{
				ThrowIfFailed(Get().SetRotation(Rotation));
			}

			DXGI_MODE_ROTATION GetRotation()
			{
				DXGI_MODE_ROTATION rotation;
				ThrowIfFailed(Get().GetRotation(rotation));
				return rotation;
			}
		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	export struct SourceSize
	{
		UINT Width;
		UINT Height;
	};

	template<>
	struct SwapChainTraits<IDXGISwapChain2>
	{
		using value_type = IDXGISwapChain2;
		using pointer = IDXGISwapChain2*;
		using const_pointer = const IDXGISwapChain2*;
		using reference = IDXGISwapChain2&;
		using cosnt_reference = const IDXGISwapChain2&;

		template<class DerivedSelf>
		class Interface : public SwapChainTraits<IDXGISwapChain1>::Interface<DerivedSelf>
		{
		private:
			using derived_self = DerivedSelf;

		public:
			HRESULT SetSourceSize(SourceSize Size)
			{
				return Get().SetSourceSize(Size.Width, Size.Height);
			}

			SourceSize GetSourceSize()
			{
				SourceSize size;
				ThrowIfFailed(Get().GetSourceSize(&size.Width, &size.Height));
				return size;
			}

			void SetMaximumFrameLatency(UINT MaxLatency)
			{
				ThrowIfFailed(Get().SetMaximumFrameLatency(&MaxLatency));
			}

			UINT STDMETHODCALLTYPE GetMaximumFrameLatency()
			{
				UINT latency;
				ThrowIfFailed(Get().GetMaximumFrameLatency(&latency));
				return latency;
			}

			HANDLE GetFrameLatencyWaitableObject()
			{
				return Get().GetFrameLatencyWaitableObject();
			}

			void SetMatrixTransform(const DXGI_MATRIX_3X2_F& pMatrix)
			{
				ThrowIfFailed(Get().SetMatrixTransform(&pMatrix));
			}

			DXGI_MATRIX_3X2_F STDMETHODCALLTYPE GetMatrixTransform()
			{
				DXGI_MATRIX_3X2_F matrix;
				ThrowIfFailed(Get().GetMatrixTransform(&matrix));
				return matrix;
			}
		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	template<>
	struct SwapChainTraits<IDXGISwapChain3>
	{
		using value_type = IDXGISwapChain3;
		using pointer = IDXGISwapChain3*;
		using const_pointer = const IDXGISwapChain3*;
		using reference = IDXGISwapChain3&;
		using cosnt_reference = const IDXGISwapChain3&;

		template<class DerivedSelf>
		class Interface : public SwapChainTraits<IDXGISwapChain2>::Interface<DerivedSelf>
		{
		private:
			using derived_self = DerivedSelf;

		public:
			UINT GetCurrentBackBufferIndex()
			{
				return Get().GetCurrentBackBufferIndex();
			}

			DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG CheckColorSpaceSupport(DXGI_COLOR_SPACE_TYPE ColorSpace)
			{
				UINT colorSupport;
				Get().CheckColorSpaceSupport(ColorSpace, &colorSupport);
				return static_cast<DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG>(colorSupport);
			}

			void SetColorSpace1(DXGI_COLOR_SPACE_TYPE ColorSpace)
			{
				ThrowIfFailed(Get().SetColorSpace1(ColorSpace));
			}

			template<std::derived_from<ID3D12CommandQueue> QueueTy>
			void ResizeBuffers1(
				UINT BufferCount,
				UINT Width,
				UINT Height,
				DXGI_FORMAT Format,
				UINT SwapChainFlags,
				std::span<UINT> pCreationNodeMask,
				std::span<TypedD3D12::Direct<QueueTy>> ppPresentQueue)
			{
				std::unique_ptr<IUnknown[]> queues = std::make_unique<IUnknown[]>(ppPresentQueue.size());

				for(size_t i = 0; i < ppPresentQueue.size(); i++)
				{
					queues[i] = ppPresentQueue[i].Get();
				}

				ThrowIfFailed(Get().ResizeBuffers1(BufferCount, Width, Height, Format, SwapChainFlags, pCreationNodeMask.data(), queues.get()));
			}
		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};
}

namespace TypedD3D
{
	template<std::derived_from<IDXGISwapChain> Ty>
	struct UntaggedTraits<Ty>
	{
		using value_type = Ty;
		using pointer = Ty*;
		using const_pointer = const Ty*;
		using reference = Ty&;
		using cosnt_reference = const Ty&;

		template<class DerivedSelf>
		using Interface = DXGI::SwapChainTraits<Ty>::template Interface<DerivedSelf>;
	};
}
