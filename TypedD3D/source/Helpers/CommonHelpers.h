#pragma once
#include "expected.hpp"
#include "COMHelpers.h"

namespace TypedD3D::Helpers::Common
{
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

    template<class IDXGIObj>
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