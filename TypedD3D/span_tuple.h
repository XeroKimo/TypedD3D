//*********************************************************
//
// Copyright (c) 2022 Renzy Alarcon
// Licensed under the MIT License (MIT).
//
//*********************************************************

#pragma once
#include <tuple>
#include <span>
#include <array>
#include <cassert>

#if __has_cpp_attribute(nodiscard)
#define XK_SPAN_TUPLE_NODISCARD [[nodiscard]]
#else
#define XK_SPAN_TUPLE_NODISCARD
#endif

namespace xk
{
    template<class First, size_t Extent, class... Ty>
    class span_tuple;

    template <class First, class... Others>
    struct span_tuple_iterator {
#ifdef __cpp_lib_concepts
        using iterator_concept = std::contiguous_iterator_tag;
#endif // __cpp_lib_concepts
        using iterator_category = std::random_access_iterator_tag;
        using value_type = std::tuple<std::remove_cv_t<First>, std::remove_cv_t<Others>...>;
        using difference_type = ptrdiff_t;
        using pointer = std::tuple<First*, Others*...>;
        using reference = std::tuple<First&, Others&...>;

        XK_SPAN_TUPLE_NODISCARD constexpr reference operator*() const noexcept {
            return std::apply([](auto&&... elements)
            {
                return reference(*elements...);
            }, _Myptr);
        }

        XK_SPAN_TUPLE_NODISCARD constexpr pointer operator->() const noexcept {
            return _Myptr;
        }

        constexpr span_tuple_iterator& operator++() noexcept {
            std::apply([](auto*&... ptrs)
            {
                (++ptrs, ...);
            }, _Myptr);
            return *this;
        }

        constexpr span_tuple_iterator operator++(int) noexcept {
            span_tuple_iterator _Tmp{ *this };
            ++* this;
            return _Tmp;
        }

        constexpr span_tuple_iterator& operator--() noexcept {
            std::apply([](auto*&... ptrs)
            {
                (--ptrs, ...);
            }, _Myptr);
            return *this;
        }

        constexpr span_tuple_iterator operator--(int) noexcept {
            span_tuple_iterator _Tmp{ *this };
            --* this;
            return _Tmp;
        }

        constexpr span_tuple_iterator& operator+=(const difference_type _Off) noexcept {
            std::apply([=](auto*&... ptrs)
            {
                ((ptrs += _Off), ...);
            }, _Myptr);
            return *this;
        }

        XK_SPAN_TUPLE_NODISCARD constexpr span_tuple_iterator operator+(const difference_type _Off) const noexcept {
            span_tuple_iterator _Tmp{ *this };
            _Tmp += _Off;
            return _Tmp;
        }

        XK_SPAN_TUPLE_NODISCARD friend constexpr span_tuple_iterator operator+(const difference_type _Off, span_tuple_iterator _Next) noexcept {
            _Next += _Off;
            return _Next;
        }

        constexpr span_tuple_iterator& operator-=(const difference_type _Off) noexcept {
            std::apply([=](auto*&... ptrs)
            {
                ((ptrs -= _Off), ...);
            }, _Myptr);
            return *this;
        }

        XK_SPAN_TUPLE_NODISCARD constexpr span_tuple_iterator operator-(const difference_type _Off) const noexcept {
            span_tuple_iterator _Tmp{ *this };
            _Tmp -= _Off;
            return _Tmp;
        }

        XK_SPAN_TUPLE_NODISCARD constexpr difference_type operator-(const span_tuple_iterator& _Right) const noexcept {
            return _Myptr - _Right._Myptr;
        }

        XK_SPAN_TUPLE_NODISCARD constexpr reference operator[](const difference_type _Off) const noexcept {
            return *(*this + _Off);
        }

        XK_SPAN_TUPLE_NODISCARD constexpr bool operator==(const span_tuple_iterator& _Right) const noexcept {

            return _Myptr == _Right._Myptr;
        }

        XK_SPAN_TUPLE_NODISCARD constexpr std::strong_ordering operator<=>(const span_tuple_iterator& _Right) const noexcept {
            return _Myptr <=> _Right._Myptr;
        }

        pointer _Myptr = nullptr;
    };

    template <class>
    inline constexpr bool Is_span_v = false;

    template <class _Ty, size_t _Extent>
    inline constexpr bool Is_span_v<std::span<_Ty, _Extent>> = true;

    template <class>
    inline constexpr bool Is_std_array_v = false;

