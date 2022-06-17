#pragma once
#include "source/Helpers/COMHelpers.h"
#include <wrl/client.h>
#include <type_traits>
#include <utility>
#include <concepts>

namespace TypedD3D::Internal
{
    template<class Ty, template<class DerivedSelf> class InterfaceTy>
    class IUnknownWrapperImpl
    {
    private:
        template<class OtherTy, template<class> class OtherInterfaceTy>
        friend class IUnknownWrapperImpl;

    public:
        using value_type = Ty;
        using pointer = Ty*;
        using reference = Ty&;

        template<class DerivedSelf>
        using interface_type = InterfaceTy<DerivedSelf>;

    private:
        struct CompressedPair : public InterfaceTy<CompressedPair>
        {
        public:
            using interface_value_type = InterfaceTy<CompressedPair>;
            using interface_pointer = InterfaceTy<CompressedPair>*;

            Microsoft::WRL::ComPtr<value_type> m_second = nullptr;
            
            interface_pointer First() { return static_cast<interface_pointer>(this); }
            Microsoft::WRL::ComPtr<value_type>& Second() { return m_second; }
            const Microsoft::WRL::ComPtr<value_type>& Second() const { return m_second; }

            pointer Get() { return m_second.Get(); }
            pointer Get() const { return m_second.Get(); }
        };

    private:
        CompressedPair m_pair;

    public:
        IUnknownWrapperImpl() = default;
        IUnknownWrapperImpl(std::nullptr_t) {};

        template<class U>
        IUnknownWrapperImpl(U* obj) : m_pair{ .m_second = obj }
        {
        }

        IUnknownWrapperImpl(const IUnknownWrapperImpl& other) : m_pair{ .m_second = other.m_pair.m_second }
        {
        }

