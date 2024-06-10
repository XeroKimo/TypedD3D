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

namespace TypedD3D
{
	template<class Ty>
	using SwapChain_t = IUnknownWrapper<Ty, UntaggedTraits>;

	export using SwapChain = SwapChain_t<IDXGISwapChain>;
	export using SwapChain1 = SwapChain_t<IDXGISwapChain1>;


	template<class Ty>
	struct SwapChainTraitsImpl;

	template<>
	struct SwapChainTraitsImpl<IDXGISwapChain>
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
			HRESULT Present(UINT SyncInterval, UINT Flags)
			{
				return Get().Present(SyncInterval, Flags);
			}

			template<Resource Ty>
			expected<Wrapper<Ty>, HRESULT> GetBuffer(UINT buffer)
			{
				expected<Microsoft::WRL::ComPtr<Ty>, HRESULT> resource = IIDToObjectForwardFunction<Ty>(&value_type::GetBuffer, Get(), buffer);

				if(!resource.has_value())
					return unexpected(resource.error());

				return Wrapper<Ty>(resource.value());
			}

			HRESULT SetFullscreenState(BOOL Fullscreen, IDXGIOutput* optTarget)
			{
				return Get().SetFullscreenState(Fullscreen, optTarget);
			}

			std::pair<BOOL, Microsoft::WRL::ComPtr<IDXGIOutput>> GetFullscreenState()
			{
				std::pair<BOOL, Microsoft::WRL::ComPtr<IDXGIOutput>> state;
				Get().GetFullscreenState(&state.first, &state.second);
				return state;
			}

			DXGI_SWAP_CHAIN_DESC GetDesc()
			{
				DXGI_SWAP_CHAIN_DESC desc;
				Get().GetDesc(&desc);
				return desc;
			}

			HRESULT ResizeBuffers(
				UINT BufferCount,
				UINT Width,
				UINT Height,
				DXGI_FORMAT NewFormat,
				UINT SwapChainFlags)
			{
				return Get().ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
			}

			HRESULT ResizeTarget(const DXGI_MODE_DESC& pNewTargetParameters)
			{
				return Get().ResizeTarget(&pNewTargetParameters);
			}

			Microsoft::WRL::ComPtr<IDXGIOutput> GetContainingOutput()
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
	struct SwapChainTraitsImpl<IDXGISwapChain1>
	{
		using value_type = IDXGISwapChain1;
		using pointer = IDXGISwapChain1*;
		using const_pointer = const IDXGISwapChain1*;
		using reference = IDXGISwapChain1&;
		using cosnt_reference = const IDXGISwapChain1&;

		template<class DerivedSelf>
		class Interface : public SwapChainTraitsImpl<IDXGISwapChain>::Interface<DerivedSelf>
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

			HRESULT GetCoreWindow(REFIID refiid, void** ppUnk)
			{
				return Get().GetCoreWindow(refiid, ppUnk);
			}

			HRESULT Present1(UINT SyncInterval, UINT PresentFlags, const DXGI_PRESENT_PARAMETERS& pPresentParameters)
			{
				return Get().Present1(SyncInterval, PresentFlags, &pPresentParameters);
			}

			BOOL IsTemporaryMonoSupported()
			{
				return Get().IsTemporaryMonoSupported();
			}

			Microsoft::WRL::ComPtr<IDXGIOutput> GetRestrictToOutput()
			{
				return UnknownObjectForwardFunction<IDXGIOutput>(&value_type::GetRestrictToOutput, Get()).value();
			}
			HRESULT SetBackgroundColor(DXGI_RGBA pColor)
			{
				return Get().SetBackgroundColor(&pColor);
			}

			expected<DXGI_RGBA, HRESULT> GetBackgroundColor(DXGI_RGBA* pColor)
			{
				DXGI_RGBA color;
				HRESULT result = Get().GetBackgroundColor(&color);
				if(FAILED(result))
					return tl::unexpected(result);
				return color;
			}