    template <class _Ty, size_t _Size>
    inline constexpr bool Is_std_array_v<std::array<_Ty, _Size>> = true;

    template<class>
    constexpr size_t std_array_size = 0;

    template<class Ty, size_t Size>
    constexpr size_t std_array_size<std::array<Ty, Size>> = Size;


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

    template<class Ty>
    struct optional
    {
        using element_type = Ty;
        using value_type = std::remove_cv_t<Ty>;
        using pointer = Ty*;
        using const_pointer = const Ty*;
        using reference = Ty&;
        using const_reference = const Ty&;
    };

    template<class>
    constexpr bool is_optional_parameter = false;

    template<class Ty>
    constexpr bool is_optional_parameter<optional<Ty>> = true;

    template<class... Ty>
    constexpr bool is_any_optional = (is_optional_parameter<Ty> || ...);


    template<class First, size_t Extent, class... Ty>
    struct Extent_type
    {
        using pointer = std::tuple<First*, Ty*...>;

        constexpr Extent_type() noexcept = default;

        constexpr explicit Extent_type(pointer&& data, size_t) noexcept : m_data{ std::move(data) } {}

        pointer m_data = {};
        static constexpr size_t m_size = Extent;
    };

    template<class First, class... Ty>
    struct Extent_type<First, std::dynamic_extent, Ty...>
    {
        using pointer = std::tuple<First*, Ty*...>;

        constexpr Extent_type() noexcept = default;

        constexpr explicit Extent_type(pointer&& data, const size_t size) noexcept :
            m_data{ data },
            m_size{ size }
        {
        }

        pointer m_data = {};
        size_t m_size{ 0 };
    };

    template<class First, size_t Extent = std::dynamic_extent, class... Ty>
    class span_tuple : private Extent_type<First, Extent, Ty...>
    {
    private:
        using base = Extent_type<First, Extent, Ty...>;
        using base::m_data;
        using base::m_size;

    public:
        using element_type = std::tuple<First, Ty...>;
        using value_type = std::tuple<std::remove_cv_t<First>, std::remove_cv_t<Ty>...>;
        using pointer = std::tuple<First*, Ty*...>;
        using const_pointer = std::tuple<const First*, const Ty*...>;
        using reference = std::tuple<First&, Ty&...>;
        using const_reference = std::tuple<const First&, const Ty&...>;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using iterator = span_tuple_iterator<First, Ty...>;
        using reverse_iterator = ::std::reverse_iterator<iterator>;

        static constexpr size_type extent = Extent;

    public:
        constexpr span_tuple() noexcept requires (Extent == 0 || Extent == std::dynamic_extent) = default;

        /// <summary>
        /// begin iterator + size based constructor
        /// </summary>
        /// <param name="FirstIt"> The iterator for the first element </param>
        /// <param name="Count"> The size of the span </param>
        /// <param name="otherIt..."> The iterators for the other elements. All assumed to have the same length as Count </param>
        template <Span_compatible_iterator<First> It, Span_compatible_iterator<Ty>... OtherIt>
        constexpr explicit(Extent != std::dynamic_extent) span_tuple(It FirstIt, size_type Count, OtherIt... otherIt) noexcept // strengthened
            : base(std::forward_as_tuple(std::to_address(FirstIt), std::to_address(otherIt)...), Count)
        {
            if constexpr(Extent != std::dynamic_extent)
            {
                assert(Count == Extent &&
                    "Cannot construct span with static extent from range [first, first + count) as count != extent");
            }
        }


        /// <summary>
        /// begin + end iterator based constructor
        /// </summary>
        /// <param name="_First"> The iterator for the first element </param>
        /// <param name="_Last"> The end point for the first iterator </param>
        /// <param name="otherIt..."> The iterators for the other elements. Assumed to have the same range as (_Last - _First) </param>
        template <Span_compatible_iterator<First> _It, Span_compatible_sentinel<_It> _Sentinel, Span_compatible_iterator<Ty>... OtherIt>
        constexpr explicit(Extent != std::dynamic_extent) span_tuple(_It _First, _Sentinel _Last, OtherIt... otherIt)
            noexcept(noexcept(_Last - _First)) // strengthened
            : base(std::forward_as_tuple(std::to_address(_First), std::to_address(otherIt)...), static_cast<size_type>(_Last - _First))
        {
            if constexpr(Extent != std::dynamic_extent)
            {
                assert(_Last - _First == Extent &&
                    "Cannot construct span with static extent from range [first, last) as last - first != extent");
            }
        }


