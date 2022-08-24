#pragma once
#include "source/Wrappers.h"
#include "source/D3D12Wrappers.h"
#include "source/Internal/IUnknownWrapper.h"
#include "source/Helpers/COMHelpers.h"
#include "source/Exceptions.h"

struct ID3D11Device;
struct ID3D12CommandQueue;

namespace TypedD3D::Internal
{
    namespace DXGI
    {
        template<class Ty>
        using Factory_t = IUnknownWrapper<Ty>;

        namespace Factory
        {
            template<class Ty>
            struct TraitsImpl;

            template<>
            struct TraitsImpl<IDXGIFactory>
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
                        auto adapter = Helpers::COM::UnknownObjectForwardFunction<IDXGIAdapter>(&value_type::EnumAdapters, Get(), Adapter);
                        if(!adapter.has_value())
                            return nullptr;

                        if constexpr(std::same_as<AdapterTy, IDXGIAdapter>)
                            return Wrapper<AdapterTy>(adapter.value());
                        else
                            return Wrapper<AdapterTy>(Helpers::COM::Cast<AdapterTy>(adapter.value()));
                    }

                    HRESULT MakeWindowAssociation(HWND WindowHandle, UINT Flags)
                    {
                        Get().MakeWindowAssociation(Flags);
                    }

                    HWND GetWindowAssociation()
                    {
                        HWND hwnd;
                        HRESULT result = Get().GetWindowAssociation(&hwnd);
                        if(FAILED(result))
                            Throw(result);
                        return hwnd;
                    }

                    template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D11Device> Device = ID3D11Device>
                    Wrapper<SwapChainTy> CreateSwapChain(Wrapper<Device> pDevice, const DXGI_SWAP_CHAIN_DESC& pDesc)
                    {
                        return Wrapper<SwapChainTy>(Helpers::COM::UnknownObjectForwardFunction<SwapChainTy>(&value_type::CreateSwapChain, Get(), pDevice.Get(), &pDesc));
                    }

                    template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
                    Wrapper<SwapChainTy> CreateSwapChain(Direct<QueueTy> commandQueue, const DXGI_SWAP_CHAIN_DESC& pDesc)
                    {
                        return Wrapper<SwapChainTy>(Helpers::COM::UnknownObjectForwardFunction<SwapChainTy>(&value_type::CreateSwapChain, Get(), commandQueue.Get(), &pDesc));
                    }

                    template<std::derived_from<IDXGIAdapter> AdapterTy>
                    Wrapper<AdapterTy> CreateSoftwareAdapter(HMODULE Module)
                    {
                        return Wrapper<AdapterTy>(Helpers::COM::UnknownObjectForwardFunction<IDXGIAdapter>(&value_type::CreateSoftwareAdapter, Get(), Module));
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct TraitsImpl<IDXGIFactory1>
            {
                using value_type = IDXGIFactory1;
                using pointer = IDXGIFactory1*;
                using const_pointer = const IDXGIFactory1*;
                using reference = IDXGIFactory1&;
                using cosnt_reference = const IDXGIFactory1&;

                template<class DerivedSelf>
                class Interface : public TraitsImpl<IDXGIFactory>::Interface<DerivedSelf>
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    template<class AdapterTy>
                        requires std::derived_from<AdapterTy, IDXGIAdapter>
                    Wrapper<AdapterTy> EnumAdapters1(UINT Adapter)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<AdapterTy>(&value_type::EnumAdapters1, Get(), Adapter).value_or(nullptr);
                        auto adapter = Helpers::COM::UnknownObjectForwardFunction<IDXGIAdapter1>(&value_type::EnumAdapters1, Get(), Adapter);
                        if(!adapter.has_value())
                            return nullptr;

                        if constexpr(std::same_as<AdapterTy, IDXGIAdapter>)
                            return Wrapper<AdapterTy>(adapter.value());
                        else
                            return Wrapper<AdapterTy>(Helpers::COM::Cast<AdapterTy>(adapter.value()));
                    }

                    BOOL IsCurrent() { return Get().IsCurrent(); }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct TraitsImpl<IDXGIFactory2>
            {
                using value_type = IDXGIFactory2;
                using pointer = IDXGIFactory2*;
                using const_pointer = const IDXGIFactory2*;
                using reference = IDXGIFactory2&;
                using cosnt_reference = const IDXGIFactory2&;

                template<class DerivedSelf>
                class Interface : public TraitsImpl<IDXGIFactory1>::Interface<DerivedSelf>
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    BOOL IsWindowedStereoEnabled() { return Get().IsWindowedStereoEnabled(); }

                    template< std::derived_from<IDXGISwapChain> SwapChain = IDXGISwapChain, std::derived_from<ID3D11Device> Device = ID3D11Device>
                    Wrapper<SwapChain> CreateSwapChainForHwnd(
                        Wrapper<Device> pDevice,
                        HWND hWnd,
                        const DXGI_SWAP_CHAIN_DESC1& pDesc,
                        const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
                        IDXGIOutput* optRestrictToOutput)
                    {
                        auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForHwnd, Get(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput);

                        if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                            return Wrapper<SwapChain>(swapChain);
                        else
                            return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain));
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
                        auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForHwnd, &Get(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput);

                        if constexpr(std::same_as<SwapChainTy, IDXGISwapChain1>)
                            return Wrapper<SwapChainTy>(swapChain);
                        else
                            return Wrapper<SwapChainTy>(Helpers::COM::Cast<SwapChainTy>(swapChain));
                    }

