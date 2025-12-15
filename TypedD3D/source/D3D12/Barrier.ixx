module;

#include <d3d12.h>

export module TypedD3D12:Barrier;
import std;
import TypedD3D.Shared;
import :Resource;

namespace TypedD3D::D3D12
{
	export struct BeforeTag {};
	export struct AfterTag {};

	export template<class Ty>
	concept BarrierEnabledType = std::same_as<Ty, D3D12_GLOBAL_BARRIER> || std::same_as<Ty, D3D12_TEXTURE_BARRIER> || std::same_as<Ty, D3D12_BUFFER_BARRIER>;

	export template <BarrierEnabledType Ty>
	constexpr D3D12_BARRIER_TYPE BarrierTypeToEnum = static_cast<D3D12_BARRIER_TYPE>(-1);

	template <>
	constexpr D3D12_BARRIER_TYPE BarrierTypeToEnum<D3D12_GLOBAL_BARRIER> = D3D12_BARRIER_TYPE_GLOBAL;

	template <>
	constexpr D3D12_BARRIER_TYPE BarrierTypeToEnum<D3D12_TEXTURE_BARRIER> = D3D12_BARRIER_TYPE_TEXTURE;

	template <>
	constexpr D3D12_BARRIER_TYPE BarrierTypeToEnum<D3D12_BUFFER_BARRIER> = D3D12_BARRIER_TYPE_BUFFER;

	export enum class RenderTargetSync
	{
		All = D3D12_BARRIER_SYNC_ALL,
		Draw = D3D12_BARRIER_SYNC_DRAW,
		RenderTarget = D3D12_BARRIER_SYNC_RENDER_TARGET,
	};

	export auto BarrierFlip(auto barrier)
	{
		std::swap(barrier.AccessBefore, barrier.AccessAfter);
		std::swap(barrier.SyncBefore, barrier.SyncAfter);

		if constexpr (std::same_as<decltype(barrier), D3D12_TEXTURE_BARRIER>)
			std::swap(barrier.LayoutBefore, barrier.LayoutAfter);

		return barrier;
	}

	struct BarrierNoAccess_t
	{
		D3D12_BARRIER_SYNC sync;

		template<class BarrierTy>
		BarrierTy operator()(BarrierTy barrier, BeforeTag)
		{
			barrier.AccessBefore = D3D12_BARRIER_ACCESS_NO_ACCESS;
			barrier.SyncBefore = sync;
			if constexpr (std::same_as<BarrierTy, D3D12_TEXTURE_BARRIER>)
				barrier.LayoutBefore = D3D12_BARRIER_LAYOUT_UNDEFINED;
			return barrier;
		}

		template<class BarrierTy>
		BarrierTy operator()(BarrierTy barrier, AfterTag)
		{
			barrier.AccessAfter = D3D12_BARRIER_ACCESS_NO_ACCESS;
			barrier.SyncAfter = sync;
			if constexpr (std::same_as<BarrierTy, D3D12_TEXTURE_BARRIER>)
				barrier.LayoutBefore = D3D12_BARRIER_LAYOUT_UNDEFINED;
			return barrier;
		}
	};

	export BarrierNoAccess_t BarrierAccessNone(D3D12_BARRIER_SYNC sync)
	{
		return { sync };
	}

	struct BarrierAccessCommon_t
	{
		D3D12_BARRIER_SYNC sync;

		template<class BarrierTy>
			requires std::same_as<BarrierTy, D3D12_GLOBAL_BARRIER> || std::same_as<BarrierTy, D3D12_BUFFER_BARRIER>
		BarrierTy operator()(BarrierTy barrier, BeforeTag)
		{
			barrier.AccessBefore = D3D12_BARRIER_ACCESS_COMMON;
			barrier.SyncBefore = sync;
			return barrier;
		}

		template<class BarrierTy>
			requires std::same_as<BarrierTy, D3D12_GLOBAL_BARRIER> || std::same_as<BarrierTy, D3D12_BUFFER_BARRIER>
		BarrierTy operator()(BarrierTy barrier, AfterTag)
		{
			barrier.AccessAfter = D3D12_BARRIER_ACCESS_COMMON;
			barrier.SyncAfter = sync;
			return barrier;
		}
	};

	struct BarrierAccessCommonTexture_t
	{
		D3D12_BARRIER_SYNC sync;
		D3D12_BARRIER_LAYOUT layout;

		D3D12_TEXTURE_BARRIER operator()(D3D12_TEXTURE_BARRIER barrier, BeforeTag)
		{
			barrier.AccessBefore = D3D12_BARRIER_ACCESS_COMMON;
			barrier.SyncBefore = sync;
			barrier.LayoutBefore = layout;
			return barrier;
		}

