#pragma once
#include "../Wrappers.h"
#include "../Internal/ComWrapper.h"
#include "expected.hpp"
#include "../Helpers/COMHelpers.h"
#include "../D3D12TypeTags.h"
#include <dxgi1_6.h>

class ID3D11Device;
struct ID3D12CommandQueue;

namespace TypedD3D::Internal
{
    namespace DXGI
    {
        template<class Ty>
        using Factory_t = InterfaceWrapper<Ty>;

        namespace Factory
        {
            template<class WrapperTy, class FactoryTy>
            class Interface;

            template<class Ty>
            using Direct = Wrapper<Ty, TypeTag::Direct>;

            template<class WrapperTy>
            class Interface<WrapperTy, IDXGIFactory>
            {
                using factory_type = IDXGIFactory;
                using wrapper_type = WrapperTy;

            public:
                template<class AdapterTy = IDXGIAdapter>
                    requires std::derived_from<AdapterTy, IDXGIAdapter>
                Wrapper<AdapterTy> EnumAdapters(UINT Adapter)
                {
                    auto adapter = Helpers::COM::UnknownObjectForwardFunction<IDXGIAdapter>(&IDXGIFactory::EnumAdapters, InternalGet(), Adapter);
                    if(!adapter.has_value())
                        return nullptr;

                    if constexpr(std::same_as<AdapterTy, IDXGIAdapter>)
                        return Wrapper<AdapterTy>(adapter.value());
                    else
                        return Wrapper<AdapterTy>(Helpers::COM::Cast<AdapterTy>(adapter.value()));
                }

                HRESULT MakeWindowAssociation(HWND WindowHandle, UINT Flags)
                {
                    InternalGet().MakeWindowAssociation(Flags);
                }

                tl::expected<HWND, HRESULT> GetWindowAssociation()
                {
                    HWND hwnd;
                    HRESULT result = InternalGet().GetWindowAssociation(&hwnd);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return hwnd;
                }

                template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D11Device> Device = ID3D11Device>
                tl::expected<Wrapper<SwapChainTy>, HRESULT> CreateSwapChain(Wrapper<Device> pDevice, const DXGI_SWAP_CHAIN_DESC& pDesc)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<SwapChainTy>(&IDXGIFactory::CreateSwapChain, InternalGet(), pDevice.Get(), &pDesc)
                        .and_then([](auto swapChain) -> tl::expected<Wrapper<SwapChainTy>, HRESULT> { return Wrapper<SwapChainTy>(swapChain); });;

                    //if(!swapChain.has_value())
                    //    return tl::unexpected(swapChain.error());

                    //if constexpr(std::same_as<SwapChain, IDXGISwapChain>)
                    //    return Wrapper<SwapChain>(swapChain.value());
                    //else
                    //    return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.value()));
                }

                template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
                tl::expected<Wrapper<SwapChainTy>, HRESULT> CreateSwapChain(Direct<QueueTy> commandQueue, const DXGI_SWAP_CHAIN_DESC& pDesc)
                {
                   return Helpers::COM::UnknownObjectForwardFunction<SwapChainTy>(&IDXGIFactory::CreateSwapChain, InternalGet(), commandQueue.Get(), &pDesc)
                        .and_then([](auto swapChain) -> tl::expected<Wrapper<SwapChainTy>, HRESULT> { return Wrapper<SwapChainTy>(swapChain); });
                }

                template<std::derived_from<IDXGIAdapter> AdapterTy>
                tl::expected<Wrapper<AdapterTy>, HRESULT> CreateSoftwareAdapter(HMODULE Module)
                {
                    auto adapter = Helpers::COM::UnknownObjectForwardFunction<IDXGIAdapter>(&IDXGIFactory::CreateSoftwareAdapter, InternalGet(), Module)
                        .and_then([](auto adapter) -> tl::expected<Wrapper<AdapterTy>, HRESULT> { return Wrapper<AdapterTy>(adapter); });
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                factory_type& InternalGet() { return *ToDerived().Get(); }
            };

            template<class WrapperTy>
            class Interface<WrapperTy, IDXGIFactory1> : public Interface<WrapperTy, IDXGIFactory>
            {
                using factory_type = IDXGIFactory1;
                using wrapper_type = WrapperTy;

