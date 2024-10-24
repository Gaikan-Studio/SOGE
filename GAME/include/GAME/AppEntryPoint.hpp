#ifndef GAME_APPENTRYPOINT_HPP
#define GAME_APPENTRYPOINT_HPP

#include <SOGE/SOGE.hpp>
#include <SOGE/Core/EntryPoint.hpp>


namespace game
{
    class Game final : public soge::Engine
    {
    public:
        Game();
        ~Game() override;
    };
}

#endif // GAME_APPENTRYPOINT_HPP
