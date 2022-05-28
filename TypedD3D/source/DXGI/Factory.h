#pragma once
#include "../Wrappers.h"
#include "../Internal/ComWrapper.h"
#include "../Utils.h"
#include "../Helpers/COMHelpers.h"
#include "../D3D12TypeTags.h"
#include <dxgi1_6.h>

class ID3D11Device;
struct ID3D12CommandQueue;

namespace TypedD3D::Internal
{
    namespace DXGI::Factory
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
            template<class AdapterTy>
                requires std::derived_from<AdapterTy, IDXGIAdapter>
            Wrapper<AdapterTy> EnumAdapters(UINT Adapter)
            {
                auto adapter = Helpers::COM::IIDToObjectForwardFunction<IDXGIAdapter>(&IDXGIFactory::EnumAdapters, InternalGet(), Adapter);
                if(!adapter.HasValue())
                    return nullptr;

                if constexpr(std::same_as<AdapterTy, IDXGIAdapter>)
                    return Wrapper<AdapterTy>(adapter.GetValue());
                else
                    return Wrapper<AdapterTy>(Helpers::COM::Cast<AdapterTy>(adapter.GetValue()));
            }

            HRESULT MakeWindowAssociation(HWND WindowHandle, UINT Flags)
            {
                InternalGet().MakeWindowAssociation(Flags);
            }

            Utils::Expected<HWND, HRESULT> GetWindowAssociation()
            {
                HWND hwnd;
                HRESULT result = InternalGet().GetWindowAssociation(&hwnd);
                if(FAILED(result))
                    return Utils::Unexpected(result);
                return hwnd;
            }

