#ifndef GAME_APPENTRYPOINT_HPP
#define GAME_APPENTRYPOINT_HPP

#include <SOGE/SOGE.hpp>


namespace soge_game
{
    class Game final : public soge::Engine
    {
        using parentClass = soge::Engine;

    public:
        Game();
        ~Game() override;
    };
}

#endif // GAME_APPENTRYPOINT_HPP
