module;


#include <dxgi1_6.h>
#include "MyExpected.h"
#include <wrl/client.h>

export module TypedDXGI:Adapter;
import TypedD3D.Shared;

namespace TypedD3D::DXGI
{
	export template<std::derived_from<IDXGIAdapter> Ty>
	using Adapter_t = IUnknownWrapper<Ty, UntaggedTraits>;

	template<class Ty>
	struct AdapterTraits;

	template<>
	struct AdapterTraits<IDXGIAdapter>
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
			expected<Microsoft::WRL::ComPtr<IDXGIOutput>, HRESULT> EnumOutputs(UINT Output)
			{
				return UnknownObjectForwardFunction<IDXGIOutput>(&value_type::EnumOutputs, Get(), Output);
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

	namespace Aliases
	{
		export using DXGIAdapter = Adapter_t<IDXGIAdapter>;
	}
}

namespace TypedD3D
{
	template<std::derived_from<IDXGIAdapter> Ty>
	struct UntaggedTraits<Ty> : public DXGI::AdapterTraits<Ty>
	{
	};
}