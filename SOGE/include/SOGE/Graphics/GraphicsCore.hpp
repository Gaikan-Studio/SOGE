#ifndef SOGE_GRAPHICS_GRAPHICSCORE_HPP
#define SOGE_GRAPHICS_GRAPHICSCORE_HPP

#include "SOGE/DI/Dependency.hpp"


namespace soge
{
    class GraphicsCore
    {
    public:
        constexpr explicit GraphicsCore() noexcept = default;

        constexpr explicit GraphicsCore(const GraphicsCore&) noexcept = delete;
        constexpr GraphicsCore& operator=(const GraphicsCore&) noexcept = delete;

        constexpr explicit GraphicsCore(GraphicsCore&&) noexcept = default;
        constexpr GraphicsCore& operator=(GraphicsCore&&) noexcept = default;

        constexpr virtual ~GraphicsCore() noexcept = default;

        virtual void Update(float aDeltaTime) = 0;
    };
}

SOGE_DI_REGISTER_NS(soge, GraphicsCore, df::Abstract<GraphicsCore>)

#endif // SOGE_GRAPHICS_GRAPHICSCORE_HPP