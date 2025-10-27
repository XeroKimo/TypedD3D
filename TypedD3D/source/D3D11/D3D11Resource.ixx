module;

#include <d3d11_4.h>
export module TypedD3D11:Resources;
import :DeviceChild;
import TypedD3D.Shared;

namespace TypedD3D
{
    export template<>
    struct UntaggedTraits<ID3D11Resource>
    {
        using value_type = ID3D11Resource;
        using pointer = ID3D11Resource*;
        using const_pointer = const ID3D11Resource*;
        using reference = ID3D11Resource&;
        using const_reference = const ID3D11Resource&;

        using inner_type = ID3D11Resource;

        template<class Derived>
        class Interface : public UntaggedTraits<ID3D11DeviceChild>
        {
        public:
            D3D11_RESOURCE_DIMENSION GetType()
            {
                D3D11_RESOURCE_DIMENSION pResourceDimension;
                Self().GetType(&pResourceDimension);
                return pResourceDimension;
            }

            void SetEvictionPriority(UINT EvictionPriority)
            {
                Self().SetEvictionPriority(EvictionPriority);
            }

            UINT GetEvictionPriority()
            {
                return Self().GetEvictionPriority();
            }

        private:
            using InterfaceBase<UntaggedTraits<Derived>>::Self;
            using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
        };
    };

    export template<>
    struct UntaggedTraits<ID3D11Buffer>
    {
        using value_type = ID3D11Buffer;
        using pointer = ID3D11Buffer*;
        using const_pointer = const ID3D11Buffer*;
        using reference = ID3D11Buffer&;
        using const_reference = const ID3D11Buffer&;

        using inner_type = ID3D11Buffer;

        template<class Derived>
        class Interface : public UntaggedTraits<ID3D11Resource>::Interface<Derived>
        {
        public:
            D3D11_BUFFER_DESC GetDesc()
            {
                D3D11_BUFFER_DESC pDesc;
                Self().GetDesc(&pDesc);
                return pDesc;
            }

        private:
            using InterfaceBase<UntaggedTraits<Derived>>::Self;
            using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
        };
    };

    export template<>
    struct UntaggedTraits<ID3D11Texture1D>
    {
        using value_type = ID3D11Texture1D;
        using pointer = ID3D11Texture1D*;
        using const_pointer = const ID3D11Texture1D*;
        using reference = ID3D11Texture1D&;
        using const_reference = const ID3D11Texture1D&;

        using inner_type = ID3D11Texture1D;
        template<class Derived>
        class Interface : public UntaggedTraits<ID3D11Resource>::Interface<Derived>
        {
        private:
            using derived_self = Derived;

        public:
            D3D11_TEXTURE1D_DESC GetDesc()
            {
                D3D11_TEXTURE1D_DESC pDesc;
                Self().GetDesc(&pDesc);
                return pDesc;
            }

        private:
            using InterfaceBase<UntaggedTraits<Derived>>::Self;
            using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
        };
    };

    export template<>
    struct UntaggedTraits<ID3D11Texture2D>
    {
        using value_type = ID3D11Texture2D;
        using pointer = ID3D11Texture2D*;
        using const_pointer = const ID3D11Texture2D*;
        using reference = ID3D11Texture2D&;
        using const_reference = const ID3D11Texture2D&;

        using inner_type = ID3D11Texture2D;
        template<class Derived>
        class Interface : public UntaggedTraits<ID3D11Resource>::Interface<Derived>
        {
        private:
            using derived_self = Derived;

        public:
            D3D11_TEXTURE2D_DESC GetDesc()
            {
                D3D11_TEXTURE2D_DESC pDesc;
                Self().GetDesc(&pDesc);
                return pDesc;
            }

        private:
            using InterfaceBase<UntaggedTraits<Derived>>::Self;
            using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
        };
    };

    export template<>
    struct UntaggedTraits<ID3D11Texture3D>
    {
        using value_type = ID3D11Texture3D;
        using pointer = ID3D11Texture3D*;
        using const_pointer = const ID3D11Texture3D*;
        using reference = ID3D11Texture3D&;
        using const_reference = const ID3D11Texture3D&;

        using inner_type = ID3D11Texture3D;

        template<class Derived>
        class Interface : public UntaggedTraits<ID3D11Resource>::Interface<Derived>
        {
        private:

        public:
            D3D11_TEXTURE3D_DESC GetDesc()
            {
                D3D11_TEXTURE3D_DESC pDesc;
                Self().GetDesc(&pDesc);
                return pDesc;
            }

        private:
            using InterfaceBase<UntaggedTraits<Derived>>::Self;
            using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
        };
    };
}