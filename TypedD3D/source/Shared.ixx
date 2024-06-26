module;

#include <format>
#include <wrl/client.h>
#include <minwindef.h>
#include <concepts>
#include <stdexcept>

export module TypedD3D.Shared;

namespace TypedD3D
{
    export enum class Stride : UINT {};
    export enum class Offset : UINT {};

    export enum class FenceValue : UINT64 {};

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

    export class HRESULTError : public std::runtime_error
    {
    private:
        HRESULT errorCode;

    public:
        HRESULTError(HRESULT result) :
            std::runtime_error{ std::format("HRESULT Error {:x}", result) },
            errorCode{ result }
        {

        }

        HRESULT ErrorCode() const noexcept { return errorCode; }
    };

    export void ThrowIfFailed(HRESULT hr)
    {
        if(FAILED(hr))
            throw HRESULTError{ hr };
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


    export template<class Ty, template<class DerivedSelf> class InterfaceTy>
    struct IUnknownWrapperImpl : public InterfaceTy<IUnknownWrapperImpl<Ty, InterfaceTy>>
    {
    public:
        using value_type = Ty;
        using pointer = Ty*;
        using reference = Ty&;

        using interface_type = InterfaceTy<IUnknownWrapperImpl<Ty, InterfaceTy>>;

    public:
        Microsoft::WRL::ComPtr<value_type> m_ptr;

    public:
        IUnknownWrapperImpl() = default;

        template<class U>
        IUnknownWrapperImpl(U* obj) : m_ptr{ obj }
        {

        }

        IUnknownWrapperImpl(const Microsoft::WRL::ComPtr<Ty>& other) : m_ptr{ other }
        {
        }

        IUnknownWrapperImpl(Microsoft::WRL::ComPtr<Ty>&& other) noexcept : m_ptr{ std::move(other) }
        {
        }

        template<class OtherTy>
        IUnknownWrapperImpl(const Microsoft::WRL::ComPtr<OtherTy>& other) : m_ptr{ other }
        {
        }

        template<class OtherTy>
        IUnknownWrapperImpl(Microsoft::WRL::ComPtr<OtherTy>&& other) noexcept : m_ptr{ std::move(other) }
        {
        }

    public:
        pointer Get() { return m_ptr.Get(); }
        pointer Get() const { return m_ptr.Get(); }
        interface_type* operator->() { return static_cast<interface_type*>(this); }
        const interface_type* operator->() const { return static_cast<const interface_type*>(this); }
    };


    export template<class Ty>
    constexpr bool is_unknown_wrapper = false;

    export template<class Ty, template<class> class Traits>
    class IUnknownWrapper : private IUnknownWrapperImpl<Ty, Traits<Ty>::Interface>
    {
        template<class OtherTy, template<class> class OtherTrait>
        friend class IUnknownWrapper;


    private:
        using Base = IUnknownWrapperImpl<Ty, Traits<Ty>::Interface>;
        using Base::m_ptr;

    public:
        using value_type = Base::value_type;
        using pointer = Base::pointer;
        using reference = Base::reference;

        using interface_type = Base::interface_type;

    public:
        using traits_type = Traits<Ty>;

    public:
        IUnknownWrapper() = default;
        IUnknownWrapper(const IUnknownWrapper& other) = default;
        IUnknownWrapper(IUnknownWrapper&& other) noexcept = default;

        template<class OtherTy>
        IUnknownWrapper(const IUnknownWrapper<OtherTy, Traits>& other) : Base{ other.m_ptr }
        {
        }

        template<class OtherTy>
        IUnknownWrapper(IUnknownWrapper<OtherTy, Traits>&& other) noexcept : Base{ std::move(other.m_ptr) }
        {
        }

        //This is here in case of using mappers as Mapper<A>::type, and OtherMapper<A>::type is not seen as the
        //same type for template template classes, despite Mapper<A>::type<B> is the same as OtherMapper<A>::type<B>
        template<class OtherTy, template<class> class OtherTraits>
            requires std::same_as<OtherTraits<Ty>, Traits<Ty>>
        IUnknownWrapper(const IUnknownWrapper<OtherTy, OtherTraits>& other) : Base{ other.m_ptr }
        {
        }

        template<class OtherTy, template<class> class OtherTraits>
            requires std::same_as<OtherTraits<Ty>, Traits<Ty>>
        IUnknownWrapper(IUnknownWrapper<OtherTy, OtherTraits>&& other) noexcept : Base{ std::move(other.m_ptr) }
        {
        }

        IUnknownWrapper(std::nullptr_t) {};

        template<class U>
        IUnknownWrapper(U* obj) : Base{ obj }
        {
        }

        IUnknownWrapper(const Microsoft::WRL::ComPtr<Ty>& other) : Base{ other }
        {
        }

        IUnknownWrapper(Microsoft::WRL::ComPtr<Ty>&& other) noexcept : Base{ std::move(other) }
        {
        }

        template<class OtherTy>
        IUnknownWrapper(const Microsoft::WRL::ComPtr<OtherTy>& other) : Base{ other }
        {
        }

        template<class OtherTy>
        IUnknownWrapper(Microsoft::WRL::ComPtr<OtherTy>&& other) noexcept : Base{ std::move(other) }
        {
        }

    public:
        IUnknownWrapper& operator=(IUnknownWrapper& other) = default;
        IUnknownWrapper& operator=(IUnknownWrapper&& other) noexcept = default;

        IUnknownWrapper& operator=(std::nullptr_t)
        {
            m_ptr = nullptr;
            return *this;
        }

        template<class OtherTy>
        IUnknownWrapper& operator=(IUnknownWrapper<OtherTy, Traits>& other)
        {
            m_ptr = other;
            return *this;
        }

        template<class OtherTy>
        IUnknownWrapper& operator=(IUnknownWrapper<OtherTy, Traits>&& other) noexcept
        {
            m_ptr = (std::move(other));
            return *this;
        }

    public:
        friend bool operator==(const IUnknownWrapper& lh, std::nullptr_t)
        {
            return lh.m_ptr == nullptr;
        }
        friend bool operator!=(const IUnknownWrapper& lh, std::nullptr_t)
        {
            return lh.m_ptr != nullptr;
        }

        template<class OtherTy>
        friend bool operator==(const IUnknownWrapper& lh, const IUnknownWrapper<OtherTy, Traits>& rh)
        {
            return lh.m_ptr == rh.m_ptr;
        }
        template<class OtherTy>
        friend bool operator!=(const IUnknownWrapper& lh, const IUnknownWrapper<OtherTy, Traits>& rh)
        {
            return lh.m_ptr != rh.m_ptr;
        }

        using Base::operator->;

    public:
        pointer Detach() { return m_ptr.Detach(); }

        void Attach(pointer p)
        {
            m_ptr.Attach(p);
        }

        using Base::Get;
        Microsoft::WRL::ComPtr<value_type> AsComPtr() const { return m_ptr; }

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
        return IUnknownWrapper<DerivedTy, Traits>(Cast<DerivedTy>(other.m_ptr));
    }

    export template<std::derived_from<IUnknown> DerivedTy, std::derived_from<IUnknown> Ty, template<class> class Traits>
    IUnknownWrapper<DerivedTy, Traits> Cast(IUnknownWrapper<Ty, Traits>&& other) noexcept
    {
        return IUnknownWrapper<DerivedTy, Traits>(Cast<DerivedTy>(std::move(other.m_ptr)));
    }

    export template<class DerivedTy, std::derived_from<IUnknown> CastTy, template<class> class Traits>
        requires is_unknown_wrapper<DerivedTy>
    DerivedTy Cast(const IUnknownWrapper<CastTy, Traits>& other) noexcept
    {
        return DerivedTy(TypedD3D::Cast<typename DerivedTy::value_type>(other.m_ptr));
    }

    export template<class DerivedTy, std::derived_from<IUnknown> CastTy, template<class> class Traits>
        requires is_unknown_wrapper<DerivedTy>
    DerivedTy Cast(IUnknownWrapper<CastTy, Traits>&& other) noexcept
    {
        return DerivedTy(TypedD3D::Cast<typename DerivedTy::value_type>(std::move(other.m_ptr)));
    }

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