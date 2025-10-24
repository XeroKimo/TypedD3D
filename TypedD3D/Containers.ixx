module;

#include <concepts>
#include <Unknwn.h>
#include <utility>
#include <d3d11.h>
#include <algorithm>

export module TypedD3D.Containers;

namespace TypedD3D
{
	template<class Ty>
	struct LiftType
	{
		using type = Ty;
		using concrete_type = Ty;
	};

	template<template<class> class Outer, class Inner>
	struct LiftType<Outer<Inner>>
	{
		template<class Ty>
		using type = Outer<Ty>;

		template<class Ty>
		using concrete_type = Outer<Inner>;
	};

	template<class Ty>
	struct InnerTypeImpl
	{
		using type = Ty;
	};

	template<template<class> class Outer, class Inner>
	struct InnerTypeImpl<Outer<Inner>>
	{
		using type = Inner;
	};

	template<class Ty>
	using InnerType = InnerTypeImpl<Ty>::type;

	export template<class Ty>
	concept IUnknownTrait = requires()
	{
		typename Ty::template Interface<void>;
	} && std::derived_from<InnerType<Ty>, IUnknown> && std::same_as<typename LiftType<Ty>::template type<InnerType<Ty>>, Ty>;

	export template<class Ty>
		concept IUnknownWrapperTest = requires(Ty t, Ty::unknown_type* ptr)
	{
		typename Ty::unknown_type;
		typename Ty::trait_type;
		{ t.Attach(ptr) } -> std::same_as<void>;
		{ t.Detach() } -> std::same_as<typename Ty::unknown_type*>;
		{ t.Get() } -> std::same_as<typename Ty::unknown_type*>;
	};

	export template<class Ty>
	concept IUnknownWeakWrapper = requires(Ty t)
	{
		{ t.Acquire() } -> std::convertible_to<ULONG>;
		{ t.Release() } -> std::convertible_to<ULONG>;
	} && IUnknownWrapperTest<Ty>;

	template<class Ty, class Ty2>
	concept SameTraitAs = std::same_as<typename LiftType<Ty>::template type<InnerType<Ty>>, typename LiftType<Ty2>::template type<InnerType<Ty>>>;

	export template<IUnknownTrait Trait>
	class InterfaceProxy : public Trait::template Interface<InterfaceProxy<Trait>>
	{
		using unknown_type = Trait::unknown_type;
		using trait_type = Trait;
		template<class Derived>
		using interface_type = trait_type::template Interface<Derived>;

	private:
		unknown_type* ptr;

		friend Trait::template Interface<InterfaceProxy>;
	public:
		InterfaceProxy(unknown_type* ptr) noexcept : ptr{ ptr }
		{
		}

	public:
		auto operator->() noexcept { return this; }

	private:
		unknown_type* Get() const noexcept { return ptr; }
	};

	export template<IUnknownTrait Trait>
	class WeakWrapper;

	export template<IUnknownTrait Trait>
	class StrongWrapper;

	export template<IUnknownTrait Trait>
	class WeakWrapper
	{
	public:
		using unknown_type = Trait::unknown_type;
		using trait_type = Trait;
		template<class Derived>
		using interface_type = trait_type::template Interface<Derived>;

		template<IUnknownTrait Trait>
		friend class WeakWrapper;

	private:
		unknown_type* ptr = nullptr;

	public:
		WeakWrapper() noexcept = default;
		WeakWrapper(const WeakWrapper&) noexcept = default;
		WeakWrapper(WeakWrapper&& other) noexcept : ptr{ std::exchange(other.ptr, nullptr) }
		{

		}

		template<IUnknownTrait Trait2>
		WeakWrapper(const WeakWrapper<Trait2>& other) noexcept : ptr{ other.ptr }
		{

		}

		template<IUnknownTrait Trait2>
		WeakWrapper(WeakWrapper<Trait2>&& other) noexcept : ptr{ std::exchange(other.ptr, nullptr) }
		{

		}
		~WeakWrapper() = default;

	public:
		WeakWrapper(std::nullptr_t) noexcept {}
		WeakWrapper(unknown_type* ptr) noexcept : ptr{ ptr } {}

		WeakWrapper(StrongWrapper<Trait> other) noexcept;

		template<IUnknownTrait Trait2>
		WeakWrapper(StrongWrapper<Trait2> other) noexcept;

	public:
		WeakWrapper& operator=(WeakWrapper other) noexcept
		{
			ptr = std::exchange(other.ptr, nullptr);
			return *this;
		}

