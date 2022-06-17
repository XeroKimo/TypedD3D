#pragma once
#include "source/Internal/IUnknownWrapper.h"
#include "source/Helpers/COMHelpers.h"
#include <d3d12.h>

namespace TypedD3D::D3D12
{
    template<class Ty>
    concept PipelineTypeTag = std::same_as<Ty, D3D12_GRAPHICS_PIPELINE_STATE_DESC> || std::same_as<Ty, D3D12_COMPUTE_PIPELINE_STATE_DESC>;
}

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        namespace PipelineState
        {
            enum class TypeEnum
            {
                Graphics,
                Compute
            };

            template<class>
            constexpr TypeEnum ToEnum;

            template<>
            constexpr TypeEnum ToEnum<D3D12_GRAPHICS_PIPELINE_STATE_DESC> = TypeEnum::Graphics;

            template<>
            constexpr TypeEnum ToEnum<D3D12_COMPUTE_PIPELINE_STATE_DESC> = TypeEnum::Compute;
        }

        template<class Ty>
        using PipelineState_t = IUnknownWrapper<ID3D12PipelineState, PipelineState::ToEnum<Ty>>;

    }

    template<D3D12::PipelineState::TypeEnum Type>
    struct Traits<ID3D12PipelineState, Type>
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
    using PipelineState_t = Internal::D3D12::PipelineState_t<Ty>;

    namespace PipelineState
    {
        using Graphics = PipelineState_t<D3D12_GRAPHICS_PIPELINE_STATE_DESC>;
        using Compute = PipelineState_t<D3D12_COMPUTE_PIPELINE_STATE_DESC>;
    }
}