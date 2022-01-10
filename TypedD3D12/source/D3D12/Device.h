#pragma once
#include "../COMHelpers.h"
#include "../Utils.h"

#include "../DXGI/SwapChain.h"
#include <d3d12.h>
namespace TypedD3D::D3D12::Device
{
    template<class Device = ID3D12Device>
    Utils::Expected<Microsoft::WRL::ComPtr<Device>, HRESULT> CreateDevice(D3D_FEATURE_LEVEL minFeatureLevel, IDXGIAdapter* optAdapter = nullptr)
    {
        using Microsoft::WRL::ComPtr;

        Utils::Expected<ComPtr<ID3D12Device>, HRESULT> device = Helpers::COM::IIDToObjectForwardFunction<ID3D12Device>(&D3D12CreateDevice, optAdapter, minFeatureLevel);

        if(!device)
            return Utils::Unexpected(device.GetError());

        if constexpr(std::is_same_v<Device, ID3D12Device>)
            return device.GetValue();
        else
            return COM::ComPtrCast<Device>(device.GetValue());
    }


};