        /// <summary>
        /// c-array based constructor
        /// </summary>
        /// <param name="first"> The c-array for the first element </param>
        /// <param name="elm"> The c-arrays for the other elements </param>
        template<size_t Size>
            requires(Extent == std::dynamic_extent || Extent == Size)
        span_tuple(First(&first)[Size], Ty(&...elm)[Size]) :
            base(std::forward_as_tuple(first, elm...), Size)
        {

        }

        /// <summary>
        /// std::array based constructor
        /// </summary>
        /// <param name="first"> The std::array for the first element </param>
        /// <param name="elm"> The std::arrays for the other elements </param>
        template <class OtherFirst, class... OtherTy, size_t Size>
            requires (Extent == std::dynamic_extent || Extent == Size) &&
        std::is_convertible_v<OtherFirst(*)[], First(*)[]> &&
            (std::is_convertible_v<OtherTy(*)[], Ty(*)[]> && ...)
            span_tuple(std::array<OtherFirst, Size>& first, std::array<OtherTy, Size>&... elm) :
            base(std::forward_as_tuple(first.data(), elm.data()...), std::size(first))
        {

        }


        /// <summary>
        /// const std::array based constructor
        /// </summary>
        /// <param name="first"> The std::array for the first element </param>
        /// <param name="elm"> The std::arrays for the other elements </param>
        template <class OtherFirst, class... OtherTy, size_t Size>
            requires (Extent == std::dynamic_extent || Extent == Size) &&
        std::is_convertible_v<const OtherFirst(*)[], First(*)[]> &&
            (std::is_convertible_v<const OtherTy(*)[], Ty(*)[]>, ...)
            span_tuple(const std::array<OtherFirst, Size>& first, const std::array<OtherTy, Size>&... elm) :
            base(std::forward_as_tuple(first.data(), elm.data()...), std::size(first))
        {

        }

        /// <summary>
        /// A copy / converting constructor that can take in other sized span_tuples. Cannot take in span_tuples with optional parameters
        /// </summary>
        /// <param name="other"> </param>
        template <class OtherFirst, class... OtherTy, size_t OtherExtent>
            requires (Extent == std::dynamic_extent || OtherExtent == std::dynamic_extent || Extent == OtherExtent) &&
        std::is_convertible_v<OtherFirst(*)[], First(*)[]> &&
            (std::is_convertible_v<OtherTy(*)[], Ty(*)[]> && ...) &&
            (!is_any_optional<OtherTy...>)
            constexpr explicit(Extent != std::dynamic_extent && OtherExtent == std::dynamic_extent) span_tuple(const span_tuple<OtherFirst, OtherExtent, OtherTy...>& other) noexcept
            : base(other.data(), other.size())
        {
            if constexpr(Extent != std::dynamic_extent)
            {
                assert(other.size() == Extent &&
                    "Cannot construct span with static extent from other span as other.size() != extent");
            }

        }

        /// <summary>
        /// Ranged based constructor
        /// </summary>
        /// <param name="first"> The range for the first element </param>
        /// <param name="elm"> The ranges for the other elements </param>
        template <Span_compatible_range<First> _Rng, Span_compatible_range<Ty>... OtherRng>
        constexpr explicit(Extent != std::dynamic_extent) span_tuple(_Rng&& _Range, OtherRng&&... OtherRange)
            : base(std::forward_as_tuple(::std::ranges::data(_Range), ::std::ranges::data(OtherRange)...), static_cast<size_type>(::std::ranges::size(_Range)))
        {
            if constexpr(Extent != std::dynamic_extent)
            {
                assert(::std::ranges::size(_Range) == Extent &&
                    "Cannot construct span with static extent from range r as std::ranges::size(r) != extent");
            }
        }

    public:

            // [span.sub] Subviews
        template <size_t _Count>
        XK_SPAN_TUPLE_NODISCARD constexpr auto first() const noexcept /* strengthened */
        {
            if constexpr(Extent != std::dynamic_extent)
            {
                static_assert(_Count <= Extent, "Count out of range in span::first()");
            }

            assert(_Count <= m_size && "Count out of range in span::first()");

            return first_impl<_Count>(std::make_index_sequence<sizeof...(Ty)>());
        }

        XK_SPAN_TUPLE_NODISCARD constexpr auto first(const size_type _Count) const noexcept
        {
            assert(_Count <= m_size && "Count out of range in span::first(count)");
            return first_impl(_Count, std::make_index_sequence<sizeof...(Ty)>());
        }

