#pragma once

#include "../Internal/ComWrapper.h"
#include "../D3D12Wrappers.h"
#include <d3d12.h>
#include <wrl/client.h>
#include <assert.h>

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        template<TypeTag Type>
        using CommandAllocator_t = InterfaceWrapper<ID3D12CommandAllocator, Type>;

        namespace CommandAllocator
        {
            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
            class Interface
            {
            public:
                static constexpr D3D12_COMMAND_LIST_TYPE value = Type;

            public:

                HRESULT Reset() { return InternalGet().Reset(); }

            private:
                ID3D12CommandAllocator& InternalGet() { return *static_cast<WrapperTy&>(*this).Get(); }
            };

            template<D3D12_COMMAND_LIST_TYPE Type>
            struct Traits
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
                    HRESULT Reset() { Get().Reset(); }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };
        }

    }
}

namespace TypedD3D::D3D12
{
    template<D3D12_COMMAND_LIST_TYPE Type>
    class CommandAllocator_t : public Internal::InterfaceWrapper<ID3D12CommandAllocator, Internal::D3D12::CommandAllocator::Traits<Type>::Interface>
    {
    private:
        using Trait = Internal::D3D12::CommandAllocator::Traits<Type>;

    public:
        static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;
    };

    namespace CommandAllocator
    {
        using Direct = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
        using Bundle = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
        using Compute = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
        using Copy = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COPY>;
    }
}