            template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
                requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
            Utils::Expected<Wrapper<SwapChain>, HRESULT> CreateSwapChain(Wrapper<Device> pDevice, const DXGI_SWAP_CHAIN_DESC& pDesc)
            {
                auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain>(&IDXGIFactory::CreateSwapChain, InternalGet(), pDevice.Get(), &pDesc);

                if(!swapChain.HasValue())
                    return Utils::Unexpected(swapChain.GetError());

                if constexpr(std::same_as<SwapChain, IDXGISwapChain>)
                    return Wrapper<SwapChain>(swapChain.GetValue());
                else
                    return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.GetValue()));
            }

            template<class SwapChain = IDXGISwapChain>
                requires std::derived_from<SwapChain, IDXGISwapChain>
            Utils::Expected<Wrapper<SwapChain>, HRESULT> CreateSwapChain(Direct<ID3D12CommandQueue> commandQueue, const DXGI_SWAP_CHAIN_DESC& pDesc)
            {
                auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain>(&IDXGIFactory::CreateSwapChain, InternalGet(), commandQueue.Get(), &pDesc);

                if(!swapChain.HasValue())
                    return Utils::Unexpected(swapChain.GetError());

                if constexpr(std::same_as<SwapChain, IDXGISwapChain>)
                    return Wrapper<SwapChain>(swapChain.GetValue());
                else
                    return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.GetValue()));
            }

            template<class AdapterTy>
                requires std::derived_from<AdapterTy, IDXGIAdapter>
            Utils::Expected<Wrapper<AdapterTy>, HRESULT> CreateSoftwareAdapter(HMODULE Module)
            {
                auto adapter = Helpers::COM::UnknownObjectForwardFunction<IDXGIAdapter>(&IDXGIFactory::CreateSoftwareAdapter, InternalGet(), Module);
                if(!adapter.HasValue())
                    return nullptr;

                if constexpr(std::same_as<AdapterTy, IDXGIAdapter>)
                    return Wrapper<AdapterTy>(adapter.GetValue());
                else
                    return Wrapper<AdapterTy>(Helpers::COM::Cast<AdapterTy>(adapter.GetValue()));
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
                auto adapter = Helpers::COM::IIDToObjectForwardFunction<IDXGIAdapter1>(&IDXGIFactory1::EnumAdapters1, InternalGet(), Adapter);
                if(!adapter.HasValue())
                    return nullptr;

                if constexpr(std::same_as<AdapterTy, IDXGIAdapter>)
                    return Wrapper<AdapterTy>(adapter.GetValue());
                else
                    return Wrapper<AdapterTy>(Helpers::COM::Cast<AdapterTy>(adapter.GetValue()));
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
                requires std::derived_from<Device, ID3D11Device> && std::derived_from<SwapChain, IDXGISwapChain>
            Utils::Expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForHwnd(
                Wrapper<Device> pDevice,
                HWND hWnd,
                const DXGI_SWAP_CHAIN_DESC1& pDesc,
                const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
                IDXGIOutput* optRestrictToOutput)
            {
                auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForHwnd, InternalGet(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput);

                if(!swapChain.HasValue())
                    return Utils::Unexpected(swapChain.GetError());

                if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                    return Wrapper<SwapChain>(swapChain.GetValue());
                else
                    return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.GetValue()));
            }

            template<class SwapChain = IDXGISwapChain>
                requires std::derived_from<SwapChain, IDXGISwapChain>
            Utils::Expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForHwnd(
                Direct<ID3D12CommandQueue> pDevice,
                HWND hWnd,
                const DXGI_SWAP_CHAIN_DESC1& pDesc,
                const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc,
                IDXGIOutput* optRestrictToOutput)
            {
                auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForHwnd, &InternalGet(), pDevice.Get(), hWnd, &pDesc, optFullscreenDesc, optRestrictToOutput);

                if(!swapChain.HasValue())
                    return Utils::Unexpected(swapChain.GetError());

                if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                    return Wrapper<SwapChain>(swapChain.GetValue());
                else
                    return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.GetValue()));
            }

            template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
                requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
            Utils::Expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForCoreWindow(
                Wrapper<Device> pDevice,
                IUnknown& pWindow,
                const DXGI_SWAP_CHAIN_DESC1& pDesc,
                IDXGIOutput* optRestrictToOutput)
            {
                auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForCoreWindow, InternalGet(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput);

                if(!swapChain.HasValue())
                    return Utils::Unexpected(swapChain.GetError());

                if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                    return Wrapper<SwapChain>(swapChain.GetValue());
                else
                    return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.GetValue()));
            }

            template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
                requires std::derived_from<SwapChain, IDXGISwapChain>
            Utils::Expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForCoreWindow(
                Direct<ID3D12CommandQueue> pDevice,
                IUnknown& pWindow,
                const DXGI_SWAP_CHAIN_DESC1& pDesc,
                IDXGIOutput* optRestrictToOutput)
            {
                auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForCoreWindow, InternalGet(), pDevice.Get(), &pWindow, &pDesc, optRestrictToOutput);

                if(!swapChain.HasValue())
                    return Utils::Unexpected(swapChain.GetError());

                if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                    return Wrapper<SwapChain>(swapChain.GetValue());
                else
                    return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.GetValue()));
            }

            Utils::Expected<LUID, HRESULT> GetSharedResourceAdapterLuid(HANDLE hResource)
            {
                LUID luid;
                HRESULT result = InternalGet().GetSharedResourceAdapterLuid(hResource, &luid);
                if(FAILED(result))
                    return Utils::Unexpected(result);
                return luid;
            }

            Utils::Expected<DWORD, HRESULT> RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg)
            {
                DWORD pdwCookie;
                HRESULT result = InternalGet().RegisterStereoStatusWindow(WindowHandle, wMsg, &pdwCookie);
                if(FAILED(result))
                    return Utils::Unexpected(result);
                return pdwCookie;
            }

            Utils::Expected<DWORD, HRESULT> RegisterStereoStatusEvent(HANDLE hEvent)
            {
                DWORD pdwCookie;
                HRESULT result = InternalGet().RegisterStereoStatusEvent(hEvent, &pdwCookie);
                if(FAILED(result))
                    return Utils::Unexpected(result);
                return pdwCookie;
            }

            void UnregisterStereoStatus(DWORD dwCookie)
            {
                InternalGet().UnregisterStereoStatus(dwCookie);
            }

            Utils::Expected<DWORD, HRESULT> RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg)
            {
                DWORD pdwCookie;
                HRESULT result = InternalGet().RegisterOcclusionStatusWindow(WindowHandle, wMsg, &pdwCookie);
                if(FAILED(result))
                    return Utils::Unexpected(result);
                return pdwCookie;
            }

            Utils::Expected<DWORD, HRESULT> RegisterOcclusionStatusEvent(HANDLE hEvent)
            {
                DWORD pdwCookie;
                HRESULT result = InternalGet().RegisterOcclusionStatusEvent(hEvent, &pdwCookie);
                if(FAILED(result))
                    return Utils::Unexpected(result);
                return pdwCookie;
            }

            void UnregisterOcclusionStatus(DWORD dwCookie)
            {
                InternalGet().UnregisterOcclusionStatus(dwCookie);
            }

            template<class SwapChain = IDXGISwapChain, class Device = ID3D11Device>
                requires std::derived_from<Device, ID3D11Device>&& std::derived_from<SwapChain, IDXGISwapChain>
            Utils::Expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForComposition(
                Wrapper<Device> pDevice,
                const DXGI_SWAP_CHAIN_DESC1& pDesc,
                IDXGIOutput* optRestrictToOutput)
            {
                auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForComposition, InternalGet(), pDevice.Get(), &pDesc, optRestrictToOutput);

                if(!swapChain.HasValue())
                    return Utils::Unexpected(swapChain.GetError());

                if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                    return Wrapper<SwapChain>(swapChain.GetValue());
                else
                    return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.GetValue()));
            }

            template<class SwapChain = IDXGISwapChain>
                requires std::derived_from<SwapChain, IDXGISwapChain>
            Utils::Expected<Wrapper<SwapChain>, HRESULT> CreateSwapChainForComposition(
                Direct<ID3D12CommandQueue> pDevice,
                const DXGI_SWAP_CHAIN_DESC1& pDesc,
                IDXGIOutput* optRestrictToOutput)
            {
                auto swapChain = Helpers::COM::UnknownObjectForwardFunction<IDXGISwapChain1>(&IDXGIFactory2::CreateSwapChainForComposition, InternalGet(), pDevice.Get(), &pDesc, optRestrictToOutput);

                if(!swapChain.HasValue())
                    return Utils::Unexpected(swapChain.GetError());

                if constexpr(std::same_as<SwapChain, IDXGISwapChain1>)
                    return Wrapper<SwapChain>(swapChain.GetValue());
                else
                    return Wrapper<SwapChain>(Helpers::COM::Cast<SwapChain>(swapChain.GetValue()));
            }

        private:
            wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
            factory_type& InternalGet() { return *ToDerived().Get(); }
        };
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
            ComWrapper<DirectXClass>::ComWrapper(other.GetComPtr())
        {

        }

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };

    template<class Ty>
    using Factory_t = InterfaceWrapper<Ty>;
}

