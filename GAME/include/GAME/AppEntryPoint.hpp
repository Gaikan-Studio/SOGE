#ifndef GAME_APPENTRYPOINT_HPP
#define GAME_APPENTRYPOINT_HPP

#include <SOGE/SOGE.hpp>


namespace soge_game
{
    class Game final : public soge::Engine
    {
    public:
        explicit Game(AccessTag);

        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;

        Game(Game&&) = delete;
        Game& operator=(Game&&) = delete;

        ~Game() override;

        void Load(AccessTag) override;
    };
}

#endif // GAME_APPENTRYPOINT_HPP
