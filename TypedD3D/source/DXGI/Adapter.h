#pragma once

#include "../Internal/ComWrapper.h"
#include "../Wrappers.h"
#include "../Helpers/COMHelpers.h"
#include <dxgi1_6.h>

namespace TypedD3D::Internal
{
    namespace DXGI
    {
        template<class Ty>
        using Adapter_t = InterfaceWrapper<Ty>;

        namespace Adapter
        {
            template<class WrapperTy, class AdapterTy>
            class Interface;

            template<class WrapperTy>
            class Interface<WrapperTy, IDXGIAdapter>
            {
            private:
                using adapter_type = IDXGIAdapter;
                using wrapper_type = WrapperTy;

            public:
                tl::expected<Microsoft::WRL::ComPtr<IDXGIOutput>, HRESULT> EnumOutputs(UINT Output)
                {
                    return Helpers::COM::UnknownObjectForwardFunction<IDXGIOutput>(&adapter_type::EnumOutputs, InternalGet(), Output);
                }

                DXGI_ADAPTER_DESC GetDesc()
                {
                    DXGI_ADAPTER_DESC desc;
                    InternalGet().GetDesc(&desc);
                    return desc;
                }

                std::pair<HRESULT, LARGE_INTEGER> CheckInterfaceSupport(REFGUID InterfaceName)
                {
                    std::pair<HRESULT, LARGE_INTEGER> result;
                    result.first = InternalGet().CheckInterfaceSupport(InterfaceName, &result.second);
                    return result;
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                adapter_type& InternalGet() { return *ToDerived().Get(); }
            };
        }
    }

    template<class DirectXClass>
        requires std::derived_from<DirectXClass, IDXGIAdapter>
    class InterfaceWrapper<DirectXClass> : public ComWrapper<DirectXClass>, private DXGI::Adapter::Interface<InterfaceWrapper<DirectXClass>, DirectXClass>
    {
    private:
        using Interface = DXGI::Adapter::Interface<InterfaceWrapper<DirectXClass>, DirectXClass>;

        template<class WrapperTy, class SwapChainTy>
        friend class DXGI::Adapter::Interface;

    public:
        static constexpr size_t tag_value = 0;
        using underlying_type = DirectXClass;

    public:
        using ComWrapper<DirectXClass>::ComWrapper;

        template<class DerivedAdapterTy>
            requires std::convertible_to<DerivedAdapterTy*, DirectXClass*>
        InterfaceWrapper(const InterfaceWrapper<DerivedAdapterTy>& other) :
            ComWrapper<DirectXClass>::ComWrapper(other.AsComPtr())
        {

        }

    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };
}


namespace TypedD3D::DXGI
{
    template<class Ty>
    using Adapter_t = Internal::DXGI::Adapter_t<Ty>;
    using Adapter = Adapter_t<IDXGIAdapter>;
}