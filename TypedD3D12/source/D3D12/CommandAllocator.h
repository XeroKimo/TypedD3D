#pragma once

#include "Meta.h"
#include "ComWrapper.h"
#include <d3d12.h>
#include <wrl/client.h>
#include <assert.h>

namespace TypedD3D::D3D12::CommandAllocator
{
    namespace Internal
    {
        using Microsoft::WRL::ComPtr;

        template<D3D12_COMMAND_LIST_TYPE Type>
        class CommandAllocator : public ComWrapper<ID3D12CommandAllocator>
        {
            using tag_type = Meta::command_list_type_tag<Type>;
            static constexpr D3D12_COMMAND_LIST_TYPE list_enum_type = tag_type::type;
        };
    }

    using Direct = Internal::CommandAllocator<D3D12_COMMAND_LIST_TYPE_DIRECT>;
    using Bundle = Internal::CommandAllocator<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
    using Compute = Internal::CommandAllocator<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
    using Copy = Internal::CommandAllocator<D3D12_COMMAND_LIST_TYPE_COPY>;
};