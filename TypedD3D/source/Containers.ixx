module;

#include <concepts>
#include <Unknwn.h>
#include <utility>
#include <d3d11.h>
#include <algorithm>
#include <array>
#include <span>
#include <vector>

export module TypedD3D.Shared:Containers;

namespace TypedD3D
{
	template<class Ty>
	struct LiftType : std::false_type
	{
		using type = Ty;
		using concrete_type = Ty;
	};

	template<template<class> class Outer, class Inner>
	struct LiftType<Outer<Inner>> : std::true_type
	{
		template<class Ty>
		using type = Outer<Ty>;

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

	export template<class Ty>
	concept LiftableType = LiftType<std::remove_cvref_t<Ty>>::value;

	export template<class Ty>
	using InnerType = InnerTypeImpl<std::remove_cvref_t<Ty>>::type;

	export template<class Ty, class NewInner>
	using ReplaceInnerType = LiftType<std::remove_cvref_t<Ty>>::template type<NewInner>;

	export template<class Ty, template<class> class NewOuter>
	using ReplaceOuterType = NewOuter<InnerType<std::remove_cvref_t<Ty>>>;

	export template<class Ty>
	concept IUnknownTrait = std::derived_from<InnerType<Ty>, IUnknown>;

	export template<class Ty>
	concept IUnknownWrapper = requires(std::remove_const_t<Ty> t, Ty::inner_type* ptr)
	{
		typename Ty::inner_type;
		typename Ty::trait_type;
		{ t.Attach(ptr) } -> std::same_as<void>;
		{ t.Detach() } -> std::same_as<typename Ty::inner_type*>;
		{ t.Get() } -> std::same_as<typename Ty::inner_type*>;
	};

	export template<class Ty>
	concept IUnknownWeakWrapper = requires(Ty t)
	{
		{ t.Acquire() } -> std::convertible_to<ULONG>;
		{ t.Release() } -> std::convertible_to<ULONG>;
	} && IUnknownWrapper<Ty>;

	template<class Ty, class Ty2>
	concept SameTraitAs = std::same_as<typename LiftType<Ty>::template type<InnerType<Ty>>, typename LiftType<Ty2>::template type<InnerType<Ty>>>;

	template<class From, class To>
	concept ConvertibleTraitTo = SameTraitAs<From, To> && std::convertible_to<InnerType<From>*, InnerType<To>*>;

	export template<IUnknownTrait Trait>
	class InterfaceProxy : public Trait::template Interface<InnerType<Trait>>
	{
		using inner_type = InnerType<Trait>;
		using trait_type = Trait;
		template<class Derived>
		using interface_type = trait_type::template Interface<Derived>;

	private:
		inner_type* ptr;

		friend Trait::template Interface<InterfaceProxy>;
	public:
		InterfaceProxy(inner_type* ptr) noexcept : ptr{ ptr }
		{
		}

	public:
		auto operator->() noexcept { return this; }
		inner_type* operator&() const noexcept { return ptr; }
		inner_type* Get() const noexcept { return ptr; }
		operator inner_type& () const noexcept { return *ptr; }
	};


	export template<IUnknownTrait Trait>
	struct InterfaceBase
	{
		InterfaceProxy<Trait>& ToDerived() { return static_cast<InterfaceProxy<Trait>&>(*this); }
		InnerType<Trait>& Self() { return *ToDerived().InterfaceProxy<Trait>::Get(); }
	};

	export template<IUnknownTrait Trait>
	class WeakWrapper;

	export template<IUnknownTrait Trait>
	class StrongWrapper;

	export template<IUnknownTrait Trait>
	class WeakWrapper
	{
	public:
		using inner_type = InnerType<Trait>;
		using trait_type = Trait;
		template<class Derived>
		using interface_type = trait_type::template Interface<Derived>;

		template<IUnknownTrait Trait>
		friend class WeakWrapper;

	private:
		inner_type* ptr = nullptr;

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
		WeakWrapper(inner_type* ptr) noexcept : ptr{ ptr } {}

		template<IUnknownWrapper Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		WeakWrapper(const Wrapper& other) noexcept : ptr{ other.Get() }
		{

		}

		template<IUnknownWrapper Wrapper>
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
		WeakWrapper& operator=(inner_type* ptr) noexcept
		{
			this->ptr = ptr;
			return *this;
		}

		template<IUnknownWrapper Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		WeakWrapper& operator=(const Wrapper& other)
		{
			ptr = other.Get();
			return *this;
		}

		template<IUnknownWrapper Wrapper>
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

		bool operator==(inner_type* ptr) const noexcept
		{
			return this->ptr == ptr;
		}

		bool operator==(const WeakWrapper& ptr) const noexcept
		{
			return this->ptr == ptr.Get();
		}

