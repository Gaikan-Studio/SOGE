#include "sogepch.hpp"

#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsSwapchain.hpp"

#include "SOGE/Graphics/Exceptions/NRIException.hpp"
#include "SOGE/Graphics/Impl/D3D12/D3D12GraphicsCore.hpp"
#include "SOGE/Window/Window.hpp"

#include <nvrhi/utils.h>


namespace soge
{
    D3D12GraphicsSwapchain::D3D12GraphicsSwapchain(D3D12GraphicsCore& aCore, const Window& aWindow)
        : GraphicsSwapchain{aWindow}, m_core{aCore}, m_nriSwapChain{nullptr}
    {
        SOGE_INFO_LOG("Creating NRI swap chain for window...");
        nri::SwapChainDesc swapChainDesc{};

        nri::Window nriWindow{};
        nriWindow.windows = nri::WindowsWindow{static_cast<HWND>(aWindow.GetNativeHandler())};
        swapChainDesc.window = nriWindow;

        swapChainDesc.commandQueue = aCore.m_nriGraphicsCommandQueue;
        swapChainDesc.width = static_cast<nri::Dim_t>(aWindow.GetWidth());
        swapChainDesc.height = static_cast<nri::Dim_t>(aWindow.GetHeight());
        swapChainDesc.textureNum = 2;
        swapChainDesc.queuedFrameNum = 2;
        swapChainDesc.format = nri::SwapChainFormat::BT709_G10_16BIT;
        swapChainDesc.waitable = true;

        const nri::SwapChainInterface& nriSwapchainInterface = aCore.m_nriInterface;
        NRIThrowIfFailed(nriSwapchainInterface.CreateSwapChain(*aCore.m_nriDevice, swapChainDesc, m_nriSwapChain),
                         "creating a swap chain for window");
        nriSwapchainInterface.SetSwapChainDebugName(*m_nriSwapChain, "SOGE swap chain");

        std::uint32_t swapChainColorTextureCount;
        nri::Texture* const* swapChainColorTextures =
            nriSwapchainInterface.GetSwapChainTextures(*m_nriSwapChain, swapChainColorTextureCount);
        SOGE_INFO_LOG("NRI swap chain texture count is {}", swapChainColorTextureCount);

        m_nvrhiTextures.reserve(swapChainColorTextureCount);
        m_nvrhiTextureRefs.reserve(swapChainColorTextureCount);
        nri::CoreInterface& nriCoreInterface = aCore.m_nriInterface;
        for (std::size_t index = 0; index < swapChainColorTextureCount; index++)
        {
            SOGE_INFO_LOG("Creating NVRHI swap chain color texture (frame {})...", index);

            nri::Texture* const nriColorTexture = swapChainColorTextures[index];
            const auto nriColorTextureDebugName = fmt::format("SOGE swap chain color texture (frame {})", index);
            nriCoreInterface.SetTextureDebugName(*nriColorTexture, nriColorTextureDebugName.c_str());

            const nri::TextureDesc& nriColorTextureDesc = nriCoreInterface.GetTextureDesc(*nriColorTexture);
            assert(nriColorTextureDesc.type == nri::TextureType::TEXTURE_2D);
            assert(nriColorTextureDesc.usage & nri::TextureUsageBits::COLOR_ATTACHMENT &&
                   nriColorTextureDesc.usage & nri::TextureUsageBits::SHADER_RESOURCE);
            assert(nriColorTextureDesc.format == nri::Format::RGBA16_SFLOAT);

            nvrhi::TextureDesc nvrhiColorTextureDesc{};
            nvrhiColorTextureDesc.dimension = nvrhi::TextureDimension::Texture2D;
            nvrhiColorTextureDesc.format = nvrhi::Format::RGBA16_FLOAT;
            nvrhiColorTextureDesc.width = nriColorTextureDesc.width;
            nvrhiColorTextureDesc.height = nriColorTextureDesc.height;
            nvrhiColorTextureDesc.isRenderTarget = true;
            nvrhiColorTextureDesc.isShaderResource = true;
            nvrhiColorTextureDesc.depth = nriColorTextureDesc.depth;
            nvrhiColorTextureDesc.mipLevels = nriColorTextureDesc.mipNum;
            nvrhiColorTextureDesc.arraySize = nriColorTextureDesc.layerNum;
            nvrhiColorTextureDesc.sampleCount = nriColorTextureDesc.sampleNum;
            nvrhiColorTextureDesc.useClearValue = true;
            nvrhiColorTextureDesc.clearValue = nvrhi::Color{};
            nvrhiColorTextureDesc.initialState = nvrhi::ResourceStates::Present;
            nvrhiColorTextureDesc.keepInitialState = true;

            nvrhi::TextureHandle nvrhiColorTexture = aCore.GetRawDevice().createHandleForNativeTexture(
                nvrhi::ObjectTypes::D3D12_Resource, nriCoreInterface.GetTextureNativeObject(*nriColorTexture),
                nvrhiColorTextureDesc);
            m_nvrhiTextures.emplace_back(nvrhiColorTexture);
            m_nvrhiTextureRefs.emplace_back(*nvrhiColorTexture);
        }
    }

    D3D12GraphicsSwapchain::D3D12GraphicsSwapchain(D3D12GraphicsSwapchain&& aOther) noexcept
        : GraphicsSwapchain{std::move(aOther)},
          m_core{aOther.m_core}, // NOLINT(bugprone-use-after-move) reason: move is no-op
          m_nriSwapChain{}
    {
        swap(aOther);
    }

    D3D12GraphicsSwapchain& D3D12GraphicsSwapchain::operator=(D3D12GraphicsSwapchain&& aOther) noexcept
    {
        swap(aOther);
        return *this;
    }

    void D3D12GraphicsSwapchain::swap(D3D12GraphicsSwapchain& aOther) noexcept
    {
        using std::swap;

        eastl::swap(m_core, aOther.m_core);

        swap(m_nriSwapChain, aOther.m_nriSwapChain);

        swap(m_nvrhiTextures, aOther.m_nvrhiTextures);
        swap(m_nvrhiTextureRefs, aOther.m_nvrhiTextureRefs);
    }

    D3D12GraphicsSwapchain::~D3D12GraphicsSwapchain()
    {
        m_nvrhiTextureRefs.clear();
        m_nvrhiTextures.clear();

        if (m_nriSwapChain != nullptr)
        {
            SOGE_INFO_LOG("Destroying NRI swap chain...");
            const nri::SwapChainInterface& nriInterface = m_core.get().m_nriInterface;
            nriInterface.DestroySwapChain(*m_nriSwapChain);
            m_nriSwapChain = nullptr;
        }
    }

    auto D3D12GraphicsSwapchain::GetTextures() -> Textures
    {
        return m_nvrhiTextureRefs;
    }

    std::uint32_t D3D12GraphicsSwapchain::GetCurrentTextureIndex()
    {
        const nri::SwapChainInterface& nriInterface = m_core.get().m_nriInterface;
        return nriInterface.AcquireNextSwapChainTexture(*m_nriSwapChain);
    }

    void D3D12GraphicsSwapchain::WaitForPresent()
    {
        const nri::SwapChainInterface& nriInterface = m_core.get().m_nriInterface;
        NRIThrowIfFailed(nriInterface.WaitForPresent(*m_nriSwapChain), "waiting for swap chain to present");
    }

    void D3D12GraphicsSwapchain::Present()
    {
        const nri::SwapChainInterface& nriInterface = m_core.get().m_nriInterface;
        NRIThrowIfFailed(nriInterface.QueuePresent(*m_nriSwapChain), "presenting swap chain");
    }
}
