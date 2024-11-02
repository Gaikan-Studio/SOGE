#ifndef SOGE_CORE_ENGINE_HPP
#define SOGE_CORE_ENGINE_HPP

#include "SOGE/System/Memory.hpp"
#include "SOGE/Core/BaseObject.hpp"


namespace soge
{
    class Engine;

    class CoolObject : public BaseObject
    {
        RTTR_ENABLE(BaseObject)

    public:
        CoolObject() = default;
        ~CoolObject() = default;

    };

    class AwesomeObject : public CoolObject
    {
        RTTR_ENABLE(CoolObject)

    public:

    };

    template <typename T>
    concept DerivedFromEngine = std::is_base_of_v<Engine, T>;

    class Engine
    {
    private:
        static UniquePtr<Engine> s_instance;
        static std::mutex s_initMutex;

        bool m_isRunning;

    protected:
        explicit Engine();

        void Shutdown();

    public:
        Engine(const Engine&) = delete;
        auto operator=(const Engine&) = delete;

        Engine(Engine&&) = delete;
        auto operator=(Engine&&) = delete;

        virtual ~Engine();

        static Engine* GetInstance();

        template <DerivedFromEngine T = Engine, typename... Args>
        static T* Reset(Args&&... args);

        void Run();
        void Update();
        void FixedUpdate();
        void RequestShutdown();
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
        std::lock_guard lock(s_initMutex);
        // Move new instance to the static instance
        s_instance = std::move(newInstance);
        // Return previously saved pointer
        return pNewInstance;
    }

    Engine* CreateApplication();
}

#endif // SOGE_CORE_ENGINE_HPP
