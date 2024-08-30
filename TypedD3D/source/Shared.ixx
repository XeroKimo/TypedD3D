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
    export class TypedD3DModule
    {
    public:
        using exception_tag = xk::ExceptionModuleTag;
    };
}

namespace xk
{
    template<>
    class ExceptionImpl<TypedD3D::TypedD3DModule>
    {
    private:
        HRESULT result;

    public:
        ExceptionImpl(HRESULT result) :
            result{ result }
        {

        }

    public:
        HRESULT ErrorCode() const noexcept { return result; }
    };
}

namespace TypedD3D
{
	using Microsoft::WRL::ComPtr;

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

    export void ThrowIfFailed(HRESULT hr)
    {
        if(FAILED(hr))
            throw xk::Exception<TypedD3DModule, xk::UnknownException>{ hr };
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

		ThrowIfFailed(std::invoke(function, args..., IID_PPV_ARGS(&unknown)));

		return unknown;
	}

	/// <summary>
	/// Forwards a function which creates or gets an COM object which would require querying it's IID
	/// </summary>
	/// <typeparam name="Unknown"></typeparam>
	/// <typeparam name="Func"></typeparam>
	/// <typeparam name="Obj"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="function">Member Function who's last 2 parameters is IID riid and void** ppv</param>
	/// <param name="obj">Object which the member function will be called with</param>
	/// <param name="...args">All arguments not including the IID and void**</param>
	/// <returns></returns>
	export template<class Unknown, class Func, class Obj, class... Args>
		ComPtr<Unknown> IIDToObjectForwardFunction(Func&& function, Obj&& obj, Args&&... args)
	{
		ComPtr<Unknown> unknown;

		ThrowIfFailed(std::invoke(function, obj, args..., IID_PPV_ARGS(&unknown)));

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

		ThrowIfFailed(std::invoke(function, args..., &unknown));

		return unknown;
	}

	/// <summary>
	/// Forwards a function which creates or gets an COM object
	/// </summary>
	/// <typeparam name="Unknown"></typeparam>
	/// <typeparam name="Func"></typeparam>
	/// <typeparam name="Obj"></typeparam>
	/// <typeparam name="...Args"></typeparam>
	/// <param name="function">Member Function who's last parameter inherits IUnknown</param>
	/// <param name="obj">Object which the member function will be called with</param>
	/// <param name="...args">All arguments not including the IUnknown param</param>
	/// <returns></returns>
	export template<class Unknown, class Func, class Obj, class... Args>
		ComPtr<Unknown> UnknownObjectForwardFunction(Func&& function, Obj&& obj, Args&&... args)
	{
		ComPtr<Unknown> unknown;

		ThrowIfFailed(std::invoke(function, obj, args..., &unknown));

		return unknown;
	}

	export template<class Ty>
	constexpr bool is_unknown_wrapper = false;

	export template<class Ty, template<class> class Traits>
		class IUnknownWrapper
	{
		template<class OtherTy, template<class> class OtherTrait>
		friend class IUnknownWrapper;

	public:
		using value_type = Ty;
		using pointer = Ty*;
		using reference = Ty&;

	public:
		template<class Ty2>
		using traits_template_type = Traits<Ty2>;
		using traits_type = Traits<Ty>;

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
		IUnknownWrapper() = default;
		IUnknownWrapper(const IUnknownWrapper& other) = default;
		IUnknownWrapper(IUnknownWrapper&& other) noexcept = default;

		template<class OtherTy>
		IUnknownWrapper(const IUnknownWrapper<OtherTy, Traits>& other) : impl{ other.impl.ptr }
		{
		}

		template<class OtherTy>
		IUnknownWrapper(IUnknownWrapper<OtherTy, Traits>&& other) noexcept : impl{ std::move(other).impl.ptr }
		{
		}

