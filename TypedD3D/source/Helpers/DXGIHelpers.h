//*********************************************************
//
// Copyright (c) 2022 Renzy Alarcon
// Licensed under the MIT License (MIT).
//
//*********************************************************

#pragma once
#include "COMHelpers.h"
#include "../Utils.h"
#include <dxgi1_6.h>
#include <type_traits>

namespace TypedD3D::Helpers::DXGI
{
    namespace Factory
    {
        enum class CreationFlags
        {
            None = 0,
            Enable_Debug = DXGI_CREATE_FACTORY_DEBUG
        };

        DEFINE_ENUM_FLAG_OPERATORS(CreationFlags);

        template<class Factory = IDXGIFactory>
        Utils::Expected<Microsoft::WRL::ComPtr<Factory>, HRESULT> Create(CreationFlags flags)
        {
            using Microsoft::WRL::ComPtr;

            if constexpr(std::is_same_v<Factory, IDXGIFactory>)
            {
                return COM::IIDToObjectForwardFunction<IDXGIFactory>(&CreateDXGIFactory);
            }
            else if constexpr(std::is_same_v<Factory, IDXGIFactory1>)
            {
                return COM::IIDToObjectForwardFunction<IDXGIFactory1>(&CreateDXGIFactory1);
            }
            else
            {
                if constexpr(std::is_same_v<Factory, IDXGIFactory2>)
                {
                    return COM::IIDToObjectForwardFunction<IDXGIFactory2>(&CreateDXGIFactory2, static_cast<UINT>(flags));
                }
                else
                {
                    Utils::Expected<ComPtr<IDXGIFactory2>, HRESULT> factory = COM::IIDToObjectForwardFunction<IDXGIFactory2>(&CreateDXGIFactory2, static_cast<UINT>(flags));

                    if(!factory)
                        return Utils::Unexpected(factory.GetError());

                    return COM::Cast<Factory>(factory.GetValue());
                }
            }
        }
    }

    namespace SwapChain
    {
        template<class SwapChain = IDXGISwapChain1>
        Utils::Expected<Microsoft::WRL::ComPtr<SwapChain>, HRESULT> Create(IDXGIFactory2& factory, IUnknown& device, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1& desc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* optFullscreenDesc = nullptr, IDXGIOutput* optRestrictToOutput = nullptr)
        {
            using Microsoft::WRL::ComPtr;

            ComPtr<IDXGISwapChain1> swapChain;
            HRESULT hr = factory.CreateSwapChainForHwnd(&device, hWnd, &desc, optFullscreenDesc, optRestrictToOutput, &swapChain);

            if(FAILED(hr))
                return Utils::Unexpected(hr);

            if constexpr(std::is_same_v<SwapChain, IDXGISwapChain1>)
                return swapChain;
            else
                return COM::Cast<SwapChain>(swapChain);
        }

        template<class SwapChain = IDXGISwapChain1>
        Utils::Expected<Microsoft::WRL::ComPtr<SwapChain>, HRESULT> CreateFlipDiscard(IDXGIFactory2& factory, IUnknown& device, HWND hWnd, DXGI_FORMAT format, UINT bufferCount, DXGI_SWAP_CHAIN_FLAG flags, bool fullScreen, IDXGIOutput* optRestrictToOutput = nullptr)
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
        inline Utils::Expected<Microsoft::WRL::ComPtr<Resource>, HRESULT> GetBuffer(IDXGISwapChain& swapChain, UINT index)
        {
            return COM::IIDToObjectForwardFunction<Resource>(&IDXGISwapChain::GetBuffer, swapChain, index);
        }
    };
}
