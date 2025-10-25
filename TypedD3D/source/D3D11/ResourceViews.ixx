module;

#include <d3d11_4.h>
#include <concepts>


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

		template<class Derived>
		class Interface : public UntaggedTraits<ID3D11DeviceChild>::Interface<Derived>
		{
		private:
			using derived_self = Derived;

		public:
			template<std::derived_from<ID3D11Resource> Ty = ID3D11Resource>
			Wrapper<Ty> GetResource() const
			{
				Wrapper<ID3D11Resource> resource;
				Self().GetResource(OutPtr{ resource });
				return Cast<Ty>(std::move(resource));
			}

		private:
			using InterfaceBase<UntaggedTraits<Derived>>::Self;
			using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
		};
	};

	export template<class Ty>
		requires (std::derived_from<Ty, ID3D11View> && (!std::same_as<ID3D11View, Ty>))
	struct UntaggedTraits<Ty>
	{
		using value_type = Ty;
		using pointer = Ty*;
		using const_pointer = const Ty*;
		using reference = Ty&;
		using const_reference = const Ty&;

		template<class Derived>
		class Interface : public UntaggedTraits<ID3D11View>::Interface<Derived>, public InterfaceBase<UntaggedTraits<Derived>>
		{
		private:
			using derived_self = Derived;

		public:
			typename D3D11::ViewToResourceDesc<value_type>::type GetDesc()
			{
				typename D3D11::ViewToResourceDesc<value_type>::type description;
				Self().GetDesc(&description);
				return description;
			}

		private:
			using InterfaceBase<UntaggedTraits<Derived>>::Self;
			using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
		};
	};
}