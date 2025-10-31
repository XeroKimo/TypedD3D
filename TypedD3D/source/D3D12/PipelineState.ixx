module;

#include <d3d12.h>
#include <concepts>


export module TypedD3D12:PipelineState;

import :Wrappers;
import TypedD3D.Shared;

namespace TypedD3D::D3D12
{
    template<class Ty>
    struct PiplineStateTypeToTraitMap;

    template<>
    struct PiplineStateTypeToTraitMap<D3D12_GRAPHICS_PIPELINE_STATE_DESC> { template<class Ty> using type = GraphicsTag<Ty>; };

    template<>
    struct PiplineStateTypeToTraitMap<D3D12_COMPUTE_PIPELINE_STATE_DESC> { template<class Ty> using type = ComputeTag<Ty>; };

    template<class DescriptionType, class UnknownType>
    using PiplineStateTypeToTrait = typename PiplineStateTypeToTraitMap<DescriptionType>::template type<UnknownType>;
    
    template<template<class> class Tag>
    concept PipelineStateEnabledTag = SameTagAs<Tag, GraphicsTag>
        || SameTagAs<Tag, ComputeTag>
        || SameTagAs<Tag, Untagged>;
}

namespace TypedD3D
{
    template<template<class> class Tag>
        requires D3D12::PipelineStateEnabledTag<Tag>
    struct Trait<Tag<ID3D12PipelineState>>
    {
        using inner_type = ID3D12PipelineState;

        using inner_tag = ID3D12PipelineState;

        template<class NewInner>
        using ReplaceInnerType = Tag<NewInner>;

        template<class NewInner>
        using trait_template = Tag<NewInner>;

        template<class Derived>
        class Interface : public InterfaceBase<Tag<Derived>>
        {
        public:
            Wrapper<ID3DBlob> GetCachedBlob()
            {
                return ForwardFunction<Wrapper<ID3DBlob>>(&inner_type::GetCachedBlob, Self());
            }

        private:
            using InterfaceBase<Tag<Derived>>::Self;
            using InterfaceBase<Tag<Derived>>::ToDerived;
        };
    };
}