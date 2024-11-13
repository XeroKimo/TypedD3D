module;

#include <d3d12.h>
#include <concepts>
#include <wrl/client.h>

export module TypedD3D12:PipelineState;

import :Wrappers;
import TypedD3D.Shared;

namespace TypedD3D::D3D12
{
    template<class Ty>
    concept PipelineTypeTag = std::same_as<Ty, D3D12_GRAPHICS_PIPELINE_STATE_DESC> || std::same_as<Ty, D3D12_COMPUTE_PIPELINE_STATE_DESC>;

    template<class Ty>
    struct PiplineStateTypeToTrait;

    template<>
    struct PiplineStateTypeToTrait<D3D12_GRAPHICS_PIPELINE_STATE_DESC> { template<class Ty> using type = GraphicsTraits<Ty>; };

    template<>
    struct PiplineStateTypeToTrait<D3D12_COMPUTE_PIPELINE_STATE_DESC> { template<class Ty> using type = ComputeTraits<Ty>; };

    template<class Ty>
    using PipelineState_t = IUnknownWrapper<ID3D12PipelineState, PiplineStateTypeToTrait<Ty>::template type>;

    //template<TraitTags Type>
    struct PipelineTraits
    {
        using value_type = ID3D12PipelineState;
        using pointer = ID3D12PipelineState*;
        using const_pointer = const ID3D12PipelineState*;
        using reference = ID3D12PipelineState&;
        using const_reference = const ID3D12PipelineState&;

        template<class DerivedSelf>
        class Interface
        {
        private:
            using derived_self = DerivedSelf;

        public:
            Microsoft::WRL::ComPtr<ID3DBlob> GetCachedBlob()
            {
                return UnknownObjectForwardFunction<ID3DBlob>(&value_type::GetCachedBlob, Get()).value();
            }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };

    template<>
    struct GraphicsTraits<ID3D12PipelineState> : PipelineTraits
    {
    };

    template<>
    struct ComputeTraits<ID3D12PipelineState> : PipelineTraits
    {
    };

    namespace Aliases
    {
        export using D3D12GraphicsPipelineState = PipelineState_t<D3D12_GRAPHICS_PIPELINE_STATE_DESC>;
        export using D3D12ComputePipelineState = PipelineState_t<D3D12_COMPUTE_PIPELINE_STATE_DESC>;
    }
}