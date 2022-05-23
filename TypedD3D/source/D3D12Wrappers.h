#pragma once
#include "Wrappers.h"
#include <d3d12.h>

namespace TypedD3D
{
    namespace Internal
    {
        enum class TypeTag
        {
            Direct,
            Compute,
            Copy,
            Bundle,
            CBV_SRV_UAV,
            Sampler,
            RTV,
            DSV,
            Graphics,
        };

        template<TypeTag value>
        constexpr D3D12_COMMAND_LIST_TYPE listType;

        template<>
        constexpr D3D12_COMMAND_LIST_TYPE listType<TypeTag::Direct> = D3D12_COMMAND_LIST_TYPE_DIRECT;

        template<>
        constexpr D3D12_COMMAND_LIST_TYPE listType<TypeTag::Compute> = D3D12_COMMAND_LIST_TYPE_COMPUTE;

        template<>
        constexpr D3D12_COMMAND_LIST_TYPE listType<TypeTag::Bundle> = D3D12_COMMAND_LIST_TYPE_BUNDLE;

        template<>
        constexpr D3D12_COMMAND_LIST_TYPE listType<TypeTag::Copy> = D3D12_COMMAND_LIST_TYPE_COPY;

        template<TypeTag value>
        constexpr D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType;

        template<>
        constexpr D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType<TypeTag::CBV_SRV_UAV> = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        template<>
        constexpr D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType<TypeTag::Sampler> = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;

        template<>
        constexpr D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType<TypeTag::RTV> = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

        template<>
        constexpr D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType<TypeTag::DSV> = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

        template<TypeTag Type>
        struct PipelineStateMapper;

        template<>
        struct PipelineStateMapper<TypeTag::Compute>
        {
            using type = D3D12_COMPUTE_PIPELINE_STATE_DESC;
        };

        template<>
        struct PipelineStateMapper<TypeTag::Graphics>
        {
            using type = D3D12_GRAPHICS_PIPELINE_STATE_DESC;
        };
    }

    template<class IUnknownTy>
    using Direct = typename Internal::InterfaceMapper<IUnknownTy, Internal::TypeTag::Direct>::type;

    template<class IUnknownTy>
    using Compute = typename Internal::InterfaceMapper<IUnknownTy, Internal::TypeTag::Compute>::type;

    template<class IUnknownTy>
    using Copy = typename Internal::InterfaceMapper<IUnknownTy, Internal::TypeTag::Copy>::type;

    template<class IUnknownTy>
    using Bundle = typename Internal::InterfaceMapper<IUnknownTy, Internal::TypeTag::Bundle>::type;

    template<class IUnknownTy>
    using CBV_SRV_UAV = typename Internal::InterfaceMapper<IUnknownTy, Internal::TypeTag::CBV_SRV_UAV>::type;

    template<class IUnknownTy>
    using Sampler = typename Internal::InterfaceMapper<IUnknownTy, Internal::TypeTag::Sampler>::type;

    template<class IUnknownTy>
    using RTV = typename Internal::InterfaceMapper<IUnknownTy, Internal::TypeTag::RTV>::type;

    template<class IUnknownTy>
    using DSV = typename Internal::InterfaceMapper<IUnknownTy, Internal::TypeTag::DSV>::type;

    template<class IUnknownTy>
    using Graphics = typename Internal::InterfaceMapper<IUnknownTy, Internal::TypeTag::Graphics>::type;
}