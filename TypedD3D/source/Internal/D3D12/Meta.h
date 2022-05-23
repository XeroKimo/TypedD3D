#pragma once
#include <d3d12.h>
#include <type_traits>

namespace TypedD3D::Internal::D3D12::Meta
{
    template<D3D12_COMMAND_LIST_TYPE Type>
    struct command_list_type_tag : std::integral_constant<D3D12_COMMAND_LIST_TYPE, Type>
    {
    };

    using direct_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_DIRECT>;
    using bundle_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_BUNDLE>;
    using compute_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_COMPUTE>;
    using copy_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_COPY>;
    using video_decode_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE>;
    using video_encode_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE>;
    using video_process_command_list_type_tag = command_list_type_tag<D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS>;


    template<D3D12_FEATURE Feature>
    struct DeviceFeatureToType;

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS>
    {
        using type = D3D12_FEATURE_DATA_D3D12_OPTIONS;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_ARCHITECTURE>
    {
        using type = D3D12_FEATURE_DATA_ARCHITECTURE;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_FEATURE_LEVELS>
    {
        using type = D3D12_FEATURE_DATA_FEATURE_LEVELS;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_FORMAT_SUPPORT>
    {
        using type = D3D12_FEATURE_DATA_FORMAT_INFO;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT>
    {
        using type = D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_SHADER_MODEL>
    {
        using type = D3D12_FEATURE_DATA_SHADER_MODEL;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS1>
    {
        using type = D3D12_FEATURE_DATA_D3D12_OPTIONS1;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_SUPPORT>
    {
        using type = D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_SUPPORT;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_ROOT_SIGNATURE>
    {
        using type = D3D12_FEATURE_DATA_ROOT_SIGNATURE;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_ARCHITECTURE1>
    {
        using type = D3D12_FEATURE_DATA_ARCHITECTURE1;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS2>
    {
        using type = D3D12_FEATURE_DATA_D3D12_OPTIONS2;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_SHADER_CACHE>
    {
        using type = D3D12_FEATURE_DATA_SHADER_CACHE;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_COMMAND_QUEUE_PRIORITY>
    {
        using type = D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS3>
    {
        using type = D3D12_FEATURE_DATA_D3D12_OPTIONS3;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_EXISTING_HEAPS>
    {
        using type = D3D12_FEATURE_DATA_EXISTING_HEAPS;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS4>
    {
        using type = D3D12_FEATURE_DATA_D3D12_OPTIONS4;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_SERIALIZATION>
    {
        using type = D3D12_FEATURE_DATA_SERIALIZATION;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_CROSS_NODE>
    {
        using type = D3D12_FEATURE_DATA_CROSS_NODE;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS5>
    {
        using type = D3D12_FEATURE_DATA_D3D12_OPTIONS5;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS6>
    {
        using type = D3D12_FEATURE_DATA_D3D12_OPTIONS6;
    };

    //Requires Windows 11
    //template<>
    //struct DeviceFeatureToType<D3D12_FEATURE_DISPLAYABLE>
    //{
    //    using type = D3D12_FEATURE_DATA_DISPLAYABLE;
    //};

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_QUERY_META_COMMAND>
    {
        using type = D3D12_FEATURE_DATA_QUERY_META_COMMAND;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS7>
    {
        using type = D3D12_FEATURE_DATA_D3D12_OPTIONS7;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_TYPE_COUNT>
    {
        using type = D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_TYPE_COUNT;
    };

    template<>
    struct DeviceFeatureToType<D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_TYPES>
    {
        using type = D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_TYPES;
    };

    //Requires Windows 11
    //template<>
    //struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS8>
    //{
    //    using type = D3D12_FEATURE_DATA_D3D12_OPTIONS8;
    //};

    //Requires Windows 11
    //template<>
    //struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS9>
    //{
    //    using type = D3D12_FEATURE_DATA_D3D12_OPTIONS9;
    //};

    //Requires Windows 11
    //template<>
    //struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS10>
    //{
    //    using type = D3D12_FEATURE_DATA_D3D12_OPTIONS10;
    //};

    //Requires Windows 11
    //template<>
    //struct DeviceFeatureToType<D3D12_FEATURE_D3D12_OPTIONS11>
    //{
    //    using type = D3D12_FEATURE_DATA_D3D12_OPTIONS11;
    //};
}