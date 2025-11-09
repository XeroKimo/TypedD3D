module;

#include <concepts>
#include <d3d12.h>

export module TypedD3D12:DescriptorHeap;
import TypedD3D.Shared;
import :Wrappers;

namespace TypedD3D::D3D12
{
    template<D3D12_DESCRIPTOR_HEAP_TYPE HeapType, D3D12_DESCRIPTOR_HEAP_FLAGS Flags>
    struct DescriptorHeapTypeToTraitMap;

    template<>
    struct DescriptorHeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> { template<class Ty> using type = CBV_SRV_UAVTag<Ty>; };

    template<>
    struct DescriptorHeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> { template<class Ty> using type = DSVTag<Ty>; };

    template<>
    struct DescriptorHeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> { template<class Ty> using type = RTVTag<Ty>; };

    template<>
    struct DescriptorHeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> { template<class Ty> using type = SamplerTag<Ty>; };

    template<>
    struct DescriptorHeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> { template<class Ty> using type = ShaderVisibleTag<CBV_SRV_UAVTag<Ty>>; };

    template<>
    struct DescriptorHeapTypeToTraitMap<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> { template<class Ty> using type = ShaderVisibleTag<SamplerTag<Ty>>; };

    export template<D3D12_DESCRIPTOR_HEAP_TYPE HeapType, D3D12_DESCRIPTOR_HEAP_FLAGS Flags, class Inner>
    using DescriptorHeapTypeToTrait = typename DescriptorHeapTypeToTraitMap<HeapType, Flags>::template type<Inner>;

    template<class Ty>
    using ShaderVisibleCBV_SRV_UAVTag = ShaderVisibleTag<CBV_SRV_UAVTag<Ty>>;

    template<class Ty>
    using ShaderVisibleSamplerTag = ShaderVisibleTag<SamplerTag<Ty>>;

    export template<class Ty>
    concept DescriptorHeapEnabledTag = std::same_as<Ty, CBV_SRV_UAVTag<InnerType<Ty>>>
        || std::same_as<Ty, DSVTag<InnerType<Ty>>>
        || std::same_as<Ty, RTVTag<InnerType<Ty>>>
        || std::same_as<Ty, SamplerTag<InnerType<Ty>>>
        || std::same_as<Ty, Untagged<InnerType<Ty>>>
        || std::same_as<Ty, ShaderVisibleCBV_SRV_UAVTag<InnerType<InnerType<Ty>>>>
        || std::same_as<Ty, ShaderVisibleSamplerTag<InnerType<InnerType<Ty>>>>;

    export template<DescriptorHeapEnabledTag Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapTraitToType = D3D12_DESCRIPTOR_HEAP_TYPE{ -1 };

    template<class Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapTraitToType<CBV_SRV_UAVTag<Ty>> = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    template<class Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapTraitToType<DSVTag<Ty>> = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

    template<class Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapTraitToType<RTVTag<Ty>> = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    template<class Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapTraitToType<SamplerTag<Ty>> = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;

    template<class Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapTraitToType<ShaderVisibleTag<CBV_SRV_UAVTag<Ty>>> = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    template<class Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeapTraitToType<ShaderVisibleTag<SamplerTag<Ty>>> = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;

    export template<DescriptorHeapEnabledTag Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_FLAGS DescriptorHeapTraitToFlag = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    template<class Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_FLAGS DescriptorHeapTraitToFlag<ShaderVisibleTag<CBV_SRV_UAVTag<Ty>>> = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    template<class Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_FLAGS DescriptorHeapTraitToFlag<ShaderVisibleTag<SamplerTag<Ty>>> = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
}

namespace TypedD3D
{
    template<template<class> class Outer, class Inner>
        requires D3D12::DescriptorHeapEnabledTag<Outer<Inner>> && std::same_as<InnerType<Inner>, D3D12_CPU_DESCRIPTOR_HANDLE>
    struct Trait<Outer<Inner>>
    {
        using inner_type = D3D12_CPU_DESCRIPTOR_HANDLE;

        using inner_tag = Inner;

        template<class NewInner>
        using trait_template = Outer<NewInner>;

