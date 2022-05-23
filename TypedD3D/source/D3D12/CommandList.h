//*********************************************************
//
// Copyright (c) 2022 Renzy Alarcon
// Licensed under the MIT License (MIT).
//
//*********************************************************
#pragma once
#include "CommandAllocator.h"
#include "DescriptorHeap.h"
#include "../Internal/D3D12/Meta.h"
#include "../Internal/ComWrapper.h"
#include "../Helpers/COMHelpers.h"
#include "../D3D12Wrappers.h"
#include <memory>
#include <d3d12.h>
#include <array>
#include <span>
#include <wrl/client.h>
#include <assert.h>
#include <optional>

#pragma warning (push)
#pragma warning (disable : 4584)

namespace TypedD3D::Internal
{
    namespace D3D12
    {
        template<class CommandListTy, TypeTag Type>
        using CommandList_t = InterfaceWrapper<CommandListTy, Type>;

        template<class CommandListTy>
        using RenderPass_t = InterfaceWrapper<CommandListTy, TypeTag::RenderPass>;

        namespace CommandList
        {
            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type, class ListTy>
            class PrivateInterface;

            //Can find all API restrictions here https://docs.microsoft.com/en-us/windows/win32/direct3d12/recording-command-lists-and-bundles#command-list-api-restrictions
            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type, class ListTy>
            class Interface;

            template<D3D12_COMMAND_LIST_TYPE Type, class ListTy>
            struct command_list_trait : Meta::command_list_type_tag<Type>
            {
                using list_value_type = ListTy;
                using allocator_value_type = TypedD3D::D3D12::CommandAllocator_t<Type>;

                template<class WrapperTy>
                using interface_type = Interface<WrapperTy, Type, ListTy>;
            };

            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
            class PrivateInterface<WrapperTy, Type, ID3D12GraphicsCommandList>
            {
            private:
                using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList>;
                using list_value_type = typename trait_value_type::list_value_type;
                using allocator_value_type = typename trait_value_type::allocator_value_type;
                using wrapper_type = WrapperTy;

            public:
                HRESULT Close()
                {
                    return InternalCommandList().Close();
                }

                HRESULT Reset(
                    allocator_value_type pAllocator,
                    ID3D12PipelineState* pInitialState)
                {
                    return InternalCommandList().Reset(pAllocator.Get(), pInitialState);
                }

                void ClearState(
                    ID3D12PipelineState* pPipelineState)
                {
                    InternalCommandList().ClearState(pPipelineState);
                }

                void DrawInstanced(
                    UINT VertexCountPerInstance,
                    UINT InstanceCount,
                    UINT StartVertexLocation,
                    UINT StartInstanceLocation)
                {
                    InternalCommandList().DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
                }

                void DrawIndexedInstanced(
                    UINT IndexCountPerInstance,
                    UINT InstanceCount,
                    UINT StartIndexLocation,
                    INT BaseVertexLocation,
                    UINT StartInstanceLocation)
                {
                    InternalCommandList().DrawIndexedInstanced(
                        IndexCountPerInstance,
                        InstanceCount,
                        StartIndexLocation,
                        BaseVertexLocation,
                        StartInstanceLocation);
                }

                void Dispatch(
                    UINT ThreadGroupCountX,
                    UINT ThreadGroupCountY,
                    UINT ThreadGroupCountZ)
                {
                    InternalCommandList().Dispatch(
                        ThreadGroupCountX,
                        ThreadGroupCountY,
                        ThreadGroupCountZ);
                }

                void CopyBufferRegion(
                    ID3D12Resource& pDstBuffer,
                    UINT64 DstOffset,
                    ID3D12Resource& pSrcBuffer,
                    UINT64 SrcOffset,
                    UINT64 NumBytes)
                {
                    InternalCommandList().CopyBufferRegion(
                        &pDstBuffer,
                        DstOffset,
                        &pSrcBuffer,
                        SrcOffset,
                        NumBytes);
                }

                void CopyTextureRegion(
                    const D3D12_TEXTURE_COPY_LOCATION& pDst,
                    UINT DstX,
                    UINT DstY,
                    UINT DstZ,
                    const D3D12_TEXTURE_COPY_LOCATION& pSrc,
                    const D3D12_BOX* pSrcBox)
                {
                    InternalCommandList().CopyTextureRegion(
                        &pDst,
                        DstX,
                        DstY,
                        DstZ,
                        &pSrc,
                        pSrcBox);
                }

                void CopyResource(
                    ID3D12Resource& pDstResource,
                    ID3D12Resource& pSrcResource)
                {
                    InternalCommandList().CopyResource(&pDstResource, &pSrcResource);
                }

                void CopyTiles(
                    ID3D12Resource& pTiledResource,
                    const D3D12_TILED_RESOURCE_COORDINATE& pTileRegionStartCoordinate,
                    const D3D12_TILE_REGION_SIZE& pTileRegionSize,
                    ID3D12Resource& pBuffer,
                    UINT64 BufferStartOffsetInBytes,
                    D3D12_TILE_COPY_FLAGS Flags)
                {
                    InternalCommandList().CopyTiles(
                        &pTiledResource,
                        &pTileRegionStartCoordinate,
                        &pTileRegionSize,
                        &pBuffer,
                        BufferStartOffsetInBytes,
                        Flags);
                }

                void ResolveSubresource(
                    ID3D12Resource& pDstResource,
                    UINT DstSubresource,
                    ID3D12Resource& pSrcResource,
                    UINT SrcSubresource,
                    DXGI_FORMAT Format)
                {
                    InternalCommandList().ResolveSubresource(
                        &pDstResource,
                        DstSubresource,
                        &pSrcResource,
                        SrcSubresource,
                        Format);
                }

                void IASetPrimitiveTopology(
                    D3D12_PRIMITIVE_TOPOLOGY PrimitiveTopology)
                {
                    InternalCommandList().IASetPrimitiveTopology(PrimitiveTopology);
                }

                void RSSetViewports(
                    std::span<D3D12_VIEWPORT> viewports)
                {
                    InternalCommandList().RSSetViewports(static_cast<UINT>(viewports.size()), viewports.data());
                }

                void RSSetScissorRects(
                    std::span<D3D12_RECT> rects)
                {
                    InternalCommandList().RSSetScissorRects(static_cast<UINT>(rects.size()), rects.data());
                }

                void OMSetBlendFactor(std::span<const float, 4> BlendFactor)
                {
                    InternalCommandList().OMSetBlendFactor(BlendFactor.data());
                }

                void OMSetStencilRef(UINT StencilRef)
                {
                    InternalCommandList().OMSetStencilRef(StencilRef);
                }

