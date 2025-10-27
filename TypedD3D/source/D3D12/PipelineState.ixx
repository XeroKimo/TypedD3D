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
    struct PiplineStateTypeToTraitMap<D3D12_GRAPHICS_PIPELINE_STATE_DESC> { template<class Ty> using type = GraphicsTraits<Ty>; };

    template<>
    struct PiplineStateTypeToTraitMap<D3D12_COMPUTE_PIPELINE_STATE_DESC> { template<class Ty> using type = ComputeTraits<Ty>; };

    template<class DescriptionType, class UnknownType>
    using PiplineStateTypeToTrait = typename PiplineStateTypeToTraitMap<DescriptionType>::template type<UnknownType>;
    

    template<class DescriptionType>
    struct PipelineTraits
    {
        using value_type = ID3D12PipelineState;
        using pointer = ID3D12PipelineState*;
        using const_pointer = const ID3D12PipelineState*;
        using reference = ID3D12PipelineState&;
        using const_reference = const ID3D12PipelineState&;

        using inner_type = ID3D12PipelineState;
        template<class DerivedSelf>
        class Interface : public InterfaceBase<PiplineStateTypeToTrait<DescriptionType, DerivedSelf>>
        {
        private:
            using derived_self = DerivedSelf;

        public:
            Wrapper<ID3DBlob> GetCachedBlob()
            {
                return ForwardFunction<Wrapper<ID3DBlob>>(&value_type::GetCachedBlob, Self());
            }

        private:
            using InterfaceBase<PiplineStateTypeToTrait<DescriptionType, DerivedSelf>>::Self;
            using InterfaceBase<PiplineStateTypeToTrait<DescriptionType, DerivedSelf>>::ToDerived;
        };
    };
}

namespace TypedD3D
{
    template<>
    struct GraphicsTraits<ID3D12PipelineState> : D3D12::PipelineTraits<D3D12_GRAPHICS_PIPELINE_STATE_DESC>
    {
    };

    template<>
    struct ComputeTraits<ID3D12PipelineState> : D3D12::PipelineTraits<D3D12_COMPUTE_PIPELINE_STATE_DESC>
    {
    };
}