module;

#include <d3d12.h>
#include <wrl/client.h>
#include <assert.h>

export module TypedD3D12:CommandAllocator;
import TypedD3D.Shared;
import :Wrappers;

namespace TypedD3D::D3D12
{
	export template<D3D12_COMMAND_LIST_TYPE Tag>
	constexpr TraitTags CommandListTypeToTraitTag;

	template<>
	constexpr TraitTags CommandListTypeToTraitTag<D3D12_COMMAND_LIST_TYPE_DIRECT> = TraitTags::Direct;

	template<>
	constexpr TraitTags CommandListTypeToTraitTag<D3D12_COMMAND_LIST_TYPE_COMPUTE> = TraitTags::Compute;

	template<>
	constexpr TraitTags CommandListTypeToTraitTag<D3D12_COMMAND_LIST_TYPE_COPY> = TraitTags::Copy;

	template<>
	constexpr TraitTags CommandListTypeToTraitTag<D3D12_COMMAND_LIST_TYPE_BUNDLE> = TraitTags::Bundle;

	export template<TraitTags Tag>
	constexpr D3D12_COMMAND_LIST_TYPE TraitTagToCommandListType;

	template<>
	constexpr D3D12_COMMAND_LIST_TYPE TraitTagToCommandListType<TraitTags::Direct> = D3D12_COMMAND_LIST_TYPE_DIRECT;

	template<>
	constexpr D3D12_COMMAND_LIST_TYPE TraitTagToCommandListType<TraitTags::Compute> = D3D12_COMMAND_LIST_TYPE_COMPUTE;

	template<>
	constexpr D3D12_COMMAND_LIST_TYPE TraitTagToCommandListType< TraitTags::Copy> = D3D12_COMMAND_LIST_TYPE_COPY;

	template<>
	constexpr D3D12_COMMAND_LIST_TYPE TraitTagToCommandListType<TraitTags::Bundle> = D3D12_COMMAND_LIST_TYPE_BUNDLE;

	export template<D3D12_COMMAND_LIST_TYPE Type>
	using CommandAllocator_t = IUnknownWrapper<ID3D12CommandAllocator, TraitTagToTypeMapper<CommandListTypeToTraitTag<Type>>::template type>;

	template<TraitTags Type>
	struct CommandAllocatorTraits
	{
		static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = TraitTagToCommandListType<Type>;

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

	template<TraitTags Tag>
		requires (Tag == TraitTags::Direct) ||
		(Tag == TraitTags::Compute) ||
		(Tag == TraitTags::Copy) ||
		(Tag == TraitTags::Bundle)
		struct TaggedTraits<ID3D12CommandAllocator, Tag> : CommandAllocatorTraits<Tag>
	{
		static constexpr TraitTags tag_value = Tag;
	};

	export template<D3D12_COMMAND_LIST_TYPE Type>
	using CommandAllocator_t = IUnknownWrapper<ID3D12CommandAllocator, GetTraitTagType<CommandListTypeToTraitTag<Type>>>;

	namespace Aliases
	{
		export using D3D12DirectCommandAllocator = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
		export using D3D12BundleCommandAllocator = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
		export using D3D12ComputeCommandAllocator = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
		export using D3D12CopyCommandAllocator = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COPY>;
	}
}