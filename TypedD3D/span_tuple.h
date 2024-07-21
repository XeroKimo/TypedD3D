////*********************************************************
////
//// Copyright (c) 2022 Renzy Alarcon
//// Licensed under the MIT License (MIT).
////
////*********************************************************
// 
#pragma once
#include <tuple>
#include <span>
#include <array>
#include <optional>
#include <type_traits>

namespace xk
{
	template<class Ty>
	struct opt_span;

	struct nullspan_t
	{
		template<class Ty>
		operator opt_span<Ty>() const { return { *this }; }
	};

	inline nullspan_t nullspan{};

	//template<class Ty>
	//struct opt_span_data
	//{
	//	Ty* _data = nullptr;

	//	Ty() = default;
	//	Ty* data() const { return _data; }
	//};

	//template<class Ty>
	//struct opt_span : opt_span_data<Ty>
	//{
	//};

	template<class Ty>
	constexpr bool is_opt_span = false;

	template<class Ty>
	constexpr bool is_opt_span<opt_span<Ty>> = true;

	template<class Ty>
	concept OptionalSpan = (is_opt_span<std::remove_cvref_t<Ty>> || std::convertible_to<Ty, nullspan_t>);
}

namespace xk
{
	template<class Ty>
	struct IsBoundedSpanCompatible : std::false_type {};

	template<class Ty>
	struct IsBoundedSpanCompatible<Ty&> : std::true_type {};

	template<class Ty, size_t N>
	struct IsBoundedSpanCompatible<Ty[N]> : std::true_type {};

	template<class Ty, size_t N>
	struct IsBoundedSpanCompatible<Ty(&)[N]> : std::true_type {};

	template<class Ty, size_t N>
	struct IsBoundedSpanCompatible<std::array<Ty, N>> : std::true_type {};

	template<class Ty, size_t N>
	struct IsBoundedSpanCompatible<std::array<Ty, N>&> : std::true_type {};

	template<class Ty, size_t N>
	struct IsBoundedSpanCompatible<std::span<Ty, N>> : std::true_type {};

	template<class Ty, size_t N>
	struct IsBoundedSpanCompatible<std::span<Ty, N>&> : std::true_type {};

	template<class Ty>
	struct Rank : std::integral_constant<size_t, 0> 
	{
		using lower_rank_type = Ty;
		using lower_rank_pointer = Ty*;
	};	
	
	template<class Ty>
	struct Rank<Ty&> : std::integral_constant<size_t, 1>
	{
		using lower_rank_type = Ty;
		using lower_rank_pointer = Ty*;
	};

	template<class Ty, size_t N>
	struct Rank<Ty[N]> : std::integral_constant<size_t, Rank<Ty>::value + 1> 
	{
		using lower_rank_type = Ty;
		using lower_rank_pointer = Ty*;
	};

	template<class Ty, size_t N>
	struct Rank<Ty(&)[N]> : std::integral_constant<size_t, Rank<Ty>::value + 1>
	{
		using lower_rank_type = Ty;
		using lower_rank_pointer = Ty*;
	};


	template<class Ty, size_t N>
	struct Rank<std::array<Ty, N>> : std::integral_constant<size_t, Rank<Ty>::value + 1> 
	{
		using lower_rank_type = Ty;
		using lower_rank_pointer = Ty*;
	};

	template<class Ty, size_t N>
	struct Rank<std::array<Ty, N>&> : std::integral_constant<size_t, Rank<Ty>::value + 1>
	{
		using lower_rank_type = Ty;
		using lower_rank_pointer = Ty*;
	};

	template<class Ty, size_t N>
	struct Rank<std::span<Ty, N>> : std::integral_constant<size_t, Rank<Ty>::value + 1> 
	{
		using lower_rank_type = Ty;
		using lower_rank_pointer = Ty*;
	};

	template<class Ty, size_t N>
	struct Rank<std::span<Ty, N>&> : std::integral_constant<size_t, Rank<Ty>::value + 1>
	{
		using lower_rank_type = Ty;
		using lower_rank_pointer = Ty*;
	};

	template<class Ty>
	using lower_rank_t = Rank<Ty>::lower_rank_type;

	template<class Ty>
	using lower_rank_pointer_t = Rank<Ty>::lower_rank_pointer;

	static_assert(Rank<std::array<int, 1>>::value == 1);

	template<class Ty>
	static constexpr size_t ArraySize = 1;

	template<class Ty, size_t N>
	static constexpr size_t ArraySize<Ty[N]> = N;

	template<class Ty, size_t N>
	static constexpr size_t ArraySize<Ty(&)[N]> = N;

	template<class Ty, size_t N>
	static constexpr size_t ArraySize<std::array<Ty, N>> = N;

	template<class Ty, size_t N>
	static constexpr size_t ArraySize<std::array<Ty, N>&> = N;

	template<class Ty, size_t N>
	static constexpr size_t ArraySize<std::span<Ty, N>> = N;

	template<class Ty, size_t N>
	static constexpr size_t ArraySize<std::span<Ty, N>&> = N;

	template<class Ty>
	concept BoundedSpanCompatible = IsBoundedSpanCompatible<Ty>::value;

	template<class Ty, class TyConvertibleTo>
	concept BoundedSpanCompatible2 = BoundedSpanCompatible<Ty> && std::is_convertible_v<lower_rank_pointer_t<Ty>, TyConvertibleTo*>;

	template<class Ty, class Ty2>
	concept BoundedSpanSizeCompatible = BoundedSpanCompatible<Ty> && BoundedSpanCompatible<Ty2> && (ArraySize<Ty> == ArraySize<Ty2>);

	template<class Ty, class Ty2, class TyConvertibleTo>
	concept BoundedSpanSizeCompatible2 = BoundedSpanSizeCompatible<Ty, Ty2> && std::is_convertible_v<lower_rank_pointer_t<Ty>, TyConvertibleTo*>;

	template<class Ty, class Ty2>
	concept OptionalSpan2 = OptionalSpan<Ty> && is_opt_span<Ty2>;

	template<class Ty>
		requires (!std::is_array_v<Ty>)
	Ty* Data(Ty* c)
	{
		return c;
	}

	template<class Ty>
	Ty* Data(opt_span<Ty> c)
	{
		return c.data();
	}

	template<class Ty>
	lower_rank_t<Ty>* Data(Ty& c)
	{
		if constexpr(requires(Ty t) { t.data(); })
			return c.data();
		else
			return &c;
	}

	template<class Ty>
	lower_rank_t<Ty>* Data(const Ty& c)
	{
		if constexpr(requires(Ty t) { t.data(); })
			return c.data();
		else
			return &c;
	}

	inline nullspan_t Data(nullspan_t)
	{
		return {};
	}

	//template<class Ty, size_t N>
	//Ty* Data(Ty (&c)[N])
	//{
	//	return c;
	//}


    template <class>
    inline constexpr bool Is_span_v = false;

    template <class _Ty, size_t _Extent>
    inline constexpr bool Is_span_v<std::span<_Ty, _Extent>> = true;

    template <class>
    inline constexpr bool Is_std_array_v = false;

    template <class _Ty, size_t _Size>
    inline constexpr bool Is_std_array_v<std::array<_Ty, _Size>> = true;

    template <class _It, class _Ty>
    concept Span_compatible_iterator = std::contiguous_iterator<_It>
        && std::is_convertible_v<std::remove_reference_t<std::iter_reference_t<_It>>(*)[], _Ty(*)[]>;

    template <class _Sentinel, class _It>
    concept Span_compatible_sentinel = std::sized_sentinel_for<_Sentinel, _It>
        && !std::is_convertible_v<_Sentinel, size_t>;

