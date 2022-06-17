#pragma once
#include "source/Internal/IUnknownWrapper.h"
#include "source/D3D12Wrappers.h"
#include <d3d12.h>


namespace TypedD3D::D3D12
{
    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    struct GPU_DESCRIPTOR_HANDLE : private D3D12_GPU_DESCRIPTOR_HANDLE
    {
        using D3D12_GPU_DESCRIPTOR_HANDLE::ptr;

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

        D3D12_GPU_DESCRIPTOR_HANDLE& Get() { return *this; }
        const D3D12_GPU_DESCRIPTOR_HANDLE& Get() const { return *this; }
        explicit operator D3D12_GPU_DESCRIPTOR_HANDLE() { return *this; }
    };

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    struct CPU_DESCRIPTOR_HANDLE : private D3D12_CPU_DESCRIPTOR_HANDLE
    {
        using D3D12_CPU_DESCRIPTOR_HANDLE::ptr;

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

        D3D12_CPU_DESCRIPTOR_HANDLE& Get() { return *this; }
        const D3D12_CPU_DESCRIPTOR_HANDLE& Get() const { return *this; }
        explicit operator D3D12_CPU_DESCRIPTOR_HANDLE() { return *this; }
    };

    static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
    static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
    static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
    static_assert(sizeof(CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

    static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
    static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
    static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
    static_assert(sizeof(GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>) == sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
};

namespace TypedD3D::Internal
{
    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    struct is_descriptor_heap_flag_compatible : std::false_type {};

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type>
    struct is_descriptor_heap_flag_compatible<Type, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> : std::true_type {};

    template<>
    struct is_descriptor_heap_flag_compatible<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> : std::true_type {};

    template<>
    struct is_descriptor_heap_flag_compatible<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> : std::true_type {};

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    concept Descriptor_Heap_Flag_Compatible = is_descriptor_heap_flag_compatible<Type, HeapFlags>::value;

    namespace D3D12
    {
        template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
        using DescriptorHeap_t = IUnknownWrapper<ID3D12DescriptorHeap, Type, HeapFlags>;

        template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
        using CPU_DESCRIPTOR_HANDLE = ::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<Type, HeapFlags>;

        template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
        using GPU_DESCRIPTOR_HANDLE = ::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<Type, HeapFlags>;

        namespace DescriptorHeap
        {
        }
    }

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    struct Traits<ID3D12DescriptorHeap, Type, HeapFlags>
    {
        using value_type = ID3D12DescriptorHeap;
        using pointer = ID3D12DescriptorHeap*;
        using const_pointer = const ID3D12DescriptorHeap*;
        using reference = ID3D12DescriptorHeap&;
        using const_reference = const ID3D12DescriptorHeap&;
        using CPU_DESCRIPTOR_HANDLE = ::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<Type, HeapFlags>;
        using GPU_DESCRIPTOR_HANDLE = ::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<Type, HeapFlags>;

        template<class DerivedSelf>
        class Interface
        {
        private:
            using derived_self = DerivedSelf;

        public:
            D3D12_DESCRIPTOR_HEAP_DESC GetDesc() { return Get().GetDesc(); }
            CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() { return CPU_DESCRIPTOR_HANDLE(Get().GetCPUDescriptorHandleForHeapStart()); }
            GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandleForHeapStart() { return GPU_DESCRIPTOR_HANDLE(Get().GetGPUDescriptorHandleForHeapStart()); }

        private:
            derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
            reference Get() { return *ToDerived().derived_self::Get(); }
        };
    };

    //template<>
    //struct ShaderVisibleMapper<ID3D12DescriptorHeap>
    //{
    //    using type = std::void_t<D3D12::DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>>;
    //};

    template<>
    struct CBV_SRV_UAVMapper<ID3D12DescriptorHeap>
    {
        using type = D3D12::DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct SamplerMapper<ID3D12DescriptorHeap>
    {
        using type = D3D12::DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    //template<>
    //struct CBV_SRV_UAVMapper<ShaderVisible<ID3D12DescriptorHeap>>
    //{
    //    using type = D3D12::DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    //};

    //template<>
    //struct SamplerMapper<ShaderVisible<ID3D12DescriptorHeap>>
    //{
    //    using type = D3D12::DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    //};

    template<>
    struct RTVMapper<ID3D12DescriptorHeap>
    {
        using type = D3D12::DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct DSVMapper<ID3D12DescriptorHeap>
    {
        using type = D3D12::DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct ShaderVisibleMapper<CBV_SRV_UAV<ID3D12DescriptorHeap>>
    {
        using type = D3D12::DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };

    template<>
    struct ShaderVisibleMapper<Sampler<ID3D12DescriptorHeap>>
    {
        using type = D3D12::DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };

    //template<>
    //struct ShaderVisibleMapper<D3D12_CPU_DESCRIPTOR_HANDLE>
    //{
    //    using type = std::void_t<::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>>;
    //};

    template<>
    struct CBV_SRV_UAVMapper<D3D12_CPU_DESCRIPTOR_HANDLE>
    {
        using type = ::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct SamplerMapper<D3D12_CPU_DESCRIPTOR_HANDLE>
    {
        using type = ::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    //template<>
    //struct CBV_SRV_UAVMapper<ShaderVisible<D3D12_CPU_DESCRIPTOR_HANDLE>>
    //{
    //    using type = ::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    //};

    //template<>
    //struct SamplerMapper<ShaderVisible<D3D12_CPU_DESCRIPTOR_HANDLE>>
    //{
    //    using type = ::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    //};

    template<>
    struct RTVMapper<D3D12_CPU_DESCRIPTOR_HANDLE>
    {
        using type = ::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct DSVMapper<D3D12_CPU_DESCRIPTOR_HANDLE>
    {
        using type = ::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct ShaderVisibleMapper<CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE>>
    {
        using type = ::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };

    template<>
    struct ShaderVisibleMapper<Sampler<D3D12_CPU_DESCRIPTOR_HANDLE>>
    {
        using type = ::TypedD3D::D3D12::CPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };

    //template<>
    //struct ShaderVisibleMapper<D3D12_GPU_DESCRIPTOR_HANDLE>
    //{
    //    using type = std::void_t<::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>>;
    //};

    template<>
    struct CBV_SRV_UAVMapper<D3D12_GPU_DESCRIPTOR_HANDLE>
    {
        using type = ::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct SamplerMapper<D3D12_GPU_DESCRIPTOR_HANDLE>
    {
        using type = ::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    //template<>
    //struct CBV_SRV_UAVMapper<ShaderVisible<D3D12_GPU_DESCRIPTOR_HANDLE>>
    //{
    //    using type = ::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    //};

    //template<>
    //struct SamplerMapper<ShaderVisible<D3D12_GPU_DESCRIPTOR_HANDLE>>
    //{
    //    using type = ::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    //};

    template<>
    struct RTVMapper<D3D12_GPU_DESCRIPTOR_HANDLE>
    {
        using type = ::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct DSVMapper<D3D12_GPU_DESCRIPTOR_HANDLE>
    {
        using type = ::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct ShaderVisibleMapper<CBV_SRV_UAV<D3D12_GPU_DESCRIPTOR_HANDLE>>
    {
        using type = ::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };

    template<>
    struct ShaderVisibleMapper<Sampler<D3D12_GPU_DESCRIPTOR_HANDLE>>
    {
        using type = ::TypedD3D::D3D12::GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };
}

namespace TypedD3D::D3D12
{
    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    using DescriptorHeap_t = Internal::D3D12::DescriptorHeap_t<Type, HeapFlags>;

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