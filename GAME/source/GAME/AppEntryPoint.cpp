#include "GAME/AppEntryPoint.hpp"


namespace game
{
    Game::Game()
    {
        SOGE_APP_INFO_LOG("Initialize game...");
    }

    Game::~Game()
    {
        SOGE_APP_INFO_LOG("Destroy game...");
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<game::Game>();
}
