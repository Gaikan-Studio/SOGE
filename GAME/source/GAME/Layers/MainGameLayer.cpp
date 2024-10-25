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
        SOGE_APP_INFO_LOG("Delta time: {0}", soge::Timestep::RealDeltaTime());
        //SOGE_APP_INFO_LOG("OnUpdate was called in GAME");
    }

    void MainGameLayer::OnFixedUpdate(float aDeltaTime)
    {
    }
}
