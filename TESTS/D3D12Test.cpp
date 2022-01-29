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


    if(!RegisterClass(&windowClass))
    {
        MessageBox(NULL, L"Could not register class", L"Error", MB_OK);
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
}

using Microsoft::WRL::ComPtr;

void TestD3DHelpers()
{
    CreateWindow();

    auto factory = TypedD3D::Helpers::COM::IIDToObjectForwardFunction<IDXGIFactory7>(&CreateDXGIFactory1);

    ComPtr<ID3D12Debug> debugLayer = TypedD3D::Helpers::COM::IIDToObjectForwardFunction<ID3D12Debug>(&D3D12GetDebugInterface).GetValue();
    debugLayer->EnableDebugLayer();

    auto device = TypedD3D::Helpers::D3D12::CreateDevice(D3D_FEATURE_LEVEL_12_0);

    ComPtr<ID3D12DebugDevice2> debugDevice = TypedD3D::Helpers::COM::Cast<ID3D12DebugDevice2>(device.GetValue());
    assert(device.HasValue());

    D3D12_COMMAND_QUEUE_DESC desc;
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;


    ComPtr<ID3D12CommandQueue> commandQueue = TypedD3D::Helpers::D3D12::CreateCommandQueue(*device.GetValue().Get(), desc).GetValue();
    auto allocator = TypedD3D::Helpers::D3D12::CreateCommandAllocator(*device.GetValue().Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);

    assert(allocator.HasValue());

    debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_FLAGS::D3D12_RLDO_SUMMARY | D3D12_RLDO_IGNORE_INTERNAL);
    device.GetValue()->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue));
    assert(commandQueue);

    auto fence = TypedD3D::Helpers::D3D12::CreateFence(*device.GetValue().Get(), D3D12_FENCE_FLAG_NONE);

    HANDLE syncEvent = CreateEventW(nullptr, false, false, nullptr);
    commandQueue->Signal(fence.GetValue().Get(), 2);
    TypedD3D::Helpers::D3D12::StallCPUThread(*fence.GetValue().Get(), 2);

    TypedD3D::D3D12::CommandList::Bundle b{};
    TypedD3D::D3D12::Device dev;

    dev.CheckFeatureSupport<D3D12_FEATURE_D3D12_OPTIONS>();

    TypedD3D::D3D12::CommandList::Direct d{};
    TypedD3D::D3D12::CommandQueue::Direct dq = dev.CreateCommandQueue<D3D12_COMMAND_LIST_TYPE_DIRECT>(D3D12_COMMAND_QUEUE_PRIORITY_NORMAL, D3D12_COMMAND_QUEUE_FLAG_NONE, 0).GetValue();
    TypedD3D::D3D12::CommandAllocator::Bundle da{};
    TypedD3D::D3D12::CommandAllocator::Direct dir{};

    TypedD3D::D3D12::CommandList::Bundle b2 = dev.CreateCommandList<D3D12_COMMAND_LIST_TYPE_BUNDLE>(da, 0, nullptr).GetValue();

    dev.CreateGraphicsPipelineState({});
    std::array<TypedD3D::D3D12::CommandList::Direct*, 1> submit;
    submit[0] = &d;
    std::span sub = submit;
    std::span<TypedD3D::D3D12::CommandList::Internal::CommandList<D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>*, 1> sub2 = submit;
    static_assert(std::same_as<TypedD3D::D3D12::CommandQueue::Direct::list_type, TypedD3D::D3D12::CommandList::Direct>);
    dq.ExecuteCommandLists(std::span(submit));
    d.Reset(dir, nullptr);
    b.Reset(da, nullptr);

    d.ExecuteBundle(b);


    auto swapChain = TypedD3D::Helpers::DXGI::SwapChain::CreateFlipDiscard(
        *factory.GetValue().Get(),
        *commandQueue.Get(),
        handle,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        2,
        DXGI_SWAP_CHAIN_FLAG{},
        false);
        

    assert(swapChain.HasValue());
    assert(factory.HasValue());


    UnregisterClassW(windowName, nullptr);
}