		D3D12_TEXTURE_BARRIER operator()(D3D12_TEXTURE_BARRIER barrier, AfterTag)
		{
			barrier.AccessAfter = D3D12_BARRIER_ACCESS_COMMON;
			barrier.SyncAfter = sync;
			barrier.LayoutAfter = layout;
			return barrier;
		}
	};

	export BarrierAccessCommon_t BarrierAccessCommon(D3D12_BARRIER_SYNC sync)
	{
		return { sync };
	}

	export BarrierAccessCommonTexture_t BarrierAccessCommon(D3D12_BARRIER_SYNC sync, D3D12_BARRIER_LAYOUT layout)
	{
		return { sync, layout };
	}

	struct BarrierAccessRenderTarget_t
	{
		RenderTargetSync sync;

		template<class BarrierTy, class TagTy>
		BarrierTy operator()(BarrierTy barrier, TagTy)
		{
			if constexpr (std::same_as<TagTy, BeforeTag>)
			{
				barrier.AccessBefore = D3D12_BARRIER_ACCESS_RENDER_TARGET;
				barrier.SyncBefore = static_cast<D3D12_BARRIER_SYNC>(sync);
				if constexpr (std::same_as<BarrierTy, D3D12_TEXTURE_BARRIER>)
					barrier.LayoutBefore = D3D12_BARRIER_LAYOUT_RENDER_TARGET;
			}
			else
			{
				barrier.AccessAfter = D3D12_BARRIER_ACCESS_RENDER_TARGET;
				barrier.SyncAfter = static_cast<D3D12_BARRIER_SYNC>(sync);
				if constexpr (std::same_as<BarrierTy, D3D12_TEXTURE_BARRIER>)
					barrier.LayoutAfter = D3D12_BARRIER_LAYOUT_RENDER_TARGET;
			}
			return barrier;
		}
	};

	export BarrierAccessRenderTarget_t BarrierAccessRenderTarget(RenderTargetSync sync)
	{
		return { sync };
	}

	export enum class VertexBufferSync
	{
		All = D3D12_BARRIER_SYNC_ALL,
		VertexShader = D3D12_BARRIER_SYNC_VERTEX_SHADING,
		Draw = D3D12_BARRIER_SYNC_DRAW,
		AllShading = D3D12_BARRIER_SYNC_ALL_SHADING
	};

	struct BarrierAccessVertexBuffer_t
	{
		VertexBufferSync sync;

		template<class TagTy>
		D3D12_TEXTURE_BARRIER operator()(D3D12_TEXTURE_BARRIER barrier, TagTy)
		{
			if constexpr (std::same_as<TagTy, BeforeTag>)
			{
				barrier.AccessBefore = D3D12_BARRIER_ACCESS_VERTEX_BUFFER;
				barrier.SyncBefore = static_cast<D3D12_BARRIER_SYNC>(sync);
			}
			else
			{
				barrier.AccessAfter = D3D12_BARRIER_ACCESS_VERTEX_BUFFER;
				barrier.SyncAfter = static_cast<D3D12_BARRIER_SYNC>(sync);
			}
			return barrier;
		}
	};

	export BarrierAccessVertexBuffer_t BarrierAccessVertexBuffer(VertexBufferSync sync)
	{
		return { sync };
	}

	export enum class ShaderResourceSync
	{
		All = D3D12_BARRIER_SYNC_ALL,
		Vertex = D3D12_BARRIER_SYNC_VERTEX_SHADING,
		Pixel = D3D12_BARRIER_SYNC_PIXEL_SHADING,
		Compute = D3D12_BARRIER_SYNC_COMPUTE_SHADING,
		Draw = D3D12_BARRIER_SYNC_DRAW,
		AllShading = D3D12_BARRIER_SYNC_ALL_SHADING,
	};

	struct BarrierAccessShaderResource_t
	{
		ShaderResourceSync sync;

		template<class BarrierTy, class TagTy>
		BarrierTy operator()(BarrierTy barrier, TagTy)
		{
			if constexpr (std::same_as<TagTy, BeforeTag>)
			{
				barrier.AccessBefore = D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
				barrier.SyncBefore = static_cast<D3D12_BARRIER_SYNC>(sync);
				if constexpr (std::same_as<BarrierTy, D3D12_TEXTURE_BARRIER>)
					barrier.LayoutBefore = D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
			}
			else
			{
				barrier.AccessAfter = D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
				barrier.SyncAfter = static_cast<D3D12_BARRIER_SYNC>(sync);
				if constexpr (std::same_as<BarrierTy, D3D12_TEXTURE_BARRIER>)
					barrier.LayoutAfter = D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
			}
			return barrier;
		}
	};

