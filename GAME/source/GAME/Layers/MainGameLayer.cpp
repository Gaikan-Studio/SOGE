#include "GAME/Layers/MainGameLayer.hpp"


namespace soge_game
{
    MainGameLayer::MainGameLayer() : soge::Layer("MainGameLayer")
    {
    }

    MainGameLayer::~MainGameLayer()
    {
    }

    void MainGameLayer::OnAttach()
    {
        SOGE_APP_INFO_LOG("Layer {0} attached", this->m_layerName.c_str());
    }

    void MainGameLayer::OnDetach()
    {
    }

    void MainGameLayer::OnUpdate()
    {
        const auto engine = soge::Engine::GetInstance();
        if (const auto inputModule = engine->GetModule<soge::InputModule>(); inputModule->IsKeyPressed(soge::Keys::W))
        {
            SOGE_APP_INFO_LOG("Key W pressed!");
        }
        else if (inputModule->IsKeyPressed(soge::Keys::Escape))
        {
            SOGE_APP_INFO_LOG("Key Escape pressed - shutting down!");
            engine->RequestShutdown();
        }
    }

    void MainGameLayer::OnFixedUpdate(float aDeltaTime)
    {
    }
}
