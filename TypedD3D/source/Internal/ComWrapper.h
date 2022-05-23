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
        Microsoft::WRL::ComPtr<T> GetComPtr() const { return m_ptr; }
    };
}