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
            RenderPass
        };

        template<TypeTag Type>
        concept Is_Command_List_Type = (Type == TypeTag::Direct || Type == TypeTag::Compute || Type == TypeTag::Copy || Type == TypeTag::Bundle);

        template<TypeTag Type>
        concept Is_Descriptor_Heap_Type = (Type == TypeTag::CBV_SRV_UAV || Type == TypeTag::Sampler || Type == TypeTag::RTV || Type == TypeTag::DSV);
                
        template<TypeTag Type>
        concept Is_Pipeline_Type = (Type == TypeTag::Graphics || Type == TypeTag::Compute);

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

        template<auto Type>
        constexpr TypeTag tagValue;

        template<>
        constexpr TypeTag tagValue<D3D12_COMMAND_LIST_TYPE_DIRECT> = TypeTag::Direct;

        template<>
        constexpr TypeTag tagValue<D3D12_COMMAND_LIST_TYPE_COMPUTE> = TypeTag::Compute;

        template<>
        constexpr TypeTag tagValue<D3D12_COMMAND_LIST_TYPE_BUNDLE> = TypeTag::Bundle;

        template<>
        constexpr TypeTag tagValue<D3D12_COMMAND_LIST_TYPE_COPY> = TypeTag::Copy;

        template<>
        constexpr TypeTag tagValue<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV> = TypeTag::CBV_SRV_UAV;

        template<>
        constexpr TypeTag tagValue<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER> = TypeTag::Sampler;

        template<>
        constexpr TypeTag tagValue<D3D12_DESCRIPTOR_HEAP_TYPE_RTV> = TypeTag::RTV;

        template<>
        constexpr TypeTag tagValue<D3D12_DESCRIPTOR_HEAP_TYPE_DSV> = TypeTag::DSV;

        template<class Ty>
        struct TypeTagMapper;

        template<>
        struct TypeTagMapper<D3D12_GRAPHICS_PIPELINE_STATE_DESC>
        {
            static constexpr TypeTag value = TypeTag::Graphics;
        };

        template<>
        struct TypeTagMapper<D3D12_COMPUTE_PIPELINE_STATE_DESC>
        {
            static constexpr TypeTag value = TypeTag::Compute;
        };

    }

    template<class IUnknownTy>
    using Direct = Internal::InterfaceWrapper<IUnknownTy, Internal::TypeTag::Direct>;

    template<class IUnknownTy>
    using Compute = Internal::InterfaceWrapper<IUnknownTy, Internal::TypeTag::Compute>;

    template<class IUnknownTy>
    using Copy = Internal::InterfaceWrapper<IUnknownTy, Internal::TypeTag::Copy>;

    template<class IUnknownTy>
    using Bundle = Internal::InterfaceWrapper<IUnknownTy, Internal::TypeTag::Bundle>;

    template<class IUnknownTy>
    using CBV_SRV_UAV = Internal::InterfaceWrapper<IUnknownTy, Internal::TypeTag::CBV_SRV_UAV>;

    template<class IUnknownTy>
    using Sampler = Internal::InterfaceWrapper<IUnknownTy, Internal::TypeTag::Sampler>;

    template<class IUnknownTy>
    using RTV = Internal::InterfaceWrapper<IUnknownTy, Internal::TypeTag::RTV>;

    template<class IUnknownTy>
    using DSV = Internal::InterfaceWrapper<IUnknownTy, Internal::TypeTag::DSV>;

    template<class IUnknownTy>
    using Graphics = Internal::InterfaceWrapper<IUnknownTy, Internal::TypeTag::Graphics>;

    template<class IUnknownTy>
    using RenderPass = Internal::InterfaceWrapper<IUnknownTy, Internal::TypeTag::RenderPass>;
}