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
        export template<D3D12_COMMAND_LIST_TYPE Type>
        using CommandAllocator_t = IUnknownWrapper<ID3D12CommandAllocator, Type>;

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
    template<>
    struct DirectMapper<ID3D12CommandAllocator>
    {
        using type = Internal::D3D12::CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
    };

    template<>
    struct ComputeMapper<ID3D12CommandAllocator>
    {
        using type = Internal::D3D12::CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
    };

    template<>
    struct CopyMapper<ID3D12CommandAllocator>
    {
        using type = Internal::D3D12::CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COPY>;
    };

    template<>
    struct BundleMapper<ID3D12CommandAllocator>
    {
        using type = Internal::D3D12::CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
    };

    template<D3D12_COMMAND_LIST_TYPE Type>
    struct Traits<ID3D12CommandAllocator, Type>
    {
        static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;

        using value_type = ID3D12CommandAllocator;
        using pointer = ID3D12CommandAllocator*;
        using const_pointer = const ID3D12CommandAllocator*;
        using reference = ID3D12CommandAllocator&;
        using const_reference = const ID3D12CommandAllocator&;

        template<class DerivedSelf>
        using Interface = typename Internal::D3D12::CommandAllocator::Traits::template Interface<DerivedSelf>;
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