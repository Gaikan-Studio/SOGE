#ifndef SOGE_CORE_ENGINE_HPP
#define SOGE_CORE_ENGINE_HPP

#include "SOGE/Core/JobSystem.hpp"
#include "SOGE/Core/LayerStack.hpp"
#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"
#include "SOGE/System/Memory.hpp"


namespace soge
{
    class Engine;

    template <typename T>
    concept DerivedFromEngine = std::derived_from<T, Engine>;

    class Engine
    {
    private:
        static UniquePtr<Engine> s_instance;
        static std::mutex s_mutex;

        bool m_isRunning;
        bool m_shutdownRequested;

        LayerStack m_renderLayers;
        JobSystem* m_jobSystem;

        di::Container m_container;
        ModuleManager m_moduleManager;

        // do not destroy modules immediately after removing them, they might be still in use
        // (because they could have been provided by dependency container)
        using RemovedModules = eastl::vector<UniquePtr<Module>>;
        RemovedModules m_removedModules;

    protected:
        explicit Engine();

        di::Container& GetDependencyContainer();

    public:
        static Engine* GetInstance();

        template <DerivedFromEngine T = Engine, typename... Args>
        static T* Reset(Args&&... args);

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        Engine(Engine&&) = delete;
        Engine& operator=(Engine&&) = delete;

        virtual ~Engine();

        void Run();
        void RequestShutdown();

        [[nodiscard]]
        bool IsRunning() const;

        di::Provider& GetDependencyProvider();

        template <DerivedFromModule T, typename... Args>
        T& CreateModule(Args&&... args);

        template <DerivedFromModule T, typename... Args>
        T& RecreateModule(Args&&... args);

        template <DerivedFromModule T>
        void RemoveModule();

        template <DerivedFromModule T>
        [[nodiscard]]
        T* GetModule() const;

        void PushLayer(Layer* aLayer);
        void PushOverlay(Layer* aOverlayLayer);
    };

    template <DerivedFromEngine T, typename... Args>
    T* Engine::Reset(Args&&... args)
    {
        // Replicating `make_unique` here because the constructor is protected
        UniquePtr<T> newInstance(new T(std::forward<Args>(args)...));
        // Save pointer to `T` to avoid dynamic cast later
        T* pNewInstance = newInstance.get();

        // Acquire lock just before moving the new instance
        // (this allows to allocate new instance in parallel compared to acquiring at the beginning of the function)
        std::lock_guard lock(s_mutex);
        // Move new instance to the static instance
        s_instance = std::move(newInstance);
        // Return previously saved pointer
        return pNewInstance;
    }

    template <DerivedFromModule T, typename... Args>
    T& Engine::CreateModule(Args&&... args)
    {
        auto [module, created] = m_moduleManager.CreateModule<T>(std::forward<Args>(args)...);
        if constexpr (di::ModuleDependency<T>)
        {
            m_container.Create<T>(module);
        }

        if (created && m_isRunning)
        {
            module.Load(m_container, m_moduleManager);
        }

        return module;
    }

    template <DerivedFromModule T, typename... Args>
    T& Engine::RecreateModule(Args&&... args)
    {
        auto [module, oldModule] = m_moduleManager.RecreateModule<T>(std::forward<Args>(args)...);
        if constexpr (di::ModuleDependency<T>)
        {
            m_container.Recreate<T>(module);
        }

        if (m_isRunning)
        {
            if (oldModule != nullptr)
            {
                oldModule->Unload(m_container, m_moduleManager);
                m_removedModules.push_back(std::move(oldModule));
            }

            module.Load(m_container, m_moduleManager);
        }

        return module;
    }

    template <DerivedFromModule T>
    void Engine::RemoveModule()
    {
        auto oldModule = m_moduleManager.RemoveModule<T>();

        if (oldModule != nullptr && m_isRunning)
        {
            oldModule->Unload(m_container, m_moduleManager);
            m_removedModules.push_back(std::move(oldModule));
        }
    }

    template <DerivedFromModule T>
    T* Engine::GetModule() const
    {
        return m_moduleManager.GetModule<T>();
    }

    Engine* CreateApplication();
}

#endif // SOGE_CORE_ENGINE_HPP
