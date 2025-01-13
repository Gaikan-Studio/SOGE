#include "GAME/AppEntryPoint.hpp"

#include "GAME/Layers/MainGameLayer.hpp"

#include <SOGE/Core/EntryPoint.hpp>
#include <SOGE/Event/EventModule.hpp>
#include <SOGE/Event/InputEvents.hpp>
#include <SOGE/Graphics/GraphicsModule.hpp>
#include <SOGE/Graphics/SimpleRenderGraph.hpp>
#include <SOGE/Graphics/TriangleEntity.hpp>
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

        const auto [window, windowUuid] = windowModule->CreateWindow();
        SOGE_INFO_LOG(R"(Created window "{}" of width {} and height {} with UUID {})",
                      soge::EAToNarrow(window.GetTitle()).c_str(), window.GetWidth(), window.GetHeight(),
                      windowUuid.str());

        graphicsModule->SetRenderTarget(window);

        auto& renderGraph = container.Provide<soge::SimpleRenderGraph>();
        graphicsModule->SetRenderGraph(renderGraph);

        const auto [entity, entityUuid] = graphicsModule->GetEntityManager().CreateEntity<soge::TriangleEntity>(
            container.Provide<soge::TriangleEntity>());
        SOGE_INFO_LOG(R"(Created graphics triangle entity with UUID {})", entityUuid.str());

        constexpr std::array vertices{
            soge::TriangleEntity::Vertex{
                .m_position = glm::vec3{-0.5f, 0.5f, 0.0f},
                .m_color = glm::vec4{0.0f, 0.0f, 1.0f, 1.0f},
            },
            soge::TriangleEntity::Vertex{
                .m_position = glm::vec3{0.5f, 0.5f, 0.0f},
                .m_color = glm::vec4{0.0f, 1.0f, 0.0f, 1.0f},
            },
            soge::TriangleEntity::Vertex{
                .m_position = glm::vec3{0.0f, -0.5f, 0.0f},
                .m_color = glm::vec4{1.0f, 0.0f, 0.0f, 1.0f},
            },
        };
        entity.UpdateVertices(vertices);

        soge::Transform transform{};
        // share state with lambda (it should be copyable)
        const auto camera = soge::CreateShared<soge::Camera>(soge::Camera{
            .m_width = static_cast<float>(window.GetWidth()),
            .m_height = static_cast<float>(window.GetHeight()),
            .m_nearPlane = 0.01f,
            .m_farPlane = 100.0f,
            .m_transform = soge::Transform{.m_position = glm::vec3{0.0f, 0.0f, -1.0f}},
            .m_projection = soge::CreateUnique<soge::PerspectiveProjection>(glm::radians(60.0f)),
        });

        // share state between two lambdas
        auto mouseDeltaX = soge::CreateShared<float>(0.0f);
        auto mouseDeltaY = soge::CreateShared<float>(0.0f);
        auto mouseMoved = [mouseDeltaX, mouseDeltaY, inputModule](const soge::MouseMovedEvent& aEvent) {
            if (!inputModule->IsKeyPressed(soge::Keys::LeftMouseButton))
            {
                return;
            }
            *mouseDeltaX = aEvent.GetXOffset();
            *mouseDeltaY = aEvent.GetYOffset();
        };
        eventModule->PushBack<soge::MouseMovedEvent>(mouseMoved);

        float pitch{}, yaw{};
        auto update = [=, &entity](const soge::UpdateEvent& aEvent) mutable {
            {
                const float x = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::D)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::A));
                const float y = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::SpaceBar)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::LeftShift));
                const float z = static_cast<float>(inputModule->IsKeyPressed(soge::Keys::W)) -
                                static_cast<float>(inputModule->IsKeyPressed(soge::Keys::S));
                const auto direction =
                    camera->m_transform.Right() * x + camera->m_transform.Up() * y + camera->m_transform.Forward() * z;
                camera->m_transform.m_position += direction * aEvent.GetDeltaTime();
            }

            if (*mouseDeltaX != 0.0f || *mouseDeltaY != 0.0f)
            {
                yaw += *mouseDeltaX * aEvent.GetDeltaTime();
                pitch += *mouseDeltaY * aEvent.GetDeltaTime();
                camera->m_transform.m_rotation = glm::quat{glm::vec3{pitch, yaw, 0.0f}};

                *mouseDeltaX = 0.0f;
                *mouseDeltaY = 0.0f;
            }

            const auto modelViewProjection =
                camera->GetProjectionMatrix() * camera->m_transform.ViewMatrix() * transform.WorldMatrix();
            entity.UpdateMatrix(modelViewProjection);
        };
        eventModule->PushBack<soge::UpdateEvent>(update);
    }
}

soge::Engine* soge::CreateApplication()
{
    return Engine::Reset<soge_game::Game>();
}
