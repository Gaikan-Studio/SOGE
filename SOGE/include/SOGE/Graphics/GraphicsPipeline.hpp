#ifndef SOGE_GRAPHICS_GRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_GRAPHICSPIPELINE_HPP

#include <nvrhi/nvrhi.h>


namespace soge
{
    class GraphicsPipeline
    {
    public:
        constexpr explicit GraphicsPipeline() = default;

        constexpr GraphicsPipeline(const GraphicsPipeline&) = delete;
        constexpr GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;

        constexpr GraphicsPipeline(GraphicsPipeline&&) noexcept = default;
        constexpr GraphicsPipeline& operator=(GraphicsPipeline&&) noexcept = default;

        constexpr virtual ~GraphicsPipeline() = default;

        [[nodiscard]]
        constexpr virtual nvrhi::IGraphicsPipeline& GetGraphicsPipeline() = 0;
    };
}

#endif // SOGE_GRAPHICS_GRAPHICSPIPELINE_HPP
