#pragma once
#include "../Wrappers.h"
#include "../Internal/ComWrapper.h"
#include "../Helpers/COMHelpers.h"
#include "DeviceChild.h"
#include <d3d11_4.h>

namespace TypedD3D::Internal
{
    namespace D3D11
    {
        namespace State
        {
            template<class Ty>
            struct ViewToStateDesc;

            template<>
            struct ViewToStateDesc<ID3D11DepthStencilState>
            {
                using type = D3D11_DEPTH_STENCIL_VIEW_DESC;
            };

            template<>
            struct ViewToStateDesc<ID3D11SamplerState>
            {
                using type = D3D11_UNORDERED_ACCESS_VIEW_DESC;
            };

            template<>
            struct ViewToStateDesc<ID3D11BlendState>
            {
                using type = D3D11_RENDER_TARGET_VIEW_DESC;
            };

            template<>
            struct ViewToStateDesc<ID3D11RasterizerState>
            {
                using type = D3D11_SHADER_RESOURCE_VIEW_DESC;
            };

            template<class WrapperTy, class ViewClassTag>
            class Interface;

            template<class Ty>
            concept StateObject = std::same_as<Ty, ID3D11DepthStencilState> ||
                std::same_as<Ty, ID3D11SamplerState> ||
                std::same_as<Ty, ID3D11BlendState> ||
                std::same_as<Ty, ID3D11RasterizerState>;

            template<class WrapperTy, StateObject StateTag>
            class Interface<WrapperTy, StateTag> : public DeviceChild::Interface<WrapperTy>
            {
            private:
                using type = StateTag;
                using wrapper_type = WrapperTy;

            public:
                typename ViewToStateDesc<type>::type GetDesc()
                {
                    typename ViewToStateDesc<type>::type description;
                    InternalGet().GetDesc(&description);
                    return description;
                }


            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                type& InternalGet() { return *ToDerived().Get(); }
            };
        }
    }

    template<D3D11::State::StateObject Ty>
    class InterfaceWrapper<Ty> : public ComWrapper<Ty>, private D3D11::State::Interface<InterfaceWrapper<Ty>, Ty>
    {
    private:
        using Interface = D3D11::State::Interface<InterfaceWrapper<Ty>, Ty>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = Ty;

    public:
        using ComWrapper<Ty>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}