#ifndef SOGE_CORE_ENGINE_HPP
#define SOGE_CORE_ENGINE_HPP

#include "SOGE/Core/LayerStack.hpp"
#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Core/CLIManager.hpp"


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
        // Used to work around UB when trying to lock a mutex by a thread that already locked it
        // https://en.cppreference.com/w/cpp/thread/mutex/lock#:~:text=If%20lock%20is%20called%20by%20a%20thread%20that%20already%20owns%20the%20mutex%2C%20the%20behavior%20is%20undefined
        thread_local static std::atomic_bool s_mutexLocked;

        class MutexLockedGuard
        {
        public:
            explicit MutexLockedGuard() noexcept;

            explicit MutexLockedGuard(const MutexLockedGuard&) = delete;
            MutexLockedGuard& operator=(const MutexLockedGuard&) = delete;

            explicit MutexLockedGuard(MutexLockedGuard&&) noexcept = delete;
            MutexLockedGuard& operator=(MutexLockedGuard&&) noexcept = delete;

            ~MutexLockedGuard() noexcept;
        };

        static void AssertMutexIsNotLocked();

        LayerStack m_renderLayers;
        bool m_isRunning;
        CLIManager m_cliManager;
        std::atomic_bool m_shutdownRequested;

        di::Container m_container;
        ModuleManager m_moduleManager;

        // do not destroy modules immediately after removing them, they might be still in use
        // (because they could have been provided by dependency container)
        using RemovedModules = eastl::vector<UniquePtr<Module>>;
        RemovedModules m_removedModules;

    protected:
        class AccessTag
        {
        private:
            friend Engine;

            explicit AccessTag() = default;

        public:
            explicit AccessTag(const AccessTag&) = delete;
            AccessTag& operator=(const AccessTag&) = delete;

            explicit AccessTag(AccessTag&&) noexcept = default;
            AccessTag& operator=(AccessTag&&) noexcept = default;

            ~AccessTag() = default;
        };

        explicit Engine(AccessTag&& aTag);

        virtual void Load(AccessTag);
        virtual void Unload(AccessTag);

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

        void HandleCLIArgs(int argc, char** argv);

    };

    template <DerivedFromEngine T, typename... Args>
    T* Engine::Reset(Args&&... args)
    {
        AssertMutexIsNotLocked();
        std::lock_guard lock(s_mutex);
        MutexLockedGuard mutexLockedGuard;

        // Replicating `make_unique` here because the constructor is protected
        UniquePtr<T> newInstance(new T(AccessTag{}, std::forward<Args>(args)...));
        // Save pointer to `T` to avoid dynamic cast later
        T* pNewInstance = newInstance.get();

        // Move new instance into the static member variable
        s_instance = std::move(newInstance);
        // Return correctly typed pointer saved earlier
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
