module;

#include <concepts>
#include <dxgi1_6.h>

#include <utility>

export module TypedDXGI:DXGIObject;
import TypedD3D.Shared;

namespace TypedD3D
{
	template<std::derived_from<IDXGIObject> Ty>
	struct UntaggedTraits<Ty>
	{
		template<class Derived>
		using Interface = Ty*;
	};
}