#ifndef SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSCORE_HPP
#define SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSCORE_HPP

#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/System/Memory.hpp"

#include <NRI.h>

#include <Extensions/NRIDeviceCreation.h>
#include <Extensions/NRIHelper.h>
#include <Extensions/NRIStreamer.h>
#include <Extensions/NRISwapChain.h>

#include <nvrhi/d3d12.h>


namespace soge
{
    class GraphicsSwapchain;
    class GraphicsRenderPass;
    class GraphicsPipeline;

    class D3D12GraphicsCore : public GraphicsCore
    {
    private:
        class NvrhiMessageCallback final : public nvrhi::IMessageCallback
        {
        public:
            explicit NvrhiMessageCallback() = default;

            NvrhiMessageCallback(const NvrhiMessageCallback&) = delete;
            NvrhiMessageCallback& operator=(const NvrhiMessageCallback&) = delete;

            NvrhiMessageCallback(NvrhiMessageCallback&&) noexcept;
            NvrhiMessageCallback& operator=(NvrhiMessageCallback&&) noexcept;

            ~NvrhiMessageCallback() override = default;

            void message(nvrhi::MessageSeverity aSeverity, const char* aMessageText) override;
        };

        class NriInterface final : public nri::CoreInterface,
                                   public nri::HelperInterface,
                                   public nri::StreamerInterface,
                                   public nri::SwapChainInterface
        {
        };

        friend class D3D12GraphicsSwapchain;

        static void NriMessageCallback(nri::Message aMessageType, const char* aFile, std::uint32_t aLine,
                                       const char* aMessage, void* aUserArg);

        void DestroySwapChain();
        void DestroyDevice();

        nri::Device* m_nriInitDevice;
        nri::Device* m_nriDevice;
        NriInterface m_nriInterface;
        nri::CommandQueue* m_nriGraphicsCommandQueue;
        nri::CallbackInterface m_nriCallbackInterface;
        nri::AllocationCallbacks m_nriAllocationCallbacks;

        NvrhiMessageCallback m_nvrhiMessageCallback;
        nvrhi::DeviceHandle m_nvrhiDevice;

        UniquePtr<GraphicsSwapchain> m_swapChain;

        std::uint64_t m_totalFrameCount;

    public:
        explicit D3D12GraphicsCore();

        D3D12GraphicsCore(const D3D12GraphicsCore&) = delete;
        D3D12GraphicsCore& operator=(const D3D12GraphicsCore&) = delete;

        D3D12GraphicsCore(D3D12GraphicsCore&& aOther) noexcept;
        D3D12GraphicsCore& operator=(D3D12GraphicsCore&& aOther) noexcept;

        ~D3D12GraphicsCore() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(D3D12GraphicsCore& aOther) noexcept;

        void SetRenderTarget(const Window& aWindow) override;
        [[nodiscard]]
        GraphicsSwapchain* GetSwapchain() override;

        void Update(RenderGraph& aRenderGraph, Entities aEntities) override;

        [[nodiscard]]
        nvrhi::IDevice& GetRawDevice() override;
        [[nodiscard]]
        eastl::string_view GetCompiledShaderExtension() const override;
    };

    using ImplGraphicsCore = D3D12GraphicsCore;
}

SOGE_DI_REGISTER_NS(soge, D3D12GraphicsCore, df::Single<D3D12GraphicsCore>,
                    tag::Overrides<D3D12GraphicsCore, GraphicsCore>)

#endif // SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSCORE_HPP
