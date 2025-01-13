#ifndef SOGE_GRAPHICS_GRAPHICSSWAPCHAIN_HPP
#define SOGE_GRAPHICS_GRAPHICSSWAPCHAIN_HPP

#include <EASTL/functional.h>
#include <EASTL/span.h>
#include <nvrhi/nvrhi.h>


namespace soge
{
    class Window;

    // TODO: link this with lifetime of the window
    class GraphicsSwapchain
    {
    public:
        constexpr explicit GraphicsSwapchain(const Window& aWindow);

        constexpr GraphicsSwapchain(const GraphicsSwapchain&) = delete;
        constexpr GraphicsSwapchain& operator=(const GraphicsSwapchain&) = delete;

        constexpr GraphicsSwapchain(GraphicsSwapchain&&) noexcept = default;
        constexpr GraphicsSwapchain& operator=(GraphicsSwapchain&&) noexcept = default;

        constexpr virtual ~GraphicsSwapchain() = default;

        using TextureRef = eastl::reference_wrapper<nvrhi::ITexture>;
        using Textures = eastl::span<TextureRef>;

        [[nodiscard]]
        constexpr virtual Textures GetTextures() = 0;
        constexpr virtual std::uint32_t GetCurrentTextureIndex() = 0;

        constexpr virtual void WaitForPresent() = 0;
        constexpr virtual void Present() = 0;
    };

    constexpr GraphicsSwapchain::GraphicsSwapchain([[maybe_unused]] const Window& aWindow)
    {
    }
}

#endif // SOGE_GRAPHICS_GRAPHICSSWAPCHAIN_HPP
