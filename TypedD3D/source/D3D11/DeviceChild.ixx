module;

#include <concepts>
#include <d3d11_4.h>
#include <wrl/client.h>

export module TypedD3D11:DeviceChild;
import TypedD3D.Shared;

namespace TypedD3D::D3D11
{
    template<class WrapperTy>
    class DeviceChildInterface
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
            return Wrapper<DeviceTy>(Cast<DeviceTy>(device));
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
            return InternalGet().GetPrivateData(guid, DataSize, pData);
        }

        //TODO: Figure out how this works to update to a more modern API
        HRESULT SetPrivateDataInterface(
            REFGUID guid,
            const IUnknown* pData)
        {
            return InternalGet().SetPrivateDataInterface(guid, pData);
        }

    private:
        wrapper_type& ToDerived() { return static_cast<wrapper_type&>(*this); }
        device_type& InternalGet() { return *ToDerived().Get(); }
    };
}