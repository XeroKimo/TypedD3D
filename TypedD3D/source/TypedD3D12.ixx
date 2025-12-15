export module TypedD3D12;
export import TypedD3D.Shared;
export import :Wrappers;
export import :D3D12Object;
export import :CommandList;
export import :CommandQueue;
export import :CommandAllocator;
export import :DescriptorHeap;
export import :PipelineState;
export import :Resource;
export import :Barrier;
export import :Device;
export import :Extensions;

export namespace TypedD3D12 = TypedD3D::D3D12;

//TODO: Finish migrating CommandLists API