        template <size_t _Count>
        XK_SPAN_TUPLE_NODISCARD constexpr auto last() const noexcept /* strengthened */
        {
            if constexpr(Extent != std::dynamic_extent)
            {
                static_assert(_Count <= Extent, "Count out of range in span::last()");
            }

            assert(_Count <= m_size && "Count out of range in span::last()");
            return last_impl<_Count>(std::make_index_sequence<sizeof...(Ty)>());
        }

        XK_SPAN_TUPLE_NODISCARD constexpr auto last(const size_type _Count) const noexcept /* strengthened */ {

            assert(_Count <= m_size && "Count out of range in span::last(count)");
            return last_impl(_Count, std::make_index_sequence<sizeof...(Ty)>());
        }

        template <size_t _Offset, size_t _Count = std::dynamic_extent>
        XK_SPAN_TUPLE_NODISCARD constexpr auto subspan() const noexcept /* strengthened */
        {
            if constexpr(Extent != std::dynamic_extent)
            {
                static_assert(_Offset <= Extent, "Offset out of range in span::subspan()");
                static_assert(_Count == std::dynamic_extent || _Count <= Extent - _Offset, "Count out of range in span::subspan()");
            }

            assert(_Offset <= m_size && "Offset out of range in span::subspan(offset, count)");
            assert(_Count <= m_size - _Offset && "Count out of range in span::subspan()");
            using _ReturnType = span_tuple<First,
                _Count != std::dynamic_extent ? _Count : (Extent != std::dynamic_extent ? Extent - _Offset : std::dynamic_extent), Ty...>;

            return subspan_impl<_Offset, _Count>(std::make_index_sequence<sizeof...(Ty)>());
        }

        XK_SPAN_TUPLE_NODISCARD constexpr auto subspan(const size_type _Offset, const size_type _Count = std::dynamic_extent) const noexcept
        {
            assert(_Offset <= m_size && "Offset out of range in span::subspan(offset, count)");
            assert((_Count == std::dynamic_extent || _Count <= m_size - _Offset) && "Count out of range in span::subspan(offset, count)");

            using _ReturnType = span_tuple<First, std::dynamic_extent, Ty...>;
            return subspan_impl(_Offset, _Count, std::make_index_sequence<sizeof...(Ty)>());
        }

    public:
        //Gets all the elements of a given offset
        constexpr reference operator[](size_t offset) const noexcept
        {
            assert(offset < m_size && "span index out of range");
            return std::apply([offset](auto&&... elements)
            {
                return reference(elements[offset]...);
            }, m_data);
        }

        //Gets the first element of all spans
        constexpr reference front() const noexcept
        {
            assert(m_size > 0 && "front of empty span");
            return std::apply([](auto&&... elements)
            {
                return reference(*elements...);
            }, m_data);
        }

        //Gets the first element of a given span
        template<size_t Index>
        constexpr std::tuple_element_t<Index, reference> front() const noexcept
        {
            assert(m_size > 0 && "front of empty span");
            return *get<Index>(m_data);
        }

        //Gets the first element of a given span
        template<class Index>
        constexpr Index& front() const noexcept
        {
            assert(m_size > 0 && "front of empty span");
            return *get<Index*>(m_data);
        }

        //Gets the last element of all spans
        constexpr reference back() const noexcept
        {
            assert(m_size > 0 && "back of empty span");
            return std::apply([offset = m_size - 1](auto&&... elements)
            {
                return reference(elements[offset]...);
            }, m_data);
        }

        //Gets the last element of a given span
        template<size_t Index>
        constexpr std::tuple_element_t<Index, reference> back() const noexcept
        {
            assert(m_size > 0 && "back of empty span");
            return get<Index>(m_data)[m_size - 1];
        }

        //Gets the last element of a given span
        template<class Index>
        constexpr Index& back() const noexcept
        {
            assert(m_size > 0 && "back of empty span");
            return get<Index*>(m_data)[m_size - 1];
        }

        //Gets pointer to the beginning of all span at a given tuple index
        constexpr pointer data() const noexcept { return m_data; }

        //Gets pointer to the beginning of all span at a given tuple index
        template<size_t Index>
        constexpr std::tuple_element_t<Index, pointer> data() const noexcept
        {
            return get<Index>(m_data);
        }

        //Gets pointer to the beginning of all span at a given tuple index
        template<class Index>
        constexpr Index* data() const noexcept
        {
            return get<Index*>(m_data);
        }

