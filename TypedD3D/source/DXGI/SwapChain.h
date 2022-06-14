#pragma once
#include "../Wrappers.h"
#include "../Internal/ComWrapper.h"
#include "../Internal/d3dConcepts.h"
#include "expected.hpp"
#include "../Helpers/COMHelpers.h"
#include "../D3D12TypeTags.h"
#include <dxgi1_6.h>
#include <span>

struct ID3D12CommandQueue;
namespace TypedD3D::Internal
{
    namespace DXGI
    {
        template<class Ty>
        using SwapChain_t = InterfaceWrapper<Ty>;


        template<class Ty>
        using Direct = Wrapper<Ty, TypeTag::Direct>;

        namespace SwapChain
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

                template<Resource Ty>
                tl::expected<Wrapper<Ty>, HRESULT> GetBuffer(UINT buffer)
                {
                    tl::expected<Microsoft::WRL::ComPtr<Ty>, HRESULT> resource = Helpers::COM::IIDToObjectForwardFunction<Ty>(&swap_chain_type::GetBuffer, InternalGet(), buffer);

                    if(!resource.has_value())
                        return tl::unexpected(resource.error());

                    return Wrapper<Ty>(resource.value());
                }

                HRESULT SetFullscreenState(BOOL Fullscreen, IDXGIOutput* optTarget)
                {
                    return InternalGet().SetFullscreenState(Fullscreen, optTarget);
                }

