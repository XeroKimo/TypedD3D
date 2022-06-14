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
        namespace Resource
        {
            template<class WrapperTy>
            class Interface : public DeviceChild::Interface<WrapperTy>
            {
            private:
                using resource_type = ID3D11Resource;
                using wrapper_type = WrapperTy;
            public:
                D3D11_RESOURCE_DIMENSION GetType()
                {
                    D3D11_RESOURCE_DIMENSION pResourceDimension;
                    InternalGet().GetType(&pResourceDimension);
                    return pResourceDimension;
                }

                void SetEvictionPriority(UINT EvictionPriority)
                {
                    InternalGet().SetEvictionPriority(EvictionPriority);
                }

                UINT GetEvictionPriority()
                {
                    return InternalGet().GetEvictionPriority();
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                resource_type& InternalGet() { return *ToDerived().Get(); }
            };
        }

        namespace Buffer
        {
            template<class WrapperTy>
            class Interface : public Resource::Interface<WrapperTy>
            {
            private:
                using resource_type = ID3D11Buffer;
                using wrapper_type = WrapperTy;
            public:
                D3D11_BUFFER_DESC GetDesc()
                {
                    D3D11_BUFFER_DESC pDesc;
                    InternalGet().GetDesc(&pDesc);
                    return pDesc;
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                resource_type& InternalGet() { return *ToDerived().Get(); }
            };
        }

        namespace Texture1D
        {
            template<class WrapperTy>
            class Interface : public Resource::Interface<WrapperTy>
            {
            private:
                using resource_type = ID3D11Texture1D;
                using wrapper_type = WrapperTy;
            public:
                D3D11_TEXTURE1D_DESC GetDesc()
                {
                    D3D11_TEXTURE1D_DESC pDesc;
                    InternalGet().GetDesc(&pDesc);
                    return pDesc;
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                resource_type& InternalGet() { return *ToDerived().Get(); }
            };
        }

        namespace Texture2D
        {
            template<class WrapperTy>
            class Interface : public Resource::Interface<WrapperTy>
            {
            private:
                using resource_type = ID3D11Texture2D;
                using wrapper_type = WrapperTy;
            public:
                D3D11_TEXTURE2D_DESC GetDesc()
                {
                    D3D11_TEXTURE2D_DESC pDesc;
                    InternalGet().GetDesc(&pDesc);
                    return pDesc;
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                resource_type& InternalGet() { return *ToDerived().Get(); }
            };
        }

        namespace Texture3D
        {
            template<class WrapperTy>
            class Interface : public Resource::Interface<WrapperTy>
            {
            private:
                using resource_type = ID3D11Texture3D;
                using wrapper_type = WrapperTy;
            public:
                D3D11_TEXTURE3D_DESC GetDesc()
                {
                    D3D11_TEXTURE3D_DESC pDesc;
                    InternalGet().GetDesc(&pDesc);
                    return pDesc;
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                resource_type& InternalGet() { return *ToDerived().Get(); }
            };
        }
    }

    template<std::same_as<ID3D11Resource> ResourceTy>
    class InterfaceWrapper<ResourceTy> : public ComWrapper<ResourceTy>, private D3D11::Resource::Interface<InterfaceWrapper<ResourceTy>>
    {
    private:
        using Interface = D3D11::Resource::Interface<InterfaceWrapper<ResourceTy>>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = ResourceTy;

    public:
        using ComWrapper<ResourceTy>::ComWrapper;

        template<std::derived_from<ID3D11Resource> OtherTy>
        InterfaceWrapper(InterfaceWrapper<OtherTy> other) : 
            ComWrapper<ResourceTy>(other.Get())
        {
        }

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };

    template<std::same_as<ID3D11Buffer> ResourceTy>
    class InterfaceWrapper<ResourceTy> : public ComWrapper<ResourceTy>, private D3D11::Buffer::Interface<InterfaceWrapper<ResourceTy>>
    {
    private:
        using Interface = D3D11::Buffer::Interface<InterfaceWrapper<ResourceTy>>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = ResourceTy;

    public:
        using ComWrapper<ResourceTy>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };

    template<std::same_as<ID3D11Texture1D> ResourceTy>
    class InterfaceWrapper<ResourceTy> : public ComWrapper<ResourceTy>, private D3D11::Texture1D::Interface<InterfaceWrapper<ResourceTy>>
    {
    private:
        using Interface = D3D11::Texture1D::Interface<InterfaceWrapper<ResourceTy>>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = ResourceTy;

    public:
        using ComWrapper<ResourceTy>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };

    template<std::same_as<ID3D11Texture2D> ResourceTy>
    class InterfaceWrapper<ResourceTy> : public ComWrapper<ResourceTy>, private D3D11::Texture2D::Interface<InterfaceWrapper<ResourceTy>>
    {
    private:
        using Interface = D3D11::Texture2D::Interface<InterfaceWrapper<ResourceTy>>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = ResourceTy;

    public:
        using ComWrapper<ResourceTy>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };

    template<std::same_as<ID3D11Texture3D> ResourceTy>
    class InterfaceWrapper<ResourceTy> : public ComWrapper<ResourceTy>, private D3D11::Texture3D::Interface<InterfaceWrapper<ResourceTy>>
    {
    private:
        using Interface = D3D11::Texture3D::Interface<InterfaceWrapper<ResourceTy>>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = ResourceTy;

    public:
        using ComWrapper<ResourceTy>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}