#include "GAME/AppEntryPoint.hpp"

#include "GAME/Layers/MainGameLayer.hpp"

#include <SOGE/AI/AiModule.hpp>
#include <SOGE/Core/EntryPoint.hpp>
#include <SOGE/Event/EventModule.hpp>
#include <SOGE/Event/InputEvents.hpp>
#include <SOGE/Graphics/Deferred/DeferredRenderGraph.hpp>
#include <SOGE/Graphics/Entities/AmbientLightEntity.hpp>
#include <SOGE/Graphics/Entities/DirectionalLightEntity.hpp>
#include <SOGE/Graphics/Entities/PointLightEntity.hpp>
#include <SOGE/Graphics/Entities/StaticMeshEntity.hpp>
#include <SOGE/Graphics/GraphicsModule.hpp>
#include <SOGE/Graphics/Primitives/Box.hpp>
#include <SOGE/Graphics/Primitives/Sphere.hpp>
#include <SOGE/Graphics/Resources/SimpleTextureResource.hpp>
#include <SOGE/Math/Camera.hpp>
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

        flecs::world world;
        {
            struct Agent
            {
            };
            struct Action
            {
            };
            struct Consideration
            {
                float m_score{};
            };
            const flecs::entity agentToAction = world.entity("Agent to action relation");
            const flecs::entity actionToAgent = world.entity("Action to agent relation");

            const flecs::entity agentToBestAction = world.entity("Agent to best action relation");
            const flecs::entity bestActionToAgent = world.entity("Best action to agent relation");

            const flecs::entity actionToConsideration = world.entity("Action to consideration relation");
            const flecs::entity considerationToAction = world.entity("Consideration to action relation");

            struct Thirst
            {
                float m_thirst{};
            };
            const flecs::entity agent = world.entity("Thirsty agent").add<Agent>().set(Thirst{.m_thirst = 0.25f});

            struct DrinkAction
            {
            };
            const flecs::entity drinkAction = world.entity("Drink water action").add<Action>().add<DrinkAction>();
            (void)agent.add(agentToAction, drinkAction);
            (void)drinkAction.add(actionToAgent, agent);

            struct DrinkConsideration
            {
            };
            const flecs::entity drinkConsideration =
                world.entity("Drink consideration").set(Consideration{}).add<DrinkConsideration>();
            (void)drinkAction.add(actionToConsideration, drinkConsideration);
            (void)drinkConsideration.add(considerationToAction, drinkAction);

            world.system<DrinkConsideration, Consideration>("Update drink consideration")
                .kind(flecs::PreUpdate)
                .with(considerationToAction, flecs::Wildcard)
                .each([considerationToAction, actionToAgent](const flecs::entity aConsideration, DrinkConsideration,
                                                             Consideration& aConsiderationData) {
                    SOGE_INFO_LOG(R"([UPD] Updating "{}" consideration...)", aConsideration.name().c_str());

                    const flecs::entity action = aConsideration.target(considerationToAction);
                    const flecs::entity agent = action.target_for<Thirst>(actionToAgent);

                    const auto& [thirst] = *agent.get<Thirst>();
                    aConsiderationData.m_score = thirst;
                });

            struct RunAction
            {
            };
            const flecs::entity runAction = world.entity("Run action").add<Action>().add<RunAction>();
            (void)agent.add(agentToAction, runAction);
            (void)runAction.add(actionToAgent, agent);

            struct RunConsideration
            {
            };
            const flecs::entity runConsideration =
                world.entity("Run consideration").set(Consideration{}).add<RunConsideration>();
            (void)runAction.add(actionToConsideration, runConsideration);
            (void)runConsideration.add(considerationToAction, runAction);

            world.system<RunConsideration, Consideration>("Update run consideration")
                .kind(flecs::PreUpdate)
                .with(considerationToAction, flecs::Wildcard)
                .each([considerationToAction, actionToAgent](const flecs::entity aConsideration, RunConsideration,
                                                             Consideration& aConsiderationData) {
                    SOGE_INFO_LOG(R"([UPD] Updating "{}" consideration...)", aConsideration.name().c_str());

                    const flecs::entity action = aConsideration.target(considerationToAction);
                    const flecs::entity agent = action.target_for<Thirst>(actionToAgent);

                    const auto& [thirst] = *agent.get<Thirst>();
                    aConsiderationData.m_score = 1.0f - thirst;
                });

            world.system<Agent>("Pick the best action for agent")
                .kind(flecs::OnUpdate)
                .with(agentToAction, flecs::Any)
                .each([=](const flecs::entity aAgent, Agent) {
                    SOGE_INFO_LOG(R"([PICK] Agent name is "{}")", aAgent.name().c_str());
                    (void)aAgent.remove(agentToBestAction, flecs::Wildcard);

                    flecs::entity bestAction;
                    decltype(Consideration::m_score) bestScore{};

                    std::int32_t index{};
                    while (flecs::entity action = aAgent.target(agentToAction, index++))
                    {
                        SOGE_INFO_LOG(R"([PICK] Action name is "{}")", action.name().c_str());
                        (void)action.remove(bestActionToAgent, flecs::Wildcard);

                        if (flecs::entity consideration = action.target_for<Consideration>(actionToConsideration))
                        {
                            const auto& [score] = *consideration.get<Consideration>();
                            SOGE_INFO_LOG(R"([PICK] Consideration "{}" score is {})", consideration.name().c_str(),
                                          score);
                            if (score > bestScore)
                            {
                                bestScore = score;
                                bestAction = action;
                            }
                        }
                    }

                    if (!bestAction)
                    {
                        return;
                    }
                    (void)aAgent.add(agentToBestAction, bestAction);
                    (void)bestAction.add(bestActionToAgent, aAgent);
                });

            world.system<Agent>("Print the best action for agent")
                .kind(flecs::OnUpdate)
                // .with(agentBestAction, flecs::Wildcard)
                .each([agentToBestAction](const flecs::entity aAgent, Agent) {
                    if (const flecs::entity bestAction = aAgent.target(agentToBestAction))
                    {
                        SOGE_INFO_LOG(R"([PICKED] Best action for agent "{}" is "{}")", aAgent.name().c_str(),
                                      bestAction.name().c_str());
                    }
                    else
                    {
                        SOGE_INFO_LOG(R"([PICKED] No best action for agent "{}")", aAgent.name().c_str());
                    }
                });

            world.system<DrinkAction>("Perform drink action")
                .kind(flecs::PostUpdate)
                .with(bestActionToAgent, flecs::Wildcard)
                .each([bestActionToAgent](const flecs::entity aAction, DrinkAction) {
                    const flecs::entity actionAgent = aAction.target(bestActionToAgent);
                    auto& [thirst] = *actionAgent.get_mut<Thirst>();

                    const auto prevThirst = thirst;
                    thirst = glm::max(prevThirst - 0.15f, 0.0f);
                    SOGE_INFO_LOG("[ACT] Drinking some water... thirst was {}, but now is {}", prevThirst, thirst);
                });

            world.system<RunAction>("Perform run action")
                .kind(flecs::PostUpdate)
                .with(bestActionToAgent, flecs::Wildcard)
                .each([bestActionToAgent](const flecs::entity aAction, RunAction) {
                    const flecs::entity actionAgent = aAction.target(bestActionToAgent);
                    auto& [thirst] = *actionAgent.get_mut<Thirst>();

                    const auto prevThirst = thirst;
                    thirst = glm::min(prevThirst + 0.1f, 1.0f);
                    SOGE_INFO_LOG("[ACT] Running somewhere... thirst was {}, but now is {}", prevThirst, thirst);
                });
        }

        const auto [window, windowUuid] = windowModule->CreateWindow();
        SOGE_INFO_LOG(R"(Created window "{}" of width {} and height {} with UUID {})",
                      soge::EAToNarrow(window.GetTitle()).c_str(), window.GetWidth(), window.GetHeight(),
                      windowUuid.str());

        graphicsModule->SetRenderTarget(window);

        auto& renderGraph = container.Provide<soge::DeferredRenderGraph>();
        graphicsModule->SetRenderGraph(renderGraph);

        const auto [texture, textureUuid] =
            graphicsModule->GetEntityManager().CreateEntity<soge::SimpleTextureResource>(
                container.Provide<soge::GraphicsCore>(), "cardboard", "./resources/textures/cardboard.png");
        SOGE_INFO_LOG(R"(Created texture with UUID {})", textureUuid.str());

        constexpr std::size_t gridSize = 3;
        constexpr glm::vec3 gridOffset{-0.5f * (gridSize - 1)};
        for (std::size_t i = 0; i < gridSize; ++i)
        {
            for (std::size_t j = 0; j < gridSize; ++j)
            {
                for (std::size_t k = 0; k < gridSize; ++k)
                {
                    const auto [box, boxUuid] = graphicsModule->GetEntityManager().CreateEntity<soge::BoxPrimitive>(
                        container.Provide<soge::BoxPrimitive>());
                    SOGE_INFO_LOG(R"(Created box ({}, {}, {}) with UUID {})", i, j, k, boxUuid.str());

                    const auto x = static_cast<float>(i);
                    const auto y = static_cast<float>(j);
                    const auto z = static_cast<float>(k);
                    box.GetTransform() = soge::Transform{
                        .m_position = glm::vec3{x, y, z} + gridOffset,
                        // .m_rotation = glm::quat{glm::vec3{0.0f, glm::radians(45.0f), 0.0f}},
                        .m_scale = glm::vec3{0.5f},
                    };
                    if ((i + j + k) % 2 == 0)
                    {
                        box.GetColorTexture() = texture.GetTextureResource();
                    }
                }
            }
        }

        const auto [sphere, sphereUuid] = graphicsModule->GetEntityManager().CreateEntity<soge::SpherePrimitive>(
            container.Provide<soge::SpherePrimitive>());
        SOGE_INFO_LOG(R"(Created sphere with UUID {})", sphereUuid.str());
        sphere.GetTransform() = soge::Transform{
            .m_position = glm::vec3{2.0f, 0.0f, 0.0f},
            .m_scale = glm::vec3{0.5f},
        };

        const auto [hog, hogUuid] = graphicsModule->GetEntityManager().CreateEntity<soge::StaticMeshEntity>(
            container.Provide<soge::StaticMeshEntity>());
        SOGE_INFO_LOG(R"(Created hog with UUID {})", hogUuid.str());
        hog.GetFilePath() = "./resources/meshes/hog.fbx";
        hog.GetTransform() = soge::Transform{
            .m_position = glm::vec3{0.0f, -1.0f, 4.0f},
            .m_rotation = glm::vec3{0.0f, glm::radians(180.0f), 0.0f},
            .m_scale = glm::vec3{0.01f},
        };
        hog.Load();

        // const auto [cake, cakeUuid] = graphicsModule->GetEntityManager().CreateEntity<soge::StaticMeshEntity>(
        //     container.Provide<soge::StaticMeshEntity>());
        // SOGE_INFO_LOG(R"(Created cake with UUID {})", cakeUuid.str());
        // cake.GetFilePath() = "./resources/meshes/cake/cake.fbx";
        // cake.GetTransform() = soge::Transform{
        //     .m_position = glm::vec3{0.0f, -2.5f, 0.0f},
        //     .m_rotation = glm::vec3{0.0f, glm::radians(180.0f), 0.0f},
        //     .m_scale = glm::vec3{0.01f},
        // };
        // cake.Load();

        // const auto [boat, boatUuid] = graphicsModule->GetEntityManager().CreateEntity<soge::StaticMeshEntity>(
        //     container.Provide<soge::StaticMeshEntity>());
        // SOGE_INFO_LOG(R"(Created boat with UUID {})", boatUuid.str());
        // boat.GetFilePath() = "./resources/meshes/boat/boat.fbx";
        // boat.GetTransform() = soge::Transform{
        //     .m_position = glm::vec3{-2.5f, 0.0f, 0.0f},
        //     .m_rotation = glm::vec3{glm::radians(90.0f), 0.0f, glm::radians(-90.0f)},
        //     .m_scale = glm::vec3{0.005f},
        // };
        // boat.Load();

        const auto [cheese, cheeseUuid] = graphicsModule->GetEntityManager().CreateEntity<soge::StaticMeshEntity>(
            container.Provide<soge::StaticMeshEntity>());
        SOGE_INFO_LOG(R"(Created cheese with UUID {})", cheeseUuid.str());
        cheese.GetFilePath() = "./resources/meshes/cheese/cheese.fbx";
        cheese.GetTransform() = soge::Transform{
            .m_position = glm::vec3{0.0f, 0.0f, 7.0f},
            // .m_rotation = glm::vec3{glm::radians(90.0f), 0.0f, glm::radians(-90.0f)},
            .m_scale = glm::vec3{0.1f},
        };
        cheese.Load();

        const auto [axe, axeUuid] = graphicsModule->GetEntityManager().CreateEntity<soge::StaticMeshEntity>(
            container.Provide<soge::StaticMeshEntity>());
        SOGE_INFO_LOG(R"(Created axe with UUID {})", axeUuid.str());
        axe.GetFilePath() = "./resources/meshes/axe/axe.fbx";
        axe.GetTransform() = soge::Transform{
            .m_position = glm::vec3{0.0f, 0.0f, 10.0f},
            .m_rotation = glm::vec3{0.0f, 0.0f, glm::radians(90.0f)},
            .m_scale = glm::vec3{0.05f},
        };
        axe.Load();

        const auto [ambientLightEntity1, ambientLightEntityUuid1] =
            graphicsModule->GetEntityManager().CreateEntity<soge::AmbientLightEntity>(
                container.Provide<soge::AmbientLightEntity>());
        SOGE_INFO_LOG(R"(Created ambient light entity with UUID {})", ambientLightEntityUuid1.str());
        ambientLightEntity1.GetIntensity() = 0.01f;

        const auto [directionalLightEntity1, directionalLightEntityUuid1] =
            graphicsModule->GetEntityManager().CreateEntity<soge::DirectionalLightEntity>(
                container.Provide<soge::DirectionalLightEntity>());
        SOGE_INFO_LOG(R"(Created directional light entity with UUID {})", directionalLightEntityUuid1.str());
        const soge::Transform directionalLightTransform1{
            .m_rotation = glm::vec3{glm::radians(45.0f), glm::radians(45.0f), 0.0f},
        };
        directionalLightEntity1.GetDirection() = directionalLightTransform1.Forward();

        const auto [pointLightEntity1, pointLightEntityUuid1] =
            graphicsModule->GetEntityManager().CreateEntity<soge::PointLightEntity>(
                container.Provide<soge::PointLightEntity>());
        SOGE_INFO_LOG(R"(Created point light entity with UUID {})", pointLightEntityUuid1.str());
        pointLightEntity1.GetAttenuation().m_linearFactor = glm::vec3{1.0f};
        pointLightEntity1.GetAttenuation().m_quadraticFactor = glm::vec3{100.0f};

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
        constexpr float cameraSpeed = 1.0f;
        constexpr float cameraSensitivity = 0.005f;
        auto update = [=, &camera, &directionalLightEntity1](const soge::UpdateEvent& aEvent) mutable {
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

            (void)world.progress(aEvent.GetDeltaTime());
        };
        eventModule->PushBack<soge::UpdateEvent>(update);
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<soge_game::Game>();
}
