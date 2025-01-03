module;

#include <d3d11_4.h>
#include <concepts>
#include <wrl/client.h>

export module TypedD3D11:ResourceViews;
import :DeviceChild;
import :Resources;
import TypedD3D.Shared;

namespace TypedD3D::D3D11
{
	template<class Ty>
	struct ViewToResourceDesc;

	template<>
	struct ViewToResourceDesc<ID3D11DepthStencilView>
	{
		using type = D3D11_DEPTH_STENCIL_VIEW_DESC;
	};

	template<>
	struct ViewToResourceDesc<ID3D11UnorderedAccessView>
	{
		using type = D3D11_UNORDERED_ACCESS_VIEW_DESC;
	};

	template<>
	struct ViewToResourceDesc<ID3D11RenderTargetView>
	{
		using type = D3D11_RENDER_TARGET_VIEW_DESC;
	};

	template<>
	struct ViewToResourceDesc<ID3D11ShaderResourceView>
	{
		using type = D3D11_SHADER_RESOURCE_VIEW_DESC;
	};
}

namespace TypedD3D
{

	template<>
	struct UntaggedTraits<ID3D11View>
	{
		using value_type = ID3D11View;
		using pointer = ID3D11View*;
		using const_pointer = const ID3D11View*;
		using reference = ID3D11View&;
		using const_reference = const ID3D11View&;

		template<class DerivedSelf>
		class Interface : public D3D11::DeviceChildInterface<DerivedSelf>
		{
		private:
			using derived_self = DerivedSelf;

		public:
			template<std::derived_from<ID3D11Resource> Ty = ID3D11Resource>
			Wrapper<Ty> GetResource() const
			{
				Microsoft::WRL::ComPtr<ID3D11Resource> resource;
				Get().GetResource(&resource);
				return Wrapper<Ty>(TypedD3D::Cast<Ty>(resource));
			}

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }

			const derived_self& ToDerived() const { return static_cast<const derived_self&>(*this); }
			reference Get() const { return *ToDerived().derived_self::Get(); }
		};
	};

	export template<class Ty>
		requires std::derived_from<Ty, ID3D11View> && (!std::same_as<ID3D11View, Ty>)
	struct UntaggedTraits<Ty>
	{
		using value_type = Ty;
		using pointer = Ty*;
		using const_pointer = const Ty*;
		using reference = Ty&;
		using const_reference = const Ty&;

		template<class DerivedSelf>
		class Interface : public UntaggedTraits<ID3D11View>::Interface<DerivedSelf>
		{
		private:
			using derived_self = DerivedSelf;

		public:
			typename D3D11::ViewToResourceDesc<value_type>::type GetDesc()
			{
				typename D3D11::ViewToResourceDesc<value_type>::type description;
				Get().GetDesc(&description);
				return description;
			}

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};
}