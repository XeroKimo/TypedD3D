#pragma once
#include <d3d12.h>
#include "ComWrapper.h"

namespace TypedD3D::D3D12::DescriptorHandle
{
    namespace Internal
    {
        template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
        class CPU_DESCRIPTOR_HANDLE
        {
        private:
            D3D12_CPU_DESCRIPTOR_HANDLE m_handle;

        public:
            CPU_DESCRIPTOR_HANDLE() = default;
            CPU_DESCRIPTOR_HANDLE(D3D12_CPU_DESCRIPTOR_HANDLE handle) :
                m_handle(handle)
            {

            }

        public:
            SIZE_T& Ptr() { return m_handle.ptr; }
            const SIZE_T& Ptr() const { return m_handle.ptr; }
            const D3D12_CPU_DESCRIPTOR_HANDLE& Data() const { return m_handle; }
        };

        template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
        class GPU_DESCRIPTOR_HANDLE
        {
        private:
            D3D12_GPU_DESCRIPTOR_HANDLE m_handle;

        public:
            GPU_DESCRIPTOR_HANDLE() = default;
            GPU_DESCRIPTOR_HANDLE(D3D12_GPU_DESCRIPTOR_HANDLE handle) :
                m_handle(handle)
            {

            }

        public:
            SIZE_T& Ptr() { return m_handle.ptr; }
            const SIZE_T& Ptr() const { return m_handle.ptr; }
            const D3D12_GPU_DESCRIPTOR_HANDLE& Data() const { return m_handle; }
        };
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
            D3D12_DESCRIPTOR_HEAP_DESC GetDesc() { return InternalGet().GetDesc(); }
            DescriptorHandle::CPU_t<Type> GetCPUDescriptorHandleForHeapStart() { return DescriptorHandle::CPU_t<Type>(InternalGet().GetCPUDescriptorHandleForHeapStart()); }
            DescriptorHandle::GPU_t<Type> GetGPUDescriptorHandleForHeapStart() { return DescriptorHandle::GPU_t<Type>(InternalGet().GetGPUDescriptorHandleForHeapStart()); }

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