module;

#include <concepts>
#include <dxgi1_6.h>

#include <utility>

export module TypedDXGI:Adapter;
import :DXGIObject;
import TypedD3D.Shared;

namespace TypedD3D
{
	template<>
	struct UntaggedTraits<IDXGIAdapter> 
	{
		using value_type = IDXGIAdapter;
		using pointer = IDXGIAdapter*;
		using const_pointer = const IDXGIAdapter*;
		using reference = IDXGIAdapter&;
		using cosnt_reference = const IDXGIAdapter&;

		using inner_type = IDXGIAdapter;
		template<class DerivedSelf>
		class Interface : public InterfaceBase<UntaggedTraits<DerivedSelf>>
		{
			using derived_self = DerivedSelf;

		public:
			Wrapper<IDXGIOutput> EnumOutputs(UINT Output)
			{
				return ForwardFunction<Wrapper<IDXGIOutput>>(&value_type::EnumOutputs, Self(), Output);
			}

			DXGI_ADAPTER_DESC GetDesc()
			{
				DXGI_ADAPTER_DESC desc;
				Self().GetDesc(&desc);
				return desc;
			}

			std::pair<HRESULT, LARGE_INTEGER> CheckInterfaceSupport(REFGUID InterfaceName)
			{
				std::pair<HRESULT, LARGE_INTEGER> result;
				result.first = Self().CheckInterfaceSupport(InterfaceName, &result.second);
				return result;
			}

		private:
			using InterfaceBase<UntaggedTraits<DerivedSelf>>::Self;
			using InterfaceBase<UntaggedTraits<DerivedSelf>>::ToDerived;
		};
	};
}