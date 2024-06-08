module;

#include "expected.hpp"
#include <wrl/client.h>

export module TypedD3D.Shared;

namespace TypedD3D
{
	export template<class Ty, class Err>
	using expected = tl::expected<Ty, Err>;

	export template<class Err>
	using unexpected = tl::unexpected<Err>;

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
    expected<ComPtr<Unknown>, HRESULT> IIDToObjectForwardFunction(Func&& function, Args&&... args)
    {
        ComPtr<Unknown> unknown;

        HRESULT hr = std::invoke(function, args..., IID_PPV_ARGS(&unknown));

        if(FAILED(hr))
            return unexpected(hr);

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
    expected<ComPtr<Unknown>, HRESULT> IIDToObjectForwardFunction(Func&& function, Obj&& obj, Args&&... args)
    {
        ComPtr<Unknown> unknown;

        HRESULT hr = std::invoke(function, obj, args..., IID_PPV_ARGS(&unknown));

        if(FAILED(hr))
            return unexpected(hr);

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
    expected<ComPtr<Unknown>, HRESULT> UnknownObjectForwardFunction(Func&& function, Args&&... args)
    {
        ComPtr<Unknown> unknown;

        HRESULT hr = std::invoke(function, args..., &unknown);

        if(FAILED(hr))
            return unexpected(hr);

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
    expected<ComPtr<Unknown>, HRESULT> UnknownObjectForwardFunction(Func&& function, Obj&& obj, Args&&... args)
    {
        ComPtr<Unknown> unknown;

        HRESULT hr = std::invoke(function, obj, args..., &unknown);

        if(FAILED(hr))
            return unexpected(hr);

        return unknown;
    }
}