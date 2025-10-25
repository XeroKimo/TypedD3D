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
	struct UntaggedTraits<Ty>
	{
		using value_type = Ty;
		using pointer = Ty*;
		using const_pointer = const Ty*;
		using reference = Ty&;
		using const_reference = const Ty&;

		template<class Derived>
		class Interface : public UntaggedTraits<ID3D11DeviceChild>::Interface<Derived>
		{

		};
	};

	template<>
	struct UntaggedTraits<ID3D11VertexShader>
	{
		using value_type = ID3D11VertexShader;
		using pointer = ID3D11VertexShader*;
		using const_pointer = const ID3D11VertexShader*;
		using reference = ID3D11VertexShader&;
		using const_reference = const ID3D11VertexShader&;

		template<class Derived>
		class Interface : public UntaggedTraits<ID3D11DeviceChild>::Interface<Derived>
		{

		};
	};
}