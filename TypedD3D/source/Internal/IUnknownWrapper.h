#pragma once
#include "source/Helpers/COMHelpers.h"
#include <wrl/client.h>
#include <type_traits>
#include <utility>
#include <concepts>

namespace TypedD3D::Internal
{
    template<class Ty, template<class DerivedSelf> class InterfaceTy>
    struct IUnknownWrapperImpl : public InterfaceTy<IUnknownWrapperImpl<Ty, InterfaceTy>>
    {
    public:
        using value_type = Ty;
        using pointer = Ty*;
        using reference = Ty&;

        using interface_type = InterfaceTy<IUnknownWrapperImpl<Ty, InterfaceTy>>;

    public:
        Microsoft::WRL::ComPtr<value_type> m_ptr;

    public:
        IUnknownWrapperImpl() = default;

        template<class U>
        IUnknownWrapperImpl(U* obj) : m_ptr{ obj }
        {

        }

        IUnknownWrapperImpl(const Microsoft::WRL::ComPtr<Ty>& other) : m_ptr{ other }
        {
        }

        IUnknownWrapperImpl(Microsoft::WRL::ComPtr<Ty>&& other) noexcept : m_ptr{ std::move(other) }
        {
        }

        template<class OtherTy>
        IUnknownWrapperImpl(const Microsoft::WRL::ComPtr<OtherTy>& other) : m_ptr{ other }
        {
        }

        template<class OtherTy>
        IUnknownWrapperImpl(Microsoft::WRL::ComPtr<OtherTy>&& other) noexcept : m_ptr{ std::move(other) }
        {
        }

    public:
        pointer Get() { return m_ptr.Get(); }
        pointer Get() const { return m_ptr.Get(); }
        interface_type* operator->() { return static_cast<interface_type*>(this); }
        const interface_type* operator->() const { return static_cast<const interface_type*>(this); }
    };


    template<class Ty, auto... Tags>
    struct Traits;


    template<class Ty>
    constexpr bool is_unknown_wrapper = false;

    template<class Ty, auto... Tags>
    class IUnknownWrapper : private IUnknownWrapperImpl<Ty, typename Traits<Ty, Tags...>::Interface>
    {
        template<class OtherTy, auto... OtherTags>
        friend class IUnknownWrapper;


    private:
        using Base = IUnknownWrapperImpl<Ty, typename Traits<Ty, Tags...>::Interface>;
        using Base::m_ptr;

    public:
        using value_type = Base::value_type;
        using pointer = Base::pointer;
        using reference = Base::reference;

        using interface_type = Base::interface_type;

    public:
        using traits_type = Traits<Ty, Tags...>;

    public:
        IUnknownWrapper() = default;
        IUnknownWrapper(const IUnknownWrapper& other) = default;
        IUnknownWrapper(IUnknownWrapper&& other) noexcept = default;

        template<class OtherTy>
        IUnknownWrapper(const IUnknownWrapper<OtherTy, Tags...>& other) : Base{ other.m_ptr }
        {
        }

        template<class OtherTy>
        IUnknownWrapper(IUnknownWrapper<OtherTy, Tags...>&& other) noexcept : Base{ std::move(other.m_ptr) }
        {
        }

        IUnknownWrapper(std::nullptr_t) {};

        template<class U>
        IUnknownWrapper(U* obj) : Base{ obj }
        {
        }

        IUnknownWrapper(const Microsoft::WRL::ComPtr<Ty>& other) : Base{ other }
        {
        }

        IUnknownWrapper(Microsoft::WRL::ComPtr<Ty>&& other) noexcept : Base{ std::move(other) }
        {
        }

        template<class OtherTy>
        IUnknownWrapper(const Microsoft::WRL::ComPtr<OtherTy>& other) : Base{ other }
        {
        }

        template<class OtherTy>
        IUnknownWrapper(Microsoft::WRL::ComPtr<OtherTy>&& other) noexcept : Base{ std::move(other) }
        {
        }

    public:
        IUnknownWrapper& operator=(IUnknownWrapper& other) = default;
        IUnknownWrapper& operator=(IUnknownWrapper&& other) noexcept = default;

        IUnknownWrapper& operator=(std::nullptr_t)
        {
            m_ptr = nullptr;
            return *this;
        }

        template<class OtherTy>
        IUnknownWrapper& operator=(IUnknownWrapper<OtherTy, Tags...>& other)
        {
            m_ptr = other;
            return *this;
        }

        template<class OtherTy>
        IUnknownWrapper& operator=(IUnknownWrapper<OtherTy, Tags...>&& other) noexcept
        {
            m_ptr = (std::move(other));
            return *this;
        }