    template <class _Rng, class _Ty>
    concept Span_compatible_range =
        !std::is_array_v<std::remove_cvref_t<_Rng>>
		&& !Is_span_v<std::remove_cvref_t<_Rng>>
		&& !Is_std_array_v<std::remove_cvref_t<_Rng>>
        && ::std::ranges::contiguous_range<_Rng>
        && ::std::ranges::sized_range<_Rng>
        && (::std::ranges::borrowed_range<_Rng> || std::is_const_v<_Ty>)
        && std::is_convertible_v<std::remove_reference_t<::std::ranges::range_reference_t<_Rng>>(*)[], _Ty(*)[]>;


	template <class _Rng>
	concept Span_compatible_range_Solo =
		!std::is_array_v<std::remove_cvref_t<_Rng>>
		&& !Is_span_v<std::remove_cvref_t<_Rng>>
		&& !Is_std_array_v<std::remove_cvref_t<_Rng>>
		&& ::std::ranges::contiguous_range<_Rng>
		&& ::std::ranges::sized_range<_Rng>
		&& ::std::ranges::borrowed_range<_Rng>;
}

namespace xk
{
	template<class Ty>
	struct opt_span
	{
		Ty* _data{};
		opt_span() = default;
		opt_span(const opt_span&) = default;
		opt_span(nullspan_t) {}

		opt_span(Ty* data) : _data{ data } {}

		template<class Ty>
			requires (BoundedSpanCompatible<Ty> && !OptionalSpan<Ty>)
		opt_span(Ty&& data) :
			_data{ Data(data) }
		{
		}

		template<std::contiguous_iterator Ty>
		opt_span(Ty data) : _data{ std::to_address(data) } {}

		template<Span_compatible_range_Solo Ty>
		opt_span(Ty&& data) : _data{ std::ranges::data(data) } {}

		Ty* data() const { return _data; }
		Ty* operator->() const { return _data; }
	};

	template<>
	struct opt_span<nullspan_t>
	{
		opt_span() = default;
		opt_span(nullspan_t) {}

		template<class Ty>
		operator opt_span<Ty>() const { return nullspan; }
	};
}

namespace xk
{
	template<size_t Extent, class First, class... Ty>
	class span_tuple;

    template<class First, class... Ty>
	using dynamic_span_tuple = span_tuple<std::dynamic_extent, First, Ty...>;

	template<class Ty, class Ty2>
	constexpr auto OptSpanOrTy()
	{
		if constexpr(std::same_as<Ty, nullspan_t>)
			return opt_span<nullspan_t>{};
		else if constexpr(OptionalSpan<Ty>)
			return Ty{};
		else
			return Ty2{};
	}

	template<class Ty, class Ty2>
	constexpr auto OptSpanOrArrayTy()
	{
		if constexpr(std::same_as<std::remove_cvref_t<Ty>, nullspan_t>)
			return opt_span<nullspan_t>{};
		else if constexpr(OptionalSpan<Ty>)
			return Ty{};
		else
			return lower_rank_t<Ty2>{};
	}

	template<class Ty>
	using optspan_or_pointer = decltype(OptSpanOrTy<Ty, Ty*>());

	template<class Ty>
	using optspan_or_value = decltype(OptSpanOrTy<Ty, Ty>());

	template<class Ty>
	using optspan_or_array_type = decltype(OptSpanOrArrayTy<Ty, Ty>());

	static_assert(std::same_as<optspan_or_array_type<nullspan_t>, opt_span<nullspan_t>>);
	static_assert(std::same_as<optspan_or_array_type<decltype(nullspan)>, opt_span<nullspan_t>>);

	template<size_t Extent, class First, class... Ty>
	struct span_tuple_data
	{
		using pointer = std::tuple<optspan_or_pointer<First>, optspan_or_pointer<Ty>...>;
		pointer _data;
		static constexpr size_t _size = Extent;

        constexpr explicit span_tuple_data(pointer&& data, size_t) noexcept : _data{ std::move(data) } {}
	};

	template<class First, class... Ty>
	struct span_tuple_data<std::dynamic_extent, First, Ty...>
	{
		using pointer = std::tuple<optspan_or_pointer<First>, optspan_or_pointer<Ty>...>;
		pointer _data{};
		size_t _size{};
		
		constexpr span_tuple_data() = default;
		constexpr explicit span_tuple_data(pointer&& data, size_t size) noexcept : _data{ std::move(data) }, _size{ size } {}
	};

	//TODO: Reimplement the rest of the span_tuple API
	//TODO: Reimplement the rest of the iterator and stuff

	template<size_t Extent, class First, class... Ty>
	class span_tuple : private span_tuple_data<Extent, First, Ty...>
	{
	public:
		using base = span_tuple_data<Extent, First, Ty...>;
		using value_type = std::tuple<First, Ty...>;
		using pointer = base::pointer;

	private:
		using base::_data;
		using base::_size;

	public:
		constexpr span_tuple() noexcept requires (Extent == std::dynamic_extent) = default;

		constexpr span_tuple(const span_tuple& other) noexcept = default;

		template<size_t OtherExtent, class OtherFirst, class... OtherTy>
			requires (Extent == std::dynamic_extent || Extent == OtherExtent)
		constexpr span_tuple(const span_tuple<OtherExtent, OtherFirst, OtherTy...>& other) noexcept :
			base{ other.data(), other.size() }
		{

		}

		//Mix and match constructor of spans or arrays with known bounds.
		//template<BoundedSpanCompatible2<First> OtherFirst, BoundedSpanSizeCompatible2<OtherFirst, Ty> ... OtherTy>
		//	requires (Extent != std::dynamic_extent)
		//constexpr span_tuple(OtherFirst&& first, OtherTy&&... elm) :
		//	base{ std::forward_as_tuple(Data(first), Data(elm)...), Extent }
		//{

		//}

		template<class OtherFirst, class ... OtherTy>
			requires (BoundedSpanCompatible2<OtherFirst, First> || OptionalSpan2<OtherFirst, First>)
			&& ((BoundedSpanSizeCompatible2<OtherTy, OtherFirst, Ty> || OptionalSpan2<OtherTy, Ty>) && ...)
		constexpr span_tuple(OtherFirst&& first, OtherTy&&... elm) :
				base{ std::forward_as_tuple(Data(first), Data(elm)...), Extent }
		{

		}


		template<BoundedSpanCompatible OtherFirst>
			requires std::is_convertible_v<lower_rank_t<OtherFirst>, First>
		constexpr span_tuple(OtherFirst&& first, Ty*... elm) :
			base{ std::forward_as_tuple(Data(first), elm...), ArraySize<OtherFirst> }
		{

		}

		template<Span_compatible_iterator<First> OtherFirst, Span_compatible_iterator<Ty>... OtherTy>
		explicit(Extent != std::dynamic_extent)
		constexpr span_tuple(OtherFirst first, size_t count, OtherTy... elm) :
			base{ std::forward_as_tuple(std::to_address(first), std::to_address(elm)...), count }
		{
		}

		template<Span_compatible_iterator<First> OtherFirst, Span_compatible_sentinel<OtherFirst> Sentinel, Span_compatible_iterator<Ty>... OtherTy>
		explicit(Extent != std::dynamic_extent)
		constexpr span_tuple(OtherFirst first, Sentinel last, OtherTy... elm) :
			base{ std::forward_as_tuple(std::to_address(first), std::to_address(elm)...),  static_cast<size_t>(last - first) }
		{
		}

		template<Span_compatible_range<First> OtherFirst, Span_compatible_range<Ty>... OtherTy>
		constexpr span_tuple(OtherFirst&& first, OtherTy&&... elm) :
			base(std::forward_as_tuple(::std::ranges::data(first), ::std::ranges::data(elm)...), static_cast<size_t>(::std::ranges::size(first)))
		{

		}

	public:
		constexpr size_t size() const noexcept { return _size; }

		constexpr pointer data() const { return _data; }

		template<size_t I>
		constexpr auto data() { return Data(std::get<I>(_data)); }