	export BarrierAccessShaderResource_t BarrierAccessShaderResource(ShaderResourceSync sync)
	{
		return { sync };
	};

	export enum class CopyResourceSync
	{
		All = D3D12_BARRIER_SYNC_ALL,
		Copy = D3D12_BARRIER_SYNC_COPY
	};

	struct BarrierAccessCopyDestination_t
	{
		CopyResourceSync sync;

		template<class BarrierTy, class TagTy>
		BarrierTy operator()(BarrierTy barrier, TagTy)
		{
			if constexpr (std::same_as<TagTy, BeforeTag>)
			{
				barrier.AccessBefore = D3D12_BARRIER_ACCESS_COPY_DEST;
				barrier.SyncBefore = static_cast<D3D12_BARRIER_SYNC>(sync);
				if constexpr (std::same_as<BarrierTy, D3D12_TEXTURE_BARRIER>)
					barrier.LayoutBefore = D3D12_BARRIER_LAYOUT_COPY_DEST;
			}
			else
			{
				barrier.AccessAfter = D3D12_BARRIER_ACCESS_COPY_DEST;
				barrier.SyncAfter = static_cast<D3D12_BARRIER_SYNC>(sync);
				if constexpr (std::same_as<BarrierTy, D3D12_TEXTURE_BARRIER>)
					barrier.LayoutAfter = D3D12_BARRIER_LAYOUT_COPY_DEST;
			}
			return barrier;
		}
	};

	export BarrierAccessCopyDestination_t BarrierAccessCopyDestination(CopyResourceSync sync)
	{
		return { sync };
	};

	struct BarrierAccessCopySource_t
	{
		CopyResourceSync sync;

		template<class BarrierTy, class TagTy>
		BarrierTy operator()(BarrierTy barrier, TagTy)
		{
			if constexpr (std::same_as<TagTy, BeforeTag>)
			{
				barrier.AccessBefore = D3D12_BARRIER_ACCESS_COPY_SOURCE;
				barrier.SyncBefore = static_cast<D3D12_BARRIER_SYNC>(sync);
				if constexpr (std::same_as<BarrierTy, D3D12_TEXTURE_BARRIER>)
					barrier.LayoutBefore = D3D12_BARRIER_LAYOUT_COPY_SOURCE;
			}
			else
			{
				barrier.AccessAfter = D3D12_BARRIER_ACCESS_COPY_SOURCE;
				barrier.SyncAfter = static_cast<D3D12_BARRIER_SYNC>(sync);
				if constexpr (std::same_as<BarrierTy, D3D12_TEXTURE_BARRIER>)
					barrier.LayoutAfter = D3D12_BARRIER_ACCESS_COPY_SOURCE;
			}
			return barrier;
		}
	};

	export BarrierAccessCopySource_t BarrierAccessCopySource(CopyResourceSync sync)
	{
		return { sync };
	};
}

namespace TypedD3D
{
	template<D3D12::BarrierEnabledType Ty>
	struct Trait<Untagged<Ty>>
	{
		using inner_type = Ty;
		using inner_tag = Ty;

		template<class NewInner>
		using trait_template = Untagged<NewInner>;

		template<class NewInner>
		using ReplaceInnerType = Untagged<NewInner>;

		template<class Derived>
		struct Interface : TypedStructInterfaceBase<Derived>
		{
			static Wrapper<Ty> Construct() requires std::same_as<Ty, D3D12_GLOBAL_BARRIER>
			{
				return {};
			}

			static Wrapper<Ty> Construct(WrapperView<ID3D12Resource> resource, D3D12_BARRIER_SUBRESOURCE_RANGE range = { 0, 1, 0, 1, 0, 1 }, D3D12_TEXTURE_BARRIER_FLAGS flags = D3D12_TEXTURE_BARRIER_FLAG_NONE) requires std::same_as<Ty, D3D12_TEXTURE_BARRIER>
			{
				return Ty
				{
					.pResource = resource.Get(),
					.Subresources = range,
					.Flags = flags
				};
			}

			static Wrapper<Ty> Construct(WrapperView<ID3D12Resource> resource) requires std::same_as<Ty, D3D12_BUFFER_BARRIER>
			{
				return Ty
				{
					.pResource = resource.Get(),
					.Offset = 0,
					.Size = resource->GetDesc().Width
				};
			}

