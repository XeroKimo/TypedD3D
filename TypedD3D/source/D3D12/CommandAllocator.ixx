module;

#include <d3d12.h>
#include <wrl/client.h>
#include <assert.h>

export module TypedD3D12:CommandAllocator;
import TypedD3D.Shared;
import :Wrappers;

namespace TypedD3D::D3D12
{
	export template<D3D12_COMMAND_LIST_TYPE Type>
	struct CommandListTypeToTrait;

	template<>
	struct CommandListTypeToTrait<D3D12_COMMAND_LIST_TYPE_DIRECT> { template<class Ty> using type = DirectTraits<Ty>; };

	template<>
	struct CommandListTypeToTrait<D3D12_COMMAND_LIST_TYPE_COMPUTE> { template<class Ty> using type = ComputeTraits<Ty>; };

	template<>
	struct CommandListTypeToTrait<D3D12_COMMAND_LIST_TYPE_COPY> { template<class Ty> using type = CopyTraits<Ty>; };

	template<>
	struct CommandListTypeToTrait<D3D12_COMMAND_LIST_TYPE_BUNDLE> { template<class Ty> using type = BundleTraits<Ty>; };

	export template<template<class> class Trait>
	constexpr D3D12_COMMAND_LIST_TYPE TraitToCommandListType;

	template<>
	constexpr D3D12_COMMAND_LIST_TYPE TraitToCommandListType<DirectTraits> = D3D12_COMMAND_LIST_TYPE_DIRECT;

	template<>
	constexpr D3D12_COMMAND_LIST_TYPE TraitToCommandListType<ComputeTraits> = D3D12_COMMAND_LIST_TYPE_COMPUTE;

	template<>
	constexpr D3D12_COMMAND_LIST_TYPE TraitToCommandListType<CopyTraits> = D3D12_COMMAND_LIST_TYPE_COPY;

	template<>
	constexpr D3D12_COMMAND_LIST_TYPE TraitToCommandListType<BundleTraits> = D3D12_COMMAND_LIST_TYPE_BUNDLE;

	export template<D3D12_COMMAND_LIST_TYPE Type>
	using CommandAllocator_t = IUnknownWrapper<ID3D12CommandAllocator, CommandListTypeToTrait<Type>::template type>;

	template<D3D12_COMMAND_LIST_TYPE Type>
	struct CommandAllocatorTraits
	{
		static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;

		using value_type = ID3D12CommandAllocator;
		using pointer = ID3D12CommandAllocator*;
		using const_pointer = const ID3D12CommandAllocator*;
		using reference = ID3D12CommandAllocator&;
		using const_reference = const ID3D12CommandAllocator&;

		template<class DerivedSelf>
		class Interface
		{
		private:
			using derived_self = DerivedSelf;

		public:
			HRESULT Reset() { return Get().Reset(); }

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};

	template<>
	struct DirectTraits<ID3D12CommandAllocator> : public CommandAllocatorTraits<D3D12_COMMAND_LIST_TYPE_DIRECT> {};

	template<>
	struct ComputeTraits<ID3D12CommandAllocator> : public CommandAllocatorTraits<D3D12_COMMAND_LIST_TYPE_COMPUTE> {};

	template<>
	struct CopyTraits<ID3D12CommandAllocator> : public CommandAllocatorTraits<D3D12_COMMAND_LIST_TYPE_COPY> {};

	template<>
	struct BundleTraits<ID3D12CommandAllocator> : public CommandAllocatorTraits<D3D12_COMMAND_LIST_TYPE_BUNDLE> {};

	namespace Aliases
	{
		export using D3D12DirectCommandAllocator = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
		export using D3D12BundleCommandAllocator = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
		export using D3D12ComputeCommandAllocator = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
		export using D3D12CopyCommandAllocator = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COPY>;
	}
}