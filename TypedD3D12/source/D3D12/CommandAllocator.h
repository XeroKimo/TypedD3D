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
            static constexpr D3D12_COMMAND_LIST_TYPE value = Type;
        };
    }

    template<D3D12_COMMAND_LIST_TYPE Type>
    using CommandAllocator_t = Internal::CommandAllocator<Type>;

    using Direct = Internal::CommandAllocator<D3D12_COMMAND_LIST_TYPE_DIRECT>;
    using Bundle = Internal::CommandAllocator<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
    using Compute = Internal::CommandAllocator<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
    using Copy = Internal::CommandAllocator<D3D12_COMMAND_LIST_TYPE_COPY>;
};