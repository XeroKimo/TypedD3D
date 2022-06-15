#pragma once
#include "Wrappers.h"
#include <d3d12.h>

namespace TypedD3D
{
    namespace Internal
    {
        template<class Ty>
        struct DirectMapper;

        template<class Ty>
        struct ComputeMapper;

        template<class Ty>
        struct CopyMapper;

        template<class Ty>
        struct BundleMapper;

        template<class Ty>
        struct CBV_SRV_UAVMapper;

        template<class Ty>
        struct SamplerMapper;

        template<class Ty>
        struct RTVMapper;

        template<class Ty>
        struct DSVMapper;

        template<class Ty>
        struct GraphicsMapper;

        template<class Ty>
        struct ShaderVisibleMapper;

        struct direct_cast_tag {};
        struct compute_cast_tag {};
        struct copy_cast_tag {};
        struct bundle_cast_tag {};
    }

    template<class Ty>
    using Direct = Internal::DirectMapper<Ty>::type;

    template<class Ty>
    using Compute = Internal::ComputeMapper<Ty>::type;

    template<class Ty>
    using Copy = Internal::CopyMapper<Ty>::type;

    template<class Ty>
    using Bundle = Internal::BundleMapper<Ty>::type;

    template<class Ty>
    using CBV_SRV_UAV = Internal::CBV_SRV_UAVMapper<Ty>::type;

    template<class Ty>
    using Sampler = Internal::SamplerMapper<Ty>::type;

    template<class Ty>
    using RTV = Internal::RTVMapper<Ty>::type;

    template<class Ty>
    using DSV = Internal::DSVMapper<Ty>::type;

    template<class Ty>
    using Graphics = Internal::GraphicsMapper<Ty>::type;

    template<class Ty>
    using ShaderVisible = Internal::ShaderVisibleMapper<Ty>::type;

    template<std::derived_from<IUnknown> OtherTy, class Ty>
        requires std::same_as<typename Ty::cast_tag, Internal::direct_cast_tag>
    Direct<OtherTy> Cast(Ty& other) noexcept
    {
        return Direct<OtherTy>(Internal::Cast<OtherTy, typename Direct<OtherTy>::interface_type>(other));
    }

    template<std::derived_from<IUnknown> OtherTy, class Ty>
        requires std::same_as<typename Ty::cast_tag, Internal::direct_cast_tag>
    Direct<OtherTy> Cast(Ty && other) noexcept
    {
        return Direct<OtherTy>(Internal::Cast<OtherTy, typename Direct<OtherTy>::interface_type>(std::move(other)));
    }

    //template<class OtherTy, template<class, D3D12_COMMAND_LIST_TYPE> class Object, class Ty>
    //Object<OtherTy, D3D12_COMMAND_LIST_TYPE_DIRECT> Cast(const Object<Ty, D3D12_COMMAND_LIST_TYPE_DIRECT>& other) noexcept
    //{
    //    return Object<OtherTy, D3D12_COMMAND_LIST_TYPE_DIRECT>(Internal::Cast<OtherTy, typename Object<OtherTy, D3D12_COMMAND_LIST_TYPE_DIRECT>::interface_type>(other));
    //}

    //template<class OtherTy, template<class, D3D12_COMMAND_LIST_TYPE> class Object, class Ty>
    //Object<OtherTy, D3D12_COMMAND_LIST_TYPE_DIRECT> Cast(Object<Ty, D3D12_COMMAND_LIST_TYPE_DIRECT>&& other) noexcept
    //{
    //    return Object<OtherTy, D3D12_COMMAND_LIST_TYPE_DIRECT>(Internal::Cast<OtherTy, typename Object<OtherTy, D3D12_COMMAND_LIST_TYPE_DIRECT>::interface_type>(std::move(other)));
    //}

    template<std::derived_from<IUnknown> OtherTy, class Ty>
        requires std::same_as<typename Ty::cast_tag, Internal::compute_cast_tag>
    Compute<OtherTy> Cast(Ty& other) noexcept
    {
        return Compute<OtherTy>(Internal::Cast<OtherTy, typename Compute<OtherTy>::interface_type>(other));
    }

    template<std::derived_from<IUnknown> OtherTy, class Ty>
        requires std::same_as<typename Ty::cast_tag, Internal::compute_cast_tag>
    Compute<OtherTy> Cast(Ty&& other) noexcept
    {
        return Compute<OtherTy>(Internal::Cast<OtherTy, typename Compute<OtherTy>::interface_type>(std::move(other)));
    }

    template<std::derived_from<IUnknown> OtherTy, class Ty>
        requires std::same_as<typename Ty::cast_tag, Internal::copy_cast_tag>
    Copy<OtherTy> Cast(Ty& other) noexcept
    {
        return Copy<OtherTy>(Internal::Cast<OtherTy, typename Copy<OtherTy>::interface_type>(other));
    }

    template<std::derived_from<IUnknown> OtherTy, class Ty>
        requires std::same_as<typename Ty::cast_tag, Internal::copy_cast_tag>
    Copy<OtherTy> Cast(Ty&& other) noexcept
    {
        return Copy<OtherTy>(Internal::Cast<OtherTy, typename Copy<OtherTy>::interface_type>(std::move(other)));
    }

    template<std::derived_from<IUnknown> OtherTy, class Ty>
        requires std::same_as<typename Ty::cast_tag, Internal::bundle_cast_tag>
    Bundle<OtherTy> Cast(Ty& other) noexcept
    {
        return Bundle<OtherTy>(Internal::Cast<OtherTy, typename Bundle<OtherTy>::interface_type>(other));
    }

    template<std::derived_from<IUnknown> OtherTy, class Ty>
        requires std::same_as<typename Ty::cast_tag, Internal::bundle_cast_tag>
    Bundle<OtherTy> Cast(Ty&& other) noexcept
    {
        return Bundle<OtherTy>(Internal::Cast<OtherTy, typename Bundle<OtherTy>::interface_type>(std::move(other)));
    }

}