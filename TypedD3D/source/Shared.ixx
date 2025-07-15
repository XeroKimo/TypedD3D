module;

#include <format>
#include <wrl/client.h>
#include <minwindef.h>
#include <concepts>
#include <stdexcept>
#include <array>
#include <span>
#include <vector>

export module TypedD3D.Shared;
import xk.Exceptions;

namespace TypedD3D
{
	export class ErrorModule
	{
	public:
		using exception_tag = xk::ErrorModuleTag;
	};
}

namespace xk
{
	template<>
	struct ExceptionData<TypedD3D::ErrorModule>
	{
		HRESULT result;
		std::string what;

		ExceptionData(HRESULT result, std::string context) :
			result{ result },
			what{ std::format("HRESULT: {:x}. {}", result, context) }
		{

		}

		std::string_view What() const noexcept { return what; }
	};
}

namespace TypedD3D
{
	using Microsoft::WRL::ComPtr;

	template<class T>
	concept AlwaysFalse = false;

	export template<class To, class From>
		ComPtr<To> Cast(const ComPtr<From>& from)
	{
		ComPtr<To> to;
		from.As(&to);
		return to;
	}

	export template<class To, class From>
		ComPtr<To> Cast(ComPtr<From>&& from) noexcept
	{
		ComPtr<To> to;
		from.As(&to);
		return to;
	}

	export template<class To, class From>
		To* Cast(From* from)
	{
		To* to;
		from->QueryInterface(&to);
		return to;
	}

	export void ThrowIfFailed(HRESULT hr, std::string context = "")
	{
		if (FAILED(hr))
			throw xk::Exception<ErrorModule>{ { hr, context } };
	}

	/// <summary>
	/// Forwards a function which creates or gets an COM object which would require querying it's IID
	/// </summary>
	/// <typeparam name="Unknown"></typeparam>
	/// <typeparam name="Func"></typeparam>
	/// <typeparam name="Obj"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="function">Function who's last 2 parameters is IID riid and void** ppv</param>
	/// <param name="...args">All arguments not including the IID and void**</param>
	/// <returns></returns>
	export template<class Unknown, class Func, class... Args>
		ComPtr<Unknown> IIDToObjectForwardFunction(Func&& function, Args&&... args)
	{
		ComPtr<Unknown> unknown;

		constexpr bool invocableHRESULT = requires() { { std::invoke(function, args..., IID_PPV_ARGS(&unknown)) } -> std::convertible_to<HRESULT>; };
		constexpr bool invocableVoid = requires() { { std::invoke(function, args..., IID_PPV_ARGS(&unknown)) } -> std::same_as<void>; };

		if constexpr (invocableHRESULT)
			ThrowIfFailed(std::invoke(function, args..., IID_PPV_ARGS(&unknown)));
		else if constexpr (invocableVoid)
			std::invoke(function, args..., IID_PPV_ARGS(&unknown));
		else
			static_assert(std::invocable<Func, Args..., const IID&, void**> && AlwaysFalse<Func>, "Forward function has some different return type that isn't handled properly");

		return unknown;
	}

	/// <summary>
	/// Forwards a function which creates or gets an COM object
	/// </summary>
	/// <typeparam name="Unknown"></typeparam>
	/// <typeparam name="Func"></typeparam>
	/// <typeparam name="Obj"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="function">Function who's last parameter inherits IUnknown</param>
	/// <param name="...args">All arguments not including the IUnknown param</param>
	/// <returns></returns>
	export template<class Unknown, class Func, class... Args>
		ComPtr<Unknown> UnknownObjectForwardFunction(Func&& function, Args&&... args)
	{
		ComPtr<Unknown> unknown;

		constexpr bool invocableHRESULT = requires() { { std::invoke(function, args..., &unknown) } -> std::convertible_to<HRESULT>; };
		constexpr bool invocableVoid = requires() { { std::invoke(function, args..., &unknown) } -> std::same_as<void>; };

		if constexpr (invocableHRESULT)
			ThrowIfFailed(std::invoke(function, args..., &unknown));
		else if constexpr (invocableVoid)
			std::invoke(function, args..., &unknown);
		else
			static_assert(std::invocable<Func, Args..., const IID&, void**> && AlwaysFalse<Func>, "Forward function has some different return type that isn't handled properly");

		return unknown;
	}

