#include "sogepch.hpp"

#include "SOGE/Core/Engine.hpp"
#include "SOGE/Core/Timestep.hpp"


namespace soge
{
    UniquePtr<Engine> Engine::s_instance(nullptr);
    std::mutex Engine::s_init_mutex;

    Engine* Engine::GetInstance()
    {
        // Fast path: return the instance if it is already initialized
        if (s_instance != nullptr)
        {
            return s_instance.get();
        }

        // Safe but slow path: initialize with default class if empty
        std::lock_guard lock(s_init_mutex);
        // Additional check to ensure we are creating new instance exactly once
        if (s_instance == nullptr)
        {
            // Replicating `make_unique` here because the constructor is protected
            s_instance = UniquePtr<Engine>(new Engine());
        }
        return s_instance.get();
    }

    Engine::Engine() : m_isRunning(false)
    {
        SOGE_INFO_LOG("Initialize engine...");
    }

    Engine::~Engine()
    {
        SOGE_INFO_LOG("Destroy engine...");
    }

    void Engine::Run()
    {
        m_isRunning = true;
        while (m_isRunning)
        {
            Timestep::StartFrame();
            Timestep::CalculateDelta();
            SOGE_INFO_LOG("DeltaTime: {0}", Timestep::RealDeltaTime());
        }

        Shutdown();
    }

    void Engine::Update()
    {
        FixedUpdate();
    }

    void Engine::FixedUpdate()
    {
    }

    void Engine::RequestShutdown()
    {
        m_isRunning = false;
    }

    void Engine::Shutdown()
    {
    }
}
