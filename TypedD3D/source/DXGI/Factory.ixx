module;

#include "dxgi1_6.h"
#include <d3d12.h>
#include <d3d11.h>
#include <concepts>
#include <gsl/pointers>

export module TypedDXGI:Factory;
import TypedD3D.Shared;
import TypedD3D12;
import :DXGIObject;

namespace TypedD3D
{
	template<class Ty>
	concept SwapChainCompatibleDevice = std::derived_from<typename Ty::inner_type, ID3D11Device>
		|| (std::derived_from<typename Ty::inner_type, ID3D12CommandQueue> && std::same_as<InnerType<Ty>, DirectTag<typename Ty::inner_type>>);

	template<>
	struct Trait<Untagged<IDXGIFactory>>
	{
		using inner_type = IDXGIFactory;

		using inner_tag = IDXGIFactory;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;
		template<class Derived>
		class Interface : public InterfaceBase<Untagged<Derived>>
		{
		private:
			using derived_self = Derived;

		public:
			template<std::derived_from<IDXGIAdapter> AdapterTy>
			Wrapper<AdapterTy> CreateSoftwareAdapter(HMODULE Module)
			{
				return ForwardFunction<IDXGIAdapter>(&inner_type::CreateSoftwareAdapter, Self(), Module);
			}

			//The template signature is like this because if I explicitly say WrapperView<ID3D11Device> or DirectView<ID3D12CommandQueue>
			//users will need to import both TypedD3D12 and TypedD3D11 for it to work as their definitions aren't visible otherwise
			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, SwapChainCompatibleDevice DeviceTy>
			Wrapper<SwapChainTy> CreateSwapChain(DeviceTy pDevice, const DXGI_SWAP_CHAIN_DESC& pDesc)
			{
				return ForwardFunction<SwapChainTy>(&inner_type::CreateSwapChain, Self(), pDevice.Get(), &pDesc);
			}

			template<std::derived_from<IDXGIAdapter> AdapterTy = IDXGIAdapter>
			Wrapper<AdapterTy> EnumAdapters(UINT Adapter)
			{
				return ForwardFunction<AdapterTy, IDXGIAdapter>(&inner_type::EnumAdapters, Self(), Adapter);
			}

			HWND GetWindowAssociation()
			{
				HWND hwnd;
				ThrowIfFailed(Self().GetWindowAssociation(&hwnd));
				return hwnd;
			}

