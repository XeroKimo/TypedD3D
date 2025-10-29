module;

#include <concepts>
#include <d3d12.h>

export module TypedD3D12:Wrappers; 
import TypedD3D.Shared;

namespace TypedD3D
{
    export template<class Ty>
    struct DirectTag;

	export template<class Ty>
    struct ComputeTag;

	export template<class Ty>
    struct CopyTag;

	export template<class Ty>
    struct BundleTag;

	export template<class Ty>
    struct CBV_SRV_UAVTag;

	export template<class Ty>
    struct SamplerTag;

	export template<class Ty>
    struct RTVTag;

	export template<class Ty>
    struct DSVTag;

	export template<class Ty>
    struct GraphicsTag;

	export template<class Ty>
    struct RenderPassTag;

	export template<class Ty>
    struct ShaderVisibleTag;

    template<class Ty>
    struct DirectMapper
    {
        using type = StrongWrapper<DirectTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<DirectTag<Ty>>
    struct DirectMapper<Ty>
    {
        using type = TypedStruct<DirectTag<Ty>>;
    };

    template<class Ty>
    struct DirectViewMapper
    {
        using type = ReplaceOuterType<typename DirectMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct ComputeMapper
    {
        using type = StrongWrapper<ComputeTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<ComputeTag<Ty>>
    struct ComputeMapper<Ty>
    {
        using type = TypedStruct<ComputeTag<Ty>>;
    };

    template<class Ty>
    struct ComputeViewMapper
    {
        using type = ReplaceOuterType<typename ComputeMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct CopyMapper
    {
        using type = StrongWrapper<CopyTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<CopyTag<Ty>>
    struct CopyMapper<Ty>
    {
        using type = TypedStruct<CopyTag<Ty>>;
    };

    template<class Ty>
    struct CopyViewMapper
    {
        using type = ReplaceOuterType<typename CopyMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct BundleMapper
    {
        using type = StrongWrapper<BundleTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<BundleTag<Ty>>
    struct BundleMapper<Ty>
    {
        using type = TypedStruct<BundleTag<Ty>>;
    };

    template<class Ty>
    struct BundleViewMapper
    {
        using type = ReplaceOuterType<typename BundleMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct CBV_SRV_UAVMapper
    {
        using type = StrongWrapper<CBV_SRV_UAVTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<CBV_SRV_UAVTag<Ty>>
    struct CBV_SRV_UAVMapper<Ty>
    {
        using type = TypedStruct<CBV_SRV_UAVTag<Ty>>;
    };

    template<class Ty>
    struct CBV_SRV_UAVViewMapper
    {
        using type = ReplaceOuterType<typename CBV_SRV_UAVMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct SamplerMapper
    {
        using type = StrongWrapper<SamplerTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<SamplerTag<Ty>>
    struct SamplerMapper<Ty>
    {
        using type = TypedStruct<SamplerTag<Ty>>;
    };

    template<class Ty>
    struct SamplerViewMapper
    {
        using type = ReplaceOuterType<typename SamplerMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct RTVMapper
    {
        using type = StrongWrapper<RTVTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<RTVTag<Ty>>
    struct RTVMapper<Ty>
    {
        using type = TypedStruct<RTVTag<Ty>>;
    };

    template<class Ty>
    struct RTVViewMapper
    {
        using type = ReplaceOuterType<typename RTVMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct DSVMapper
    {
        using type = StrongWrapper<DSVTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<DSVTag<Ty>>
    struct DSVMapper<Ty>
    {
        using type = TypedStruct<DSVTag<Ty>>;
    };

    template<class Ty>
    struct DSVViewMapper
    {
        using type = ReplaceOuterType<typename DSVMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct GraphicsMapper
    {
        using type = StrongWrapper<GraphicsTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<GraphicsTag<Ty>>
    struct GraphicsMapper<Ty>
    {
        using type = TypedStruct<GraphicsTag<Ty>>;
    };

    template<class Ty>
    struct GraphicsViewMapper
    {
        using type = ReplaceOuterType<typename GraphicsMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct RenderPassMapper
    {
        using type = StrongWrapper<RenderPassTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<RenderPassTag<Ty>>
    struct RenderPassMapper<Ty>
    {
        using type = TypedStruct<RenderPassTag<Ty>>;
    };

    template<class Ty>
    struct RenderPassViewMapper
    {
        using type = ReplaceOuterType<typename RenderPassMapper<Ty>::type, WeakWrapper>;
    };

    template<class Ty>
    struct ShaderVisibleMapper
    {
        using type = StrongWrapper<ShaderVisibleTag<Ty>>;
    };

    template<class Ty>
        requires TypedStructTrait<ShaderVisibleTag<Ty>>
    struct ShaderVisibleMapper<Ty>
    {
        using type = TypedStruct<ShaderVisibleTag<Ty>>;
    };

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
    using RenderPass = RenderPassMapper<Ty>::type;

    export template<class Ty>
    using RenderPassView = RenderPassViewMapper<Ty>::type;

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