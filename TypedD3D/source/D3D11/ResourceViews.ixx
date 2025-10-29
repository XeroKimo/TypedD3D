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
	struct Trait<Untagged<ID3D11View>>
	{
		using inner_type = ID3D11View;

		using inner_tag = ID3D11View;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		class Interface : public Trait<Untagged<ID3D11DeviceChild>>::Interface<Derived>
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
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};

	export template<class Ty>
		requires (std::derived_from<Ty, ID3D11View> && (!std::same_as<ID3D11View, Ty>))
	struct Trait<Untagged<Ty>>
	{
		using inner_type = Ty;

		using inner_tag = Ty;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		class Interface : public Trait<Untagged<ID3D11View>>::Interface<Derived>
		{
		private:
			using derived_self = Derived;

		public:
			typename D3D11::ViewToResourceDesc<inner_type>::type GetDesc()
			{
				typename D3D11::ViewToResourceDesc<inner_type>::type description;
				Self().GetDesc(&description);
				return description;
			}

		private:
			using InterfaceBase<Untagged<Derived>>::Self;
			using InterfaceBase<Untagged<Derived>>::ToDerived;
		};
	};
}