                void SetPipelineState(ID3D12PipelineState* pPipelineState)
                {
                    InternalCommandList().SetPipelineState(pPipelineState);
                }

                void ResourceBarrier(std::span<const D3D12_RESOURCE_BARRIER> Barriers)
                {
                    InternalCommandList().ResourceBarrier(static_cast<UINT>(Barriers.size()), Barriers.data());
                }

                void ExecuteBundle(InterfaceWrapper<ID3D12GraphicsCommandList, TypeTag::Bundle> pCommandList);

                void SetDescriptorHeaps(CBV_SRV_UAV<ID3D12DescriptorHeap> descriptorHeap)
                {
                    ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
                    InternalCommandList().SetDescriptorHeaps(1, heaps);
                }

                void SetDescriptorHeaps(Sampler<ID3D12DescriptorHeap> descriptorHeap)
                {
                    ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
                    InternalCommandList().SetDescriptorHeaps(1, heaps);
                }

                void SetDescriptorHeaps(CBV_SRV_UAV<ID3D12DescriptorHeap> cbv_srv_uavHeap, Sampler<ID3D12DescriptorHeap> samplerHeap)
                {
                    ID3D12DescriptorHeap* heaps[] = { cbv_srv_uavHeap.Get(), samplerHeap.Get() };
                    InternalCommandList().SetDescriptorHeaps(2, heaps);
                }

                void SetComputeRootSignature(ID3D12RootSignature* pRootSignature)
                {
                    InternalCommandList().SetComputeRootSignature(pRootSignature);
                }

                void SetGraphicsRootSignature(ID3D12RootSignature* pRootSignature)
                {
                    InternalCommandList().SetGraphicsRootSignature(pRootSignature);
                }

                void SetComputeRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
                {
                    InternalCommandList().SetComputeRootDescriptorTable(RootParameterIndex, BaseDescriptor);
                }

                void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
                {
                    InternalCommandList().SetGraphicsRootDescriptorTable(RootParameterIndex, BaseDescriptor);
                }

                void SetComputeRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
                {
                    InternalCommandList().SetComputeRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
                }

                void SetGraphicsRoot32BitConstant(UINT RootParameterIndex, UINT SrcData, UINT DestOffsetIn32BitValues)
                {
                    InternalCommandList().SetGraphicsRoot32BitConstant(RootParameterIndex, SrcData, DestOffsetIn32BitValues);
                }

                void SetComputeRoot32BitConstants(
                    UINT RootParameterIndex,
                    UINT Num32BitValuesToSet,
                    const void* pSrcData,
                    UINT DestOffsetIn32BitValues)
                {
                    InternalCommandList().SetComputeRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
                }

                void SetGraphicsRoot32BitConstants(
                    UINT RootParameterIndex,
                    UINT Num32BitValuesToSet,
                    const void* pSrcData,
                    UINT DestOffsetIn32BitValues)
                {
                    InternalCommandList().SetGraphicsRoot32BitConstants(RootParameterIndex, Num32BitValuesToSet, pSrcData, DestOffsetIn32BitValues);
                }

