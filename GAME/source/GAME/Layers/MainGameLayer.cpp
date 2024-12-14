#include "GAME/Layers/MainGameLayer.hpp"


namespace soge_game
{
    MainGameLayer::MainGameLayer() : Layer("MainGameLayer")
    {
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
        const auto engine = soge::Engine::GetInstance();
        const auto inputModule = engine->GetModule<soge::InputModule>();
        if (inputModule == nullptr)
        {
            return;
        }

        if (inputModule->IsKeyPressed(soge::Keys::W))
        {
            SOGE_APP_INFO_LOG("Key W pressed...");
        }
        else if (inputModule->IsKeyPressed(soge::Keys::Escape))
        {
            SOGE_APP_INFO_LOG("Key Escape pressed: shutting down...");
            engine->RequestShutdown();
        }
    }

    void MainGameLayer::OnFixedUpdate(float aDeltaTime)
    {
    }
}