		template<IUnknownWrapper Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		bool operator==(const Wrapper& ptr) const noexcept
		{
			return this->ptr == ptr.Get();
		}

	public:
		InterfaceProxy<Trait> operator->() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<Trait>>, inner_type*>) { return ptr; }
		InterfaceProxy<Trait> operator*() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<Trait>>, inner_type*>) { return ptr; }

		inner_type* operator->() const noexcept { return ptr; }
		inner_type& operator*() const noexcept { return *ptr; }
		
		explicit operator bool() const noexcept { return ptr; }

		friend void swap(WeakWrapper& lh, WeakWrapper& rh) noexcept
		{
			std::swap(lh.ptr, rh.ptr);
		}

	public:
		inner_type* Get() const { return ptr; }

		void Attach(inner_type* ptr) noexcept
		{
			this->ptr = ptr;
		}

		inner_type* Detach() noexcept
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
		using inner_type = InnerType<Trait>;
		using trait_type = Trait;
		template<class Derived>
		using interface_type = trait_type::template Interface<Derived>;

		template<IUnknownTrait Trait>
		friend class StrongWrapper;

	private:
		inner_type* ptr = nullptr;

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
		StrongWrapper(inner_type* ptr) noexcept : ptr{ ptr }
		{
			if(ptr)
				Acquire();
		}

		template<IUnknownWrapper Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		StrongWrapper(const Wrapper& other) noexcept : ptr{ other.Get() }
		{
			if constexpr(IUnknownWeakWrapper<Wrapper>)
			{
				if(ptr)
					Acquire();
			}
		}

		template<IUnknownWrapper Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		StrongWrapper(Wrapper&& other) noexcept : ptr{ other.Detach() }
		{
			if constexpr(IUnknownWeakWrapper<Wrapper>)
			{
				if(ptr)
					Acquire();
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
		StrongWrapper& operator=(inner_type* ptr) noexcept
		{
			StrongWrapper temp{ ptr };
			swap(*this, temp);
			return *this;
		}

		template<IUnknownWrapper Wrapper>
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

		template<IUnknownWrapper Wrapper>
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

		bool operator==(inner_type* ptr) const noexcept
		{
			return this->ptr == ptr;
		}

		bool operator==(const StrongWrapper& ptr) const noexcept
		{
			return this->ptr == ptr.Get();
		}

		template<IUnknownWrapper Wrapper>
			requires ConvertibleTraitTo<typename Wrapper::trait_type, trait_type>
		bool operator==(const Wrapper& ptr) const noexcept
		{
			return this->ptr == ptr.Get();
		}

	public:
		InterfaceProxy<Trait> operator->() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<Trait>>, inner_type*>) { return ptr; }
		InterfaceProxy<Trait> operator*() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<Trait>>, inner_type*>) { return ptr; }

		inner_type* operator->() const noexcept { return ptr; }
		inner_type& operator*() const noexcept { return *ptr; }

		explicit operator bool() const noexcept { return ptr; }

		friend void swap(StrongWrapper& lh, StrongWrapper& rh) noexcept
		{
			std::swap(lh.ptr, rh.ptr);
		}

	public:
		inner_type* Get() const noexcept { return ptr; }

		void Attach(inner_type* ptr) noexcept
		{
			if(this->ptr)
				this->ptr->Release();
			this->ptr = ptr;
		}

		inner_type* Detach() noexcept
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

	export template<class Wrapper>
	class OutPtr
	{
		Wrapper::inner_type* ptr;
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

		operator void** () noexcept { return reinterpret_cast<void**>(&ptr); }
		operator Wrapper::inner_type** () noexcept { return &ptr; }
	};

	export template<class To, class From, template<class> class Trait>
	WeakWrapper<Trait<To>> Cast(const WeakWrapper<Trait<From>>& ptr) noexcept
	{
		if(!ptr)
			return {};

		WeakWrapper<Trait<To>> to;

		if(SUCCEEDED(ptr.Get()->QueryInterface<To>(OutPtr{ to })))
			to->Release();

		return to;
	}

	export template<class To, class From, template<class> class Trait>
	WeakWrapper<Trait<To>> Cast(WeakWrapper<Trait<From>>&& ptr) noexcept
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

	export template<class Ty, bool IsConst>
	class ElementReference;

	export template<IUnknownWrapper Wrapper, bool IsConst>
	class ElementReference<Wrapper, IsConst>
	{
	public:
		using inner_type = Wrapper::inner_type;
		using trait_type = Wrapper::trait_type;
		template<class Derived>
		using interface_type = trait_type::template Interface<Derived>;

	private:
		std::conditional_t<IsConst, inner_type* const, inner_type*>& ptr = nullptr;