namespace TypedD3D::DXGI
{
    template<class Ty>
    using Factory_t = Internal::Factory_t<Ty>;

    namespace Factory
    {
        template<class FactoryTy = IDXGIFactory>
        Utils::Expected<Wrapper<FactoryTy>, HRESULT> Create()
        {
            auto factory = Helpers::COM::IIDToObjectForwardFunction<FactoryTy>(&CreateDXGIFactory);

            if(!factory.HasValue())
                return Utils::Unexpected(factory.GetError());

            return Wrapper<FactoryTy>(factory.GetValue());
        }

        template<class FactoryTy = IDXGIFactory>
        Utils::Expected<Wrapper<FactoryTy>, HRESULT> Create1()
        {
            auto factory = Helpers::COM::IIDToObjectForwardFunction<FactoryTy>(&CreateDXGIFactory1);

            if(!factory.HasValue())
                return Utils::Unexpected(factory.GetError());

            return Wrapper<FactoryTy>(factory.GetValue());
        }

        template<class FactoryTy = IDXGIFactory>
        Utils::Expected<Wrapper<FactoryTy>, HRESULT> Create2(UINT flags)
        {
            auto factory = Helpers::COM::IIDToObjectForwardFunction<FactoryTy>(&CreateDXGIFactory2, flags);

            if(!factory.HasValue())
                return Utils::Unexpected(factory.GetError());

            return Wrapper<FactoryTy>(factory.GetValue());
        }
    }
}