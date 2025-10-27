module;

#include <utility>
#include <d3d12.h>

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

        using inner_type = ID3D12Resource;
        template<class Derived>
        class Interface : public InterfaceBase<UntaggedTraits<Derived>>
        {
        private:
            using derived_self = Derived;

        public:
            std::byte* Map(UINT Subresource, const D3D12_RANGE* optReadRange)
            {
                void* dataPtr;
                ThrowIfFailed(Self().Map(Subresource, optReadRange, &dataPtr));
                return static_cast<std::byte*>(dataPtr);
            }

            void Unmap(UINT Subresource, const D3D12_RANGE* optWrittenRange)
            {
                Self().Unmap(0, nullptr);
            }

            D3D12_RESOURCE_DESC GetDesc() { return Self().GetDesc(); }

            D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return Self().GetGPUVirtualAddress(); }

            HRESULT WriteToSubresource(
                UINT DstSubresource,
                const D3D12_BOX* optDstBox,
                const void* pSrcData,
                UINT SrcRowPitch,
                UINT SrcDepthPitch)
            {
                return Self().WriteToSubresource(DstSubresource, optDstBox, &pSrcData, SrcRowPitch, SrcDepthPitch);
            }

            HRESULT STDMETHODCALLTYPE ReadFromSubresource(
                void* pDstData,
                UINT DstRowPitch,
                UINT DstDepthPitch,
                UINT SrcSubresource,
                const D3D12_BOX* optSrcBox)
            {
                return Self().ReadFromSubresource(pDstData, DstRowPitch, DstDepthPitch, SrcSubresource, optSrcBox);
            }

            std::pair<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> GetHeapProperties()
            {
                std::pair<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> properties;
                Self().GetHeapProperties(&properties.first, &properties.second);
                return properties;
            }

        private:
            using InterfaceBase<UntaggedTraits<Derived>>::Self;
            using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
        };
    };

    template<>
    struct UntaggedTraits<ID3D12Resource1>
    {
        using value_type = ID3D12Resource1;
        using pointer = ID3D12Resource1*;
        using const_pointer = const ID3D12Resource1*;
        using reference = ID3D12Resource1&;
        using const_reference = const ID3D12Resource1&;

        template<class Derived>
        class Interface : public UntaggedTraits<ID3D12Resource>::Interface<Derived>
        {
		public:
			//TODO: Figure out how this works to update to a more modern API
			HRESULT GetProtectedResourceSession(
				REFIID riid,
				_COM_Outptr_opt_  void** ppProtectedSession)
			{
				return Self().GetProtectedResourceSession(riid, ppProtectedSession);
			}

        private:
            using InterfaceBase<UntaggedTraits<Derived>>::Self;
            using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
        };
    };

    template<>
    struct UntaggedTraits<ID3D12Resource2>
    {
        using value_type = ID3D12Resource2;
        using pointer = ID3D12Resource2*;
        using const_pointer = const ID3D12Resource2*;
        using reference = ID3D12Resource2&;
        using const_reference = const ID3D12Resource2&;

        template<class Derived>
        class Interface : public UntaggedTraits<ID3D12Resource>::Interface<Derived>
        {
        private:
            using derived_self = Derived;

		public:
			D3D12_RESOURCE_DESC1 GetDesc1()
			{
				return Self().GetDesc1();
			}

        private:
            using InterfaceBase<UntaggedTraits<Derived>>::Self;
            using InterfaceBase<UntaggedTraits<Derived>>::ToDerived;
        };
    };
}