		template<size_t I>
		constexpr const auto data() const { return Data(std::get<I>(_data)); }

		template<size_t I>
		constexpr size_t size_bytes() const noexcept { return sizeof(std::tuple_element_t<I, value_type>) * _size; }
	};

	//Creates a std::span from a span_tuple at the given Index
    template<size_t Index, size_t Extent, class First, class... Ty>
    constexpr auto get(span_tuple<Extent, First, Ty...> span)
    {
        return std::span<std::tuple_element_t<Index, typename span_tuple<Extent, First, Ty...>::value_type>, Extent>(get<Index>(span.data()), span.size());
    }

    //Creates a std::span from a span_tuple at the given Index
    template<class Index, size_t Extent, class First, class... Ty>
    constexpr auto get(span_tuple<Extent, First, Ty...> span)
    {
        return std::span<Index, Extent>(std::get<Index*>(span.data()), span.size());
    }

	//template<BoundedSpanCompatible Ty, BoundedSpanSizeCompatible<Ty>... OtherTy>
	//span_tuple(Ty&&, OtherTy&&...) -> span_tuple<ArraySize<Ty>, lower_rank_t<Ty>, lower_rank_t<OtherTy>...>;

	template<class Ty, class... OtherTy>
		requires (BoundedSpanCompatible<Ty>)
		&& ((BoundedSpanSizeCompatible<Ty, OtherTy> || OptionalSpan<OtherTy>) && ...)
	span_tuple(Ty&&, OtherTy&&...) -> span_tuple<ArraySize<Ty>, optspan_or_array_type<Ty>, optspan_or_array_type<OtherTy>...>;

	template<BoundedSpanCompatible Ty, class... OtherTy>
	span_tuple(Ty&&, OtherTy*...) -> span_tuple<ArraySize<Ty>, lower_rank_t<Ty>, OtherTy...>;

	template<std::contiguous_iterator Ty, std::contiguous_iterator... OtherTy>
	span_tuple(Ty, size_t, OtherTy...) -> span_tuple<std::dynamic_extent, std::iter_value_t<Ty>, std::iter_value_t<OtherTy>...>;

	template<std::contiguous_iterator Ty, std::sized_sentinel_for<Ty> Sentinel, std::contiguous_iterator... OtherTy>
	span_tuple(Ty, Sentinel, OtherTy...) -> span_tuple<std::dynamic_extent, std::iter_value_t<Ty>, std::iter_value_t<OtherTy>...>;

	template<Span_compatible_range_Solo Ty, Span_compatible_range_Solo... OtherTy>
	span_tuple(Ty&&, OtherTy&&...) -> span_tuple<std::dynamic_extent, std::ranges::range_value_t<Ty>, std::ranges::range_value_t<OtherTy>...>;
}