	public:
		ElementReference(const ElementReference&) = delete;
		ElementReference(ElementReference&&) = delete;

	public:
		ElementReference(std::conditional_t<IsConst, inner_type* const, inner_type*>& ptr) : ptr{ ptr }
		{

		}

	public:
		ElementReference& operator=(const ElementReference&) = delete;
		ElementReference& operator=(ElementReference&&) = delete;

	public:
		ElementReference& operator=(std::nullptr_t) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::operator=() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			Wrapper temp{ nullptr };
			inner_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

		ElementReference& operator=(inner_type* other) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::operator=() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			Wrapper temp{ other };
			inner_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

		ElementReference& operator=(const Wrapper& other) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::operator=() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			Wrapper temp{ other };
			inner_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

		ElementReference& operator=(Wrapper&& other) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::operator=() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			Wrapper temp{ std::move(other) };
			inner_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

		template<IUnknownWrapper OtherWrapper>
			requires ConvertibleTraitTo<typename OtherWrapper::trait_type, trait_type>
		ElementReference& operator=(const OtherWrapper& other) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::operator=() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			Wrapper temp{ other };
			inner_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

		template<IUnknownWrapper OtherWrapper>
			requires ConvertibleTraitTo<typename OtherWrapper::trait_type, trait_type>
		ElementReference& operator=(OtherWrapper&& other) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::operator=() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			Wrapper temp{ std::move(other) };
			inner_type* temp2 = ptr;
			ptr = temp.Detach();
			temp.Attach(temp2);
			return *this;
		}

	public:
		bool operator==(std::nullptr_t) const
		{
			return ptr == nullptr;
		}

		bool operator==(inner_type* rh) const
		{
			return ptr == rh;
		}

		bool operator==(const Wrapper& rh) const
		{
			return ptr == rh.Get();
		}

		template<IUnknownWrapper OtherWrapper>
		bool operator==(const OtherWrapper& rh) const
		{
			return ptr == rh.Get();
		}
	public:
		operator Wrapper() const noexcept { return { ptr }; }

