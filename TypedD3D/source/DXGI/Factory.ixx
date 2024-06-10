module;

#include "dxgi1_6.h"
#include <concepts>

export module TypedDXGI:Factory;
import TypedD3D.Shared;
import TypedD3D12;
//import :Wrappers;

struct ID3D11Device;
struct ID3D12CommandQueue;

namespace TypedD3D::DXGI
{
	export template<class Ty>
	using Factory_t = IUnknownWrapper<Ty, UntaggedTraits>;

	template<class Ty>
	struct FactoryTraits;

	template<>
	struct FactoryTraits<IDXGIFactory>
	{
		using value_type = IDXGIFactory;
		using pointer = IDXGIFactory*;
		using const_pointer = const IDXGIFactory*;
		using reference = IDXGIFactory&;
		using cosnt_reference = const IDXGIFactory&;

		template<class DerivedSelf>
		class Interface
		{
		private:
			using derived_self = DerivedSelf;

		public:
			template<class AdapterTy = IDXGIAdapter>
				requires std::derived_from<AdapterTy, IDXGIAdapter>
			Wrapper<AdapterTy> EnumAdapters(UINT Adapter)
			{
				auto adapter = UnknownObjectForwardFunction<IDXGIAdapter>(&value_type::EnumAdapters, Get(), Adapter);
				if(!adapter.has_value())
					return nullptr;

				if constexpr(std::same_as<AdapterTy, IDXGIAdapter>)
					return Wrapper<AdapterTy>(adapter.value());
				else
					return Wrapper<AdapterTy>(Cast<AdapterTy>(adapter.value()));
			}

			HRESULT MakeWindowAssociation(HWND WindowHandle, UINT Flags)
			{
				Get().MakeWindowAssociation(Flags);
			}

