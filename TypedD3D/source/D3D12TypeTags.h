#pragma once
namespace TypedD3D
{
    namespace Internal
    {
        enum class TypeTag
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
            RenderPass
        };
    }
}