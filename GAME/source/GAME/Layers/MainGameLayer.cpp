#include "GAME/Layers/MainGameLayer.hpp"


namespace soge_game
{
    MainGameLayer::MainGameLayer() : Layer("MainGameLayer")
    {
        SOGE_INFO_LOG("Global engine instance pointer is {}", static_cast<void*>(soge::Engine::GetInstance()));
    }

    void MainGameLayer::OnAttach()
    {
        SOGE_APP_INFO_LOG(R"(Layer "{}" attached...)", m_layerName.c_str());
    }

    void MainGameLayer::OnDetach()
    {
        SOGE_APP_INFO_LOG(R"(Layer "{}" detached...)", m_layerName.c_str());
    }

    void MainGameLayer::OnUpdate()
    {
        if (m_inputModule->IsKeyPressed(soge::Keys::W))
        {
            SOGE_APP_INFO_LOG("Key W pressed!");
        }
        else if (m_inputModule->IsKeyPressed(soge::Keys::Escape))
        {
            SOGE_APP_INFO_LOG("Key Escape pressed - shutting down!");
            soge::Engine::GetInstance()->RequestShutdown();
        }
    }

    void MainGameLayer::OnFixedUpdate(float aDeltaTime)
    {
    }
}
