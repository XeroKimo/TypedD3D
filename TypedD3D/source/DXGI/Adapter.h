#pragma once

#include "../Internal/ComWrapper.h"
#include "../Wrappers.h"
#include "../Helpers/COMHelpers.h"
#include <dxgi1_6.h>

namespace TypedD3D::DXGI
{
    template<std::derived_from<IDXGIAdapter> Ty>
    class Adapter_t;
}

namespace TypedD3D::Internal
{
    namespace DXGI
    {
        template<class Ty>
        using Adapter_t = ::TypedD3D::DXGI::Adapter_t<Ty>;

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

            template<class Ty>
            struct Traits
            {
                using value_type = Ty;
                using pointer = Ty*;
                using const_pointer = const Ty*;
                using reference = Ty&;
                using cosnt_reference = const Ty&;

                template<class DerivedSelf>
                using Interface = TraitsImpl<Ty>::template Interface<DerivedSelf>;
            };
        }
    }

    template<std::derived_from<IDXGIAdapter> Ty>
    struct WrapperMapper<Ty>
    {
        using type = DXGI::Adapter_t<Ty>;
    };
}


namespace TypedD3D::DXGI
{
    template<std::derived_from<IDXGIAdapter> Ty>
    class Adapter_t : public Internal::InterfaceWrapper<Ty, typename Internal::DXGI::Adapter::Traits<Ty>::Interface>
    {

    };

    using Adapter = Adapter_t<IDXGIAdapter>;
}