module;

#include <d3d12.h>
#include <wrl/client.h>
#include <assert.h>

export module TypedD3D12:CommandAllocator;
import TypedD3D.Shared;
import :Wrappers;

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        export template<D3D12_COMMAND_LIST_TYPE Tag>
        constexpr D3D12TraitTags CommandListTypeToTraitTag;

        template<>
        constexpr D3D12TraitTags CommandListTypeToTraitTag<D3D12_COMMAND_LIST_TYPE_DIRECT> = D3D12TraitTags::Direct;

        template<>
        constexpr D3D12TraitTags CommandListTypeToTraitTag<D3D12_COMMAND_LIST_TYPE_COMPUTE> = D3D12TraitTags::Compute;

        template<>
        constexpr D3D12TraitTags CommandListTypeToTraitTag<D3D12_COMMAND_LIST_TYPE_COPY> = D3D12TraitTags::Copy;

        template<>
        constexpr D3D12TraitTags CommandListTypeToTraitTag<D3D12_COMMAND_LIST_TYPE_BUNDLE> = D3D12TraitTags::Bundle;

        export template<D3D12TraitTags Tag>
        constexpr D3D12_COMMAND_LIST_TYPE TraitTagToCommandListType;

        template<>
        constexpr D3D12_COMMAND_LIST_TYPE TraitTagToCommandListType<D3D12TraitTags::Direct> = D3D12_COMMAND_LIST_TYPE_DIRECT;

        template<>
        constexpr D3D12_COMMAND_LIST_TYPE TraitTagToCommandListType<D3D12TraitTags::Compute> = D3D12_COMMAND_LIST_TYPE_COMPUTE;

        template<>
        constexpr D3D12_COMMAND_LIST_TYPE TraitTagToCommandListType< D3D12TraitTags::Copy> = D3D12_COMMAND_LIST_TYPE_COPY;

        template<>
        constexpr D3D12_COMMAND_LIST_TYPE TraitTagToCommandListType<D3D12TraitTags::Bundle> = D3D12_COMMAND_LIST_TYPE_BUNDLE;

        export template<D3D12_COMMAND_LIST_TYPE Type>
        using CommandAllocator_t = IUnknownWrapper<ID3D12CommandAllocator, TraitTagToTypeMapper<CommandListTypeToTraitTag<Type>>::template type>;

        namespace CommandAllocator
        {
            struct Traits
            {
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
        }
    }
}

namespace TypedD3D
{
    template<D3D12TraitTags Type>
    struct CommandAllocatorTraits
    {
        static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Internal::D3D12::TraitTagToCommandListType<Type>;

        using value_type = ID3D12CommandAllocator;
        using pointer = ID3D12CommandAllocator*;
        using const_pointer = const ID3D12CommandAllocator*;
        using reference = ID3D12CommandAllocator&;
        using const_reference = const ID3D12CommandAllocator&;

        template<class DerivedSelf>
        using Interface = typename Internal::D3D12::CommandAllocator::Traits::template Interface<DerivedSelf>;
    };

    template<D3D12TraitTags Tag>
        requires (Tag == D3D12TraitTags::Direct) ||
            (Tag == D3D12TraitTags::Compute) ||
            (Tag == D3D12TraitTags::Copy) ||
            (Tag == D3D12TraitTags::Bundle)
    struct D3D12TaggedTraits<ID3D12CommandAllocator, Tag> : CommandAllocatorTraits<Tag>
    {
        static constexpr D3D12TraitTags tag_value = Tag;
    };
}

namespace TypedD3D::D3D12
{
    export template<D3D12_COMMAND_LIST_TYPE Type>
    using CommandAllocator_t = TypedD3D::Internal::D3D12::CommandAllocator_t<Type>;

    namespace CommandAllocator
    {
        export using Direct = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
        export using Bundle = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
        export using Compute = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
        export using Copy = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COPY>;
    }
}