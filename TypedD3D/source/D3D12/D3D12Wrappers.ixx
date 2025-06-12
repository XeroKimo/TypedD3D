module;

#include <concepts>
#include <d3d12.h>

export module TypedD3D12:Wrappers; 
import TypedD3D.Shared;

namespace TypedD3D::D3D12
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
    struct RenderPassMapper
    {
        using type = IUnknownWrapper<Ty, RenderPassTraits>;
    };

    template<class Ty>
	struct ShaderVisibleMapper
	{
		using type = Ty;
	};

    template<std::derived_from<IUnknown> Ty, template<class> class WrapperTraitTy>
    struct ShaderVisibleMapper<IUnknownWrapper<Ty, WrapperTraitTy>>
    {
        template<class Ty>
        using Trait = ShaderVisibleTraits<IUnknownWrapper<Ty, WrapperTraitTy>>;

        using type = IUnknownWrapper<Ty, Trait>;
    };

    export template<class Ty>
    using Wrapper = TypedD3D::Wrapper<Ty>;

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