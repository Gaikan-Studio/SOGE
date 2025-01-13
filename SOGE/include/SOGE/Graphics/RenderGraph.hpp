#ifndef SOGE_GRAPHICS_RENDERGRAPH_HPP
#define SOGE_GRAPHICS_RENDERGRAPH_HPP

#include "SOGE/DI/Dependency.hpp"

#include <EASTL/functional.h>
#include <EASTL/span.h>


namespace soge
{
    class GraphicsCore;
    class GraphicsEntity;

    class RenderGraph
    {
    public:
        constexpr explicit RenderGraph(GraphicsCore& aCore) noexcept;

        constexpr RenderGraph(const RenderGraph&) = delete;
        constexpr RenderGraph& operator=(const RenderGraph&) = delete;

        constexpr RenderGraph(RenderGraph&&) noexcept = default;
        constexpr RenderGraph& operator=(RenderGraph&&) noexcept = default;

        constexpr virtual ~RenderGraph() = default;

        using EntityRef = eastl::reference_wrapper<GraphicsEntity>;
        using Entities = eastl::span<EntityRef>;

        constexpr virtual void Execute(Entities aEntities) = 0;
    };

    constexpr RenderGraph::RenderGraph(GraphicsCore&) noexcept
    {
    }
}

SOGE_DI_REGISTER_NS(soge, RenderGraph, df::Abstract<RenderGraph>)

#endif // SOGE_GRAPHICS_RENDERGRAPH_HPP
