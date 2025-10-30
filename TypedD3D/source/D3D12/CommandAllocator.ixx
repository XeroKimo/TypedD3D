module;

#include <d3d12.h>

#include <assert.h>

export module TypedD3D12:CommandAllocator;
import TypedD3D.Shared;
import :Wrappers;

namespace TypedD3D::D3D12
{
	template<D3D12_COMMAND_LIST_TYPE Type>
	struct CommandListTypeToTraitMap;

	template<>
	struct CommandListTypeToTraitMap<D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT>
	{
		template<class Ty>
		using type = DirectTag<Ty>;
	};

	template<>
	struct CommandListTypeToTraitMap<D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE>
	{
		template<class Ty>
		using type = ComputeTag<Ty>;
	};

	template<>
	struct CommandListTypeToTraitMap<D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COPY>
	{
		template<class Ty>
		using type = CopyTag<Ty>;
	};

	template<>
	struct CommandListTypeToTraitMap<D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_BUNDLE>
	{
		template<class Ty>
		using type = BundleTag<Ty>;
	};

	template<D3D12_COMMAND_LIST_TYPE Type, class IUnknown>
	using CommandListTypeToTrait = typename CommandListTypeToTraitMap<Type>::template type<IUnknown>;

	template<template<class> class Tag>
	concept CommandAllocatorEnabledTag = SameTagAs<Tag, Untagged>
		|| SameTagAs<Tag, DirectTag>
		|| SameTagAs<Tag, ComputeTag>
		|| SameTagAs<Tag, CopyTag>
		|| SameTagAs<Tag, BundleTag>;

	export template<template<class> class Ty>
		requires CommandAllocatorEnabledTag<Ty>
	constexpr D3D12_COMMAND_LIST_TYPE CommandListTraitToType = static_cast<D3D12_COMMAND_LIST_TYPE>(-1);

	template<>
	constexpr D3D12_COMMAND_LIST_TYPE CommandListTraitToType<DirectTag> = D3D12_COMMAND_LIST_TYPE_DIRECT;
	template<>
	constexpr D3D12_COMMAND_LIST_TYPE CommandListTraitToType<ComputeTag> = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	template<>
	constexpr D3D12_COMMAND_LIST_TYPE CommandListTraitToType<CopyTag> = D3D12_COMMAND_LIST_TYPE_COPY;
	template<>
	constexpr D3D12_COMMAND_LIST_TYPE CommandListTraitToType<BundleTag> = D3D12_COMMAND_LIST_TYPE_BUNDLE;
}

namespace TypedD3D
{
	template<template<class> class Tag>
		requires D3D12::CommandAllocatorEnabledTag<Tag>
	struct Trait<Tag<ID3D12CommandAllocator>>
	{
		using inner_type = ID3D12CommandAllocator;

		using inner_tag = ID3D12CommandAllocator;

		template<class NewInner>
		using ReplaceInnerType = Tag<NewInner>;

		template<class NewInner>
		using trait_template = Tag<NewInner>;

		template<class Derived>
		class Interface : public InterfaceBase<Tag<Derived>>
		{
		public:
			HRESULT Reset() { return Self().Reset(); }

		private:
			using InterfaceBase<Tag<Derived>>::Self;
			using InterfaceBase<Tag<Derived>>::ToDerived;
		};
	};
}

//namespace TypedD3D::D3D12
//{
//	export template<D3D12_COMMAND_LIST_TYPE Type, class UnknownType>
//	using CommandAllocator_t = StrongWrapper<CommandListTypeToTrait<type, UnknownType>>;
//
//	export template<D3D12_COMMAND_LIST_TYPE Type, class UnknownType>
//	using CommandAllocatorView_t = WeakWrapper<CommandListTypeToTrait<type, UnknownType>>;
//
//	export template<D3D12_COMMAND_LIST_TYPE Type>
//	using CommandAllocator = CommandAllocator_t<Type, ID3D12CommandAllocator>;
//
//	export template<D3D12_COMMAND_LIST_TYPE Type>
//	using CommandAllocatorView_t = CommandAllocatorView_t<Type, ID3D12CommandAllocator>;
//}