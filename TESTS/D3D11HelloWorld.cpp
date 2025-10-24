#include <Windows.h>
#include <tuple>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <span>
#include <dxgi1_6.h>
#include <d3d11_4.h>
#include <memory>
#include <cassert>
#include <array>
#include <iostream>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif
import TypedD3D11;
import TypedDXGI;
import TypedD3D.Containers;
import xk.Exceptions;
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

void D3D11HelloWorld()
{
	CreateWindow();

	TypedD3D::Wrapper<IDXGIFactory2> factory = TypedDXGI::CreateFactory1<IDXGIFactory2>();
	D3D_FEATURE_LEVEL levels = D3D_FEATURE_LEVEL_12_0;
	
	auto [device, deviceContext] = TypedD3D11::CreateDevice<TypedD3D::Wrapper<ID3D11Device>, TypedD3D::Wrapper<ID3D11DeviceContext>>(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, std::span(&levels, 1), D3D11_SDK_VERSION);
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
		nullptr);
	TypedD3D::WeakWrapper<TypedD3D::UntaggedTraits<ID3D11Device>> testDevice{ device.Get() };

	TypedD3D::WeakWrapper<TypedD3D::UntaggedTraits<ID3D11Debug>> debugDevice = TypedD3D::Cast<ID3D11Debug>(testDevice);
	TypedD3D::TestArray<TypedD3D::StrongWrapper<TypedD3D::UntaggedTraits<ID3D11Device>>, 2> arr{ testDevice, testDevice };
	const auto& arr2 = arr;
	//testDevice->CheckCounterInfo();
	//arr[0]->CheckCounterInfo();
	arr[0] = testDevice;
	arr[0] = nullptr;
	arr[0] = std::move(testDevice);
	testDevice = std::move(arr)[0];
	testDevice = arr[1];
	arr[1] = nullptr;
	arr[0] == arr[1];

	arr2[0] = arr[1];
	//arr2[0] == arr[0];
	//TypedD3D::StrongWrapper<TypedD3D::UntaggedTraits<ID3D11Device>> fafda = std::move(arr[1]);

	//TypedD3D::StrongWrapper<TypedD3D::UntaggedTraits<ID3D11Debug>> debugDevice = TypedD3D::Cast<ID3D11Debug>(testDevice);
	//ComPtr<ID3D11Debug> debugDevice = TypedD3D::Cast<ID3D11Debug>(device.AsComPtr());
	{
		//TypedD3D::TestArray<TypedD3D::StrongWrapper<TypedD3D::UntaggedTraits<ID3D11Device>>, 2> arr2{ std::move(arr) };
		//TypedD3D::WeakWrapper<TypedD3D::UntaggedTraits<ID3D11Device5>> testDevice5 = TypedD3D::Cast<ID3D11Device5>(testDevice);
		static_assert(std::same_as<TypedD3D::UntaggedTraits<ID3D11Device>::Interface<int>, TypedD3D::StrongWrapper<TypedD3D::UntaggedTraits<ID3D11Device>>::interface_type<int>>);
		static_assert(!std::same_as<TypedD3D::UntaggedTraits<ID3D11Device>::Interface<int>, ID3D11Device*>);
		static_assert(std::same_as<TypedD3D::UntaggedTraits<ID3D11Device5>::Interface<int>, ID3D11Device5*>);
		TypedD3D::StrongWrapper<TypedD3D::UntaggedTraits<ID3D11Device>> testDevice2{ device.Get() };
		//TypedD3D::StrongWrapper<TypedD3D::UntaggedTraits<ID3D11Device>> testDevice4{ testDevice2 };
		TypedD3D::StrongWrapper<TypedD3D::UntaggedTraits<ID3D11Device>> testDevice3 = TypedD3D::Cast<ID3D11Device5>(testDevice2);
		//testDevice3->QueryInterface<ID3D11Device>(nullptr);
		//TypedD3D::Wrapper<IDXGISwapChain1> swapChain1 = swapChain;

		UINT backBuffer = 0;
		MSG msg;


		TypedD3D::Wrapper<ID3D11RenderTargetView> rtv;
		rtv = device->CreateRenderTargetView(swapChain->GetBuffer<ID3D11Resource>(0), nullptr);


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

		TypedD3D::Wrapper<ID3D11VertexShader> vs = device->CreateVertexShader(*vertexBlob.Get(), nullptr);

		ComPtr<ID3DBlob> pixelBlob;
		hr = D3DCompileFromFile(L"PixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBlob, nullptr);
		if(FAILED(hr))
			return;

		TypedD3D::Wrapper<ID3D11PixelShader> ps = device->CreatePixelShader(*pixelBlob.Get(), nullptr);

		std::array<D3D11_INPUT_ELEMENT_DESC, 1> inputElements
		{
			D3D11_INPUT_ELEMENT_DESC
			{
				.SemanticName = "POSITION",
				.SemanticIndex = 0,
				.Format = DXGI_FORMAT_R32G32B32_FLOAT,
				.InputSlot = 0,
				.AlignedByteOffset = 0,
				.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0
			}
		};

		TypedD3D::Wrapper<ID3D11InputLayout> inputLayout = device->CreateInputLayout(inputElements, *vertexBlob.Get());

		auto vertices = std::to_array<Vertex>(
			{
				{ -0.5f, -0.5f, 0 },
				{  0.0f,  0.5f, 0 },
				{  0.5f, -0.5f, 0 },
			});

		D3D11_BUFFER_DESC desc
		{
			.ByteWidth = vertices.size() * sizeof(Vertex),
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = sizeof(Vertex)
		};
		D3D11_SUBRESOURCE_DATA data
		{
			.pSysMem = vertices.data(),
			.SysMemPitch = sizeof(Vertex),
			.SysMemSlicePitch = sizeof(Vertex) * vertices.size()
		};
		TypedD3D::Wrapper<ID3D11Buffer> vertexBuffer = device->CreateBuffer(desc, &data);
		D3D11_VIEWPORT viewport
		{
			.TopLeftX = 0,
			.TopLeftY = 0,
			.Width = windowWidth,
			.Height = windowHeight,
			.MinDepth = 0,
			.MaxDepth = 1
		};

		D3D11_RECT rect
		{
			.left = 0,
			.top = 0,
			.right = static_cast<LONG>(windowWidth),
			.bottom = static_cast<LONG>(windowHeight)
		};
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
				deviceContext->ClearRenderTargetView(rtv, std::array{ 0.f, 0.3f, 0.7f, 1.f });
				deviceContext->OMSetRenderTargets(rtv, nullptr);

				deviceContext->IASetInputLayout(inputLayout);
				deviceContext->RSSetViewports(std::span(&viewport, 1));
				deviceContext->RSSetScissorRects(std::span(&rect, 1));
				deviceContext->VSSetShader(vs, {});
				deviceContext->PSSetShader(ps, {});
				deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				deviceContext->IASetVertexBuffers(0, vertexBuffer.Get(), sizeof(Vertex), 0);
				deviceContext->Draw(3, 0);

				swapChain->Present(1, 0);
			}
		}

		assert(swapChain != nullptr);
		assert(factory != nullptr);


		UnregisterClassW(windowName, nullptr);
	}
	static_assert(std::same_as<TypedD3D::UntaggedTraits<ID3D11Debug>::Interface<int>, ID3D11Debug*>);
	debugDevice->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_SUMMARY | D3D11_RLDO_FLAGS::D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
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