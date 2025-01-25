#ifndef SOGE_GRAPHICS_GRAPHICSPRIMITIVE_HPP
#define SOGE_GRAPHICS_GRAPHICSPRIMITIVE_HPP

#include "SOGE/Graphics/Deferred/GeometryGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class GraphicsPrimitive : public GraphicsEntity, public GeometryGraphicsPipelineEntity
    {
    public:
        [[nodiscard]]
        constexpr virtual Transform GetTransform() const = 0;
        [[nodiscard]]
        constexpr virtual glm::vec3 GetColor() const = 0;
        [[nodiscard]]
        constexpr virtual eastl::span<const Vertex> GetVertices() const = 0;
        [[nodiscard]]
        constexpr virtual eastl::span<const Index> GetIndices() const = 0;
    };
}

#endif // SOGE_GRAPHICS_GRAPHICSPRIMITIVE_HPP
