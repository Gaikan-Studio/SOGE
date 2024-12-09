#ifndef GAME_LAYERS_MAINGAMELAYER_HPP
#define GAME_LAYERS_MAINGAMELAYER_HPP

#include <SOGE/SOGE.hpp>


namespace soge_game
{
    class MainGameLayer final : public soge::Layer
    {
    private:
    public:
        explicit MainGameLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnFixedUpdate(float aDeltaTime) override;
    };
}

#endif // GAME_LAYERS_MAINGAMELAYER_HPP
