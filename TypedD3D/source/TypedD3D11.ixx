export module TypedD3D11;
export import TypedD3D.Shared;
export import :DeviceChild;
export import :Device;
export import :DeviceContext;
export import :InputLayout;
export import :Resources;
export import :ResourceViews;
export import :States;
export import :Shaders;

export namespace TypedD3D11 = TypedD3D::D3D11;

namespace TypedD3D::D3D11
{
	export template<class Ty>
	using Wrapper = TypedD3D::Wrapper<Ty>;
}