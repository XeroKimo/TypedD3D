# Requirements
- C++20
- VS2022 for now as I'm rather new to making libraries
- Git

In order to build, you must pull with submodules included.

# TypedD3D
TypedD3D is a C++ wrapper library for DirectX family related of APIs that is meant to provide stronger typing (D3D12), and a API the reflects more modern practices. This means removing out params, make non-optional pointer params be references etc. 

The aim is to put no extra memory overhead for each class and introduce little to no performance overhead to each function so it is as if you are writing raw DirectX code with extra type safety and modern practices. 

All wrapper classes whose class inherits from IUnknown is wrapped around a ComPtr and acts as closely as to one, meaning you can using .Get() to get the pointer to the underlying object, while operator->() has been overloaded to expose the libraries' modern API instead of the old.

# API Usage
The libraries' wrapper interface are exposed in a very simple way, by replacing ComPtr\<> with TypedD3D::Wrapper\<>. You might get some broken code as I delibrately don't expose operator&() and GetAddressOf() related functions as the library should work fine without them when updating your codebase. 

There are however, 3 ways to refer to the same class:  
- TypedD3D::Wrapper\<ID3D12Device> *(Suggested)*
- TypedD3D::D3D12::Device_t\<ID3D12Device> *(Maybe for template programming)*
- TypedD3D::D3D12::Device *(Old way, explicit to the library's names)*
  
Casting to more derived versions of a class becomes more familiar too with TypedD3D::Cast\<ID3D12Device5>(device).
  
## DX12 specific API
DX12 has extra wrapper interfaces to help tag their specific types. They are:

### Direct\<>
Compatible with any class that takes with D3D12_COMMAND_LIST_TYPE to initialize</br>
**Eg:** Direct\<ID3D12CommandList>

### Compute\<>
Compatible with any class that takes with D3D12_COMMAND_LIST_TYPE to initialize and ID3D12PipelineState</br>
**Eg:** Compute\<ID3D12PipelineState>

### Bundle\<>
Compatible ID3D12CommandList and it's derived classes and ID3D12CommandAllocator </br>
**Eg:** Bundle\<ID3D12CommandList>

### Copy\<>
Compatible with any class that takes with D3D12_COMMAND_LIST_TYPE to initialize </br>
**Eg:** Copy\<ID3D12CommandList>
### CBV_SRV_UAV\<>
Compatible with ID3D12DescriptorHeap, D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE </br>
**Eg:** CBV_SRV_UAV\<ID3D12DescriptorHeap>

### Sampler\<>
Compatible with ID3D12DescriptorHeap, D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE </br>
**Eg:** Sampler\<ID3D12DescriptorHeap>

### RTV\<>
Compatible with ID3D12DescriptorHeap, D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE </br>
**Eg:** RTV\<ID3D12DescriptorHeap>

### DSV\<>
Compatible with ID3D12DescriptorHeap, D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE </br>
**Eg:** DSV\<ID3D12DescriptorHeap>

### Graphics\<>
Compatible with ID3D12PipelineState </br>
**Eg:** Graphics\<ID3D12PipelineState>

### ShaderVisible\<>
Compatible with CBV_SRV_UAV and Sampler compatible classes </br>
**Eg:** ShaderVisible\<Sampler\<ID3D12DescriptorHeap>>

Various interfaces like the command lists and command queues only work when they have the same type, so I provide tagged wrappers for interfaces like those. Command lists also have their interfaces restricted based on what kind of command list was created based on [microsoft's document](https://docs.microsoft.com/en-us/windows/win32/direct3d12/recording-command-lists-and-bundles#command-list-api-restrictions), so they no longer have to constantly be referred which commands are available for what type of list.
  
## DX12 Implemented Types
- ID3D12Device up to ID3D12Device5
- ID3D12CommandList up to ID3D12GraphicsCommandList5
- ID3D12CommandAllocator
- ID3D12CommandQueue
- ID3D12DescriptorHeap*
- D3D12_CPU_DESCRIPTOR_HANDLE*
- D3D12_GPU_DESCRIPTOR_HANDLE*
- ID3D12PipelineState

## DX11 Implemented Types
- ID3D11Device
- ID3D11DeviceContext
- ID3D11Resource
- ID3D11Views*
- ID3D11States**
- ID3D11Shaders***

\*Covers all classes that inherit from ID3D11View include ID3D11View itself</br>
\*\*Covers all classes that has state in it's name</br>
\*\*\*Covers all classes that has shader in it's name</br>

## DXGI Implemented Types
- IDXGIFactory up to IDXGIFactory2
- IDXGISwapChain up to IDXGISwapChain3
- IDXGIAdapter