        //Gets the size of the span
        constexpr size_t size() const noexcept { return m_size; }

        //Gets the size of the span
        template<size_t Index>
        constexpr size_t size() const noexcept
        {
            return m_size;
        }

        //Gets the size of the span
        template<class Index>
        constexpr size_t size() const noexcept
        {
            return m_size;
        }

        //Gets the total size in bytes that a given span is occupying
        template<size_t Index>
        constexpr size_t size_bytes() const noexcept { return sizeof(std::tuple_element_t<Index, value_type>) * m_size; }

        //Gets the total size in bytes that a given span is occupying
        template<class Index>
            requires std::same_as<Index, First> || (std::same_as<Index, Ty> || ...)
        constexpr size_t size_bytes() const noexcept { return sizeof(Index) * m_size; }

        constexpr bool empty() const noexcept { return m_size == 0; }

        template<size_t Index>
        constexpr bool empty() const noexcept { return m_size == 0; }

        template<class Index>
        constexpr bool empty() const noexcept { return m_size == 0; }

    //Creates a std::span from a span_tuple at the given Index
        template<size_t Index, class First, size_t Extent, class... Ty>
        friend constexpr auto get(span_tuple<First, Extent, Ty...> span);

    //Creates a std::span from a span_tuple at the given Index
        template<class Index, class First, size_t Extent, class... Ty>
        friend constexpr auto get(span_tuple<First, Extent, Ty...> span);


    // [span.iterators] Iterator support
        XK_SPAN_TUPLE_NODISCARD constexpr iterator begin() const noexcept
        {
            const auto _End = std::apply([size = m_size](auto*... ptrs)
            {
                ((ptrs += size), ...);
                return std::tuple(ptrs...);
            }, m_data);
            return { m_data };
        }

        XK_SPAN_TUPLE_NODISCARD constexpr iterator end() const noexcept
        {
            const auto _End = std::apply([size = m_size](auto*... ptrs)
            {
                ((ptrs += size), ...);
                return std::tuple(ptrs...);
            }, m_data);
            return { _End };
        }

        XK_SPAN_TUPLE_NODISCARD constexpr reverse_iterator rbegin() const noexcept
        {
            return reverse_iterator{ end() };
        }

        XK_SPAN_TUPLE_NODISCARD constexpr reverse_iterator rend() const noexcept
        {
            return reverse_iterator{ begin() };
        }

    private:
        template<size_t Count, class T, T... Indexs>
        XK_SPAN_TUPLE_NODISCARD constexpr auto first_impl(std::index_sequence<Indexs...>) const noexcept
        {
            return span_tuple<First, Count, Ty...>{std::get<0>(m_data), Count, std::get<Indexs + 1>(m_data)...};
        }

        template<class T, T... Indexs>
        XK_SPAN_TUPLE_NODISCARD constexpr auto first_impl(size_t count, std::index_sequence<Indexs...>) const noexcept
        {
            return span_tuple<First, std::dynamic_extent, Ty...>{std::get<0>(m_data), count, std::get<Indexs + 1>(m_data)...};
        }
        template<size_t Count, class T, T... Indexs>
        XK_SPAN_TUPLE_NODISCARD constexpr auto last_impl(std::index_sequence<Indexs...>) const noexcept
        {
            return span_tuple<First, Count, Ty...>{std::get<0>(m_data) + (m_size - Count), Count, (std::get<Indexs + 1>(m_data) + (m_size - Count))...};
        }

        template<class T, T... Indexs>
        XK_SPAN_TUPLE_NODISCARD constexpr auto last_impl(size_t count, std::index_sequence<Indexs...>) const noexcept
        {
            return span_tuple<First, std::dynamic_extent, Ty...>{std::get<0>(m_data) + (m_size - count), count, (std::get<Indexs + 1>(m_data) + (m_size - count))...};
        }

        template<size_t Offset, size_t Count, class T, T... Indexs>
        XK_SPAN_TUPLE_NODISCARD constexpr auto subspan_impl(std::index_sequence<Indexs...>) const noexcept
        {
            using _ReturnType = span_tuple<First,
                Count != std::dynamic_extent ? Count : (Extent != std::dynamic_extent ? Extent - Offset : std::dynamic_extent), Ty...>;
            return _ReturnType{ std::get<0>(m_data) + Offset, Count == std::dynamic_extent ? m_size - Offset : Count, (std::get<Indexs + 1>(m_data) + Offset)... };
        }

