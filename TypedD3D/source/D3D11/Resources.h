#pragma once
#include "source/Wrappers.h"
#include "source/Internal/IUnknownWrapper.h"
#include "source/Helpers/COMHelpers.h"
#include "DeviceChild.h"
#include <d3d11_4.h>

namespace TypedD3D::Internal
{
    namespace D3D11
    {
        namespace Resource
        {
        }

        namespace Buffer
        {
        }

        namespace Texture1D
        {
        }

        namespace Texture2D
        {
        }

        namespace Texture3D
        {
        }
    }

    template<>
    struct Traits<ID3D11Resource>
    {
        using value_type = ID3D11Resource;
        using pointer = ID3D11Resource*;
        using const_pointer = const ID3D11Resource*;
        using reference = ID3D11Resource&;
        using const_reference = const ID3D11Resource&;

        template<class DerivedSelf>
        class Interface : public D3D11::DeviceChild::Interface<DerivedSelf>
        {
        private:
            using derived_self = DerivedSelf;

        public:
            D3D11_RESOURCE_DIMENSION GetType()
            {
                D3D11_RESOURCE_DIMENSION pResourceDimension;
                Get().GetType(&pResourceDimension);
                return pResourceDimension;
            }

            void SetEvictionPriority(UINT EvictionPriority)
            {
                Get().SetEvictionPriority(EvictionPriority);
            }

            UINT GetEvictionPriority()
            {
                return Get().GetEvictionPriority();
            }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };

    template<>
    struct Traits<ID3D11Buffer>
    {
        using value_type = ID3D11Buffer;
        using pointer = ID3D11Buffer*;
        using const_pointer = const ID3D11Buffer*;
        using reference = ID3D11Buffer&;
        using const_reference = const ID3D11Buffer&;

        template<class DerivedSelf>
        class Interface : public Traits<ID3D11Resource>::Interface<DerivedSelf>
        {
        private:
            using derived_self = DerivedSelf;

        public:
            D3D11_BUFFER_DESC GetDesc()
            {
                D3D11_BUFFER_DESC pDesc;
                Get().GetDesc(&pDesc);
                return pDesc;
            }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };

    template<>
    struct Traits<ID3D11Texture1D>
    {
        using value_type = ID3D11Texture1D;
        using pointer = ID3D11Texture1D*;
        using const_pointer = const ID3D11Texture1D*;
        using reference = ID3D11Texture1D&;
        using const_reference = const ID3D11Texture1D&;

        template<class DerivedSelf>
        class Interface : public Traits<ID3D11Resource>::Interface<DerivedSelf>
        {
        private:
            using derived_self = DerivedSelf;

        public:
            D3D11_TEXTURE1D_DESC GetDesc()
            {
                D3D11_TEXTURE1D_DESC pDesc;
                Get().GetDesc(&pDesc);
                return pDesc;
            }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };

    template<>
    struct Traits<ID3D11Texture2D>
    {
        using value_type = ID3D11Texture2D;
        using pointer = ID3D11Texture2D*;
        using const_pointer = const ID3D11Texture2D*;
        using reference = ID3D11Texture2D&;
        using const_reference = const ID3D11Texture2D&;

        template<class DerivedSelf>
        class Interface : public Traits<ID3D11Resource>::Interface<DerivedSelf>
        {
        private:
            using derived_self = DerivedSelf;

        public:
            D3D11_TEXTURE2D_DESC GetDesc()
            {
                D3D11_TEXTURE2D_DESC pDesc;
                Get().GetDesc(&pDesc);
                return pDesc;
            }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };

    template<>
    struct Traits<ID3D11Texture3D>
    {
        using value_type = ID3D11Texture3D;
        using pointer = ID3D11Texture3D*;
        using const_pointer = const ID3D11Texture3D*;
        using reference = ID3D11Texture3D&;
        using const_reference = const ID3D11Texture3D&;

        template<class DerivedSelf>
        class Interface : public Traits<ID3D11Resource>::Interface<DerivedSelf>
        {
        private:
            using derived_self = DerivedSelf;

        public:
            D3D11_TEXTURE3D_DESC GetDesc()
            {
                D3D11_TEXTURE3D_DESC pDesc;
                Get().GetDesc(&pDesc);
                return pDesc;
            }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };
}