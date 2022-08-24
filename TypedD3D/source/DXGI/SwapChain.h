#pragma once
#include "source/Wrappers.h"
#include "source/D3D12Wrappers.h"
#include "source/Internal/IUnknownWrapper.h"
#include "source/Internal/d3dConcepts.h"
#include "source/Helpers/COMHelpers.h"
#include <dxgi1_6.h>
#include <span>
#include <d3d12.h>
#include <memory>

struct ID3D12CommandQueue;

namespace TypedD3D::Internal
{
    namespace DXGI
    {
        template<class Ty>
        using SwapChain_t = IUnknownWrapper<Ty>;

        namespace SwapChain
        {
            template<class Ty>
            struct TraitsImpl;

            template<>
            struct TraitsImpl<IDXGISwapChain>
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
                        Helpers::ThrowIfFailed(Get().Present(SyncInterval, Flags));
                    }

                    template<Resource Ty>
                    Wrapper<Ty> GetBuffer(UINT buffer)
                    {
                        return Wrapper<Ty>(Helpers::COM::IIDToObjectForwardFunction<Ty>(&value_type::GetBuffer, Get(), buffer));
                    }

                    void SetFullscreenState(BOOL Fullscreen, IDXGIOutput* optTarget)
                    {
                        Helpers::ThrowIfFailed(Get().SetFullscreenState(Fullscreen, optTarget));
                    }

                    std::pair<BOOL, Microsoft::WRL::ComPtr<IDXGIOutput>> GetFullscreenState()
                    {
                        std::pair<BOOL, Microsoft::WRL::ComPtr<IDXGIOutput>> state;
                        Helpers::ThrowIfFailed(Get().GetFullscreenState(&state.first, &state.second));
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
                        Helpers::ThrowIfFailed(Get().ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags));
                    }

                    void ResizeTarget(const DXGI_MODE_DESC& pNewTargetParameters)
                    {
                        Helpers::ThrowIfFailed(Get().ResizeTarget(&pNewTargetParameters));
                    }

                    Microsoft::WRL::ComPtr<IDXGIOutput> GetContainingOutput()
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<IDXGIOutput>(&value_type::GetContainingOutput, Get()).value();
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
            struct TraitsImpl<IDXGISwapChain1>
            {
                using value_type = IDXGISwapChain1;
                using pointer = IDXGISwapChain1*;
                using const_pointer = const IDXGISwapChain1*;
                using reference = IDXGISwapChain1&;
                using cosnt_reference = const IDXGISwapChain1&;

                template<class DerivedSelf>
                class Interface : public TraitsImpl<IDXGISwapChain>::Interface<DerivedSelf>
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
                        Helpers::ThrowIfFailed(Get().GetFullscreenDesc(&desc));
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
                        Helpers::ThrowIfFailed(Get().GetCoreWindow(refiid, ppUnk));
                    }

                    void Present1(UINT SyncInterval, UINT PresentFlags, const DXGI_PRESENT_PARAMETERS& pPresentParameters)
                    {
                        Helpers::ThrowIfFailed(Get().Present1(SyncInterval, PresentFlags, &pPresentParameters));
                    }

                    BOOL IsTemporaryMonoSupported()
                    {
                        return Get().IsTemporaryMonoSupported();
                    }

