#pragma once
#include "source/Wrappers.h"
#include "source/Internal/IUnknownWrapper.h"
#include "source/Helpers/COMHelpers.h"
#include "DeviceChild.h"
#include "Resources.h"
#include <d3d11_4.h>

namespace TypedD3D::Internal
{
    namespace D3D11
    {
        namespace View
        {
            template<class Ty>
            struct ViewToResourceDesc;

            template<>
            struct ViewToResourceDesc<ID3D11DepthStencilView>
            {
                using type = D3D11_DEPTH_STENCIL_VIEW_DESC;
            };

            template<>
            struct ViewToResourceDesc<ID3D11UnorderedAccessView>
            {
                using type = D3D11_UNORDERED_ACCESS_VIEW_DESC;
            };

            template<>
            struct ViewToResourceDesc<ID3D11RenderTargetView>
            {
                using type = D3D11_RENDER_TARGET_VIEW_DESC;
            };

            template<>
            struct ViewToResourceDesc<ID3D11ShaderResourceView>
            {
                using type = D3D11_SHADER_RESOURCE_VIEW_DESC;
            };
        }
    }

    template<>
    struct Traits<ID3D11View>
    {
        using value_type = ID3D11View;
        using pointer = ID3D11View*;
        using const_pointer = const ID3D11View*;
        using reference = ID3D11View&;
        using const_reference = const ID3D11View&;

        template<class DerivedSelf>
        class Interface : public D3D11::DeviceChild::Interface<DerivedSelf>
        {
        private:
            using derived_self = DerivedSelf;

        public:
            Wrapper<ID3D11Resource> GetResource()
            {
                Microsoft::WRL::ComPtr<ID3D11Resource> resource;
                Get().GetResource(&resource);
                return Wrapper<ID3D11Resource>(resource);
            }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };

    template<class Ty>
        requires std::derived_from<Ty, ID3D11View> && (!std::same_as<ID3D11View, Ty>)
    struct Traits<Ty>
    {
        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using const_reference = const Ty&;

        template<class DerivedSelf>
        class Interface : public Traits<ID3D11View>::Interface<DerivedSelf>
        {
        private:
            using derived_self = DerivedSelf;

        public:
            typename D3D11::View::ViewToResourceDesc<value_type>::type GetDesc()
            {
                typename D3D11::View::ViewToResourceDesc<value_type>::type description;
                Get().GetDesc(&description);
                return description;
            }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };

}