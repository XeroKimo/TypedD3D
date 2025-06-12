module;

#include <concepts>
#include <d3d12.h>

export module TypedD3D12:DescriptorHeap;
import TypedD3D.Shared;
import :Wrappers;

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
        using ::D3D12_CPU_DESCRIPTOR_HANDLE::ptr;

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

    template<D3D12_DESCRIPTOR_HEAP_TYPE HeapType>
    struct HeapTypeToTrait;

    template<>
    struct HeapTypeToTrait<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV> { template<class Ty> using type = CBV_SRV_UAVTraits<Ty>; };

    template<>
    struct HeapTypeToTrait<D3D12_DESCRIPTOR_HEAP_TYPE_DSV> { template<class Ty> using type = DSVTraits<Ty>; };

    template<>
    struct HeapTypeToTrait<D3D12_DESCRIPTOR_HEAP_TYPE_RTV> { template<class Ty> using type = RTVTraits<Ty>; };

    template<>
    struct HeapTypeToTrait<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER> { template<class Ty> using type = SamplerTraits<Ty>; };

    template<template<class> class Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE TraitToHeapType;

    template<>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE TraitToHeapType<CBV_SRV_UAVTraits> = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    template<>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE TraitToHeapType<DSVTraits> = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

    template<>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE TraitToHeapType<RTVTraits> = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    template<>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE TraitToHeapType<SamplerTraits> = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;

    template<D3D12_DESCRIPTOR_HEAP_TYPE HeapType>
    struct HeapTypeToWrapper;

    template<>
    struct HeapTypeToWrapper<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV> { template<class Ty> using type = CBV_SRV_UAV<Ty>; };

    template<>
    struct HeapTypeToWrapper<D3D12_DESCRIPTOR_HEAP_TYPE_DSV> { template<class Ty> using type = DSV<Ty>; };

    template<>
    struct HeapTypeToWrapper<D3D12_DESCRIPTOR_HEAP_TYPE_RTV> { template<class Ty> using type = RTV<Ty>; };

    template<>
    struct HeapTypeToWrapper<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER> { template<class Ty> using type = Sampler<Ty>; };

    template<template<class> class Ty>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE WrapperToHeapType;

    template<>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE WrapperToHeapType<CBV_SRV_UAV> = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

    template<>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE WrapperToHeapType<DSV> = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

    template<>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE WrapperToHeapType<RTV> = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    template<>
    constexpr D3D12_DESCRIPTOR_HEAP_TYPE WrapperToHeapType<Sampler> = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;

    template<D3D12_DESCRIPTOR_HEAP_TYPE HeapType, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    struct HeapTraitMapper;    
    
    template<D3D12_DESCRIPTOR_HEAP_TYPE HeapType>
    struct HeapTraitMapper<HeapType, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>
    {
        template<class Ty>
        using type = HeapTypeToTrait<HeapType>::template type<Ty>;
    };
    
    template<D3D12_DESCRIPTOR_HEAP_TYPE HeapType>
    struct HeapTraitMapper<HeapType, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>
    {
        template<class Ty>
        using type = ShaderVisibleTraits<typename HeapTypeToWrapper<HeapType>::template type<Ty>>;
    };

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    using GetTraitType = HeapTraitMapper<Type, HeapFlags>::type;

    template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    struct DescriptorHeapTraits
    {
        using value_type = ID3D12DescriptorHeap;
        using pointer = ID3D12DescriptorHeap*;
        using const_pointer = const ID3D12DescriptorHeap*;
        using reference = ID3D12DescriptorHeap&;
        using const_reference = const ID3D12DescriptorHeap&;
        using CPU_DESCRIPTOR_HANDLE = CPU_DESCRIPTOR_HANDLE<Type, HeapFlags>;
        using GPU_DESCRIPTOR_HANDLE = GPU_DESCRIPTOR_HANDLE<Type, HeapFlags>;

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

    template<>
    struct CBV_SRV_UAVTraits<ID3D12DescriptorHeap> : DescriptorHeapTraits<TraitToHeapType<CBV_SRV_UAVTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> {};

    template<>
    struct DSVTraits<ID3D12DescriptorHeap> : DescriptorHeapTraits<TraitToHeapType<DSVTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> {};

    template<>
    struct RTVTraits<ID3D12DescriptorHeap> : DescriptorHeapTraits<TraitToHeapType<RTVTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> {};

    template<>
    struct SamplerTraits<ID3D12DescriptorHeap> : DescriptorHeapTraits<TraitToHeapType<SamplerTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE> {};

    template<>
    struct ShaderVisibleTraits<CBV_SRV_UAV<ID3D12DescriptorHeap>> : DescriptorHeapTraits<TraitToHeapType<CBV_SRV_UAVTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> {};

    template<>
    struct ShaderVisibleTraits<Sampler<ID3D12DescriptorHeap>> : DescriptorHeapTraits<TraitToHeapType<SamplerTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE> {};

    export template<D3D12_DESCRIPTOR_HEAP_TYPE Type, D3D12_DESCRIPTOR_HEAP_FLAGS HeapFlags>
    using DescriptorHeap_t = IUnknownWrapper<ID3D12DescriptorHeap, HeapTraitMapper<Type, HeapFlags>::template type>;

    template<>
    struct CBV_SRV_UAVMapper<::D3D12_CPU_DESCRIPTOR_HANDLE> { using type = CPU_DESCRIPTOR_HANDLE<TraitToHeapType<CBV_SRV_UAVTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>; };

    template<>
    struct DSVMapper<::D3D12_CPU_DESCRIPTOR_HANDLE> { using type = CPU_DESCRIPTOR_HANDLE<TraitToHeapType<DSVTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>; };

    template<>
    struct RTVMapper<::D3D12_CPU_DESCRIPTOR_HANDLE> { using type = CPU_DESCRIPTOR_HANDLE<TraitToHeapType<RTVTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>; };

    template<>
    struct SamplerMapper<::D3D12_CPU_DESCRIPTOR_HANDLE> { using type = CPU_DESCRIPTOR_HANDLE<TraitToHeapType<SamplerTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>; };

    template<>
    struct CBV_SRV_UAVMapper<::D3D12_GPU_DESCRIPTOR_HANDLE> { using type = GPU_DESCRIPTOR_HANDLE<TraitToHeapType<CBV_SRV_UAVTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>; };

    template<>
    struct DSVMapper<::D3D12_GPU_DESCRIPTOR_HANDLE> { using type = GPU_DESCRIPTOR_HANDLE<TraitToHeapType<DSVTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>; };

    template<>
    struct RTVMapper<::D3D12_GPU_DESCRIPTOR_HANDLE> { using type = GPU_DESCRIPTOR_HANDLE<TraitToHeapType<RTVTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>; };

    template<>
    struct SamplerMapper<::D3D12_GPU_DESCRIPTOR_HANDLE> { using type = GPU_DESCRIPTOR_HANDLE<TraitToHeapType<SamplerTraits>, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>; };

    template<>
    struct ShaderVisibleMapper<CBV_SRV_UAV<::D3D12_GPU_DESCRIPTOR_HANDLE>>
    {
        using type = GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };

    template<>
    struct ShaderVisibleMapper<Sampler<::D3D12_GPU_DESCRIPTOR_HANDLE>>
    {
        using type = GPU_DESCRIPTOR_HANDLE<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
    };




	static_assert(std::same_as<ShaderVisible<Sampler<ID3D12DescriptorHeap>>, IUnknownWrapperImpl<ID3D12DescriptorHeap, ShaderVisibleTraits<Sampler<ID3D12DescriptorHeap>>>>);
	static_assert(std::same_as<typename ShaderVisible<Sampler<ID3D12DescriptorHeap>>::traits_type, ShaderVisibleTraits<Sampler<ID3D12DescriptorHeap>>>);

	static_assert(std::convertible_to<CBV_SRV_UAVTraits<ID3D12DescriptorHeap>, HeapTraitMapper<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>::template type<ID3D12DescriptorHeap>>);
	static_assert(std::same_as<DSVTraits<ID3D12DescriptorHeap>, HeapTraitMapper<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>::template type<ID3D12DescriptorHeap>>);
	static_assert(std::same_as<RTVTraits<ID3D12DescriptorHeap>, HeapTraitMapper<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>::template type<ID3D12DescriptorHeap>>);
	static_assert(std::same_as<SamplerTraits<ID3D12DescriptorHeap>, HeapTraitMapper<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>::template type<ID3D12DescriptorHeap>>);

	static_assert(std::same_as<ShaderVisibleTraits<CBV_SRV_UAV<ID3D12DescriptorHeap>>, HeapTraitMapper<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>::template type<ID3D12DescriptorHeap>>);
	static_assert(std::same_as<ShaderVisibleTraits<Sampler<ID3D12DescriptorHeap>>, HeapTraitMapper<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>::template type<ID3D12DescriptorHeap>>);


    namespace Aliases
    {
        export using D3D12ShaderVisibleCBV_SRV_UAVDescriptorHeap = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
        export using D3D12ShaderVisibleSamplerDescriptorHeap = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
        export using D3D12ShaderVisibleRTVDescriptorHeap = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;
        export using D3D12ShaderVisibleDSVDescriptorHeap = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE>;

        export using D3D12CBV_SRV_UAVDescriptorHeap = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
        export using D3D12SamplerDescriptorHeap = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
        export using D3D12RTVDescriptorHeap = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
        export using D3D12DSVDescriptorHeap = DescriptorHeap_t<D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE>;
    }
}