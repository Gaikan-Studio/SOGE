#include "sogepch.hpp"

#include "SOGE/Core/Engine.hpp"

#include "SOGE/Core/Timestep.hpp"
#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Graphics/GraphicsModule.hpp"
#include "SOGE/Input/InputModule.hpp"
#include "SOGE/Sound/SoundModule.hpp"
#include "SOGE/Window/WindowModule.hpp"


namespace soge
{
    UniquePtr<Engine> Engine::s_instance{nullptr};
    std::mutex Engine::s_mutex{};
    thread_local std::atomic_bool Engine::s_mutexLocked{};

    Engine* Engine::GetInstance()
    {
        // Fast path: return the instance if it is already initialized
        if (s_instance != nullptr)
        {
            return s_instance.get();
        }

        // Safe but slow path: initialize with default class if empty
        AssertMutexIsNotLocked();
        std::lock_guard lock(s_mutex);
        MutexLockedGuard mutexLockedGuard;

        // Additional check to ensure we are creating new instance exactly once
        if (s_instance == nullptr)
        {
            // Replicating `make_unique` here because the constructor is protected
            s_instance = UniquePtr<Engine>(new Engine(AccessTag{}));
        }
        return s_instance.get();
    }

    Engine::MutexLockedGuard::MutexLockedGuard() noexcept
    {
        s_mutexLocked = true;
    }

    Engine::MutexLockedGuard::~MutexLockedGuard() noexcept
    {
        s_mutexLocked = false;
    }

    void Engine::AssertMutexIsNotLocked()
    {
        if (!s_mutexLocked.load())
        {
            return;
        }

        SOGE_ERROR_LOG("Engine mutex is already locked by the current thread!");
        std::exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
    }

    Engine::Engine([[maybe_unused]] AccessTag&& aTag) : m_isRunning(false), m_shutdownRequested(false)
    {
        SOGE_INFO_LOG("Initialize engine...");

        CreateModule<EventModule>();
        CreateModule<InputModule>();
        CreateModule<SoundModule>();
        CreateModule<WindowModule>();
        CreateModule<GraphicsModule>();
    }

    void Engine::Load(AccessTag)
    {
    }

    void Engine::Unload(AccessTag)
    {
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
        AssertMutexIsNotLocked();
        std::lock_guard lock(s_mutex);
        MutexLockedGuard mutexLockedGuard;

        m_isRunning = true;
        for (Module& module : m_moduleManager)
        {
            module.Load(m_container, m_moduleManager);
        }
        Load(AccessTag{});

        m_shutdownRequested = false;
        while (!m_shutdownRequested)
        {
            Timestep::StartFrame();
            Timestep::CalculateDelta();

            GetModule<InputModule>()->Update();
            GetModule<SoundModule>()->Update();

            const auto eventModule = GetModule<EventModule>();
            eventModule->Enqueue<UpdateEvent>(Timestep::DeltaTime());
            eventModule->DispatchQueue<UpdateEvent>();

            for (const auto layer : m_renderLayers)
            {
                layer->OnUpdate();
            }

            GetModule<GraphicsModule>()->Update();
        }

        Unload(AccessTag{});
        for (Module& module : m_moduleManager | std::views::reverse)
        {
            module.Unload(m_container, m_moduleManager);
        }
        m_isRunning = false;
        m_removedModules.clear();
        m_container.Clear();
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

    void Engine::HandleCLIArgs(int argc, char** argv)
    {
        m_cliManager.ParseArgs(argc, argv);
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