    public:
        friend bool operator==(const IUnknownWrapper& lh, std::nullptr_t)
        {
            return lh.m_ptr == nullptr;
        }
        friend bool operator!=(const IUnknownWrapper& lh, std::nullptr_t)
        {
            return lh.m_ptr != nullptr;
        }

        template<class OtherTy>
        friend bool operator==(const IUnknownWrapper& lh, const IUnknownWrapper<OtherTy, Tags...>& rh)
        {
            return lh.m_ptr == rh.m_ptr;
        }
        template<class OtherTy>
        friend bool operator!=(const IUnknownWrapper& lh, const IUnknownWrapper<OtherTy, Tags...>& rh)
        {
            return lh.m_ptr != rh.m_ptr;
        }

        using Base::operator->;

    public:
        pointer Detach() { return m_ptr.Detach(); }

        void Attach(pointer p)
        {
            m_ptr.Attach(p);
        }

        using Base::Get;
        Microsoft::WRL::ComPtr<value_type> AsComPtr() const { return m_ptr; }

        template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... CastTags>
        friend Internal::IUnknownWrapper<DerivedTy, CastTags...> Cast(const Internal::IUnknownWrapper<Ty, CastTags...>& other) noexcept;

        template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... CastTags>
        friend Internal::IUnknownWrapper<DerivedTy, CastTags...> Cast(Internal::IUnknownWrapper<Ty, CastTags...>&& other) noexcept;

        template<class DerivedTy, std::derived_from<IUnknown> CastTy, auto... CastTags>
            requires Internal::is_unknown_wrapper<DerivedTy>
        friend DerivedTy Cast(const Internal::IUnknownWrapper<CastTy, CastTags...>& other) noexcept;

        template<class DerivedTy, std::derived_from<IUnknown> CastTy, auto... CastTags>
            requires Internal::is_unknown_wrapper<DerivedTy>
        friend DerivedTy Cast(Internal::IUnknownWrapper<CastTy, CastTags...>&& other) noexcept;
    };

    template<class Ty, auto... Tags>
    constexpr bool is_unknown_wrapper<IUnknownWrapper<Ty, Tags...>> = true;


    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... CastTags>
    Internal::IUnknownWrapper<DerivedTy, CastTags...> Cast(const Internal::IUnknownWrapper<Ty, CastTags...>& other) noexcept
    {
        return IUnknownWrapper<DerivedTy, CastTags...>(Helpers::COM::Cast<DerivedTy>(other.m_ptr));
    }

    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... CastTags>
    Internal::IUnknownWrapper<DerivedTy, CastTags...> Cast(Internal::IUnknownWrapper<Ty, CastTags...>&& other) noexcept
    {
        return IUnknownWrapper<DerivedTy, CastTags...>(Helpers::COM::Cast<DerivedTy>(std::move(other.m_ptr)));
    }

    template<class DerivedTy, std::derived_from<IUnknown> CastTy, auto... CastTags>
        requires Internal::is_unknown_wrapper<DerivedTy>
    DerivedTy Cast(const Internal::IUnknownWrapper<CastTy, CastTags...>& other) noexcept
    {
        return DerivedTy(Helpers::COM::Cast<typename DerivedTy::value_type>(other.m_ptr));
    }

    template<class DerivedTy, std::derived_from<IUnknown> CastTy, auto... CastTags>
        requires Internal::is_unknown_wrapper<DerivedTy>
    DerivedTy Cast(Internal::IUnknownWrapper<CastTy, CastTags...>&& other) noexcept
    {
        return DerivedTy(Helpers::COM::Cast<typename DerivedTy::value_type>(std::move(other.m_ptr)));
    }
}

namespace TypedD3D
{
    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
    Internal::IUnknownWrapper<DerivedTy, Tags...> Cast(const Internal::IUnknownWrapper<Ty, Tags...>& other) noexcept
    {
        return Internal::Cast<DerivedTy>(other);
    }

    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
    Internal::IUnknownWrapper<DerivedTy, Tags...> Cast(Internal::IUnknownWrapper<Ty, Tags...>&& other) noexcept
    {
        return Internal::Cast<DerivedTy>(std::move(other));
    }

    template<class DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
        requires Internal::is_unknown_wrapper<DerivedTy>
    DerivedTy Cast(const Internal::IUnknownWrapper<Ty, Tags...>& other) noexcept
    {
        return Internal::Cast<DerivedTy>(other);
    }

    template<class DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
        requires Internal::is_unknown_wrapper<DerivedTy>
    DerivedTy Cast(Internal::IUnknownWrapper<Ty, Tags...>&& other) noexcept
    {
        return Internal::Cast<DerivedTy>(std::move(other));
    }
}