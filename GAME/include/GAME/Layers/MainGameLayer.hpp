#ifndef GAME_LAYERS_MAINGAMELAYER_HPP
#define GAME_LAYERS_MAINGAMELAYER_HPP

#include <SOGE/SOGE.hpp>


namespace soge_game
{
    class MainGameLayer final : public soge::Layer
    {
    private:


    public:
        MainGameLayer();
        ~MainGameLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnFixedUpdate(float aDeltaTime) override;

    };
}

#endif // GAME_LAYERS_MAINGAMELAYER_HPP