	export template<class Ty>
	constexpr bool is_unknown_wrapper = false;

	template<class Ty>
	struct GetTraitTemplate : std::false_type {};

	template<template<class> class Trait, class Ty>
	struct GetTraitTemplate<Trait<Ty>> : std::true_type
	{
		template<class T>
		using type = Trait<T>;
	};

	template<class Ty>
	concept IsTrait = (GetTraitTemplate<Ty>::value == true);

	template<class A, class B, class Ty>
	concept IsSameTrait = std::same_as<typename GetTraitTemplate<A>::template type<Ty>, typename GetTraitTemplate<B>::template type<Ty>>;

	export template<class Ty, IsTrait Traits>
	class IUnknownWrapperImpl
	{
		template<class OtherTy, IsTrait OtherTrait>
		friend class IUnknownWrapperImpl;

	public:
		using value_type = Ty;
		using pointer = Ty*;
		using reference = Ty&;

	public:
		template<class Ty2>
		using traits_template_type = GetTraitTemplate<Traits>::template type<Ty2>;
		using traits_type = Traits;

	private:
		struct Impl;
		using interface_type = traits_type::template Interface<Impl>;

		struct Impl : public interface_type
		{
			ComPtr<value_type> ptr;

			Impl() = default;

			template<class U>
			Impl(U* obj) : ptr{ obj }
			{

			}

			Impl(const Microsoft::WRL::ComPtr<Ty>& other) : ptr{ other }
			{
			}

			Impl(Microsoft::WRL::ComPtr<Ty>&& other) noexcept : ptr{ std::move(other) }
			{
			}

			template<class OtherTy>
			Impl(const Microsoft::WRL::ComPtr<OtherTy>& other) : ptr{ other }
			{
			}

			template<class OtherTy>
			Impl(Microsoft::WRL::ComPtr<OtherTy>&& other) noexcept : ptr{ std::move(other) }
			{
			}

			pointer Get() { return ptr.Get(); }
			pointer Get() const { return ptr.Get(); }
		};

		Impl impl;

	public:
		IUnknownWrapperImpl() = default;
		IUnknownWrapperImpl(const IUnknownWrapperImpl& other) = default;
		IUnknownWrapperImpl(IUnknownWrapperImpl&& other) noexcept = default;

		template<class OtherTy, class OtherTraits>
			requires IsSameTrait<Traits, OtherTraits, Ty>
		IUnknownWrapperImpl(const IUnknownWrapperImpl<OtherTy, OtherTraits>& other) : impl{ other.impl.ptr }
		{
		}

		template<class OtherTy, class OtherTraits>
			requires IsSameTrait<Traits, OtherTraits, Ty>
		IUnknownWrapperImpl(IUnknownWrapperImpl<OtherTy, OtherTraits>&& other) noexcept : impl{ std::move(other).impl.ptr }
		{
		}

		IUnknownWrapperImpl(std::nullptr_t) {};

		template<class U>
		IUnknownWrapperImpl(U* obj) : impl{ obj }
		{
		}

		IUnknownWrapperImpl(const Microsoft::WRL::ComPtr<Ty>& other) : impl{ other }
		{
		}

		IUnknownWrapperImpl(Microsoft::WRL::ComPtr<Ty>&& other) noexcept : impl{ std::move(other) }
		{
		}

		template<class OtherTy>
		IUnknownWrapperImpl(const Microsoft::WRL::ComPtr<OtherTy>& other) : impl{ other }
		{
		}

		template<class OtherTy>
		IUnknownWrapperImpl(Microsoft::WRL::ComPtr<OtherTy>&& other) noexcept : impl{ std::move(other) }
		{
		}

