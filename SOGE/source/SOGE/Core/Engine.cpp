#include "sogepch.hpp"

#include "SOGE/Core/Engine.hpp"
#include "SOGE/Core/Timestep.hpp"


namespace soge
{
    Engine* Engine::s_instance = nullptr;
    Engine* Engine::GetInstance()
    {
        if (s_instance == nullptr)
        {
            s_instance = new Engine();
        }

        return s_instance;
    }

    Engine::Engine()
    {
        SOGE_INFO_LOG("Initialize engine...");

        m_isRunning = false;
    }

    void Engine::Run()
    {
        m_isRunning = true;
        while (m_isRunning)
        {
            Timestep::StartFrame();
            Timestep::CalculateDelta();

        }

        this->Shutdown();
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

    void Engine::HandleCLIArgs(int argc, char** argv)
    {
        m_cliManager.ParseArgs(argc, argv);
    }

    void Engine::Shutdown()
    {
    }
}