			expected<HWND, HRESULT> GetWindowAssociation()
			{
				HWND hwnd;
				HRESULT result = Get().GetWindowAssociation(&hwnd);
				if(FAILED(result))
					return tl::unexpected(result);
				return hwnd;
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D11Device> Device = ID3D11Device>
			expected<Wrapper<SwapChainTy>, HRESULT> CreateSwapChain(Wrapper<Device> pDevice, const DXGI_SWAP_CHAIN_DESC& pDesc)
			{
				return UnknownObjectForwardFunction<SwapChainTy>(&value_type::CreateSwapChain, Get(), pDevice.Get(), &pDesc)
					.and_then([](auto swapChain) -> expected<Wrapper<SwapChainTy>, HRESULT> { return Wrapper<SwapChainTy>(swapChain); });;
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
			expected<Wrapper<SwapChainTy>, HRESULT> CreateSwapChain(TypedD3D12::Direct<QueueTy> commandQueue, const DXGI_SWAP_CHAIN_DESC& pDesc)
			{
				return UnknownObjectForwardFunction<SwapChainTy>(&value_type::CreateSwapChain, Get(), commandQueue.Get(), &pDesc)
					.and_then([](auto swapChain) -> expected<Wrapper<SwapChainTy>, HRESULT> { return Wrapper<SwapChainTy>(swapChain); });
			}

			template<std::derived_from<IDXGIAdapter> AdapterTy>
			expected<Wrapper<AdapterTy>, HRESULT> CreateSoftwareAdapter(HMODULE Module)
			{
				auto adapter = UnknownObjectForwardFunction<IDXGIAdapter>(&value_type::CreateSoftwareAdapter, Get(), Module)
					.and_then([](auto adapter) -> expected<Wrapper<AdapterTy>, HRESULT> { return Wrapper<AdapterTy>(adapter); });
			}

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	template<>
	struct FactoryTraits<IDXGIFactory1>
	{
		using value_type = IDXGIFactory1;
		using pointer = IDXGIFactory1*;
		using const_pointer = const IDXGIFactory1*;
		using reference = IDXGIFactory1&;
		using cosnt_reference = const IDXGIFactory1&;

		template<class DerivedSelf>
		class Interface : public FactoryTraits<IDXGIFactory>::Interface<DerivedSelf>
		{
		private:
			using derived_self = DerivedSelf;

		public:
			template<class AdapterTy>
				requires std::derived_from<AdapterTy, IDXGIAdapter>
			Wrapper<AdapterTy> EnumAdapters1(UINT Adapter)
			{
				return UnknownObjectForwardFunction<AdapterTy>(&value_type::EnumAdapters1, Get(), Adapter).value_or(nullptr);
				auto adapter = UnknownObjectForwardFunction<IDXGIAdapter1>(&value_type::EnumAdapters1, Get(), Adapter);
				if(!adapter.has_value())
					return nullptr;

				if constexpr(std::same_as<AdapterTy, IDXGIAdapter>)
					return Wrapper<AdapterTy>(adapter.value());
				else
					return Wrapper<AdapterTy>(Cast<AdapterTy>(adapter.value()));
			}

			BOOL IsCurrent() { return Get().IsCurrent(); }

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	template<>
	struct FactoryTraits<IDXGIFactory2>
	{
		using value_type = IDXGIFactory2;
		using pointer = IDXGIFactory2*;
		using const_pointer = const IDXGIFactory2*;
		using reference = IDXGIFactory2&;
		using cosnt_reference = const IDXGIFactory2&;

		template<class DerivedSelf>
		class Interface : public FactoryTraits<IDXGIFactory1>::Interface<DerivedSelf>
		{
		private:
			using derived_self = DerivedSelf;

		public:
			BOOL IsWindowedStereoEnabled() { return Get().IsWindowedStereoEnabled(); }


			//TODO: Temporary fix, make sure only ID3D11Devices can be accepted here
			template< std::derived_from<IDXGISwapChain> SwapChain = IDXGISwapChain, class /*std::derived_from<ID3D11Device>*/ Device = ID3D11Device*>
			expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForHwnd(
				/*Wrapper<Device>*/Device pDevice,
				HWND hWnd,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				auto swapChain = UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForHwnd, Get(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput);

				if(!swapChain.has_value())
					return tl::unexpected(swapChain.error());

				if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
					return Wrapper<SwapChain>(swapChain.value());
				else
					return Wrapper<SwapChain>(TypedD3D::Cast<SwapChain>(swapChain.value()));
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, class QueueTy>
				requires std::same_as<typename QueueTy::value_type, ID3D12CommandQueue>
			expected<Wrapper<SwapChainTy>, HRESULT> CreateSwapChainForHwnd(
				QueueTy pDevice,
				HWND hWnd,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				auto swapChain = UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForHwnd, &Get(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput);

				if(!swapChain.has_value())
					return tl::unexpected(swapChain.error());

				if constexpr(std::same_as<SwapChainTy, IDXGISwapChain1>)
					return Wrapper<SwapChainTy>(swapChain.value());
				else
					return Wrapper<SwapChainTy>(TypedD3D::Cast<SwapChainTy>(swapChain.value()));
			}

			template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
				requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
			expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForCoreWindow(
				Wrapper<Device> pDevice,
				IUnknown& pWindow,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				auto swapChain = UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForCoreWindow, Get(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput);

				if(!swapChain.has_value())
					return tl::unexpected(swapChain.error());

				if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
					return Wrapper<SwapChain>(swapChain.value());
				else
					return Wrapper<SwapChain>(Cast<SwapChain>(swapChain.value()));
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, class QueueTy>
				requires std::same_as<typename QueueTy::value_type, ID3D12CommandQueue>
			expected<Wrapper<SwapChainTy>, HRESULT> CreateSwapChainForCoreWindow(
				QueueTy pDevice,
				IUnknown& pWindow,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				auto swapChain = UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForCoreWindow, Get(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput);

				if(!swapChain.has_value())
					return tl::unexpected(swapChain.error());

				if constexpr(std::same_as<SwapChainTy, IDXGISwapChain1>)
					return Wrapper<SwapChainTy>(swapChain.value());
				else
					return Wrapper<SwapChainTy>(Cast<SwapChainTy>(swapChain.value()));
			}

			expected<LUID, HRESULT> GetSharedResourceAdapterLuid(HANDLE hResource)
			{
				LUID luid;
				HRESULT result = Get().GetSharedResourceAdapterLuid(hResource, &luid);
				if(FAILED(result))
					return tl::unexpected(result);
				return luid;
			}

			expected<DWORD, HRESULT> RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg)
			{
				DWORD pdwCookie;
				HRESULT result = Get().RegisterStereoStatusWindow(WindowHandle, wMsg, &pdwCookie);
				if(FAILED(result))
					return tl::unexpected(result);
				return pdwCookie;
			}

			expected<DWORD, HRESULT> RegisterStereoStatusEvent(HANDLE hEvent)
			{
				DWORD pdwCookie;
				HRESULT result = Get().RegisterStereoStatusEvent(hEvent, &pdwCookie);
				if(FAILED(result))
					return tl::unexpected(result);
				return pdwCookie;
			}

			void UnregisterStereoStatus(DWORD dwCookie)
			{
				Get().UnregisterStereoStatus(dwCookie);
			}

			expected<DWORD, HRESULT> RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg)
			{
				DWORD pdwCookie;
				HRESULT result = Get().RegisterOcclusionStatusWindow(WindowHandle, wMsg, &pdwCookie);
				if(FAILED(result))
					return tl::unexpected(result);
				return pdwCookie;
			}

			expected<DWORD, HRESULT> RegisterOcclusionStatusEvent(HANDLE hEvent)
			{
				DWORD pdwCookie;
				HRESULT result = Get().RegisterOcclusionStatusEvent(hEvent, &pdwCookie);
				if(FAILED(result))
					return tl::unexpected(result);
				return pdwCookie;
			}

			void UnregisterOcclusionStatus(DWORD dwCookie)
			{
				Get().UnregisterOcclusionStatus(dwCookie);
			}

			template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
				requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
			expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForComposition(
				Wrapper<Device> pDevice,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				auto swapChain = UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForComposition, Get(), pDevice.Get(), &pDesc, optRestrictToOutput);

				if(!swapChain.has_value())
					return tl::unexpected(swapChain.error());

				if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
					return Wrapper<SwapChain>(swapChain.value());
				else
					return Wrapper<SwapChain>(Cast<SwapChain>(swapChain.value()));
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
			expected<Wrapper<SwapChainTy>, HRESULT> CreateSwapChainForComposition(
				TypedD3D12::Direct<QueueTy> pDevice,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				auto swapChain = UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForComposition, Get(), pDevice.Get(), &pDesc, optRestrictToOutput);

				if(!swapChain.has_value())
					return tl::unexpected(swapChain.error());

				if constexpr(std::same_as<SwapChainTy, IDXGISwapChain1>)
					return Wrapper<SwapChainTy>(swapChain.value());
				else
					return Wrapper<SwapChainTy>(Cast<SwapChainTy>(swapChain.value()));
			}

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	export template<class FactoryTy = IDXGIFactory>
	expected<Wrapper<FactoryTy>, HRESULT> CreateFactory()
	{
		auto factory = IIDToObjectForwardFunction<FactoryTy>(&::CreateDXGIFactory);

		if(!factory.has_value())
			return tl::unexpected(factory.error());

		return Wrapper<FactoryTy>(factory.value());
	}

	export template<class FactoryTy = IDXGIFactory>
	expected<Wrapper<FactoryTy>, HRESULT> CreateFactory1()
	{
		auto factory = IIDToObjectForwardFunction<FactoryTy>(&::CreateDXGIFactory1);

		if(!factory.has_value())
			return tl::unexpected(factory.error());

		return Wrapper<FactoryTy>(factory.value());
	}

	export template<class FactoryTy = IDXGIFactory>
	expected<Wrapper<FactoryTy>, HRESULT> CreateFactory2(UINT flags)
	{
		auto factory = IIDToObjectForwardFunction<FactoryTy>(&::CreateDXGIFactory2, flags);

		if(!factory.has_value())
			return tl::unexpected(factory.error());

		return Wrapper<FactoryTy>(factory.value());
	}
}

namespace TypedD3D
{
	template<std::derived_from<IDXGIFactory> Ty>
	struct UntaggedTraits<Ty>
	{
		using value_type = Ty;
		using pointer = Ty*;
		using const_pointer = const Ty*;
		using reference = Ty&;
		using cosnt_reference = const Ty&;

		template<class DerivedSelf>
		using Interface = DXGI::FactoryTraits<Ty>::template Interface<DerivedSelf>;
	};
}