module;

#include <format>
#include <Unknwn.h>
#include <minwindef.h>
#include <concepts>
#include <stdexcept>
#include <d3dcommon.h>

export module TypedD3D.Shared;
export import :Containers;

namespace TypedD3D
{
	export template<class Derived, class Base, class... Exceptions>
	concept DerivedFromExcept = std::derived_from<Derived, Base> && ((!std::derived_from<Derived, Exceptions>) && ...);

	export template<class Base, class Ty, class Ty2>
	concept DefineTraitRange = std::derived_from<Ty2, Ty> && ((std::derived_from<Base, Ty> && !std::derived_from<Base, Ty2>) || std::same_as<Base, Ty2>);

	export template<template<class> class Ty, template<class> class Ty2>
	concept SameTagAs = std::same_as<Ty<void>, Ty2<void>>;

	export template<std::derived_from<IUnknown> To, std::derived_from<IUnknown> From>
	To* Cast(From* from)
	{
		To* to;
		from->QueryInterface(&to);
		return to;
	}

	export void ThrowIfFailed(HRESULT result)
	{
		if(FAILED(result))
			throw std::runtime_error("Failed\n");
	}

	export template<IUnknownWrapper Wrapper, class BaseType = typename Wrapper::inner_type, class Func, class... Args>
	Wrapper ForwardFunction(Func function, Args&&... args)
	{
		using BaseTrait = ReplaceInnerType<typename Wrapper::trait_type, BaseType>;
		using BaseWrapper = ReplaceInnerType<Wrapper, BaseTrait>;
		static constexpr bool InvokeIID = requires(Wrapper w) { { std::invoke(function, std::forward<Args>(args)..., __uuidof(w), OutPtr{ w }) } -> std::convertible_to<HRESULT>; };
		static constexpr bool InvokeVoidIID = requires(Wrapper w) { { std::invoke(function, std::forward<Args>(args)..., __uuidof(w), OutPtr{ w }) } -> std::same_as<void>; };
		static constexpr bool InvokeHRESULT = requires(BaseWrapper w) { { std::invoke(function, std::forward<Args>(args)..., OutPtr{ w }) } -> std::convertible_to<HRESULT>; };

		if constexpr(InvokeIID)
		{
			Wrapper unknown;
			ThrowIfFailed(std::invoke(function, std::forward<Args>(args)..., __uuidof(unknown), OutPtr{ unknown }));
			return unknown;
		}
		else if constexpr(InvokeVoidIID)
		{
			Wrapper unknown;
			std::invoke(function, std::forward<Args>(args)..., __uuidof(unknown), OutPtr{ unknown });
			return unknown;
		}
		else if constexpr(InvokeHRESULT)
		{
			BaseWrapper unknown;
			ThrowIfFailed(std::invoke(function, std::forward<Args>(args)..., OutPtr{ unknown }));
			if constexpr(std::same_as<typename Wrapper::inner_type, BaseType>)
				return unknown;
			else
				return Cast<typename Wrapper::inner_type>(std::move(unknown));
		}
		else
		{
			BaseWrapper unknown;
			std::invoke(function, std::forward<Args>(args)..., OutPtr{ unknown });
			if constexpr(std::same_as<typename Wrapper::inner_type, BaseType>)
				return unknown;
			else
				return Cast<typename Wrapper::inner_type>(std::move(unknown));
		}
	}

	//export template<class Ty>
	//struct UntaggedTraits;

	//template<>
	//struct UntaggedTraits<ID3DBlob>
	//{
	//	using inner_type = ID3DBlob;
	//	template<class Derived>
	//	using Interface = ID3DBlob*;

	//	template<class NewInner>
	//	using trait_template = UntaggedTraits<NewInner>;
	//};

	//template<>
	//struct UntaggedTraits<IUnknown>
	//{
	//	using inner_type = IUnknown;
	//	template<class Derived>
	//	using Interface = IUnknown*;

	//	template<class NewInner>
	//	using trait_template = UntaggedTraits<NewInner>;
	//};

	template<>
	struct Trait<Untagged<ID3DBlob>>
	{
		using inner_type = ID3DBlob;

		using inner_tag = ID3DBlob;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class Derived>
		using Interface = ID3DBlob*;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;
	};

	template<>
	struct Trait<Untagged<IUnknown>>
	{
		using inner_type = IUnknown;

		using inner_tag = IUnknown;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class Derived>
		using Interface = IUnknown*;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;
	};

	export template<class Ty>
	struct WrapperMapper;

	export template<class Ty>
	struct WrapperViewMapper;

	export template<std::derived_from<IUnknown> Ty>
	struct WrapperMapper<Ty>
	{
		using type = StrongWrapper<Untagged<Ty>>;
	};

	export template<std::derived_from<IUnknown> Ty>
	struct WrapperViewMapper<Ty>
	{
		using type = ReplaceOuterType<typename WrapperMapper<Ty>::type, WeakWrapper>;
	};

	export template<class Ty>
	using Wrapper = WrapperMapper<Ty>::type;

	export template<class Ty>
	using WrapperView = WrapperViewMapper<Ty>::type;
}