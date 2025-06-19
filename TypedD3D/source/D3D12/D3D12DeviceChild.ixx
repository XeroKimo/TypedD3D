module;
#include <memory>
#include <d3d12.h>
#include <array>
#include <span>
#include <wrl/client.h>
#include <assert.h>
#include <optional>

#pragma warning (push)
#pragma warning (disable : 4584)

export module TypedD3D12:DeviceChild;
import TypedD3D.Shared;


namespace TypedD3D::D3D12
{
	struct DeviceChildTraits
	{
		using value_type = ID3D12DeviceChild;
		using pointer = ID3D12DeviceChild*;
		using const_pointer = const ID3D12DeviceChild*;
		using reference = ID3D12DeviceChild&;
		using const_reference = const ID3D12DeviceChild&;

		template<class DerivedSelf>
		class Interface
		{
		private:
			using derived_self = DerivedSelf;

		public:
			template<std::derived_from<ID3D12Device> DeviceTy = ID3D12Device>
			Wrapper<DeviceTy> GetDevice()
			{
				return IIDToObjectForwardFunction<DeviceTy>(&value_type::GetDevice, Get());
			}

		private:
			derived_self& ToDerived() { return static_cast<derived_self&>(*this); }
			reference Get() { return *ToDerived().derived_self::Get(); }
		};
	};
}