	public:
		IUnknownWrapperImpl& operator=(const IUnknownWrapperImpl& other) = default;
		IUnknownWrapperImpl& operator=(IUnknownWrapperImpl&& other) noexcept = default;

		IUnknownWrapperImpl& operator=(std::nullptr_t)
		{
			impl.ptr = nullptr;
			return *this;
		}

		template<class OtherTy>
		IUnknownWrapperImpl& operator=(const IUnknownWrapperImpl<OtherTy, Traits>& other)
		{
			impl.ptr = other.impl.ptr;
			return *this;
		}

		template<class OtherTy>
		IUnknownWrapperImpl& operator=(IUnknownWrapperImpl<OtherTy, Traits>&& other) noexcept
		{
			impl.ptr = std::move(other).impl.ptr;
			return *this;
		}

	public:
		friend bool operator==(const IUnknownWrapperImpl& lh, std::nullptr_t)
		{
			return lh.impl.ptr == nullptr;
		}
		friend bool operator!=(const IUnknownWrapperImpl& lh, std::nullptr_t)
		{
			return lh.impl.ptr != nullptr;
		}

		template<class OtherTy>
		friend bool operator==(const IUnknownWrapperImpl& lh, const IUnknownWrapperImpl<OtherTy, Traits>& rh)
		{
			return lh.impl.ptr == rh.impl.ptr;
		}
		template<class OtherTy>
		friend bool operator!=(const IUnknownWrapperImpl& lh, const IUnknownWrapperImpl<OtherTy, Traits>& rh)
		{
			return lh.impl.ptr != rh.impl.ptr;
		}

		operator bool() const { return impl.ptr != nullptr; }

	public:
		pointer Detach() { return impl.ptr.Detach(); }

		void Attach(pointer p)
		{
			impl.ptr.Attach(p);
		}

		pointer Get() { return impl.ptr.Get(); }
		pointer Get() const { return impl.ptr.Get(); }

		void Swap(IUnknownWrapperImpl& other) noexcept
		{
			impl.ptr.Swap(other.impl.ptr);
		}
		pointer* GetAddressOf() { return impl.ptr.GetAddressOf(); }
		const pointer* GetAddressOf() const { return impl.ptr.GetAddressOf(); }
		interface_type* operator->() { return &impl; }
		const interface_type* operator->() const { return &impl; }
		Microsoft::WRL::ComPtr<value_type> AsComPtr() const { return impl.ptr; }

		template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, class Traits>
		friend IUnknownWrapperImpl<DerivedTy, typename GetTraitTemplate<Traits>::template type<DerivedTy>> Cast(const IUnknownWrapperImpl<Ty, Traits>& other) noexcept;

		template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, class Traits>
		friend IUnknownWrapperImpl<DerivedTy, typename GetTraitTemplate<Traits>::template type<DerivedTy>> Cast(IUnknownWrapperImpl<Ty, Traits>&& other) noexcept;

		template<class DerivedTy, std::derived_from<IUnknown> Ty, class Traits>
			requires is_unknown_wrapper<DerivedTy> && IsSameTrait<typename DerivedTy::traits_type, Traits, Ty>
		friend DerivedTy Cast(const IUnknownWrapperImpl<Ty, Traits>& other) noexcept;

		template<class DerivedTy, std::derived_from<IUnknown> Ty, class Traits>
			requires is_unknown_wrapper<DerivedTy> && IsSameTrait<typename DerivedTy::traits_type, Traits, Ty>
		friend DerivedTy Cast(IUnknownWrapperImpl<Ty, Traits>&& other) noexcept;
	};

	export template<class Ty, class Traits>
	constexpr bool is_unknown_wrapper<IUnknownWrapperImpl<Ty, Traits>> = true;

	export template<class Ty, class Traits>
	constexpr bool is_unknown_wrapper<const IUnknownWrapperImpl<Ty, Traits>> = true;

	export template<class Ty, template<class> class Traits>
	using IUnknownWrapper = IUnknownWrapperImpl<Ty, Traits<Ty>>;

	
	export template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, class Traits>
	IUnknownWrapperImpl<DerivedTy, typename GetTraitTemplate<Traits>::template type<DerivedTy>> Cast(const IUnknownWrapperImpl<Ty, Traits>& other) noexcept
	{
		return { Cast<DerivedTy>(other.impl.ptr) };
	}

