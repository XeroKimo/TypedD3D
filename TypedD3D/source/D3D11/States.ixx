module;

#include <d3d11_4.h>
#include <concepts>

export module TypedD3D11:States;
import :DeviceChild;
import TypedD3D.Shared;

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

            template<class Ty>
            concept StateObject = std::same_as<Ty, ID3D11DepthStencilState> ||
                std::same_as<Ty, ID3D11SamplerState> ||
                std::same_as<Ty, ID3D11BlendState> ||
                std::same_as<Ty, ID3D11RasterizerState>;
        }
    }
}

namespace TypedD3D
{
    template<Internal::D3D11::State::StateObject Ty>
    struct Traits<Ty>
    {
        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using const_reference = const Ty&;

        template<class DerivedSelf>
        class Interface : public Internal::D3D11::DeviceChild::Interface<DerivedSelf>
        {
        private:
            using derived_self = DerivedSelf;

        public:
            typename Internal::D3D11::State::ViewToStateDesc<value_type>::type GetDesc()
            {
                typename Internal::D3D11::State::ViewToStateDesc<value_type>::type description;
                Get().GetDesc(&description);
                return description;
            }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };
}