		template<IUnknownTrait Trait2>
		WeakWrapper& operator=(WeakWrapper<Trait2> other) noexcept
		{
			ptr = std::exchange(other.ptr, nullptr);
			return *this;
		}

	public:
		WeakWrapper& operator=(std::nullptr_t) noexcept
		{
			ptr = nullptr;
			return *this;
		}
		WeakWrapper& operator=(unknown_type* ptr) noexcept
		{
			this->ptr = ptr;
			return *this;
		}

		WeakWrapper& operator=(StrongWrapper<Trait> other);

		template<IUnknownTrait Trait2>
		WeakWrapper& operator=(StrongWrapper<Trait2> other);

	public:
		bool operator==(std::nullptr_t) const noexcept
		{
			return ptr == nullptr;
		}

		bool operator==(unknown_type* ptr) const noexcept
		{
			return this->ptr == ptr;
		}

		bool operator==(const WeakWrapper& ptr) const noexcept
		{
			return this->ptr == ptr;
		}

		template<class OtherTrait>
			requires std::equality_comparable_with<unknown_type, typename OtherTrait::unknown_type>
		bool operator==(const WeakWrapper<OtherTrait>& ptr) const noexcept
		{
			return this->ptr == ptr;
		}

	public:
		InterfaceProxy<Trait> operator->() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<Trait>>, unknown_type*>) { return ptr; }
		InterfaceProxy<Trait> operator*() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<Trait>>, unknown_type*>) { return ptr; }

		unknown_type* operator->() const noexcept { return ptr; }
		unknown_type& operator*() const noexcept { return *ptr; }
		
		explicit operator bool() const noexcept { return ptr; }

		friend void swap(WeakWrapper& lh, WeakWrapper& rh) noexcept
		{
			std::swap(lh.ptr, rh.ptr);
		}

	public:
		unknown_type* Get() const { return ptr; }

		void Attach(unknown_type* ptr) noexcept
		{
			this->ptr = ptr;
		}

		unknown_type* Detach() noexcept
		{
			return std::exchange(ptr, nullptr);
		}

		ULONG Acquire() noexcept
		{
			return ptr->AddRef();
		}
		ULONG Release() noexcept
		{
			return ptr->Release();
		}
	};

	export template<IUnknownTrait Trait>
	class StrongWrapper
	{
	public:
		using unknown_type = Trait::unknown_type;
		using trait_type = Trait;
		template<class Derived>
		using interface_type = trait_type::template Interface<Derived>;

		template<IUnknownTrait Trait>
		friend class StrongWrapper;

	private:
		unknown_type* ptr = nullptr;

	public:
		StrongWrapper() noexcept = default;
		StrongWrapper(const StrongWrapper& other) noexcept : ptr{ other.ptr }
		{
			if(ptr)
				Acquire();
		}
		StrongWrapper(StrongWrapper&& other) noexcept : ptr{ std::exchange(other.ptr, nullptr) }
		{

		}

		template<IUnknownTrait Trait2>
			requires SameTraitAs<Trait, Trait2>
		StrongWrapper(const StrongWrapper<Trait2>& other) noexcept : ptr{ other.ptr }
		{
			if(ptr)
				Acquire();
		}

		template<IUnknownTrait Trait2>
			requires SameTraitAs<Trait, Trait2>
		StrongWrapper(StrongWrapper<Trait2>&& other) noexcept : ptr{ std::exchange(other.ptr, nullptr) }
		{

		}

		~StrongWrapper()
		{
			if(ptr)
				Release();
		}

	public:
		StrongWrapper(std::nullptr_t) noexcept {}
		StrongWrapper(unknown_type* ptr) noexcept : ptr{ ptr }
		{
			if(ptr)
				Acquire();
		}

		StrongWrapper(WeakWrapper<Trait> other) noexcept : ptr{ other.Get() }
		{
			if(ptr)
				Acquire();
		}

		template<IUnknownTrait Trait2>
			//requires CommonTrait<Trait, Trait2>
		StrongWrapper(WeakWrapper<Trait2> other) noexcept : ptr{ other.Get() }
		{
			if(ptr)
				Acquire();
		}

	public:
		StrongWrapper& operator=(StrongWrapper other) noexcept
		{
			Attach(other.Detach());
			return *this;
		}

		template<IUnknownTrait Trait2>
		StrongWrapper& operator=(StrongWrapper<Trait2> other) noexcept
		{
			Attach(other.Detach());
			return *this;
		}

	public:
		StrongWrapper& operator=(std::nullptr_t) noexcept
		{
			StrongWrapper temp;
			swap(*this, temp);
			return *this;
		}
		StrongWrapper& operator=(unknown_type* ptr) noexcept
		{
			StrongWrapper temp{ ptr };
			swap(*this, temp);
			return *this;
		}
		StrongWrapper& operator=(WeakWrapper<Trait> other) noexcept
		{
			if(other)
				other.Acquire();

			Attach(other.Detach());
			return *this;
		}

		template<IUnknownTrait Trait2>
		StrongWrapper& operator=(WeakWrapper<Trait2> other) noexcept
		{
			if(other)
				other.Acquire();

			Attach(other.Detach());
			return *this;
		}

	public:
		bool operator==(std::nullptr_t) const noexcept
		{
			return ptr == nullptr;
		}

		bool operator==(unknown_type* ptr) const noexcept
		{
			return this->ptr == ptr;
		}

		bool operator==(const StrongWrapper& ptr) const noexcept
		{
			return this->ptr == ptr;
		}

		template<class OtherTrait>
			requires std::equality_comparable_with<unknown_type, typename OtherTrait::unknown_type>
		bool operator==(const StrongWrapper<OtherTrait>& ptr) const noexcept
		{
			return this->ptr == ptr;
		}

	public:
		InterfaceProxy<Trait> operator->() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<Trait>>, unknown_type*>) { return ptr; }
		InterfaceProxy<Trait> operator*() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<Trait>>, unknown_type*>) { return ptr; }

		unknown_type* operator->() const noexcept { return ptr; }
		unknown_type& operator*() const noexcept { return *ptr; }

		explicit operator bool() const noexcept { return ptr; }

		friend void swap(StrongWrapper& lh, StrongWrapper& rh) noexcept
		{
			std::swap(lh.ptr, rh.ptr);
		}

	public:
		unknown_type* Get() const noexcept { return ptr; }

		void Attach(unknown_type* ptr) noexcept
		{
			if(this->ptr)
				this->ptr->Release();
			this->ptr = ptr;
		}

		unknown_type* Detach() noexcept
		{
			return std::exchange(ptr, nullptr);
		}

	private:
		ULONG Acquire() noexcept
		{
			return ptr->AddRef();
		}
		ULONG Release() noexcept
		{
			return ptr->Release();
		}
	};

	template<class Wrapper>
	class OutPtr
	{
		Wrapper::unknown_type* ptr;
		Wrapper& wrapper;

	public:
		OutPtr(Wrapper& wrapper) noexcept :
			wrapper{ wrapper }
		{
		}

		~OutPtr()
		{
			wrapper.Attach(ptr);
		}

		operator void** () noexcept { return &ptr; }
		operator Wrapper::unknown_type** () noexcept { return &ptr; }
	};

	template<IUnknownTrait Trait1, IUnknownTrait Trait2>
	bool operator==(const StrongWrapper<Trait1>& lh, const WeakWrapper<Trait2> rh) noexcept
	{
		return lh.Get() == rh.Get();
	}

	export template<class To, class From, template<class> class Trait>
	WeakWrapper<Trait<To>> Cast(WeakWrapper<Trait<From>> ptr) noexcept
	{
		if(!ptr)
			return {};

		WeakWrapper<Trait<To>> to;

		if(SUCCEEDED(ptr.Get()->QueryInterface<To>(OutPtr{ to })))
			to->Release();

		return to;
	}

	export template<class To, class From, template<class> class Trait>
	StrongWrapper<Trait<To>> Cast(const StrongWrapper<Trait<From>>& ptr) noexcept
	{
		if(!ptr)
			return {};

		StrongWrapper<Trait<To>> out;
		ptr.Get()->QueryInterface<To>(OutPtr{ out });
		return out;
	}

	export template<class To, class From, template<class> class Trait>
	StrongWrapper<Trait<To>> Cast(StrongWrapper<Trait<From>>&& ptr) noexcept
	{
		if(!ptr)
			return {};

		StrongWrapper<Trait<To>> out;
		ptr.Get()->QueryInterface<To>(OutPtr{ out });
		return out;
	}

	export template<class Wrapper>
	class ElementReference
	{
	public:
		using unknown_type = Wrapper::unknown_type;
		using trait_type = Wrapper::trait_type;
		template<class Derived>
		using interface_type = trait_type::template Interface<Derived>;

	private:
		unknown_type*& ptr = nullptr;

	public:
		ElementReference(const ElementReference&) = delete;
		ElementReference(ElementReference&&) = delete;

	public:
		ElementReference(unknown_type*& ptr) : ptr{ ptr }
		{

		}

	public:
		ElementReference& operator=(const ElementReference&) = delete;
		ElementReference& operator=(ElementReference&&) = delete;

	public:
		ElementReference& operator=(const Wrapper& other) noexcept
		{
			Wrapper temp{ other };
			unknown_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

		ElementReference& operator=(Wrapper&& other) noexcept
		{
			Wrapper temp{ std::move(other) };
			unknown_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

	public:
		bool operator==(std::nullptr_t) const
		{
			return ptr == nullptr;
		}

		bool operator==(unknown_type* rh) const
		{
			return ptr == rh;
		}

		bool operator==(const Wrapper& rh) const
		{
			return ptr == rh.Get();
		}
	public:
		operator Wrapper() const noexcept { return { ptr }; }

		InterfaceProxy<trait_type> operator->() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<trait_type>>, unknown_type*>) { return ptr; }
		InterfaceProxy<trait_type> operator*() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<trait_type>>, unknown_type*>) { return ptr; }

		unknown_type* operator->() const noexcept { return ptr; }
		unknown_type& operator*() const noexcept { return *ptr; }

		explicit operator bool() const noexcept { return ptr; }

	public:
		unknown_type* Get() const noexcept { return ptr; }

		void Attach(unknown_type* ptr) noexcept
		{
			if(this->ptr)
				this->ptr->Release();
			this->ptr = ptr;
		}

		unknown_type* Detach() noexcept
		{
			return std::exchange(ptr, nullptr);
		}

		ULONG Acquire() noexcept requires IUnknownWeakWrapper<Wrapper>
		{
			return ptr->AddRef();
		}

		ULONG Release() noexcept requires IUnknownWeakWrapper<Wrapper>
		{
			return ptr->Release();
		}

	private:
		ULONG Acquire() noexcept
		{
			return ptr->AddRef();
		}
		ULONG Release() noexcept
		{
			return ptr->Release();
		}
	};

	template<class Wrapper>
	WeakWrapper(ElementReference<Wrapper>) -> WeakWrapper<typename Wrapper::trait_type>;

	template<class Wrapper>
	StrongWrapper(ElementReference<Wrapper>) -> StrongWrapper<typename Wrapper::trait_type>;


	export template<class Wrapper, std::size_t N>
	class TestArray
	{
		using unknown_type = Wrapper::unknown_type;

		std::array<unknown_type*, N> values = {};

	public:
		TestArray() = default;
		TestArray(const TestArray& other) noexcept : values{ other.values }
		{
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				Acquire();
			}
		}
		TestArray(TestArray&& other) noexcept : values{ other.values }
		{
			std::ranges::fill(other.values, nullptr);
		}

		template<class... Wrappers>
			requires (std::convertible_to<Wrappers, Wrapper> && ...) && (sizeof...(Wrappers) <= N)
		TestArray(Wrappers... wrap) noexcept : values{ wrap.Get()... }
		{
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				Acquire();
			}
		}

		~TestArray()
		{
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				Release();
			}
		}

		TestArray& operator=(const TestArray& other) noexcept
		{
			auto temp = other;
			std::swap(values, temp.values);
			return *this;
		}

		TestArray& operator=(TestArray&& other) noexcept
		{
			auto temp = std::move(other);
			std::swap(values, temp.values);
			return *this;
		}

		ElementReference<Wrapper> operator[](std::size_t i)& { return values[i]; }
		Wrapper operator[](std::size_t i)&& 
		{
			Wrapper out;
			out.Attach(std::exchange(values[i], nullptr));
			return out; 
		}

	private:
		void Acquire()
		{
			for(unknown_type* ptr : values)
			{
				if(ptr)
					ptr->AddRef();
			}
		}

		void Release()
		{
			for(unknown_type* ptr : values)
			{
				if(ptr)
					ptr->Release();
			}
		}
	};

	template<IUnknownTrait Trait>
	WeakWrapper<Trait>::WeakWrapper(StrongWrapper<Trait> other) noexcept :
		ptr{ other.Get() }
	{

	}

	template<IUnknownTrait Trait>
	template<IUnknownTrait Trait2>
	WeakWrapper<Trait>::WeakWrapper(StrongWrapper<Trait2> other) noexcept :
		ptr{ other.Get() }
	{

	}

	template<IUnknownTrait Trait>
	WeakWrapper<Trait>& WeakWrapper<Trait>::operator=(StrongWrapper<Trait> other)
	{
		ptr = other.Get();
		return *this;
	}

	template<IUnknownTrait Trait>
	template<IUnknownTrait Trait2>
	WeakWrapper<Trait>& WeakWrapper<Trait>::operator=(StrongWrapper<Trait2> other)
	{
		ptr = other.Get();
		return *this;
	}
}