#pragma once
#include <d3d12.h>

namespace TypedD3D::D3D12::Meta
{
    template<D3D12_COMMAND_LIST_TYPE Type>
    struct command_list_type_tag
    {
        static constexpr D3D12_COMMAND_LIST_TYPE type = Type;
    };

    using direct_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_DIRECT>;
    using bundle_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
    using compute_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
    using copy_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_COPY>;
    using video_decode_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE>;
    using video_encode_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE>;
    using video_process_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS>;



}