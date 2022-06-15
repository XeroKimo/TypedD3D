#pragma once
#include "../Internal/ComWrapper.h"
#include "../Helpers/COMHelpers.h"
#include <d3d12.h>

namespace TypedD3D::D3D12
{
    template<class Ty>
    concept PipelineTypeTag = std::same_as<Ty, D3D12_GRAPHICS_PIPELINE_STATE_DESC> || std::same_as<Ty, D3D12_COMPUTE_PIPELINE_STATE_DESC>;

    template<PipelineTypeTag Ty>
    class PipelineState_t;
}

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        template<class Ty>
        using PipelineState_t = TypedD3D::D3D12::PipelineState_t<Ty>;

        namespace PipelineState
        {
            struct Traits
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
                        return Helpers::COM::UnknownObjectForwardFunction<ID3DBlob>(&value_type::GetCachedBlob, Get()).value();
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };
        }
    }

    template<>
    struct GraphicsMapper<ID3D12PipelineState>
    {
        using type = D3D12::PipelineState_t<D3D12_GRAPHICS_PIPELINE_STATE_DESC>;
    };

    template<>
    struct ComputeMapper<ID3D12PipelineState>
    {
        using type = D3D12::PipelineState_t<D3D12_COMPUTE_PIPELINE_STATE_DESC>;
    };
}

namespace TypedD3D::D3D12
{
    template<PipelineTypeTag Ty>
    class PipelineState_t : public Internal::InterfaceWrapper<ID3D12PipelineState, typename Internal::D3D12::PipelineState::Traits::Interface>
    {

    };

    namespace PipelineState
    {
        using Graphics = PipelineState_t<D3D12_GRAPHICS_PIPELINE_STATE_DESC>;
        using Compute = PipelineState_t<D3D12_COMPUTE_PIPELINE_STATE_DESC>;
    }
}