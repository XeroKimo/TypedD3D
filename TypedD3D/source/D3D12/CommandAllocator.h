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
        }
    }

    template<class DirectXClass, TypeTag Type>
        requires std::is_base_of_v<ID3D12CommandAllocator, DirectXClass> && Is_Command_List_Type<Type>
    class InterfaceWrapper<DirectXClass, Type> : public ComWrapper<DirectXClass>, private D3D12::CommandAllocator::Interface<InterfaceWrapper<DirectXClass, Type>, listType<Type>>
    {
    private:
        using Interface = D3D12::CommandAllocator::Interface<InterfaceWrapper<DirectXClass, Type>, listType<Type>>;
        friend Interface;

    public:
        static constexpr TypeTag tag_value = Type;
        using underlying_type = DirectXClass;

    public:
        using ComWrapper<DirectXClass>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}

namespace TypedD3D::D3D12
{
    template<D3D12_COMMAND_LIST_TYPE Type>
    using CommandAllocator_t = TypedD3D::Internal::D3D12::CommandAllocator_t<TypedD3D::Internal::tagValue<Type>>;

    namespace CommandAllocator
    {
        using Direct = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_DIRECT>;
        using Bundle = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
        using Compute = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
        using Copy = CommandAllocator_t<D3D12_COMMAND_LIST_TYPE_COPY>;
    }
}