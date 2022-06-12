#pragma once
#include "DeviceChild.h"


namespace TypedD3D::Internal
{
    namespace D3D11
    {
        namespace InputLayout
        {
            template<class WrapperTy>
            class Interface : public DeviceChild::Interface<WrapperTy>
            {
            };
        }
    }

    template<std::derived_from<ID3D11InputLayout> Ty>
    class InterfaceWrapper<Ty> : public ComWrapper<Ty>, private D3D11::InputLayout::Interface<InterfaceWrapper<Ty>>
    {
    private:
        using Interface = D3D11::InputLayout::Interface<InterfaceWrapper<Ty>>;
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