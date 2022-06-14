#pragma once
#include <wrl/client.h>
#include <type_traits>

namespace TypedD3D::Internal
{
    template<class T>
    class ComWrapper
    {

    private:
        Microsoft::WRL::ComPtr<T> m_ptr;

    public:
        ComWrapper() = default;
        ComWrapper(std::nullptr_t) {}
        ComWrapper(Microsoft::WRL::ComPtr<T> ptr) :
            m_ptr(ptr)
        {

        }

        template<class Derived>
            requires std::is_base_of_v<T, Derived>
        ComWrapper(Microsoft::WRL::ComPtr<Derived> ptr) :
            m_ptr(ptr)
        {
        }

        bool operator==(ComWrapper other) const noexcept { return m_ptr == other.m_ptr; }
        bool operator!=(ComWrapper other) const noexcept { return m_ptr != other.m_ptr; }

        bool operator==(std::nullptr_t) const noexcept { return m_ptr == nullptr; }
        bool operator!=(std::nullptr_t) const noexcept { return m_ptr != nullptr; }

        operator bool() const noexcept { return m_ptr; }

    public:
        T* Get() const { return m_ptr.Get(); }
        Microsoft::WRL::ComPtr<T>& AsComPtr() { return m_ptr; }
        const Microsoft::WRL::ComPtr<T>& AsComPtr() const { return m_ptr; }
    };

    enum class Increase_Ref_Tag {};

    template<class Ty, template<class DerivedSelf> class InterfaceTy>
    class InterfaceWrapper
    {
    private:
        using ptr_base = Microsoft::WRL::ComPtr<Ty>;

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

            pointer m_second = nullptr;
            
            interface_pointer& First() { return static_cast<interface_pointer>(this); }
            pointer Second() { return m_second; }
        };

    private:
        CompressedPair m_pair;

    public:
        InterfaceWrapper() = default;
        InterfaceWrapper(std::nullptr_t) {};

        /// <summary>
        /// Takes ownership of pointer, does not increase ref
        /// </summary>
        /// <typeparam name="U"></typeparam>
        /// <param name="obj"></param>
        template<class U>
        InterfaceWrapper(U* obj) : m_pair{ obj } 
        {
        }

        /// <summary>
        /// Takes ownership of pointer, increases ref
        /// </summary>
        /// <typeparam name="U"></typeparam>
        template<class U>
        InterfaceWrapper(U* obj, Increase_Ref_Tag) : m_pair{ obj }
        {
        }

        InterfaceWrapper(const InterfaceWrapper& other) : m_pair{ other.m_pair }
        {
            InternalAddRef();
        }

        InterfaceWrapper(InterfaceWrapper&& other) noexcept : m_pair{ std::move(other.m_pair) }
        {
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
        InterfaceWrapper(const InterfaceWrapper<OtherTy, OtherInterfaceTy>& other) : m_pair{ other.m_pair }
        {
            InternalAddRef();
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
        InterfaceWrapper(InterfaceWrapper<OtherTy, OtherInterfaceTy>&& other) noexcept : m_pair{ std::move(other.m_pair) }
        {
        }

        ~InterfaceWrapper()
        {
            InternalRelease();
        }

    public:
        InterfaceWrapper& operator=(std::nullptr_t)
        {
            InternalRelease();
            m_pair.First() = nullptr;
        }
        InterfaceWrapper& operator=(const InterfaceWrapper& other)
        {
            InternalRelease();
            m_pair.First() = other.m_pair.First();
            InternalAddRef();
        }
        InterfaceWrapper& operator=(InterfaceWrapper&& other)
        {
            InternalRelease();
            m_pair.First() = std::move(other.m_pair.First());
        }

        template<class OtherTy, template<class> class OtherInterfaceTy>
        InterfaceWrapper& operator=(const InterfaceWrapper<OtherTy, OtherInterfaceTy>& other)
        {
            InternalRelease();
            m_pair.First() = other.m_pair.First();
            InternalAddRef();
        }
        template<class OtherTy, template<class> class OtherInterfaceTy>
        InterfaceWrapper& operator=(InterfaceWrapper<OtherTy, OtherInterfaceTy>&& other)
        {
            InternalRelease();
            m_pair.First() = std::move(other.m_pair.First());
        }


        friend bool operator==(const InterfaceWrapper& lh, const std::nullptr_t) noexcept
        {
            return lh.Get() == nullptr;
        }

        friend bool operator!=(const InterfaceWrapper& lh, const std::nullptr_t) noexcept
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
        /// <summary>
        /// Releases ownership of the pointer giving someone else responsibility for calling    
        /// </summary>
        /// <returns></returns>
        pointer Detach() { return std::exchange(m_pair.First(), nullptr); }

        /// <summary>
        /// Takes ownership of a new pointer, does not increase ref of the newly owned pointer
        /// </summary>
        /// <param name="p"></param>
        void Attach(pointer p)
        {
            InternalRelease();
            m_pair.First() = p;
        }
        pointer Get() const { return m_pair.First(); }
        CompressedPair::interface_pointer operator->() { return m_pair.Second(); }

    private:
        void InternalAddRef()
        {
            if(m_pair.First())
                m_pair.First()->AddRef();
        }
        void InternalRelease()
        {
            pointer oldVal = m_pair.First();
            m_pair.First() = nullptr;

            if(oldVal)
            {
                oldVal->Release();
            }
        }
    };


    template<class OtherTy, template<class OtherDerivedSelf> class OtherInterfaceTy, class Ty, template<class DerivedSelf> class InterfaceTy>
    InterfaceWrapper<OtherTy, OtherInterfaceTy> Cast(const InterfaceWrapper<Ty, InterfaceTy>& other) noexcept
    {
        OtherTy* temp;
        other.Get()->QueryInterface(&temp);
        return InterfaceWrapper<OtherTy, OtherInterfaceTy>(temp);
    }


    template<class OtherTy, template<class OtherDerivedSelf> class OtherInterfaceTy, class Ty, template<class DerivedSelf> class InterfaceTy>
    InterfaceWrapper<OtherTy, OtherInterfaceTy> Cast(InterfaceWrapper<Ty, InterfaceTy>&& other) noexcept 
    {
        OtherTy* temp;
        p->QueryInterface(&temp);
        return InterfaceWrapper<OtherTy, OtherInterfaceTy>(temp);
    }
}