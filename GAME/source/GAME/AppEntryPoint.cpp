#include "GAME/AppEntryPoint.hpp"

#include "GAME/Layers/MainGameLayer.hpp"

#include <SOGE/Core/EntryPoint.hpp>
#include <SOGE/Event/EventModule.hpp>
#include <SOGE/Event/InputEvents.hpp>


namespace soge_game
{
    Game::Game(const AccessTag aTag) : Engine(aTag)
    {
        SOGE_APP_INFO_LOG("Initialize game...");

        // auto& eventManager = GetDependencyProvider().Provide<soge::EventModule>();

        // eventManager.PushBack<soge::KeyPressedEvent>([](const soge::KeyPressedEvent& aEvent) {
        //     SOGE_APP_INFO_LOG(R"(Key "{}" pressed {} times)", aEvent.GetKey().ToCString(), aEvent.GetRepeatCount());
        // });
        // eventManager.PushBack<soge::KeyReleasedEvent>([](const soge::KeyReleasedEvent& aEvent) {
        //     SOGE_APP_INFO_LOG(R"(Key "{}" released)", aEvent.GetKey().ToCString());
        // });

        // eventManager.PushBack<soge::MouseButtonPressedEvent>([](const soge::MouseButtonPressedEvent& aEvent) {
        //     const auto key = aEvent.GetKey();
        //     SOGE_APP_INFO_LOG(R"(Mouse button "{}" pressed {} times)", key.ToCString(), aEvent.GetRepeatCount());
        // });
        // eventManager.PushBack<soge::MouseButtonReleasedEvent>([](const soge::MouseButtonReleasedEvent& aEvent) {
        //     SOGE_APP_INFO_LOG(R"(Mouse button "{}" released)", aEvent.GetKey().ToCString());
        // });

        // eventManager.PushBack<soge::MouseMovedEvent>([](const soge::MouseMovedEvent& aEvent) {
        //     SOGE_APP_INFO_LOG(R"(Mouse position is ({}, {}))", aEvent.GetRelativeX(), aEvent.GetRelativeY());
        // });
        // eventManager.PushBack<soge::MouseWheelEvent>([](const soge::MouseWheelEvent& aEvent) {
        //     SOGE_APP_INFO_LOG(R"(Mouse wheel changed by ({}, {}))", aEvent.GetXOffset(), aEvent.GetYOffset());
        // });
    }

    Game::~Game()
    {
        SOGE_APP_INFO_LOG("Destroy game...");

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void Game::Load(AccessTag)
    {
        PushLayer(new MainGameLayer());
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<soge_game::Game>();
}