			HRESULT SetRotation(DXGI_MODE_ROTATION Rotation)
			{
				return Get().SetRotation(Rotation);
			}

			expected<DXGI_MODE_ROTATION, HRESULT> GetRotation()
			{
				DXGI_MODE_ROTATION rotation;
				HRESULT result = Get().GetRotation(rotation);
				if(FAILED(result))
					return tl::unexpected(result);
				return rotation;
			}
		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	template<>
	struct SwapChainTraitsImpl<IDXGISwapChain2>
	{
		using value_type = IDXGISwapChain2;
		using pointer = IDXGISwapChain2*;
		using const_pointer = const IDXGISwapChain2*;
		using reference = IDXGISwapChain2&;
		using cosnt_reference = const IDXGISwapChain2&;

		template<class DerivedSelf>
		class Interface : public SwapChainTraitsImpl<IDXGISwapChain1>::Interface<DerivedSelf>
		{
		private:
			using derived_self = DerivedSelf;

		public:
			HRESULT SetSourceSize(UINT Width, UINT Height)
			{
				return Get().SetSourceSize(Width, Height);
			}

			expected<std::pair<UINT, UINT>, HRESULT> GetSourceSize()
			{
				std::pair<UINT, UINT> size;
				HRESULT result = Get().GetSourceSize(&size.first, &size.second);
				if(FAILED(result))
					return tl::unexpected(result);
				return size;
			}

			HRESULT SetMaximumFrameLatency(UINT MaxLatency)
			{
				return Get().SetMaximumFrameLatency(&MaxLatency);
			}

			expected<UINT, HRESULT> STDMETHODCALLTYPE GetMaximumFrameLatency()
			{
				UINT latency;
				HRESULT result = Get().GetMaximumFrameLatency(&latency);
				if(FAILED(result))
					return tl::unexpected(result);
				return latency;
			}

			HANDLE GetFrameLatencyWaitableObject()
			{
				return Get().GetFrameLatencyWaitableObject();
			}

			HRESULT SetMatrixTransform(const DXGI_MATRIX_3X2_F& pMatrix)
			{
				return Get().SetMatrixTransform(&pMatrix);
			}

			expected<DXGI_MATRIX_3X2_F, HRESULT> STDMETHODCALLTYPE GetMatrixTransform()
			{
				DXGI_MATRIX_3X2_F matrix;
				HRESULT result = Get().GetMatrixTransform(&matrix);
				if(FAILED(result))
					return tl::unexpected(result);
				return matrix;
			}
		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	template<>
	struct SwapChainTraitsImpl<IDXGISwapChain3>
	{
		using value_type = IDXGISwapChain3;
		using pointer = IDXGISwapChain3*;
		using const_pointer = const IDXGISwapChain3*;
		using reference = IDXGISwapChain3&;
		using cosnt_reference = const IDXGISwapChain3&;

		template<class DerivedSelf>
		class Interface : public SwapChainTraitsImpl<IDXGISwapChain2>::Interface<DerivedSelf>
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

			HRESULT SetColorSpace1(DXGI_COLOR_SPACE_TYPE ColorSpace)
			{
				return Get().SetColorSpace1(ColorSpace);
			}

			template<std::derived_from<ID3D12CommandQueue> QueueTy>
			HRESULT ResizeBuffers1(
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

				return Get().ResizeBuffers1(BufferCount, Width, Height, Format, SwapChainFlags, pCreationNodeMask.data(), queues.get());
			}
		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	template<std::derived_from<IDXGISwapChain> Ty>
	struct UntaggedTraits<Ty>
	{
		using value_type = Ty;
		using pointer = Ty*;
		using const_pointer = const Ty*;
		using reference = Ty&;
		using cosnt_reference = const Ty&;

		template<class DerivedSelf>
		using Interface = SwapChainTraitsImpl<Ty>::template Interface<DerivedSelf>;
	};
}