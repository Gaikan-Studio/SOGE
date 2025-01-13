#ifndef SOGE_GRAPHICS_GRAPHICSCORE_HPP
#define SOGE_GRAPHICS_GRAPHICSCORE_HPP

#include "SOGE/DI/Dependency.hpp"
#include "SOGE/Graphics/ExecuteCommandLists.hpp"

#include <EASTL/functional.h>
#include <EASTL/span.h>


namespace soge
{
    class Window;
    class GraphicsSwapchain;
    class GraphicsEntity;

    class RenderGraph;

    class GraphicsCore
    {
    public:
        constexpr explicit GraphicsCore() noexcept = default;

        constexpr GraphicsCore(const GraphicsCore&) noexcept = delete;
        constexpr GraphicsCore& operator=(const GraphicsCore&) noexcept = delete;

        constexpr GraphicsCore(GraphicsCore&&) noexcept = default;
        constexpr GraphicsCore& operator=(GraphicsCore&&) noexcept = default;

        constexpr virtual ~GraphicsCore() noexcept = default;

        constexpr virtual void SetRenderTarget(const Window& aWindow) = 0;
        [[nodiscard]]
        constexpr virtual GraphicsSwapchain* GetSwapchain() = 0;

        using EntityRef = eastl::reference_wrapper<GraphicsEntity>;
        using Entities = eastl::span<EntityRef>;

        constexpr virtual void Update(RenderGraph& aRenderGraph, Entities aEntities) = 0;

        [[nodiscard]]
        constexpr virtual nvrhi::IDevice& GetRawDevice() = 0;
        [[nodiscard]]
        constexpr virtual eastl::string_view GetCompiledShaderExtension() const = 0;

        template <CommandListRange T>
        void ExecuteCommandLists(T&& aCommandLists,
                                 nvrhi::CommandQueue aExecutionQueue = nvrhi::CommandQueue::Graphics);
        void ExecuteCommandList(const nvrhi::CommandListHandle& aCommandList,
                                nvrhi::CommandQueue aExecutionQueue = nvrhi::CommandQueue::Graphics);
    };

    template <CommandListRange T>
    void GraphicsCore::ExecuteCommandLists(T&& aCommandLists, const nvrhi::CommandQueue aExecutionQueue)
    {
        soge::ExecuteCommandLists(GetRawDevice(), std::forward<T>(aCommandLists), aExecutionQueue);
    }

    inline void GraphicsCore::ExecuteCommandList(const nvrhi::CommandListHandle& aCommandList,
                                                 const nvrhi::CommandQueue aExecutionQueue)
    {
        GetRawDevice().executeCommandList(aCommandList, aExecutionQueue);
    }
}

SOGE_DI_REGISTER_NS(soge, GraphicsCore, df::Abstract<GraphicsCore>)

#endif // SOGE_GRAPHICS_GRAPHICSCORE_HPP