		InterfaceProxy<trait_type> operator->() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<trait_type>>, inner_type*>) { return ptr; }
		InterfaceProxy<trait_type> operator*() const noexcept requires (!std::same_as<interface_type<InterfaceProxy<trait_type>>, inner_type*>) { return ptr; }

		inner_type* operator->() const noexcept { return ptr; }
		inner_type& operator*() const noexcept { return *ptr; }

		explicit operator bool() const noexcept { return ptr; }

	public:
		inner_type* Get() const noexcept { return ptr; }

		void Attach(inner_type* ptr) noexcept
		{
			static_assert(!IsConst, "Calling ElementReferece<true>::Attach() is not allowed as it's a const proxy. The function still needs exists to satisfy a concept as to act as if the function is const");
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				if(this->ptr)
					this->ptr->Release();
			}

			this->ptr = ptr;
		}		
		

		inner_type* Detach() noexcept
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
	WeakWrapper(ElementReference<Wrapper, IsConst>) -> WeakWrapper<typename Wrapper::trait_type>;

	template<class Wrapper, bool IsConst>
	StrongWrapper(ElementReference<Wrapper, IsConst>) -> StrongWrapper<typename Wrapper::trait_type>;


	export template<class To, class From, template<class> class Wrapper, template<class> class Trait, bool IsConst>
	Wrapper<Trait<To>> Cast(const ElementReference<Wrapper<Trait<From>>, IsConst>& ptr) noexcept
	{
		return Cast<To>(Wrapper<Trait<From>>{ ptr });
	}

	export template<class Wrapper, bool IsConst>
	class ContiguousIterator
	{
		using inner_type = Wrapper::inner_type;

	private:
		std::conditional_t<IsConst, inner_type* const, inner_type*>* ptr = nullptr;

	public:
		ContiguousIterator() = default;
		ContiguousIterator(std::conditional_t<IsConst, inner_type* const, inner_type*>* ptr) : ptr{ ptr }
		{

		}

	public:
		ContiguousIterator& operator++()
		{
			++ptr;
			return *this;
		}
		ContiguousIterator operator++(int)
		{
			return ptr++;
		}
		ContiguousIterator& operator--()
		{
			--ptr;
			return *this;
		}
		ContiguousIterator operator--(int)
		{
			return ptr--;
		}

		ContiguousIterator operator+(std::size_t i) const
		{
			return ptr + i;
		}

		ContiguousIterator& operator+=(std::size_t i)
		{
			ptr += i;
			return *this;
		}

		ContiguousIterator operator-(std::size_t i) const
		{
			return ptr + i;
		}

		ContiguousIterator& operator-=(std::size_t i)
		{
			ptr += i;
			return *this;
		}

		size_t operator-(const ContiguousIterator& other) const
		{
			return ptr - other.ptr;
		}

		bool operator==(const ContiguousIterator& other) const
		{
			return ptr == other.ptr;
		}

		auto Raw() const { return ptr; }

		ElementReference<Wrapper, IsConst> operator*() const { return *ptr; }
		ElementReference<Wrapper, IsConst> operator->() const { return *ptr; }
	};

	export template <class Ty>
	concept TypedStructTrait = LiftableType<Ty> && !std::derived_from<InnerType<Ty>, IUnknown>;

	export template<TypedStructTrait Ty>
	struct TypedStruct : private InnerType<Ty>, public Ty::template Interface<TypedStruct<Ty>>
	{
		using inner_type = InnerType<Ty>;
		TypedStruct() = default;
		TypedStruct(const InnerType<Ty>& other) : InnerType<Ty>{ other }
		{

		}

		TypedStruct& operator=(const InnerType<Ty>& other)
		{
			*static_cast<InnerType<Ty>*>(this) = other;
			return *this;
		}

		InnerType<Ty>& Raw() { return *this; }
		const InnerType<Ty>& Raw() const { return *this; }
	};

	export template<class Derived>
		struct TypedStructInterfaceBase
	{
		Derived& ToDerived() { return static_cast<Derived&>(*this); }
		auto& Self() { return ToDerived().Derived::Raw(); }

		const Derived& ToDerived() const { return static_cast<const Derived&>(*this); }
		const auto& Self() const { return ToDerived().Derived::Raw(); }
	};

	export template<class Wrapper, std::size_t N>
	class Array;

	export template<IUnknownWrapper Wrapper, std::size_t N>
	class Array<Wrapper, N>
	{
		using inner_type = Wrapper::inner_type;

		std::array<inner_type*, N> values = {};

	public:
		Array() = default;
		Array(const Array& other) noexcept : values{ other.values }
		{
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				Acquire();
			}
		}
		Array(Array&& other) noexcept : values{ other.values }
		{
			std::ranges::fill(other.values, nullptr);
		}

		template<class... Wrappers>
			requires (std::convertible_to<Wrappers, Wrapper> && ...) && (sizeof...(Wrappers) <= N)
		Array(Wrappers&&... wrap) noexcept : values{ Wrapper{std::forward<Wrappers>(wrap)}.Detach()... }
		{
		}

		~Array()
		{
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				Release();
			}
		}

		Array& operator=(const Array& other) noexcept
		{
			auto temp = other;
			std::swap(values, temp.values);
			return *this;
		}

		Array& operator=(Array&& other) noexcept
		{
			auto temp = std::move(other);
			std::swap(values, temp.values);
			return *this;
		}

		ElementReference<Wrapper, false> operator[](std::size_t i) & { return values[i]; }
		ElementReference<Wrapper, true> operator[](std::size_t i) const& { return values[i]; }
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

		ElementReference<Wrapper, false> at(std::size_t i) & { return values.at(i); }
		ElementReference<Wrapper, true> at(std::size_t i) const& { return values.at(i); }
		Wrapper at(std::size_t i)&&
		{
			Wrapper out;
			out.Attach(std::exchange(values.at(i), nullptr));
			return out;
		}
		Wrapper at(std::size_t i) const&&
		{
			return values.at(i);
		}

		ElementReference<Wrapper, false> front()& { return values.front(); }
		ElementReference<Wrapper, true> front() const& { return values.front(); }

		Wrapper front()&&
		{
			Wrapper out;
			out.Attach(std::exchange(values.front(), nullptr));
			return out;
		}
		Wrapper front() const&& { return values.front(); }

		ElementReference<Wrapper, false> back()& { return values.back(); }
		ElementReference<Wrapper, true> back() const& { return values.back(); }

		Wrapper back()&&
		{
			Wrapper out;
			out.Attach(std::exchange(values.back(), nullptr));
			return out;
		}
		Wrapper back() const&& { return values.back(); }

		auto data() { return values.data(); }
		const auto data() const { return values.data(); }

		ContiguousIterator<Wrapper, false> begin() { return &values[0]; }
		ContiguousIterator<Wrapper, false> end() { return &values[values.size()]; }

		ContiguousIterator<Wrapper, true> begin() const { return &values[0]; }
		ContiguousIterator<Wrapper, true> end() const { return &values[values.size()]; }

		size_t size() const { return values.size(); }

	private:
		void Acquire()
		{
			for(inner_type* ptr : values)
			{
				if(ptr)
					ptr->AddRef();
			}
		}

		void Release()
		{
			for(inner_type* ptr : values)
			{
				if(ptr)
					ptr->Release();
			}
		}
	};	
	
	template<IUnknownWrapper Wrapper, std::convertible_to<Wrapper>... Tys>
	Array(Wrapper, Tys...) -> Array<Wrapper, sizeof...(Tys) + 1>;
	
	template<IUnknownWrapper Wrapper, std::convertible_to<Wrapper>... Tys>
	Array(ElementReference<Wrapper, false>, Tys...) -> Array<Wrapper, sizeof...(Tys) + 1>;
	
	template<IUnknownWrapper Wrapper, std::convertible_to<Wrapper>... Tys>
	Array(ElementReference<Wrapper, true>, Tys...) -> Array<Wrapper, sizeof...(Tys) + 1>;

	export template<IUnknownWrapper Wrapper>
	class Vector
	{
		using inner_type = Wrapper::inner_type;

		std::vector<inner_type*> values = {};

	public:
		Vector() = default;
		Vector(const Vector& other) noexcept : values{ other.values }
		{
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				Acquire();
			}
		}
		Vector(Vector&& other) noexcept : values{ std::move(other).values }
		{
		}

		Vector(std::initializer_list<Wrapper> list)
		{
			for(Wrapper w : list)
			{
				values.push_back(w.Detach());
			}
		}

		template<class... Wrappers>
			requires (std::convertible_to<Wrappers, Wrapper> && ...) && (sizeof...(Wrappers) <= N)
		Vector(Wrappers&&... wrap) noexcept : values{ Wrapper{std::forward<Wrappers>(wrap)}.Detach()... }
		{
		}

		~Vector()
		{
			if constexpr(!IUnknownWeakWrapper<Wrapper>)
			{
				Release();
			}
		}

		Vector& operator=(const Vector& other) noexcept
		{
			auto temp = other;
			std::swap(values, temp.values);
			return *this;
		}

		Vector& operator=(Vector&& other) noexcept
		{
			auto temp = std::move(other);
			std::swap(values, temp.values);
			return *this;
		}

		ElementReference<Wrapper, false> operator[](std::size_t i) & { return values[i]; }
		ElementReference<Wrapper, true> operator[](std::size_t i) const& { return values[i]; }
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

		ElementReference<Wrapper, false> at(std::size_t i) & { return values.at(i); }
		ElementReference<Wrapper, true> at(std::size_t i) const& { return values.at(i); }
		Wrapper at(std::size_t i)&&
		{
			Wrapper out;
			out.Attach(std::exchange(values.at(i), nullptr));
			return out;
		}
		Wrapper at(std::size_t i) const&&
		{
			return values.at(i);
		}

		ElementReference<Wrapper, false> front()& { return values.front(); }
		ElementReference<Wrapper, true> front() const& { return values.front(); }

		Wrapper front()&&
		{
			Wrapper out;
			out.Attach(std::exchange(values.front(), nullptr));
			return out;
		}
		Wrapper front() const&& { return values.front(); }

		ElementReference<Wrapper, false> back()& { return values.back(); }
		ElementReference<Wrapper, true> back() const& { return values.back(); }

		Wrapper back()&&
		{
			Wrapper out;
			out.Attach(std::exchange(values.back(), nullptr));
			return out;
		}
		Wrapper back() const&& { return values.back(); }

		auto data() { return values.data(); }
		const auto data() const { return values.data(); }

		ContiguousIterator<Wrapper, false> begin() { return &values[0]; }
		ContiguousIterator<Wrapper, false> end() { return &values[values.size()]; }

		ContiguousIterator<Wrapper, true> begin() const { return &values[0]; }
		ContiguousIterator<Wrapper, true> end() const { return &values[values.size()]; }

		size_t size() const { return values.size(); }

		void push_back(Wrapper p)
		{
			values.push_back(p.Detach());
		}

		void pop_back()
		{
			Wrapper p;
			p.Attach(values.back());
			values.pop_back();
		}

		void resize(size_t newSize)
		{
			if(!empty())
			{
				for(size_t i = size() - 1; i >= newSize; i++)
				{
					Wrapper w;
					w.Attach(values[i]);
				}
			}

			values.resize(newSize);
		}

		bool empty() const { return values.empty(); }

	private:
		void Acquire()
		{
			for(inner_type* ptr : values)
			{
				if(ptr)
					ptr->AddRef();
			}
		}

		void Release()
		{
			for(inner_type* ptr : values)
			{
				if(ptr)
					ptr->Release();
			}
		}
	};


	template<IUnknownWrapper Wrapper, std::convertible_to<Wrapper>... Tys>
	Vector(Wrapper, Tys...) -> Vector<Wrapper>;

	template<IUnknownWrapper Wrapper, std::convertible_to<Wrapper>... Tys>
	Vector(ElementReference<Wrapper, false>, Tys...) -> Vector<Wrapper>;

	template<IUnknownWrapper Wrapper, std::convertible_to<Wrapper>... Tys>
	Vector(ElementReference<Wrapper, true>, Tys...) -> Vector<Wrapper>;

	export template<class Ty, std::size_t N = std::dynamic_extent>
	class Span;

	export template<IUnknownWrapper Wrapper, std::size_t N>
	class Span<Wrapper, N>
	{
		using inner_type = Wrapper::inner_type;

		std::span<std::conditional_t<std::is_const_v<Wrapper>, inner_type* const, inner_type*>, N> values = {};

	public:
		Span() = default;
		Span(const Span&) = default;
		Span(Span&&) noexcept = default;

		Span(inner_type** v, size_t count) requires (N == std::dynamic_extent) :
			values{ v, count }
		{

		}

		Span(ContiguousIterator<Wrapper, std::is_const_v<Wrapper>> begin, ContiguousIterator<Wrapper, std::is_const_v<Wrapper>> end) requires (N == std::dynamic_extent) :
			values{ begin.Raw(), end - begin }
		{

		}

		Span(ContiguousIterator<Wrapper, std::is_const_v<Wrapper>> begin, size_t count) requires (N == std::dynamic_extent) :
			values{ begin.Raw(), count }
		{

		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent)
		Span(const Span<std::remove_const_t<Wrapper>, OtherN>& v) :
			values{ v.data(), v.size() }
		{

		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent) && std::is_const_v<Wrapper>
		Span(const Span<Wrapper, OtherN>& v) :
			values{ v.data(), v.size() }
		{

		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent)
		Span(Array<std::remove_const_t<Wrapper>, OtherN>& v) : values{ v.data(), v.size() }
		{

		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent) && std::is_const_v<Wrapper>
		Span(const Array<std::remove_const_t<Wrapper>, OtherN>& v) : values{ v.data(), v.size() }
		{

		}

		Span(Vector<std::remove_const_t<Wrapper>>& v) : values{ v.data(), v.size() }
		{

		}

		Span(const Vector<std::remove_const_t<Wrapper>>& v) : values{ v.data(), v.size() }
		{

		}

		Span& operator=(const Span&) = default;
		Span& operator=(Span&&) noexcept = default;

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent)
		Span& operator=(const Span<std::remove_const_t<Wrapper>, OtherN>& v)
		{
			values = decltype(values){ v.data(), v.size() };
			return *this;
		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent) && std::is_const_v<Wrapper>
		Span& operator=(const Span<Wrapper, OtherN>& v)
		{
			values = decltype(values){ v.data(), v.size() };
			return *this;
		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent)
		Span& operator=(Array<std::remove_const_t<Wrapper>, OtherN>& v)
		{
			values = decltype(values){ v.data(), v.size() };
			return *this;
		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent)
		Span& operator=(const Array<std::remove_const_t<Wrapper>, OtherN>& v)
		{
			values = decltype(values){ v.data(), v.size() };
			return *this;
		}

		Span& operator=(Vector<std::remove_const_t<Wrapper>>& v)
		{
			values = decltype(values){ v.data(), v.size() };
			return *this;
		}

		Span& operator=(const Vector<std::remove_const_t<Wrapper>>& v)
		{
			values = decltype(values){ v.data(), v.size() };
			return *this;
		}

		ElementReference<Wrapper, false> operator[](std::size_t i) & { return values[i]; }
		ElementReference<Wrapper, true> operator[](std::size_t i) const& { return values[i]; }
		Wrapper operator[](std::size_t i)&& 
		{
			Wrapper out;
			out.Attach(std::exchange(std::move(values)[i], nullptr));
			return out;
		}
		Wrapper operator[](std::size_t i) const&& 
		{
			return std::move(values)[i];
		}

		ElementReference<Wrapper, false> at(std::size_t i) & { return values.at(i); }
		ElementReference<Wrapper, true> at(std::size_t i) const& { return values.at(i); }
		Wrapper at(std::size_t i)&&
		{
			Wrapper out;
			out.Attach(std::exchange(std::move(values).at(i), nullptr));
			return out;
		}
		Wrapper at(std::size_t i) const&&
		{
			return std::move(values).at(i);
		}

		ElementReference<Wrapper, false> front()& { return values.front(); }
		ElementReference<Wrapper, true> front() const& { return values.front(); }

		Wrapper front()&&
		{
			Wrapper out;
			out.Attach(std::exchange(std::move(values).front(), nullptr));
			return out;
		}
		Wrapper front() const&& { return std::move(values).front(); }

		ElementReference<Wrapper, false> back()& { return values.back(); }
		ElementReference<Wrapper, true> back() const& { return values.back(); }

		Wrapper back()&&
		{
			Wrapper out;
			out.Attach(std::exchange(std::move(values).back(), nullptr));
			return out;
		}
		Wrapper back() const&& { return std::move(values).back(); }

		auto data() { return values.data(); }
		const auto data() const { return values.data(); }

		ContiguousIterator<Wrapper, false> begin() { return &values[0]; }
		ContiguousIterator<Wrapper, false> end() { return &values[values.size()]; }

		ContiguousIterator<Wrapper, true> begin() const { return &values[0]; }
		ContiguousIterator<Wrapper, true> end() const { return &values[values.size()]; }

		size_t size() const { return values.size(); }
	};

	template<IUnknownWrapper Wrapper, std::size_t N>
	Span(Array<Wrapper, N>) -> Span<Wrapper, N>;

	template<IUnknownWrapper Wrapper, bool IsConst>
	Span(ContiguousIterator<Wrapper, IsConst>, ContiguousIterator<Wrapper, IsConst>) -> Span<Wrapper>;

	template<IUnknownWrapper Wrapper, bool IsConst>
	Span(ContiguousIterator<Wrapper, IsConst>, size_t) -> Span<Wrapper>;




	export template<class Wrapper, bool IsConst>
		requires std::same_as<TypedStruct<InnerType<Wrapper>>, std::remove_const_t<Wrapper>>
	class ElementReference<Wrapper, IsConst> : public InnerType<Wrapper>::template Interface<ElementReference<Wrapper, IsConst>>
	{
		using trait_type = InnerType<Wrapper>;
		using inner_type = InnerType<trait_type>;

	private:
		std::conditional_t<IsConst, const inner_type&, inner_type&> ref;

	public:
		ElementReference(inner_type& ref) : ref{ ref }
		{

		}


	public:
		inner_type& Raw() noexcept { return ref; }
		const inner_type& Raw() const noexcept { return ref; }

		operator Wrapper() const { return ref; }
	};

	export template<class Wrapper, std::size_t N>
		requires std::same_as<TypedStruct<InnerType<Wrapper>>, std::remove_const_t<Wrapper>>
	class Array<Wrapper, N>
	{
		using trait_type = InnerType<Wrapper>;
		using inner_type = InnerType<trait_type>;

		std::array<inner_type, N> values = {};

	public:
		Array() = default;
		Array(std::initializer_list<inner_type> il) : values{ il }
		{

		}
		Array(std::initializer_list<Wrapper> il)
		{
			std::transform(il.begin(), il.end(), values.begin(), [](const Wrapper& w) {  return w.Raw(); });
		}

		ElementReference<Wrapper, false> operator[](std::size_t i)& { return values[i]; }
		ElementReference<Wrapper, true> operator[](std::size_t i) const& { return values[i]; }
		Wrapper operator[](std::size_t i)&& { std::move(values)[i]; }
		Wrapper operator[](std::size_t i) const&&
		{
			return std::move(values)[i];
		}

		ElementReference<Wrapper, false> at(std::size_t i)& { return values.at(i); }
		ElementReference<Wrapper, true> at(std::size_t i) const& { return values.at(i); }
		Wrapper at(std::size_t i)&& { std::move(values).at(i); }
		Wrapper at(std::size_t i) const&&
		{
			return std::move(values).at(i);
		}

		ElementReference<Wrapper, false> front()& { return values.front(); }
		ElementReference<Wrapper, true> front() const& { return values.front(); }

		Wrapper front()&& { std::move(values).front(); }
		Wrapper front() const&& { return std::move(values).front(); }

		ElementReference<Wrapper, false> back()& { return values.back(); }
		ElementReference<Wrapper, true> back() const& { return values.back(); }

		Wrapper back()&& { return std::move(values).back(); }
		Wrapper back() const&& { return std::move(values).back(); }

		auto data() { return values.data(); }
		const auto data() const { return values.data(); }

		ContiguousIterator<Wrapper, false> begin() { return &values[0]; }
		ContiguousIterator<Wrapper, false> end() { return &values[values.size()]; }

		ContiguousIterator<Wrapper, true> begin() const { return &values[0]; }
		ContiguousIterator<Wrapper, true> end() const { return &values[values.size()]; }

		size_t size() const { return values.size(); }
	};

	export template<class Wrapper, std::size_t N>
		requires std::same_as<TypedStruct<InnerType<Wrapper>>, std::remove_const_t<Wrapper>>
	class Span<Wrapper, N>
	{
		using trait_type = InnerType<Wrapper>;
		using inner_type = InnerType<trait_type>;

		std::span<std::conditional_t<std::is_const_v<Wrapper>, const inner_type, inner_type>, N> values = {};

	public:
		Span() = default;
		Span(const Span&) = default;
		Span(Span&&) noexcept = default;

		Span(inner_type* v, size_t count) requires (N == std::dynamic_extent) :
			values{ v, count }
		{

		}

		Span(ContiguousIterator<Wrapper, std::is_const_v<Wrapper>> begin, ContiguousIterator<Wrapper, std::is_const_v<Wrapper>> end) requires (N == std::dynamic_extent) :
			values{ begin.Raw(), end - begin }
		{

		}

		Span(ContiguousIterator<Wrapper, std::is_const_v<Wrapper>> begin, size_t count) requires (N == std::dynamic_extent) :
			values{ begin.Raw(), count }
		{

		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent)
		Span(const Span<std::remove_const_t<Wrapper>, OtherN>& v) :
			values{ v.data(), v.size() }
		{

		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent) && std::is_const_v<Wrapper>
		Span(const Span<Wrapper, OtherN>& v) :
			values{ v.data(), v.size() }
		{

		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent)
		Span(Array<std::remove_const_t<Wrapper>, OtherN>& v) : values{ v.data(), v.size() }
		{

		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent) && std::is_const_v<Wrapper>
		Span(const Array<std::remove_const_t<Wrapper>, OtherN>& v) : values{ v.data(), v.size() }
		{

		}

		//Span(Vector<Wrapper>& v) : values{ v.data(), v.size() }
		//{

		//}

		//Span(const Vector<Wrapper>& v) : values{ v.data(), v.size() }
		//{

		//}

		Span& operator=(const Span&) = default;
		Span& operator=(Span&&) noexcept = default;

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent)
		Span& operator=(const Span<std::remove_const_t<Wrapper>, OtherN>& v)
		{
			values = decltype(values){ v.data(), v.size() };
			return *this;
		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent) && std::is_const_v<Wrapper>
		Span& operator=(const Span<Wrapper, OtherN> & v)
		{
			values = decltype(values){ v.data(), v.size() };
			return *this;
		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent)
		Span& operator=(Array<std::remove_const_t<Wrapper>, OtherN>& v)
		{
			values = decltype(values){ v.data(), v.size() };
			return *this;
		}

		template<size_t OtherN>
			requires (OtherN == N || N == std::dynamic_extent)
		Span& operator=(const Array<std::remove_const_t<Wrapper>, OtherN>& v)
		{
			values = decltype(values){ v.data(), v.size() };
			return *this;
		}

		//Span& operator=(Vector<Wrapper>& v)
		//{
		//	values = decltype(values){ v.data(), v.size() };
		//	return *this;
		//}

		//Span& operator=(const Vector<Wrapper>& v)
		//{
		//	values = decltype(values){ v.data(), v.size() };
		//	return *this;
		//}

		ElementReference<Wrapper, false> operator[](std::size_t i)& { return values[i]; }
		ElementReference<Wrapper, true> operator[](std::size_t i) const& { return values[i]; }
		Wrapper operator[](std::size_t i)&&
		{
			return std::move(values)[i];
		}
		Wrapper operator[](std::size_t i) const&&
		{
			return std::move(values)[i];
		}

		ElementReference<Wrapper, false> at(std::size_t i)& { return values.at(i); }
		ElementReference<Wrapper, true> at(std::size_t i) const& { return values.at(i); }
		Wrapper at(std::size_t i)&&
		{
			return std::move(values).at(i);
		}
		Wrapper at(std::size_t i) const&&
		{
			return std::move(values).at(i);
		}

		ElementReference<Wrapper, false> front()& { return values.front(); }
		ElementReference<Wrapper, true> front() const& { return values.front(); }

		Wrapper front()&& { return std::move(values).front(); }
		Wrapper front() const&& { return std::move(values).front(); }

		ElementReference<Wrapper, false> back()& { return values.back(); }
		ElementReference<Wrapper, true> back() const& { return values.back(); }

		Wrapper back()&& { return std::move(values).back(); }
		Wrapper back() const&& { return std::move(values).back(); }

		auto data() { return values.data(); }
		const auto data() const { return values.data(); }

		ContiguousIterator<Wrapper, false> begin() { return &values[0]; }
		ContiguousIterator<Wrapper, false> end() { return &values[values.size()]; }

		ContiguousIterator<Wrapper, true> begin() const { return &values[0]; }
		ContiguousIterator<Wrapper, true> end() const { return &values[values.size()]; }

		size_t size() const { return values.size(); }
	};

	template<class Ty, std::size_t N>
	Span(Array<TypedStruct<Ty>, N>) -> Span<TypedStruct<Ty>, N>;
}