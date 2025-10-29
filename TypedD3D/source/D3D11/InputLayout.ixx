module;
#include <d3d11_4.h>

export module TypedD3D11:InputLayout;
import :DeviceChild;

import TypedD3D.Shared;

namespace TypedD3D
{
    template<>
    struct Trait<Untagged<ID3D11InputLayout>>
    {

        using inner_type = ID3D11InputLayout;

        using inner_tag = ID3D11InputLayout;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;

        template<class Derived>
        class Interface : public Trait<Untagged<ID3D11DeviceChild>>::Interface<Derived>
        {

        };
    };
}