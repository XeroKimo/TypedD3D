module;

#include <utility>
#include <d3d12.h>
#include <wrl/client.h>
#include <assert.h>
#include <cstddef>

export module TypedD3D12:Resource;
import TypedD3D.Shared;

namespace TypedD3D
{
    template<>
    struct UntaggedTraits<ID3D12Resource>
    {
        using value_type = ID3D12Resource;
        using pointer = ID3D12Resource*;
        using const_pointer = const ID3D12Resource*;
        using reference = ID3D12Resource&;
        using const_reference = const ID3D12Resource&;

        template<class DerivedSelf>
        class Interface
        {
        private:
            using derived_self = DerivedSelf;

        public:
            std::byte* Map(UINT Subresource, const D3D12_RANGE* optReadRange)
            {
                void* dataPtr;
                ThrowIfFailed(Get().Map(Subresource, optReadRange, &dataPtr));
                return static_cast<std::byte*>(dataPtr);
            }

            void Unmap(UINT Subresource, const D3D12_RANGE* optWrittenRange)
            {
                Get().Unmap(0, nullptr);
            }

            D3D12_RESOURCE_DESC GetDesc() { return Get().GetDesc(); }

            D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return Get().GetGPUVirtualAddress(); }

            HRESULT WriteToSubresource(
                UINT DstSubresource,
                const D3D12_BOX* optDstBox,
                const void* pSrcData,
                UINT SrcRowPitch,
                UINT SrcDepthPitch)
            {
                return Get().WriteToSubresource(DstSubresource, optDstBox, &pSrcData, SrcRowPitch, SrcDepthPitch);
            }

            HRESULT STDMETHODCALLTYPE ReadFromSubresource(
                void* pDstData,
                UINT DstRowPitch,
                UINT DstDepthPitch,
                UINT SrcSubresource,
                const D3D12_BOX* optSrcBox)
            {
                return Get().ReadFromSubresource(pDstData, DstRowPitch, DstDepthPitch, SrcSubresource, optSrcBox);
            }

            std::pair<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> GetHeapProperties()
            {
                std::pair<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> properties;
                Get().GetHeapProperties(&properties.first, &properties.second);
                return properties;
            }
        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };
}

namespace TypedD3D::D3D12
{
    namespace Aliases
    {
        export using D3D12Resource = IUnknownWrapper<ID3D12Resource, UntaggedTraits>;
    }
}