                std::pair<bool, Microsoft::WRL::ComPtr<IDXGIOutput>> GetFullscreenState()
                {
                    std::pair<bool, Microsoft::WRL::ComPtr<IDXGIOutput>> state;
                    InternalGet().GetFullscreenState(&state.first, &state.second);
                    return state;
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

                HRESULT ResizeTarget(const DXGI_MODE_DESC& pNewTargetParameters)
                {
                    return InternalGet().ResizeTarget(&pNewTargetParameters);
                }

                Microsoft::WRL::ComPtr<IDXGIOutput> GetContainingOutput()
                {
                    return Helpers::COM::UnknownObjectForwardFunction<IDXGIOutput>(&swap_chain_type::GetContainingOutput, InternalGet()).value();
                }

                DXGI_FRAME_STATISTICS GetFrameStatistics()
                {
                    DXGI_FRAME_STATISTICS stats;
                    InternalGet().GetFrameStatistics(stats);
                    return stats;
                }

                UINT GetLastPresentCount()
                {
                    UINT LastPresentCount;
                    InternalGet().GetLastPresentCount(&LastPresentCount);
                    return LastPresentCount;
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
                DXGI_SWAP_CHAIN_DESC1 GetDesc1()
                {
                    DXGI_SWAP_CHAIN_DESC1 desc;
                    InternalGet().GetDesc1(&desc);
                    return desc;
                }

                DXGI_SWAP_CHAIN_FULLSCREEN_DESC GetFullscreenDesc()
                {
                    DXGI_SWAP_CHAIN_FULLSCREEN_DESC desc;
                    HRESULT result = InternalGet().GetFullscreenDesc(&desc);
                    return desc;
                }

                HWND GetHwnd()
                {
                    HWND hwnd;
                    InternalGet().GetHwnd(&hwnd);
                    return hwnd;
                }

                HRESULT GetCoreWindow(REFIID refiid, void** ppUnk)
                {
                    return InternalGet().GetCoreWindow(refiid, ppUnk);
                }

                HRESULT Present1(UINT SyncInterval, UINT PresentFlags, const DXGI_PRESENT_PARAMETERS& pPresentParameters)
                {
                    return InternalGet().Present1(SyncInterval, PresentFlags, &pPresentParameters);
                }

                BOOL IsTemporaryMonoSupported()
                {
                    return InternalGet().IsTemporaryMonoSupported();
                }

                Microsoft::WRL::ComPtr<IDXGIOutput> GetRestrictToOutput()
                {
                    return Helpers::COM::UnknownObjectForwardFunction<IDXGIOutput>(&swap_chain_type::GetRestrictToOutput, InternalGet()).value();
                }
                HRESULT SetBackgroundColor(DXGI_RGBA pColor)
                {
                    return InternalGet().SetBackgroundColor(&pColor);
                }

                tl::expected<DXGI_RGBA, HRESULT> GetBackgroundColor(DXGI_RGBA* pColor)
                {
                    DXGI_RGBA color;
                    HRESULT result = InternalGet().GetBackgroundColor(&color);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return color;
                }

                HRESULT SetRotation(DXGI_MODE_ROTATION Rotation)
                {
                    return InternalGet().SetRotation(Rotation);
                }

                tl::expected<DXGI_MODE_ROTATION, HRESULT> GetRotation()
                {
                    DXGI_MODE_ROTATION rotation;
                    HRESULT result = InternalGet().GetRotation(rotation);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return rotation;
                }
            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                swap_chain_type& InternalGet() { return *ToDerived().Get(); }
            };

            template<class WrapperTy>
            class Interface<WrapperTy, IDXGISwapChain3> : public Interface<WrapperTy, IDXGISwapChain2>
            {
            private:
                using swap_chain_type = IDXGISwapChain3;
                using wrapper_type = WrapperTy;

            public:
                UINT GetCurrentBackBufferIndex()
                {
                    return InternalGet().GetCurrentBackBufferIndex();
                }

                DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG CheckColorSpaceSupport(DXGI_COLOR_SPACE_TYPE ColorSpace)
                {
                    UINT colorSupport;
                    InternalGet().CheckColorSpaceSupport(ColorSpace, &colorSupport);
                    return static_cast<DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG>(colorSupport);
                }

                HRESULT SetColorSpace1(DXGI_COLOR_SPACE_TYPE ColorSpace)
                {
                    return InternalGet().SetColorSpace1(ColorSpace);
                }

                template<std::derived_from<ID3D12CommandQueue> QueueTy>
                HRESULT ResizeBuffers1(
                    UINT BufferCount,
                    UINT Width,
                    UINT Height,
                    DXGI_FORMAT Format,
                    UINT SwapChainFlags,
                    std::span<UINT> pCreationNodeMask,
                    std::span<Direct<QueueTy>> ppPresentQueue)
                {
                    std::unique_ptr<IUnknown[]> queues = std::make_unique<IUnknown[]>(ppPresentQueue.size());

                    for(size_t i = 0; i < ppPresentQueue.size(); i++)
                    {
                        queues[i] = ppPresentQueue[i].Get();
                    }

                    return InternalGet().ResizeBuffers1(BufferCount, Width, Height, Format, SwapChainFlags, pCreationNodeMask.data(), queues.get());
                }
            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                swap_chain_type& InternalGet() { return *ToDerived().Get(); }
            };

            template<class WrapperTy>
            class Interface<WrapperTy, IDXGISwapChain2> : public Interface<WrapperTy, IDXGISwapChain1>
            {
            private:
                using swap_chain_type = IDXGISwapChain2;
                using wrapper_type = WrapperTy;

            public:
                HRESULT SetSourceSize(UINT Width, UINT Height)
                {
                    return InternalGet().SetSourceSize(Width, Height);
                }

                tl::expected<std::pair<UINT, UINT>, HRESULT> GetSourceSize()
                {
                    std::pair<UINT, UINT> size;
                    HRESULT result = InternalGet().GetSourceSize(&size.first, &size.second);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return size;
                }

                HRESULT SetMaximumFrameLatency(UINT MaxLatency)
                {
                    return InternalGet().SetMaximumFrameLatency(&MaxLatency);
                }

                tl::expected<UINT, HRESULT> STDMETHODCALLTYPE GetMaximumFrameLatency()
                {
                    UINT latency;
                    HRESULT result = InternalGet().GetMaximumFrameLatency(&latency);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return latency;
                }

                HANDLE GetFrameLatencyWaitableObject() 
                { 
                    return InternalGet().GetFrameLatencyWaitableObject(); 
                }

                HRESULT SetMatrixTransform(const DXGI_MATRIX_3X2_F& pMatrix)
                {
                    return InternalGet().SetMatrixTransform(&pMatrix);
                }

                tl::expected<DXGI_MATRIX_3X2_F, HRESULT> STDMETHODCALLTYPE GetMatrixTransform()
                {
                    DXGI_MATRIX_3X2_F matrix;
                    HRESULT result = InternalGet().GetMatrixTransform(&matrix);
                    if(FAILED(result))
                        return tl::unexpected(result);
                    return matrix;
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                swap_chain_type& InternalGet() { return *ToDerived().Get(); }
            };
        }

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
    using SwapChain_t = Internal::DXGI::SwapChain_t<Ty>;
    using SwapChain = SwapChain_t<IDXGISwapChain>;
    using SwapChain1 = SwapChain_t<IDXGISwapChain1>;
}