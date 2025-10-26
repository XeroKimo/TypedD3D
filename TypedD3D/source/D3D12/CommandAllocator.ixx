module;

#include <d3d12.h>

#include <assert.h>

export module TypedD3D12:CommandAllocator;
import TypedD3D.Shared;
import :Wrappers;

namespace TypedD3D::D3D12
{
	template<D3D12_COMMAND_LIST_TYPE Type>
	struct CommandListTypeToTraitMap;

	template<>
	struct CommandListTypeToTraitMap<D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT>
	{
		template<class Ty>
		using type = DirectTraits<Ty>;
	};

	template<D3D12_COMMAND_LIST_TYPE Type, class IUnknown>
	using CommandListTypeToTrait = typename CommandListTypeToTraitMap<Type>::template type<IUnknown>;

	template<D3D12_COMMAND_LIST_TYPE Type>
	struct CommandAllocatorTraits
	{
		static constexpr D3D12_COMMAND_LIST_TYPE command_list_value = Type;

		using value_type = ID3D12CommandAllocator;
		using pointer = ID3D12CommandAllocator*;
		using const_pointer = const ID3D12CommandAllocator*;
		using reference = ID3D12CommandAllocator&;
		using const_reference = const ID3D12CommandAllocator&;

		template<class Derived>
		class Interface : public InterfaceBase<CommandListTypeToTrait<Type, Derived>>
		{
		public:
			HRESULT Reset() { return Self().Reset(); }

		private:
			using InterfaceBase<CommandListTypeToTrait<Type, Derived>>::Self;
			using InterfaceBase<CommandListTypeToTrait<Type, Derived>>::ToDerived;
		};
	};
}

namespace TypedD3D
{
	template<>
	struct DirectTraits<ID3D12CommandAllocator> : public D3D12::CommandAllocatorTraits<D3D12_COMMAND_LIST_TYPE_DIRECT> 
	{
	};

	template<>
	struct ComputeTraits<ID3D12CommandAllocator> : public D3D12::CommandAllocatorTraits<D3D12_COMMAND_LIST_TYPE_COMPUTE> 
	{
	};

	template<>
	struct CopyTraits<ID3D12CommandAllocator> : public D3D12::CommandAllocatorTraits<D3D12_COMMAND_LIST_TYPE_COPY> 
	{
	};

	template<>
	struct BundleTraits<ID3D12CommandAllocator> : public D3D12::CommandAllocatorTraits<D3D12_COMMAND_LIST_TYPE_BUNDLE> 
	{
	};
}

//namespace TypedD3D::D3D12
//{
//	export template<D3D12_COMMAND_LIST_TYPE Type, class UnknownType>
//	using CommandAllocator_t = StrongWrapper<CommandListTypeToTrait<type, UnknownType>>;
//
//	export template<D3D12_COMMAND_LIST_TYPE Type, class UnknownType>
//	using CommandAllocatorView_t = WeakWrapper<CommandListTypeToTrait<type, UnknownType>>;
//
//	export template<D3D12_COMMAND_LIST_TYPE Type>
//	using CommandAllocator = CommandAllocator_t<Type, ID3D12CommandAllocator>;
//
//	export template<D3D12_COMMAND_LIST_TYPE Type>
//	using CommandAllocatorView_t = CommandAllocatorView_t<Type, ID3D12CommandAllocator>;
//}