module;

#include <concepts>
#include <dxgi1_6.h>

#include <utility>

export module TypedDXGI:Adapter;
import :DXGIObject;
import TypedD3D.Shared;

namespace TypedD3D
{
	static_assert(IUnknownTrait<Untagged<IDXGIOutput>>);
	template<>
	struct Trait<Untagged<IDXGIAdapter>>
	{

		using inner_type = IDXGIAdapter;

		using inner_tag = IDXGIAdapter;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class DerivedSelf>
		class Interface : public InterfaceBase<Trait<Untagged<DerivedSelf>>>
		{
			using derived_self = DerivedSelf;

		public:
			Wrapper<IDXGIOutput> EnumOutputs(UINT Output)
			{
				return ForwardFunction<Wrapper<IDXGIOutput>>(&inner_type::EnumOutputs, Self(), Output);
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
			using InterfaceBase<Trait<Untagged<DerivedSelf>>>::Self;
			using InterfaceBase<Trait<Untagged<DerivedSelf>>>::ToDerived;
		};
	};
}