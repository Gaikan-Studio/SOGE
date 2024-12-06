#include "GAME/AppEntryPoint.hpp"
#include "GAME/Layers/MainGameLayer.hpp"

#include <SOGE/Core/EntryPoint.hpp>


namespace soge_game
{
    Game::Game()
    {
        SOGE_APP_INFO_LOG("Initialize game...");
        parentClass::PushLayer(new MainGameLayer());
    }

    Game::~Game()
    {
        SOGE_APP_INFO_LOG("Destroy game...");
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<soge_game::Game>();
}
