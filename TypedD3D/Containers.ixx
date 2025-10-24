module;

#include <concepts>
#include <Unknwn.h>
#include <utility>
#include <d3d11.h>
#include <algorithm>
#include <array>

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

	template<class From, class To>
	concept ConvertibleTraitTo = SameTraitAs<From, To> && std::convertible_to<InnerType<From>*, InnerType<To>*>;

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
		unknown_type* operator&() const { return ptr; }
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
			requires ConvertibleTraitTo<Trait2, trait_type>
		WeakWrapper(const WeakWrapper<Trait2>& other) noexcept : ptr{ other.ptr }
		{

		}

		template<IUnknownTrait Trait2>
			requires ConvertibleTraitTo<Trait2, trait_type>
		WeakWrapper(WeakWrapper<Trait2>&& other) noexcept : ptr{ std::exchange(other.ptr, nullptr) }
		{

		}
		~WeakWrapper() = default;

	public:
		WeakWrapper(std::nullptr_t) noexcept {}
		WeakWrapper(unknown_type* ptr) noexcept : ptr{ ptr } {}

		template<IUnknownWrapperTest Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		WeakWrapper(const Wrapper& other) noexcept : ptr{ other.Get() }
		{

		}

		template<IUnknownWrapperTest Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		WeakWrapper(Wrapper&& other) noexcept : ptr{ other.Detach() }
		{
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				if(ptr)
					Release();
			}
		}

	public:
		WeakWrapper& operator=(WeakWrapper other) noexcept
		{
			ptr = std::exchange(other.ptr, nullptr);
			return *this;
		}

		template<IUnknownTrait Trait2>
			requires ConvertibleTraitTo<Trait2, trait_type>
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

		template<IUnknownWrapperTest Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		WeakWrapper& operator=(const Wrapper& other)
		{
			ptr = other.Get();
			return *this;
		}

		template<IUnknownWrapperTest Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		WeakWrapper& operator=(Wrapper&& other) noexcept
		{
			ptr = other.Detach();
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				if(ptr)
					Release();
			}
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

		bool operator==(const WeakWrapper& ptr) const noexcept
		{
			return this->ptr == ptr.Get();
		}

		template<class OtherTrait>
			requires std::equality_comparable_with<unknown_type*, typename OtherTrait::unknown_type*>
		bool operator==(const WeakWrapper<OtherTrait>& ptr) const noexcept
		{
			return this->ptr == ptr.Get();
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

		ULONG Acquire() const noexcept
		{
			return ptr->AddRef();
		}
		ULONG Release() const noexcept
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
			requires ConvertibleTraitTo<Trait2, trait_type>
		StrongWrapper(const StrongWrapper<Trait2>& other) noexcept : ptr{ other.ptr }
		{
			if(ptr)
				Acquire();
		}

		template<IUnknownTrait Trait2>
			requires ConvertibleTraitTo<Trait2, trait_type>
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

		template<IUnknownWrapperTest Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		StrongWrapper(const Wrapper& other) noexcept : ptr{ other.Get() }
		{
			if constexpr(IUnknownWeakWrapper<Wrapper>)
			{
				if(other)
					other.Acquire();
			}
		}

		template<IUnknownWrapperTest Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		StrongWrapper(Wrapper&& other) noexcept : ptr{ other.Detach() }
		{
			if constexpr(IUnknownWeakWrapper<Wrapper>)
			{
				if(other)
					other.Acquire();
			}
		}

	public:
		StrongWrapper& operator=(StrongWrapper other) noexcept
		{
			Attach(other.Detach());
			return *this;
		}

		template<IUnknownTrait Trait2>
			requires ConvertibleTraitTo<Trait2, trait_type>
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

		template<IUnknownWrapperTest Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		StrongWrapper& operator=(const Wrapper& other)
		{
			if constexpr(IUnknownWeakWrapper<Wrapper>)
			{
				if(other)
					other.Acquire();
			}
			Attach(other.Get());
			return *this;
		}

		template<IUnknownWrapperTest Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		StrongWrapper& operator=(Wrapper&& other) noexcept
		{
			if constexpr(IUnknownWeakWrapper<Wrapper>)
			{
				if(other)
					other.Acquire();
			}
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
			return this->ptr == ptr.Get();
		}

		template<class OtherTrait>
			requires std::equality_comparable_with<unknown_type*, typename OtherTrait::unknown_type*>
		bool operator==(const StrongWrapper<OtherTrait>& ptr) const noexcept
		{
			return this->ptr == ptr.Get();
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

	export template<IUnknownWrapperTest Wrapper, bool IsConst>
	class ElementReferenceTest
	{
	public:
		using unknown_type = Wrapper::unknown_type;
		using trait_type = Wrapper::trait_type;
		template<class Derived>
		using interface_type = trait_type::template Interface<Derived>;

	private:
		std::conditional_t<IsConst, unknown_type* const, unknown_type*>& ptr = nullptr;

	public:
		ElementReferenceTest(const ElementReferenceTest&) = delete;
		ElementReferenceTest(ElementReferenceTest&&) = delete;

	public:
		ElementReferenceTest(std::conditional_t<IsConst, unknown_type* const, unknown_type*>& ptr) : ptr{ ptr }
		{

		}

	public:
		ElementReferenceTest& operator=(const ElementReferenceTest&) = delete;
		ElementReferenceTest& operator=(ElementReferenceTest&&) = delete;

	public:
		ElementReferenceTest& operator=(std::nullptr_t) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::operator=() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			Wrapper temp{ nullptr };
			unknown_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

		ElementReferenceTest& operator=(unknown_type* other) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::operator=() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			Wrapper temp{ other };
			unknown_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

		ElementReferenceTest& operator=(const Wrapper& other) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::operator=() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			Wrapper temp{ other };
			unknown_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

		ElementReferenceTest& operator=(Wrapper&& other) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::operator=() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
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

		template<IUnknownWrapperTest OtherWrapper>
		bool operator==(const OtherWrapper& rh) const
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
			static_assert(!IsConst, "Calling ElementReferece<true>::Attach() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				if(this->ptr)
					this->ptr->Release();
			}

			this->ptr = ptr;
		}		
		

		unknown_type* Detach() noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::Detach() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			return std::exchange(ptr, nullptr);
		}

		ULONG Acquire() const noexcept requires IUnknownWeakWrapper<Wrapper>
		{
			return ptr->AddRef();
		}

		ULONG Release() const noexcept requires IUnknownWeakWrapper<Wrapper>
		{
			return ptr->Release();
		}

	private:
		ULONG Acquire() const noexcept
		{
			return ptr->AddRef();
		}
		ULONG Release() const noexcept
		{
			return ptr->Release();
		}
	};

	template<class Wrapper, bool IsConst>
	WeakWrapper(ElementReferenceTest<Wrapper, IsConst>) -> WeakWrapper<typename Wrapper::trait_type>;

	template<class Wrapper, bool IsConst>
	StrongWrapper(ElementReferenceTest<Wrapper, IsConst>) -> StrongWrapper<typename Wrapper::trait_type>;


	export template<class To, class From, template<class> class Wrapper, template<class> class Trait, bool IsConst>
	Wrapper<Trait<To>> Cast(const ElementReferenceTest<Wrapper<Trait<From>>, IsConst>& ptr) noexcept
	{
		return Cast<To>(Wrapper<Trait<From>>{ ptr });
	}

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
		TestArray(Wrappers&&... wrap) noexcept : values{ Wrapper{std::forward<Wrappers>(wrap)}.Detach()... }
		{
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

		ElementReferenceTest<Wrapper, false> operator[](std::size_t i) & { return values[i]; }
		ElementReferenceTest<Wrapper, true> operator[](std::size_t i) const& { return values[i]; }
		Wrapper operator[](std::size_t i)&& 
		{
			Wrapper out;
			out.Attach(std::exchange(values[i], nullptr));
			return out;
		}
		Wrapper operator[](std::size_t i) const&& 
		{
			return values[i];
		}

		ElementReferenceTest<Wrapper, false> At(std::size_t i) & { return values.at(i); }
		ElementReferenceTest<Wrapper, true> At(std::size_t i) const& { return values.at(i); }
		Wrapper At(std::size_t i)&&
		{
			Wrapper out;
			out.Attach(std::exchange(values.at(i), nullptr));
			return out;
		}
		Wrapper At(std::size_t i) const&&
		{
			return values.at(i);
		}

		ElementReferenceTest<Wrapper, false> Front()& { return values.front(); }
		ElementReferenceTest<Wrapper, true> Front() const& { return values.front(); }

		Wrapper Front()&& 
		{
			Wrapper out;
			out.Attach(std::exchange(values.front(), nullptr));
			return out;
		}
		Wrapper Front() const&& { return values.front(); }

		ElementReferenceTest<Wrapper, false> Back()& { return values.back(); }
		ElementReferenceTest<Wrapper, true> Back() const& { return values.back(); }

		Wrapper Back()&&
		{
			Wrapper out;
			out.Attach(std::exchange(values.back(), nullptr));
			return out;
		}
		Wrapper Back() const&& { return values.back(); }

		auto Data() { return values.data(); }
		const auto Data() const { return values.data(); }

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

}