#pragma once
#include "../Wrappers.h"
#include "../Internal/ComWrapper.h"
#include "../Utils.h"
#include "../Helpers/COMHelpers.h"
#include <dxgi1_6.h>

namespace TypedD3D::Internal
{
    namespace DXGI::SwapChain
    {
        template<class WrapperTy, class SwapChainTy>
        class Interface;
        
        template<class WrapperTy>
        class Interface<WrapperTy, IDXGISwapChain>
        {
        private:
            using swap_chain_type = IDXGISwapChain;
            using wrapper_type = WrapperTy;

        public:
            HRESULT Present(UINT SyncInterval, UINT Flags)
            {
                return InternalGet().Present(SyncInterval, Flags);
            }

            HRESULT GetBuffer(REFIID riid, void** ppSurface)
            {
                return InternalGet().GetBuffer(riid, ppSurface);
            }

            HRESULT SetFullscreenState(BOOL Fullscreen, IDXGIOutput* pTarget)
            {
                return InternalGet().SetFullscreenState(Fullscreen, pTarget);
            }

            HRESULT GetFullscreenState(BOOL* pFullscreen, IDXGIOutput** ppTarget)
            {
                return InternalGet().GetFullscreenState(pFullscreen, ppTarget);
            }

            DXGI_SWAP_CHAIN_DESC GetDesc()
            {
                DXGI_SWAP_CHAIN_DESC desc;
                InternalGet().GetDesc(&desc);
                return desc;
            }

            HRESULT ResizeBuffers(
                UINT BufferCount,
                UINT Width,
                UINT Height,
                DXGI_FORMAT NewFormat,
                UINT SwapChainFlags)
            {
                return InternalGet().ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
            }

            HRESULT ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters)
            {
                return InternalGet().ResizeTarget(pNewTargetParameters);
            }

            HRESULT GetContainingOutput(IDXGIOutput** ppOutput)
            {
                return InternalGet().GetContainingOutput(ppOutput);
            }

            HRESULT GetFrameStatistics(DXGI_FRAME_STATISTICS* pStats)
            {
                return InternalGet().GetFrameStatistics(pStats);
            }

            HRESULT GetLastPresentCount(UINT* pLastPresentCount)
            {
                return InternalGet().GetLastPresentCount(pLastPresentCount);
            }
        private:
            wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
            swap_chain_type& InternalGet() { return *ToDerived().Get(); }
        };
        
        template<class WrapperTy>
        class Interface<WrapperTy, IDXGISwapChain1> : public Interface<WrapperTy, IDXGISwapChain>
        {
        private:
            using swap_chain_type = IDXGISwapChain1;
            using wrapper_type = WrapperTy;

        public:
            DXGI_SWAP_CHAIN_DESC1 GetDesc()
            {
                DXGI_SWAP_CHAIN_DESC1 desc;
                InternalGet().GetDesc1(&desc);
                return desc;
            }

            HRESULT GetFullscreenDesc(DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pDesc)
            {
                return InternalGet().GetFullscreenDesc(pDesc);
            }

            HRESULT GetHwnd(HWND* pHwnd)
            {
                return InternalGet().GetHwnd(pHwnd);
            }

            HRESULT GetCoreWindow(REFIID refiid, void** ppUnk)
            {
                return InternalGet().GetCoreWindow(refiid, ppUnk);
            }

            HRESULT Present1(UINT SyncInterval, UINT PresentFlags, const DXGI_PRESENT_PARAMETERS* pPresentParameters)
            {
                return InternalGet().Present1(SyncInterval, PresentFlags, pPresentParameters);
            }

            BOOL IsTemporaryMonoSupported()
            {
                return InternalGet().IsTemporaryMonoSupported();
            }

            HRESULT GetRestrictToOutput(IDXGIOutput** ppRestrictToOutput)
            {
                return InternalGet().GetRestrictToOutput(ppRestrictToOutput);
            }
            HRESULT SetBackgroundColor(const DXGI_RGBA* pColor)
            {
                return InternalGet().SetBackgroundColor(pColor);
            }

            HRESULT GetBackgroundColor(DXGI_RGBA* pColor)
            {
                return InternalGet().GetBackgroundColor(pColor);
            }

            HRESULT SetRotation(DXGI_MODE_ROTATION Rotation)
            {
                return InternalGet().SetRotation(Rotation);
            }

            HRESULT GetRotation(DXGI_MODE_ROTATION* pRotation)
            {
                return InternalGet().GetRotation(pRotation);
            }
        private:
            wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
            swap_chain_type& InternalGet() { return *ToDerived().Get(); }
        };

    }

    template<class DirectXClass>
        requires std::derived_from<DirectXClass, IDXGISwapChain>
    class InterfaceWrapper<DirectXClass> : public ComWrapper<DirectXClass>, private DXGI::SwapChain::Interface<InterfaceWrapper<DirectXClass>, DirectXClass>
    {
    private:
        using Interface = DXGI::SwapChain::Interface<InterfaceWrapper<DirectXClass>, DirectXClass>;

        template<class WrapperTy, class SwapChainTy>
        friend class DXGI::SwapChain::Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = DirectXClass;

    public:
        using ComWrapper<DirectXClass>::ComWrapper;

        template<class DerivedSwapChainTy>
            requires std::convertible_to<DerivedSwapChainTy*, DirectXClass*>
        InterfaceWrapper(const InterfaceWrapper<DerivedSwapChainTy>& other) :
            ComWrapper<DirectXClass>::ComWrapper(other.GetComPtr())
        {

        }

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}