                    Microsoft::WRL::ComPtr<IDXGIOutput> GetRestrictToOutput()
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<IDXGIOutput>(&value_type::GetRestrictToOutput, Get()).value();
                    }

                    void SetBackgroundColor(DXGI_RGBA pColor)
                    {
                        Helpers::ThrowIfFailed(Get().SetBackgroundColor(&pColor));
                    }

                    DXGI_RGBA GetBackgroundColor()
                    {
                        DXGI_RGBA color;
                        Helpers::ThrowIfFailed(Get().GetBackgroundColor(&color));
                        return color;
                    }

                    void SetRotation(DXGI_MODE_ROTATION Rotation)
                    {
                        Helpers::ThrowIfFailed(Get().SetRotation(Rotation));
                    }

                    DXGI_MODE_ROTATION GetRotation()
                    {
                        DXGI_MODE_ROTATION rotation;
                        Helpers::ThrowIfFailed(Get().GetRotation(rotation));
                        return rotation;
                    }
                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct TraitsImpl<IDXGISwapChain2>
            {
                using value_type = IDXGISwapChain2;
                using pointer = IDXGISwapChain2*;
                using const_pointer = const IDXGISwapChain2*;
                using reference = IDXGISwapChain2&;
                using cosnt_reference = const IDXGISwapChain2&;

                template<class DerivedSelf>
                class Interface : public TraitsImpl<IDXGISwapChain1>::Interface<DerivedSelf>
                {
                private:
                    using derived_self = DerivedSelf;

                public:
                    void SetSourceSize(UINT Width, UINT Height)
                    {
                        Helpers::ThrowIfFailed(Get().SetSourceSize(Width, Height));
                    }

                    std::pair<UINT, UINT> GetSourceSize()
                    {
                        std::pair<UINT, UINT> size;
                        Helpers::ThrowIfFailed(Get().GetSourceSize(&size.first, &size.second));
                        return size;
                    }

                    void SetMaximumFrameLatency(UINT MaxLatency)
                    {
                        Helpers::ThrowIfFailed(Get().SetMaximumFrameLatency(&MaxLatency));
                    }

                    UINT GetMaximumFrameLatency()
                    {
                        UINT latency;
                        Helpers::ThrowIfFailed(Get().GetMaximumFrameLatency(&latency));
                        return latency;
                    }

                    HANDLE GetFrameLatencyWaitableObject()
                    {
                        return Get().GetFrameLatencyWaitableObject();
                    }

                    void SetMatrixTransform(const DXGI_MATRIX_3X2_F& pMatrix)
                    {
                        Helpers::ThrowIfFailed(Get().SetMatrixTransform(&pMatrix));
                    }

                    DXGI_MATRIX_3X2_F GetMatrixTransform()
                    {
                        DXGI_MATRIX_3X2_F matrix;
                        Helpers::ThrowIfFailed(Get().GetMatrixTransform(&matrix));
                        return matrix;
                    }
                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };

            template<>
            struct TraitsImpl<IDXGISwapChain3>
            {
                using value_type = IDXGISwapChain3;
                using pointer = IDXGISwapChain3*;
                using const_pointer = const IDXGISwapChain3*;
                using reference = IDXGISwapChain3&;
                using cosnt_reference = const IDXGISwapChain3&;

                template<class DerivedSelf>
                class Interface : public TraitsImpl<IDXGISwapChain2>::Interface<DerivedSelf>
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
                        Helpers::ThrowIfFailed(Get().SetColorSpace1(ColorSpace));
                    }

                    template<std::derived_from<ID3D12CommandQueue> QueueTy>
                    void ResizeBuffers1(
                        UINT BufferCount,
                        UINT Width,
                        UINT Height,
                        DXGI_FORMAT Format,
                        UINT SwapChainFlags,
                        std::span<UINT> pCreationNodeMask,
                        std::span<Direct<QueueTy>> ppPresentQueue)
                    {
                        std::unique_ptr<IUnknown*[]> queues = std::make_unique<IUnknown*[]>(ppPresentQueue.size());

                        for(size_t i = 0; i < ppPresentQueue.size(); i++)
                        {
                            queues[i] = ppPresentQueue[i].Get();
                        }

                        Helpers::ThrowIfFailed(Get().ResizeBuffers1(BufferCount, Width, Height, Format, SwapChainFlags, pCreationNodeMask.data(), queues.get()));
                    }
                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };
        }
    }

    template<std::derived_from<IDXGISwapChain> Ty>
    struct Traits<Ty>
    {
        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using cosnt_reference = const Ty&;

        template<class DerivedSelf>
        using Interface = DXGI::SwapChain::TraitsImpl<Ty>::template Interface<DerivedSelf>;
    };

}


namespace TypedD3D::DXGI
{
    template<std::derived_from<IDXGISwapChain> Ty>
    using SwapChain_t = Internal::DXGI::SwapChain_t<Ty>;

    using SwapChain = SwapChain_t<IDXGISwapChain>;
    using SwapChain1 = SwapChain_t<IDXGISwapChain1>;
}