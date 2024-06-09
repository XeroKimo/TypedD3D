#include <d3dcompiler.h>
#include <d3d12sdklayers.h>
#include <assert.h>
#include <Windows.h>
#include <exception>
#include <wrl/client.h>
#include "d3dx12.h"
#include "source/Helpers/D3D12Helpers.h"
#include "source/D3D12Wrappers.h"

import TypedD3D12;
import TypedDXGI;

#undef CreateWindow
namespace
{
    HWND handle;
    constexpr const wchar_t* windowName = L"Library Test";
    constexpr float windowWidth = 800;
    constexpr float windowHeight = 600;

    struct Vertex
    {
        float x;
        float y;
        float z;
    };
}


static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void CreateWindow();

using Microsoft::WRL::ComPtr;

void D3D12HelloWorld()
{
    CreateWindow();
    TypedD3D::D3D12::CommandList_t<ID3D12GraphicsCommandList, D3D12_COMMAND_LIST_TYPE_BUNDLE> b;
    //TypedD3D::tl::expected<ComPtr<IDXGIFactory2>, HRESULT> factory = TypedD3D::IIDToObjectForwardFunction<IDXGIFactory2>(&CreateDXGIFactory1);
    //TypedD3D::Wrapper<IDXGIAdapter> adapter = factory->EnumAdapters<IDXGIAdapter>(0);
    ComPtr<ID3D12Debug> debugLayer = TypedD3D::Helpers::D3D12::GetDebugInterface().value();
    debugLayer->EnableDebugLayer();
    TypedD3D::Wrapper<IDXGIFactory2> factory = TypedD3D::DXGI::Factory::Create1<IDXGIFactory2>().value();
    TypedD3D::Wrapper<ID3D12Device1> device = TypedD3D::D3D12::CreateDevice<ID3D12Device1>(D3D_FEATURE_LEVEL_12_0).value();
    ComPtr<ID3D12DebugDevice> debugDevice = TypedD3D::Cast<ID3D12DebugDevice>(device.Get());

    constexpr UINT backBufferCount = 2;

    TypedD3D::Direct<ID3D12CommandQueue> commandQueue = device->CreateCommandQueue<D3D12_COMMAND_LIST_TYPE_DIRECT>(D3D12_COMMAND_QUEUE_PRIORITY_NORMAL, D3D12_COMMAND_QUEUE_FLAG_NONE, 0).value();
    std::array<TypedD3D::Direct<ID3D12CommandAllocator>, backBufferCount> commandAllocators;
    commandAllocators[0] = device->CreateCommandAllocator<D3D12_COMMAND_LIST_TYPE_DIRECT>().value();
    commandAllocators[1] = device->CreateCommandAllocator<D3D12_COMMAND_LIST_TYPE_DIRECT>().value();
    TypedD3D::Direct<ID3D12GraphicsCommandList> temp = device->CreateCommandList<D3D12_COMMAND_LIST_TYPE_DIRECT>(commandAllocators[0]).value();
    TypedD3D::Direct<ID3D12GraphicsCommandList1> commandList = TypedD3D::Cast<TypedD3D::Direct<ID3D12GraphicsCommandList1>>(temp);
    TypedD3D::Compute<ID3D12GraphicsCommandList1> commandList54;

    //TypedD3D::Compute<ID3D12GraphicsCommandList1> cltest = commandList;
    UINT64 fenceValue = 0;
    ComPtr<ID3D12Fence> fence = device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE).value();
    HANDLE syncEvent = CreateEventW(nullptr, false, false, nullptr);

    TypedD3D::Wrapper<IDXGISwapChain3> swapChain = factory->CreateSwapChainForHwnd<IDXGISwapChain3>(
        commandQueue,
        handle,
        DXGI_SWAP_CHAIN_DESC1
        {
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .SampleDesc
            {
                .Count = 1
            },
            .BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = backBufferCount,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
        },
        nullptr,
        nullptr).value();

    TypedD3D::RTV<ID3D12DescriptorHeap> swapChainBufferDescriptorHeap = device->CreateDescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>(2, 0).value();

    UINT rtvOffset = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    TypedD3D::RTV<D3D12_CPU_DESCRIPTOR_HANDLE> descriptorHandle = swapChainBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    DXGI_SWAP_CHAIN_DESC1 desc = swapChain->GetDesc1();

    std::array<TypedD3D::Wrapper<ID3D12Resource>, 2> swapChainBuffers;

    for(UINT i = 0; i < desc.BufferCount; i++)
    {
        swapChainBuffers[i] = swapChain->GetBuffer<ID3D12Resource>(i).value();

        device->CreateRenderTargetView(*swapChainBuffers[i].Get(), nullptr, descriptorHandle);
        descriptorHandle = descriptorHandle.Offset(1, rtvOffset);
    }



    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc
    {
        .NumParameters = 0,
        .pParameters = nullptr,
        .NumStaticSamplers = 0,
        .pStaticSamplers = nullptr,
        .Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
    };

    ComPtr<ID3DBlob> signatureBlob;
    D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, nullptr);
    ComPtr<ID3D12RootSignature> rootSignature = device->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize()).value();

    ComPtr<ID3DBlob> vertexBlob;
    ComPtr<ID3DBlob> errorBlob;
    HRESULT hr = D3DCompileFromFile(L"VertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexBlob, &errorBlob);
    if(FAILED(hr))
    {
        char* message = static_cast<char*>(errorBlob->GetBufferPointer());
        std::unique_ptr<wchar_t[]> messageT = std::make_unique<wchar_t[]>(errorBlob->GetBufferSize());
        MultiByteToWideChar(CP_ACP, MB_COMPOSITE, message, static_cast<int>(errorBlob->GetBufferSize()), messageT.get(), static_cast<int>(errorBlob->GetBufferSize()));
        MessageBox(handle, messageT.get(), L"Error", MB_OK);
        return;
    }
    D3D12_SHADER_BYTECODE vertexByteCode{};
    vertexByteCode.BytecodeLength = vertexBlob->GetBufferSize();
    vertexByteCode.pShaderBytecode = vertexBlob->GetBufferPointer();

    ComPtr<ID3DBlob> pixelBlob;
    hr = D3DCompileFromFile(L"PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBlob, nullptr);
    if(FAILED(hr))
        return;

    D3D12_SHADER_BYTECODE pixelByteCode{};
    pixelByteCode.BytecodeLength = pixelBlob->GetBufferSize();
    pixelByteCode.pShaderBytecode = pixelBlob->GetBufferPointer();

    std::array<D3D12_INPUT_ELEMENT_DESC, 1> inputLayout
    {
        D3D12_INPUT_ELEMENT_DESC
        {
            .SemanticName = "POSITION",
            .SemanticIndex = 0,
            .Format = DXGI_FORMAT_R32G32B32_FLOAT,
            .InputSlot = 0,
            .AlignedByteOffset = 0,
            .InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
            .InstanceDataStepRate = 0
        }
    };

    D3D12_INPUT_LAYOUT_DESC layoutDesc
    {
        .pInputElementDescs = inputLayout.data(),
        .NumElements = static_cast<UINT>(inputLayout.size())
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineState
    {
        .pRootSignature = rootSignature.Get(),
        .VS = vertexByteCode,
        .PS = pixelByteCode,
        .BlendState = D3D12_BLEND_DESC
        {
            .AlphaToCoverageEnable = false,
            .IndependentBlendEnable = false
        },
        .SampleMask = 0xff'ff'ff'ff,
        .RasterizerState = D3D12_RASTERIZER_DESC
        {
            .FillMode = D3D12_FILL_MODE_SOLID,
            .CullMode = D3D12_CULL_MODE_NONE,
            .FrontCounterClockwise = true,
            .DepthBias = 0,
            .DepthBiasClamp = 0,
            .SlopeScaledDepthBias = 0,
            .DepthClipEnable = true,
            .MultisampleEnable = false,
            .AntialiasedLineEnable = false,
            .ForcedSampleCount = 0,
            .ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF,
        },
        .InputLayout = layoutDesc,
        .PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
        .NumRenderTargets = 1,
        .SampleDesc = TypedD3D::Helpers::Common::GetDescription(*swapChain.Get()).SampleDesc,
    };

    graphicsPipelineState.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
    graphicsPipelineState.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
    graphicsPipelineState.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
    graphicsPipelineState.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
    graphicsPipelineState.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
    graphicsPipelineState.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
    graphicsPipelineState.BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
    graphicsPipelineState.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

    graphicsPipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

    tl::expected<TypedD3D::D3D12::PipelineState::Graphics, HRESULT> pipelineState = device->CreateGraphicsPipelineState(graphicsPipelineState);
    if(!pipelineState)
    {
        pipelineState.error();
    }

    auto vertices = std::to_array<Vertex>(
        {
            { -0.5f, -0.5f, 0 },
            {  0.0f,  0.5f, 0 },
            {  0.5f, -0.5f, 0 },
        });

    D3D12_HEAP_PROPERTIES vertexHeap
    {
        .Type = D3D12_HEAP_TYPE_DEFAULT,
        .CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
        .MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
        .CreationNodeMask = 0,
        .VisibleNodeMask = 0
    };

    D3D12_RESOURCE_DESC vertexDesc
    {
        .Dimension = D3D12_RESOURCE_DIMENSION_BUFFER,
        .Alignment = 0,
        .Width = static_cast<UINT>(vertices.size() * sizeof(Vertex)),
        .Height = 1,
        .DepthOrArraySize = 1,
        .MipLevels = 1,
        .Format = DXGI_FORMAT_UNKNOWN,
        .SampleDesc = {.Count = 1, . Quality = 0},
        .Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        .Flags = D3D12_RESOURCE_FLAG_NONE
    };


    ComPtr<ID3D12Resource> vertexResource = device->CreateCommittedResource(
        vertexHeap,
        D3D12_HEAP_FLAG_NONE,
        vertexDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr).value();


    D3D12_HEAP_PROPERTIES uploadProperties
    {
        .Type = D3D12_HEAP_TYPE_UPLOAD,
        .CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
        .MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
        .CreationNodeMask = 0,
        .VisibleNodeMask = 0
    };
    ComPtr<ID3D12Resource> vertexUpload = device->CreateCommittedResource(
        uploadProperties,
        D3D12_HEAP_FLAG_NONE,
        vertexDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr).value();

    D3D12_SUBRESOURCE_DATA vertexData
    {
        .pData = vertices.data(),
        .RowPitch = static_cast<UINT>(vertices.size() * sizeof(Vertex)),
        .SlicePitch = static_cast<UINT>(vertices.size() * sizeof(Vertex)),
    };

    UpdateSubresources(commandList.Get(), vertexResource.Get(), vertexUpload.Get(), 0, 0, 1, &vertexData);

    D3D12_RESOURCE_BARRIER barrier = TypedD3D::Helpers::D3D12::TransitionBarrier(
        *vertexResource.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
    commandList->ResourceBarrier(std::span(&barrier, 1));
    commandList->Close();
    TypedD3D::Direct<ID3D12CommandList> a = commandList;
    std::array submitList = std::to_array<TypedD3D::Direct<ID3D12CommandList>>({ commandList });
    commandQueue->ExecuteCommandLists(std::span(submitList));

    TypedD3D::Helpers::D3D12::FlushCommandQueue(*commandQueue.Get(), *fence.Get(), fenceValue, syncEvent);

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView
    {
        .BufferLocation = vertexResource->GetGPUVirtualAddress(),
        .SizeInBytes = static_cast<UINT>(sizeof(Vertex) * vertices.size()),
        .StrideInBytes = static_cast<UINT>(sizeof(Vertex))
    };

    D3D12_VIEWPORT viewport
    {
        .TopLeftX = 0,
        .TopLeftY = 0,
        .Width = windowWidth,
        .Height = windowHeight,
        .MinDepth = 0,
        .MaxDepth = 1
    };

    D3D12_RECT rect
    {
        .left = 0,
        .top = 0,
        .right = static_cast<LONG>(windowWidth),
        .bottom = static_cast<LONG>(windowHeight)
    };

    UINT backBuffer = 0;
    MSG msg;

    std::array<UINT64, backBufferCount> frameWaitValues = {};

    std::vector<ID3D12Resource*> resources(backBufferCount);
    for(size_t i = 0; i < backBufferCount; i++)
    {
        resources[i] = swapChain->GetBuffer<ID3D12Resource>(i).value().Get();
    }


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
            TypedD3D::Helpers::D3D12::Frame(
                { *swapChain.Get() },
                { *fence.Get() },
                *commandQueue.Get(),
                frameWaitValues[backBuffer],
                fenceValue,
                backBuffer,
                frameWaitValues.size(),
                [&](const TypedD3D::Helpers::D3D12::FrameData& frameData)
                {
                    TypedD3D::Helpers::D3D12::RecordAndExecute(
                        *commandList.Get(), 
                        *commandAllocators[frameData.backBufferIndex].Get(), 
                        frameData.commandQueue,
                        [&](TypedD3D::Direct<ID3D12GraphicsCommandList1> commandList)
                        {
                            D3D12_RESOURCE_BARRIER beginBarrier = TypedD3D::Helpers::D3D12::TransitionBarrier(
                                *swapChainBuffers[frameData.backBufferIndex].Get(),
                                D3D12_RESOURCE_STATE_PRESENT,
                                D3D12_RESOURCE_STATE_RENDER_TARGET);

                            D3D12_RESOURCE_BARRIER endBarrier = TypedD3D::Helpers::D3D12::TransitionBarrier(
                                *swapChainBuffers[frameData.backBufferIndex].Get(),
                                D3D12_RESOURCE_STATE_RENDER_TARGET,
                                D3D12_RESOURCE_STATE_PRESENT);

                            TypedD3D::Helpers::D3D12::ResourceBarrier(*commandList.Get(), std::span(&beginBarrier, 1), std::span(&endBarrier, 1),
                                [&](TypedD3D::Direct<ID3D12GraphicsCommandList1> commandList)
                                {
                                    TypedD3D::RTV<D3D12_CPU_DESCRIPTOR_HANDLE> backBufferHandle = swapChainBufferDescriptorHeap->GetCPUDescriptorHandleForHeapStart().Offset(frameData.backBufferIndex, rtvOffset);
                                    commandList->ClearRenderTargetView(backBufferHandle, std::to_array({ 0.f, 0.3f, 0.7f, 1.f }), {});
                                    commandList->OMSetRenderTargets(std::span(&backBufferHandle, 1), true, nullptr);

                                    commandList->SetPipelineState(pipelineState.value().Get());
                                    commandList->SetGraphicsRootSignature(rootSignature.Get());
                                    commandList->RSSetViewports(std::span(&viewport, 1));
                                    commandList->RSSetScissorRects(std::span(&rect, 1));
                                    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
                                    commandList->IASetVertexBuffers(0, std::span(&vertexBufferView, 1));
                                    commandList->DrawInstanced(3, 1, 0, 0);
                                });
                        });
                });
        }
    }
    TypedD3D::Helpers::D3D12::FlushCommandQueue(*commandQueue.Get(), *fence.Get(), fenceValue, syncEvent);

    debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_FLAGS::D3D12_RLDO_SUMMARY | D3D12_RLDO_IGNORE_INTERNAL);

    assert(swapChain != nullptr);
    assert(factory != nullptr);


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