//
//#pragma once
//#include <tuple>
//#include <span>
//#include <array>
//#include <cassert>
//
//#if __has_cpp_attribute(nodiscard)
//#define XK_SPAN_TUPLE_NODISCARD [[nodiscard]]
//#else
//#define XK_SPAN_TUPLE_NODISCARD
//#endif
//
//namespace xk
//{
//    template<size_t Extent, class First, class... Ty>
//    class span_tuple;
//
//    template<class First, class... Ty>
//    using dynamic_span_tuple = span_tuple<std::dynamic_extent, First, Ty...>;
//
//
//    template <class First, class... Others>
//    struct span_tuple_iterator {
//#ifdef __cpp_lib_concepts
//        using iterator_concept = std::contiguous_iterator_tag;
//#endif // __cpp_lib_concepts
//        using iterator_category = std::random_access_iterator_tag;
//        using value_type = std::tuple<std::remove_cv_t<First>, std::remove_cv_t<Others>...>;
//        using difference_type = ptrdiff_t;
//        using pointer = std::tuple<First*, Others*...>;
//        using reference = std::tuple<First&, Others&...>;
//
//        XK_SPAN_TUPLE_NODISCARD constexpr reference operator*() const noexcept {
//            return std::apply([](auto&&... elements)
//            {
//                return reference(*elements...);
//            }, _Myptr);
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr pointer operator->() const noexcept {
//            return _Myptr;
//        }
//
//        constexpr span_tuple_iterator& operator++() noexcept {
//            std::apply([](auto*&... ptrs)
//            {
//                (++ptrs, ...);
//            }, _Myptr);
//            return *this;
//        }
//
//        constexpr span_tuple_iterator operator++(int) noexcept {
//            span_tuple_iterator _Tmp{ *this };
//            ++* this;
//            return _Tmp;
//        }
//
//        constexpr span_tuple_iterator& operator--() noexcept {
//            std::apply([](auto*&... ptrs)
//            {
//                (--ptrs, ...);
//            }, _Myptr);
//            return *this;
//        }
//
//        constexpr span_tuple_iterator operator--(int) noexcept {
//            span_tuple_iterator _Tmp{ *this };
//            --* this;
//            return _Tmp;
//        }
//
//        constexpr span_tuple_iterator& operator+=(const difference_type _Off) noexcept {
//            std::apply([=](auto*&... ptrs)
//            {
//                ((ptrs += _Off), ...);
//            }, _Myptr);
//            return *this;
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr span_tuple_iterator operator+(const difference_type _Off) const noexcept {
//            span_tuple_iterator _Tmp{ *this };
//            _Tmp += _Off;
//            return _Tmp;
//        }
//
//        XK_SPAN_TUPLE_NODISCARD friend constexpr span_tuple_iterator operator+(const difference_type _Off, span_tuple_iterator _Next) noexcept {
//            _Next += _Off;
//            return _Next;
//        }
//
//        constexpr span_tuple_iterator& operator-=(const difference_type _Off) noexcept {
//            std::apply([=](auto*&... ptrs)
//            {
//                ((ptrs -= _Off), ...);
//            }, _Myptr);
//            return *this;
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr span_tuple_iterator operator-(const difference_type _Off) const noexcept {
//            span_tuple_iterator _Tmp{ *this };
//            _Tmp -= _Off;
//            return _Tmp;
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr difference_type operator-(const span_tuple_iterator& _Right) const noexcept {
//            return _Myptr - _Right._Myptr;
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr reference operator[](const difference_type _Off) const noexcept {
//            return *(*this + _Off);
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr bool operator==(const span_tuple_iterator& _Right) const noexcept {
//
//            return _Myptr == _Right._Myptr;
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr std::strong_ordering operator<=>(const span_tuple_iterator& _Right) const noexcept {
//            return _Myptr <=> _Right._Myptr;
//        }
//
//        pointer _Myptr = nullptr;
//    };
//
//    template<class Ty>
//    struct optional
//    {
//        using element_type = Ty;
//        using value_type = std::remove_cv_t<Ty>;
//        using pointer = Ty*;
//        using const_pointer = const Ty*;
//        using reference = Ty&;
//        using const_reference = const Ty&;
//    };
//
//    template<class>
//    constexpr bool is_optional_parameter = false;
//
//    template<class Ty>
//    constexpr bool is_optional_parameter<optional<Ty>> = true;
//
//    template<class... Ty>
//    constexpr bool is_any_optional = (is_optional_parameter<Ty> || ...);
//
//
//    template<size_t Extent, class First, class... Ty>
//    struct Extent_type
//    {
//        using pointer = std::tuple<First*, Ty*...>;
//
//        constexpr Extent_type() noexcept = default;
//
//        constexpr explicit Extent_type(pointer&& data, size_t) noexcept : m_data{ std::move(data) } {}
//
//        pointer m_data = {};
//        static constexpr size_t m_size = Extent;
//    };
//
//    template<class First, class... Ty>
//    struct Extent_type<std::dynamic_extent, First, Ty...>
//    {
//        using pointer = std::tuple<First*, Ty*...>;
//
//        constexpr Extent_type() noexcept = default;
//
//        constexpr explicit Extent_type(pointer&& data, const size_t size) noexcept :
//            m_data{ data },
//            m_size{ size }
//        {
//        }
//
//        pointer m_data = {};
//        size_t m_size{ 0 };
//    };
//
//    template<size_t Extent, class First, class... Ty>
//    class span_tuple : private Extent_type<Extent, First, Ty...>
//    {
//    private:
//        using base = Extent_type<Extent, First, Ty...>;
//        using base::m_data;
//        using base::m_size;
//
//    public:
//        using element_type = std::tuple<First, Ty...>;
//        using value_type = std::tuple<std::remove_cv_t<First>, std::remove_cv_t<Ty>...>;
//        using pointer = std::tuple<First*, Ty*...>;
//        using const_pointer = std::tuple<const First*, const Ty*...>;
//        using reference = std::tuple<First&, Ty&...>;
//        using const_reference = std::tuple<const First&, const Ty&...>;
//        using size_type = size_t;
//        using difference_type = ptrdiff_t;
//        using iterator = span_tuple_iterator<First, Ty...>;
//        using reverse_iterator = ::std::reverse_iterator<iterator>;
//
//        static constexpr size_type extent = Extent;
//
//    public:
//        constexpr span_tuple() noexcept requires (Extent == 0 || Extent == std::dynamic_extent) = default;
//        constexpr span_tuple(First& first, Ty&... others) noexcept requires (Extent == 1)
//            : base(std::forward_as_tuple(&first, &others...), Extent)
//        {
//
//        }
//        /// <summary>
//        /// begin iterator + size based constructor
//        /// </summary>
//        /// <param name="FirstIt"> The iterator for the first element </param>
//        /// <param name="Count"> The size of the span </param>
//        /// <param name="otherIt..."> The iterators for the other elements. All assumed to have the same length as Count </param>
//        template <Span_compatible_iterator<First> It, Span_compatible_iterator<Ty>... OtherIt>
//        constexpr explicit(Extent != std::dynamic_extent) span_tuple(It FirstIt, size_type Count, OtherIt... otherIt) noexcept // strengthened
//            : base(std::forward_as_tuple(std::to_address(FirstIt), std::to_address(otherIt)...), Count)
//        {
//            if constexpr(Extent != std::dynamic_extent)
//            {
//                assert(Count == Extent &&
//                    "Cannot construct span with static extent from range [first, first + count) as count != extent");
//            }
//        }
//
//
//        /// <summary>
//        /// begin + end iterator based constructor
//        /// </summary>
//        /// <param name="_First"> The iterator for the first element </param>
//        /// <param name="_Last"> The end point for the first iterator </param>
//        /// <param name="otherIt..."> The iterators for the other elements. Assumed to have the same range as (_Last - _First) </param>
//        template <Span_compatible_iterator<First> _It, Span_compatible_sentinel<_It> _Sentinel, Span_compatible_iterator<Ty>... OtherIt>
//        constexpr explicit(Extent != std::dynamic_extent) span_tuple(_It _First, _Sentinel _Last, OtherIt... otherIt)
//            noexcept(noexcept(_Last - _First)) // strengthened
//            : base(std::forward_as_tuple(std::to_address(_First), std::to_address(otherIt)...), static_cast<size_type>(_Last - _First))
//        {
//            if constexpr(Extent != std::dynamic_extent)
//            {
//                assert(_Last - _First == Extent &&
//                    "Cannot construct span with static extent from range [first, last) as last - first != extent");
//            }
//        }
//
//
//        /// <summary>
//        /// c-array based constructor
//        /// </summary>
//        /// <param name="first"> The c-array for the first element </param>
//        /// <param name="elm"> The c-arrays for the other elements </param>
//        template<size_t Size>
//            requires(Extent == std::dynamic_extent || Extent == Size)
//        span_tuple(First(&first)[Size], Ty(&...elm)[Size]) :
//            base(std::forward_as_tuple(first, elm...), Size)
//        {
//
//        }
//
//        /// <summary>
//        /// std::array based constructor
//        /// </summary>
//        /// <param name="first"> The std::array for the first element </param>
//        /// <param name="elm"> The std::arrays for the other elements </param>
//        template <class OtherFirst, class... OtherTy, size_t Size>
//            requires (Extent == std::dynamic_extent || Extent == Size) &&
//        std::is_convertible_v<OtherFirst(*)[], First(*)[]> &&
//            (std::is_convertible_v<OtherTy(*)[], Ty(*)[]> && ...)
//            span_tuple(std::array<OtherFirst, Size>& first, std::array<OtherTy, Size>&... elm) :
//            base(std::forward_as_tuple(first.data(), elm.data()...), std::size(first))
//        {
//
//        }
//
//
//        /// <summary>
//        /// const std::array based constructor
//        /// </summary>
//        /// <param name="first"> The std::array for the first element </param>
//        /// <param name="elm"> The std::arrays for the other elements </param>
//        template <class OtherFirst, class... OtherTy, size_t Size>
//            requires (Extent == std::dynamic_extent || Extent == Size) &&
//        std::is_convertible_v<const OtherFirst(*)[], First(*)[]> &&
//            (std::is_convertible_v<const OtherTy(*)[], Ty(*)[]>, ...)
//            span_tuple(const std::array<OtherFirst, Size>& first, const std::array<OtherTy, Size>&... elm) :
//            base(std::forward_as_tuple(first.data(), elm.data()...), std::size(first))
//        {
//
//        }
//
//
//        /// <summary>
//        /// c-array based constructor with dynamic others
//        /// </summary>
//        /// <param name="first"> The c-array for the first element </param>
//        /// <param name="elm"> The c-arrays for the other elements </param>
//        template<size_t Size>
//            requires(Extent == std::dynamic_extent)
//        span_tuple(First(&first)[Size], Ty*... elm) :
//            base(std::forward_as_tuple(first, elm...), Size)
//        {
//
//        }
//
//        /// <summary>
//        /// std::array based constructor with dynamic others
//        /// </summary>
//        /// <param name="first"> The std::array for the first element </param>
//        /// <param name="elm"> The std::arrays for the other elements </param>
//        template <class OtherFirst, class... OtherTy, size_t Size>
//            requires (Extent == std::dynamic_extent) &&
//        std::is_convertible_v<OtherFirst(*)[], First(*)[]> &&
//            (std::is_convertible_v<OtherTy(*)[], Ty(*)[]> && ...)
//            span_tuple(std::array<OtherFirst, Size>& first, OtherTy*... elm) :
//            base(std::forward_as_tuple(first.data(), elm...), std::size(first))
//        {
//
//        }
//
//
//        /// <summary>
//        /// const std::array based constructor with dynamic others
//        /// </summary>
//        /// <param name="first"> The std::array for the first element </param>
//        /// <param name="elm"> The std::arrays for the other elements </param>
//        template <class OtherFirst, class... OtherTy, size_t Size>
//            requires (Extent == std::dynamic_extent) &&
//        std::is_convertible_v<const OtherFirst(*)[], First(*)[]> &&
//            (std::is_convertible_v<const OtherTy(*)[], Ty(*)[]>, ...)
//            span_tuple(const std::array<OtherFirst, Size>& first, const OtherTy*... elm) :
//            base(std::forward_as_tuple(first.data(), elm...), std::size(first))
//        {
//
//        }
//
//        /// <summary>
//        /// A copy / converting constructor that can take in other sized span_tuples. Cannot take in span_tuples with optional parameters
//        /// </summary>
//        /// <param name="other"> </param>
//        template <class OtherFirst, class... OtherTy, size_t OtherExtent>
//            requires ((Extent == std::dynamic_extent && OtherExtent != std::dynamic_extent) || Extent == OtherExtent) &&
//        std::is_convertible_v<OtherFirst(*)[], First(*)[]> &&
//            (std::is_convertible_v<OtherTy(*)[], Ty(*)[]> && ...) &&
//            (!is_any_optional<OtherTy...>)
//            constexpr explicit(Extent != std::dynamic_extent && OtherExtent == std::dynamic_extent) span_tuple(const span_tuple<OtherExtent, OtherFirst, OtherTy...>& other) noexcept
//            : base(other.data(), other.size())
//        {
//            if constexpr(Extent != std::dynamic_extent)
//            {
//                assert(other.size() == Extent &&
//                    "Cannot construct span with static extent from other span as other.size() != extent");
//            }
//
//        }
//
//        /// <summary>
//        /// Ranged based constructor
//        /// </summary>
//        /// <param name="first"> The range for the first element </param>
//        /// <param name="elm"> The ranges for the other elements </param>
//        template <Span_compatible_range<First> _Rng, Span_compatible_range<Ty>... OtherRng>
//        constexpr explicit(Extent != std::dynamic_extent) span_tuple(_Rng&& _Range, OtherRng&&... OtherRange)
//            : base(std::forward_as_tuple(::std::ranges::data(_Range), ::std::ranges::data(OtherRange)...), static_cast<size_type>(::std::ranges::size(_Range)))
//        {
//            if constexpr(Extent != std::dynamic_extent)
//            {
//                assert(::std::ranges::size(_Range) == Extent &&
//                    "Cannot construct span with static extent from range r as std::ranges::size(r) != extent");
//            }
//        }
//
//    public:
//
//            // [span.sub] Subviews
//        template <size_t _Count>
//        XK_SPAN_TUPLE_NODISCARD constexpr auto first() const noexcept /* strengthened */
//        {
//            if constexpr(Extent != std::dynamic_extent)
//            {
//                static_assert(_Count <= Extent, "Count out of range in span::first()");
//            }
//
//            assert(_Count <= m_size && "Count out of range in span::first()");
//
//            return first_impl<_Count>(std::make_index_sequence<sizeof...(Ty)>());
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr auto first(const size_type _Count) const noexcept
//        {
//            assert(_Count <= m_size && "Count out of range in span::first(count)");
//            return first_impl(_Count, std::make_index_sequence<sizeof...(Ty)>());
//        }
//
//        template <size_t _Count>
//        XK_SPAN_TUPLE_NODISCARD constexpr auto last() const noexcept /* strengthened */
//        {
//            if constexpr(Extent != std::dynamic_extent)
//            {
//                static_assert(_Count <= Extent, "Count out of range in span::last()");
//            }
//
//            assert(_Count <= m_size && "Count out of range in span::last()");
//            return last_impl<_Count>(std::make_index_sequence<sizeof...(Ty)>());
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr auto last(const size_type _Count) const noexcept /* strengthened */ {
//
//            assert(_Count <= m_size && "Count out of range in span::last(count)");
//            return last_impl(_Count, std::make_index_sequence<sizeof...(Ty)>());
//        }
//
//        template <size_t _Offset, size_t _Count = std::dynamic_extent>
//        XK_SPAN_TUPLE_NODISCARD constexpr auto subspan() const noexcept /* strengthened */
//        {
//            if constexpr(Extent != std::dynamic_extent)
//            {
//                static_assert(_Offset <= Extent, "Offset out of range in span::subspan()");
//                static_assert(_Count == std::dynamic_extent || _Count <= Extent - _Offset, "Count out of range in span::subspan()");
//            }
//
//            assert(_Offset <= m_size && "Offset out of range in span::subspan(offset, count)");
//            assert(_Count <= m_size - _Offset && "Count out of range in span::subspan()");
//            using _ReturnType = span_tuple<First,
//                _Count != std::dynamic_extent ? _Count : (Extent != std::dynamic_extent ? Extent - _Offset : std::dynamic_extent), Ty...>;
//
//            return subspan_impl<_Offset, _Count>(std::make_index_sequence<sizeof...(Ty)>());
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr auto subspan(const size_type _Offset, const size_type _Count = std::dynamic_extent) const noexcept
//        {
//            assert(_Offset <= m_size && "Offset out of range in span::subspan(offset, count)");
//            assert((_Count == std::dynamic_extent || _Count <= m_size - _Offset) && "Count out of range in span::subspan(offset, count)");
//
//            using _ReturnType = span_tuple<First, std::dynamic_extent, Ty...>;
//            return subspan_impl(_Offset, _Count, std::make_index_sequence<sizeof...(Ty)>());
//        }
//
//    public:
//        //Gets all the elements of a given offset
//        constexpr reference operator[](size_t offset) const noexcept
//        {
//            assert(offset < m_size && "span index out of range");
//            return std::apply([offset](auto&&... elements)
//            {
//                return reference(elements[offset]...);
//            }, m_data);
//        }
//
//        //Gets the first element of all spans
//        constexpr reference front() const noexcept
//        {
//            assert(m_size > 0 && "front of empty span");
//            return std::apply([](auto&&... elements)
//            {
//                return reference(*elements...);
//            }, m_data);
//        }
//
//        //Gets the first element of a given span
//        template<size_t Index>
//        constexpr std::tuple_element_t<Index, reference> front() const noexcept
//        {
//            assert(m_size > 0 && "front of empty span");
//            return *get<Index>(m_data);
//        }
//
//        //Gets the first element of a given span
//        template<class Index>
//        constexpr Index& front() const noexcept
//        {
//            assert(m_size > 0 && "front of empty span");
//            return *get<Index*>(m_data);
//        }
//
//        //Gets the last element of all spans
//        constexpr reference back() const noexcept
//        {
//            assert(m_size > 0 && "back of empty span");
//            return std::apply([offset = m_size - 1](auto&&... elements)
//            {
//                return reference(elements[offset]...);
//            }, m_data);
//        }
//
//        //Gets the last element of a given span
//        template<size_t Index>
//        constexpr std::tuple_element_t<Index, reference> back() const noexcept
//        {
//            assert(m_size > 0 && "back of empty span");
//            return get<Index>(m_data)[m_size - 1];
//        }
//
//        //Gets the last element of a given span
//        template<class Index>
//        constexpr Index& back() const noexcept
//        {
//            assert(m_size > 0 && "back of empty span");
//            return get<Index*>(m_data)[m_size - 1];
//        }
//
//        //Gets pointer to the beginning of all span at a given tuple index
//        constexpr pointer data() const noexcept { return m_data; }
//
//        //Gets pointer to the beginning of all span at a given tuple index
//        template<size_t Index>
//        constexpr std::tuple_element_t<Index, pointer> data() const noexcept
//        {
//            return get<Index>(m_data);
//        }
//
//        //Gets pointer to the beginning of all span at a given tuple index
//        template<class Index>
//        constexpr Index* data() const noexcept
//        {
//            return get<Index*>(m_data);
//        }
//
//        //Gets the size of the span
//        constexpr size_t size() const noexcept { return m_size; }
//
//        //Gets the size of the span
//        template<size_t Index>
//        constexpr size_t size() const noexcept
//        {
//            return m_size;
//        }
//
//        //Gets the size of the span
//        template<class Index>
//        constexpr size_t size() const noexcept
//        {
//            return m_size;
//        }
//
//        //Gets the total size in bytes that a given span is occupying
//        template<size_t Index>
//        constexpr size_t size_bytes() const noexcept { return sizeof(std::tuple_element_t<Index, value_type>) * m_size; }
//
//        //Gets the total size in bytes that a given span is occupying
//        template<class Index>
//            requires std::same_as<Index, First> || (std::same_as<Index, Ty> || ...)
//        constexpr size_t size_bytes() const noexcept { return sizeof(Index) * m_size; }
//
//        constexpr bool empty() const noexcept { return m_size == 0; }
//
//        template<size_t Index>
//        constexpr bool empty() const noexcept { return m_size == 0; }
//
//        template<class Index>
//        constexpr bool empty() const noexcept { return m_size == 0; }
//
//    //Creates a std::span from a span_tuple at the given Index
//        template<size_t Index, size_t Extent, class First, class... Ty>
//        friend constexpr auto get(span_tuple<Extent, First, Ty...> span);
//
//    //Creates a std::span from a span_tuple at the given Index
//        template<class Index, size_t Extent, class First, class... Ty>
//        friend constexpr auto get(span_tuple<Extent, First, Ty...> span);
//
//
//    // [span.iterators] Iterator support
//        XK_SPAN_TUPLE_NODISCARD constexpr iterator begin() const noexcept
//        {
//            const auto _End = std::apply([size = m_size](auto*... ptrs)
//            {
//                ((ptrs += size), ...);
//                return std::tuple(ptrs...);
//            }, m_data);
//            return { m_data };
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr iterator end() const noexcept
//        {
//            const auto _End = std::apply([size = m_size](auto*... ptrs)
//            {
//                ((ptrs += size), ...);
//                return std::tuple(ptrs...);
//            }, m_data);
//            return { _End };
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr reverse_iterator rbegin() const noexcept
//        {
//            return reverse_iterator{ end() };
//        }
//
//        XK_SPAN_TUPLE_NODISCARD constexpr reverse_iterator rend() const noexcept
//        {
//            return reverse_iterator{ begin() };
//        }
//
//    private:
//        template<size_t Count, class T, T... Indexs>
//        XK_SPAN_TUPLE_NODISCARD constexpr auto first_impl(std::index_sequence<Indexs...>) const noexcept
//        {
//            return span_tuple<First, Count, Ty...>{std::get<0>(m_data), Count, std::get<Indexs + 1>(m_data)...};
//        }
//
//        template<class T, T... Indexs>
//        XK_SPAN_TUPLE_NODISCARD constexpr auto first_impl(size_t count, std::index_sequence<Indexs...>) const noexcept
//        {
//            return span_tuple<First, std::dynamic_extent, Ty...>{std::get<0>(m_data), count, std::get<Indexs + 1>(m_data)...};
//        }
//        template<size_t Count, class T, T... Indexs>
//        XK_SPAN_TUPLE_NODISCARD constexpr auto last_impl(std::index_sequence<Indexs...>) const noexcept
//        {
//            return span_tuple<First, Count, Ty...>{std::get<0>(m_data) + (m_size - Count), Count, (std::get<Indexs + 1>(m_data) + (m_size - Count))...};
//        }
//
//        template<class T, T... Indexs>
//        XK_SPAN_TUPLE_NODISCARD constexpr auto last_impl(size_t count, std::index_sequence<Indexs...>) const noexcept
//        {
//            return span_tuple<First, std::dynamic_extent, Ty...>{std::get<0>(m_data) + (m_size - count), count, (std::get<Indexs + 1>(m_data) + (m_size - count))...};
//        }
//
//        template<size_t Offset, size_t Count, class T, T... Indexs>
//        XK_SPAN_TUPLE_NODISCARD constexpr auto subspan_impl(std::index_sequence<Indexs...>) const noexcept
//        {
//            using _ReturnType = span_tuple<First,
//                Count != std::dynamic_extent ? Count : (Extent != std::dynamic_extent ? Extent - Offset : std::dynamic_extent), Ty...>;
//            return _ReturnType{ std::get<0>(m_data) + Offset, Count == std::dynamic_extent ? m_size - Offset : Count, (std::get<Indexs + 1>(m_data) + Offset)... };
//        }
//
//        template<class T, T... Indexs>
//        XK_SPAN_TUPLE_NODISCARD constexpr auto subspan_impl(size_t offset, size_t count, std::index_sequence<Indexs...>) const noexcept
//        {
//            return span_tuple<First, std::dynamic_extent, Ty...>{ std::get<0>(m_data) + offset, count == std::dynamic_extent ? m_size - offset : count, (std::get<Indexs + 1>(m_data) + offset)... };
//        }
//    };
//
//    template<class Ty>
//    struct span_element_type
//    {
//        using type = Ty;
//    };
//
//    template<class Ty>
//    struct span_element_type<optional<Ty>>
//    {
//        using type = Ty;
//    };
//
//    template<class Ty>
//    using span_element_t = typename span_element_type<Ty>::type;
//
//    struct null_optional_type
//    {
//    };
//
//    constexpr null_optional_type nullopt;
//
//    template<class>
//    constexpr bool is_null_opt = false;
//
//    template<>
//    constexpr bool is_null_opt<null_optional_type> = true;
//
//    template<class It, class Ty>
//    struct optional_iterator
//    {
//        using pointer = typename Ty::pointer;
//        using element_type = typename Ty::element_type;
//        using difference_type = std::ptrdiff_t;
//
//        optional_iterator(It it) : val(it) {}
//
//        It val = {};
//
//        operator It () { return val; }
//        constexpr pointer operator->() noexcept
//        {
//            return std::to_address(val);
//        }
//        constexpr pointer operator->() const noexcept
//        {
//            return std::to_address(val);
//        }
//    };
//
//    template<class Ty>
//    struct optional_iterator<null_optional_type, Ty>
//    {
//        using pointer = typename Ty::pointer;
//        using element_type = typename Ty::element_type;
//        using difference_type = std::ptrdiff_t;
//
//        optional_iterator(null_optional_type) {};
//        constexpr pointer operator->() noexcept
//        {
//            return nullptr;
//        }
//        constexpr pointer operator->() const noexcept
//        {
//            return nullptr;
//        }
//    };
//
//    template<class It, class Ty, size_t Size>
//    struct optional_array
//    {
//        using pointer = typename Ty::pointer;
//
//        optional_array(Ty::element_type(&arr)[Size]) : val(arr) {}
//        optional_array(std::array<typename Ty::element_type, Size>& arr) : val(arr.data()) {}
//        optional_array(const std::array<typename Ty::element_type, Size>& arr) : val(arr.data()) {}
//
//        pointer val;
//
//        pointer data() const { return val; }
//    };
//
//    template<class Ty, size_t Size>
//    struct optional_array<null_optional_type, Ty, Size>
//    {
//        using pointer = typename Ty::pointer;
//
//        optional_array(null_optional_type) {};
//        pointer data() const { return nullptr; }
//    };
//
//    template <class _It, class _Ty>
//    concept Span_compatible_optional_iterator = std::contiguous_iterator<_It>
//        && std::is_convertible_v<std::remove_reference_t<std::iter_reference_t<_It>>(*)[], span_element_t<_Ty>(*)[]>
//        || (is_optional_parameter<_Ty> && is_null_opt<_It>);
//
//    template <class OtherTy, size_t Size, class Ty>
//    concept Span_compatible_optional_array = std::is_bounded_array_v<OtherTy[Size]>
//        || (is_optional_parameter<Ty> && is_null_opt<Ty>);
//
//    template <class OtherTy, class Ty, size_t Size>
//    concept Span_compatible_optional_std_array = Is_std_array_v<OtherTy> &&
//        std::is_convertible_v<typename OtherTy::value_type(*)[], span_element_t<Ty>(*)[]> &&
//        std_array_size<OtherTy> == Size ||
//        (is_optional_parameter<Ty> && is_null_opt<Ty>);
//
//    template <class OtherTy, class Ty, size_t Size>
//    concept Span_compatible_optional_const_std_array = Is_std_array_v<OtherTy> &&
//        std::is_convertible_v<const typename OtherTy::value_type(*)[], span_element_t<Ty>(*)[]> &&
//        std_array_size<OtherTy> == Size ||
//        (is_optional_parameter<Ty> && is_null_opt<Ty>);
//
//    template<class First, class... Ty>
//        requires is_any_optional<Ty...>
//    class span_tuple<std::dynamic_extent, First, Ty...> : private Extent_type<std::dynamic_extent, First, span_element_t<Ty>...>
//    {
//    private:
//        using base = Extent_type<std::dynamic_extent, First, span_element_t<Ty>...>;
//        using base::m_data;
//        using base::m_size;
//
//    public:
//        using element_type = std::tuple<First, span_element_t<Ty>...>;
//        using value_type = std::tuple<std::remove_cv_t<First>, std::remove_cv_t<span_element_t<Ty>>...>;
//        using pointer = std::tuple<First*, span_element_t<Ty>*...>;
//        using const_pointer = std::tuple<const First*, const span_element_t<Ty>*...>;
//        using reference = std::tuple<First&, span_element_t<Ty>&...>;
//        using const_reference = std::tuple<const First&, const span_element_t<Ty>&...>;
//        using size_type = size_t;
//        using difference_type = ptrdiff_t;
//        using iterator = span_tuple_iterator<First, span_element_t<Ty>...>;
//        using reverse_iterator = ::std::reverse_iterator<iterator>;
//
//        static constexpr size_type extent = std::dynamic_extent;
//
//    public:
//        constexpr span_tuple() noexcept = default;
//
//        /// <summary>
//        /// begin iterator + size based constructor
//        /// </summary>
//        /// <param name="FirstIt"> The iterator for the first element </param>
//        /// <param name="Count"> The size of the span </param>
//        /// <param name="otherIt..."> The iterators for the other elements. All assumed to have the same length as Count. Optional params can be xk::nullopt to signify 0 length </param>
//        template <Span_compatible_iterator<First> It, Span_compatible_optional_iterator<Ty>... OtherIt>
//        constexpr span_tuple(It FirstIt, size_type Count, OtherIt... otherIt) noexcept // strengthened
//            : base(std::forward_as_tuple(std::to_address(FirstIt), std::to_address(optional_iterator<OtherIt, Ty>(otherIt))...), Count)
//        {
//        }
//
//
//        /// <summary>
//        /// begin + end iterator based constructor
//        /// </summary>
//        /// <param name="_First"> The iterator for the first element </param>
//        /// <param name="_Last"> The end point for the first iterator </param>
//        /// <param name="otherIt..."> The iterators for the other elements. Assumed to have the same range as (_Last - _First). Optional params can be xk::nullopt to signify 0 length </param>
//        template <Span_compatible_iterator<First> _It, Span_compatible_sentinel<_It> _Sentinel, Span_compatible_optional_iterator<Ty>... OtherIt>
//        constexpr span_tuple(_It _First, _Sentinel _Last, OtherIt... otherIt) noexcept(noexcept(_Last - _First)) // strengthened
//            : base(std::forward_as_tuple(std::to_address(_First), std::to_address(optional_iterator<OtherIt, Ty>(otherIt))...), static_cast<size_type>(_Last - _First))
//        {
//
//        }
//
//        /// <summary>
//        /// c-array based constructor
//        /// </summary>
//        /// <param name="first"> The c-array for the first element </param>
//        /// <param name="elm"> The c-arrays for the other elements. Optional params can be xk::nullopt to signify 0 length </param>
//        template<size_t Size, Span_compatible_optional_array<Size, Ty>... OtherTy>
//        span_tuple(First(&first)[Size], OtherTy&... Ty) :
//            base(std::forward_as_tuple(first, optional_array<OtherTy, Ty, Size>(Ty).data()...), Size)
//        {
//
//        }
//
//
//        /// <summary>
//        /// std::array based constructor
//        /// </summary>
//        /// <param name="first"> The std::array for the first element </param>
//        /// <param name="elm"> The std::arrays for the other elements. Optional params can be xk::nullopt to signify 0 length </param>
//        template <class OtherFirst, size_t Size, Span_compatible_optional_std_array<Ty, Size>... OtherTy>
//            requires std::is_convertible_v<OtherFirst(*)[], First(*)[]>
//        span_tuple(std::array<OtherFirst, Size>& first, OtherTy&... elm) :
//            base(std::forward_as_tuple(first.data(), optional_array<OtherTy, Ty, Size>(elm).data()...), std::size(first))
//        {
//
//        }
//
//        /// <summary>
//        /// const std::array based constructor
//        /// </summary>
//        /// <param name="first"> The std::array for the first element </param>
//        /// <param name="elm"> The std::arrays for the other elements. Optional params can be xk::nullopt to signify 0 length </param>
//        template <class OtherFirst, size_t Size, Span_compatible_optional_const_std_array<Ty, Size>... OtherTy>
//            requires std::is_convertible_v<const OtherFirst(*)[], First(*)[]>
//        span_tuple(const std::array<OtherFirst, Size>& first, const OtherTy&... elm) :
//            base(std::forward_as_tuple(first.data(), optional_array<OtherTy, Ty, Size>(elm).data()...), std::size(first))
//        {
//
//        }
//
//        /// <summary>
//        /// c-array based constructor with dynamic others
//        /// </summary>
//        /// <param name="first"> The c-array for the first element </param>
//        /// <param name="elm"> The c-arrays for the other elements </param>
//        template<size_t Size>
//        span_tuple(First(&first)[Size], Ty*... elm) :
//            base(std::forward_as_tuple(first, elm...), Size)
//        {
//
//        }
//
//        /// <summary>
//        /// std::array based constructor with dynamic others
//        /// </summary>
//        /// <param name="first"> The std::array for the first element </param>
//        /// <param name="elm"> The std::arrays for the other elements </param>
//        template <class OtherFirst, class... OtherTy, size_t Size>
//            requires std::is_convertible_v<OtherFirst(*)[], First(*)[]> &&
//            (std::is_convertible_v<OtherTy(*)[], Ty(*)[]> && ...)
//            span_tuple(std::array<OtherFirst, Size>& first, OtherTy*... elm) :
//            base(std::forward_as_tuple(first.data(), elm...), std::size(first))
//        {
//
//        }
//
//
//        /// <summary>
//        /// const std::array based constructor with dynamic others
//        /// </summary>
//        /// <param name="first"> The std::array for the first element </param>
//        /// <param name="elm"> The std::arrays for the other elements </param>
//        template <class OtherFirst, class... OtherTy, size_t Size>
//            requires std::is_convertible_v<const OtherFirst(*)[], First(*)[]> &&
//            (std::is_convertible_v<const OtherTy(*)[], Ty(*)[]>, ...)
//            span_tuple(const std::array<OtherFirst, Size>& first, const OtherTy*... elm) :
//            base(std::forward_as_tuple(first.data(), elm...), std::size(first))
//        {
//
//        }
//
//        /// <summary>
//        /// A copy / converting constructor that can take in other sized span_tuples
//        /// </summary>
//        /// <param name="other"> </param>
//        template <class OtherFirst, class... OtherTy, size_t OtherExtent>
//            requires std::is_convertible_v<OtherFirst(*)[], First(*)[]> &&
//        (std::is_convertible_v<OtherTy(*)[], Ty(*)[]> && ...)
//            constexpr explicit(OtherExtent == std::dynamic_extent) span_tuple(const span_tuple<OtherExtent, OtherFirst, OtherTy...>& other) noexcept
//            : base(other.data(), other.size())
//        {
//
//        }
//
//
//        /// <summary>
//        /// Ranged based constructor (Not implemented)
//        /// </summary>
//        /// <param name="first"> The range for the first element </param>
//        /// <param name="elm"> The ranges for the other elements </param>
//        //template <Span_compatible_range<First> _Rng, Span_compatible_range<Ty>... OtherRng>
//        //constexpr span_tuple(_Rng&& _Range, OtherRng&&... OtherRange)
//        //    : base(std::forward_as_tuple(::std::ranges::data(_Range), ::std::ranges::data(OtherRange)...), static_cast<size_type>(::std::ranges::size(_Range)))
//        //{
//
//        //}
//
//    public:
//        //Gets pointers to the beginning of all spans
//        constexpr pointer data() const noexcept { return m_data; }
//
//        //Gets pointer to the beginning of all span at a given tuple index
//        template<size_t Index>
//        constexpr std::tuple_element_t<Index, pointer> data() const noexcept
//        {
//            return get<Index>(m_data);
//        }
//
//        //Gets pointer to the beginning of all span at a given tuple index
//        template<class Index>
//        constexpr Index* data() const noexcept
//        {
//            return get<Index*>(m_data);
//        }
//
//        //Gets the size of the span
//        constexpr size_t size() const noexcept { return m_size; }
//
//        //Gets the size for a given span
//        template<size_t Index>
//        constexpr size_t size() const noexcept
//        {
//            return (std::get<Index>(m_data) == nullptr || m_size == 0) ? 0 : m_size;
//        }
//
//        //Gets the size for a given span
//        template<class Index>
//        constexpr size_t size() const noexcept
//        {
//            return (std::get<Index>(m_data) == nullptr || m_size == 0) ? 0 : m_size;
//        }
//
//        //Gets the total size in bytes that a given span is occupying
//        template<size_t Index>
//        constexpr size_t size_bytes() const noexcept
//        {
//            return (std::get<Index>(m_data) == nullptr || m_size == 0) ? 0 : sizeof(std::tuple_element_t<Index, value_type>) * m_size;
//        }
//
//        //Gets the total size in bytes that a given span is occupying
//        template<class Index>
//            requires std::same_as<Index, First> || (std::same_as<Index, span_element_t<Ty>> || ...)
//        constexpr size_t size_bytes() const noexcept
//        {
//            return (std::get<Index>(m_data) == nullptr || m_size == 0) ? 0 : sizeof(std::tuple_element_t<Index, value_type>) * m_size;
//        }
//
//        constexpr bool empty() const noexcept { return m_size == 0; }
//
//        template<size_t Index>
//        constexpr bool empty() const noexcept { return std::get<Index>(m_data) == nullptr || m_size == 0; }
//
//        template<class Index>
//        constexpr bool empty() const noexcept { return std::get<Index>(m_data) == nullptr || m_size == 0; }
//
//    //Creates a std::span from a span_tuple at the given Index
//        template<size_t Index, size_t Extent, class First, class... Ty>
//        friend constexpr auto get(span_tuple<Extent, First, Ty...> span);
//
//    //Creates a std::span from a span_tuple at the given Index
//        template<class Index, size_t Extent, class First, class... Ty>
//        friend constexpr auto get(span_tuple<Extent, First, Ty...> span);
//    };
//
//    template<size_t Extent, class First>
//    class span_tuple<Extent, First>;
//
//    //Creates a std::span from a span_tuple at the given Index
//    template<size_t Index, size_t Extent, class First, class... Ty>
//    constexpr auto get(span_tuple<Extent, First, Ty...> span)
//    {
//        return std::span<std::tuple_element_t<Index, typename span_tuple<Extent, First, Ty...>::value_type>, Extent>(get<Index>(span.data()), span.size());
//    }
//
//    //Creates a std::span from a span_tuple at the given Index
//    template<class Index, size_t Extent, class First, class... Ty>
//    constexpr auto get(span_tuple<Extent, First, Ty...> span)
//    {
//        return std::span<Index, Extent>(std::get<Index*>(span.data()), span.size());
//    }
//
//    template<std::contiguous_iterator Ty, std::contiguous_iterator... OtherTy>
//    span_tuple<::std::dynamic_extent, typename Ty::value_type, typename OtherTy::value_type...> make_span_tuple(Ty first, size_t count, OtherTy... others)
//    {
//        return span_tuple<::std::dynamic_extent, typename Ty::value_type, typename OtherTy::value_type...>{first, count, others...};
//    }
//
//    template<class Ty, class... OtherTy>
//    span_tuple<1, Ty, OtherTy...> make_span_tuple(Ty& first, OtherTy&... others)
//    {
//        return span_tuple<1, Ty, OtherTy...>{ first, others... };
//    }
//
//    template<std::contiguous_iterator Ty, std::sized_sentinel_for<Ty> _Sentinel, std::contiguous_iterator... OtherTy>
//    span_tuple<::std::dynamic_extent, typename Ty::value_type, typename OtherTy::value_type...> make_span_tuple(Ty first, _Sentinel _Last, OtherTy... others)
//    {
//        return span_tuple<::std::dynamic_extent, typename Ty::value_type, typename OtherTy::value_type...>{first, _Last, others...};
//    }
//
//    template<class Ty, class... OtherTy>
//    span_tuple<::std::dynamic_extent, Ty, OtherTy...> make_span_tuple(Ty* first, Ty* _Last, OtherTy*... others)
//    {
//        return span_tuple<::std::dynamic_extent, Ty, OtherTy...>{first, _Last, others...};
//    }
//
//    template<class Ty, class... OtherTy>
//    span_tuple<::std::dynamic_extent, Ty, OtherTy...> make_span_tuple(Ty* first, size_t count, OtherTy*... others)
//    {
//        return span_tuple<::std::dynamic_extent, Ty, OtherTy...>{first, count, others...};
//    }
//
//    template<class Ty, class... OtherTy, size_t Extent>
//    span_tuple<Extent, Ty, OtherTy...> make_span_tuple(Ty (&first)[Extent], OtherTy (&... others)[Extent])
//    {
//        return span_tuple<Extent, Ty, OtherTy...>{first, others...};
//    }
//
//    template<class Ty, class... OtherTy, size_t Extent>
//    span_tuple<Extent, Ty, OtherTy...> make_span_tuple(std::array<Ty, Extent>& first, std::array<OtherTy, Extent>&... others)
//    {
//        return span_tuple<Extent, Ty, OtherTy...>{first, others...};
//    }
//
//    template<class Ty, class... OtherTy, size_t Extent>
//    span_tuple<Extent, Ty, OtherTy...> make_span_tuple(const std::array<Ty, Extent>& first, const std::array<OtherTy, Extent>&... others)
//    {
//        return span_tuple<Extent, Ty, OtherTy...>{first, others...};
//    }
//
//    template<class Ty, class... OtherTy, size_t Extent>
//    span_tuple<::std::dynamic_extent, Ty, OtherTy...> make_span_tuple(Ty (&first)[Extent], OtherTy*... others)
//    {
//        return span_tuple<::std::dynamic_extent, Ty, OtherTy...>{first, others...};
//    }
//
//    template<class Ty, class... OtherTy, size_t Extent>
//    span_tuple<::std::dynamic_extent, Ty, OtherTy...> make_span_tuple(std::array<Ty, Extent>& first, OtherTy*... others)
//    {
//        return span_tuple<::std::dynamic_extent, Ty, OtherTy...>{first, others...};
//    }
//
//    template<class Ty, class... OtherTy, size_t Extent>
//    span_tuple<::std::dynamic_extent, Ty, OtherTy...> make_span_tuple(const std::array<Ty, Extent>& first, const OtherTy*... others)
//    {
//        return span_tuple<::std::dynamic_extent, Ty, OtherTy...>{first, others...};
//    }
//}
//
//namespace std
//{
//    template<size_t Extent, class First, class... Others>
//    struct tuple_size<xk::span_tuple<Extent, First, Others...>> : std::integral_constant<size_t, sizeof...(Others) + 1>
//    {
//
//    };
//
//    template<size_t Index, size_t Extent, class First, class... Others>
//    struct tuple_element<Index, xk::span_tuple<Extent, First, Others...>>
//    {
//        using type = std::span<std::tuple_element_t<Index, typename xk::span_tuple<Extent, First, Others...>::value_type>, Extent>;
//    };
//}