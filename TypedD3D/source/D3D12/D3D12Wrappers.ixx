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

    export template<class Ty>
    using DirectTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Direct>;

    export template<class Ty>
    using ComputeTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Compute>;

    export template<class Ty>
    using CopyTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Copy>;

    export template<class Ty>
    using BundleTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Bundle>;

    export template<class Ty>
    using CBV_SRV_UAVTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::CBV_SRV_UAV>;

    export template<class Ty>
    using SamplerTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Sampler>;

    export template<class Ty>
    using RTVTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::RTV>;

    export template<class Ty>
    using DSVTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::DSV>;

    export template<class Ty>
    using GraphicsTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Graphics>;

    export template<class Ty>
    using RenderPassTraits = D3D12TaggedTraits<Ty, D3D12TraitTags::Render_Pass>;

    export template<D3D12TraitTags Tag>
    struct TraitTagToTypeMapper;

    template<>
    struct TraitTagToTypeMapper<D3D12TraitTags::Direct>
    {
        template<class Ty>
        using type = DirectTraits<Ty>;
    };

    template<>
    struct TraitTagToTypeMapper<D3D12TraitTags::Compute>
    {
        template<class Ty>
        using type = ComputeTraits<Ty>;
    };

    template<>
    struct TraitTagToTypeMapper<D3D12TraitTags::Copy>
    {
        template<class Ty>
        using type = CopyTraits<Ty>;
    };

    template<>
    struct TraitTagToTypeMapper<D3D12TraitTags::Bundle>
    {
        template<class Ty>
        using type = BundleTraits<Ty>;
    };

    template<>
    struct TraitTagToTypeMapper<D3D12TraitTags::CBV_SRV_UAV>
    {
        template<class Ty>
        using type = CBV_SRV_UAVTraits<Ty>;
    };

    template<>
    struct TraitTagToTypeMapper<D3D12TraitTags::Sampler>
    {
        template<class Ty>
        using type = SamplerTraits<Ty>;
    };

    template<>
    struct TraitTagToTypeMapper<D3D12TraitTags::RTV>
    {
        template<class Ty>
        using type = RTVTraits<Ty>;
    };

    template<>
    struct TraitTagToTypeMapper<D3D12TraitTags::DSV>
    {
        template<class Ty>
        using type = DSVTraits<Ty>;
    };

    template<>
    struct TraitTagToTypeMapper<D3D12TraitTags::Graphics>
    {
        template<class Ty>
        using type = GraphicsTraits<Ty>;
    };

    template<>
    struct TraitTagToTypeMapper<D3D12TraitTags::Render_Pass>
    {
        template<class Ty>
        using type = RenderPassTraits<Ty>;
    };
    
    export template<D3D12TraitTags Tag>
    using GetTraitTagType = TraitTagToTypeMapper<Tag>::template type;
    
    export template<D3D12TraitTags Tag, class Ty>
    using GetTraitTagType2 = TraitTagToTypeMapper<Tag>::template type<Ty>;

    template<class Ty>
    struct DirectMapper
    {
        using type = IUnknownWrapper<Ty, DirectTraits>;
    };

    template<class Ty>
    struct ComputeMapper
    {
        using type = IUnknownWrapper<Ty, ComputeTraits>;
    };

    template<class Ty>
    struct CopyMapper
    {
        using type = IUnknownWrapper<Ty, CopyTraits>;
    };

    template<class Ty>
    struct BundleMapper
    {
        using type = IUnknownWrapper<Ty, BundleTraits>;
    };

    template<class Ty>
    struct CBV_SRV_UAVMapper
    {
        using type = IUnknownWrapper<Ty, CBV_SRV_UAVTraits>;
    };

    template<class Ty>
    struct SamplerMapper
    {
        using type = IUnknownWrapper<Ty, SamplerTraits>;
    };

    template<class Ty>
    struct RTVMapper
    {
        using type = IUnknownWrapper<Ty, RTVTraits>;
    };

    template<class Ty>
    struct DSVMapper
    {
        using type = IUnknownWrapper<Ty, DSVTraits>;
    };

    template<class Ty>
    struct GraphicsMapper
    {
        using type = IUnknownWrapper<Ty, GraphicsTraits>;
    };

    template<class Ty>
    struct ShaderVisibleMapper;

    template<class Ty>
    struct RenderPassMapper
    {
        using type = IUnknownWrapper<Ty, RenderPassTraits>;
    };

    export template<class Ty>
    using Direct = DirectMapper<Ty>::type;

    export template<class Ty>
    using Compute = ComputeMapper<Ty>::type;

    export template<class Ty>
    using Copy = CopyMapper<Ty>::type;

    export template<class Ty>
    using Bundle = BundleMapper<Ty>::type;

    export template<class Ty>
    using CBV_SRV_UAV = CBV_SRV_UAVMapper<Ty>::type;

    export template<class Ty>
    using Sampler = SamplerMapper<Ty>::type;

    export template<class Ty>
    using RTV = RTVMapper<Ty>::type;

    export template<class Ty>
    using DSV = DSVMapper<Ty>::type;

    export template<class Ty>
    using Graphics = GraphicsMapper<Ty>::type;

    export template<class Ty>
    using ShaderVisible = ShaderVisibleMapper<Ty>::type;
}