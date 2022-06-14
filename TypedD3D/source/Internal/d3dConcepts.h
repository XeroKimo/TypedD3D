#pragma once
#include <concepts>

struct ID3D11Resource;
struct ID3D12Resource;

namespace TypedD3D::Internal
{
    template<class Ty>
    concept Resource = std::derived_from<Ty, ID3D11Resource> || std::derived_from<Ty, ID3D12Resource>;
}