#include "TypedD3D.h"
#include <d3d12sdklayers.h>
#include <assert.h>
#include <Windows.h>
#include <exception>
#include <wrl/client.h>

namespace
{
    HWND handle;
    constexpr const wchar_t* windowName = L"Library Test";
}

#undef CreateWindow

static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;
    }

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

void CreateWindow()
{
    WNDCLASSW windowClass{};
    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hInstance = NULL;
    windowClass.lpfnWndProc = WndProc;
    windowClass.lpszClassName = windowName;
    windowClass.style = CS_HREDRAW | CS_VREDRAW;


    if(!RegisterClassW(&windowClass))
    {
        MessageBoxW(NULL, L"Could not register class", L"Error", MB_OK);
        throw std::exception("Could not register class");
    }

    handle = CreateWindowExW(0,
        windowName,
        windowName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        NULL,
        NULL,
        NULL,
        NULL);

    if(!handle)
    {
        MessageBox(NULL, L"Could not create window", L"Error", MB_OK);
        throw std::exception("Could not create window");
    }

    ShowWindow(handle, SW_RESTORE);
}

using Microsoft::WRL::ComPtr;

void TestD3DHelpers()
{
    CreateWindow();

    TypedD3D::Utils::Expected<ComPtr<IDXGIFactory7>, HRESULT> factory = TypedD3D::Helpers::COM::IIDToObjectForwardFunction<IDXGIFactory7>(&CreateDXGIFactory1);

    ComPtr<ID3D12Debug> debugLayer = TypedD3D::Helpers::COM::IIDToObjectForwardFunction<ID3D12Debug>(&D3D12GetDebugInterface).GetValue();
    debugLayer->EnableDebugLayer();

    TypedD3D::D3D12::Device device = TypedD3D::D3D12::CreateDevice(D3D_FEATURE_LEVEL_12_0).GetValue();
    ComPtr<ID3D12DebugDevice2> debugDevice = TypedD3D::Helpers::COM::Cast<ID3D12DebugDevice2>(device.GetComPtr());

    TypedD3D::D3D12::CommandQueue::Direct commandQueue = device.CreateCommandQueue<D3D12_COMMAND_LIST_TYPE_DIRECT>(D3D12_COMMAND_QUEUE_PRIORITY_NORMAL, D3D12_COMMAND_QUEUE_FLAG_NONE, 0).GetValue();
    std::array<TypedD3D::D3D12::CommandAllocator::Direct, 2> commandAllocators;
    commandAllocators[0] = device.CreateCommandAllocator<D3D12_COMMAND_LIST_TYPE_DIRECT>().GetValue();
    commandAllocators[1] = device.CreateCommandAllocator<D3D12_COMMAND_LIST_TYPE_DIRECT>().GetValue();
    TypedD3D::D3D12::CommandList::Direct commandList = device.CreateCommandList<D3D12_COMMAND_LIST_TYPE_DIRECT>(commandAllocators[0], 0, nullptr).GetValue();
    commandList.Close();

    UINT64 fenceValue = 0;
    ComPtr<ID3D12Fence> fence = device.CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE).GetValue();
    HANDLE syncEvent = CreateEventW(nullptr, false, false, nullptr);

    constexpr UINT backBufferCount = 2;

    TypedD3D::Utils::Expected<ComPtr<IDXGISwapChain1>, HRESULT> swapChain = TypedD3D::Helpers::DXGI::SwapChain::CreateFlipDiscard(
        *factory.GetValue().Get(),
        *commandQueue.Get(),
        handle,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        backBufferCount,
        DXGI_SWAP_CHAIN_FLAG{},
        false);
    
    UINT backBuffer = 0;
    MSG msg;
    while(true)
    {
        if(PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);

            if(msg.message == WM_QUIT)
                break;
        }
        else
        {
            commandAllocators[backBuffer].Reset();
            commandList.Reset(commandAllocators[backBuffer], nullptr);
            commandList.Close();

            std::array submitList = std::to_array({ &commandList });
            commandQueue.ExecuteCommandLists(std::span(submitList));
            TypedD3D::Helpers::D3D12::FlushCommandQueue(*commandQueue.Get(), *fence.Get(), fenceValue, syncEvent);

            swapChain.GetValue()->Present(1, 0);
            backBuffer = (backBuffer + 1) % backBufferCount;
        }
    }
    TypedD3D::Helpers::D3D12::FlushCommandQueue(*commandQueue.Get(), *fence.Get(), fenceValue, syncEvent);

    debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_FLAGS::D3D12_RLDO_SUMMARY | D3D12_RLDO_IGNORE_INTERNAL);



    assert(swapChain.HasValue());
    assert(factory.HasValue());


    UnregisterClassW(windowName, nullptr);
}
