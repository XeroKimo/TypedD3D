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

        template<class Tag>
        class CommandAllocator : public ComWrapper<ID3D12CommandAllocator>
        {
            static constexpr D3D12_COMMAND_LIST_TYPE list_enum_type = Tag::type;
        };
    }

    using Direct = Internal::CommandAllocator<Meta::direct_command_list_type_tag>;
    using Bundle = Internal::CommandAllocator<Meta::bundle_command_list_type_tag>;
    using Compute = Internal::CommandAllocator<Meta::compute_command_list_type_tag>;
    using Copy = Internal::CommandAllocator<Meta::copy_command_list_type_tag>;
};