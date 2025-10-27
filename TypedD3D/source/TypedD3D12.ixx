export module TypedD3D12;
export import TypedD3D.Shared;
export import :CommandList;
export import :CommandQueue;
export import :CommandAllocator;
export import :DescriptorHeap;
export import :PipelineState;
export import :Resource;
export import :Device;
export import :Wrappers;
export import :D3D12Object;

export namespace TypedD3D12 = TypedD3D::D3D12;

//TODO: Remove inheriting the base traits for CommandLists, CommandAllocators, CommandQueues, DescriptorHeaps in order to reduce the visible names when compiling fails. This will result in more duplication, but it's a price for better debuggability
//TODO: Finish migrating CommandLists API