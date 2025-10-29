module;

#include <d3d11_4.h>
export module TypedD3D11:Resources;
import :DeviceChild;
import TypedD3D.Shared;

namespace TypedD3D
{
    export template<>
    struct Trait<Untagged<ID3D11Resource>>
    {

        using inner_type = ID3D11Resource;

        using inner_tag = ID3D11Resource;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;
        template<class Derived>
        class Interface : public Trait<Untagged<ID3D11DeviceChild>>::Interface<Derived>
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
            using InterfaceBase<Untagged<Derived>>::Self;
            using InterfaceBase<Untagged<Derived>>::ToDerived;
        };
    };

    static_assert(IUnknownTrait<Untagged<ID3D11Resource>>);

    export template<>
    struct Trait<Untagged<ID3D11Buffer>>
    {
        using inner_type = ID3D11Buffer;

        using inner_tag = ID3D11Buffer;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;

        template<class Derived>
        class Interface : public Trait<Untagged<ID3D11Resource>>::Interface<Derived>
        {
        public:
            D3D11_BUFFER_DESC GetDesc()
            {
                D3D11_BUFFER_DESC pDesc;
                Self().GetDesc(&pDesc);
                return pDesc;
            }

        private:
            using InterfaceBase<Untagged<Derived>>::Self;
            using InterfaceBase<Untagged<Derived>>::ToDerived;
        };
    };

    export template<>
    struct Trait<Untagged<ID3D11Texture1D>>
    {

        using inner_tag = ID3D11Texture1D;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;
        using inner_type = ID3D11Texture1D;
        template<class Derived>
        class Interface : public Trait<Untagged<ID3D11Resource>>::Interface<Derived>
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
            using InterfaceBase<Untagged<Derived>>::Self;
            using InterfaceBase<Untagged<Derived>>::ToDerived;
        };
    };

    export template<>
    struct Trait<Untagged<ID3D11Texture2D>>
    {

        using inner_tag = ID3D11Texture2D;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;
        using inner_type = ID3D11Texture2D;
        template<class Derived>
        class Interface : public Trait<Untagged<ID3D11Resource>>::Interface<Derived>
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
            using InterfaceBase<Untagged<Derived>>::Self;
            using InterfaceBase<Untagged<Derived>>::ToDerived;
        };
    };

    export template<>
    struct Trait<Untagged<ID3D11Texture3D>>
    {
        using inner_tag = ID3D11Texture3D;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;

        using inner_type = ID3D11Texture3D;

        template<class Derived>
        class Interface : public Trait<Untagged<ID3D11Resource>>::Interface<Derived>
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
            using InterfaceBase<Untagged<Derived>>::Self;
            using InterfaceBase<Untagged<Derived>>::ToDerived;
        };
    };
}