#include "sogepch.hpp"

#include "SOGE/Core/Engine.hpp"

#include "SOGE/Core/JobSystem.hpp"
#include "SOGE/Core/Timestep.hpp"
#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Input/InputModule.hpp"
#include "SOGE/Utils/StringHelpers.hpp"
#include "SOGE/Window/WindowModule.hpp"

#include <ranges>

#undef CreateWindow


namespace soge
{
    UniquePtr<Engine> Engine::s_instance(nullptr);
    std::mutex Engine::s_mutex;

    Engine* Engine::GetInstance()
    {
        // Fast path: return the instance if it is already initialized
        if (s_instance != nullptr)
        {
            return s_instance.get();
        }

        // Safe but slow path: initialize with default class if empty
        std::lock_guard lock(s_mutex);
        // Additional check to ensure we are creating new instance exactly once
        if (s_instance == nullptr)
        {
            // Replicating `make_unique` here because the constructor is protected
            s_instance = UniquePtr<Engine>(new Engine());
        }
        return s_instance.get();
    }

    Engine::Engine() : m_isRunning(false), m_shutdownRequested(false), m_jobs(nullptr)
    {
        SOGE_INFO_LOG("Initialize engine...");

        CreateModule<EventModule>();
        CreateModule<InputModule>();
        CreateModule<WindowModule>();
    }

    Engine::~Engine()
    {
        SOGE_INFO_LOG("Destroy engine...");
    }

    void Engine::Run()
    {
        if (m_isRunning)
        {
            return;
        }

        // Prevent users from resetting engine while it is running
        std::lock_guard lock(s_mutex);

        m_jobs = &m_container.Provide<JobSystem>();

        m_isRunning = true;
        for (Module& module : m_moduleManager)
        {
            module.Load(m_container, m_moduleManager);
        }

        const auto [window, uuid] = GetModule<WindowModule>()->CreateWindow();
        SOGE_INFO_LOG(R"(Created window "{}" of width {} and height {} with UUID {})",
                      EAToNarrow(window.GetTitle()).c_str(), window.GetWidth(), window.GetHeight(), uuid.str());

        m_shutdownRequested = false;
        while (!m_shutdownRequested)
        {
            Timestep::StartFrame();
            Timestep::CalculateDelta();

            GetModule<InputModule>()->Update();

            const auto eventModule = GetModule<EventModule>();
            eventModule->Dispatch<UpdateEvent>(Timestep::DeltaTime());

            for (const auto layer : m_renderLayers)
            {
                layer->OnUpdate();
            }
        }

        for (Module& module : m_moduleManager | std::views::reverse)
        {
            module.Unload(m_container, m_moduleManager);
        }
        m_isRunning = false;
        m_removedModules.clear();
        m_container.Clear();
        m_jobs = nullptr;
    }

    bool Engine::IsRunning() const
    {
        return m_isRunning;
    }

    void Engine::RequestShutdown()
    {
        m_shutdownRequested = true;
    }

    void Engine::PushLayer(Layer* aLayer)
    {
        m_renderLayers.PushLayer(aLayer);
        aLayer->OnAttach();
    }

    void Engine::PushOverlay(Layer* aOverlayLayer)
    {
        m_renderLayers.PushOverlay(aOverlayLayer);
        aOverlayLayer->OnAttach();
    }

    di::Container& Engine::GetDependencyContainer()
    {
        return m_container;
    }

    di::Provider& Engine::GetDependencyProvider()
    {
        return m_container;
    }
}