	export template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, class Traits>
	IUnknownWrapperImpl<DerivedTy, typename GetTraitTemplate<Traits>::template type<DerivedTy>> Cast(IUnknownWrapperImpl<Ty, Traits>&& other) noexcept
	{
		return { Cast<DerivedTy>(std::move(other.impl.ptr)) };
	}

	export template<class DerivedTy, std::derived_from<IUnknown> Ty, class Traits>
		requires is_unknown_wrapper<DerivedTy> && IsSameTrait<typename DerivedTy::traits_type, Traits, Ty>
	DerivedTy Cast(const IUnknownWrapperImpl<Ty, Traits>& other) noexcept
	{
		return { Cast<typename DerivedTy::value_type>(other.impl.ptr) };
	}

	export template<class DerivedTy, std::derived_from<IUnknown> Ty, class Traits>
		requires is_unknown_wrapper<DerivedTy> && IsSameTrait<typename DerivedTy::traits_type, Traits, Ty>
	DerivedTy Cast(IUnknownWrapperImpl<Ty, Traits>&& other) noexcept
	{
		return { Cast<typename DerivedTy::value_type>(std::move(other.impl.ptr)) };
	}

	export template<class Ty, bool IsConst>
		requires is_unknown_wrapper<Ty>
	class ElementReferenceT
	{
		using value_type = Ty::value_type;
		using pointer = Ty::pointer;
		using reference = Ty::reference;

		struct Ref;
		using interface_type = Ty::traits_type::template Interface<Ref>;
		struct Ref : public interface_type
		{
			pointer& ptr;

			pointer Get() { return ptr; }
			pointer Get() const { return ptr; }

			Ref() = default;
			Ref(pointer& ptr) :
				ptr{ ptr }
			{

			}

			Ref& operator=(std::nullptr_t) requires (!IsConst)
			{
				if (ptr)
					ptr->Release();
				ptr = nullptr;

				return *this;
			}

			Ref& operator=(const Ty& other) requires (!IsConst)
			{
				if (ptr)
					ptr->Release();
				ptr = other.Get();
				if (ptr)
					ptr->AddRef();
				return *this;
			}

			Ref& operator=(Ty&& other) noexcept requires (!IsConst)
			{
				if (ptr)
					ptr->Release();
				ptr = other.Detach();
				return *this;
			}

			Ref& operator=(const Ref& other) requires (!IsConst)
			{
				if (ptr)
					ptr->Release();
				ptr = other.Get();
				if (ptr)
					ptr->AddRef();
				return *this;
			}

			Ref& operator=(Ref&& other) noexcept requires (!IsConst)
			{
				if (ptr)
					ptr->Release();
				ptr = std::exchange(other.ptr, nullptr);
				return *this;
			}

			void Swap(Ty& other) noexcept requires (!IsConst)
			{
				other.Attach(std::exchange(ptr, other.Detach()));
			}
		};


		Ref ref;

	public:
		ElementReferenceT(pointer& ptr) :
			ref{ ptr }
		{
		}

		ElementReferenceT(Ty& ptr) :
			ref{ ptr.GetAddressOf() }
		{

		}

		ElementReferenceT& operator=(std::nullptr_t) requires (!IsConst)
		{
			ref = nullptr;
			return *this;
		}

		ElementReferenceT& operator=(const Ty& other)  requires (!IsConst)
		{
			ref = other;
			return *this;
		}

		ElementReferenceT& operator=(Ty&& other) noexcept requires (!IsConst)
		{
			ref = std::move(other);
			return *this;
		}

		ElementReferenceT& operator=(const ElementReferenceT& other)  requires (!IsConst)
		{
			ref = other.ref;
			return *this;
		}

		ElementReferenceT& operator=(ElementReferenceT&& other) noexcept requires (!IsConst)
		{
			ref = std::move(other).ref;
			return *this;
		}

