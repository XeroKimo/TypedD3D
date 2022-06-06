#include "TypedD3D11.h"
#include "TypedDXGI.h"
#include <Windows.h>
#include <tuple>

#undef CreateWindow
namespace
{
    HWND handle;
    constexpr const wchar_t* windowName = L"Library Test";
    constexpr float windowWidth = 800;
    constexpr float windowHeight = 600;
}

struct Vertex
{
    float x;
    float y;
    float z;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void CreateWindow();

using Microsoft::WRL::ComPtr;

void D3D11HelloWorld()
{
    CreateWindow();

    TypedD3D::Wrapper<IDXGIFactory2> factory = TypedD3D::DXGI::Factory::Create1<IDXGIFactory2>().value();
    D3D_FEATURE_LEVEL levels = D3D_FEATURE_LEVEL_12_0;

    auto [device, deviceContext] = TypedD3D::D3D11::CreateDevice<TypedD3D::Wrapper<ID3D11Device>, TypedD3D::Wrapper<ID3D11DeviceContext>>(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, std::span(&levels, 1), D3D11_SDK_VERSION).value();
    TypedD3D::Wrapper<IDXGISwapChain3> swapChain = factory->CreateSwapChainForHwnd<IDXGISwapChain3>(
        device,
        handle,
        DXGI_SWAP_CHAIN_DESC1
        {
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .SampleDesc
            {
                .Count = 1
            },
            .BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = 2,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
        },
        nullptr,
        nullptr).value();

    UINT backBuffer = 0;
    MSG msg;

    //std::array<UINT64, backBufferCount> frameWaitValues = {};

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
            //commandAllocators[backBuffer]->Reset();
            //commandList->Reset(commandAllocators[backBuffer], nullptr);

            //D3D12_RESOURCE_BARRIER barrier = TypedD3D::Helpers::D3D12::ResourceBarrier::Transition(
            //    *swapChainBuffers[backBuffer].Get(),
            //    D3D12_RESOURCE_STATE_PRESENT,
            //    D3D12_RESOURCE_STATE_RENDER_TARGET);

            //commandList->ResourceBarrier(std::span(&barrier, 1));

            //auto backBufferHandle = swapChainBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
            //backBufferHandle.ptr += static_cast<SIZE_T>(rtvOffset) * backBuffer;
            //commandList->ClearRenderTargetView(backBufferHandle, std::to_array({ 0.f, 0.3f, 0.7f, 1.f }), {});
            //commandList->OMSetRenderTargets(std::span(&backBufferHandle, 1), true, nullptr);

            //commandList->SetPipelineState(pipelineState.value().Get());
            //commandList->SetGraphicsRootSignature(rootSignature.Get());
            //commandList->RSSetViewports(std::span(&viewport, 1));
            //commandList->RSSetScissorRects(std::span(&rect, 1));
            //commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            //commandList->IASetVertexBuffers(0, std::span(&vertexBufferView, 1));
            //commandList->DrawInstanced(3, 1, 0, 0);


            //barrier = TypedD3D::Helpers::D3D12::ResourceBarrier::Transition(
            //    *swapChainBuffers[backBuffer].Get(),
            //    D3D12_RESOURCE_STATE_RENDER_TARGET,
            //    D3D12_RESOURCE_STATE_PRESENT);
            //commandList->ResourceBarrier(std::span(&barrier, 1));

            //commandList->Close();

            //std::array submitList = std::to_array<TypedD3D::Direct<ID3D12CommandList>>({ commandList });
            //commandQueue->ExecuteCommandLists(std::span(submitList));

            swapChain->Present(1, 0);
        }
    }

    //assert(swapChain.has_value());
    //assert(factory.has_value());


    UnregisterClassW(windowName, nullptr);
}




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
        static_cast<int>(windowWidth),
        static_cast<int>(windowHeight),
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