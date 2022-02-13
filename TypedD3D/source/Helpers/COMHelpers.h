//*********************************************************
//
// Copyright (c) 2022 Renzy Alarcon
// Licensed under the MIT License (MIT).
//
//*********************************************************
#pragma once
#include "../Utils.h"
#include <wrl/client.h>

namespace TypedD3D::Helpers::COM
{
    template<class To, class From>
    Microsoft::WRL::ComPtr<To> Cast(Microsoft::WRL::ComPtr<From> from)
    {
        Microsoft::WRL::ComPtr<To> to;
        from.As(&to);
        return to;
    }

    template<class To, class From>
    To* Cast(From* from)
    {
        To* to;
        from->QueryInterface(&to);
        return to;
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
    template<class Unknown, class Func, class... Args>
    Utils::Expected<Microsoft::WRL::ComPtr<Unknown>, HRESULT> IIDToObjectForwardFunction(Func&& function, Args&&... args)
    {
        using Microsoft::WRL::ComPtr;
        ComPtr<Unknown> unknown;

        HRESULT hr = std::invoke(function, args..., IID_PPV_ARGS(&unknown));

        if(FAILED(hr))
            return Utils::Unexpected(hr);

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
    template<class Unknown, class Func, class Obj, class... Args>
    Utils::Expected<Microsoft::WRL::ComPtr<Unknown>, HRESULT> IIDToObjectForwardFunction(Func&& function, Obj&& obj, Args&&... args)
    {
        using Microsoft::WRL::ComPtr;
        ComPtr<Unknown> unknown;

        HRESULT hr = std::invoke(function, obj, args..., IID_PPV_ARGS(&unknown));

        if(FAILED(hr))
            return Utils::Unexpected(hr);

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
    template<class Unknown, class Func, class... Args>
    Utils::Expected<Microsoft::WRL::ComPtr<Unknown>, HRESULT> UnknownObjectForwardFunction(Func&& function, Args&&... args)
    {
        using Microsoft::WRL::ComPtr;
        ComPtr<Unknown> unknown;

        HRESULT hr = std::invoke(function, args..., &unknown);

        if(FAILED(hr))
            return Utils::Unexpected(hr);

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
    template<class Unknown, class Func, class Obj, class... Args>
    Utils::Expected<Microsoft::WRL::ComPtr<Unknown>, HRESULT> UnknownObjectForwardFunction(Func&& function, Obj&& obj, Args&&... args)
    {
        using Microsoft::WRL::ComPtr;
        ComPtr<Unknown> unknown;

        HRESULT hr = std::invoke(function, obj, args..., &unknown);

        if(FAILED(hr))
            return Utils::Unexpected(hr);

        return unknown;
    }
};