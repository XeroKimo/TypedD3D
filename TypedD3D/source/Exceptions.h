#pragma once
#include <stdexcept>
#include <windows.h>

namespace TypedD3D
{
    class HRESULTError : public std::domain_error
    {
    private:
        HRESULT m_code;

    public:
        HRESULTError(const char* what, HRESULT code) :
            std::domain_error(what),
            m_code(code)
        {

        }

        HRESULTError(const std::string& what, HRESULT code) :
            std::domain_error(what),
            m_code(code)
        {

        }

    public:
        HRESULT Code() const noexcept { return m_code; }
    };

    namespace Helpers
    {
        void ThrowIfFailed(HRESULT result);
        void Throw(HRESULT result);
    }
}