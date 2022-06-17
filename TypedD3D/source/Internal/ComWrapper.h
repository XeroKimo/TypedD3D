#pragma once
#include "source/Helpers/COMHelpers.h"
#include <wrl/client.h>
#include <type_traits>
#include <utility>

namespace TypedD3D::Internal
{
    template<class Ty, template<class DerivedSelf> class InterfaceTy>
    class InterfaceWrapper
    {
    private:
        template<class OtherTy, template<class> class OtherInterfaceTy>
        friend class InterfaceWrapper;

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
        InterfaceWrapper() = default;
        InterfaceWrapper(std::nullptr_t) {};

        template<class U>
        InterfaceWrapper(U* obj) : m_pair{ .m_second = obj } 
        {
        }

        InterfaceWrapper(const InterfaceWrapper& other) : m_pair{ .m_second = other.m_pair.m_second }
        {
        }

        InterfaceWrapper(InterfaceWrapper&& other) noexcept : m_pair{ .m_second = std::move(other.Detach()) }
        {
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
        InterfaceWrapper(const InterfaceWrapper<OtherTy, OtherInterfaceTy>& other) : m_pair{ .m_second = other.m_pair.m_second }
        {
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
        InterfaceWrapper(InterfaceWrapper<OtherTy, OtherInterfaceTy>&& other) noexcept : m_pair{ .m_second = std::move(other.m_pair.m_second) }
        {
        }

        InterfaceWrapper(const Microsoft::WRL::ComPtr<value_type>& other) : m_pair{ .m_second = other }
        {
        }

        InterfaceWrapper(Microsoft::WRL::ComPtr<value_type>&& other) noexcept : m_pair{ .m_second = std::move(other) }
        {
        }

        template<class OtherTy>
        InterfaceWrapper(const Microsoft::WRL::ComPtr<OtherTy>& other) : m_pair{ .m_second = other }
        {
        }

        template<class OtherTy>
        InterfaceWrapper(Microsoft::WRL::ComPtr<OtherTy>&& other) noexcept : m_pair{ .m_second = std::move(other) }
        {
        }

    public:
        InterfaceWrapper& operator=(std::nullptr_t)
        {
            m_pair.Second() = nullptr;
            return *this;
        }
        InterfaceWrapper& operator=(const InterfaceWrapper& other)
        {
            m_pair.Second() = other.m_pair.Second();
            return *this;
        }
        InterfaceWrapper& operator=(InterfaceWrapper&& other)
        {
            m_pair.Second() = std::move(other.m_pair.Second());
            return *this;
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
        InterfaceWrapper& operator=(const InterfaceWrapper<OtherTy, OtherInterfaceTy>& other)
        {
            m_pair.Second() = other.m_pair.Second();
            return *this;
        }
        template<class OtherTy, template<class> class OtherInterfaceTy>
        InterfaceWrapper& operator=(InterfaceWrapper<OtherTy, OtherInterfaceTy>&& other)
        {
            m_pair.Second() = std::move(other.m_pair.Second());
            return *this;
        }

        friend bool operator==(const InterfaceWrapper& lh, std::nullptr_t) noexcept
        {
            return lh.Get() == nullptr;
        }

        friend bool operator!=(const InterfaceWrapper& lh, std::nullptr_t) noexcept
        {
            return lh.Get() != nullptr;
        }
        template<class OtherTy, template<class> class OtherInterfaceTy>
        friend bool operator==(const InterfaceWrapper& lh, const InterfaceWrapper<OtherTy, OtherInterfaceTy>& rh) noexcept
        {
            return lh.Get() == rh.Get();
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
        friend bool operator!=(const InterfaceWrapper& lh, const InterfaceWrapper<OtherTy, OtherInterfaceTy>& rh) noexcept
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
        CompressedPair::interface_pointer operator->() { return m_pair.First(); }

        template<class OtherTy, template<class OtherDerivedSelf> class OtherInterfaceTy, class Ty, template<class DerivedSelf> class InterfaceTy>
        friend InterfaceWrapper<OtherTy, OtherInterfaceTy> Cast(InterfaceWrapper<Ty, InterfaceTy>& other) noexcept;

        template<class OtherTy, template<class OtherDerivedSelf> class OtherInterfaceTy, class Ty, template<class DerivedSelf> class InterfaceTy>
        friend InterfaceWrapper<OtherTy, OtherInterfaceTy> Cast(InterfaceWrapper<Ty, InterfaceTy>&& other) noexcept;
    };


    template<class OtherTy, template<class OtherDerivedSelf> class OtherInterfaceTy, class Ty, template<class DerivedSelf> class InterfaceTy>
    InterfaceWrapper<OtherTy, OtherInterfaceTy> Cast(InterfaceWrapper<Ty, InterfaceTy>& other) noexcept
    {
        return InterfaceWrapper<OtherTy, OtherInterfaceTy>(Helpers::COM::Cast<OtherTy>(other.m_pair.Second()));
    }


    template<class OtherTy, template<class OtherDerivedSelf> class OtherInterfaceTy, class Ty, template<class DerivedSelf> class InterfaceTy>
    InterfaceWrapper<OtherTy, OtherInterfaceTy> Cast(InterfaceWrapper<Ty, InterfaceTy>&& other) noexcept 
    {
        return InterfaceWrapper<OtherTy, OtherInterfaceTy>(Helpers::COM::Cast<OtherTy>(std::move(other.m_pair.Second())));
    }
}