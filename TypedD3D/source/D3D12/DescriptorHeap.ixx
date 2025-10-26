module;

#include <concepts>
#include <d3d12.h>

export module TypedD3D12:DescriptorHeap;
import TypedD3D.Shared;
import :Wrappers;

namespace TypedD3D
{
    template<class Ty>
    struct ShaderVisibleCBV_SRV_UAVTrait;

    template<class Ty>
    struct ShaderVisibleSamplerTrait;
}

namespace TypedD3D::D3D12
{
    template<D3D12_DESCRIPTOR_HEAP_TYPE HeapType, D3D12_DESCRIPTOR_HEAP_FLAGS Flags>
    struct HeapTypeToTraitMap;

    template<>
    struct HeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> { template<class Ty> using type = CBV_SRV_UAVTraits<Ty>; };

    template<>
    struct HeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> { template<class Ty> using type = DSVTraits<Ty>; };

    template<>
    struct HeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> { template<class Ty> using type = RTVTraits<Ty>; };

    template<>
    struct HeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> { template<class Ty> using type = SamplerTraits<Ty>; };

    template<>
    struct HeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> { template<class Ty> using type = ShaderVisibleCBV_SRV_UAVTrait<Ty>; };

    template<>
    struct HeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> { template<class Ty> using type = ShaderVisibleSamplerTrait<Ty>; };

    template<D3D12_DESCRIPTOR_HEAP_TYPE HeapType, D3D12_DESCRIPTOR_HEAP_FLAGS Flags, class Inner>
    using HeapTypeToTrait = typename HeapTypeToTraitMap<HeapType, Flags>::template type<Inner>;


    template<class Derived, D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    struct DescriptorHandleInterface : TypedStructInterfaceBase<Derived>
    {
        auto& Ptr() { return Self().ptr; }
        const auto& Ptr() const { return Self().ptr; }

        auto Offset(INT64 offsetInDescriptors, UINT64 incrementSize)
        {
            auto copy = ToDerived();
            copy.Ptr() += incrementSize * offsetInDescriptors;
            return copy;
        }
        auto Offset(INT64 offset)
        {
            auto copy = ToDerived();
            copy.Ptr() += offset;
            return copy;
        }

    private:
        using TypedStructInterfaceBase<Derived>::Self;
        using TypedStructInterfaceBase<Derived>::ToDerived;
    };

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    struct DescriptorHeapTraits
    {
        using value_type = ID3D12DescriptorHeap;
        using pointer = ID3D12DescriptorHeap*;
        using const_pointer = const ID3D12DescriptorHeap*;
        using reference = ID3D12DescriptorHeap&;
        using const_reference = const ID3D12DescriptorHeap&;


        template<class Derived>
        class Interface : public InterfaceBase<HeapTypeToTrait<Type, HeapFlags, Derived>>
        {
        public:
            D3D12_DESCRIPTOR_HEAP_DESC GetDesc() { return Self().GetDesc(); }
            auto GetCPUDescriptorHandleForHeapStart();
            auto GetGPUDescriptorHandleForHeapStart();

        private:
            using InterfaceBase<HeapTypeToTrait<Type, HeapFlags, Derived>>::Self;
            using InterfaceBase<HeapTypeToTrait<Type, HeapFlags, Derived>>::ToDerived;
        };
    };
};