        template<class T, T... Indexs>
        XK_SPAN_TUPLE_NODISCARD constexpr auto subspan_impl(size_t offset, size_t count, std::index_sequence<Indexs...>) const noexcept
        {
            return span_tuple<First, std::dynamic_extent, Ty...>{ std::get<0>(m_data) + offset, count == std::dynamic_extent ? m_size - offset : count, (std::get<Indexs + 1>(m_data) + offset)... };
        }
    };

    template<class Ty>
    struct span_element_type
    {
        using type = Ty;
    };

    template<class Ty>
    struct span_element_type<optional<Ty>>
    {
        using type = Ty;
    };

    template<class Ty>
    using span_element_t = typename span_element_type<Ty>::type;

    struct null_optional_type
    {
    };

    constexpr null_optional_type nullopt;

    template<class>
    constexpr bool is_null_opt = false;

    template<>
    constexpr bool is_null_opt<null_optional_type> = true;

    template<class It, class Ty>
    struct optional_iterator
    {
        using pointer = typename Ty::pointer;
        using element_type = typename Ty::element_type;
        using difference_type = std::ptrdiff_t;

        optional_iterator(It it) : val(it) {}

        It val = {};

        operator It () { return val; }
        constexpr pointer operator->() noexcept
        {
            return std::to_address(val);
        }
        constexpr pointer operator->() const noexcept
        {
            return std::to_address(val);
        }
    };

    template<class Ty>
    struct optional_iterator<null_optional_type, Ty>
    {
        using pointer = typename Ty::pointer;
        using element_type = typename Ty::element_type;
        using difference_type = std::ptrdiff_t;

        optional_iterator(null_optional_type) {};
        constexpr pointer operator->() noexcept
        {
            return nullptr;
        }
        constexpr pointer operator->() const noexcept
        {
            return nullptr;
        }
    };

    template<class It, class Ty, size_t Size>
    struct optional_array
    {
        using pointer = typename Ty::pointer;

        optional_array(Ty::element_type(&arr)[Size]) : val(arr) {}
        optional_array(std::array<typename Ty::element_type, Size>& arr) : val(arr.data()) {}
        optional_array(const std::array<typename Ty::element_type, Size>& arr) : val(arr.data()) {}

        pointer val;

        pointer data() const { return val; }
    };

    template<class Ty, size_t Size>
    struct optional_array<null_optional_type, Ty, Size>
    {
        using pointer = typename Ty::pointer;

        optional_array(null_optional_type) {};
        pointer data() const { return nullptr; }
    };

    template <class _It, class _Ty>
    concept Span_compatible_optional_iterator = std::contiguous_iterator<_It>
        && std::is_convertible_v<std::remove_reference_t<std::iter_reference_t<_It>>(*)[], span_element_t<_Ty>(*)[]>
        || (is_optional_parameter<_Ty> && is_null_opt<_It>);

    template <class OtherTy, size_t Size, class Ty>
    concept Span_compatible_optional_array = std::is_bounded_array_v<OtherTy[Size]>
        || (is_optional_parameter<Ty> && is_null_opt<Ty>);

    template <class OtherTy, class Ty, size_t Size>
    concept Span_compatible_optional_std_array = Is_std_array_v<OtherTy> &&
        std::is_convertible_v<typename OtherTy::value_type(*)[], span_element_t<Ty>(*)[]> &&
        std_array_size<OtherTy> == Size ||
        (is_optional_parameter<Ty> && is_null_opt<Ty>);

    template <class OtherTy, class Ty, size_t Size>
    concept Span_compatible_optional_const_std_array = Is_std_array_v<OtherTy> &&
        std::is_convertible_v<const typename OtherTy::value_type(*)[], span_element_t<Ty>(*)[]> &&
        std_array_size<OtherTy> == Size ||
        (is_optional_parameter<Ty> && is_null_opt<Ty>);

    template<class First, class... Ty>
        requires is_any_optional<Ty...>
    class span_tuple<First, std::dynamic_extent, Ty...> : private Extent_type<First, std::dynamic_extent, span_element_t<Ty>...>
    {
    private:
        using base = Extent_type<First, std::dynamic_extent, span_element_t<Ty>...>;
        using base::m_data;
        using base::m_size;

