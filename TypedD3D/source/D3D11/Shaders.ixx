module;

#include <concepts>
#include <d3d11_4.h>
export module TypedD3D11:Shaders;
import :DeviceChild;
import TypedD3D.Shared;

namespace TypedD3D::D3D11
{
	template<class Ty>
	concept ShaderConcept =
		std::same_as<Ty, ID3D11VertexShader> ||
		std::same_as<Ty, ID3D11DomainShader> ||
		std::same_as<Ty, ID3D11HullShader> ||
		std::same_as<Ty, ID3D11GeometryShader> ||
		std::same_as<Ty, ID3D11PixelShader> ||
		std::same_as<Ty, ID3D11ComputeShader>;
}

namespace TypedD3D
{
	template<D3D11::ShaderConcept Ty>
	struct Trait<Untagged<Ty>>
	{
		using inner_type = Ty;

		using inner_tag = Ty;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		class Interface : public Trait<Untagged<ID3D11DeviceChild>>::Interface<Derived>
		{

		};
	};
}