		//This is here in case of using mappers as Mapper<A>::type, and OtherMapper<A>::type is not seen as the
		//same type for template template classes, despite Mapper<A>::type<B> is the same as OtherMapper<A>::type<B>
		template<class OtherTy, template<class> class OtherTraits>
			requires std::same_as<OtherTraits<Ty>, Traits<Ty>>
		IUnknownWrapper(const IUnknownWrapper<OtherTy, OtherTraits>& other) : impl{ other.impl.ptr }
		{
		}

		template<class OtherTy, template<class> class OtherTraits>
			requires std::same_as<OtherTraits<Ty>, Traits<Ty>>
		IUnknownWrapper(IUnknownWrapper<OtherTy, OtherTraits>&& other) noexcept : impl{ std::move(other).impl.ptr }
		{
		}

		IUnknownWrapper(std::nullptr_t) {};

		template<class U>
		IUnknownWrapper(U* obj) : impl{ obj }
		{
		}

		IUnknownWrapper(const Microsoft::WRL::ComPtr<Ty>& other) : impl{ other }
		{
		}

		IUnknownWrapper(Microsoft::WRL::ComPtr<Ty>&& other) noexcept : impl{ std::move(other) }
		{
		}

		template<class OtherTy>
		IUnknownWrapper(const Microsoft::WRL::ComPtr<OtherTy>& other) : impl{ other }
		{
		}

		template<class OtherTy>
		IUnknownWrapper(Microsoft::WRL::ComPtr<OtherTy>&& other) noexcept : impl{ std::move(other) }
		{
		}

	public:
		IUnknownWrapper& operator=(IUnknownWrapper& other) = default;
		IUnknownWrapper& operator=(IUnknownWrapper&& other) noexcept = default;

		IUnknownWrapper& operator=(std::nullptr_t)
		{
			impl.ptr = nullptr;
			return *this;
		}

		template<class OtherTy>
		IUnknownWrapper& operator=(IUnknownWrapper<OtherTy, Traits>& other)
		{
			impl.ptr = other;
			return *this;
		}

		template<class OtherTy>
		IUnknownWrapper& operator=(IUnknownWrapper<OtherTy, Traits>&& other) noexcept
		{
			impl.ptr = std::move(other).impl.ptr;
			return *this;
		}

	public:
		friend bool operator==(const IUnknownWrapper& lh, std::nullptr_t)
		{
			return lh.impl.ptr == nullptr;
		}
		friend bool operator!=(const IUnknownWrapper& lh, std::nullptr_t)
		{
			return lh.impl.ptr != nullptr;
		}

		template<class OtherTy>
		friend bool operator==(const IUnknownWrapper& lh, const IUnknownWrapper<OtherTy, Traits>& rh)
		{
			return lh.impl.ptr == rh.impl.ptr;
		}
		template<class OtherTy>
		friend bool operator!=(const IUnknownWrapper& lh, const IUnknownWrapper<OtherTy, Traits>& rh)
		{
			return lh.impl.ptr != rh.impl.ptr;
		}

	public:
		pointer Detach() { return impl.ptr.Detach(); }

		void Attach(pointer p)
		{
			impl.ptr.Attach(p);
		}

		pointer Get() { return impl.ptr.Get(); }
		pointer Get() const { return impl.ptr.Get(); }

		void Swap(IUnknownWrapper& other) noexcept
		{
			impl.ptr.Swap(other.impl.ptr);
		}
		pointer* GetAddressOf() { return impl.ptr.GetAddressOf(); }
		const pointer* GetAddressOf() const { return impl.ptr.GetAddressOf(); }
		interface_type* operator->() { return &impl; }
		const interface_type* operator->() const { return &impl; }
		Microsoft::WRL::ComPtr<value_type> AsComPtr() const { return impl.ptr; }

		template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, template<class> class Traits>
		friend IUnknownWrapper<DerivedTy, Traits> Cast(const IUnknownWrapper<Ty, Traits>& other) noexcept;

		template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, template<class> class Traits>
		friend IUnknownWrapper<DerivedTy, Traits> Cast(IUnknownWrapper<Ty, Traits>&& other) noexcept;