                void SetComputeRootConstantBufferView(
                    UINT RootParameterIndex,
                    D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                {
                    InternalCommandList().SetComputeRootConstantBufferView(RootParameterIndex, BufferLocation);
                }

                void SetGraphicsRootConstantBufferView(
                    UINT RootParameterIndex,
                    D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                {
                    InternalCommandList().SetGraphicsRootConstantBufferView(RootParameterIndex, BufferLocation);
                }

                void SetComputeRootShaderResourceView(
                    UINT RootParameterIndex,
                    D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                {
                    InternalCommandList().SetComputeRootShaderResourceView(RootParameterIndex, BufferLocation);
                }

                void SetGraphicsRootShaderResourceView(
                    UINT RootParameterIndex,
                    D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                {
                    InternalCommandList().SetGraphicsRootShaderResourceView(RootParameterIndex, BufferLocation);
                }

                void SetComputeRootUnorderedAccessView(
                    UINT RootParameterIndex,
                    D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                {
                    InternalCommandList().SetComputeRootUnorderedAccessView(RootParameterIndex, BufferLocation);
                }

                void SetGraphicsRootUnorderedAccessView(
                    UINT RootParameterIndex,
                    D3D12_GPU_VIRTUAL_ADDRESS BufferLocation)
                {
                    InternalCommandList().SetGraphicsRootUnorderedAccessView(RootParameterIndex, BufferLocation);
                }

                void IASetIndexBuffer(
                    const D3D12_INDEX_BUFFER_VIEW* pView)
                {
                    InternalCommandList().IASetIndexBuffer(pView);
                }

                void IASetVertexBuffers(
                    UINT StartSlot,
                    std::span<const D3D12_VERTEX_BUFFER_VIEW> views)
                {
                    InternalCommandList().IASetVertexBuffers(StartSlot, static_cast<UINT>(views.size()), views.data());
                }

                void SOSetTargets(
                    UINT StartSlot,
                    std::span<const D3D12_STREAM_OUTPUT_BUFFER_VIEW> views)
                {
                    InternalCommandList().SOSetTargets(StartSlot, static_cast<UINT>(views.size()), views.data());
                }

                void OMSetRenderTargets(
                    std::span<const RTV<D3D12_CPU_DESCRIPTOR_HANDLE>> pRenderTargetDescriptors,
                    BOOL RTsSingleHandleToDescriptorRange,
                    const DSV<D3D12_CPU_DESCRIPTOR_HANDLE>* pDepthStencilDescriptor)
                {
                    std::unique_ptr<D3D12_CPU_DESCRIPTOR_HANDLE[]> renderTargets = std::make_unique<D3D12_CPU_DESCRIPTOR_HANDLE[]>(pRenderTargetDescriptors.size());

                    for(size_t i = 0; i < pRenderTargetDescriptors.size(); i++)
                    {
                        renderTargets[i] = pRenderTargetDescriptors[i];
                    }

                    const D3D12_CPU_DESCRIPTOR_HANDLE* depthStencil = (pDepthStencilDescriptor) ? pDepthStencilDescriptor : nullptr;

                    InternalCommandList().OMSetRenderTargets(static_cast<UINT>(pRenderTargetDescriptors.size()), renderTargets.get(), RTsSingleHandleToDescriptorRange, depthStencil);
                }

                void ClearDepthStencilView(
                    DSV<D3D12_CPU_DESCRIPTOR_HANDLE> DepthStencilView,
                    D3D12_CLEAR_FLAGS ClearFlags,
                    FLOAT Depth,
                    UINT8 Stencil,
                    std::span<const D3D12_RECT> rects)
                {
                    InternalCommandList().ClearDepthStencilView(DepthStencilView, ClearFlags, Depth, Stencil, static_cast<UINT>(rects.size()), rects.data());
                }

                void ClearRenderTargetView(
                    RTV<D3D12_CPU_DESCRIPTOR_HANDLE> RenderTargetView,
                    std::span<const float, 4> colorRGBA,
                    std::span<const D3D12_RECT> rects)
                {
                    InternalCommandList().ClearRenderTargetView(RenderTargetView, colorRGBA.data(), static_cast<UINT>(rects.size()), rects.data());
                }

                void ClearUnorderedAccessViewUint(
                    CBV_SRV_UAV<D3D12_GPU_DESCRIPTOR_HANDLE> ViewGPUHandleInCurrentHeap,
                    CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE> ViewCPUHandle,
                    ID3D12Resource& pResource,
                    std::span<const UINT, 4> values,
                    std::span<const D3D12_RECT> rects)
                {
                    InternalCommandList().ClearUnorderedAccessViewUint(ViewGPUHandleInCurrentHeap, ViewCPUHandle, &pResource, values.data(), static_cast<UINT>(rects.size()), rects.data());
                }

                void ClearUnorderedAccessViewFloat(
                    CBV_SRV_UAV<D3D12_GPU_DESCRIPTOR_HANDLE> ViewGPUHandleInCurrentHeap,
                    CBV_SRV_UAV<D3D12_CPU_DESCRIPTOR_HANDLE> ViewCPUHandle,
                    ID3D12Resource& pResource,
                    std::span<const float, 4> values,
                    std::span<const D3D12_RECT> rects)
                {
                    InternalCommandList().ClearUnorderedAccessViewFloat(ViewGPUHandleInCurrentHeap, ViewCPUHandle, &pResource, values.data(), static_cast<UINT>(rects.size()), rects.data());
                }

                void DiscardResource(
                    ID3D12Resource& pResource,
                    const D3D12_DISCARD_REGION* pRegion)
                {
                    InternalCommandList().DiscardResource(&pResource, pRegion);
                }

                void BeginQuery(
                    ID3D12QueryHeap& pQueryHeap,
                    D3D12_QUERY_TYPE Type,
                    UINT Index)
                {
                    InternalCommandList().BeginQuery(&pQueryHeap, Type, Index);
                }

                void EndQuery(
                    ID3D12QueryHeap& pQueryHeap,
                    D3D12_QUERY_TYPE Type,
                    UINT Index)
                {
                    InternalCommandList().EndQuery(&pQueryHeap, Type, Index);
                }

                void ResolveQueryData(
                    ID3D12QueryHeap& pQueryHeap,
                    D3D12_QUERY_TYPE Type,
                    UINT StartIndex,
                    UINT NumQueries,
                    ID3D12Resource& pDestinationBuffer,
                    UINT64 AlignedDestinationBufferOffset)
                {
                    InternalCommandList().ResolveQueryData(&pQueryHeap, Type, StartIndex, NumQueries, &pDestinationBuffer, AlignedDestinationBufferOffset);
                }

                void SetPredication(
                    ID3D12Resource* pBuffer,
                    UINT64 AlignedBufferOffset,
                    D3D12_PREDICATION_OP Operation)
                {
                    InternalCommandList().SetPredication(pBuffer, AlignedBufferOffset, Operation);
                }

                void SetMarker(
                    UINT Metadata,
                    const void* pData,
                    UINT Size)
                {
                    InternalCommandList().SetMarker(Metadata, pData, Size);
                }

                void BeginEvent(
                    UINT Metadata,
                    const void* pData,
                    UINT Size)
                {
                    InternalCommandList().BeginEvent(Metadata, pData, Size);
                }

                void EndEvent()
                {
                    InternalCommandList().EndEvent();
                }

                void ExecuteIndirect(
                    ID3D12CommandSignature& commandSignature,
                    UINT maxCommandCount,
                    ID3D12Resource& argumentBuffer,
                    UINT64 argumentBufferOffset,
                    ID3D12Resource* optCountBuffer = nullptr,
                    UINT64 optCountBufferOffset = 0)
                {
                    InternalCommandList().ExecuteIndirect(&commandSignature, maxCommandCount, &argumentBuffer, argumentBufferOffset, optCountBuffer, optCountBufferOffset);
                }

                //In code order                                                          //Alphabetical order
                //using Base::Close;                                                     using Base::BeginEvent;
                //using Base::Reset;                                                     using Base::BeginQuery;
                //using Base::ClearState;                                                using Base::ClearDepthStencilView;
                //using Base::DrawInstanced;                                             using Base::ClearRenderTargetView;
                //using Base::DrawIndexedInstanced;                                      using Base::ClearState;
                //using Base::Dispatch;                                                  using Base::ClearUnorderedAccessViewFloat;
                //using Base::CopyBufferRegion;                                          using Base::ClearUnorderedAccessViewUint;
                //using Base::CopyTextureRegion;                                         using Base::Close;
                //using Base::CopyResource;                                              using Base::CopyBufferRegion;
                //using Base::CopyTiles;                                                 using Base::CopyResource;
                //using Base::ResolveSubresource;                                        using Base::CopyTextureRegion;
                //using Base::IASetPrimitiveTopology;                                    using Base::CopyTiles;
                //using Base::RSSetViewports;                                            using Base::DiscardResource;
                //using Base::RSSetScissorRects;                                         using Base::Dispatch;
                //using Base::OMSetBlendFactor;                                          using Base::DrawIndexedInstanced;
                //using Base::OMSetStencilRef;                                           using Base::DrawInstanced;
                //using Base::SetPipelineState;                                          using Base::EndEvent;
                //using Base::ResourceBarrier;                                           using Base::EndQuery;
                //using Base::ExecuteBundle;                                             using Base::ExecuteBundle;
                //using Base::SetDescriptorHeaps;                                        using Base::ExecuteIndirect;
                //using Base::SetComputeRootSignature;                                   using Base::IASetIndexBuffer;
                //using Base::SetGraphicsRootSignature;                                  using Base::IASetPrimitiveTopology;
                //using Base::SetComputeRootDescriptorTable;                             using Base::IASetVertexBuffers;
                //using Base::SetGraphicsRootDescriptorTable;                            using Base::OMSetBlendFactor;
                //using Base::SetComputeRoot32BitConstant;                               using Base::OMSetRenderTargets;
                //using Base::SetGraphicsRoot32BitConstant;                              using Base::OMSetStencilRef;
                //using Base::SetComputeRoot32BitConstants;                              using Base::Reset;
                //using Base::SetGraphicsRoot32BitConstants;                             using Base::ResolveQueryData;
                //using Base::SetComputeRootConstantBufferView;                          using Base::ResolveSubresource;
                //using Base::SetGraphicsRootConstantBufferView;                         using Base::ResourceBarrier;
                //using Base::SetComputeRootShaderResourceView;                          using Base::RSSetScissorRects;
                //using Base::SetGraphicsRootShaderResourceView;                         using Base::RSSetViewports;
                //using Base::SetComputeRootUnorderedAccessView;                         using Base::SetComputeRoot32BitConstant;
                //using Base::SetGraphicsRootUnorderedAccessView;                        using Base::SetComputeRoot32BitConstants;
                //using Base::IASetIndexBuffer;                                          using Base::SetComputeRootConstantBufferView;
                //using Base::IASetVertexBuffers;                                        using Base::SetComputeRootDescriptorTable;
                //using Base::SOSetTargets;                                              using Base::SetComputeRootShaderResourceView;
                //using Base::OMSetRenderTargets;                                        using Base::SetComputeRootSignature;
                //using Base::ClearDepthStencilView;                                     using Base::SetComputeRootUnorderedAccessView;
                //using Base::ClearRenderTargetView;                                     using Base::SetDescriptorHeaps;
                //using Base::ClearUnorderedAccessViewUint;                              using Base::SetGraphicsRoot32BitConstant;
                //using Base::ClearUnorderedAccessViewFloat;                             using Base::SetGraphicsRoot32BitConstants;
                //using Base::DiscardResource;                                           using Base::SetGraphicsRootConstantBufferView;
                //using Base::BeginQuery;                                                using Base::SetGraphicsRootDescriptorTable;
                //using Base::EndQuery;                                                  using Base::SetGraphicsRootShaderResourceView;
                //using Base::ResolveQueryData;                                          using Base::SetGraphicsRootSignature;
                //using Base::SetPredication;                                            using Base::SetGraphicsRootUnorderedAccessView;
                //using Base::SetMarker;                                                 using Base::SetMarker;
                //using Base::BeginEvent;                                                using Base::SetPipelineState;
                //using Base::EndEvent;                                                  using Base::SetPredication;
                //using Base::ExecuteIndirect;                                           using Base::SOSetTargets;

            private:
                list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
                using Base::Close;
                using Base::Dispatch;
                using Base::DrawIndexedInstanced;
                using Base::DrawInstanced;
                using Base::ExecuteIndirect;
                using Base::IASetIndexBuffer;
                using Base::IASetPrimitiveTopology;
                using Base::IASetVertexBuffers;
                using Base::OMSetBlendFactor;
                using Base::OMSetStencilRef;
                using Base::Reset;
                using Base::SetComputeRoot32BitConstant;
                using Base::SetComputeRoot32BitConstants;
                using Base::SetComputeRootConstantBufferView;
                using Base::SetComputeRootDescriptorTable;
                using Base::SetComputeRootShaderResourceView;
                using Base::SetComputeRootSignature;
                using Base::SetComputeRootUnorderedAccessView;
                using Base::SetDescriptorHeaps;
                using Base::SetGraphicsRoot32BitConstant;
                using Base::SetGraphicsRoot32BitConstants;
                using Base::SetGraphicsRootConstantBufferView;
                using Base::SetGraphicsRootDescriptorTable;
                using Base::SetGraphicsRootShaderResourceView;
                using Base::SetGraphicsRootSignature;
                using Base::SetGraphicsRootUnorderedAccessView;
                using Base::SetMarker;
                using Base::SetPipelineState;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
                using Base::Close;
                using Base::CopyBufferRegion;
                using Base::CopyResource;
                using Base::CopyTextureRegion;
                using Base::CopyTiles;
                using Base::DiscardResource;
                using Base::Dispatch;
                using Base::DrawIndexedInstanced;
                using Base::DrawInstanced;
                using Base::EndQuery;
                using Base::Reset;
                using Base::ResolveQueryData;
                using Base::ResourceBarrier;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;
            public:
                using Base::ClearState;
                using Base::ClearUnorderedAccessViewFloat;
                using Base::ClearUnorderedAccessViewUint;
                using Base::Close;
                using Base::CopyBufferRegion;
                using Base::CopyResource;
                using Base::CopyTextureRegion;
                using Base::CopyTiles;
                using Base::DiscardResource;
                using Base::Dispatch;
                using Base::EndQuery;
                using Base::ExecuteIndirect;
                using Base::Reset;
                using Base::ResolveQueryData;
                using Base::ResourceBarrier;
                using Base::RSSetScissorRects;
                using Base::RSSetViewports;
                using Base::SetComputeRoot32BitConstant;
                using Base::SetComputeRoot32BitConstants;
                using Base::SetComputeRootConstantBufferView;
                using Base::SetComputeRootDescriptorTable;
                using Base::SetComputeRootShaderResourceView;
                using Base::SetComputeRootSignature;
                using Base::SetComputeRootUnorderedAccessView;
                using Base::SetDescriptorHeaps;
                using Base::SetPipelineState;
                using Base::SetPredication;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList> :
                public PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>;
            };



            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
            class PrivateInterface<WrapperTy, Type, ID3D12GraphicsCommandList1>
            {
            private:
                using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList1>;
                using list_value_type = typename trait_value_type::list_value_type;
                using allocator_value_type = typename trait_value_type::allocator_value_type;
                using wrapper_type = WrapperTy;

            public:
                void AtomicCopyBufferUINT(
                    ID3D12Resource& pDstBuffer,
                    UINT64 DstOffset,
                    ID3D12Resource& pSrcBuffer,
                    UINT64 SrcOffset,
                    UINT Dependencies,
                    ID3D12Resource* const* ppDependentResources,
                    const D3D12_SUBRESOURCE_RANGE_UINT64* pDependentSubresourceRanges)
                {
                    InternalCommandList().AtomicCopyBufferUINT(
                        &pDstBuffer,
                        DstOffset,
                        &pSrcBuffer,
                        SrcOffset,
                        Dependencies,
                        ppDependentResources,
                        pDependentSubresourceRanges);
                }

                void AtomicCopyBufferUINT64(
                    ID3D12Resource& pDstBuffer,
                    UINT64 DstOffset,
                    ID3D12Resource& pSrcBuffer,
                    UINT64 SrcOffset,
                    UINT Dependencies,
                    ID3D12Resource* const* ppDependentResources,
                    const D3D12_SUBRESOURCE_RANGE_UINT64* pDependentSubresourceRanges)
                {
                    InternalCommandList().AtomicCopyBufferUINT64(
                        &pDstBuffer,
                        DstOffset,
                        &pSrcBuffer,
                        SrcOffset,
                        Dependencies,
                        ppDependentResources,
                        pDependentSubresourceRanges);
                }

                void OMSetDepthBounds(
                    FLOAT Min,
                    FLOAT Max)
                {
                    InternalCommandList().OMSetDepthBounds(Min, Max);
                }

                void SetSamplePositions(
                    UINT NumSamplesPerPixel,
                    UINT NumPixels,
                    D3D12_SAMPLE_POSITION* pSamplePositions)
                {
                    InternalCommandList().SetSamplePositions(
                        NumSamplesPerPixel,
                        NumPixels,
                        pSamplePositions);
                }

                void ResolveSubresourceRegion(
                    ID3D12Resource& pDstResource,
                    UINT DstSubresource,
                    UINT DstX,
                    UINT DstY,
                    ID3D12Resource& pSrcResource,
                    UINT SrcSubresource,
                    D3D12_RECT* pSrcRect,
                    DXGI_FORMAT Format,
                    D3D12_RESOLVE_MODE ResolveMode)
                {
                    InternalCommandList().ResolveSubresourceRegion(
                        &pDstResource,
                        DstSubresource,
                        DstX,
                        DstY,
                        &pSrcResource,
                        SrcSubresource,
                        pSrcRect,
                        Format,
                        ResolveMode
                    );
                }

                void SetViewInstanceMask(
                    UINT Mask)
                {
                    InternalCommandList().SetViewInstanceMask(Mask);
                }


                //In code order                                                          //Alphabetical order
                //using Base::AtomicCopyBufferUINT;                                      using Base::AtomicCopyBufferUINT;
                //using Base::AtomicCopyBufferUINT64;                                    using Base::AtomicCopyBufferUINT64;
                //using Base::OMSetDepthBounds;                                          using Base::OMSetDepthBounds;
                //using Base::SetSamplePositions;                                        using Base::ResolveSubresourceRegion;
                //using Base::ResolveSubresourceRegion;                                  using Base::SetSamplePositions;
                //using Base::SetViewInstanceMask;                                       using Base::SetViewInstanceMask;     
            private:
                list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList1> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList1>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList1>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
                using Base::OMSetDepthBounds;
                using Base::SetSamplePositions;
                using Base::SetViewInstanceMask;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList1> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList1>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList1>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
                using Base::AtomicCopyBufferUINT;
                using Base::AtomicCopyBufferUINT64;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList1> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList1>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList1>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;
            public:
                using Base::AtomicCopyBufferUINT;
                using Base::AtomicCopyBufferUINT64;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1> :
                public PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1>;
            };



            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
            class PrivateInterface<WrapperTy, Type, ID3D12GraphicsCommandList2>
            {
            private:
                using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList2>;
                using list_value_type = typename trait_value_type::list_value_type;
                using allocator_value_type = typename trait_value_type::allocator_value_type;
                using wrapper_type = WrapperTy;

            public:
                void WriteBufferImmediate(
                    std::span<D3D12_WRITEBUFFERIMMEDIATE_PARAMETER> pParams,
                    std::span<D3D12_WRITEBUFFERIMMEDIATE_MODE> pModes = {})
                {
                    assert(pParams.size() == pModes.size() || pModes.size() == 0);
                    InternalCommandList().WriteBufferImmediate(static_cast<UINT>(pParams.size()), pParams.data(), pModes.data());
                }


                //In code order                                                          //Alphabetical order
                //using Base::WriteBufferImmediate;                                      using Base::WriteBufferImmediate;
            private:
                list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList2> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList2>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList1>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList2>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
                using Base::WriteBufferImmediate;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList2> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList2>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList1>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList2>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
                using Base::WriteBufferImmediate;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList2> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList2>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList1>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList2>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;
            public:
                using Base::WriteBufferImmediate;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2> :
                public PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2>;
            };



            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
            class PrivateInterface<WrapperTy, Type, ID3D12GraphicsCommandList3>
            {
            private:
                using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList3>;
                using list_value_type = typename trait_value_type::list_value_type;
                using allocator_value_type = typename trait_value_type::allocator_value_type;
                using wrapper_type = WrapperTy;

            public:
                void SetProtectedResourceSession(
                    ID3D12ProtectedResourceSession* pProtectedResourceSession)
                {
                    InternalCommandList().SetProtectedResourceSession(pProtectedResourceSession);
                }


                //In code order                                                          //Alphabetical order
                //using Base::SetProtectedResourceSession;                               using Base::SetProtectedResourceSession;
            private:
                list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList3> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList3>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList2>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList3>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList3> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList3>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList2>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList3>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
                using Base::SetProtectedResourceSession;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList3> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList3>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList2>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList3>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;
            public:
                using Base::SetProtectedResourceSession;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3> :
                public PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3>;
            };



            template<class WrapperTy, class ListTy>
            class RenderPassInterface;

            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
            class PrivateInterface<WrapperTy, Type, ID3D12GraphicsCommandList4>
            {
            private:
                using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList4>;
                using list_value_type = typename trait_value_type::list_value_type;
                using allocator_value_type = typename trait_value_type::allocator_value_type;
                using wrapper_type = WrapperTy;

            public:
                RenderPass_t<ID3D12GraphicsCommandList4> BeginRenderPass(
                    std::span<const D3D12_RENDER_PASS_RENDER_TARGET_DESC> renderTargets,
                    const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* pDepthStencil,
                    D3D12_RENDER_PASS_FLAGS Flags);

                void EndRenderPass()
                {
                    InternalCommandList().EndRenderPass();
                }

                void InitializeMetaCommand(
                    ID3D12MetaCommand& pMetaCommand,
                    const void* pInitializationParametersData,
                    SIZE_T InitializationParametersDataSizeInBytes)
                {
                    InternalCommandList().InitializeMetaCommand(&pMetaCommand, pInitializationParametersData, InitializationParametersDataSizeInBytes);
                }

                void InitializeMetaCommand(
                    ID3D12MetaCommand& pMetaCommand)
                {
                    InitializeMetaCommand(pMetaCommand, nullptr, 0);
                }

                template<class T>
                void InitializeMetaCommand(
                    ID3D12MetaCommand& pMetaCommand,
                    const T& pInitializationParametersData)
                {
                    InitializeMetaCommand(pMetaCommand, &pInitializationParametersData, sizeof(T));
                }

                void ExecuteMetaCommand(
                    ID3D12MetaCommand& pMetaCommand,
                    const void* pExecutionParametersData,
                    SIZE_T ExecutionParametersDataSizeInBytes)
                {
                    InternalCommandList().ExecuteMetaCommand(&pMetaCommand, pExecutionParametersData, ExecutionParametersDataSizeInBytes);
                }

                void ExecuteMetaCommand(
                    ID3D12MetaCommand& pMetaCommand)
                {
                    ExecuteMetaCommand(pMetaCommand, nullptr, 0);
                }

                template<class T>
                void ExecuteMetaCommand(
                    ID3D12MetaCommand& pMetaCommand,
                    const T& pExecutionParametersData)
                {
                    ExecuteMetaCommand(pMetaCommand, &pExecutionParametersData, sizeof(T));
                }

                void BuildRaytracingAccelerationStructure(
                    const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& pDesc,
                    std::span<const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC> pPostbuildInfoDescs)
                {
                    InternalCommandList().BuildRaytracingAccelerationStructure(
                        &pDesc,
                        static_cast<UINT>(pPostbuildInfoDescs.size()),
                        pPostbuildInfoDescs.data());
                }

                void EmitRaytracingAccelerationStructurePostbuildInfo(
                    const D3D12_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO_DESC& pDesc,
                    std::span<const D3D12_GPU_VIRTUAL_ADDRESS> pSourceAccelerationStructureData)
                {
                    InternalCommandList().EmitRaytracingAccelerationStructurePostbuildInfo(
                        &pDesc,
                        static_cast<UINT>(pSourceAccelerationStructureData.size()),
                        pSourceAccelerationStructureData.data());
                }

                void CopyRaytracingAccelerationStructure(
                    D3D12_GPU_VIRTUAL_ADDRESS DestAccelerationStructureData,
                    D3D12_GPU_VIRTUAL_ADDRESS SourceAccelerationStructureData,
                    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_COPY_MODE Mode)
                {
                    InternalCommandList().CopyRaytracingAccelerationStructure(
                        DestAccelerationStructureData,
                        SourceAccelerationStructureData,
                        Mode);
                }

                void SetPipelineState1(
                    ID3D12StateObject* pStateObject)
                {
                    InternalCommandList().SetPipelineState1(pStateObject);
                }

                void DispatchRays(
                    const D3D12_DISPATCH_RAYS_DESC& pDesc)
                {
                    InternalCommandList().DispatchRays(&pDesc);
                }


                //In code order                                                          //Alphabetical order
                //using Base::BeginRenderPass;                                           using Base::BeginRenderPass;
                //using Base::EndRenderPass;                                             using Base::BuildRaytracingAccelerationStructure;
                //using Base::InitializeMetaCommand;                                     using Base::CopyRaytracingAccelerationStructure;
                //using Base::ExecuteMetaCommand;                                        using Base::DispatchRays;
                //using Base::BuildRaytracingAccelerationStructure;                      using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
                //using Base::EmitRaytracingAccelerationStructurePostbuildInfo;          using Base::EndRenderPass;
                //using Base::CopyRaytracingAccelerationStructure;                       using Base::ExecuteMetaCommand;
                //using Base::SetPipelineState1;                                         using Base::InitializeMetaCommand;
                //using Base::DispatchRays;                                              using Base::SetPipelineState1;
            private:
                list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList4> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList4>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList3>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList4>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
                using Base::DispatchRays;
                using Base::SetPipelineState1;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList4> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList4>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList3>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList4>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList4> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList4>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList3>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList4>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;
            public:
                using Base::BuildRaytracingAccelerationStructure;
                using Base::CopyRaytracingAccelerationStructure;
                using Base::DispatchRays;
                using Base::EmitRaytracingAccelerationStructurePostbuildInfo;
                using Base::ExecuteMetaCommand;
                using Base::InitializeMetaCommand;
                using Base::SetPipelineState1;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4> :
                public PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4>;
            };

            template<class WrapperTy>
            class RenderPassInterface<WrapperTy, ID3D12GraphicsCommandList4> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>,
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1>,
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2>,
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3>,
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4>
            {
                template<class WrapperTy, D3D12_COMMAND_LIST_TYPE type, class ListTy>
                friend class PrivateInterface;

                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList>;
                using Base1 = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList1>;
                using Base2 = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList2>;
                using Base3 = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList3>;
                using Base4 = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4>;

            public:
                using Base::BeginQuery;
                using Base::Close;
                using Base::DrawIndexedInstanced;
                using Base::DrawInstanced;
                using Base::EndEvent;
                using Base::EndQuery;
                using Base::ExecuteBundle;
                using Base::ExecuteIndirect;
                using Base::IASetIndexBuffer;
                using Base::IASetPrimitiveTopology;
                using Base::IASetVertexBuffers;
                using Base::OMSetBlendFactor;
                using Base::OMSetStencilRef;
                using Base::Reset;
                using Base::ResourceBarrier;
                using Base::RSSetScissorRects;
                using Base::RSSetViewports;
                using Base::SetComputeRoot32BitConstant;
                using Base::SetComputeRoot32BitConstants;
                using Base::SetComputeRootConstantBufferView;
                using Base::SetComputeRootDescriptorTable;
                using Base::SetComputeRootShaderResourceView;
                using Base::SetComputeRootSignature;
                using Base::SetComputeRootUnorderedAccessView;
                using Base::SetDescriptorHeaps;
                using Base::SetGraphicsRoot32BitConstant;
                using Base::SetGraphicsRoot32BitConstants;
                using Base::SetGraphicsRootConstantBufferView;
                using Base::SetGraphicsRootDescriptorTable;
                using Base::SetGraphicsRootShaderResourceView;
                using Base::SetGraphicsRootSignature;
                using Base::SetGraphicsRootUnorderedAccessView;
                using Base::SetMarker;
                using Base::SetPipelineState;
                using Base::SetPredication;
                using Base::SOSetTargets;

                using Base1::OMSetDepthBounds;
                using Base1::SetSamplePositions;
                using Base1::SetViewInstanceMask;

                using Base2::WriteBufferImmediate;

                using Base4::EndRenderPass;
            };



            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
            class PrivateInterface<WrapperTy, Type, ID3D12GraphicsCommandList5>
            {
            private:
                using trait_value_type = command_list_trait<Type, ID3D12GraphicsCommandList5>;
                using list_value_type = typename trait_value_type::list_value_type;
                using allocator_value_type = typename trait_value_type::allocator_value_type;
                using wrapper_type = WrapperTy;

            public:
                void RSSetShadingRate(
                    D3D12_SHADING_RATE baseShadingRate,
                    const D3D12_SHADING_RATE_COMBINER* combiners)
                {
                    InternalCommandList().RSSetShadingRate(
                        baseShadingRate,
                        combiners);
                }

                void RSSetShadingRateImage(
                    ID3D12Resource* shadingRateImage)
                {
                    InternalCommandList().RSSetShadingRateImage(
                        shadingRateImage);
                }

            public:

                //In code order                                                          //Alphabetical order
                //using Base::RSSetShadingRate;                                          using Base::RSSetShadingRate;
                //using Base::RSSetShadingRateImage;                                     using Base::RSSetShadingRateImage;
            private:
                list_value_type& InternalCommandList() { return *static_cast<wrapper_type&>(*this).Get(); }
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList5> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList5>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList4>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_BUNDLE, ID3D12GraphicsCommandList5>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
                using Base::RSSetShadingRate;
                using Base::RSSetShadingRateImage;
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList5> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList5>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList4>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COPY, ID3D12GraphicsCommandList5>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;

            public:
            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList5> :
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList5>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList4>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_COMPUTE, ID3D12GraphicsCommandList5>;

                //Enables casting to WrapperTy as WrapperTy would not know it inherits from PrivateInterface
                friend Base;
            public:

            };

            template<class WrapperTy>
            class Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList5> :
                public PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList5>,
                public Interface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList4>
            {
                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList5>;
            };

            template<class WrapperTy>
            class RenderPassInterface<WrapperTy, ID3D12GraphicsCommandList5> :
                public RenderPassInterface<WrapperTy, ID3D12GraphicsCommandList4>,
                private PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList5>
            {
                template<class WrapperTy, D3D12_COMMAND_LIST_TYPE type, class ListTy>
                friend class PrivateInterface;

                using Base = PrivateInterface<WrapperTy, D3D12_COMMAND_LIST_TYPE_DIRECT, ID3D12GraphicsCommandList5>;
            public:
                using Base::RSSetShadingRate;
                using Base::RSSetShadingRateImage;
            };
        }
    }

    template<class DirectXClass, TypeTag Type>
        requires std::is_base_of_v<ID3D12GraphicsCommandList, DirectXClass> && Is_Command_List_Type<Type>
    class InterfaceWrapper<DirectXClass, Type> : public ComWrapper<DirectXClass>, private D3D12::CommandList::Interface<InterfaceWrapper<DirectXClass, Type>, listType<Type>, DirectXClass>
    {
    private:
        using Interface = D3D12::CommandList::Interface<InterfaceWrapper<DirectXClass, Type>, listType<Type>, DirectXClass>;
        friend Interface;

        template<class WrapperT, D3D12_COMMAND_LIST_TYPE Type2, class List>
        friend class D3D12::CommandList::PrivateInterface;

    public:
        using trait_value_type = D3D12::CommandList::command_list_trait<listType<Type>, DirectXClass>;
        using list_value_type = typename trait_value_type::list_value_type;
        using allocator_value_type = typename trait_value_type::allocator_value_type;
        static constexpr D3D12_COMMAND_LIST_TYPE value = trait_value_type::value;

    public:
        static constexpr TypeTag tag_value = Type;
        using underlying_type = DirectXClass;

    public:
        using ComWrapper<DirectXClass>::ComWrapper;

        template<class DerivedListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList, DerivedListTy>
        InterfaceWrapper(const InterfaceWrapper<DerivedListTy, Type>& other) :
            ComWrapper<DirectXClass>::ComWrapper(other.GetComPtr())
        {

        }

        template<class OtherListTy>
            requires (tag_value == TypeTag::RenderPass) && std::is_base_of_v<ID3D12GraphicsCommandList4, OtherListTy>
        explicit InterfaceWrapper(const InterfaceWrapper<OtherListTy, TypeTag::RenderPass>& other) :
            ComWrapper<DirectXClass>::ComWrapper(other.GetComPtr())
        {

        }

        template<class DerivedListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList, DerivedListTy>
        InterfaceWrapper& operator=(const InterfaceWrapper<DerivedListTy, Type>& other) 
        { 
            ComWrapper<DirectXClass>::operator=(other);
            return *this;
        }

        template<class OtherListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList4, OtherListTy>
        InterfaceWrapper& operator=(const InterfaceWrapper<OtherListTy, TypeTag::RenderPass>& other)
        { 
            ComWrapper<DirectXClass>::operator=(other);
            return *this;
        }

        template<class DerivedListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList, DerivedListTy>
        InterfaceWrapper& operator=(InterfaceWrapper<DerivedListTy, Type>&& other) noexcept
        { 
            ComWrapper<DirectXClass>::operator=(std::move(other));
            return *this;
        }

        template<class OtherListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList4, OtherListTy>
        InterfaceWrapper& operator=(InterfaceWrapper<OtherListTy, TypeTag::RenderPass>&& other) noexcept
        { 
            ComWrapper<DirectXClass>::operator=(std::move(other));
            return *this;
        }
    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };

    template<class DirectXClass>
        requires std::is_base_of_v<ID3D12GraphicsCommandList4, DirectXClass>
    class InterfaceWrapper<DirectXClass, TypeTag::RenderPass> : public ComWrapper<DirectXClass>, private D3D12::CommandList::RenderPassInterface<InterfaceWrapper<DirectXClass, TypeTag::RenderPass>, DirectXClass>
    {
    private:
        using Interface = D3D12::CommandList::RenderPassInterface<InterfaceWrapper<DirectXClass, TypeTag::RenderPass>, DirectXClass>;
        friend Interface;

        template<class WrapperT, D3D12_COMMAND_LIST_TYPE Type2, class List>
        friend class D3D12::CommandList::PrivateInterface;

    public:
        using trait_value_type = D3D12::CommandList::command_list_trait<D3D12_COMMAND_LIST_TYPE_DIRECT, DirectXClass>;
        using list_value_type = typename trait_value_type::list_value_type;
        using allocator_value_type = typename trait_value_type::allocator_value_type;
        static constexpr D3D12_COMMAND_LIST_TYPE value = trait_value_type::value;

    public:
        static constexpr TypeTag tag_value = TypeTag::RenderPass;
        using underlying_type = DirectXClass;

    public:
        using ComWrapper<DirectXClass>::ComWrapper;

        template<class OtherListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList4, OtherListTy>
        explicit InterfaceWrapper(const InterfaceWrapper<OtherListTy, TypeTag::Direct>& other) :
            ComWrapper<DirectXClass>::ComWrapper(other.GetComPtr())
        {

        }

        template<class DerivedListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList4, DerivedListTy>
        InterfaceWrapper(const InterfaceWrapper<DerivedListTy, TypeTag::RenderPass>& other) :
            ComWrapper<DirectXClass>::ComWrapper(other.GetComPtr())
        {

        }

        template<class DerivedListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList4, DerivedListTy>
        InterfaceWrapper& operator=(const InterfaceWrapper<DerivedListTy, TypeTag::Direct>& other)
        {
            ComWrapper<DirectXClass>::operator=(other);
            return *this;
        }

        template<class OtherListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList4, OtherListTy>
        InterfaceWrapper& operator=(const InterfaceWrapper<OtherListTy, TypeTag::RenderPass>& other)
        {
            ComWrapper<DirectXClass>::operator=(other);
            return *this;
        }

        template<class DerivedListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList, DerivedListTy>
        InterfaceWrapper& operator=(InterfaceWrapper<DerivedListTy, TypeTag::Direct>&& other) noexcept
        {
            ComWrapper<DirectXClass>::operator=(std::move(other));
            return *this;
        }

        template<class OtherListTy>
            requires std::is_base_of_v<ID3D12GraphicsCommandList4, OtherListTy>
        InterfaceWrapper& operator=(InterfaceWrapper<OtherListTy, TypeTag::RenderPass>&& other) noexcept
        {
            ComWrapper<DirectXClass>::operator=(std::move(other));
            return *this;
        }
    public:
        Interface* GetInterface() { return this; }
        Interface* operator->() { return this; }
    };

    template<class DirectXClass, TypeTag Type>
        requires std::same_as<ID3D12CommandList, DirectXClass> && Is_Command_List_Type<Type> && (Type != TypeTag::Bundle)
    class InterfaceWrapper<DirectXClass, Type> : public ComWrapper<DirectXClass>
    {
    public:
        using trait_value_type = D3D12::CommandList::command_list_trait<listType<Type>, DirectXClass>;
        using list_value_type = typename trait_value_type::list_value_type;
        using allocator_value_type = typename trait_value_type::allocator_value_type;
        static constexpr D3D12_COMMAND_LIST_TYPE value = trait_value_type::value;

    public:
        static constexpr TypeTag tag_value = Type;
        using underlying_type = DirectXClass;

    public:
        using ComWrapper<DirectXClass>::ComWrapper;

        template<class DerivedListTy>
            requires std::is_base_of_v<ID3D12CommandList, DirectXClass>
        InterfaceWrapper(const InterfaceWrapper<DerivedListTy, Type>& other) :
            ComWrapper<DirectXClass>::ComWrapper(other.GetComPtr())
        {

        }
    };

    namespace D3D12
    {
        namespace CommandList
        {
            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
            void PrivateInterface<WrapperTy, Type, ID3D12GraphicsCommandList>::ExecuteBundle(InterfaceWrapper<ID3D12GraphicsCommandList, TypeTag::Bundle> pCommandList)
            {
                InternalCommandList().ExecuteBundle(pCommandList.Get());
            }


            template<class WrapperTy, D3D12_COMMAND_LIST_TYPE Type>
            RenderPass_t<ID3D12GraphicsCommandList4> PrivateInterface<WrapperTy, Type, ID3D12GraphicsCommandList4>::BeginRenderPass(std::span<const D3D12_RENDER_PASS_RENDER_TARGET_DESC> renderTargets, const D3D12_RENDER_PASS_DEPTH_STENCIL_DESC* pDepthStencil, D3D12_RENDER_PASS_FLAGS Flags)
            {
                InternalCommandList().BeginRenderPass(static_cast<UINT>(renderTargets.size()), renderTargets.data(), pDepthStencil, Flags);
                return RenderPass<ID3D12GraphicsCommandList4>(static_cast<wrapper_type&>(*this).GetComPtr());
            }
        }
    }
}

namespace TypedD3D::D3D12
{
    using Microsoft::WRL::ComPtr;

    template<class ListTy, D3D12_COMMAND_LIST_TYPE Type>
    using CommandList_t = TypedD3D::Internal::D3D12::CommandList_t<ListTy, TypedD3D::Internal::tagValue<Type>>;

    template<class ListTy>
    using RenderPass_t = TypedD3D::Internal::D3D12::RenderPass_t<ListTy>;

    using RenderPass4 = RenderPass_t<ID3D12GraphicsCommandList4>;
    using RenderPass5 = RenderPass_t<ID3D12GraphicsCommandList5>;
    namespace CommandList
    {
        template<class ListTy>
        using Direct_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_DIRECT>;

        using DirectBase = Direct_t<ID3D12CommandList>;
        using Direct = Direct_t<ID3D12GraphicsCommandList>;
        using Direct1 = Direct_t<ID3D12GraphicsCommandList1>;
        using Direct2 = Direct_t<ID3D12GraphicsCommandList2>;
        using Direct3 = Direct_t<ID3D12GraphicsCommandList3>;
        using Direct4 = Direct_t<ID3D12GraphicsCommandList4>;
        using Direct5 = Direct_t<ID3D12GraphicsCommandList5>;
        //using Direct6 = Internal::Direct<ID3D12GraphicsCommandList6>;

        template<class ListTy>
        using Bundle_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_BUNDLE>;

        using Bundle = Bundle_t<ID3D12GraphicsCommandList>;
        using Bundle1 = Bundle_t<ID3D12GraphicsCommandList1>;
        using Bundle2 = Bundle_t<ID3D12GraphicsCommandList2>;
        using Bundle3 = Bundle_t<ID3D12GraphicsCommandList3>;
        using Bundle4 = Bundle_t<ID3D12GraphicsCommandList4>;
        using Bundle5 = Bundle_t<ID3D12GraphicsCommandList5>;
        //using Bundle6 = Internal::Bundle<ID3D12GraphicsCommandList6>;

        template<class ListTy>
        using Compute_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_COMPUTE>;

        using ComputeBase = Direct_t<ID3D12CommandList>;
        using Compute = Compute_t<ID3D12GraphicsCommandList>;
        using Compute1 = Compute_t<ID3D12GraphicsCommandList1>;
        using Compute2 = Compute_t<ID3D12GraphicsCommandList2>;
        using Compute3 = Compute_t<ID3D12GraphicsCommandList3>;
        using Compute4 = Compute_t<ID3D12GraphicsCommandList4>;
        using Compute5 = Compute_t<ID3D12GraphicsCommandList5>;
        //using Compute6 = Internal::Compute<ID3D12GraphicsCommandList6>;

        template<class ListTy>
        using Copy_t = CommandList_t<ListTy, D3D12_COMMAND_LIST_TYPE_COPY>;

        using CopyBase = Direct_t<ID3D12CommandList>;
        using Copy = Copy_t<ID3D12GraphicsCommandList>;
        using Copy1 = Copy_t<ID3D12GraphicsCommandList1>;
        using Copy2 = Copy_t<ID3D12GraphicsCommandList2>;
        using Copy3 = Copy_t<ID3D12GraphicsCommandList3>;
        using Copy4 = Copy_t<ID3D12GraphicsCommandList4>;
        using Copy5 = Copy_t<ID3D12GraphicsCommandList5>;
        //using Copy6 = Internal::Copy<ID3D12GraphicsCommandList6>;

    }
};

#pragma warning(pop)
