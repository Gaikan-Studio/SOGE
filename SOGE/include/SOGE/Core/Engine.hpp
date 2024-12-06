#ifndef SOGE_CORE_ENGINE_HPP
#define SOGE_CORE_ENGINE_HPP

#include "SOGE/Core/DI/Container.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Core/EventManager.hpp"
#include "SOGE/Input/InputManager.hpp"
#include "SOGE/System/Window.hpp"
#include "SOGE/Core/LayerStack.hpp"


namespace soge
{
    class Engine;

    template <typename T>
    concept DerivedFromEngine = std::is_base_of_v<Engine, T>;

    class Engine
    {
    private:
        static UniquePtr<Engine> s_instance;
        static std::mutex s_mutex;

        LayerStack m_renderLayers;
        UniquePtr<EventManager> m_eventManager;
        UniquePtr<Window> m_systemWindow;
        SharedPtr<InputManager> m_inputManager;

        bool m_isRunning;
        bool m_shutdownRequested;

        di::Container m_container;

    protected:
        explicit Engine();

    public:
        Engine(const Engine&) = delete;
        auto operator=(const Engine&) = delete;

        Engine(Engine&&) = delete;
        auto operator=(Engine&&) = delete;
        virtual ~Engine();

        void Run();
        void RequestShutdown();
        void PushLayer(Layer* aLayer);
        void PushOverlay(Layer* aOverlayLayer);

        [[nodiscard]]
        bool IsRunning() const;

        EventManager* GetEventManager() const;
        di::Container& GetContainer();

    public:
        static Engine* GetInstance();
        static InputManager* GetInputManager();
        template <DerivedFromEngine T = Engine, typename... Args>
        static T* Reset(Args&&... args);

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

    Engine* CreateApplication();
}

#endif // SOGE_CORE_ENGINE_HPP