    public:
        using element_type = std::tuple<First, span_element_t<Ty>...>;
        using value_type = std::tuple<std::remove_cv_t<First>, std::remove_cv_t<span_element_t<Ty>>...>;
        using pointer = std::tuple<First*, span_element_t<Ty>*...>;
        using const_pointer = std::tuple<const First*, const span_element_t<Ty>*...>;
        using reference = std::tuple<First&, span_element_t<Ty>&...>;
        using const_reference = std::tuple<const First&, const span_element_t<Ty>&...>;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using iterator = span_tuple_iterator<First, span_element_t<Ty>...>;
        using reverse_iterator = ::std::reverse_iterator<iterator>;

        static constexpr size_type extent = std::dynamic_extent;

    public:
        constexpr span_tuple() noexcept = default;

        /// <summary>
        /// begin iterator + size based constructor
        /// </summary>
        /// <param name="FirstIt"> The iterator for the first element </param>
        /// <param name="Count"> The size of the span </param>
        /// <param name="otherIt..."> The iterators for the other elements. All assumed to have the same length as Count. Optional params can be xk::nullopt to signify 0 length </param>
        template <Span_compatible_iterator<First> It, Span_compatible_optional_iterator<Ty>... OtherIt>
        constexpr span_tuple(It FirstIt, size_type Count, OtherIt... otherIt) noexcept // strengthened
            : base(std::forward_as_tuple(std::to_address(FirstIt), std::to_address(optional_iterator<OtherIt, Ty>(otherIt))...), Count)
        {
        }


        /// <summary>
        /// begin + end iterator based constructor
        /// </summary>
        /// <param name="_First"> The iterator for the first element </param>
        /// <param name="_Last"> The end point for the first iterator </param>
        /// <param name="otherIt..."> The iterators for the other elements. Assumed to have the same range as (_Last - _First). Optional params can be xk::nullopt to signify 0 length </param>
        template <Span_compatible_iterator<First> _It, Span_compatible_sentinel<_It> _Sentinel, Span_compatible_optional_iterator<Ty>... OtherIt>
        constexpr span_tuple(_It _First, _Sentinel _Last, OtherIt... otherIt) noexcept(noexcept(_Last - _First)) // strengthened
            : base(std::forward_as_tuple(std::to_address(_First), std::to_address(optional_iterator<OtherIt, Ty>(otherIt))...), static_cast<size_type>(_Last - _First))
        {

        }

        /// <summary>
        /// c-array based constructor
        /// </summary>
        /// <param name="first"> The c-array for the first element </param>
        /// <param name="elm"> The c-arrays for the other elements. Optional params can be xk::nullopt to signify 0 length </param>
        template<size_t Size, Span_compatible_optional_array<Size, Ty>... OtherTy>
        span_tuple(First(&first)[Size], OtherTy&... Ty) :
            base(std::forward_as_tuple(first, optional_array<OtherTy, Ty, Size>(Ty).data()...), Size)
        {

        }


        /// <summary>
        /// std::array based constructor
        /// </summary>
        /// <param name="first"> The std::array for the first element </param>
        /// <param name="elm"> The std::arrays for the other elements. Optional params can be xk::nullopt to signify 0 length </param>
        template <class OtherFirst, size_t Size, Span_compatible_optional_std_array<Ty, Size>... OtherTy>
            requires std::is_convertible_v<OtherFirst(*)[], First(*)[]>
        span_tuple(std::array<OtherFirst, Size>& first, OtherTy&... elm) :
            base(std::forward_as_tuple(first.data(), optional_array<OtherTy, Ty, Size>(elm).data()...), std::size(first))
        {

        }

        /// <summary>
        /// const std::array based constructor
        /// </summary>
        /// <param name="first"> The std::array for the first element </param>
        /// <param name="elm"> The std::arrays for the other elements. Optional params can be xk::nullopt to signify 0 length </param>
        template <class OtherFirst, size_t Size, Span_compatible_optional_const_std_array<Ty, Size>... OtherTy>
            requires std::is_convertible_v<const OtherFirst(*)[], First(*)[]>
        span_tuple(const std::array<OtherFirst, Size>& first, const OtherTy&... elm) :
            base(std::forward_as_tuple(first.data(), optional_array<OtherTy, Ty, Size>(elm).data()...), std::size(first))
        {

        }

        /// <summary>
        /// A copy / converting constructor that can take in other sized span_tuples
        /// </summary>
        /// <param name="other"> </param>
        template <class OtherFirst, class... OtherTy, size_t OtherExtent>
            requires std::is_convertible_v<OtherFirst(*)[], First(*)[]> &&
        (std::is_convertible_v<OtherTy(*)[], Ty(*)[]> && ...)
            constexpr explicit(OtherExtent == std::dynamic_extent) span_tuple(const span_tuple<OtherFirst, OtherExtent, OtherTy...>& other) noexcept
            : base(other.data(), other.size())
        {

        }


