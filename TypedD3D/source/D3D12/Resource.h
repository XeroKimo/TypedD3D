#pragma once

#include "../Internal/ComWrapper.h"
#include "../D3D12Wrappers.h"
#include <d3d12.h>
#include <wrl/client.h>
#include <assert.h>
#include <cstddef>

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        namespace Resource
        {
            template<class WrapperTy>
            class Interface
            {
            public:
                tl::expected<std::byte*, HRESULT> Map(UINT Subresource, const D3D12_RANGE* optReadRange)
                {
                    void* dataPtr;

                    HRESULT result = InternalGet().Map(Subresource, optReadRange, &dataPtr);
                    if(FAILED(result))
                        return tl::unexpected(result);

                    return static_cast<std::byte*>(dataPtr);
                }

                void Unmap(UINT Subresource, const D3D12_RANGE* optWrittenRange)
                {
                    InternalGet().Unmap(0, nullptr);
                }

                D3D12_RESOURCE_DESC GetDesc() { return InternalGet().GetDesc(); }

                D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return InternalGet().GetGPUVirtualAddress(); }

                HRESULT WriteToSubresource(
                    UINT DstSubresource,
                    const D3D12_BOX* optDstBox,
                    const void* pSrcData,
                    UINT SrcRowPitch,
                    UINT SrcDepthPitch)
                {
                    return InternalGet().WriteToSubresource(DstSubresource, optDstBox, &pSrcData, SrcRowPitch, SrcDepthPitch);
                }

                HRESULT STDMETHODCALLTYPE ReadFromSubresource(
                    void* pDstData,
                    UINT DstRowPitch,
                    UINT DstDepthPitch,
                    UINT SrcSubresource,
                    const D3D12_BOX* optSrcBox)
                {
                    return InternalGet().ReadFromSubresource(pDstData, DstRowPitch, DstDepthPitch, SrcSubresource, optSrcBox);
                }

                std::pair<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> GetHeapProperties()
                {
                    std::pair<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> properties;
                    InternalGet().GetHeapProperties(&properties.first, &properties.second);
                    return properties;
                }

            private:
                ID3D12Resource& InternalGet() { return *static_cast<WrapperTy&>(*this).Get(); }
            };
        }
    }

    template<class DirectXClass>
        requires std::derived_from<DirectXClass, ID3D12Resource>
    class InterfaceWrapper<DirectXClass> : public ComWrapper<DirectXClass>, private D3D12::Resource::Interface<InterfaceWrapper<DirectXClass>>
    {
    private:
        using Interface = D3D12::Resource::Interface<InterfaceWrapper<DirectXClass>>;
        friend Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = DirectXClass;

    public:
        using ComWrapper<DirectXClass>::ComWrapper;

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}

namespace TypedD3D::D3D12
{
    using Resource = Wrapper<ID3D12Resource>;
}