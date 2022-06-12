#pragma once
#include "../Wrappers.h"
#include "../Internal/ComWrapper.h"
#include "../Helpers/COMHelpers.h"
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

            template<class WrapperTy, class ViewClassTag>
            class Interface;

            template<class WrapperTy>
            class Interface<WrapperTy, ID3D11View> : public DeviceChild::Interface<WrapperTy>
            {
            private:
                using type = ID3D11View;
                using wrapper_type = WrapperTy;

            public:
                Wrapper<ID3D11Resource> GetResource()
                {
                    Microsoft::WRL::ComPtr<ID3D11Resource> resource;
                    InternalGet().GetResource(&resource);
                    return Wrapper<ID3D11Resource>(resource);
                }


            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                type& InternalGet() { return *ToDerived().Get(); }
            };

            template<class WrapperTy, class ViewClassTag>
                requires std::derived_from<ViewClassTag, ID3D11View> && (!std::same_as<ID3D11View, ViewClassTag>)
            class Interface<WrapperTy, ViewClassTag> : public Interface<WrapperTy, ID3D11View>
            {
            private:
                using type = ViewClassTag;
                using wrapper_type = WrapperTy;

            public:
                typename ViewToResourceDesc<type>::type GetDesc()
                {
                    typename ViewToResourceDesc<type>::type description;
                    InternalGet().GetDesc(&description);
                    return description;
                }


            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                type& InternalGet() { return *ToDerived().Get(); }
            };
        }
    }

    template<std::derived_from<ID3D11View> Ty>
    class InterfaceWrapper<Ty> : public ComWrapper<Ty>, private D3D11::View::Interface<InterfaceWrapper<Ty>, Ty>
    {
    private:
        using Interface = D3D11::View::Interface<InterfaceWrapper<Ty>>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = Ty;

    public:
        using ComWrapper<Ty>::ComWrapper;

        template<std::derived_from<ID3D11Resource> OtherTy>
        InterfaceWrapper(InterfaceWrapper<OtherTy> other) :
            ComWrapper<Ty>(other.Get())
        {
        }

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}