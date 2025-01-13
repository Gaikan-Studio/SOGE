#ifndef SOGE_GRAPHICS_SIMPLERENDERGRAPH_HPP
#define SOGE_GRAPHICS_SIMPLERENDERGRAPH_HPP

#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"
#include "SOGE/Graphics/RenderGraph.hpp"
#include "SOGE/Graphics/TriangleGraphicsPipeline.hpp"


namespace soge
{
    class SimpleRenderGraph : public RenderGraph
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<FinalGraphicsRenderPass> m_finalRenderPass;
        eastl::reference_wrapper<TriangleGraphicsPipeline> m_trianglePipeline;

        eastl::vector<nvrhi::CommandListHandle> m_commandLists;

    public:
        explicit SimpleRenderGraph(GraphicsCore& aCore, FinalGraphicsRenderPass& aFinalRenderPass,
                                   TriangleGraphicsPipeline& aTrianglePipeline);

        void Execute(Entities aEntities) override;
    };
}

SOGE_DI_REGISTER_NS(soge, SimpleRenderGraph,
                    df::Single<SimpleRenderGraph, GraphicsCore, FinalGraphicsRenderPass, TriangleGraphicsPipeline>,
                    tag::Overrides<SimpleRenderGraph, RenderGraph>)

#endif // SOGE_GRAPHICS_SIMPLERENDERGRAPH_HPP
