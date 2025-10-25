module;

#include <d3d11_4.h>
#include <concepts>

export module TypedD3D11:States;
import :DeviceChild;
import TypedD3D.Shared;

namespace TypedD3D::D3D11
{
	template<class Ty>
	struct ViewToStateDesc;

	template<>
	struct ViewToStateDesc<ID3D11DepthStencilState>
	{
		using type = D3D11_DEPTH_STENCIL_VIEW_DESC;
	};

	template<>
	struct ViewToStateDesc<ID3D11SamplerState>
	{
		using type = D3D11_UNORDERED_ACCESS_VIEW_DESC;
	};

	template<>
	struct ViewToStateDesc<ID3D11BlendState>
	{
		using type = D3D11_RENDER_TARGET_VIEW_DESC;
	};

	template<>
	struct ViewToStateDesc<ID3D11RasterizerState>
	{
		using type = D3D11_SHADER_RESOURCE_VIEW_DESC;
	};

	template<class Ty>
	concept StateObject = 
		std::same_as<Ty, ID3D11DepthStencilState> ||
		std::same_as<Ty, ID3D11SamplerState> ||
		std::same_as<Ty, ID3D11BlendState> ||
		std::same_as<Ty, ID3D11RasterizerState>;
}

namespace TypedD3D
{
	template<D3D11::StateObject Ty>
	struct UntaggedTraits<Ty>
	{
		using value_type = Ty;
		using pointer = Ty*;
		using const_pointer = const Ty*;
		using reference = Ty&;
		using const_reference = const Ty&;

		template<class Derived>
		class Interface : public UntaggedTraits<ID3D11DeviceChild>::Interface<Derived>, public InterfaceBase<UntaggedTraits<Derived>>
		{
		public:
			typename D3D11::ViewToStateDesc<value_type>::type GetDesc()
			{
				typename D3D11::ViewToStateDesc<value_type>::type description;
				Self().GetDesc(&description);
				return description;
			}

		private:
			using InterfaceBase<UntaggedTraits<Derived>>::Self;
			using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
		};
	};
}