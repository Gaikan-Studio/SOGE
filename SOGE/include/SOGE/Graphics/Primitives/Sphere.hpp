#ifndef SOGE_GRAPHICS_PRIMITIVES_SPHERE_HPP
#define SOGE_GRAPHICS_PRIMITIVES_SPHERE_HPP

#include "SOGE/Graphics/Entities/GeometryEntity.hpp"
#include "SOGE/Graphics/GraphicsPrimitive.hpp"


namespace soge
{
    [[nodiscard]]
    GeometryEntity CreateSphere(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                const Transform& aTransform = Transform{},
                                const GeometryEntity::Material& aMaterial = {}, uint32_t aMeridians = 32,
                                uint32_t aParallels = 16, float aRadius = 1.0f, glm::vec3 aColor = glm::vec3{1.0f});

    [[nodiscard]]
    eastl::vector<GeometryEntity::Vertex> CreateSphereVertices(uint32_t aMeridians = 32, uint32_t aParallels = 16,
                                                               float aRadius = 1.0f,
                                                               glm::vec3 aColor = glm::vec3{1.0f});

    [[nodiscard]]
    eastl::vector<GeometryEntity::Index> CreateSphereIndices(uint32_t aMeridians = 32, uint32_t aParallels = 16);

    class SpherePrimitive : public GraphicsPrimitive
    {
    private:
        GeometryEntity m_geometryEntity;
        uint32_t m_meridians;
        uint32_t m_parallels;
        float m_radius;
        glm::vec3 m_color;
        bool m_shouldWrite;

    public:
        using Material = GeometryEntity::Material;

        explicit SpherePrimitive(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                 const Transform& aTransform = Transform{}, const Material& aMaterial = {},
                                 uint32_t aMeridians = 32, uint32_t aParallels = 16, float aRadius = 1.0f,
                                 glm::vec3 aColor = glm::vec3{1.0f});

        [[nodiscard]]
        Transform GetTransform() const override;
        Transform& GetTransform();

        [[nodiscard]]
        Material GetMaterial() const;
        Material& GetMaterial();

        [[nodiscard]]
        uint32_t GetMeridians() const;
        uint32_t& GetMeridians();

        [[nodiscard]]
        uint32_t GetParallels() const;
        uint32_t& GetParallels();

        [[nodiscard]]
        float GetRadius() const;
        float& GetRadius();

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

SOGE_DI_REGISTER_NS(soge, SpherePrimitive, df::Factory<SpherePrimitive, GraphicsCore, GeometryGraphicsPipeline>)

#endif // SOGE_GRAPHICS_PRIMITIVES_SPHERE_HPP