		void Swap(Ty& other) noexcept requires (!IsConst)
		{
			ref.Swap(other);
		}

		pointer Get() { return ref.ptr; }
		pointer Get() const { return ref.ptr; }
		pointer Detach()  requires (!IsConst) { return std::exchange(ref.ptr, nullptr); }

		void Attach(pointer p) requires (!IsConst)
		{
			if (ref.ptr)
				ref.ptr->Release();

			ref.ptr = p;
		}

		Ty ToWrapper() const { return Ty{ ref.ptr }; }

		template<class OtherTy>
			requires std::convertible_to<Ty, OtherTy>
		operator OtherTy() const& { return OtherTy{ ref.ptr }; }

		interface_type* operator->() { return &ref; }
		interface_type* operator->() const { return &ref; }
	};

	export template<class Ty>
		using ElementReference = ElementReferenceT<Ty, false>;

	export template<class Ty>
		using ConstElementReference = ElementReferenceT<Ty, true>;



	export template<class Ty, size_t Extent>
		requires is_unknown_wrapper<Ty>
	class Array
	{
	public:
		using value_type = Ty::pointer;
		using pointer = Ty::pointer*;
		using reference = Ty::reference;

	public:
		template<class Ty2>
		using traits_template_type = Ty::template traits_template_type<Ty2>;
		using traits_type = Ty::traits_type;

	public:
		std::array<value_type, Extent> _values{};

		Array() = default;

		Array(const Array& other) :
			_values{ other._values }
		{
			for (value_type p : _values)
			{
				if (p)
				{
					auto foo = p->AddRef();
					int i = 0;
				}
			}
		}

		Array(Array&& other) noexcept
		{
			for (size_t i = 0; i < Extent; i++)
			{
				std::swap(other._values[i], _values[i]);
			}
		}

		template<std::convertible_to<Ty>... Ty2>
		Array(Ty2... values) :
			_values{ values.Get()... }
		{
			for (value_type p : _values)
			{
				if (p)
				{
					auto foo = p->AddRef();
					int i = 0;
				}
			}
		}

		~Array()
		{
			for (value_type p : _values)
			{
				if (p)
				{
					auto foo = p->Release();
					int i = 0;
				}
			}
		}

		Array& operator=(const Array& other)
		{
			Array temp{ other };
			_values.swap(temp._values);
			return *this;
		}

		Array& operator=(Array&& other) noexcept
		{
			Array temp{ std::move(other) };
			_values.swap(temp._values);
			return *this;
		}

	public:
		ElementReference<Ty> operator[](size_t index)
		{
			return { _values[index] };
		}

		ConstElementReference<Ty> operator[](size_t index) const
		{
			return { _values[index] };
		}

		explicit operator std::array<value_type, Extent>() const { return _values; }
	public:
		ElementReference<Ty> at(size_t index) { return { _values.at(index) }; }
		ElementReference<Ty> front() { return { _values.front() }; }
		ElementReference<Ty> back() { return { _values.back() }; }

		ConstElementReference<Ty> at(size_t index) const { return { _values.at(index) }; }
		ConstElementReference<Ty> front() const { return { _values.front() }; }
		ConstElementReference<Ty> back() const { return { _values.back() }; }
		pointer data() noexcept { return _values.data(); }
		const pointer data() const noexcept { return _values.data(); }
		size_t size() const noexcept { return _values.size(); }
		bool empty() const noexcept { return _values.empty(); }
		bool max_size() const noexcept { return _values.max_size(); }

		std::array<value_type, Extent> ToUntyped() const { return _values; }
	};

	export template<class Ty, class Allocator = std::allocator<typename Ty::pointer>>
		requires is_unknown_wrapper<Ty>
	class Vector
	{
	public:
		using wrapper_type = Ty;
		using value_type = Ty::pointer;
		using pointer = Ty::pointer*;
		using reference = Ty::reference;

	public:
		template<class Ty2>
		using traits_template_type = Ty::template traits_template_type<Ty2>;
		using traits_type = Ty::traits_type;