            public:
                template<class AdapterTy>
                    requires std::derived_from<AdapterTy, IDXGIAdapter>
                Wrapper<AdapterTy> EnumAdapters1(UINT Adapter)
                {
                    auto adapter = Helpers::COM::UnknownObjectForwardFunction<IDXGIAdapter1>(&IDXGIFactory1::EnumAdapters1, InternalGet(), Adapter);
                    if(!adapter.has_value())
                        return nullptr;

                    if constexpr(std::same_as<AdapterTy, IDXGIAdapter>)
                        return Wrapper<AdapterTy>(adapter.value());
                    else
                        return Wrapper<AdapterTy>(Helpers::COM::Cast<AdapterTy>(adapter.value()));
                }

                BOOL IsCurrent() { return InternalGet().IsCurrent(); }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                factory_type& InternalGet() { return *ToDerived().Get(); }
            };

            template<class WrapperTy>
            class Interface<WrapperTy, IDXGIFactory2> : public Interface<WrapperTy, IDXGIFactory1>
            {
                using factory_type = IDXGIFactory2;
                using wrapper_type = WrapperTy;

            public:
                BOOL IsWindowedStereoEnabled() { return InternalGet().IsWindowedStereoEnabled(); }

                template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
                    requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
                tl::expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForHwnd(
                    Wrapper<Device> pDevice,
                    HWND hWnd,
                    const DXGI_SWAP_CHAIN_DESC1& pDesc,
                    const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
                    IDXGIOutput* optRestrictToOutput)
                {
                    auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForHwnd, InternalGet(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput);

                    if(!swapChain.has_value())
                        return tl::unexpected(swapChain.error());

                    if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                        return Wrapper<SwapChain>(swapChain.value());
                    else
                        return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.value()));
                }

                template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
                tl::expected<Wrapper<SwapChainTy>, HRESULT> CreateSwapChainForHwnd(
                    Direct<QueueTy> pDevice,
                    HWND hWnd,
                    const DXGI_SWAP_CHAIN_DESC1& pDesc,
                    const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
                    IDXGIOutput* optRestrictToOutput)
                {
                    auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForHwnd, &InternalGet(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput);

                    if(!swapChain.has_value())
                        return tl::unexpected(swapChain.error());

                    if constexpr(std::same_as<SwapChainTy, IDXGISwapChain1>)
                        return Wrapper<SwapChainTy>(swapChain.value());
                    else
                        return Wrapper<SwapChainTy>(Helpers::COM::Cast<SwapChainTy>(swapChain.value()));
                }

