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
            constexpr D3D12TraitTags ToEnum<D3D12_GRAPHICS_PIPELINE_STATE_DESC> = D3D12TraitTags::Graphics;

            template<>
            constexpr D3D12TraitTags ToEnum<D3D12_COMPUTE_PIPELINE_STATE_DESC> = D3D12TraitTags::Compute;
        }

        template<class Ty>
        using PipelineState_t = IUnknownWrapper<ID3D12PipelineState, TraitTagToTypeMapper<PipelineState::ToEnum<Ty>>::template type>;

    }

    template<D3D12TraitTags Type>
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
}

namespace TypedD3D
{
    template<D3D12TraitTags Tag>
    struct D3D12TaggedTraits<ID3D12PipelineState, Tag> : Internal::PipelineTraits<Tag>
    {
    };
}

namespace TypedD3D::D3D12
{
    export template<PipelineTypeTag Ty>
    using PipelineState_t = Internal::D3D12::PipelineState_t<Ty>;

    namespace PipelineState
    {
        export using Graphics = PipelineState_t<D3D12_GRAPHICS_PIPELINE_STATE_DESC>;
        export using Compute = PipelineState_t<D3D12_COMPUTE_PIPELINE_STATE_DESC>;
    }
}