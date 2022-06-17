#pragma once
#include "Wrappers.h"
#include <d3d12.h>

namespace TypedD3D
{
    namespace Internal
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
    }

    template<class Ty>
    using Direct = Internal::DirectMapper<Ty>::type;

    template<class Ty>
    using Compute = Internal::ComputeMapper<Ty>::type;

    template<class Ty>
    using Copy = Internal::CopyMapper<Ty>::type;

    template<class Ty>
    using Bundle = Internal::BundleMapper<Ty>::type;

    template<class Ty>
    using CBV_SRV_UAV = Internal::CBV_SRV_UAVMapper<Ty>::type;

    template<class Ty>
    using Sampler = Internal::SamplerMapper<Ty>::type;

    template<class Ty>
    using RTV = Internal::RTVMapper<Ty>::type;

    template<class Ty>
    using DSV = Internal::DSVMapper<Ty>::type;

    template<class Ty>
    using Graphics = Internal::GraphicsMapper<Ty>::type;

    template<class Ty>
    using ShaderVisible = Internal::ShaderVisibleMapper<Ty>::type;
}