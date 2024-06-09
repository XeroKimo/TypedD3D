module;

#include <concepts>
#include <d3d11_4.h>
export module TypedD3D11:Shaders;
import :DeviceChild;
import TypedD3D.Shared;

namespace TypedD3D
{
	template<class Ty>
	concept ShaderConcept =
		std::same_as<Ty, ID3D11VertexShader> ||
		std::same_as<Ty, ID3D11DomainShader> ||
		std::same_as<Ty, ID3D11HullShader> ||
		std::same_as<Ty, ID3D11GeometryShader> ||
		std::same_as<Ty, ID3D11PixelShader> ||
		std::same_as<Ty, ID3D11ComputeShader>;

	template<ShaderConcept Ty>
	struct UntaggedTraits<Ty>
	{
		using value_type = Ty;
		using pointer = Ty*;
		using const_pointer = const Ty*;
		using reference = Ty&;
		using const_reference = const Ty&;

		template<class DerivedSelf>
		class Interface : public D3D11DeviceChildInterface<DerivedSelf>
		{

		};
	};

	export using D3D11VertexShader = Wrapper<ID3D11VertexShader>;
	export using D3D11DomainShader = Wrapper<ID3D11DomainShader>;
	export using D3D11HullShader = Wrapper<ID3D11HullShader>;
	export using D3D11GeometryShader = Wrapper<ID3D11GeometryShader>;
	export using D3D11PixelShader = Wrapper<ID3D11PixelShader>;
	export using D3D11ComputeShader = Wrapper<ID3D11ComputeShader>;
}