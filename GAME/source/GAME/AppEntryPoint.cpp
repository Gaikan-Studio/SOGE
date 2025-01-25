#include "GAME/AppEntryPoint.hpp"

#include "GAME/Layers/MainGameLayer.hpp"

#include <SOGE/AI/AiModule.hpp>
#include <SOGE/Core/EntryPoint.hpp>
#include <SOGE/Event/EventModule.hpp>
#include <SOGE/Event/InputEvents.hpp>
#include <SOGE/Graphics/Deferred/DeferredRenderGraph.hpp>
#include <SOGE/Graphics/Entities/AmbientLightEntity.hpp>
#include <SOGE/Graphics/Entities/DirectionalLightEntity.hpp>
#include <SOGE/Graphics/Entities/StaticMeshEntity.hpp>
#include <SOGE/Graphics/GraphicsModule.hpp>
#include <SOGE/Math/Camera.hpp>
#include <SOGE/Math/Random/Random.hpp>
#include <SOGE/Window/WindowModule.hpp>

#undef CreateWindow


namespace soge_game
{
    Game::Game(AccessTag&& aTag) : Engine(std::move(aTag))
    {
        SOGE_APP_INFO_LOG("Initialize game...");
    }

    Game::~Game()
    {
        SOGE_APP_INFO_LOG("Destroy game...");

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void Game::Load(AccessTag)
    {
        PushLayer(new MainGameLayer());

        auto& container = GetDependencyContainer();
        const auto eventModule = GetModule<soge::EventModule>();
        const auto inputModule = GetModule<soge::InputModule>();
        const auto windowModule = GetModule<soge::WindowModule>();
        const auto graphicsModule = GetModule<soge::GraphicsModule>();
        const auto soundModule = GetModule<soge::SoundModule>();
        const auto aiModule = GetModule<soge::AiModule>();

        soge::Random random;

        using Consideration = soge::AiModule::Consideration;

        struct Thirst
        {
            float m_thirst{};
            float m_timeBeforeNextDrink{};
            bool m_wasRunning{};
        };

        struct DrinkAction
        {
        };

        struct RunAction
        {
        };

        struct DrinkConsideration
        {
        };

        struct RunConsideration
        {
        };

        auto agent = aiModule->CreateAgent("Thirsty agent");
        agent.GetEntity().set(Thirst{.m_thirst = 0.25f});

        auto drinkAction = aiModule->CreateAction("Drink water action");
        (void)drinkAction.GetEntity().add<DrinkAction>();

        aiModule->AttachActionToAgent(drinkAction, agent);

        auto drinkConsideration = aiModule->CreateConsideration("Drink consideration");
        (void)drinkConsideration.GetEntity().add<DrinkConsideration>();

        aiModule->AttachConsiderationToAction(drinkConsideration, drinkAction);

        auto runAction = aiModule->CreateAction("Run action");
        (void)runAction.GetEntity().add<RunAction>();

        aiModule->AttachActionToAgent(runAction, agent);

        auto runConsideration = aiModule->CreateConsideration("Run consideration");
        (void)runConsideration.GetEntity().add<RunConsideration>();

        aiModule->AttachConsiderationToAction(runConsideration, runAction);

        // auto agent2 = aiModule->CreateAgent("Thirsty agent 2");
        // agent2.GetEntity().set(Thirst{.m_thirst = 0.75f});
        //
        // auto drinkAction2 = aiModule->CreateAction("Drink water action 2");
        // (void)drinkAction2.GetEntity().add<DrinkAction>();
        //
        // aiModule->AttachActionToAgent(drinkAction2, agent2);
        //
        // auto drinkConsideration2 = aiModule->CreateConsideration("Drink consideration 2");
        // (void)drinkConsideration2.GetEntity().add<DrinkConsideration>();
        //
        // aiModule->AttachConsiderationToAction(drinkConsideration2, drinkAction2);
        //
        // auto runAction2 = aiModule->CreateAction("Run action 2");
        // (void)runAction2.GetEntity().add<RunAction>();
        //
        // aiModule->AttachActionToAgent(runAction2, agent2);
        //
        // auto runConsideration2 = aiModule->CreateConsideration("Run consideration 2");
        // (void)runConsideration2.GetEntity().add<RunConsideration>();
        //
        // aiModule->AttachConsiderationToAction(runConsideration2, runAction2);

        aiModule->CreateConsiderationSystem<DrinkConsideration>("Update drink consideration")
            .each([aiModule](const flecs::entity aEntity, Consideration& aConsideration, DrinkConsideration) {
                auto consideration = aiModule->GetConsideration(aEntity);
                SOGE_INFO_LOG(R"([UPD] Updating "{}" consideration...)", consideration->GetName().data());

                auto action = aiModule->GetAttachedAction(*consideration);
                auto agent = aiModule->GetAttachedAgent(*action);

                const auto& [thirst, timeBeforeNextDrink, wasRunning] = *agent->GetEntity().get<Thirst>();
                aConsideration.m_score = timeBeforeNextDrink <= 0.0f ? thirst : 0.0f;
            });

        aiModule->CreateConsiderationSystem<RunConsideration>("Update run consideration")
            .each([aiModule](const flecs::entity aEntity, Consideration& aConsideration, RunConsideration) {
                auto consideration = aiModule->GetConsideration(aEntity);
                SOGE_INFO_LOG(R"([UPD] Updating "{}" consideration...)", consideration->GetName().data());

                auto action = aiModule->GetAttachedAction(*consideration);
                auto agent = aiModule->GetAttachedAgent(*action);

                const auto& [thirst, timeBeforeNextDrink, wasRunning] = *agent->GetEntity().get<Thirst>();
                aConsideration.m_score = thirst < 0.3f ? 1.0f - thirst : 0.0f;
            });

        aiModule->CreateActionSystem<DrinkAction>("Perform drink action")
            .each([aiModule, &random](const flecs::entity aEntity, DrinkAction) {
                auto action = aiModule->GetAction(aEntity);
                auto agent = aiModule->GetAgentFromBest(*action);

                auto& [thirst, timeBeforeNextDrink, wasRunning] = *agent->GetEntity().get_mut<Thirst>();
                const auto prevThirst = thirst;
                thirst = glm::max(prevThirst - random.GenFloat(0.4f, 0.6f), 0.0f);
                timeBeforeNextDrink = 3.0f;
                SOGE_INFO_LOG(R"([ACT] "{}" is drinking some water... thirst was {}, but now is {})",
                              agent->GetName().data(), prevThirst, thirst);
            });

        aiModule->CreateActionSystem<RunAction>("Perform run action")
            .each([aiModule, &random](const flecs::iter& aIter, const std::size_t aIndex, RunAction) {
                const auto entity = aIter.entity(aIndex);
                auto action = aiModule->GetAction(entity);
                auto agent = aiModule->GetAgentFromBest(*action);

                auto& [thirst, timeBeforeNextDrink, wasRunning] = *agent->GetEntity().get_mut<Thirst>();
                const auto prevThirst = thirst;
                thirst = glm::min(prevThirst + random.GenFloat(0.3f, 0.4f) * aIter.delta_time(), 1.0f);
                wasRunning = true;
                SOGE_INFO_LOG(R"([ACT] "{}" is running somewhere... thirst was {}, but now is {})",
                              agent->GetName().data(), prevThirst, thirst);
            });

        const auto [window, windowUuid] = windowModule->CreateWindow();
        SOGE_INFO_LOG(R"(Created window "{}" of width {} and height {} with UUID {})",
                      soge::EAToNarrow(window.GetTitle()).c_str(), window.GetWidth(), window.GetHeight(),
                      windowUuid.str());

        graphicsModule->SetRenderTarget(window);

        auto& renderGraph = container.Provide<soge::DeferredRenderGraph>();
        graphicsModule->SetRenderGraph(renderGraph);

        const auto [human, humanUuid] = graphicsModule->GetEntityManager().CreateEntity<soge::StaticMeshEntity>(
            container.Provide<soge::StaticMeshEntity>());
        SOGE_INFO_LOG(R"(Created human with UUID {})", humanUuid.str());
        human.GetFilePath() = "./resources/meshes/cartoon-human.fbx";
        human.GetTransform() = soge::Transform{
            .m_position = glm::vec3{0.0f, -0.75f, 0.0f},
            .m_rotation = glm::vec3{0.0f, glm::radians(180.0f), 0.0f},
            .m_scale = glm::vec3{0.001f},
        };
        human.Load();

        const auto [ambientLightEntity1, ambientLightEntityUuid1] =
            graphicsModule->GetEntityManager().CreateEntity<soge::AmbientLightEntity>(
                container.Provide<soge::AmbientLightEntity>());
        SOGE_INFO_LOG(R"(Created ambient light entity with UUID {})", ambientLightEntityUuid1.str());
        ambientLightEntity1.GetIntensity() = 0.1f;

        const auto [directionalLightEntity1, directionalLightEntityUuid1] =
            graphicsModule->GetEntityManager().CreateEntity<soge::DirectionalLightEntity>(
                container.Provide<soge::DirectionalLightEntity>());
        SOGE_INFO_LOG(R"(Created directional light entity with UUID {})", directionalLightEntityUuid1.str());
        const soge::Transform directionalLightTransform1{
            .m_rotation = glm::vec3{glm::radians(45.0f), glm::radians(45.0f), 0.0f},
        };
        directionalLightEntity1.GetDirection() = directionalLightTransform1.Forward();

        const auto [camera, cameraUuid] = graphicsModule->GetCameraManager().CreateCamera({
            .m_width = static_cast<float>(window.GetWidth()),
            .m_height = static_cast<float>(window.GetHeight()),
            .m_nearPlane = 0.01f,
            .m_farPlane = 100.0f,
            .m_transform = soge::Transform{.m_position = glm::vec3{0.0f, 0.0f, -2.0f}},
            .m_projection = soge::CreateUnique<soge::PerspectiveProjection>(glm::radians(60.0f)),
        });
        SOGE_INFO_LOG(R"(Created camera with UUID {})", cameraUuid.str());

        const auto [viewport, viewportUuid] = graphicsModule->GetViewportManager().CreateViewport({
            .m_viewport = {static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight())},
            .m_cameraId = cameraUuid,
        });
        SOGE_INFO_LOG(R"(Created viewport with UUID {})", viewportUuid.str());

        const auto soundMixer = soundModule->GetChannelMixer();

        constexpr auto ambientSoundChannelName = "Ambient";
        soundMixer->CreateChannel(ambientSoundChannelName);

        constexpr auto effectSoundChannelName = "Effect";
        soundMixer->CreateChannel(effectSoundChannelName);

        const auto ambientSound =
            soundModule->CreateSoundResource("Ambient sound", "./resources/sounds/Sea waves and seagulls.wav", false);
        soundModule->LoadSoundResource(ambientSound);

        const auto metalBarFallingSound =
            soundModule->CreateSoundResource("Metal bar falling sound", "./resources/sounds/metal-bar.mp3", true);
        soundModule->LoadSoundResource(metalBarFallingSound);

        // share state between two lambdas
        auto cameraMouseDeltaX = soge::CreateShared<float>(0.0f);
        auto cameraMouseDeltaY = soge::CreateShared<float>(0.0f);
        auto lightMouseDeltaX = soge::CreateShared<float>(0.0f);
        auto lightMouseDeltaY = soge::CreateShared<float>(0.0f);
        auto mouseMoved = [cameraMouseDeltaX, cameraMouseDeltaY, lightMouseDeltaX, lightMouseDeltaY,
                           inputModule](const soge::MouseMovedEvent& aEvent) {
            if (inputModule->IsKeyPressed(soge::Keys::LeftMouseButton))
            {
                *cameraMouseDeltaX = aEvent.GetXOffset();
                *cameraMouseDeltaY = aEvent.GetYOffset();
            }
            if (inputModule->IsKeyPressed(soge::Keys::RightMouseButton))
            {
                *lightMouseDeltaX = aEvent.GetXOffset();
                *lightMouseDeltaY = aEvent.GetYOffset();
            }
        };
        eventModule->PushBack<soge::MouseMovedEvent>(mouseMoved);

        auto mouseWheel = [&directionalLightEntity1](const soge::MouseWheelEvent& aEvent) {
            directionalLightEntity1.GetColor().r += aEvent.GetXOffset() * 0.1f;
            directionalLightEntity1.GetColor().r = glm::max(directionalLightEntity1.GetColor().r, 1.0f);
        };
        eventModule->PushBack<soge::MouseWheelEvent>(mouseWheel);

        bool ambientSoundFlag = false;
        auto soundUpdate = [soundMixer, ambientSound, metalBarFallingSound,
                            ambientSoundFlag](const soge::KeyPressedEvent& aEvent) mutable {
            if (aEvent.GetKey() == soge::Keys::T)
            {
                ambientSoundFlag = !ambientSoundFlag;
                if (ambientSoundFlag)
                {
                    soundMixer->PlayOnChannel(ambientSoundChannelName, ambientSound);
                }
                else
                {
                    soundMixer->StopChannel(ambientSoundChannelName);
                }
            }
            else if (aEvent.GetKey() == soge::Keys::E)
            {
                soundMixer->PlayOnChannel(effectSoundChannelName, metalBarFallingSound);
            }
        };
        eventModule->PushBack<soge::KeyPressedEvent>(soundUpdate);

        float cameraPitch{}, cameraYaw{};
        float lightPitch{glm::radians(45.0f)}, lightYaw{glm::radians(45.0f)};
        constexpr float cameraSpeed = 1.5f;
        constexpr float cameraSensitivity = 0.005f;
        auto update = [=, &human, &camera, &directionalLightEntity1](const soge::UpdateEvent& aEvent) mutable {
            {
                const float x = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::D)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::A));
                const float y = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::SpaceBar)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::LeftShift));
                const float z = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::W)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::S));
                const auto direction =
                    camera.m_transform.Right() * x + camera.m_transform.Up() * y + camera.m_transform.Forward() * z;
                camera.m_transform.m_position += direction * cameraSpeed * aEvent.GetDeltaTime();
            }

            cameraYaw += *cameraMouseDeltaX * cameraSensitivity;
            cameraPitch += *cameraMouseDeltaY * cameraSensitivity;
            camera.m_transform.m_rotation = glm::vec3{cameraPitch, cameraYaw, 0.0f};

            if (*lightMouseDeltaX != 0.0f || *lightMouseDeltaY != 0.0f)
            {
                lightYaw += *lightMouseDeltaX * cameraSensitivity;
                lightPitch += *lightMouseDeltaY * cameraSensitivity;
                directionalLightEntity1.GetDirection() =
                    soge::Transform{.m_rotation = glm::vec3{lightPitch, lightYaw, 0.0f}}.Forward();
            }

            *cameraMouseDeltaX = 0.0f;
            *cameraMouseDeltaY = 0.0f;
            *lightMouseDeltaX = 0.0f;
            *lightMouseDeltaY = 0.0f;

            auto& [thirst, timeBeforeNextDrink, wasRunning] = *agent.GetEntity().get_mut<Thirst>();
            thirst = glm::min(thirst + 0.1f * aEvent.GetDeltaTime(), 1.0f);
            timeBeforeNextDrink = glm::max(timeBeforeNextDrink - aEvent.GetDeltaTime(), 0.0f);
            SOGE_INFO_LOG("Agent thirst is {}, time before next drink is {}, if agent was running recently? {}", thirst,
                          timeBeforeNextDrink, wasRunning);

            human.GetColor() = glm::vec3{wasRunning ? 1.0f : 0.0f, 1.0f - thirst, 0.0f};
            wasRunning = false;
        };
        eventModule->PushBack<soge::UpdateEvent>(update);
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<soge_game::Game>();
}
