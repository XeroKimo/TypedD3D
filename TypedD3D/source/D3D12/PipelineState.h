#pragma once
#include "ComWrapper.h"
#include "../Helpers/COMHelpers.h"
#include <d3d12.h>

namespace TypedD3D::Internal
{
    namespace PipelineState
    {
        template<class WrapperTy>
        class Interface
        {
        public:
            Microsoft::WRL::ComPtr<ID3DBlob> GetCachedBlob()
            {
                return Helpers::COM::UnknownObjectForwardFunction<ID3DBlob>(&ID3D12PipelineState::GetCachedBlob, InternalGet()).GetValue();
            }

        private:
            ID3D12PipelineState& InternalGet() { return *static_cast<WrapperTy&>(*this).Get(); }
        };
    }

    template<class DirectXClass, TypeTag Type>
        requires std::is_base_of_v<ID3D12PipelineState, DirectXClass> && Is_Pipeline_Type<Type>
    class InterfaceWrapper<DirectXClass, Type> : public ComWrapper<DirectXClass>, private PipelineState::Interface<InterfaceWrapper<DirectXClass, Type>>
    {
    private:
        using Interface = PipelineState::Interface<InterfaceWrapper<DirectXClass, Type>>;
        friend Interface;

    public:
        static constexpr TypeTag tag_value = Type;
        using underlying_type = DirectXClass;
        using ComWrapper<DirectXClass>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };

    template<TypeTag Type>
    using PipelineState_t = InterfaceWrapper<ID3D12PipelineState, Type>;

    template<class IUnknownTy, TypeTag Type>
        requires std::is_base_of_v<ID3D12PipelineState, IUnknownTy>
    struct InterfaceMapper<IUnknownTy, Type>
    {
        using type = PipelineState_t<Type>;
    };
}

namespace TypedD3D::D3D12
{
    template<class PipelineStateDesc>
    using PipelineState_t = TypedD3D::Internal::PipelineState_t<TypedD3D::Internal::TypeTagMapper<PipelineStateDesc>::value>;

    namespace PipelineState
    {
        using Graphics = PipelineState_t<D3D12_GRAPHICS_PIPELINE_STATE_DESC>;
        using Compute = PipelineState_t<D3D12_COMPUTE_PIPELINE_STATE_DESC>;
    }
}