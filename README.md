# TypedD3D
A bunch of functions and helpers classes for D3D12, and DXGI libraries

# Namespaces
## Helpers 
A bunch of helper functions that interface more with the raw APIs of the libraries, this could end up being more stand alone

## D3D12 
Contains a bunch of wrapper classes with the aim of just providing stronger types and a more modern C++ like interface. 
Various interfaces like the command lists and command queues only work when they have the same type, so I provide stronger typed wrapper classes for interfaces like those. Command lists also have their interfaces restricted based on what kind of command list was created based on [microsoft's document](https://docs.microsoft.com/en-us/windows/win32/direct3d12/recording-command-lists-and-bundles#command-list-api-restrictions), so they no longer have to constantly be referred which commands are available for what type of list.
All wrapper classes wrap around a ComPtr and is intended to work like a ComPtr, so manual reference counting is not required

# Requirements
C++20
