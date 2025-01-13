#include "sogepch.hpp"

#include "SOGE/Graphics/GraphicsModule.hpp"

#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"
#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Graphics/GraphicsCompilePreproc.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

#include SOGE_ABS_COMPILED_GRAPHICS_IMPL_HEADER(SOGE/Graphics, GraphicsCore.hpp)


namespace soge
{
    GraphicsModule::GraphicsModule() : m_graphicsCore{nullptr}, m_renderGraph{nullptr}
    {
    }

    void GraphicsModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        SOGE_LOG_TITLE_DIVIDER(GRAPHICS MODULE)

        aModuleManager.CreateModule<EventModule>();
        aContainer.Create<ImplGraphicsCore>();

        m_graphicsCore = &aContainer.Provide<GraphicsCore>();

        SOGE_INFO_LOG("Graphics module loaded...");
    }

    void GraphicsModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_entityManager.Clear();

        m_renderGraph = nullptr;
        m_graphicsCore = nullptr;

        SOGE_INFO_LOG("Graphics module unloaded...");
    }

    void GraphicsModule::SetRenderTarget(const Window& aWindow)
    {
        if (m_graphicsCore == nullptr)
        {
            return;
        }

        m_graphicsCore->SetRenderTarget(aWindow);
    }

    void GraphicsModule::SetRenderGraph(RenderGraph& aRenderGraph)
    {
        m_renderGraph = &aRenderGraph;
    }

    void GraphicsModule::Update()
    {
        if (m_graphicsCore == nullptr && m_renderGraph == nullptr)
        {
            return;
        }

        m_graphicsCore->Update(*m_renderGraph, m_entityManager.GetEntities());
    }

    GraphicsEntityManager& GraphicsModule::GetEntityManager() noexcept
    {
        return m_entityManager;
    }
}