        IUnknownWrapperImpl(IUnknownWrapperImpl&& other) noexcept : m_pair{ .m_second = std::move(other.Detach()) }
        {
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
        IUnknownWrapperImpl(const IUnknownWrapperImpl<OtherTy, OtherInterfaceTy>& other) : m_pair{ .m_second = other.m_pair.m_second }
        {
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
        IUnknownWrapperImpl(IUnknownWrapperImpl<OtherTy, OtherInterfaceTy>&& other) noexcept : m_pair{ .m_second = std::move(other.m_pair.m_second) }
        {
        }

        IUnknownWrapperImpl(const Microsoft::WRL::ComPtr<value_type>& other) : m_pair{ .m_second = other }
        {
        }

        IUnknownWrapperImpl(Microsoft::WRL::ComPtr<value_type>&& other) noexcept : m_pair{ .m_second = std::move(other) }
        {
        }

        template<class OtherTy>
        IUnknownWrapperImpl(const Microsoft::WRL::ComPtr<OtherTy>& other) : m_pair{ .m_second = other }
        {
        }

        template<class OtherTy>
        IUnknownWrapperImpl(Microsoft::WRL::ComPtr<OtherTy>&& other) noexcept : m_pair{ .m_second = std::move(other) }
        {
        }

    public:
        IUnknownWrapperImpl& operator=(std::nullptr_t)
        {
            m_pair.Second() = nullptr;
            return *this;
        }
        IUnknownWrapperImpl& operator=(const IUnknownWrapperImpl& other)
        {
            m_pair.Second() = other.m_pair.Second();
            return *this;
        }
        IUnknownWrapperImpl& operator=(IUnknownWrapperImpl&& other) noexcept
        {
            m_pair.Second() = std::move(other.m_pair.Second());
            return *this;
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
            requires std::convertible_to<OtherTy*, pointer>
        IUnknownWrapperImpl& operator=(const IUnknownWrapperImpl<OtherTy, OtherInterfaceTy>& other)
        {
            m_pair.Second() = other.m_pair.Second();
            return *this;
        }
        template<class OtherTy, template<class> class OtherInterfaceTy>
            requires std::convertible_to<OtherTy*, pointer>
        IUnknownWrapperImpl& operator=(IUnknownWrapperImpl<OtherTy, OtherInterfaceTy>&& other)
        {
            m_pair.Second() = std::move(other.m_pair.Second());
            return *this;
        }

        friend bool operator==(const IUnknownWrapperImpl& lh, std::nullptr_t) noexcept
        {
            return lh.Get() == nullptr;
        }

        friend bool operator!=(const IUnknownWrapperImpl& lh, std::nullptr_t) noexcept
        {
            return lh.Get() != nullptr;
        }
        template<class OtherTy, template<class> class OtherInterfaceTy>
            requires std::convertible_to<OtherTy*, pointer>
        friend bool operator==(const IUnknownWrapperImpl& lh, const IUnknownWrapperImpl<OtherTy, OtherInterfaceTy>& rh) noexcept
        {
            return lh.Get() == rh.Get();
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
            requires std::convertible_to<OtherTy*, pointer>
        friend bool operator!=(const IUnknownWrapperImpl& lh, const IUnknownWrapperImpl<OtherTy, OtherInterfaceTy>& rh) noexcept
        {
            return lh.Get() != rh.Get();
        }

    public:
        pointer Detach() { return m_pair.Second().Detach(); }

        void Attach(pointer p)
        {
            m_pair.Second().Attach(p);
        }
        pointer Get() { return m_pair.Second().Get(); }
        pointer Get() const { return m_pair.Second().Get(); }
        Microsoft::WRL::ComPtr<value_type> AsComPtr() const { return m_pair.Second(); }
        CompressedPair::interface_pointer operator->() { return m_pair.First(); }

        template<class OtherTy, template<class OtherDerivedSelf> class OtherInterfaceTy, class Ty, template<class DerivedSelf> class InterfaceTy>
        friend IUnknownWrapperImpl<OtherTy, OtherInterfaceTy> Cast(IUnknownWrapperImpl<Ty, InterfaceTy>& other) noexcept;

        template<class OtherTy, template<class OtherDerivedSelf> class OtherInterfaceTy, class Ty, template<class DerivedSelf> class InterfaceTy>
        friend IUnknownWrapperImpl<OtherTy, OtherInterfaceTy> Cast(IUnknownWrapperImpl<Ty, InterfaceTy>&& other) noexcept;
    };


    template<class OtherTy, template<class OtherDerivedSelf> class OtherInterfaceTy, class Ty, template<class DerivedSelf> class InterfaceTy>
    IUnknownWrapperImpl<OtherTy, OtherInterfaceTy> Cast(IUnknownWrapperImpl<Ty, InterfaceTy>& other) noexcept
    {
        return IUnknownWrapperImpl<OtherTy, OtherInterfaceTy>(Helpers::COM::Cast<OtherTy>(other.m_pair.Second()));
    }


    template<class OtherTy, template<class OtherDerivedSelf> class OtherInterfaceTy, class Ty, template<class DerivedSelf> class InterfaceTy>
    IUnknownWrapperImpl<OtherTy, OtherInterfaceTy> Cast(IUnknownWrapperImpl<Ty, InterfaceTy>&& other) noexcept
    {
        return IUnknownWrapperImpl<OtherTy, OtherInterfaceTy>(Helpers::COM::Cast<OtherTy>(std::move(other.m_pair.Second())));
    }


    template<class Ty, auto... Tags>
    struct Traits;

    template<class Ty, auto... Tags>
    class IUnknownWrapper : public IUnknownWrapperImpl<Ty, typename Traits<Ty, Tags...>::Interface>
    {
    private:
        using Base = IUnknownWrapperImpl<Ty, typename Traits<Ty, Tags...>::Interface>;

    public:
        using traits_type = Traits<Ty, Tags...>;

    public:

        IUnknownWrapper() = default;
        IUnknownWrapper(const IUnknownWrapper& other) = default;
        IUnknownWrapper(IUnknownWrapper&& other) noexcept = default;

        template<class OtherTy, auto... OtherTags>
            requires ((OtherTags == Tags) && ...)
        IUnknownWrapper(const IUnknownWrapper<OtherTy, OtherTags...>& other) : Base(other)
        {
        }

        template<class OtherTy, auto... OtherTags>
            requires ((OtherTags == Tags) && ...)
        IUnknownWrapper(IUnknownWrapper<OtherTy, OtherTags...>&& other) noexcept : Base(std::move(other))
        {
        }

        IUnknownWrapper(const Base& other) : Base(other)
        {
        }
        IUnknownWrapper(Base&& other) noexcept : Base(std::move(other))
        {
        }

        IUnknownWrapper(std::nullptr_t) {};

        template<class U>
        IUnknownWrapper(U* obj) : Base(obj)
        {
        }

        IUnknownWrapper(const Microsoft::WRL::ComPtr<Ty>& other) : Base(other)
        {
        }

        IUnknownWrapper(Microsoft::WRL::ComPtr<Ty>&& other) noexcept : Base(std::move(other))
        {
        }

        template<class OtherTy>
        IUnknownWrapper(const Microsoft::WRL::ComPtr<OtherTy>& other) : Base(other)
        {
        }

        template<class OtherTy>
        IUnknownWrapper(Microsoft::WRL::ComPtr<OtherTy>&& other) noexcept : Base(std::move(other))
        {
        }

        IUnknownWrapper& operator=(const IUnknownWrapper& other) = default;
        IUnknownWrapper& operator=(IUnknownWrapper&& other) noexcept = default;

        IUnknownWrapper& operator=(const Base& other)
        {
            Base::operator=(other);
            return *this;
        }
        IUnknownWrapper& operator=(Base&& other) noexcept
        {
            Base::operator=(std::move(other));
            return *this;
        }

        template<class OtherTy, auto... OtherTags>
            requires ((OtherTags == Tags) && ...)
        IUnknownWrapper& operator=(const IUnknownWrapper<OtherTy, OtherTags...>& other)
        {
            Base::operator=(other);
            return *this;
        }

        template<class OtherTy, auto... OtherTags>
            requires ((OtherTags == Tags) && ...)
        IUnknownWrapper& operator=(IUnknownWrapper<OtherTy, OtherTags...>&& other) noexcept
        {
            Base::operator=(std::move(other));
            return *this;
        }
    };

    template<class Ty>
    constexpr bool is_unknown_wrapper = false;

    template<class Ty, auto... Tags>
    constexpr bool is_unknown_wrapper<IUnknownWrapper<Ty, Tags...>> = true;
}

namespace TypedD3D
{
    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
    Internal::IUnknownWrapper<DerivedTy, Tags...> Cast(Internal::IUnknownWrapper<Ty, Tags...>& other) noexcept
    {
        return Internal::IUnknownWrapper<DerivedTy, Tags...>(Internal::Cast<DerivedTy, typename Internal::IUnknownWrapper<DerivedTy, Tags...>::interface_type>(other));
    }

    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
    Internal::IUnknownWrapper<DerivedTy, Tags...> Cast(Internal::IUnknownWrapper<Ty, Tags...>&& other) noexcept
    {
        return Internal::IUnknownWrapper<DerivedTy, Tags...>(Internal::Cast<DerivedTy, typename Internal::IUnknownWrapper<DerivedTy, Tags...>::interface_type>(std::move(other)));
    }

    template<class DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
        requires Internal::is_unknown_wrapper<DerivedTy>&& std::is_assignable_v<DerivedTy, Internal::IUnknownWrapper<typename DerivedTy::value_type, Tags...>>
    DerivedTy Cast(Internal::IUnknownWrapper<Ty, Tags...>& other) noexcept
    {
        return DerivedTy(Internal::Cast<typename DerivedTy::value_type, typename DerivedTy::interface_type>(other));
    }

    template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, auto... Tags>
        requires Internal::is_unknown_wrapper<DerivedTy>&& std::is_assignable_v<DerivedTy, Internal::IUnknownWrapper<typename DerivedTy::value_type, Tags...>>
    DerivedTy Cast(Internal::IUnknownWrapper<Ty, Tags...>&& other) noexcept
    {
        return DerivedTy(Internal::Cast<typename DerivedTy::value_type, typename DerivedTy::interface_type>(std::move(other)));
    }
}