export module TypedDXGI;
export import TypedD3D.Shared;
export import :Adapter;
export import :Factory;
export import :SwapChain;

namespace TypedD3D::DXGI
{
	export template<class Ty>
		using Wrapper = TypedD3D::Wrapper<Ty>;
}

export namespace TypedDXGI = TypedD3D::DXGI;