		template<class DerivedTy, std::derived_from<IUnknown> CastTy, template<class> class Traits>
			requires is_unknown_wrapper<DerivedTy>
		friend DerivedTy Cast(const IUnknownWrapper<CastTy, Traits>& other) noexcept;

		template<class DerivedTy, std::derived_from<IUnknown> CastTy, template<class> class Traits>
			requires is_unknown_wrapper<DerivedTy>
		friend DerivedTy Cast(IUnknownWrapper<CastTy, Traits>&& other) noexcept;
	};

	export template<class Ty, template<class> class Traits>
		constexpr bool is_unknown_wrapper<IUnknownWrapper<Ty, Traits>> = true;


	export template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, template<class> class Traits>
		IUnknownWrapper<DerivedTy, Traits> Cast(const IUnknownWrapper<Ty, Traits>& other) noexcept
	{
		return IUnknownWrapper<DerivedTy, Traits>(Cast<DerivedTy>(other.impl.ptr));
	}

	export template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, template<class> class Traits>
		IUnknownWrapper<DerivedTy, Traits> Cast(IUnknownWrapper<Ty, Traits>&& other) noexcept
	{
		return IUnknownWrapper<DerivedTy, Traits>(Cast<DerivedTy>(std::move(other.impl.ptr)));
	}

	export template<class DerivedTy, std::derived_from<IUnknown> CastTy, template<class> class Traits>
		requires is_unknown_wrapper<DerivedTy>
	DerivedTy Cast(const IUnknownWrapper<CastTy, Traits>& other) noexcept
	{
		return DerivedTy(TypedD3D::Cast<typename DerivedTy::value_type>(other.impl.ptr));
	}

	export template<class DerivedTy, std::derived_from<IUnknown> CastTy, template<class> class Traits>
		requires is_unknown_wrapper<DerivedTy>
	DerivedTy Cast(IUnknownWrapper<CastTy, Traits>&& other) noexcept
	{
		return DerivedTy(TypedD3D::Cast<typename DerivedTy::value_type>(std::move(other.impl.ptr)));
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
				if(ptr)
					ptr->Release();
				ptr = nullptr;

				return *this;
			}

			Ref& operator=(const Ty& other) requires (!IsConst)
			{
				if(ptr)
					ptr->Release();
				ptr = other.Get();
				if(ptr)
					ptr->AddRef();
				return *this;
			}

			Ref& operator=(Ty&& other) noexcept requires (!IsConst)
			{
				if(ptr)
					ptr->Release();
				ptr = other.Detach();
				return *this;
			}

			Ref& operator=(const Ref& other) requires (!IsConst)
			{
				if(ptr)
					ptr->Release();
				ptr = other.Get();
				if(ptr)
					ptr->AddRef();
				return *this;
			}

			Ref& operator=(Ref&& other) noexcept requires (!IsConst)
			{
				if(ptr)
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
			if(ref.ptr)
				ref.ptr->Release();

			ref.ptr = p;
		}

		operator Ty() const& { return Ty{ ref.ptr }; }
		operator Ty()&& { return Ty{ std::exchange(ref.ptr, nullptr) }; }
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
		using value_type = Ty::value_type;
		using pointer = Ty::pointer;
		using reference = Ty::reference;

	public:
		template<class Ty2>
		using traits_template_type = Ty::template traits_template_type<Ty2>;
		using traits_type = Ty::traits_type;

	public:
		std::array<pointer, Extent> _values;

	public:
		ElementReference<Ty> operator[](size_t index)
		{
			return { _values[index] };
		}

		ConstElementReference<Ty> operator[](size_t index) const
		{
			return { _values[index] };
		}

		explicit operator std::array<pointer, Extent>() const { return _values; }
	public:
		ElementReference<Ty> at(size_t index) { return { _values.at(index) }; }
		ElementReference<Ty> front() { return { _values.front() }; }
		ElementReference<Ty> back() { return { _values.back() }; }

