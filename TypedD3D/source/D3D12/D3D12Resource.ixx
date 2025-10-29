module;

#include <utility>
#include <d3d12.h>

#include <cassert>
#include <cstddef>

export module TypedD3D12:Resource;
import TypedD3D.Shared;

namespace TypedD3D
{
    template<>
    struct Trait<Untagged<ID3D12Resource>>
    {
        using inner_type = ID3D12Resource;

        using inner_tag = ID3D12Resource;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;

        template<class Derived>
        class Interface : public InterfaceBase<Untagged<Derived>>
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
            using InterfaceBase<Untagged<Derived>>::Self;
            using InterfaceBase<Untagged<Derived>>::ToDerived;
        };
    };


    template<>
    struct Trait<Untagged<ID3D12Resource1>>
    {
        using inner_type = ID3D12Resource1;

        using inner_tag = ID3D12Resource1;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;

        template<class Derived>
        class Interface : public Trait<Untagged<ID3D12Resource>>::Interface<Derived>
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
            using InterfaceBase<Untagged<Derived>>::Self;
            using InterfaceBase<Untagged<Derived>>::ToDerived;
        };
    };

    template<>
    struct Trait<Untagged<ID3D12Resource2>>
    {
        using inner_type = ID3D12Resource2;

        using inner_tag = ID3D12Resource2;

        template<class NewInner>
        using ReplaceInnerType = Untagged<NewInner>;

        template<class NewInner>
        using trait_template = Untagged<NewInner>;

        template<class Derived>
        class Interface : public Trait<Untagged<ID3D12Resource1>>::Interface<Derived>
        {
        private:
            using derived_self = Derived;

		public:
			D3D12_RESOURCE_DESC1 GetDesc1()
			{
				return Self().GetDesc1();
			}

        private:
            using InterfaceBase<Untagged<Derived>>::Self;
            using InterfaceBase<Untagged<Derived>>::ToDerived;
        };
    };
}