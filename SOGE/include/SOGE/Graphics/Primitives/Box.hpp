#ifndef SOGE_GRAPHICS_PRIMITIVES_BOX_HPP
#define SOGE_GRAPHICS_PRIMITIVES_BOX_HPP

#include "SOGE/Graphics/Entities/GeometryEntity.hpp"
#include "SOGE/Graphics/GraphicsPrimitive.hpp"


namespace soge
{
    [[nodiscard]]
    GeometryEntity CreateBox(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                             const Transform& aTransform = Transform{}, GeometryEntity::Material aMaterial = {},
                             glm::vec3 aDimensions = glm::vec3{1.0f}, glm::vec3 aColor = glm::vec3{1.0f});

    [[nodiscard]]
    constexpr eastl::array<GeometryEntity::Vertex, 24> CreateBoxVertices(glm::vec3 aDimensions = glm::vec3{1.0f},
                                                                         glm::vec3 aColor = glm::vec3{1.0f});

    [[nodiscard]]
    constexpr eastl::array<GeometryEntity::Index, 36> CreateBoxIndices();

    class BoxPrimitive : public GraphicsPrimitive
    {
    private:
        GeometryEntity m_geometryEntity;
        glm::vec3 m_dimensions;
        glm::vec3 m_color;
        bool m_shouldWrite;

    public:
        using Material = GeometryEntity::Material;

        explicit BoxPrimitive(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                              const Transform& aTransform = Transform{}, const Material& aMaterial = Material{},
                              glm::vec3 aDimensions = glm::vec3{1.0f}, glm::vec3 aColor = glm::vec3{1.0f});

        [[nodiscard]]
        Transform GetTransform() const override;
        Transform& GetTransform();

        [[nodiscard]]
        nvrhi::ITexture* GetColorTexture() const;
        nvrhi::TextureHandle& GetColorTexture();

        [[nodiscard]]
        Material GetMaterial() const;
        Material& GetMaterial();

        [[nodiscard]]
        glm::vec3 GetDimensions() const;
        glm::vec3& GetDimensions();

        [[nodiscard]]
        glm::vec3 GetColor() const override;
        glm::vec3& GetColor();

        [[nodiscard]]
        eastl::span<const Vertex> GetVertices() const override;
        [[nodiscard]]
        eastl::span<const Index> GetIndices() const override;

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetVertexBuffer(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetIndexBuffer(Tag) override;

        void WriteResources(nvrhi::ICommandList& aCommandList) override;
    };
}

SOGE_DI_REGISTER_NS(soge, BoxPrimitive, df::Factory<BoxPrimitive, GraphicsCore, GeometryGraphicsPipeline>)

/////////////////////////////////////
// constexpr function implementations
/////////////////////////////////////

namespace soge
{
    constexpr eastl::array<GeometryEntity::Vertex, 24> CreateBoxVertices(const glm::vec3 aDimensions,
                                                                         const glm::vec3 aColor)
    {
        using Vertex = GeometryEntity::Vertex;

        const float right = aDimensions.x / 2;
        const float left = -aDimensions.x / 2;
        const float top = aDimensions.y / 2;
        const float bottom = -aDimensions.y / 2;
        const float forward = aDimensions.z / 2;
        const float backward = -aDimensions.z / 2;

        return {
            // Front face
            Vertex{
                .m_position = glm::vec3{left, bottom, forward},
                .m_normal = glm::vec3{0.0f, 0.0f, 1.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 1.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, bottom, forward},
                .m_normal = glm::vec3{0.0f, 0.0f, 1.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 1.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, top, forward},
                .m_normal = glm::vec3{0.0f, 0.0f, 1.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 0.0f},
            },
            Vertex{
                .m_position = glm::vec3{left, top, forward},
                .m_normal = glm::vec3{0.0f, 0.0f, 1.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 0.0f},
            },

            // Back face
            Vertex{
                .m_position = glm::vec3{left, bottom, backward},
                .m_normal = glm::vec3{0.0f, 0.0f, -1.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 1.0f},
            },
            Vertex{
                .m_position = glm::vec3{left, top, backward},
                .m_normal = glm::vec3{0.0f, 0.0f, -1.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 0.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, top, backward},
                .m_normal = glm::vec3{0.0f, 0.0f, -1.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 0.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, bottom, backward},
                .m_normal = glm::vec3{0.0f, 0.0f, -1.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 1.0f},
            },

            // Top Face
            Vertex{
                .m_position = glm::vec3{left, top, backward},
                .m_normal = glm::vec3{0.0f, 1.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 1.0f},
            },
            Vertex{
                .m_position = glm::vec3{left, top, forward},
                .m_normal = glm::vec3{0.0f, 1.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 0.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, top, forward},
                .m_normal = glm::vec3{0.0f, 1.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 0.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, top, backward},
                .m_normal = glm::vec3{0.0f, 1.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 1.0f},
            },

            // Bottom Face
            Vertex{
                .m_position = glm::vec3{left, bottom, backward},
                .m_normal = glm::vec3{0.0f, -1.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 1.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, bottom, backward},
                .m_normal = glm::vec3{0.0f, -1.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 1.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, bottom, forward},
                .m_normal = glm::vec3{0.0f, -1.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 0.0f},
            },
            Vertex{
                .m_position = glm::vec3{left, bottom, forward},
                .m_normal = glm::vec3{0.0f, -1.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 0.0f},
            },

            // Left Face
            Vertex{
                .m_position = glm::vec3{left, bottom, forward},
                .m_normal = glm::vec3{-1.0f, 0.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 1.0f},
            },
            Vertex{
                .m_position = glm::vec3{left, top, forward},
                .m_normal = glm::vec3{-1.0f, 0.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 0.0f},
            },
            Vertex{
                .m_position = glm::vec3{left, top, backward},
                .m_normal = glm::vec3{-1.0f, 0.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 0.0f},
            },
            Vertex{
                .m_position = glm::vec3{left, bottom, backward},
                .m_normal = glm::vec3{-1.0f, 0.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 1.0f},
            },

            // Right Face
            Vertex{
                .m_position = glm::vec3{right, bottom, backward},
                .m_normal = glm::vec3{1.0f, 0.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 1.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, top, backward},
                .m_normal = glm::vec3{1.0f, 0.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{0.0f, 0.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, top, forward},
                .m_normal = glm::vec3{1.0f, 0.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 0.0f},
            },
            Vertex{
                .m_position = glm::vec3{right, bottom, forward},
                .m_normal = glm::vec3{1.0f, 0.0f, 0.0f},
                .m_color = aColor,
                .m_texCoord = glm::vec2{1.0f, 1.0f},
            },
        };
    }

    constexpr eastl::array<GeometryEntity::Index, 36> CreateBoxIndices()
    {
        // clang-format off
        return {
            // Front face
            0u, 1u, 2u,
            0u, 2u, 3u,
            // Back face
            4u, 5u, 6u,
            4u, 6u, 7u,
            // Top Face
            8u, 9u, 10u,
            8u, 10u, 11u,
            // Bottom Face
            12u, 13u, 14u,
            12u, 14u, 15u,
            // Left Face
            16u, 17u, 18u,
            16u, 18u, 19u,
            // Right Face
            20u, 21u, 22u,
            20u, 22u, 23u,
        };
        // clang-format on
    }
}

#endif // SOGE_GRAPHICS_PRIMITIVES_BOX_HPP
