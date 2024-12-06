#include "sogepch.hpp"

#include "SOGE/Core/Engine.hpp"
#include "SOGE/Core/Timestep.hpp"
#include "SOGE/Input/InputTypes.hpp"


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

    Engine::Engine() : m_isRunning(false), m_shutdownRequested(false)
    {
        SOGE_INFO_LOG("Initialize engine...");

        m_eventManager = CreateUnique<EventManager>();
        m_systemWindow = UniquePtr<Window>(Window::Create());

        m_inputManager = CreateShared<InputManager>();
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
        m_isRunning = true;

        m_shutdownRequested = false;
        while (!m_shutdownRequested)
        {
            Timestep::StartFrame();
            Timestep::CalculateDelta();

            m_inputManager->Update();
            for (auto layer : m_renderLayers)
            {
                layer->OnUpdate();
            }
        }

        m_isRunning = false;
    }

    bool Engine::IsRunning() const
    {
        return m_isRunning;
    }

    EventManager* Engine::GetEventManager() const
    {
        return m_eventManager.get();
    }

    InputManager* Engine::GetInputManager()
    {
        return Engine::GetInstance()->m_inputManager.get();
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

    di::Container& Engine::GetContainer()
    {
        return m_container;
    }
}
