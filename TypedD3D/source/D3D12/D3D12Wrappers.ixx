export module TypedD3D12:Wrappers;

namespace TypedD3D
{
    template<class Ty>
    struct DirectMapper;

    template<class Ty>
    struct ComputeMapper;

    template<class Ty>
    struct CopyMapper;

    template<class Ty>
    struct BundleMapper;

    template<class Ty>
    struct CBV_SRV_UAVMapper;

    template<class Ty>
    struct SamplerMapper;

    template<class Ty>
    struct RTVMapper;

    template<class Ty>
    struct DSVMapper;

    template<class Ty>
    struct GraphicsMapper;

    template<class Ty>
    struct ShaderVisibleMapper;

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