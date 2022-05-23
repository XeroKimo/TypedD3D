#pragma once
#include "../Internal/ComWrapper.h"
#include "../D3D12Wrappers.h"
#include <d3d12.h>

namespace TypedD3D::Internal
{
    template<TypeTag Type>
        requires Is_Descriptor_Heap_Type<Type>
    class InterfaceWrapper<D3D12_CPU_DESCRIPTOR_HANDLE, Type> : public D3D12_CPU_DESCRIPTOR_HANDLE
    {
    private:
        using CPU_DESCRIPTOR_HANDLE = InterfaceWrapper<D3D12_CPU_DESCRIPTOR_HANDLE, Type>;

    public:
        static constexpr TypeTag tag_value = Type;
        static constexpr D3D12_DESCRIPTOR_HEAP_TYPE value = descriptorHeapType<Type>;

    public:
        InterfaceWrapper() = default;
        InterfaceWrapper(D3D12_CPU_DESCRIPTOR_HANDLE handle) :
            D3D12_CPU_DESCRIPTOR_HANDLE(handle)
        {

        }

    public:
        CPU_DESCRIPTOR_HANDLE Offset(INT64 offsetInDescriptors, UINT64 incrementSize)
        {
            auto copy = *this;
            copy.ptr += incrementSize * offsetInDescriptors;
            return copy;
        }
        CPU_DESCRIPTOR_HANDLE Offset(INT64 offset)
        {
            auto copy = *this;
            copy.ptr += offset;
            return copy;
        }
    };

    template<TypeTag Type>
        requires Is_Descriptor_Heap_Type<Type>
    class InterfaceWrapper<D3D12_GPU_DESCRIPTOR_HANDLE, Type> : public D3D12_GPU_DESCRIPTOR_HANDLE
    {
    private:
        using GPU_DESCRIPTOR_HANDLE = InterfaceWrapper<D3D12_GPU_DESCRIPTOR_HANDLE, Type>;

    public:
        static constexpr TypeTag tag_value = Type;
        static constexpr D3D12_DESCRIPTOR_HEAP_TYPE value = descriptorHeapType<Type>;

    public:
        InterfaceWrapper() = default;
        InterfaceWrapper(D3D12_GPU_DESCRIPTOR_HANDLE handle) :
            D3D12_GPU_DESCRIPTOR_HANDLE(handle)
        {

        }

    public:
        GPU_DESCRIPTOR_HANDLE Offset(INT64 offsetInDescriptors, UINT64 incrementSize)
        {
            auto copy = *this;
            copy.ptr += incrementSize * offsetInDescriptors;
            return copy;
        }

        GPU_DESCRIPTOR_HANDLE Offset(INT64 offset)
        {
            auto copy = *this;
            copy.ptr += offset;
            return copy;
        }
    };

    namespace D3D12
    {
        template<TypeTag Type>
        using DescriptorHeap_t = InterfaceWrapper<ID3D12DescriptorHeap, Type>;

        namespace DescriptorHeap
        {
            template<TypeTag Type>
            using CPU_DESCRIPTOR_HANDLE = InterfaceWrapper<D3D12_CPU_DESCRIPTOR_HANDLE, Type>;

            template<TypeTag Type>
            using GPU_DESCRIPTOR_HANDLE = InterfaceWrapper<D3D12_GPU_DESCRIPTOR_HANDLE, Type>;

            static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<TypeTag::CBV_SRV_UAV>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<TypeTag::CBV_SRV_UAV>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<TypeTag::DSV>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<TypeTag::DSV>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

            static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<TypeTag::RTV>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<TypeTag::RTV>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<TypeTag::Sampler>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<TypeTag::Sampler>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

            template<class WrapperTy, D3D12_DESCRIPTOR_HEAP_TYPE Type>
            class Interface
            {
            public:
                using CPU_DESCRIPTOR_HANDLE = CPU_DESCRIPTOR_HANDLE<tagValue<Type>>;
                using GPU_DESCRIPTOR_HANDLE = GPU_DESCRIPTOR_HANDLE<tagValue<Type>>;

            public:
                D3D12_DESCRIPTOR_HEAP_DESC GetDesc() { return InternalGet().GetDesc(); }
                CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() { return CPU_DESCRIPTOR_HANDLE(InternalGet().GetCPUDescriptorHandleForHeapStart()); }
                GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() { return GPU_DESCRIPTOR_HANDLE(InternalGet().GetGPUDescriptorHandleForHeapStart()); }

            private:
                ID3D12DescriptorHeap& InternalGet() { return  *static_cast<WrapperTy&>(*this).Get(); }
            };
        }
    }
    template<class DirectXClass, TypeTag Type>
        requires std::is_base_of_v<ID3D12DescriptorHeap, DirectXClass> && Is_Descriptor_Heap_Type<Type>
    class InterfaceWrapper<DirectXClass, Type> : public ComWrapper<DirectXClass>, private D3D12::DescriptorHeap::Interface<InterfaceWrapper<DirectXClass, Type>, descriptorHeapType<Type>>
    {
    private:
        using Interface = D3D12::DescriptorHeap::Interface<InterfaceWrapper<DirectXClass, Type>, descriptorHeapType<Type>>;
        friend Interface;

    public:
        using CPU_DESCRIPTOR_HANDLE = Interface::CPU_DESCRIPTOR_HANDLE;
        using GPU_DESCRIPTOR_HANDLE = Interface::GPU_DESCRIPTOR_HANDLE;

        static constexpr TypeTag tag_value = Type;
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
    template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
    using DescriptorHeap_t = TypedD3D::Internal::D3D12::DescriptorHeap_t<TypedD3D::Internal::tagValue<Type>>;

    namespace DescriptorHeap
    {
        using CBV_SRV_UAV = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>;
        using Sampler = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER>;
        using RTV = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_RTV>;
        using DSV = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_DSV>;
    }
}