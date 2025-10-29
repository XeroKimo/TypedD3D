module;
#include <memory>
#include <d3d12.h>
#include <array>
#include <span>

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
		using inner_type = ID3D12DeviceChild;

		using inner_tag = ID3D12DeviceChild;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<IUnknownTrait Derived>
		class Interface : public InterfaceBase<Derived>
		{
		private:
			using derived_self = Derived;

		public:
			template<std::derived_from<ID3D12Device> DeviceTy = ID3D12Device>
			Wrapper<DeviceTy> GetDevice()
			{
				return ForwardFunction<DeviceTy>(&inner_type::GetDevice, Self());
			}

		private:
			using InterfaceBase<Derived>::Self;
			using InterfaceBase<Derived>::ToDerived;
		};
	};
}

namespace TypedD3D
{
	//template<>
	//struct Trait<Untagged<ID3D12DeviceChild>>
	//{
	//	using inner_type = ID3D12CommandAllocator;

	//	template<class NewInner>
	//	using trait_template = Untagged<NewInner>;

	//	template<class Derived>
	//	class Interface : public InterfaceBase<Untagged<Derived>>
	//	{
	//	private:
	//		using derived_self = Derived;

	//	public:
	//		template<std::derived_from<ID3D12Device> DeviceTy = ID3D12Device>
	//		Wrapper<DeviceTy> GetDevice()
	//		{
	//			return ForwardFunction<DeviceTy>(&inner_type::GetDevice, Self());
	//		}

	//	private:
	//		using InterfaceBase<Untagged<Derived>>::Self;
	//		using InterfaceBase<Untagged<Derived>>::ToDerived;
	//	};
	//};
}