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

			void MakeWindowAssociation(HWND WindowHandle, UINT Flags)
			{
				ThrowIfFailed(Get().MakeWindowAssociation(Flags));
			}

			HWND GetWindowAssociation()
			{
				HWND hwnd;
				ThrowIfFailed(Get().GetWindowAssociation(&hwnd));
				return hwnd;
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D11Device> Device = ID3D11Device>
			Wrapper<SwapChainTy> CreateSwapChain(Wrapper<Device> pDevice, const DXGI_SWAP_CHAIN_DESC& pDesc)
			{
				return UnknownObjectForwardFunction<SwapChainTy>(&value_type::CreateSwapChain, Get(), pDevice.Get(), &pDesc);
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
			Wrapper<SwapChainTy> CreateSwapChain(TypedD3D12::Direct<QueueTy> commandQueue, const DXGI_SWAP_CHAIN_DESC& pDesc)
			{
				return UnknownObjectForwardFunction<SwapChainTy>(&value_type::CreateSwapChain, Get(), commandQueue.Get(), &pDesc);
			}

			template<std::derived_from<IDXGIAdapter> AdapterTy>
			Wrapper<AdapterTy> CreateSoftwareAdapter(HMODULE Module)
			{
				return UnknownObjectForwardFunction<IDXGIAdapter>(&value_type::CreateSoftwareAdapter, Get(), Module);
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
			Wrapper<SwapChain> CreateSwapChainForHwnd(
				/*Wrapper<Device>*/Device pDevice,
				HWND hWnd,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				return TypedD3D::Cast<SwapChain>(UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForHwnd, Get(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput));
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, class QueueTy>
				requires std::same_as<typename QueueTy::value_type, ID3D12CommandQueue>
			Wrapper<SwapChainTy> CreateSwapChainForHwnd(
				QueueTy pDevice,
				HWND hWnd,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				return TypedD3D::Cast<SwapChainTy>(UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForHwnd, &Get(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput));
			}

			template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
				requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
			Wrapper<SwapChain> CreateSwapChainForCoreWindow(
				Wrapper<Device> pDevice,
				IUnknown& pWindow,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				return TypedD3D::Cast<SwapChain>(UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForCoreWindow, Get(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput));
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, class QueueTy>
				requires std::same_as<typename QueueTy::value_type, ID3D12CommandQueue>
			Wrapper<SwapChainTy> CreateSwapChainForCoreWindow(
				QueueTy pDevice,
				IUnknown& pWindow,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				return TypedD3D::Cast<SwapChainTy>(UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForCoreWindow, Get(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput));
			}

			LUID GetSharedResourceAdapterLuid(HANDLE hResource)
			{
				LUID luid;
				ThrowIfFailed(Get().GetSharedResourceAdapterLuid(hResource, &luid));
				return luid;
			}

			DWORD RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg)
			{
				DWORD pdwCookie;
				ThrowIfFailed(Get().RegisterStereoStatusWindow(WindowHandle, wMsg, &pdwCookie));
				return pdwCookie;
			}

			DWORD RegisterStereoStatusEvent(HANDLE hEvent)
			{
				DWORD pdwCookie;
				ThrowIfFailed(Get().RegisterStereoStatusEvent(hEvent, &pdwCookie));
				return pdwCookie;
			}

			void UnregisterStereoStatus(DWORD dwCookie)
			{
				Get().UnregisterStereoStatus(dwCookie);
			}

			DWORD RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg)
			{
				DWORD pdwCookie;
				ThrowIfFailed(Get().RegisterOcclusionStatusWindow(WindowHandle, wMsg, &pdwCookie));
				return pdwCookie;
			}

			DWORD RegisterOcclusionStatusEvent(HANDLE hEvent)
			{
				DWORD pdwCookie;
				ThrowIfFailed(Get().RegisterOcclusionStatusEvent(hEvent, &pdwCookie));
				return pdwCookie;
			}

			void UnregisterOcclusionStatus(DWORD dwCookie)
			{
				Get().UnregisterOcclusionStatus(dwCookie);
			}

			template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
				requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
			Wrapper<SwapChain> CreateSwapChainForComposition(
				Wrapper<Device> pDevice,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				return TypedD3D::Cast<SwapChain>(UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForComposition, Get(), pDevice.Get(), &pDesc, optRestrictToOutput));
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
			Wrapper<SwapChainTy> CreateSwapChainForComposition(
				TypedD3D12::Direct<QueueTy> pDevice,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				return TypedD3D::Cast<SwapChainTy>(UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForComposition, Get(), pDevice.Get(), &pDesc, optRestrictToOutput));
			}

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	export template<class FactoryTy = IDXGIFactory>
	Wrapper<FactoryTy> CreateFactory()
	{
		return IIDToObjectForwardFunction<FactoryTy>(&::CreateDXGIFactory);
	}

	export template<class FactoryTy = IDXGIFactory>
	Wrapper<FactoryTy> CreateFactory1()
	{
		return IIDToObjectForwardFunction<FactoryTy>(&::CreateDXGIFactory1);
	}

	export template<class FactoryTy = IDXGIFactory>
	Wrapper<FactoryTy> CreateFactory2(UINT flags)
	{
		return IIDToObjectForwardFunction<FactoryTy>(&::CreateDXGIFactory2, flags);
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