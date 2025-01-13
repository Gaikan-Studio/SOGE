#ifndef SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSSWAPCHAIN_HPP
#define SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSSWAPCHAIN_HPP

#include "SOGE/Graphics/GraphicsSwapchain.hpp"

#include <NRI.h>

#include <Extensions/NRISwapChain.h>

#include <EASTL/vector.h>


namespace soge
{
    class D3D12GraphicsCore;

    class D3D12GraphicsSwapchain : public GraphicsSwapchain
    {
    private:
        eastl::reference_wrapper<D3D12GraphicsCore> m_core;

        nri::SwapChain* m_nriSwapChain;

        eastl::vector<nvrhi::TextureHandle> m_nvrhiTextures;
        eastl::vector<TextureRef> m_nvrhiTextureRefs;

    public:
        explicit D3D12GraphicsSwapchain(D3D12GraphicsCore& aCore, const Window& aWindow);

        D3D12GraphicsSwapchain(const D3D12GraphicsSwapchain&) = delete;
        D3D12GraphicsSwapchain& operator=(const D3D12GraphicsSwapchain&) = delete;

        D3D12GraphicsSwapchain(D3D12GraphicsSwapchain&& aOther) noexcept;
        D3D12GraphicsSwapchain& operator=(D3D12GraphicsSwapchain&& aOther) noexcept;

        ~D3D12GraphicsSwapchain() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(D3D12GraphicsSwapchain& aOther) noexcept;

        [[nodiscard]]
        Textures GetTextures() override;
        std::uint32_t GetCurrentTextureIndex() override;

        void WaitForPresent() override;
        void Present() override;
    };

    using ImplGraphicsSwapchain = D3D12GraphicsSwapchain;
}

#endif // SOGE_GRAPHICS_IMPL_D3D12_D3D12GRAPHICSSWAPCHAIN_HPP
