module;

#include "gsl/pointers"
#include <d3d11_4.h>
#include <span>



export module TypedD3D11:Device;
import :DeviceChild;
import :States;
import :Shaders;
import :InputLayout;
import :ResourceViews;
import :Resources;

import TypedD3D.Shared;

namespace TypedD3D::D3D11
{
	template<D3D11_FEATURE Feature>
	struct DeviceFeatureMap;

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_THREADING>
	{
		using type = D3D11_FEATURE_DATA_THREADING;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_DOUBLES>
	{
		using type = D3D11_FEATURE_DATA_DOUBLES;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_FORMAT_SUPPORT>
	{
		using type = D3D11_FEATURE_DATA_FORMAT_SUPPORT;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_FORMAT_SUPPORT2>
	{
		using type = D3D11_FEATURE_DATA_FORMAT_SUPPORT;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS>
	{
		using type = D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D11_OPTIONS>
	{
		using type = D3D11_FEATURE_DATA_D3D11_OPTIONS;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_ARCHITECTURE_INFO>
	{
		using type = D3D11_FEATURE_DATA_ARCHITECTURE_INFO;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D9_OPTIONS>
	{
		using type = D3D11_FEATURE_DATA_D3D9_OPTIONS;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_SHADER_MIN_PRECISION_SUPPORT>
	{
		using type = D3D11_FEATURE_DATA_SHADER_MIN_PRECISION_SUPPORT;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D9_SHADOW_SUPPORT>
	{
		using type = D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D11_OPTIONS1>
	{
		using type = D3D11_FEATURE_DATA_D3D11_OPTIONS1;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D9_SIMPLE_INSTANCING_SUPPORT>
	{
		using type = D3D11_FEATURE_DATA_D3D9_SIMPLE_INSTANCING_SUPPORT;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_MARKER_SUPPORT>
	{
		using type = D3D11_FEATURE_DATA_MARKER_SUPPORT;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D9_OPTIONS1>
	{
		using type = D3D11_FEATURE_DATA_D3D9_OPTIONS1;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D11_OPTIONS2>
	{
		using type = D3D11_FEATURE_DATA_D3D11_OPTIONS2;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D11_OPTIONS3>
	{
		using type = D3D11_FEATURE_DATA_D3D11_OPTIONS3;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT>
	{
		using type = D3D11_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D11_OPTIONS4>
	{
		using type = D3D11_FEATURE_DATA_D3D11_OPTIONS4;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_SHADER_CACHE>
	{
		using type = D3D11_FEATURE_DATA_SHADER_CACHE;
	};

	template<>
	struct DeviceFeatureMap<D3D11_FEATURE_D3D11_OPTIONS5>
	{
		using type = D3D11_FEATURE_DATA_D3D11_OPTIONS5;
	};
}

namespace TypedD3D
{
	template<std::derived_from<ID3D11Device> Ty>
	struct UntaggedTraits<Ty>
	{
		template<class Derived>
		using Interface = Ty*;
	};

	template<>
	struct UntaggedTraits<ID3D11Device>
	{
		using unknown_type = ID3D11Device;

		template<class Derived>
		struct Interface : public InterfaceBase<UntaggedTraits<Derived>>
		{
			HRESULT CheckCounter(
				const D3D11_COUNTER_DESC& desc,
				D3D11_COUNTER_TYPE& type,
				UINT& activeCounters,
				LPSTR optName,
				UINT* optNameLength,
				LPSTR optUnits,
				UINT* optUnitsLength,
				LPSTR optDescription,
				UINT* optDescriptionLength)
			{
				return Self().CheckCounter(
					&desc,
					&type,
					&activeCounters,
					optName,
					optNameLength,
					optUnits,
					optUnitsLength,
					optDescription,
					optDescriptionLength);
			}

			D3D11_COUNTER_INFO CheckCounterInfo()
			{
				D3D11_COUNTER_INFO info;
				Self().CheckCounterInfo(&info);
				return info;
			}

			template<D3D11_FEATURE Feature>
			typename D3D11::DeviceFeatureMap<Feature>::type CheckFeatureSupport()
			{
				typename D3D11::DeviceFeatureMap<Feature>::type data;
				ThrowIfFailed(Self().CheckFeatureSupport(Feature, &data, sizeof(data)));
				return data;
			}

			D3D11_FORMAT_SUPPORT CheckFormatSupport(DXGI_FORMAT format)
			{
				UINT formatSupport;
				ThrowIfFailed(Self().CheckFormatSupport(format, &formatSupport));
				return static_cast<D3D11_FORMAT_SUPPORT>(formatSupport);
			}

			UINT CheckMultisampleQualityLevels(DXGI_FORMAT format, UINT sampleCount)
			{
				UINT qualityLevels;
				ThrowIfFailed(Self().CheckMultisampleQualityLevels(format, sampleCount, &qualityLevels));
				return qualityLevels;
			}

			Wrapper<ID3D11BlendState> CreateBlendState(const D3D11_BLEND_DESC& desc)
			{
				return ForwardFunction<Wrapper<ID3D11BlendState>>(&unknown_type::CreateBlendState, Self(), &desc);
			}

			Wrapper<ID3D11Buffer> CreateBuffer(
				const D3D11_BUFFER_DESC& desc,
				const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
			{
				return ForwardFunction<Wrapper<ID3D11Buffer>>(&unknown_type::CreateBuffer, Self(), &desc, optInitialData);
			}

			Wrapper<ID3D11ClassLinkage> CreateClassLinkage()
			{
				return ForwardFunction<Wrapper<ID3D11ClassLinkage>>(&unknown_type::CreateClassLinkage, Self());
			}

			Wrapper<ID3D11ComputeShader> CreateComputeShader(
				gsl::not_null<WrapperView<ID3DBlob>> pShaderBytecode,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return CreateComputeShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), pClassLinkage);
			}

			Wrapper<ID3D11ComputeShader> CreateComputeShader(
				gsl::not_null<const void*> pShaderBytecode,
				SIZE_T BytecodeLength,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return ForwardFunction<Wrapper<ID3D11ComputeShader>>(&unknown_type::CreateComputeShader, Self(), pShaderBytecode, BytecodeLength, pClassLinkage.Get());
			}

			Wrapper<ID3D11Counter> CreateCounter(
				const D3D11_COUNTER_DESC& pCounterDesc)
			{
				return ForwardFunction<Wrapper<ID3D11Counter>>(&unknown_type::CreateCounter, Self(), &pCounterDesc);
			}

			template<std::derived_from<ID3D11DeviceContext> DeviceContextTy = ID3D11DeviceContext>
			Wrapper<DeviceContextTy> CreateDeferredContext(
				UINT ContextFlags)
			{
				return ForwardFunction<Wrapper<DeviceContextTy>, ID3D11DeviceContext>(&unknown_type::CreateDeferredContext, Self(), ContextFlags);
			}

			Wrapper<ID3D11DepthStencilState> CreateDepthStencilState(
				const D3D11_DEPTH_STENCIL_DESC& pDepthStencilDesc)
			{
				return ForwardFunction<Wrapper<ID3D11DepthStencilState>>(&unknown_type::CreateDepthStencilState, Self(), &pDepthStencilDesc);
			}

			Wrapper<ID3D11DepthStencilView> CreateDepthStencilView(
				gsl::not_null<WrapperView<ID3D11Resource>> pResource,
				const D3D11_DEPTH_STENCIL_VIEW_DESC* optDesc = nullptr)
			{
				return ForwardFunction<Wrapper<ID3D11DepthStencilView>>(&unknown_type::CreateDepthStencilView, Self(), pResource.get().Get(), optDesc);
			}

			Wrapper<ID3D11DomainShader> CreateDomainShader(
				gsl::not_null<WrapperView<ID3DBlob>> pShaderBytecode,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return CreateDomainShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), pClassLinkage);
			}

			Wrapper<ID3D11DomainShader> CreateDomainShader(
				gsl::not_null<const void*> pShaderBytecode,
				SIZE_T BytecodeLength,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return ForwardFunction<Wrapper<ID3D11DomainShader>>(&unknown_type::CreateDomainShader, Self(), pShaderBytecode, BytecodeLength, pClassLinkage.Get());
			}

			Wrapper<ID3D11GeometryShader> CreateGeometryShader(
				gsl::not_null<WrapperView<ID3DBlob>> pShaderBytecode,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return CreateGeometryShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), pClassLinkage);
			}

			Wrapper<ID3D11GeometryShader> CreateGeometryShader(
				gsl::not_null<const void*> pShaderBytecode,
				SIZE_T BytecodeLength,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return ForwardFunction<Wrapper<ID3D11GeometryShader>>(&unknown_type::CreateGeometryShader, Self(), pShaderBytecode, BytecodeLength, pClassLinkage.Get());
			}

			Wrapper<ID3D11GeometryShader> CreateGeometryShaderWithStreamOutput(
				gsl::not_null<WrapperView<ID3DBlob>> pShaderBytecode,
				std::span<const D3D11_SO_DECLARATION_ENTRY> pSODeclaration,
				std::span<const UINT> pBufferStrides,
				UINT RasterizedStream,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return CreateGeometryShaderWithStreamOutput(
					pShaderBytecode->GetBufferPointer(),
					pShaderBytecode->GetBufferSize(),
					pSODeclaration,
					pBufferStrides,
					RasterizedStream,
					pClassLinkage);
			}

			Wrapper<ID3D11GeometryShader> CreateGeometryShaderWithStreamOutput(
				gsl::not_null<const void*> pShaderBytecode,
				SIZE_T BytecodeLength,
				std::span<const D3D11_SO_DECLARATION_ENTRY> pSODeclaration,
				std::span<const UINT> pBufferStrides,
				UINT RasterizedStream,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return ForwardFunction<Wrapper<ID3D11GeometryShader>>(&unknown_type::CreateGeometryShaderWithStreamOutput,
					Self(),
					pShaderBytecode,
					BytecodeLength,
					pSODeclaration.data(),
					pSODeclaration.size(),
					pBufferStrides.data(),
					pBufferStrides.size(),
					RasterizedStream,
					pClassLinkage.Get());
			}

			Wrapper<ID3D11HullShader> CreateHullShader(
				gsl::not_null<WrapperView<ID3DBlob>> pShaderBytecode,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return CreateHullShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), pClassLinkage);
			}

			Wrapper<ID3D11HullShader> CreateHullShader(
				gsl::not_null<const void*> pShaderBytecode,
				SIZE_T BytecodeLength,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return ForwardFunction<Wrapper<ID3D11HullShader>>(&unknown_type::CreateHullShader, Self(), pShaderBytecode, BytecodeLength, pClassLinkage.Get());
			}

			Wrapper<ID3D11InputLayout> CreateInputLayout(
				std::span<const D3D11_INPUT_ELEMENT_DESC> inputElementDescs,
				gsl::not_null<WrapperView<ID3DBlob>> pShaderBytecodeWithInputSignature)
			{
				return CreateInputLayout(inputElementDescs, pShaderBytecodeWithInputSignature->GetBufferPointer(), pShaderBytecodeWithInputSignature->GetBufferSize());
			}

			Wrapper<ID3D11InputLayout> CreateInputLayout(
				D3D11_INPUT_ELEMENT_DESC inputElementDescs,
				gsl::not_null<WrapperView<ID3DBlob>> pShaderBytecodeWithInputSignature)
			{
				return CreateInputLayout(inputElementDescs, pShaderBytecodeWithInputSignature->GetBufferPointer(), pShaderBytecodeWithInputSignature->GetBufferSize());
			}

			Wrapper<ID3D11InputLayout> CreateInputLayout(
				D3D11_INPUT_ELEMENT_DESC inputElementDescs,
				gsl::not_null<const void*> pShaderBytecodeWithInputSignature,
				SIZE_T BytecodeLength)
			{
				return CreateInputLayout(
					std::span{ &inputElementDescs, 1 },
					pShaderBytecodeWithInputSignature,
					BytecodeLength);
			}

			Wrapper<ID3D11InputLayout> CreateInputLayout(
				std::span<const D3D11_INPUT_ELEMENT_DESC> inputElementDescs,
				gsl::not_null<const void*> pShaderBytecodeWithInputSignature,
				SIZE_T BytecodeLength)
			{
				return ForwardFunction<Wrapper<ID3D11InputLayout>>(
					&unknown_type::CreateInputLayout,
					Self(),
					inputElementDescs.data(),
					static_cast<UINT>(inputElementDescs.size()),
					pShaderBytecodeWithInputSignature,
					BytecodeLength);
			}

			Wrapper<ID3D11PixelShader> CreatePixelShader(
				gsl::not_null<WrapperView<ID3DBlob>> pShaderBytecode,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return CreatePixelShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), pClassLinkage);
			}

			Wrapper<ID3D11PixelShader> CreatePixelShader(
				gsl::not_null<const void*> pShaderBytecode,
				SIZE_T BytecodeLength,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return ForwardFunction<Wrapper<ID3D11PixelShader>>(&unknown_type::CreatePixelShader, Self(), pShaderBytecode, BytecodeLength, pClassLinkage.Get());
			}

			Wrapper<ID3D11Predicate> CreatePredicate(const D3D11_QUERY_DESC& pPredicateDesc)
			{
				return ForwardFunction<Wrapper<ID3D11Predicate>>(&unknown_type::CreatePredicate, Self(), &pPredicateDesc);
			}

			Wrapper<ID3D11Query> CreateQuery(const D3D11_QUERY_DESC& pQueryDesc)
			{
				return ForwardFunction<Wrapper<ID3D11Query>>(&unknown_type::CreateQuery, Self(), &pQueryDesc);
			}

			Wrapper<ID3D11RasterizerState>  CreateRasterizerState(
				const D3D11_RASTERIZER_DESC& pRasterizerDesc)
			{
				return ForwardFunction<Wrapper<ID3D11RasterizerState>>(&unknown_type::CreateRasterizerState, Self(), &pRasterizerDesc);
			}

			Wrapper<ID3D11RenderTargetView> CreateRenderTargetView(
				gsl::not_null<WrapperView<ID3D11Resource>> pResource,
				const D3D11_RENDER_TARGET_VIEW_DESC* optDesc = nullptr)
			{
				return ForwardFunction<Wrapper<ID3D11RenderTargetView>>(&unknown_type::CreateRenderTargetView, Self(), pResource.get().Get(), optDesc);
			}

			Wrapper<ID3D11SamplerState> CreateSamplerState(
				const D3D11_SAMPLER_DESC& pSamplerDesc)
			{
				return ForwardFunction<Wrapper<ID3D11SamplerState>>(&unknown_type::CreateSamplerState, Self(), &pSamplerDesc);
			}

			Wrapper<ID3D11ShaderResourceView> CreateShaderResourceView(
				gsl::not_null<WrapperView<ID3D11Resource>> pResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC* optDesc = nullptr)
			{
				return ForwardFunction<Wrapper<ID3D11ShaderResourceView>>(&unknown_type::CreateShaderResourceView, Self(), pResource.get().Get(), optDesc);
			}

			Wrapper<ID3D11Texture1D> CreateTexture1D(
				const D3D11_TEXTURE1D_DESC& pDesc,
				const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
			{
				return ForwardFunction<Wrapper<ID3D11Texture1D>>(&unknown_type::CreateTexture1D, Self(), &pDesc, optInitialData);
			}

			Wrapper<ID3D11Texture2D> CreateTexture2D(
				const D3D11_TEXTURE2D_DESC& pDesc,
				const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
			{
				return ForwardFunction<Wrapper<ID3D11Texture2D>>(&unknown_type::CreateTexture2D, Self(), &pDesc, optInitialData);
			}

			Wrapper<ID3D11Texture3D> CreateTexture3D(
				const D3D11_TEXTURE3D_DESC& pDesc,
				const D3D11_SUBRESOURCE_DATA* optInitialData = nullptr)
			{
				return ForwardFunction<Wrapper<ID3D11Texture3D>>(&unknown_type::CreateTexture3D, Self(), &pDesc, optInitialData);
			}

			Wrapper<ID3D11UnorderedAccessView> CreateUnorderedAccessView(
				gsl::not_null<WrapperView<ID3D11Resource>> pResource,
				const D3D11_UNORDERED_ACCESS_VIEW_DESC* optDesc = nullptr)
			{
				return ForwardFunction<Wrapper<ID3D11UnorderedAccessView>>(&unknown_type::CreateUnorderedAccessView, Self(), pResource.get().Get(), optDesc);
			}


			Wrapper<ID3D11VertexShader> CreateVertexShader(
				gsl::not_null<WrapperView<ID3DBlob>> pShaderBytecode,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return CreateVertexShader(pShaderBytecode->GetBufferPointer(), pShaderBytecode->GetBufferSize(), pClassLinkage);
			}

			Wrapper<ID3D11VertexShader> CreateVertexShader(
				gsl::not_null<const void*> pShaderBytecode,
				SIZE_T BytecodeLength,
				WrapperView<ID3D11ClassLinkage> pClassLinkage)
			{
				return ForwardFunction<Wrapper<ID3D11VertexShader>>(&unknown_type::CreateVertexShader, Self(), pShaderBytecode, BytecodeLength, pClassLinkage.Get());
			}

			D3D11_CREATE_DEVICE_FLAG GetCreationFlags()
			{
				return static_cast<D3D11_CREATE_DEVICE_FLAG>(Self().GetCreationFlags());
			}

			HRESULT GetDeviceRemovedReason()
			{
				return Self().GetDeviceRemovedReason();
			}

			D3D11_RAISE_FLAG GetExceptionMode()
			{
				return static_cast<D3D11_RAISE_FLAG>(Self().GetExceptionMode());
			}

			D3D_FEATURE_LEVEL GetFeatureLevel()
			{
				return Self().GetFeatureLevel();
			}

			template<std::derived_from<ID3D11DeviceContext> DeviceContextTy = ID3D11DeviceContext>
            Wrapper<DeviceContextTy> GetImmediateContext()
            {
                return ForwardFunction<Wrapper<DeviceContextTy>, ID3D11DeviceContext>(&unknown_type::GetImmediateContext, Self());
            }

			HRESULT GetPrivateData(REFGUID guid,
				UINT* pDataSize,
				void* pData)
			{
				return Self().GetPrivateData(guid, pDataSize, pData);
			}

			HRESULT OpenSharedResource(
				HANDLE hResource,
				REFIID ReturnedInterface,
				void** ppResource)
			{
				return Self().OpenSharedResource(hResource, ReturnedInterface, ppResource);
			}

			HRESULT SetExceptionMode(
				D3D11_RAISE_FLAG flags)
			{
				return Self().SetExceptionMode(flags);
			}

			HRESULT SetPrivateData(
				REFGUID    guid,
				UINT       DataSize,
				const void* pData)
			{
				return Self().SetPrivateData(guid, DataSize, pData);
			}

			HRESULT SetPrivateDataInterface(
				REFGUID        guid,
				const IUnknown* pData)
			{
				return Self().SetPrivateDataInterface(guid, pData);
			}
		private:
			using InterfaceBase<UntaggedTraits<Derived>>::Self;
			using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
		};
	};
}

namespace TypedD3D::D3D11
{
	export template<class DeviceTy = ID3D11Device, class DeviceContextTy = ID3D11DeviceContext>
	auto CreateDevice(
		IDXGIAdapter* optAdapter,
		D3D_DRIVER_TYPE DriverType,
		HMODULE Software,
		UINT Flags,
		std::span<const D3D_FEATURE_LEVEL> pFeatureLevels,
		UINT SDKVersion,
		D3D_FEATURE_LEVEL* optSelectedFeatureLevel = nullptr)
	{
		Wrapper<ID3D11Device> device;
		Wrapper<ID3D11DeviceContext> deviceContext;
		ThrowIfFailed(D3D11CreateDevice(
			optAdapter,
			DriverType,
			Software,
			Flags,
			pFeatureLevels.data(),
			static_cast<UINT>(pFeatureLevels.size()),
			SDKVersion,
			OutPtr{ device },
			optSelectedFeatureLevel,
			OutPtr{ deviceContext }));

		if constexpr(std::same_as<DeviceTy, ID3D11Device> && std::same_as<DeviceContextTy, ID3D11DeviceContext>)
			return std::pair{ device, deviceContext };
		if constexpr(std::same_as<DeviceTy, ID3D11Device>)
			return std::pair{ device, Cast<DeviceContextTy>(std::move(deviceContext)) };
		if constexpr(std::same_as<DeviceContextTy, ID3D11DeviceContext>)
			return std::pair{ Cast<DeviceTy>(std::move(device)), deviceContext };

		return std::pair{ Cast<DeviceTy>(std::move(device)), Cast<DeviceContextTy>(std::move(deviceContext)) };
	}

	export template<class DeviceTy = ID3D11Device, class DeviceContextTy = ID3D11DeviceContext>
    auto CreateDevice(
            IDXGIAdapter* optAdapter,
            D3D_DRIVER_TYPE DriverType,
            HMODULE Software,
            UINT Flags,
            D3D_FEATURE_LEVEL pFeatureLevels,
            UINT SDKVersion,
			D3D_FEATURE_LEVEL* optSelectedFeatureLevel = nullptr)
    {
        return CreateDevice<DeviceTy, DeviceContextTy>(optAdapter, DriverType, Software, Flags, { &pFeatureLevels, 1 }, SDKVersion, optSelectedFeatureLevel);
    }
}
