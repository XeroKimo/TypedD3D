#pragma once
#include "../Internal/ComWrapper.h"
#include "../D3D12Wrappers.h"
#include <d3d12.h>

namespace TypedD3D::Internal
{
    template<TypeTag Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    struct is_descriptor_heap_flag_compatible : std::false_type {};

    template<TypeTag Type>
    struct is_descriptor_heap_flag_compatible<Type, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> : std::true_type {};

    template<>
    struct is_descriptor_heap_flag_compatible<TypeTag::CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> : std::true_type {};

    template<>
    struct is_descriptor_heap_flag_compatible<TypeTag::Sampler, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> : std::true_type {};

    template<TypeTag Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    concept Descriptor_Heap_Flag_Compatible = is_descriptor_heap_flag_compatible<Type, HeapFlags>::value;

    template<TypeTag Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
        requires Is_Descriptor_Heap_Type<Type> && Descriptor_Heap_Flag_Compatible<Type, HeapFlags>
    class InterfaceWrapper<D3D12_CPU_DESCRIPTOR_HANDLE, Type, HeapFlags> : private D3D12_CPU_DESCRIPTOR_HANDLE
    {
    private:
        using CPU_DESCRIPTOR_HANDLE = InterfaceWrapper<D3D12_CPU_DESCRIPTOR_HANDLE, Type, HeapFlags>;

    public:
        static constexpr TypeTag tag_value = Type;
        static constexpr D3D12_DESCRIPTOR_HEAP_TYPE value = descriptorHeapType<Type>;

        using D3D12_CPU_DESCRIPTOR_HANDLE::ptr;

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

        D3D12_CPU_DESCRIPTOR_HANDLE& Get() { return *this; }
        const D3D12_CPU_DESCRIPTOR_HANDLE& Get() const { return *this; }
        explicit operator D3D12_CPU_DESCRIPTOR_HANDLE() { return *this; }
    };

    template<TypeTag Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
        requires Is_Descriptor_Heap_Type<Type> && Descriptor_Heap_Flag_Compatible<Type, HeapFlags>
    class InterfaceWrapper<D3D12_GPU_DESCRIPTOR_HANDLE, Type, HeapFlags> : private D3D12_GPU_DESCRIPTOR_HANDLE
    {
    private:
        using GPU_DESCRIPTOR_HANDLE = InterfaceWrapper<D3D12_GPU_DESCRIPTOR_HANDLE, Type, HeapFlags>;

    public:
        static constexpr TypeTag tag_value = Type;
        static constexpr D3D12_DESCRIPTOR_HEAP_TYPE value = descriptorHeapType<Type>;

        using D3D12_GPU_DESCRIPTOR_HANDLE::ptr;

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

        D3D12_GPU_DESCRIPTOR_HANDLE& Get() { return *this; }
        const D3D12_GPU_DESCRIPTOR_HANDLE& Get() const { return *this; }
        explicit operator D3D12_GPU_DESCRIPTOR_HANDLE() { return *this; }
    };

    namespace D3D12
    {
        template<TypeTag Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
        using DescriptorHeap_t = InterfaceWrapper<ID3D12DescriptorHeap, Type, HeapFlags>;

        namespace DescriptorHeap
        {
            template<TypeTag Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
            using CPU_DESCRIPTOR_HANDLE = InterfaceWrapper<D3D12_CPU_DESCRIPTOR_HANDLE, Type, HeapFlags>;

            template<TypeTag Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
            using GPU_DESCRIPTOR_HANDLE = InterfaceWrapper<D3D12_GPU_DESCRIPTOR_HANDLE, Type, HeapFlags>;

            static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<TypeTag::CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<TypeTag::RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<TypeTag::DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<TypeTag::Sampler, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

            static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<TypeTag::CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<TypeTag::RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<TypeTag::DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<TypeTag::Sampler, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

            template<class WrapperTy, D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
            class Interface
            {
            public:
                using CPU_DESCRIPTOR_HANDLE = CPU_DESCRIPTOR_HANDLE<tagValue<Type>, HeapFlags>;
                using GPU_DESCRIPTOR_HANDLE = GPU_DESCRIPTOR_HANDLE<tagValue<Type>, HeapFlags>;

            public:
                D3D12_DESCRIPTOR_HEAP_DESC GetDesc() { return InternalGet().GetDesc(); }
                CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() { return CPU_DESCRIPTOR_HANDLE(InternalGet().GetCPUDescriptorHandleForHeapStart()); }
                GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() { return GPU_DESCRIPTOR_HANDLE(InternalGet().GetGPUDescriptorHandleForHeapStart()); }

            private:
                ID3D12DescriptorHeap& InternalGet() { return  *static_cast<WrapperTy&>(*this).Get(); }
            };
        }
    }
    template<class DirectXClass, TypeTag Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
        requires std::is_base_of_v<ID3D12DescriptorHeap, DirectXClass> && Is_Descriptor_Heap_Type<Type> && Descriptor_Heap_Flag_Compatible<Type, HeapFlags>
    class InterfaceWrapper<DirectXClass, Type, HeapFlags> : public ComWrapper<DirectXClass>, private D3D12::DescriptorHeap::Interface<InterfaceWrapper<DirectXClass, Type, HeapFlags>, descriptorHeapType<Type>, HeapFlags>
    {
    private:
        using Interface = D3D12::DescriptorHeap::Interface<InterfaceWrapper<DirectXClass, Type, HeapFlags>, descriptorHeapType<Type>, HeapFlags>;
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
    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    using DescriptorHeap_t = TypedD3D::Internal::D3D12::DescriptorHeap_t<TypedD3D::Internal::tagValue<Type>, HeapFlags>;

    namespace DescriptorHeap
    {
        using Shader_Visible_CBV_SRV_UAV = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
        using Shader_Visible_Sampler = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
        using Shader_Visible_RTV = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
        using Shader_Visible_DSV = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;

        using CBV_SRV_UAV = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
        using Sampler = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
        using RTV = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
        using DSV = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    }
}