module;
#include <concepts>
#include <d3d12.h>

export module TypedD3D12:D3D12Object;
import	TypedD3D.Shared;

namespace TypedD3D
{
	template<std::derived_from<ID3D12Object> Ty>
	struct Trait<Untagged<Ty>>
	{
		using inner_type = Ty;

		using inner_tag = Ty;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class Derived>
		using Interface = Ty*;
	};
}