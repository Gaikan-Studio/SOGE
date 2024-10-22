#ifndef SOGE_CORE_ENGINE_HPP
#define SOGE_CORE_ENGINE_HPP

#include "SOGE/Core/CLIManager.hpp"


namespace soge
{
    class Engine
    {
    private:
        bool m_isRunning;
        CLIManager m_cliManager;

    protected:
        Engine();
        static Engine* s_instance;

        void Shutdown();

    public:
        Engine(Engine&) = delete;
        auto operator=(Engine&) = delete;

        void Run();
        void Update();
        void FixedUpdate();
        void RequestShutdown();

        void HandleCLIArgs(int argc, char** argv);

    public:
        static Engine* GetInstance();
    };

    Engine* CreateApplication();
}

#endif // SOGE_CORE_ENGINE_HPP
