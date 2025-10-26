module;

#include <concepts>
#include <d3d12.h>

export module TypedD3D12:Wrappers; 
import TypedD3D.Shared;

namespace TypedD3D
{
    export template<class Ty>
    struct DirectTraits;

	export template<class Ty>
    struct ComputeTraits;

	export template<class Ty>
    struct CopyTraits;

	export template<class Ty>
    struct BundleTraits;

	export template<class Ty>
    struct CBV_SRV_UAVTraits;

	export template<class Ty>
    struct SamplerTraits;

	export template<class Ty>
    struct RTVTraits;

	export template<class Ty>
    struct DSVTraits;

	export template<class Ty>
    struct GraphicsTraits;

	export template<class Ty>
    struct RenderPassTraits;

	export template<class Ty>
    struct ShaderVisibleTraits;

    template<class Ty>
    struct DirectMapper
    {
        using type = StrongWrapper<DirectTraits<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<DirectTraits<Ty>>
    struct DirectMapper<Ty>
    {
        using type = TypedStruct<DirectTraits<Ty>>;
    };

    template<class Ty>
    struct DirectViewMapper
    {
        using type = ReplaceOuterType<typename DirectMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct ComputeMapper
    {
        using type = StrongWrapper<ComputeTraits<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<ComputeTraits<Ty>>
    struct ComputeMapper<Ty>
    {
        using type = TypedStruct<ComputeTraits<Ty>>;
    };

    template<class Ty>
    struct ComputeViewMapper
    {
        using type = ReplaceOuterType<typename ComputeMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct CopyMapper
    {
        using type = StrongWrapper<CopyTraits<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<CopyTraits<Ty>>
    struct CopyMapper<Ty>
    {
        using type = TypedStruct<CopyTraits<Ty>>;
    };

    template<class Ty>
    struct CopyViewMapper
    {
        using type = ReplaceOuterType<typename CopyMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct BundleMapper
    {
        using type = StrongWrapper<BundleTraits<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<BundleTraits<Ty>>
    struct BundleMapper<Ty>
    {
        using type = TypedStruct<BundleTraits<Ty>>;
    };

    template<class Ty>
    struct BundleViewMapper
    {
        using type = ReplaceOuterType<typename BundleMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct CBV_SRV_UAVMapper
    {
        using type = StrongWrapper<CBV_SRV_UAVTraits<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<CBV_SRV_UAVTraits<Ty>>
    struct CBV_SRV_UAVMapper<Ty>
    {
        using type = TypedStruct<CBV_SRV_UAVTraits<Ty>>;
    };

    template<class Ty>
    struct CBV_SRV_UAVViewMapper
    {
        using type = ReplaceOuterType<typename CBV_SRV_UAVMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct SamplerMapper
    {
        using type = StrongWrapper<SamplerTraits<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<SamplerTraits<Ty>>
    struct SamplerMapper<Ty>
    {
        using type = TypedStruct<SamplerTraits<Ty>>;
    };

    template<class Ty>
    struct SamplerViewMapper
    {
        using type = ReplaceOuterType<typename SamplerMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct RTVMapper
    {
        using type = StrongWrapper<RTVTraits<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<RTVTraits<Ty>>
    struct RTVMapper<Ty>
    {
        using type = TypedStruct<RTVTraits<Ty>>;
    };

    template<class Ty>
    struct RTVViewMapper
    {
        using type = ReplaceOuterType<typename RTVMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct DSVMapper
    {
        using type = StrongWrapper<DSVTraits<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<DSVTraits<Ty>>
    struct DSVMapper<Ty>
    {
        using type = TypedStruct<DSVTraits<Ty>>;
    };

    template<class Ty>
    struct DSVViewMapper
    {
        using type = ReplaceOuterType<typename DSVMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct GraphicsMapper
    {
        using type = StrongWrapper<GraphicsTraits<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<GraphicsTraits<Ty>>
    struct GraphicsMapper<Ty>
    {
        using type = TypedStruct<GraphicsTraits<Ty>>;
    };

    template<class Ty>
    struct GraphicsViewMapper
    {
        using type = ReplaceOuterType<typename GraphicsMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct RenderPassMapper
    {
        using type = StrongWrapper<RenderPassTraits<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<RenderPassTraits<Ty>>
    struct RenderPassMapper<Ty>
    {
        using type = TypedStruct<RenderPassTraits<Ty>>;
    };

    template<class Ty>
    struct RenderPassViewMapper
    {
        using type = ReplaceOuterType<typename RenderPassMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct ShaderVisibleMapper;

    template<class Ty>
    struct ShaderVisibleViewMapper
    {
        using type = ReplaceOuterType<typename ShaderVisibleMapper<Ty>::type, WeakWrapper>;
    };

    export template<class Ty>
    using Direct = DirectMapper<Ty>::type;

    export template<class Ty>
    using DirectView = DirectViewMapper<Ty>::type;

    export template<class Ty>
    using Compute = ComputeMapper<Ty>::type;

    export template<class Ty>
    using ComputeView = ComputeViewMapper<Ty>::type;

    export template<class Ty>
    using Copy = CopyMapper<Ty>::type;

    export template<class Ty>
    using CopyView = CopyViewMapper<Ty>::type;

    export template<class Ty>
    using Bundle = BundleMapper<Ty>::type;

    export template<class Ty>
    using BundleView = BundleViewMapper<Ty>::type;

    export template<class Ty>
    using CBV_SRV_UAV = CBV_SRV_UAVMapper<Ty>::type;

    export template<class Ty>
    using CBV_SRV_UAVView = CBV_SRV_UAVViewMapper<Ty>::type;

    export template<class Ty>
    using Sampler = SamplerMapper<Ty>::type;

    export template<class Ty>
    using SamplerView = SamplerViewMapper<Ty>::type;

    export template<class Ty>
    using RTV = RTVMapper<Ty>::type;

    export template<class Ty>
    using RTVView = RTVViewMapper<Ty>::type;

    export template<class Ty>
    using DSV = DSVMapper<Ty>::type;

    export template<class Ty>
    using DSVView = DSVViewMapper<Ty>::type;

    export template<class Ty>
    using Graphics = GraphicsMapper<Ty>::type;

    export template<class Ty>
    using GraphicsView = GraphicsViewMapper<Ty>::type;

    export template<class Ty>
    using ShaderVisible = ShaderVisibleMapper<Ty>::type;

    export template<class Ty>
    using ShaderVisibleView = ShaderVisibleViewMapper<Ty>::type;
}