module;

#include "dxgi1_6.h"
#include <concepts>

export module TypedDXGI:Factory;
import TypedD3D.Shared;
//import TypedD3D12;
//import :Wrappers;

struct ID3D11Device;
struct ID3D12CommandQueue;

namespace TypedD3D
{
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
			template<std::derived_from<IDXGIAdapter> AdapterTy = IDXGIAdapter>
			Wrapper<AdapterTy> EnumAdapters(UINT Adapter)
			{
				return ForwardFunction<AdapterTy, IDXGIAdapter>(&inner_type::EnumAdapters, Self(), Adapter);
			}

			void MakeWindowAssociation(HWND WindowHandle, UINT Flags)
			{
				ThrowIfFailed(Self().MakeWindowAssociation(Flags));
			}

			HWND GetWindowAssociation()
			{
				HWND hwnd;
				ThrowIfFailed(Self().GetWindowAssociation(&hwnd));
				return hwnd;
			}

			template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D11Device> Device = ID3D11Device>
			Wrapper<SwapChainTy> CreateSwapChain(Wrapper<Device> pDevice, const DXGI_SWAP_CHAIN_DESC& pDesc)
			{
				return ForwardFunction<SwapChainTy>(&inner_type::CreateSwapChain, Self(), pDevice.Get(), &pDesc);
			}

			//template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
			//Wrapper<SwapChainTy> CreateSwapChain(TypedD3D12::Direct<QueueTy> commandQueue, const DXGI_SWAP_CHAIN_DESC& pDesc)
			//{
			//	return ForwardFunction<SwapChainTy>(&inner_type::CreateSwapChain, Self(), commandQueue.Get(), &pDesc);
			//}

			template<std::derived_from<IDXGIAdapter> AdapterTy>
			Wrapper<AdapterTy> CreateSoftwareAdapter(HMODULE Module)
			{
				return ForwardFunction<IDXGIAdapter>(&inner_type::CreateSoftwareAdapter, Self(), Module);
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
			BOOL IsWindowedStereoEnabled() { return Self().IsWindowedStereoEnabled(); }


			//TODO: Temporary fix, make sure only ID3D11Devices can be accepted here
			template< std::derived_from<IDXGISwapChain> SwapChain = IDXGISwapChain, class /*std::derived_from<ID3D11Device>*/ Device = ID3D11Device*>
			Wrapper<SwapChain> CreateSwapChainForHwnd(
				/*Wrapper<Device>*/Device pDevice,
				HWND hWnd,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				return ForwardFunction<Wrapper<SwapChain>, IDXGISwapChain1>(&inner_type::CreateSwapChainForHwnd, Self(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput);
			}

			//template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, class QueueTy>
			//	requires std::same_as<typename QueueTy::value_type, ID3D12CommandQueue>
			//Wrapper<SwapChainTy> CreateSwapChainForHwnd(
			//	QueueTy pDevice,
			//	HWND hWnd,
			//	const DXGI_SWAP_CHAIN_DESC1& pDesc,
			//	const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
			//	IDXGIOutput* optRestrictToOutput)
			//{
			//	return ForwardFunction<SwapChain, IDXGISwapChain1>(&inner_type::CreateSwapChainForHwnd, &Get(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput);
			//}

			template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
				requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
			Wrapper<SwapChain> CreateSwapChainForCoreWindow(
				Wrapper<Device> pDevice,
				IUnknown& pWindow,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				return ForwardFunction<SwapChain, IDXGISwapChain1>(&inner_type::CreateSwapChainForCoreWindow, Self(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput);
			}

			//template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, class QueueTy>
			//	requires std::same_as<typename QueueTy::value_type, ID3D12CommandQueue>
			//Wrapper<SwapChainTy> CreateSwapChainForCoreWindow(
			//	QueueTy pDevice,
			//	IUnknown& pWindow,
			//	const DXGI_SWAP_CHAIN_DESC1& pDesc,
			//	IDXGIOutput* optRestrictToOutput)
			//{
			//	return ForwardFunction<SwapChain, IDXGISwapChain1>(&inner_type::CreateSwapChainForCoreWindow, Self(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput);
			//}

			LUID GetSharedResourceAdapterLuid(HANDLE hResource)
			{
				LUID luid;
				ThrowIfFailed(Self().GetSharedResourceAdapterLuid(hResource, &luid));
				return luid;
			}

			DWORD RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg)
			{
				DWORD pdwCookie;
				ThrowIfFailed(Self().RegisterStereoStatusWindow(WindowHandle, wMsg, &pdwCookie));
				return pdwCookie;
			}

			DWORD RegisterStereoStatusEvent(HANDLE hEvent)
			{
				DWORD pdwCookie;
				ThrowIfFailed(Self().RegisterStereoStatusEvent(hEvent, &pdwCookie));
				return pdwCookie;
			}

			void UnregisterStereoStatus(DWORD dwCookie)
			{
				Self().UnregisterStereoStatus(dwCookie);
			}

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

			void UnregisterOcclusionStatus(DWORD dwCookie)
			{
				Self().UnregisterOcclusionStatus(dwCookie);
			}

			template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
				requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
			Wrapper<SwapChain> CreateSwapChainForComposition(
				Wrapper<Device> pDevice,
				const DXGI_SWAP_CHAIN_DESC1& pDesc,
				IDXGIOutput* optRestrictToOutput)
			{
				return ForwardFunction<SwapChain, IDXGISwapChain1>(&inner_type::CreateSwapChainForComposition, Self(), pDevice.Get(), &pDesc, optRestrictToOutput);
			}

			//template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
			//Wrapper<SwapChainTy> CreateSwapChainForComposition(
			//	TypedD3D12::Direct<QueueTy> pDevice,
			//	const DXGI_SWAP_CHAIN_DESC1& pDesc,
			//	IDXGIOutput* optRestrictToOutput)
			//{
			//	return TypedD3D::Cast<SwapChainTy>(UnknownObjectForwardFunction<IDXGISwapChain1>(&inner_type::CreateSwapChainForComposition, Self(), pDevice.Get(), &pDesc, optRestrictToOutput));
			//}

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