#pragma once

#include "source/Internal/IUnknownWrapper.h"
#include "source/D3D12Wrappers.h"
#include <d3d12.h>
#include <wrl/client.h>
#include <assert.h>

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        template<D3D12_COMMAND_LIST_TYPE Type>
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
        using Interface = typename D3D12::CommandAllocator::Traits::template Interface<DerivedSelf>;
    };

    template<>
    struct DirectMapper<ID3D12CommandAllocator>
    {
        using type = D3D12::CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
    };

    template<>
    struct ComputeMapper<ID3D12CommandAllocator>
    {
        using type = D3D12::CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
    };

    template<>
    struct CopyMapper<ID3D12CommandAllocator>
    {
        using type = D3D12::CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COPY>;
    };

    template<>
    struct BundleMapper<ID3D12CommandAllocator>
    {
        using type = D3D12::CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
    };
}

namespace TypedD3D::D3D12
{
    template<D3D12_COMMAND_LIST_TYPE Type>
    using CommandAllocator_t = TypedD3D::Internal::D3D12::CommandAllocator_t<Type>;

    namespace CommandAllocator
    {
        using Direct = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
        using Bundle = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
        using Compute = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
        using Copy = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COPY>;
    }
}