		ConstElementReference<Ty> at(size_t index) const { return { _values.at(index) }; }
		ConstElementReference<Ty> front() const { return { _values.front() }; }
		ConstElementReference<Ty> back() const { return { _values.back() }; }
		pointer* data() noexcept { return _values.data(); }
		const pointer* data() const noexcept { return _values.data(); }
		size_t size() const noexcept { return _values.size(); }
		bool empty() const noexcept { return _values.empty(); }
		bool max_size() const noexcept { return _values.max_size(); }

		std::array<pointer, Extent> ToUntyped() const { return _values; }
	};

	export template<class Ty, class Allocator = std::allocator<typename Ty::pointer>>
		requires is_unknown_wrapper<Ty>
	class Vector
	{
	public:
		using value_type = Ty::value_type;
		using pointer = Ty::pointer;
		using reference = Ty::reference;

	public:
		template<class Ty2>
		using traits_template_type = Ty::template traits_template_type<Ty2>;
		using traits_type = Ty::traits_type;

	private:
		std::vector<pointer, Allocator> _values;

	public:
		Vector() = default;
		Vector(std::initializer_list<Ty> list)
		{
			for(Ty v : list)
			{
				push_back(v);
			}
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

		explicit operator std::vector<pointer, Allocator>() const { return _values; }
	public:
		ElementReference<Ty> at(size_t index) { return { _values.at(index) }; }
		ElementReference<Ty> front() { return { _values.front() }; }
		ElementReference<Ty> back() { return { _values.back() }; }

		ConstElementReference<Ty> at(size_t index) const { return { _values.at(index) }; }
		ConstElementReference<Ty> front() const { return { _values.front() }; }
		ConstElementReference<Ty> back() const { return { _values.back() }; }
		pointer* data() noexcept { return _values.data(); }
		const pointer* data() const noexcept { return _values.data(); }
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

		std::vector<pointer, Allocator> ToUntyped() const { return _values; }
	};

	export template<class Ty, size_t Extent = std::dynamic_extent>
		requires is_unknown_wrapper<Ty>
	class Span
	{
	public:
		using value_type = Ty::value_type;
		using pointer = Ty::pointer;
		using reference = Ty::reference;

	public:
		template<class Ty2>
		using traits_template_type = Ty::template traits_template_type<Ty2>;
		using traits_type = Ty::traits_type;

	private:
		std::span<pointer, Extent> m_span;

	public:
		Span() requires (Extent == std::dynamic_extent) = default;

		template<size_t OtherExtent>
			requires(OtherExtent == Extent || Extent == std::dynamic_extent)
		Span(Array<Ty, OtherExtent>& arr) :
			m_span{ arr.data(), arr.size() }
		{

		}

		explicit(Extent != std::dynamic_extent)
		Span(pointer* p, size_t count) :
			m_span{ p, count }
		{

		}

		explicit(Extent != std::dynamic_extent)
		Span(Vector<Ty>& arr) :
			m_span{ arr.data(), arr.size() }
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
		pointer* data() noexcept { return m_span.data(); }
		const pointer* data() const noexcept { return m_span.data(); }
		size_t size() const noexcept { return m_span.size(); }


		std::span<pointer, Extent> ToUntyped() const { return m_span; }
	};

	template<class Ty, size_t Extent>
	Span(Array<Ty, Extent>&) -> Span<Ty, Extent>;

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
		if constexpr(HasDesc3<IDXGIObj>)
			return GetDescriptionHelper<&IDXGIObj::GetDesc3>::GetDescription(obj);
		else if constexpr(HasDesc2<IDXGIObj>)
			return GetDescriptionHelper<&IDXGIObj::GetDesc2>::GetDescription(obj);
		else if constexpr(HasDesc1<IDXGIObj>)
			return GetDescriptionHelper<&IDXGIObj::GetDesc1>::GetDescription(obj);
		else
			return GetDescriptionHelper<&IDXGIObj::GetDesc>::GetDescription(obj);
	}
}