                template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
                    requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
                tl::expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForCoreWindow(
                    Wrapper<Device> pDevice,
                    IUnknown& pWindow,
                    const DXGI_SWAP_CHAIN_DESC1& pDesc,
                    IDXGIOutput* optRestrictToOutput)
                {
                    auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForCoreWindow, InternalGet(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput);

                    if(!swapChain.has_value())
                        return tl::unexpected(swapChain.error());

                    if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                        return Wrapper<SwapChain>(swapChain.value());
                    else
                        return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.value()));
                }

                template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
                tl::expected<Wrapper<SwapChainTy>, HRESULT> CreateSwapChainForCoreWindow(
                    Direct<QueueTy> pDevice,
                    IUnknown& pWindow,
                    const DXGI_SWAP_CHAIN_DESC1& pDesc,
                    IDXGIOutput* optRestrictToOutput)
                {
                    auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForCoreWindow, InternalGet(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput);

                    if(!swapChain.has_value())
                        return tl::unexpected(swapChain.error());

                    if constexpr(std::same_as<SwapChainTy, IDXGISwapChain1>)
                        return Wrapper<SwapChainTy>(swapChain.value());
                    else
                        return Wrapper<SwapChainTy>(Helpers::COM::Cast<SwapChainTy>(swapChain.value()));
                }

                tl::expected<LUID, HRESULT> GetSharedResourceAdapterLuid(HANDLE hResource)
                {
                    LUID luid;
                    HRESULT result = InternalGet().GetSharedResourceAdapterLuid(hResource, &luid);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return luid;
                }

                tl::expected<DWORD, HRESULT> RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg)
                {
                    DWORD pdwCookie;
                    HRESULT result = InternalGet().RegisterStereoStatusWindow(WindowHandle, wMsg, &pdwCookie);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return pdwCookie;
                }

                tl::expected<DWORD, HRESULT> RegisterStereoStatusEvent(HANDLE hEvent)
                {
                    DWORD pdwCookie;
                    HRESULT result = InternalGet().RegisterStereoStatusEvent(hEvent, &pdwCookie);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return pdwCookie;
                }

                void UnregisterStereoStatus(DWORD dwCookie)
                {
                    InternalGet().UnregisterStereoStatus(dwCookie);
                }

                tl::expected<DWORD, HRESULT> RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg)
                {
                    DWORD pdwCookie;
                    HRESULT result = InternalGet().RegisterOcclusionStatusWindow(WindowHandle, wMsg, &pdwCookie);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return pdwCookie;
                }

                tl::expected<DWORD, HRESULT> RegisterOcclusionStatusEvent(HANDLE hEvent)
                {
                    DWORD pdwCookie;
                    HRESULT result = InternalGet().RegisterOcclusionStatusEvent(hEvent, &pdwCookie);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return pdwCookie;
                }

                void UnregisterOcclusionStatus(DWORD dwCookie)
                {
                    InternalGet().UnregisterOcclusionStatus(dwCookie);
                }

                template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
                    requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
                tl::expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForComposition(
                    Wrapper<Device> pDevice,
                    const DXGI_SWAP_CHAIN_DESC1& pDesc,
                    IDXGIOutput* optRestrictToOutput)
                {
                    auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForComposition, InternalGet(), pDevice.Get(), &pDesc, optRestrictToOutput);

                    if(!swapChain.has_value())
                        return tl::unexpected(swapChain.error());

                    if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                        return Wrapper<SwapChain>(swapChain.value());
                    else
                        return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.value()));
                }

                template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
                tl::expected<Wrapper<SwapChainTy>, HRESULT> CreateSwapChainForComposition(
                    Direct<QueueTy> pDevice,
                    const DXGI_SWAP_CHAIN_DESC1& pDesc,
                    IDXGIOutput* optRestrictToOutput)
                {
                    auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForComposition, InternalGet(), pDevice.Get(), &pDesc, optRestrictToOutput);

                    if(!swapChain.has_value())
                        return tl::unexpected(swapChain.error());

                    if constexpr(std::same_as<SwapChainTy, IDXGISwapChain1>)
                        return Wrapper<SwapChainTy>(swapChain.value());
                    else
                        return Wrapper<SwapChainTy>(Helpers::COM::Cast<SwapChainTy>(swapChain.value()));
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                factory_type& InternalGet() { return *ToDerived().Get(); }
            };
        }
    }

    template<class DirectXClass>
        requires std::derived_from<DirectXClass, IDXGIFactory>
    class InterfaceWrapper<DirectXClass> : public ComWrapper<DirectXClass>, private DXGI::Factory::Interface<InterfaceWrapper<DirectXClass>, DirectXClass>
    {
    private:
        using Interface = DXGI::Factory::Interface<InterfaceWrapper<DirectXClass>, DirectXClass>;
        friend Interface;

        template<class WrapperTy, class FactoryTy>
        friend class DXGI::Factory::Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = DirectXClass;

    public:
        using ComWrapper<DirectXClass>::ComWrapper;

        template<class DerivedFactoryTy>
            requires std::convertible_to<DerivedFactoryTy*, DirectXClass*>
        InterfaceWrapper(const InterfaceWrapper<DerivedFactoryTy>& other) :
            ComWrapper<DirectXClass>::ComWrapper(other.AsComPtr())
        {

        }

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}

namespace TypedD3D::DXGI
{
    template<class Ty>
    using Factory_t = Internal::DXGI::Factory_t<Ty>;

    namespace Factory
    {
        template<class FactoryTy = IDXGIFactory>
        tl::expected<Wrapper<FactoryTy>, HRESULT> Create()
        {
            auto factory = Helpers::COM::IIDToObjectForwardFunction<FactoryTy>(&CreateDXGIFactory);

            if(!factory.has_value())
                return tl::unexpected(factory.error());

            return Wrapper<FactoryTy>(factory.value());
        }

        template<class FactoryTy = IDXGIFactory>
        tl::expected<Wrapper<FactoryTy>, HRESULT> Create1()
        {
            auto factory = Helpers::COM::IIDToObjectForwardFunction<FactoryTy>(&CreateDXGIFactory1);

            if(!factory.has_value())
                return tl::unexpected(factory.error());

            return Wrapper<FactoryTy>(factory.value());
        }

        template<class FactoryTy = IDXGIFactory>
        tl::expected<Wrapper<FactoryTy>, HRESULT> Create2(UINT flags)
        {
            auto factory = Helpers::COM::IIDToObjectForwardFunction<FactoryTy>(&CreateDXGIFactory2, flags);

            if(!factory.has_value())
                return tl::unexpected(factory.error());

            return Wrapper<FactoryTy>(factory.value());
        }
    }
}