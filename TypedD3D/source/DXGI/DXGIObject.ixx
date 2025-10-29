module;

#include <concepts>
#include <dxgi1_6.h>

#include <utility>

export module TypedDXGI:DXGIObject;
import TypedD3D.Shared;

namespace TypedD3D
{
	template<std::derived_from<IDXGIObject> Ty>
	struct Trait<Untagged<Ty>>
	{
		using inner_type = Ty;

		using inner_tag = Ty;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class Derived>
		using Interface = Ty*;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

	};

	static_assert(IUnknownTrait<Untagged<IDXGIOutput>>);
}