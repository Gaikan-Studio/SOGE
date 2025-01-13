#include "sogepch.hpp"

#include "SOGE/Graphics/SimpleRenderGraph.hpp"

#include "SOGE/Graphics/GraphicsEntity.hpp"
#include "SOGE/Graphics/TriangleGraphicsPipeline.hpp"


namespace soge
{
    SimpleRenderGraph::SimpleRenderGraph(GraphicsCore& aCore, FinalGraphicsRenderPass& aFinalRenderPass,
                                         TriangleGraphicsPipeline& aTrianglePipeline)
        : RenderGraph{aCore}, m_core{aCore}, m_finalRenderPass{aFinalRenderPass}, m_trianglePipeline{aTrianglePipeline}
    {
    }

    void SimpleRenderGraph::Execute(const Entities aEntities)
    {
        const auto clearCommandList = m_finalRenderPass.get().CreateClearFramebufferCommandList();

        m_commandLists.reserve(aEntities.size() + 1);
        m_commandLists.emplace_back(clearCommandList);
        for (const auto& entity : aEntities)
        {
            auto commandList = entity.get().Update(m_finalRenderPass, m_trianglePipeline);
            m_commandLists.emplace_back(commandList);
        }

        m_core.get().ExecuteCommandLists(m_commandLists, nvrhi::CommandQueue::Graphics);

        m_commandLists.clear();
    }
}