	private:
		std::vector<value_type, Allocator> _values;

	public:
		Vector() = default;
		Vector(const Vector& other) :
			_values{ other._values }
		{
			for (Ty v : _values)
			{
				if (v)
					v.Get()->AddRef();
			}
		}

		Vector(Vector&& other) noexcept
		{
			for (size_t i = 0; i < _values.size(); i++)
			{
				std::swap(other._values[i], _values[i]);
			}
		}

		Vector(std::initializer_list<Ty> list)
		{
			for (Ty v : list)
			{
				push_back(v);

				if(v)
					v.Get()->AddRef();
			}
		}

		~Vector()
		{
			for (value_type v : _values)
			{
				if (v)
					v->Release();
			}
		}

		Vector& operator=(const Vector& other)
		{
			Vector temp{ other };
			_values.swap(temp._values);
			return *this;
		}

		Vector& operator=(Vector&& other) noexcept
		{
			Vector temp{ std::move(other) };
			_values.swap(temp._values);
			return *this;
		}

	public:
		ElementReference<Ty> operator[](size_t index)
		{
			return { _values[index] };
		}

		ConstElementReference<Ty> operator[](size_t index) const
		{
			return { _values[index] };
		}

		explicit operator std::vector<value_type, Allocator>() const { return _values; }
	public:
		ElementReference<Ty> at(size_t index) { return { _values.at(index) }; }
		ElementReference<Ty> front() { return { _values.front() }; }
		ElementReference<Ty> back() { return { _values.back() }; }

		ConstElementReference<Ty> at(size_t index) const { return { _values.at(index) }; }
		ConstElementReference<Ty> front() const { return { _values.front() }; }
		ConstElementReference<Ty> back() const { return { _values.back() }; }
		auto data() noexcept { return _values.data(); }
		const auto data() const noexcept { return _values.data(); }
		size_t size() const noexcept { return _values.size(); }
		bool empty() const noexcept { return _values.empty(); }
		bool max_size() const noexcept { return _values.max_size(); }
		void reserve(size_t count) { _values.reserve(count); }
		size_t capacity() { return _values.capacity(); }

		void push_back(const Ty& value) { _values.push_back(value.Get()); }
		void push_back(Ty&& value) { _values.push_back(value.Detach()); }
		void pop_back() { _values.pop_back(); }

		void swap(Vector& other) noexcept { _values.swap(other._values); }
		void resize(size_t count) noexcept { _values.resize(count); }
		void resize(size_t count, const Ty& value) noexcept { _values.resize(count, value.Get()); }
		void clear() { _values.clear(); }

		std::vector<value_type, Allocator> ToUntyped() const { return _values; }
	};

	export template<class Ty, size_t Extent = std::dynamic_extent>
		requires is_unknown_wrapper<Ty>
	class Span
	{

	public:
		using value_type = std::conditional_t<std::is_const_v<Ty>, const typename Ty::pointer, typename Ty::pointer>;
		using pointer = Ty::pointer*;
		using reference = Ty::reference;

	public:
		template<class Ty2>
		using traits_template_type = Ty::template traits_template_type<Ty2>;
		using traits_type = Ty::traits_type;

	private:
		std::span<value_type, Extent> m_span;

	public:
		Span() requires (Extent == std::dynamic_extent) = default;

		template<size_t OtherExtent>
			requires(OtherExtent == Extent || Extent == std::dynamic_extent)
		Span(Array<Ty, OtherExtent>& arr) :
			m_span{ arr.data(), arr.size() }
		{

		}

		explicit(Extent != std::dynamic_extent)
			Span(pointer p, size_t count) :
			m_span{ p, count }
		{

		}
		template<class Range>
		explicit(Extent != std::dynamic_extent)
		Span(Range&& range) :
			m_span{ range.data(), range.size() }
		{

		}
	public:
		ElementReference<Ty> operator[](size_t index)
		{
			return { m_span[index] };
		}

		ConstElementReference<Ty> operator[](size_t index) const
		{
			return { m_span[index] };
		}

