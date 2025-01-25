#ifndef SOGE_GRAPHICS_ENTITIES_GEOMETRYENTITY_HPP
#define SOGE_GRAPHICS_ENTITIES_GEOMETRYENTITY_HPP

#include "SOGE/Graphics/Deferred/GeometryGraphicsPipeline.hpp"
#include "SOGE/Graphics/GraphicsEntity.hpp"


namespace soge
{
    class GeometryEntity : public GraphicsEntity, public GeometryGraphicsPipelineEntity
    {
    public:
        using Material = ConstantBufferData::Material;

    private:
        void CreateBindingSet();

        void WriteConstantBuffer(nvrhi::ICommandList& aCommandList);
        void WriteVertexBuffer(nvrhi::ICommandList& aCommandList);
        void WriteIndexBuffer(nvrhi::ICommandList& aCommandList);

        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsPipeline> m_pipeline;

        Transform m_transform;
        Material m_material;
        glm::vec3 m_color;
        eastl::vector<Vertex> m_vertices;
        eastl::vector<Index> m_indices;

        bool m_shouldWriteConstantBuffer;
        bool m_shouldWriteVertexBuffer;
        bool m_shouldWriteIndexBuffer;

        nvrhi::BindingSetHandle m_nvrhiBindingSet;
        nvrhi::TextureHandle m_nvrhiOneByOneTexture;
        nvrhi::TextureHandle m_nvrhiColorTexture;
        nvrhi::SamplerHandle m_nvrhiColorTextureSampler;
        nvrhi::BufferHandle m_nvrhiConstantBuffer;
        nvrhi::BufferHandle m_nvrhiVertexBuffer;
        nvrhi::BufferHandle m_nvrhiIndexBuffer;

    public:
        explicit GeometryEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                Transform aTransform = Transform{}, Material aMaterial = {},
                                eastl::vector<Vertex> aVertices = {}, eastl::vector<Index> aIndices = {},
                                nvrhi::TextureHandle aColorTexture = {});

        [[nodiscard]]
        const Transform& GetTransform() const;
        Transform& GetTransform();

        [[nodiscard]]
        const Material& GetMaterial() const;
        Material& GetMaterial();

        [[nodiscard]]
        eastl::span<const Vertex> GetVertices() const;
        eastl::vector<Vertex>& GetVertices();

        [[nodiscard]]
        eastl::span<const Index> GetIndices() const;
        eastl::vector<Index>& GetIndices();

        [[nodiscard]]
        glm::vec3 GetColor() const;
        glm::vec3& GetColor();

        [[nodiscard]]
        nvrhi::ITexture* GetColorTexture() const;
        nvrhi::TextureHandle& GetColorTexture();

        [[nodiscard]]
        nvrhi::BindingSetHandle GetBindingSet(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetVertexBuffer(Tag) override;
        [[nodiscard]]
        nvrhi::BufferHandle GetIndexBuffer(Tag) override;

        void WriteResources(nvrhi::ICommandList& aCommandList) override;
    };
}

SOGE_DI_REGISTER_NS(soge, GeometryEntity, df::Factory<GeometryEntity, GraphicsCore, GeometryGraphicsPipeline>)

#endif // SOGE_GRAPHICS_ENTITIES_GEOMETRYENTITY_HPP
