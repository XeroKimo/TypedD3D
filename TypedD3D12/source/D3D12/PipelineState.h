#pragma once
#include "ComWrapper.h"
#include "../Helpers/COMHelpers.h"
#include <d3d12.h>

namespace TypedD3D::D3D12::PipelineState
{
    namespace Internal
    {
        template<class PipelineTy>
        class PipelineState : public ComWrapper<ID3D12PipelineState>
        {
        public:
            using ComWrapper<ID3D12PipelineState>::ComWrapper;

        public:
            Microsoft::WRL::ComPtr<ID3DBlob> GetCachedBlob()
            {
                return Helpers::COM::UnknownObjectForwardFunction<ID3DBlob>(&ID3D12PipelineState::GetCachedBlob, Get());
            }
        };
        
    }

    using Graphics = Internal::PipelineState<D3D12_GRAPHICS_PIPELINE_STATE_DESC>;
    using Compute = Internal::PipelineState<D3D12_COMPUTE_PIPELINE_STATE_DESC>;
}