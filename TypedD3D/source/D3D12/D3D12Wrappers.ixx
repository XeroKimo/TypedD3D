export module TypedD3D12:Wrappers; 
import TypedD3D.Shared;

namespace TypedD3D
{
    export enum class D3D12TraitTags
    {
        Direct,
        Compute,
        Copy,
        Bundle,
        CBV_SRV_UAV,
        Sampler,
        RTV,
        DSV,
        Graphics,
        Render_Pass
    };

    export template<class Ty, D3D12TraitTags Tag>
    struct D3D12TaggedTraits;

    template<class Ty>
    using DirectTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Direct>;

    template<class Ty>
    using ComputeTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Compute>;

    template<class Ty>
    using CopyTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Copy>;

    template<class Ty>
    using BundleTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Bundle>;

    template<class Ty>
    using CBV_SRV_UAVTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::CBV_SRV_UAV>;

    template<class Ty>
    using SamplerTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Sampler>;

    template<class Ty>
    using RTVTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::RTV>;

    template<class Ty>
    using DSVTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::DSV>;

    template<class Ty>
    using GraphicsTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Graphics>;

    template<class Ty>
    using RenderPassTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Render_Pass>;

    export template<D3D12TraitTags Tag>
    struct TraitTagToTypeMapper
    {
        template<class Ty>
        using type = D3D12TaggedTraits<Ty, Tag>;
    };
    
    export template<D3D12TraitTags Tag>
    using GetTraitTagType = TraitTagToTypeMapper<Tag>::template type;

    template<class Ty, D3D12TraitTags Tag>
    struct D3D12TaggedWrapperMapper
    {
        using type = IUnknownWrapper<Ty, TraitTagToTypeMapper<Tag>::type>;
    };

    template<class Ty>
    struct ShaderVisibleMapper;

    export template<class Ty>
    using Direct = D3D12TaggedWrapperMapper<Ty, D3D12TraitTags::Direct>::type;

    export template<class Ty>
    using Compute = D3D12TaggedWrapperMapper<Ty, D3D12TraitTags::Compute>::type;

    export template<class Ty>
    using Copy = D3D12TaggedWrapperMapper<Ty, D3D12TraitTags::Copy>::type;

    export template<class Ty>
    using Bundle = D3D12TaggedWrapperMapper<Ty, D3D12TraitTags::Bundle>::type;

    export template<class Ty>
    using CBV_SRV_UAV = D3D12TaggedWrapperMapper<Ty, D3D12TraitTags::CBV_SRV_UAV>::type;

    export template<class Ty>
    using Sampler = D3D12TaggedWrapperMapper<Ty, D3D12TraitTags::Sampler>::type;

    export template<class Ty>
    using RTV = D3D12TaggedWrapperMapper<Ty, D3D12TraitTags::RTV>::type;

    export template<class Ty>
    using DSV = D3D12TaggedWrapperMapper<Ty, D3D12TraitTags::DSV>::type;

    export template<class Ty>
    using Graphics = D3D12TaggedWrapperMapper<Ty, D3D12TraitTags::Graphics>::type;
}