			static Wrapper<Ty> Construct(WrapperView<ID3D12Resource> resource, UINT64 offset, UINT64 size) requires std::same_as<Ty, D3D12_BUFFER_BARRIER>
			{
				return Ty
				{
					.pResource = resource.Get(),
					.Offset = offset,
					.Size = size
				};
			}

			template<std::invocable<Ty, D3D12::BeforeTag> Func>
			Wrapper<Ty> Before(Func func)
			{
				return { func(Self(), D3D12::BeforeTag{}) };
			}

			template<std::invocable<Ty, D3D12::AfterTag> Func>
			Wrapper<Ty> After(Func func)
			{
				return { func(Self(), D3D12::AfterTag{}) };
			}

			Wrapper<Ty> Flip()
			{
				auto temp = Self();
				std::swap(temp.SyncBefore, temp.SyncAfter);
				std::swap(temp.AccessBefore, temp.AccessAfter);
				if constexpr (std::same_as<Ty, D3D12_TEXTURE_BARRIER>)
					std::swap(temp.LayoutBefore, temp.LayoutAfter);
				return temp;
			}

		private:
			using TypedStructInterfaceBase<Derived>::Self;
			using TypedStructInterfaceBase<Derived>::ToDerived;
		};
	};
}

namespace TypedD3D::D3D12
{
	export template<class... Ty>
		requires (BarrierEnabledType<InnerType<InnerType<Ty>>> && ...)
	std::array<D3D12_BARRIER_GROUP, sizeof...(Ty)> MakeBarrierGroup(std::span<Ty>... barriers)
	{
		std::array<D3D12_BARRIER_GROUP, sizeof...(Ty)> group;
		std::size_t i = 0;
		auto SetBarrierData = []<class Ty2>(D3D12_BARRIER_GROUP& group, std::span<Ty2> barrier)
		{
			if constexpr (D3D12::BarrierTypeToEnum<InnerType<InnerType<Ty2>>> == D3D12_BARRIER_TYPE_GLOBAL)
			{
				static_assert(sizeof(Ty2) == sizeof(D3D12_GLOBAL_BARRIER));
				group.pGlobalBarriers = std::bit_cast<const D3D12_GLOBAL_BARRIER*>(barrier.data());
			}
			else if constexpr (D3D12::BarrierTypeToEnum<InnerType<InnerType<Ty2>>> == D3D12_BARRIER_TYPE_TEXTURE)
			{
				static_assert(sizeof(Ty2) == sizeof(D3D12_TEXTURE_BARRIER));
				group.pTextureBarriers = std::bit_cast<const D3D12_TEXTURE_BARRIER*>(barrier.data());
			}
			else
			{
				static_assert(sizeof(Ty2) == sizeof(D3D12_BUFFER_BARRIER));
				group.pBufferBarriers = std::bit_cast<const D3D12_BUFFER_BARRIER*>(barrier.data());
			}
		};
		((group[i].NumBarriers = static_cast<UINT>(barriers.size()), group[i].Type = D3D12::BarrierTypeToEnum<InnerType<InnerType<Ty>>>, SetBarrierData(group[i], barriers), i++), ...);
		return group;
	}

	export template<class... Ty, std::size_t... N>
		requires (BarrierEnabledType<InnerType<InnerType<Ty>>> && ...)
	std::array<D3D12_BARRIER_GROUP, sizeof...(Ty)> MakeBarrierGroup(Span<Ty, N>... barriers)
	{
		std::array<D3D12_BARRIER_GROUP, sizeof...(Ty)> group;
		std::size_t i = 0;
		auto SetBarrierData = []<class Ty2, std::size_t N>(D3D12_BARRIER_GROUP& group, Span<Ty2, N> barrier)
		{
			if constexpr (D3D12::BarrierTypeToEnum<InnerType<InnerType<Ty2>>> == D3D12_BARRIER_TYPE_GLOBAL)
			{
				group.pGlobalBarriers = barrier.data();
			}
			else if constexpr (D3D12::BarrierTypeToEnum<InnerType<InnerType<Ty2>>> == D3D12_BARRIER_TYPE_TEXTURE)
			{
				group.pTextureBarriers = barrier.data();
			}
			else
			{
				group.pBufferBarriers = barrier.data();
			}
		};
		((group[i].NumBarriers = static_cast<UINT>(barriers.size()), group[i].Type = D3D12::BarrierTypeToEnum<InnerType<InnerType<Ty>>>, SetBarrierData(group[i], barriers), i++), ...);
		return group;
	}
}