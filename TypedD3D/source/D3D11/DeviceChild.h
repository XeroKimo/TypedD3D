#pragma once
#include "source/Wrappers.h"
#include "source/Internal/IUnknownWrapper.h"
#include "source/Helpers/COMHelpers.h"
#include <d3d11_4.h>

namespace TypedD3D::Internal
{
    namespace D3D11
    {
        namespace DeviceChild
        {
            template<class WrapperTy>
            class Interface
            {
            private:
                using device_type = ID3D11DeviceChild;
                using wrapper_type = WrapperTy;
            public:
                template<std::derived_from<ID3D11Device> DeviceTy = ID3D11Device>
                Wrapper<DeviceTy> GetDevice()
                {
                    Microsoft::WRL::ComPtr<ID3D11Device> device;
                    InternalGet().GetDevice(&device);
                    return Wrapper<DeviceTy>(Helpers::COM::Cast<DeviceTy>(device));
                }

                //TODO: Figure out how this works to update to a more modern API
                HRESULT GetPrivateData(
                    REFGUID guid,
                    UINT* pDataSize,
                    void* pData)
                {
                    return InternalGet().GetPrivateData(guid, pDataSize, pData);
                }

                //TODO: Figure out how this works to update to a more modern API
                HRESULT SetPrivateData(
                    REFGUID guid,
                    UINT DataSize,
                    const void* pData)
                {
                    return SetPrivateData().GetPrivateData(guid, DataSize, pData);
                }

                //TODO: Figure out how this works to update to a more modern API
                HRESULT SetPrivateDataInterface(
                    REFGUID guid,
                    const IUnknown* pData)
                {
                    return SetPrivateData().SetPrivateDataInterface(guid, pData);
                }

            private:
                wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
                device_type& InternalGet() { return *ToDerived().Get(); }
            };
        }
    }
}