module;

#include <concepts>
#include <d3d11_4.h>


export module TypedD3D11:DeviceChild;
import TypedD3D.Shared;

namespace TypedD3D
{
    template<DerivedFromExcept<ID3D11DeviceChild, 
        ID3D11VertexShader,
        ID3D11DomainShader,
        ID3D11HullShader,
        ID3D11GeometryShader,
        ID3D11PixelShader,
        ID3D11ComputeShader,
        ID3D11DepthStencilState,
        ID3D11SamplerState,
        ID3D11BlendState,
        ID3D11RasterizerState,
        ID3D11View> 
    Ty>
    struct Trait<Untagged<Ty>>
    {
        using inner_type = Ty;

        using inner_tag = Ty;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class Derived>
        using Interface = Ty*;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;
    };

    template<std::derived_from<ID3D11Debug> Ty>
    struct Trait<Untagged<Ty>>
    {
        using inner_tag = Ty;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        using inner_type = Ty;
        template<class Derived>
        using Interface = Ty*;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;
    };

    template<>
    struct Trait<Untagged<ID3D11DeviceChild>>
    {
        using inner_type = ID3D11DeviceChild;

        using inner_tag = ID3D11DeviceChild;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;
        template<class Derived>
        struct Interface : public InterfaceBase<Untagged<Derived>>
        {
        private:
            using unknown_type = ID3D11DeviceChild;
            using derived_type = Derived;

        public:
            template<std::derived_from<ID3D11Device> DeviceTy = ID3D11Device>
            Wrapper<DeviceTy> GetDevice()
            {
                ID3D11Device* device;
                Self().GetDevice(&device);
                Wrapper<DeviceTy> out;
                out.Attach(device);
                return Cast<DeviceTy>(std::move(device));
            }

            //TODO: Figure out how this works to update to a more modern API
            HRESULT GetPrivateData(
                REFGUID guid,
                UINT* pDataSize,
                void* pData)
            {
                return Self().GetPrivateData(guid, pDataSize, pData);
            }

            //TODO: Figure out how this works to update to a more modern API
            HRESULT SetPrivateData(
                REFGUID guid,
                UINT DataSize,
                const void* pData)
            {
                return Self().SetPrivateData(guid, DataSize, pData);
            }

            //TODO: Figure out how this works to update to a more modern API
            HRESULT SetPrivateDataInterface(
                REFGUID guid,
                const IUnknown* pData)
            {
                return Self().SetPrivateDataInterface(guid, pData);
            }

        private:
            using InterfaceBase<Untagged<Derived>>::Self;
            using InterfaceBase<Untagged<Derived>>::ToDerived;
        };
    };
}