        /// <summary>
        /// Ranged based constructor (Not implemented)
        /// </summary>
        /// <param name="first"> The range for the first element </param>
        /// <param name="elm"> The ranges for the other elements </param>
        //template <Span_compatible_range<First> _Rng, Span_compatible_range<Ty>... OtherRng>
        //constexpr span_tuple(_Rng&& _Range, OtherRng&&... OtherRange)
        //    : base(std::forward_as_tuple(::std::ranges::data(_Range), ::std::ranges::data(OtherRange)...), static_cast<size_type>(::std::ranges::size(_Range)))
        //{

        //}

    public:
        //Gets pointers to the beginning of all spans
        constexpr pointer data() const noexcept { return m_data; }

        //Gets pointer to the beginning of all span at a given tuple index
        template<size_t Index>
        constexpr std::tuple_element_t<Index, pointer> data() const noexcept
        {
            return get<Index>(m_data);
        }

        //Gets pointer to the beginning of all span at a given tuple index
        template<class Index>
        constexpr Index* data() const noexcept
        {
            return get<Index*>(m_data);
        }

        //Gets the size of the span
        constexpr size_t size() const noexcept { return m_size; }

        //Gets the size for a given span
        template<size_t Index>
        constexpr size_t size() const noexcept
        {
            return (std::get<Index>(m_data) == nullptr || m_size == 0) ? 0 : m_size;
        }

        //Gets the size for a given span
        template<class Index>
        constexpr size_t size() const noexcept
        {
            return (std::get<Index>(m_data) == nullptr || m_size == 0) ? 0 : m_size;
        }

        //Gets the total size in bytes that a given span is occupying
        template<size_t Index>
        constexpr size_t size_bytes() const noexcept
        {
            return (std::get<Index>(m_data) == nullptr || m_size == 0) ? 0 : sizeof(std::tuple_element_t<Index, value_type>) * m_size;
        }

        //Gets the total size in bytes that a given span is occupying
        template<class Index>
            requires std::same_as<Index, First> || (std::same_as<Index, span_element_t<Ty>> || ...)
        constexpr size_t size_bytes() const noexcept
        {
            return (std::get<Index>(m_data) == nullptr || m_size == 0) ? 0 : sizeof(std::tuple_element_t<Index, value_type>) * m_size;
        }

        constexpr bool empty() const noexcept { return m_size == 0; }

        template<size_t Index>
        constexpr bool empty() const noexcept { return std::get<Index>(m_data) == nullptr || m_size == 0; }

        template<class Index>
        constexpr bool empty() const noexcept { return std::get<Index>(m_data) == nullptr || m_size == 0; }

    //Creates a std::span from a span_tuple at the given Index
        template<size_t Index, class First, size_t Extent, class... Ty>
        friend constexpr auto get(span_tuple<First, Extent, Ty...> span);

    //Creates a std::span from a span_tuple at the given Index
        template<class Index, class First, size_t Extent, class... Ty>
        friend constexpr auto get(span_tuple<First, Extent, Ty...> span);
    };

    template<class First, size_t Extent>
    class span_tuple<First, Extent>;

    //Creates a std::span from a span_tuple at the given Index
    template<size_t Index, class First, size_t Extent, class... Ty>
    constexpr auto get(span_tuple<First, Extent, Ty...> span)
    {
        return std::span<std::tuple_element_t<Index, typename span_tuple<First, Extent, Ty...>::value_type>, Extent>(get<Index>(span.data()), span.size());
    }

    //Creates a std::span from a span_tuple at the given Index
    template<class Index, class First, size_t Extent, class... Ty>
    constexpr auto get(span_tuple<First, Extent, Ty...> span)
    {
        return std::span<Index, Extent>(std::get<Index*>(span.data()), span.size());
    }
}

namespace std
{
    template<class First, size_t Extent, class... Others>
    struct tuple_size<xk::span_tuple<First, Extent, Others...>> : std::integral_constant<size_t, sizeof...(Others) + 1>
    {

    };

    template<size_t Index, class First, size_t Extent, class... Others>
    struct tuple_element<Index, xk::span_tuple<First, Extent, Others...>>
    {
        using type = std::span<std::tuple_element_t<Index, typename xk::span_tuple<First, Extent, Others...>::value_type>, Extent>;
    };
}