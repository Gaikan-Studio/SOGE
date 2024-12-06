#include "GAME/Layers/MainGameLayer.hpp"
#include "GAME/AppEntryPoint.hpp"


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
        auto input = Game::GetInputManager();
        auto keyboard = input->GetKeyboard();

        if (keyboard->IsKeyPressed(soge::Keys::A))
        {
            SOGE_APP_INFO_LOG("Key pressed, yosh!");
        }

    }

    void MainGameLayer::OnFixedUpdate(float aDeltaTime)
    {
    }
}
