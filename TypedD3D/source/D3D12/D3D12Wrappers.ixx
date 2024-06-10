export module TypedD3D12:Wrappers; 
import TypedD3D.Shared;

namespace TypedD3D::D3D12
{
    export enum class TraitTags
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

    export template<class Ty, TraitTags Tag>
    struct TaggedTraits;

    template<class Ty>
    using DirectTraits = TaggedTraits<Ty, TraitTags::Direct>;

    template<class Ty>
    using ComputeTraits = TaggedTraits<Ty, TraitTags::Compute>;

    template<class Ty>
    using CopyTraits = TaggedTraits<Ty, TraitTags::Copy>;

    template<class Ty>
    using BundleTraits = TaggedTraits<Ty, TraitTags::Bundle>;

    template<class Ty>
    using CBV_SRV_UAVTraits = TaggedTraits<Ty, TraitTags::CBV_SRV_UAV>;

    template<class Ty>
    using SamplerTraits = TaggedTraits<Ty, TraitTags::Sampler>;

    template<class Ty>
    using RTVTraits = TaggedTraits<Ty, TraitTags::RTV>;

    template<class Ty>
    using DSVTraits = TaggedTraits<Ty, TraitTags::DSV>;

    template<class Ty>
    using GraphicsTraits = TaggedTraits<Ty, TraitTags::Graphics>;

    template<class Ty>
    using RenderPassTraits = TaggedTraits<Ty, TraitTags::Render_Pass>;

    export template<TraitTags Tag>
    struct TraitTagToTypeMapper
    {
        template<class Ty>
        using type = TaggedTraits<Ty, Tag>;
    };
    
    export template<TraitTags Tag>
    using GetTraitTagType = TraitTagToTypeMapper<Tag>::template type;

    template<class Ty, TraitTags Tag>
    struct TaggedWrapperMapper
    {
        using type = IUnknownWrapper<Ty, TraitTagToTypeMapper<Tag>::type>;
    };

    template<class Ty>
    struct ShaderVisibleMapper;

    export template<class Ty>
    using Wrapper = TypedD3D::Wrapper<Ty>;

    export template<class Ty>
    using Direct = TaggedWrapperMapper<Ty, TraitTags::Direct>::type;

    export template<class Ty>
    using Compute = TaggedWrapperMapper<Ty, TraitTags::Compute>::type;

    export template<class Ty>
    using Copy = TaggedWrapperMapper<Ty, TraitTags::Copy>::type;

    export template<class Ty>
    using Bundle = TaggedWrapperMapper<Ty, TraitTags::Bundle>::type;

    export template<class Ty>
    using CBV_SRV_UAV = TaggedWrapperMapper<Ty, TraitTags::CBV_SRV_UAV>::type;

    export template<class Ty>
    using Sampler = TaggedWrapperMapper<Ty, TraitTags::Sampler>::type;

    export template<class Ty>
    using RTV = TaggedWrapperMapper<Ty, TraitTags::RTV>::type;

    export template<class Ty>
    using DSV = TaggedWrapperMapper<Ty, TraitTags::DSV>::type;

    export template<class Ty>
    using Graphics = TaggedWrapperMapper<Ty, TraitTags::Graphics>::type;
}