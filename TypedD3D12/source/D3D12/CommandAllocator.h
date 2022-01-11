#pragma once

#include <d3d12.h>
#include <wrl/client.h>

namespace TypedD3D::D3D12::CommandAllocator
{
    namespace Internal
    {
        using Microsoft::WRL::ComPtr;
        class CommandAllocator
        {
        private:
            ComPtr<ID3D12CommandAllocator> m_allocator;

        public:
            ComPtr<ID3D12CommandAllocator> Get() const { return m_allocator; }
        };
    }

    class Direct : private Internal::CommandAllocator
    {
        using Base = Internal::CommandAllocator;

    public:
        using Base::Get;
    };

    class Bundle : private Internal::CommandAllocator
    {
        using Base = Internal::CommandAllocator;

    public:
        using Base::Get;
    };

    class Compute : private Internal::CommandAllocator
    {
        using Base = Internal::CommandAllocator;

    public:
        using Base::Get;
    };

    class Copy : private Internal::CommandAllocator
    {
        using Base = Internal::CommandAllocator;

    public:
        using Base::Get;
    };
};