namespace TypedD3D
{
    template<>
    struct CBV_SRV_UAVTraits<D3D12_CPU_DESCRIPTOR_HANDLE> 
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct DSVTraits<D3D12_CPU_DESCRIPTOR_HANDLE> 
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct RTVTraits<D3D12_CPU_DESCRIPTOR_HANDLE>
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct SamplerTraits<D3D12_CPU_DESCRIPTOR_HANDLE>
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct ShaderVisibleCBV_SRV_UAVTrait<D3D12_CPU_DESCRIPTOR_HANDLE>
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };

    template<>
    struct ShaderVisibleSamplerTrait<D3D12_CPU_DESCRIPTOR_HANDLE>
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };




    template<>
    struct CBV_SRV_UAVTraits<D3D12_GPU_DESCRIPTOR_HANDLE> 
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct DSVTraits<D3D12_GPU_DESCRIPTOR_HANDLE>
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct RTVTraits<D3D12_GPU_DESCRIPTOR_HANDLE>
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct SamplerTraits<D3D12_GPU_DESCRIPTOR_HANDLE>
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    };

    template<>
    struct ShaderVisibleCBV_SRV_UAVTrait<D3D12_GPU_DESCRIPTOR_HANDLE>
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };

    template<>
    struct ShaderVisibleMapper<CBV_SRV_UAV<D3D12_GPU_DESCRIPTOR_HANDLE>>
    {
        using type = TypedStruct<ShaderVisibleCBV_SRV_UAVTrait<D3D12_GPU_DESCRIPTOR_HANDLE>>;
    };

    template<>
    struct ShaderVisibleSamplerTrait<D3D12_GPU_DESCRIPTOR_HANDLE>
    {
        template<class Derived>
        using Interface = D3D12::DescriptorHandleInterface<Derived, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };

    template<>
    struct ShaderVisibleMapper<Sampler<D3D12_GPU_DESCRIPTOR_HANDLE>>
    {
        using type = TypedStruct<ShaderVisibleSamplerTrait<D3D12_GPU_DESCRIPTOR_HANDLE>>;
    };



    template<>
    struct CBV_SRV_UAVTraits<ID3D12DescriptorHeap> : public D3D12::DescriptorHeapTraits<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> 
    {
    };

    template<>
    struct DSVTraits<ID3D12DescriptorHeap> : public D3D12::DescriptorHeapTraits<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>
    {
    };

    template<>
    struct RTVTraits<ID3D12DescriptorHeap> : public D3D12::DescriptorHeapTraits<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>
    {
    };

    template<>
    struct SamplerTraits<ID3D12DescriptorHeap> : public D3D12::DescriptorHeapTraits<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>
    {
    };

    template<>
    struct ShaderVisibleCBV_SRV_UAVTrait<ID3D12DescriptorHeap> : public D3D12::DescriptorHeapTraits<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>
    {

    };

    template<>
    struct ShaderVisibleMapper<CBV_SRV_UAV<ID3D12DescriptorHeap>>
    {
        using type = StrongWrapper<ShaderVisibleCBV_SRV_UAVTrait<ID3D12DescriptorHeap>>;
    };

    template<>
    struct ShaderVisibleSamplerTrait<ID3D12DescriptorHeap> : public D3D12::DescriptorHeapTraits<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>
    {

    };

    template<>
    struct ShaderVisibleMapper<Sampler<ID3D12DescriptorHeap>>
    {
        using type = StrongWrapper<ShaderVisibleSamplerTrait<ID3D12DescriptorHeap>>;
    };
}

namespace TypedD3D::D3D12
{

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    template<class Derived>
    auto DescriptorHeapTraits<Type, HeapFlags>::Interface<Derived>::GetGPUDescriptorHandleForHeapStart()
    {
        using GPU_DESCRIPTOR_HANDLE = TypedStruct<HeapTypeToTrait<Type, HeapFlags, D3D12_GPU_DESCRIPTOR_HANDLE>>;
        return GPU_DESCRIPTOR_HANDLE(Self().GetGPUDescriptorHandleForHeapStart()); 
    }

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    template<class Derived>
    auto DescriptorHeapTraits<Type, HeapFlags>::Interface<Derived>::GetCPUDescriptorHandleForHeapStart()
    {
        using CPU_DESCRIPTOR_HANDLE = TypedStruct<HeapTypeToTrait<Type, HeapFlags, D3D12_CPU_DESCRIPTOR_HANDLE>>;
        return CPU_DESCRIPTOR_HANDLE(Self().GetCPUDescriptorHandleForHeapStart()); 
    }
}