			void MakeWindowAssociation(HWND WindowHandle, UINT Flags)
			{
				ThrowIfFailed(Self().MakeWindowAssociation(Flags));
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	template<>
	struct Trait<Untagged<IDXGIFactory1>>
	{
		using inner_type = IDXGIFactory1;

		using inner_tag = IDXGIFactory1;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		class Interface : public Trait<Untagged<IDXGIFactory>>::Interface<Derived>
		{
		public:
			template<std::derived_from<IDXGIAdapter> AdapterTy>
			Wrapper<AdapterTy> EnumAdapters1(UINT Adapter)
			{
				return ForwardFunction<AdapterTy, IDXGIAdapter1>(&inner_type::EnumAdapters1, Self(), Adapter);
			}

			BOOL IsCurrent() { return Self().IsCurrent(); }

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};

	};

	template<>
	struct Trait<Untagged<IDXGIFactory2>>
	{
		using inner_type = IDXGIFactory2;

		using inner_tag = IDXGIFactory2;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		class Interface : public Trait<Untagged<IDXGIFactory1>>::Interface<Derived>
		{
		private:
			using derived_self = Derived;

		public:
			//The template signature is like this because if I explicitly say WrapperView<ID3D11Device> or DirectView<ID3D12CommandQueue>
			//users will need to import both TypedD3D12 and TypedD3D11 for it to work as their definitions aren't visible otherwise
			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, SwapChainCompatibleDevice DeviceTy>
			Wrapper<SwapChainTy> CreateSwapChainForComposition(
				DeviceTy commandQueue,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				WrapperView<IDXGIOutput> optRestrictToOutput)
			{
				return ForwardFunction<Wrapper<SwapChainTy>, IDXGISwapChain1>(&inner_type::CreateSwapChainForComposition, Self(), commandQueue.Get(), &pDesc, optRestrictToOutput.Get());
			}

			//The template signature is like this because if I explicitly say WrapperView<ID3D11Device> or DirectView<ID3D12CommandQueue>
			//users will need to import both TypedD3D12 and TypedD3D11 for it to work as their definitions aren't visible otherwise
			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, SwapChainCompatibleDevice DeviceTy>
			Wrapper<SwapChainTy> CreateSwapChainForCoreWindow(
				DeviceTy pDevice,
				WrapperView<IUnknown> pWindow,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				WrapperView<IDXGIOutput> optRestrictToOutput)
			{
				return ForwardFunction<Wrapper<SwapChainTy>, IDXGISwapChain1>(&inner_type::CreateSwapChainForCoreWindow, Self(), pDevice.Get(), pWindow.Get(), &pDesc, optRestrictToOutput.Get());
			}

			//The template signature is like this because if I explicitly say WrapperView<ID3D11Device> or DirectView<ID3D12CommandQueue>
			//users will need to import both TypedD3D12 and TypedD3D11 for it to work as their definitions aren't visible otherwise
			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, SwapChainCompatibleDevice DeviceTy>
			Wrapper<SwapChainTy> CreateSwapChainForHwnd(
				DeviceTy pDevice,
				HWND hWnd,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
				WrapperView<IDXGIOutput> optRestrictToOutput)
			{
				return ForwardFunction<Wrapper<SwapChainTy>, IDXGISwapChain1>(&inner_type::CreateSwapChainForHwnd, Self(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput.Get());
			}

			LUID GetSharedResourceAdapterLuid(HANDLE hResource)
			{
				LUID luid;
				ThrowIfFailed(Self().GetSharedResourceAdapterLuid(hResource, &luid));
				return luid;
			}

			BOOL IsWindowedStereoEnabled() { return Self().IsWindowedStereoEnabled(); }

			DWORD RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg)
			{
				DWORD pdwCookie;
				ThrowIfFailed(Self().RegisterOcclusionStatusWindow(WindowHandle, wMsg, &pdwCookie));
				return pdwCookie;
			}

			DWORD RegisterOcclusionStatusEvent(HANDLE hEvent)
			{
				DWORD pdwCookie;
				ThrowIfFailed(Self().RegisterOcclusionStatusEvent(hEvent, &pdwCookie));
				return pdwCookie;
			}

			DWORD RegisterStereoStatusEvent(HANDLE hEvent)
			{
				DWORD pdwCookie;
				ThrowIfFailed(Self().RegisterStereoStatusEvent(hEvent, &pdwCookie));
				return pdwCookie;
			}

			DWORD RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg)
			{
				DWORD pdwCookie;
				ThrowIfFailed(Self().RegisterStereoStatusWindow(WindowHandle, wMsg, &pdwCookie));
				return pdwCookie;
			}

			void UnregisterOcclusionStatus(DWORD dwCookie)
			{
				Self().UnregisterOcclusionStatus(dwCookie);
			}

			void UnregisterStereoStatus(DWORD dwCookie)
			{
				Self().UnregisterStereoStatus(dwCookie);
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};
}

namespace TypedD3D::DXGI
{
	export template<class FactoryTy = IDXGIFactory>
		Wrapper<FactoryTy> CreateFactory()
	{
		return ForwardFunction<Wrapper<FactoryTy>>(&::CreateDXGIFactory);
	}

	export template<class FactoryTy = IDXGIFactory>
		Wrapper<FactoryTy> CreateFactory1()
	{
		return ForwardFunction<Wrapper<FactoryTy>>(&::CreateDXGIFactory1);
	}

	export template<class FactoryTy = IDXGIFactory>
		Wrapper<FactoryTy> CreateFactory2(UINT flags)
	{
		return ForwardFunction<Wrapper<FactoryTy>>(&::CreateDXGIFactory2, flags);
	}
}