	public:
		ElementReference<Ty> front() { return { m_span.front() }; }
		ElementReference<Ty> back() { return { m_span.back() }; }

		ConstElementReference<Ty> front() const { return { m_span.front() }; }
		ConstElementReference<Ty> back() const { return { m_span.back() }; }
		auto data() const noexcept { return m_span.data(); }
		size_t size() const noexcept { return m_span.size(); }


		std::span<pointer, Extent> ToUntyped() const { return m_span; }
	};

	template<class Ty, size_t Extent>
	Span(Array<Ty, Extent>&) -> Span<Ty, Extent>;

	template<class Range>
	Span(Range&&) -> Span<typename Range::wrapper_type>;

	template<class Range>
	Span(const Range&) -> Span<const typename Range::wrapper_type>;

	export template<class Ty>
		struct UntaggedTraits;

	export template<class Ty>
		struct WrapperMapper;

	template<std::derived_from<IUnknown> Ty>
	struct WrapperMapper<Ty>
	{
		using type = IUnknownWrapper<Ty, UntaggedTraits>;
	};

	export template<class Ty>
		using Wrapper = WrapperMapper<Ty>::type;



	template<auto Func>
	struct GetDescriptionHelper;

	template<class ObjTy, class RetVal, class OutVal, RetVal(ObjTy::* Func)(OutVal)>
	struct GetDescriptionHelper<Func>
	{
		using ret_type = RetVal;
		using type = std::remove_pointer_t<OutVal>;

		static type GetDescription(ObjTy& obj)
		{
			type desc;
			std::invoke(Func, obj, &desc);
			return desc;
		}
	};

	template<class ObjTy, class RetVal, RetVal(ObjTy::* Func)()>
	struct GetDescriptionHelper<Func>
	{
		using ret_type = RetVal;
		using type = std::remove_pointer_t<RetVal>;

		static type GetDescription(ObjTy& obj)
		{
			return std::invoke(Func, obj);
		}
	};

	template<class T>
	concept HasDesc1 = requires(T v, typename GetDescriptionHelper<&T::GetDesc1>::type d1)
	{
		{ v.GetDesc1(&d1) } -> std::same_as<typename GetDescriptionHelper<&T::GetDesc1>::ret_type>;
	} || requires(T v)
	{
		{ v.GetDesc1() } -> std::same_as<typename GetDescriptionHelper<&T::GetDesc1>::ret_type>;
	};

	template<class T>
	concept HasDesc2 = requires(T v, typename GetDescriptionHelper<&T::GetDesc2>::type d)
	{
		{ v.GetDesc2(&d) } -> std::same_as<typename GetDescriptionHelper<&T::GetDesc2>::ret_type>;
	} || requires(T v)
	{
		{ v.GetDesc2() } -> std::same_as<typename GetDescriptionHelper<&T::GetDesc2>::ret_type>;
	};

	template<class T>
	concept HasDesc3 = requires(T v, typename GetDescriptionHelper<&T::GetDesc3>::type d)
	{
		{ v.GetDesc3(&d) } -> std::same_as<typename GetDescriptionHelper<&T::GetDesc3>::ret_type>;
	} || requires(T v)
	{
		{ v.GetDesc3() } -> std::same_as<typename GetDescriptionHelper<&T::GetDesc3>::ret_type>;
	};

	export template<class IDXGIObj>
		auto GetDescription(IDXGIObj& obj)
	{
		if constexpr (HasDesc3<IDXGIObj>)
			return GetDescriptionHelper<&IDXGIObj::GetDesc3>::GetDescription(obj);
		else if constexpr (HasDesc2<IDXGIObj>)
			return GetDescriptionHelper<&IDXGIObj::GetDesc2>::GetDescription(obj);
		else if constexpr (HasDesc1<IDXGIObj>)
			return GetDescriptionHelper<&IDXGIObj::GetDesc1>::GetDescription(obj);
		else
			return GetDescriptionHelper<&IDXGIObj::GetDesc>::GetDescription(obj);
	}
}