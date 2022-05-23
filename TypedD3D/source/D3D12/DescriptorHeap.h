#pragma once
#include "ComWrapper.h"
#include "../D3D12Wrappers.h"
#include <d3d12.h>

namespace TypedD3D::D3D12::DescriptorHandle
{
    namespace Internal
    {
        template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
        class CPU_DESCRIPTOR_HANDLE : public D3D12_CPU_DESCRIPTOR_HANDLE
        {
        public:
            CPU_DESCRIPTOR_HANDLE() = default;
            CPU_DESCRIPTOR_HANDLE(D3D12_CPU_DESCRIPTOR_HANDLE handle) :
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

        static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
        static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_DSV>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
        static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_RTV>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
        static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

        template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
        class GPU_DESCRIPTOR_HANDLE : public D3D12_GPU_DESCRIPTOR_HANDLE
        {
        public:
            GPU_DESCRIPTOR_HANDLE() = default;
            GPU_DESCRIPTOR_HANDLE(D3D12_GPU_DESCRIPTOR_HANDLE handle) :
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

        static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>) == sizeof(D3D12_GPU_DESCRIPTOR_HANDLE));
        static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_DSV>) == sizeof(D3D12_GPU_DESCRIPTOR_HANDLE));
        static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_RTV>) == sizeof(D3D12_GPU_DESCRIPTOR_HANDLE));
        static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER>) == sizeof(D3D12_GPU_DESCRIPTOR_HANDLE));
    }

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
    using CPU_t = Internal::CPU_DESCRIPTOR_HANDLE<Type>;

    using CPU_CBV_SRV_UAV = CPU_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>;
    using CPU_SAMPLER = CPU_t<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER>;
    using CPU_RTV = CPU_t<D3D12_DESCRIPTOR_HEAP_TYPE_RTV>;
    using CPU_DSV = CPU_t<D3D12_DESCRIPTOR_HEAP_TYPE_DSV>;

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
    using GPU_t = Internal::GPU_DESCRIPTOR_HANDLE<Type>;
    
    using GPU_CBV_SRV_UAV = GPU_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>;
    using GPU_SAMPLER = GPU_t<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER>;
    using GPU_RTV = GPU_t<D3D12_DESCRIPTOR_HEAP_TYPE_RTV>;
    using GPU_DSV = GPU_t<D3D12_DESCRIPTOR_HEAP_TYPE_DSV>;
}

namespace TypedD3D::D3D12::DescriptorHeap
{
    namespace Internal
    {
        template<class WrapperTy, D3D12_DESCRIPTOR_HEAP_TYPE Type>
        class DescriptorHeapInterface
        {
        public:
            using CPU_HANDLE = DescriptorHandle::CPU_t<Type>;
            using GPU_HANDLE = DescriptorHandle::GPU_t<Type>;

        public:
            D3D12_DESCRIPTOR_HEAP_DESC GetDesc() { return InternalGet().GetDesc(); }
            CPU_HANDLE GetCPUDescriptorHandleForHeapStart() { return CPU_HANDLE(InternalGet().GetCPUDescriptorHandleForHeapStart()); }
            GPU_HANDLE GetGPUDescriptorHandleForHeapStart() { return GPU_HANDLE(InternalGet().GetGPUDescriptorHandleForHeapStart()); }

        private:
            ID3D12DescriptorHeap& InternalGet() { return  *static_cast<WrapperTy&>(*this).Get(); }
        };

        template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
        class DescriptorHeap : public ComWrapper<ID3D12DescriptorHeap>, private DescriptorHeapInterface<DescriptorHeap<Type>, Type>
        {
            static_assert(Type != D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES, "Num types is not a valid descriptor heap type");

            template<class WrapperTy2, D3D12_DESCRIPTOR_HEAP_TYPE Type2>
            friend class DescriptorHeapInterface;

        public:
            static constexpr D3D12_DESCRIPTOR_HEAP_TYPE value = Type;

        public:
            using ComWrapper<ID3D12DescriptorHeap>::ComWrapper;

        public:
            DescriptorHeapInterface<DescriptorHeap<Type>, Type>* GetInterface() { return this; }
            DescriptorHeapInterface<DescriptorHeap<Type>, Type>* operator->() { return this; }
        };
    }

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
    using DescriptorHeap_t = Internal::DescriptorHeap<Type>;

    using CBV_SRV_UAV = Internal::DescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>;
    using Sampler = Internal::DescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER>;
    using RTV = Internal::DescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_RTV>;
    using DSV = Internal::DescriptorHeap<D3D12_DESCRIPTOR_HEAP_TYPE_DSV>;
}

namespace TypedD3D::Internal
{
    template<class IUnknownTy, TypeTag Type>
        requires std::is_base_of_v<ID3D12DescriptorHeap, IUnknownTy>
    struct InterfaceMapper<IUnknownTy, Type>
    {
        using type = TypedD3D::D3D12::DescriptorHeap::DescriptorHeap_t<descriptorHeapType<Type>>;
    };

    template<class CpuDescriptorHandle, TypeTag Type>
        requires std::same_as<D3D12_CPU_DESCRIPTOR_HANDLE, CpuDescriptorHandle>    
    struct InterfaceMapper<CpuDescriptorHandle, Type>
    {
        using type = TypedD3D::D3D12::DescriptorHandle::CPU_t<descriptorHeapType<Type>>;
    };

    template<class GpuDescriptorHandle, TypeTag Type>
        requires std::same_as<D3D12_GPU_DESCRIPTOR_HANDLE, GpuDescriptorHandle>    
    struct InterfaceMapper<GpuDescriptorHandle, Type>
    {
        using type = TypedD3D::D3D12::DescriptorHandle::GPU_t<descriptorHeapType<Type>>;
    };
};