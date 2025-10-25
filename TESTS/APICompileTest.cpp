
#include <d3d11_4.h>
#include <dxgi1_6.h>

import TypedD3D11;
//import TypedD3D12;
//import TypedDXGI;

using namespace TypedD3D;

void D3D11()
{
#pragma region Device

	D3D11::CreateDevice<ID3D11Device, ID3D11DeviceContext>(nullptr, D3D_DRIVER_TYPE{}, HMODULE{}, UINT{}, std::span<D3D_FEATURE_LEVEL>{}, UINT{});
	D3D11::CreateDevice<ID3D11Device, ID3D11DeviceContext>(nullptr, D3D_DRIVER_TYPE{}, HMODULE{}, UINT{}, D3D_FEATURE_LEVEL{}, UINT{});

	Wrapper<ID3D11Device> device;
	Wrapper<ID3D11Buffer> buffer = device->CreateBuffer(D3D11_BUFFER_DESC{}, nullptr);
	Wrapper<ID3D11Texture1D> texture1 = device->CreateTexture1D(D3D11_TEXTURE1D_DESC{}, nullptr);
	Wrapper<ID3D11Texture2D> texture2 = device->CreateTexture2D(D3D11_TEXTURE2D_DESC{}, nullptr);
	Wrapper<ID3D11Texture3D> texture3 = device->CreateTexture3D(D3D11_TEXTURE3D_DESC{}, nullptr);
	Wrapper<ID3D11Resource> resource = buffer;
	resource = texture1;
	resource = texture2;
	resource = texture3;
	Wrapper<ID3D11ShaderResourceView> srv = device->CreateShaderResourceView(resource, nullptr);
	Wrapper<ID3D11UnorderedAccessView> uav = device->CreateUnorderedAccessView(resource, nullptr);
	Wrapper<ID3D11RenderTargetView> rtv = device->CreateRenderTargetView(resource, nullptr);
	Wrapper<ID3D11DepthStencilView> dsv = device->CreateDepthStencilView(resource, nullptr);
	{
		ID3DBlob* temp = nullptr;
		Wrapper<ID3D11InputLayout> layout = device->CreateInputLayout(D3D11_INPUT_ELEMENT_DESC{}, temp);
		layout = device->CreateInputLayout(std::span<D3D11_INPUT_ELEMENT_DESC>{}, temp);
		layout = device->CreateInputLayout(D3D11_INPUT_ELEMENT_DESC{}, temp->GetBufferPointer(), {});
		layout = device->CreateInputLayout(std::span<D3D11_INPUT_ELEMENT_DESC>{}, temp->GetBufferPointer(), {});

		Wrapper<ID3D11VertexShader> vs = device->CreateVertexShader(temp, nullptr);
		vs = device->CreateVertexShader(temp->GetBufferPointer(), {}, nullptr);

		Wrapper<ID3D11GeometryShader> gs = device->CreateGeometryShader(temp, nullptr);
		gs = device->CreateGeometryShader(temp->GetBufferPointer(), {}, nullptr);
		gs = device->CreateGeometryShaderWithStreamOutput(temp, std::span<D3D11_SO_DECLARATION_ENTRY>{}, std::span<UINT>{}, UINT{}, nullptr);
		gs = device->CreateGeometryShaderWithStreamOutput(temp->GetBufferPointer(), {}, std::span<D3D11_SO_DECLARATION_ENTRY>{}, std::span<UINT>{}, UINT{}, nullptr);

		Wrapper<ID3D11PixelShader> ps = device->CreatePixelShader(temp, nullptr);
		ps = device->CreatePixelShader(temp->GetBufferPointer(), {}, nullptr);

		Wrapper<ID3D11HullShader> hs = device->CreateHullShader(temp, nullptr);
		hs = device->CreateHullShader(temp->GetBufferPointer(), {}, nullptr);

		Wrapper<ID3D11DomainShader> ds = device->CreateDomainShader(temp, nullptr);
		ds = device->CreateDomainShader(temp->GetBufferPointer(), {}, nullptr);

		Wrapper<ID3D11ComputeShader> cs = device->CreateComputeShader(temp, nullptr);
		cs = device->CreateComputeShader(temp->GetBufferPointer(), {}, nullptr);
	}
	Wrapper<ID3D11ClassLinkage> classLink = device->CreateClassLinkage();
	Wrapper<ID3D11BlendState> blendState = device->CreateBlendState({});
	Wrapper<ID3D11DepthStencilState> depthStencilState = device->CreateDepthStencilState({});
	Wrapper<ID3D11RasterizerState> rasterizerState = device->CreateRasterizerState({});
	Wrapper<ID3D11SamplerState> samplerState = device->CreateSamplerState({});
	Wrapper<ID3D11Query> query = device->CreateQuery({});
	Wrapper<ID3D11Predicate> predicate = device->CreatePredicate({});
	Wrapper<ID3D11Counter> counter = device->CreateCounter({});
	Wrapper<ID3D11DeviceContext> deferredContext = device->CreateDeferredContext({});
	HRESULT hr = device->OpenSharedResource({}, {}, {});
	D3D11_FORMAT_SUPPORT formatSupport = device->CheckFormatSupport({});
	UINT multisampleQuality = device->CheckMultisampleQualityLevels({}, {});
	D3D11_COUNTER_INFO counterInfo = device->CheckCounterInfo();
	D3D11_COUNTER_TYPE counterType;
	UINT activeCounters;
	hr = device->CheckCounter({}, counterType, activeCounters, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
	{
		auto f = device->CheckFeatureSupport<D3D11_FEATURE_THREADING>();
		auto f1 = device->CheckFeatureSupport<D3D11_FEATURE_DOUBLES>();
		auto f2 = device->CheckFeatureSupport<D3D11_FEATURE_FORMAT_SUPPORT>();
		auto f3 = device->CheckFeatureSupport<D3D11_FEATURE_FORMAT_SUPPORT2>();
		auto f4 = device->CheckFeatureSupport<D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS>();
		auto f5 = device->CheckFeatureSupport<D3D11_FEATURE_D3D11_OPTIONS>();
		auto f6 = device->CheckFeatureSupport<D3D11_FEATURE_ARCHITECTURE_INFO>();
		auto f7 = device->CheckFeatureSupport<D3D11_FEATURE_D3D9_OPTIONS>();
		auto f8 = device->CheckFeatureSupport<D3D11_FEATURE_SHADER_MIN_PRECISION_SUPPORT>();
		auto f9 = device->CheckFeatureSupport<D3D11_FEATURE_D3D9_SHADOW_SUPPORT>();
		auto f10 = device->CheckFeatureSupport<D3D11_FEATURE_D3D11_OPTIONS1>();
		auto f11 = device->CheckFeatureSupport<D3D11_FEATURE_D3D9_SIMPLE_INSTANCING_SUPPORT>();
		auto f12 = device->CheckFeatureSupport<D3D11_FEATURE_MARKER_SUPPORT>();
		auto f13 = device->CheckFeatureSupport<D3D11_FEATURE_D3D9_OPTIONS1>();
		auto f14 = device->CheckFeatureSupport<D3D11_FEATURE_D3D11_OPTIONS2>();
		auto f15 = device->CheckFeatureSupport<D3D11_FEATURE_D3D11_OPTIONS3>();
		auto f16 = device->CheckFeatureSupport<D3D11_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT>();
		auto f17 = device->CheckFeatureSupport<D3D11_FEATURE_D3D11_OPTIONS4>();
		auto f18 = device->CheckFeatureSupport<D3D11_FEATURE_SHADER_CACHE>();
		auto f19 = device->CheckFeatureSupport<D3D11_FEATURE_D3D11_OPTIONS5>();
	}
	hr = device->GetPrivateData({}, {}, {});
	hr = device->SetPrivateData({}, {}, {});
	hr = device->SetPrivateDataInterface({}, {});
	D3D_FEATURE_LEVEL level = device->GetFeatureLevel();
	UINT creationFlags = device->GetCreationFlags();
	hr = device->GetDeviceRemovedReason();
	Wrapper<ID3D11DeviceContext> immediateContext = device->GetImmediateContext();
	device->SetExceptionMode({});
	UINT exceptionMode = device->GetExceptionMode();

#pragma endregion //end Device

#pragma region DeviceContext
	Wrapper<ID3D11DeviceContext> deviceContext;

	{
		deviceContext->VSSetShader({}, nullptr);
		deviceContext->VSSetShader({}, Span<WrapperView<ID3D11ClassInstance>>{});
		deviceContext->VSSetConstantBuffers({}, Span<WrapperView<ID3D11Buffer>>{});
		deviceContext->VSSetConstantBuffers({}, Wrapper<ID3D11Buffer>{});
		deviceContext->VSSetSamplers({}, Wrapper<ID3D11SamplerState>{});
		deviceContext->VSSetSamplers({}, Span<WrapperView<ID3D11SamplerState>>{});
		deviceContext->VSSetShaderResources({}, Wrapper<ID3D11ShaderResourceView>{});
		deviceContext->VSSetShaderResources({}, Span<WrapperView<ID3D11ShaderResourceView>>{});
		Vector<Wrapper<ID3D11Buffer>> buffers = deviceContext->VSGetConstantBuffers(0, 0);
		Vector<Wrapper<ID3D11SamplerState>> samplers = deviceContext->VSGetSamplers(0, 0);
		UINT numInstances = 256;
		std::pair<Wrapper<ID3D11VertexShader>, Vector<Wrapper<ID3D11ClassInstance>>> shader = deviceContext->VSGetShader(numInstances);
		std::pair<Wrapper<ID3D11VertexShader>, UINT> shader2 = deviceContext->VSGetShader({});
		Vector<Wrapper<ID3D11ShaderResourceView>> resources = deviceContext->VSGetShaderResources(0, 0);
	}
	{
		deviceContext->PSSetShader({}, nullptr);
		deviceContext->PSSetShader({}, Span<WrapperView<ID3D11ClassInstance>>{});
		deviceContext->PSSetConstantBuffers({}, Span<WrapperView<ID3D11Buffer>>{});
		deviceContext->PSSetConstantBuffers({}, Wrapper<ID3D11Buffer>{});
		deviceContext->PSSetSamplers({}, Wrapper<ID3D11SamplerState>{});
		deviceContext->PSSetSamplers({}, Span<WrapperView<ID3D11SamplerState>>{});
		deviceContext->PSSetShaderResources({}, Wrapper<ID3D11ShaderResourceView>{});
		deviceContext->PSSetShaderResources({}, Span<WrapperView<ID3D11ShaderResourceView>>{});
		Vector<Wrapper<ID3D11Buffer>> buffers = deviceContext->PSGetConstantBuffers(0, 0);
		Vector<Wrapper<ID3D11SamplerState>> samplers = deviceContext->PSGetSamplers(0, 0);
		UINT numInstances = 256;
		std::pair<Wrapper<ID3D11PixelShader>, Vector<Wrapper<ID3D11ClassInstance>>> shader = deviceContext->PSGetShader(numInstances);
		std::pair<Wrapper<ID3D11PixelShader>, UINT> shader2 = deviceContext->PSGetShader({});
		Vector<Wrapper<ID3D11ShaderResourceView>> resources = deviceContext->PSGetShaderResources(0, 0);
	}
	{
		deviceContext->GSSetShader({}, nullptr);
		deviceContext->GSSetShader({}, Span<WrapperView<ID3D11ClassInstance>>{});
		deviceContext->GSSetConstantBuffers({}, Span<WrapperView<ID3D11Buffer>>{});
		deviceContext->GSSetConstantBuffers({}, Wrapper<ID3D11Buffer>{});
		deviceContext->GSSetSamplers({}, Wrapper<ID3D11SamplerState>{});
		deviceContext->GSSetSamplers({}, Span<WrapperView<ID3D11SamplerState>>{});
		deviceContext->GSSetShaderResources({}, Wrapper<ID3D11ShaderResourceView>{});
		deviceContext->GSSetShaderResources({}, Span<WrapperView<ID3D11ShaderResourceView>>{});
		Vector<Wrapper<ID3D11Buffer>> buffers = deviceContext->GSGetConstantBuffers(0, 0);
		Vector<Wrapper<ID3D11SamplerState>> samplers = deviceContext->GSGetSamplers(0, 0);
		UINT numInstances = 256;
		std::pair<Wrapper<ID3D11GeometryShader>, Vector<Wrapper<ID3D11ClassInstance>>> shader = deviceContext->GSGetShader(numInstances);
		std::pair<Wrapper<ID3D11GeometryShader>, UINT> shader2 = deviceContext->GSGetShader({});
		Vector<Wrapper<ID3D11ShaderResourceView>> resources = deviceContext->GSGetShaderResources(0, 0);
	}
	{
		deviceContext->HSSetShader({}, nullptr);
		deviceContext->HSSetShader({}, Span<WrapperView<ID3D11ClassInstance>>{});
		deviceContext->HSSetConstantBuffers({}, Span<WrapperView<ID3D11Buffer>>{});
		deviceContext->HSSetConstantBuffers({}, Wrapper<ID3D11Buffer>{});
		deviceContext->HSSetSamplers({}, Wrapper<ID3D11SamplerState>{});
		deviceContext->HSSetSamplers({}, Span<WrapperView<ID3D11SamplerState>>{});
		deviceContext->HSSetShaderResources({}, Wrapper<ID3D11ShaderResourceView>{});
		deviceContext->HSSetShaderResources({}, Span<WrapperView<ID3D11ShaderResourceView>>{});
		Vector<Wrapper<ID3D11Buffer>> buffers = deviceContext->HSGetConstantBuffers(0, 0);
		Vector<Wrapper<ID3D11SamplerState>> samplers = deviceContext->HSGetSamplers(0, 0);
		UINT numInstances = 256;
		std::pair<Wrapper<ID3D11HullShader>, Vector<Wrapper<ID3D11ClassInstance>>> shader = deviceContext->HSGetShader(numInstances);
		std::pair<Wrapper<ID3D11HullShader>, UINT> shader2 = deviceContext->HSGetShader({});
		Vector<Wrapper<ID3D11ShaderResourceView>> resources = deviceContext->HSGetShaderResources(0, 0);
	}
	{
		deviceContext->DSSetShader({}, nullptr);
		deviceContext->DSSetShader({}, Span<WrapperView<ID3D11ClassInstance>>{});
		deviceContext->DSSetConstantBuffers({}, Span<WrapperView<ID3D11Buffer>>{});
		deviceContext->DSSetConstantBuffers({}, Wrapper<ID3D11Buffer>{});
		deviceContext->DSSetSamplers({}, Wrapper<ID3D11SamplerState>{});
		deviceContext->DSSetSamplers({}, Span<WrapperView<ID3D11SamplerState>>{});
		deviceContext->DSSetShaderResources({}, Wrapper<ID3D11ShaderResourceView>{});
		deviceContext->DSSetShaderResources({}, Span<WrapperView<ID3D11ShaderResourceView>>{});
		Vector<Wrapper<ID3D11Buffer>> buffers = deviceContext->DSGetConstantBuffers(0, 0);
		Vector<Wrapper<ID3D11SamplerState>> samplers = deviceContext->DSGetSamplers(0, 0);
		UINT numInstances = 256;
		std::pair<Wrapper<ID3D11DomainShader>, Vector<Wrapper<ID3D11ClassInstance>>> shader = deviceContext->DSGetShader(numInstances);
		std::pair<Wrapper<ID3D11DomainShader>, UINT> shader2 = deviceContext->DSGetShader({});
		Vector<Wrapper<ID3D11ShaderResourceView>> resources = deviceContext->DSGetShaderResources(0, 0);
	}
	{
		deviceContext->CSSetShader({}, nullptr);
		deviceContext->CSSetShader({}, Span<WrapperView<ID3D11ClassInstance>>{});
		deviceContext->CSSetConstantBuffers({}, Span<WrapperView<ID3D11Buffer>>{});
		deviceContext->CSSetConstantBuffers({}, Wrapper<ID3D11Buffer>{});
		deviceContext->CSSetSamplers({}, Wrapper<ID3D11SamplerState>{});
		deviceContext->CSSetSamplers({}, Span<WrapperView<ID3D11SamplerState>>{});
		deviceContext->CSSetShaderResources({}, Wrapper<ID3D11ShaderResourceView>{});
		deviceContext->CSSetShaderResources({}, Span<WrapperView<ID3D11ShaderResourceView>>{});
		deviceContext->CSSetUnorderedAccessViews({}, nullptr, {});
		deviceContext->CSSetUnorderedAccessViews({}, D3D11::CSSetUnorderedAccessViewsData{ {}, nullptr, nullptr });
		Vector<Wrapper<ID3D11Buffer>> buffers = deviceContext->CSGetConstantBuffers(0, 0);
		Vector<Wrapper<ID3D11SamplerState>> samplers = deviceContext->CSGetSamplers(0, 0);
		UINT numInstances = 256;
		std::pair<Wrapper<ID3D11ComputeShader>, Vector<Wrapper<ID3D11ClassInstance>>> shader = deviceContext->CSGetShader(numInstances);
		std::pair<Wrapper<ID3D11ComputeShader>, UINT> shader2 = deviceContext->CSGetShader({});
		Vector<Wrapper<ID3D11ShaderResourceView>> resources = deviceContext->CSGetShaderResources(0, 0);
		Vector<Wrapper<ID3D11UnorderedAccessView>> uavs = deviceContext->CSGetUnorderedAccessViews(0, 0);
	}
	deviceContext->DrawIndexed({}, {}, {});
	deviceContext->Draw({}, {});
	deviceContext->DrawIndexedInstanced({}, {}, {}, {}, {});
	deviceContext->DrawInstanced({}, {}, {}, {});
	deviceContext->DrawAuto();
	deviceContext->DrawIndexedInstancedIndirect(buffer, {});
	deviceContext->DrawInstancedIndirect(buffer, {});
	deviceContext->Map(resource, {}, D3D11_MAP{}, {});
	deviceContext->Unmap(resource, {});
	deviceContext->Dispatch({}, {}, {});
	deviceContext->DispatchIndirect(buffer, {});
	{
		deviceContext->Begin(*predicate.Get());
		deviceContext->End(*predicate.Get());
		deviceContext->GetData(*predicate.Get(), {});
	}
	{
		deviceContext->SetPredication(predicate.Get(), {});
		deviceContext->GetPredication();
	}

	deviceContext->CopyResource(resource, resource);
	deviceContext->CopySubresourceRegion(resource, {}, {}, {}, {}, resource, {}, {});
	deviceContext->CopyStructureCount(buffer, {}, uav);
	deviceContext->UpdateSubresource(resource, {}, {}, {}, {}, {});
	deviceContext->ResolveSubresource(resource, {}, resource, {}, {});


	ID3D11CommandList* cl = nullptr;
	deviceContext->FinishCommandList({});
	deviceContext->ExecuteCommandList(*cl, {});

	deviceContext->GenerateMips(srv);
	deviceContext->SetResourceMinLOD(resource, {});
	deviceContext->GetResourceMinLOD(resource);

	deviceContext->ClearRenderTargetView(rtv, {});
	deviceContext->ClearUnorderedAccessViewFloat(uav, {});
	deviceContext->ClearUnorderedAccessViewUint(uav, {});
	deviceContext->ClearDepthStencilView(dsv, {}, {}, {});
	deviceContext->ClearState();
	deviceContext->Flush();
	deviceContext->GetType();
	deviceContext->GetContextFlags();

	deviceContext->RSSetState({});
	deviceContext->RSSetViewports(D3D11_VIEWPORT{});
	deviceContext->RSSetViewports(std::span<D3D11_VIEWPORT>{});
	deviceContext->RSSetScissorRects(D3D11_RECT{});
	deviceContext->RSSetScissorRects(std::span<D3D11_RECT>{});
	deviceContext->RSGetState();
	deviceContext->RSGetViewports();
	deviceContext->RSGetScissorRects();

	deviceContext->IASetVertexBuffers({}, nullptr, 0, 0);
	deviceContext->IASetVertexBuffers({}, D3D11::IASetVertexBuffersData{ {}, nullptr, nullptr, nullptr });
	deviceContext->IASetIndexBuffer({}, {}, {});
	deviceContext->IASetPrimitiveTopology({});
	deviceContext->IASetInputLayout({});
	deviceContext->IAGetVertexBuffers({}, {});
	deviceContext->IAGetIndexBuffer();
	deviceContext->IAGetPrimitiveTopology();
	Wrapper<ID3D11InputLayout> layout = deviceContext->IAGetInputLayout();

	deviceContext->OMSetRenderTargets(nullptr, nullptr);
	deviceContext->OMSetRenderTargets(Span<WrapperView<ID3D11RenderTargetView>>{}, nullptr);
	deviceContext->OMSetRenderTargetsAndUnorderedAccessViews({}, nullptr, {}, {}, nullptr);
	deviceContext->OMSetBlendState({}, {}, {});
	deviceContext->OMSetDepthStencilState({}, {});
	deviceContext->OMGetRenderTargets(0);
	deviceContext->OMGetRenderTargetsAndUnorderedAccessViews({}, {}, {});
	deviceContext->OMGetBlendState();
	deviceContext->OMGetDepthStencilState();

	deviceContext->SOSetTargets(D3D11::SOSetTargetsData{ {}, nullptr, nullptr });
	deviceContext->SOGetTargets({});
#pragma endregion// end DeviceContext
}