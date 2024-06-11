module;

#include <dxgi1_6.h>
#include <type_traits>

#include <d3d12.h>
#include <wrl/client.h>

export module TypedD3D.Legacy.DXGIHelpers;
import TypedD3D.Shared;

export namespace TypedD3D::Helpers::DXGI
{
    using Microsoft::WRL::ComPtr;
    namespace Factory
    {
        enum class CreationFlags
        {
            None = 0,
            Enable_Debug = DXGI_CREATE_FACTORY_DEBUG
        };

        DEFINE_ENUM_FLAG_OPERATORS(CreationFlags);

        template<class Factory = IDXGIFactory>
        ComPtr<Factory> Create(CreationFlags flags)
        {

            if constexpr(std::is_same_v<Factory, IDXGIFactory>)
            {
                return IIDToObjectForwardFunction<IDXGIFactory>(&CreateDXGIFactory);
            }
            else if constexpr(std::is_same_v<Factory, IDXGIFactory1>)
            {
                return IIDToObjectForwardFunction<IDXGIFactory1>(&CreateDXGIFactory1);
            }
            else
            {
                if constexpr(std::is_same_v<Factory, IDXGIFactory2>)
                {
                    return IIDToObjectForwardFunction<IDXGIFactory2>(&CreateDXGIFactory2, static_cast<UINT>(flags));
                }
                else
                {
                    return Cast<Factory>(IIDToObjectForwardFunction<IDXGIFactory2>(&CreateDXGIFactory2, static_cast<UINT>(flags)));
                }
            }
        }
    }

    namespace SwapChain
    {
        template<class SwapChain = IDXGISwapChain1>
        ComPtr<SwapChain> Create(IDXGIFactory2& factory, IUnknown& device, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1& desc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc = nullptr, IDXGIOutput* optRestrictToOutput = nullptr)
        {
            ComPtr<IDXGISwapChain1> swapChain;
            return Cast<SwapChain>(factory.CreateSwapChainForHwnd(&device, hWnd, &desc, optFullscreenDesc, optRestrictToOutput, &swapChain));
        }

        template<class SwapChain = IDXGISwapChain1>
        ComPtr<SwapChain> CreateFlipDiscard(IDXGIFactory2& factory, IUnknown& device, HWND hWnd, DXGI_FORMAT format, UINT bufferCount, DXGI_SWAP_CHAIN_FLAG flags, bool fullScreen, IDXGIOutput* optRestrictToOutput = nullptr)
        {
            DXGI_SWAP_CHAIN_DESC1 desc{};
            desc.Width = 0;
            desc.Height = 0;
            desc.Format = format;
            desc.Stereo = false;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT;
            desc.BufferCount = (bufferCount < 2) ? 2 : bufferCount;
            desc.Scaling = DXGI_SCALING_NONE;
            desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

            //Through testing, this value with flip discard can only work with DXGI_ALPHA_MODE_UNSPECIFIED or DXGI_ALPHA_MODE_IGNORE
            desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
            desc.Flags = flags | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc{};
            fullscreenDesc.RefreshRate.Denominator = fullscreenDesc.RefreshRate.Numerator = 0;
            fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            fullscreenDesc.Windowed = !fullScreen;

            return Create<SwapChain>(factory, device, hWnd, desc, &fullscreenDesc, optRestrictToOutput);
        }

        template<class Resource = ID3D12Resource>
        ComPtr<Resource> GetBuffer(IDXGISwapChain& swapChain, UINT index)
        {
            return IIDToObjectForwardFunction<Resource>(&IDXGISwapChain::GetBuffer, swapChain, index);
        }
    };
}