                    template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
                        requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
                    Wrapper<SwapChain> CreateSwapChainForCoreWindow(
                        Wrapper<Device> pDevice,
                        IUnknown& pWindow,
                        const DXGI_SWAP_CHAIN_DESC1& pDesc,
                        IDXGIOutput* optRestrictToOutput)
                    {
                        auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForCoreWindow, Get(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput);

                        if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                            return Wrapper<SwapChain>(swapChain);
                        else
                            return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain));
                    }

                    template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, class QueueTy>
                        requires std::same_as<typename QueueTy::value_type, ID3D12CommandQueue>
                    Wrapper<SwapChainTy> CreateSwapChainForCoreWindow(
                        QueueTy pDevice,
                        IUnknown& pWindow,
                        const DXGI_SWAP_CHAIN_DESC1& pDesc,
                        IDXGIOutput* optRestrictToOutput)
                    {
                        auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForCoreWindow, Get(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput);

                        if constexpr(std::same_as<SwapChainTy, IDXGISwapChain1>)
                            return Wrapper<SwapChainTy>(swapChain);
                        else
                            return Wrapper<SwapChainTy>(Helpers::COM::Cast<SwapChainTy>(swapChain));
                    }

                    LUID GetSharedResourceAdapterLuid(HANDLE hResource)
                    {
                        LUID luid;
                        HRESULT result = Get().GetSharedResourceAdapterLuid(hResource, &luid);
                        if(FAILED(result))
                            Throw(result);
                        return luid;
                    }

                    DWORD RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg)
                    {
                        DWORD pdwCookie;
                        HRESULT result = Get().RegisterStereoStatusWindow(WindowHandle, wMsg, &pdwCookie);
                        if(FAILED(result))
                            Throw(result);
                        return pdwCookie;
                    }

                    DWORD RegisterStereoStatusEvent(HANDLE hEvent)
                    {
                        DWORD pdwCookie;
                        HRESULT result = Get().RegisterStereoStatusEvent(hEvent, &pdwCookie);
                        if(FAILED(result))
                            Throw(result);
                        return pdwCookie;
                    }

                    void UnregisterStereoStatus(DWORD dwCookie)
                    {
                        Get().UnregisterStereoStatus(dwCookie);
                    }

                    DWORD RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg)
                    {
                        DWORD pdwCookie;
                        HRESULT result = Get().RegisterOcclusionStatusWindow(WindowHandle, wMsg, &pdwCookie);
                        if(FAILED(result))
                            Throw(result);
                        return pdwCookie;
                    }

                    DWORD RegisterOcclusionStatusEvent(HANDLE hEvent)
                    {
                        DWORD pdwCookie;
                        HRESULT result = Get().RegisterOcclusionStatusEvent(hEvent, &pdwCookie);
                        if(FAILED(result))
                            Throw(result);
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
                        auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForComposition, Get(), pDevice.Get(), &pDesc, optRestrictToOutput);

                        if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                            return Wrapper<SwapChain>(swapChain);
                        else
                            return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain));
                    }

                    template<std::derived_from<IDXGISwapChain> SwapChainTy = IDXGISwapChain, std::derived_from<ID3D12CommandQueue> QueueTy>
                    Wrapper<SwapChainTy> CreateSwapChainForComposition(
                        Direct<QueueTy> pDevice,
                        const DXGI_SWAP_CHAIN_DESC1& pDesc,
                        IDXGIOutput* optRestrictToOutput)
                    {
                        auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&value_type::CreateSwapChainForComposition, Get(), pDevice.Get(), &pDesc, optRestrictToOutput);

                        if constexpr(std::same_as<SwapChainTy, IDXGISwapChain1>)
                            return Wrapper<SwapChainTy>(swapChain);
                        else
                            return Wrapper<SwapChainTy>(Helpers::COM::Cast<SwapChainTy>(swapChain));
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };
        }
    }

    template<std::derived_from<IDXGIFactory> Ty>
    struct Traits<Ty>
    {
        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using cosnt_reference = const Ty&;

        template<class DerivedSelf>
        using Interface = DXGI::Factory::TraitsImpl<Ty>::template Interface<DerivedSelf>;
    };
}

namespace TypedD3D::DXGI
{
    template<std::derived_from<IDXGIFactory> Ty>
    using Factory_t = Internal::DXGI::Factory_t<Ty>;

    namespace Factory
    {
        template<class FactoryTy = IDXGIFactory>
        Wrapper<FactoryTy> Create()
        {
            auto factory = Helpers::COM::IIDToObjectForwardFunction<FactoryTy>(&CreateDXGIFactory);

            return Wrapper<FactoryTy>(factory);
        }

        template<class FactoryTy = IDXGIFactory>
        Wrapper<FactoryTy> Create1()
        {
            auto factory = Helpers::COM::IIDToObjectForwardFunction<FactoryTy>(&CreateDXGIFactory1);

            return Wrapper<FactoryTy>(factory);
        }

        template<class FactoryTy = IDXGIFactory>
        Wrapper<FactoryTy> Create2(UINT flags)
        {
            auto factory = Helpers::COM::IIDToObjectForwardFunction<FactoryTy>(&CreateDXGIFactory2, flags);

            return Wrapper<FactoryTy>(factory);
        }
    }
}