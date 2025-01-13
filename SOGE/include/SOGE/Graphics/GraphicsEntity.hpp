#ifndef SOGE_GRAPHICS_GRAPHICSENTITY_HPP
#define SOGE_GRAPHICS_GRAPHICSENTITY_HPP

#include <nvrhi/nvrhi.h>


namespace soge
{
    class GraphicsRenderPass;
    class GraphicsPipeline;

    class GraphicsEntity
    {
    public:
        constexpr explicit GraphicsEntity() noexcept = default;

        constexpr GraphicsEntity(const GraphicsEntity&) = delete;
        constexpr GraphicsEntity& operator=(const GraphicsEntity&) = delete;

        constexpr GraphicsEntity(GraphicsEntity&&) noexcept = default;
        constexpr GraphicsEntity& operator=(GraphicsEntity&&) noexcept = default;

        constexpr virtual ~GraphicsEntity() noexcept = default;

        [[nodiscard]]
        constexpr virtual nvrhi::CommandListHandle Update(GraphicsRenderPass& aRenderPass,
                                                          GraphicsPipeline& aPipeline) = 0;
    };
}

#endif // SOGE_GRAPHICS_GRAPHICSENTITY_HPP
