#ifndef SOGE_GRAPHICS_TRIANGLEENTITY_HPP
#define SOGE_GRAPHICS_TRIANGLEENTITY_HPP

#include "SOGE/DI/Dependency.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"
#include "SOGE/Graphics/TriangleGraphicsPipeline.hpp"

#include <EASTL/span.h>
#include <glm/mat4x4.hpp>


namespace soge
{
    class TriangleEntity : public GraphicsEntity
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<TriangleGraphicsPipeline> m_pipeline;

        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;
        nvrhi::BindingSetHandle m_nvrhiBindingSet;

    public:
        using Vertex = TriangleGraphicsPipeline::Vertex;
        using Vertices = eastl::span<const Vertex>;

        explicit TriangleEntity(GraphicsCore& aCore, TriangleGraphicsPipeline& aPipeline);

        void UpdateVertices(Vertices aVertices);
        void UpdateMatrix(const glm::mat4x4& aMatrix);

        [[nodiscard]]
        nvrhi::CommandListHandle Update(GraphicsRenderPass& aRenderPass, GraphicsPipeline& aPipeline) override;
    };
}

SOGE_DI_REGISTER_NS(soge, TriangleEntity, df::Factory<TriangleEntity, GraphicsCore, TriangleGraphicsPipeline>)

#endif // SOGE_GRAPHICS_TRIANGLEENTITY_HPP