        template<class NewInner>
        using ReplaceInnerType = trait_template<TypedD3D::ReplaceInnerType<Inner, NewInner>>;

        template<class Derived>
        struct Interface : TypedStructInterfaceBase<Derived>
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
    };

    template<template<class> class Outer, class Inner>
        requires D3D12::DescriptorHeapEnabledTag<Outer<Inner>>&& std::same_as<InnerType<Inner>, D3D12_GPU_DESCRIPTOR_HANDLE>
    struct Trait<Outer<Inner>>
    {
        using inner_type = D3D12_GPU_DESCRIPTOR_HANDLE;

        using inner_tag = Inner;

        template<class NewInner>
        using trait_template = Outer<NewInner>;

        template<class NewInner>
        using ReplaceInnerType = trait_template<TypedD3D::ReplaceInnerType<Inner, NewInner>>;

        template<class Derived>
        struct Interface : TypedStructInterfaceBase<Derived>
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
    };

    template<template<class> class Outer, class Inner>
        requires D3D12::DescriptorHeapEnabledTag<Outer<Inner>>&& std::same_as<InnerType<Inner>, ID3D12DescriptorHeap>
    struct Trait<Outer<Inner>>
    {
        using inner_type = ID3D12DescriptorHeap;

        using inner_tag = Inner;

        template<class NewInner>
        using trait_template = Outer<NewInner>;

        template<class NewInner>
        using ReplaceInnerType = trait_template<TypedD3D::ReplaceInnerType<Inner, NewInner>>;

        template<class Derived>
        class Interface : public InterfaceBase<ReplaceInnerType<Derived>>
        {
        public:
            D3D12_DESCRIPTOR_HEAP_DESC GetDesc() { return Self().GetDesc(); }
            TypedStruct<ReplaceInnerType<D3D12_CPU_DESCRIPTOR_HANDLE>> GetCPUDescriptorHandleForHeapStart()
            {
                return Self().GetCPUDescriptorHandleForHeapStart();
            }
            TypedStruct<ReplaceInnerType<D3D12_GPU_DESCRIPTOR_HANDLE>>  GetGPUDescriptorHandleForHeapStart()
            {
                return Self().GetGPUDescriptorHandleForHeapStart();
            }

        private:
            using InterfaceBase<ReplaceInnerType<Derived>>::Self;
            using InterfaceBase<ReplaceInnerType<Derived>>::ToDerived;
        };
    };

    template<>
    struct ShaderVisibleMapper<CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE>>
    {
        using type = TypedStruct<D3D12::ShaderVisibleCBV_SRV_UAVTag<D3D12_CPU_DESCRIPTOR_HANDLE>>;
    };

    template<>
    struct ShaderVisibleMapper<CBV_SRV_UAV<D3D12_GPU_DESCRIPTOR_HANDLE>>
    {
        using type = TypedStruct<ShaderVisibleTag<CBV_SRV_UAVTag<D3D12_GPU_DESCRIPTOR_HANDLE>>>;
    };

    template<>
    struct ShaderVisibleMapper<CBV_SRV_UAV<ID3D12DescriptorHeap>>
    {
        using type = StrongWrapper<ShaderVisibleTag<CBV_SRV_UAVTag<ID3D12DescriptorHeap>>>;
    };

    template<>
    struct ShaderVisibleMapper<Sampler<D3D12_CPU_DESCRIPTOR_HANDLE>>
    {
        using type = TypedStruct<ShaderVisibleTag<SamplerTag<D3D12_CPU_DESCRIPTOR_HANDLE>>>;
    };

    template<>
    struct ShaderVisibleMapper<Sampler<D3D12_GPU_DESCRIPTOR_HANDLE>>
    {
        using type = TypedStruct<ShaderVisibleTag<SamplerTag<D3D12_GPU_DESCRIPTOR_HANDLE>>>;
    };

    template<>
    struct ShaderVisibleMapper<Sampler<ID3D12DescriptorHeap>>
    {
        using type = StrongWrapper<ShaderVisibleTag<SamplerTag<ID3D12DescriptorHeap>>>;
    };
}