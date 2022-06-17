#pragma once

#include "source/Internal/IUnknownWrapper.h"
#include "source/Wrappers.h"
#include "source/Helpers/COMHelpers.h"
#include <dxgi1_6.h>

namespace TypedD3D::DXGI
{
}

namespace TypedD3D::Internal
{
    namespace DXGI
    {
        template<std::derived_from<IDXGIAdapter> Ty>
        using Adapter_t = IUnknownWrapper<Ty>;

        namespace Adapter
        {
            template<class Ty>
            struct TraitsImpl;

            template<>
            struct TraitsImpl<IDXGIAdapter>
            {
                using value_type = IDXGIAdapter;
                using pointer = IDXGIAdapter*;
                using const_pointer = const IDXGIAdapter*;
                using reference = IDXGIAdapter&;
                using cosnt_reference = const IDXGIAdapter&;

                template<class DerivedSelf>
                class Interface
                {
                    using derived_self = DerivedSelf;

                public:
                    tl::expected<Microsoft::WRL::ComPtr<IDXGIOutput>, HRESULT> EnumOutputs(UINT Output)
                    {
                        return Helpers::COM::UnknownObjectForwardFunction<IDXGIOutput>(&value_type::EnumOutputs, Get(), Output);
                    }

                    DXGI_ADAPTER_DESC GetDesc()
                    {
                        DXGI_ADAPTER_DESC desc;
                        Get().GetDesc(&desc);
                        return desc;
                    }

                    std::pair<HRESULT, LARGE_INTEGER> CheckInterfaceSupport(REFGUID InterfaceName)
                    {
                        std::pair<HRESULT, LARGE_INTEGER> result;
                        result.first = Get().CheckInterfaceSupport(InterfaceName, &result.second);
                        return result;
                    }

                private:
                    derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
                    reference Get() { return *ToDerived().derived_self::Get(); }
                };
            };
        }
    }


    template<std::derived_from<IDXGIAdapter> Ty>
    struct Traits<Ty>
    {
        using value_type = Ty;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using cosnt_reference = const Ty&;

        template<class DerivedSelf>
        using Interface = DXGI::Adapter::TraitsImpl<Ty>::template Interface<DerivedSelf>;
    };
}


namespace TypedD3D::DXGI
{
    template<std::derived_from<IDXGIAdapter> Ty>
    using Adapter_t = Internal::DXGI::Adapter_t<Ty>;

    using Adapter = Adapter_t<IDXGIAdapter>;
}