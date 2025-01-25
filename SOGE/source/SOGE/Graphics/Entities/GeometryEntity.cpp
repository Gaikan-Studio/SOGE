#include "sogepch.hpp"

#include "SOGE/Graphics/Entities/GeometryEntity.hpp"

#include "SOGE/Graphics/Utils/GraphicsCommandListGuard.hpp"


namespace soge
{
    GeometryEntity::GeometryEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline, Transform aTransform,
                                   Material aMaterial, eastl::vector<Vertex> aVertices, eastl::vector<Index> aIndices,
                                   nvrhi::TextureHandle aColorTexture)
        : m_core{aCore}, m_pipeline{aPipeline}, m_transform{aTransform}, m_material{aMaterial}, m_color{1.0f},
          m_vertices{std::move(aVertices)}, m_indices{std::move(aIndices)}, m_shouldWriteConstantBuffer{true},
          m_shouldWriteVertexBuffer{true}, m_shouldWriteIndexBuffer{true}, m_nvrhiColorTexture{std::move(aColorTexture)}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        SOGE_INFO_LOG("Creating NVRHI constant buffer for geometry entity...");
        nvrhi::BufferDesc constantBufferDesc{};
        constantBufferDesc.byteSize = sizeof(ConstantBufferData);
        constantBufferDesc.isConstantBuffer = true;
        constantBufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        constantBufferDesc.keepInitialState = true;
        constantBufferDesc.debugName = "SOGE geometry entity constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(constantBufferDesc);

        SOGE_INFO_LOG("Creating NVRHI color texture sampler for geometry entity...");
        nvrhi::SamplerDesc samplerDesc{};
        m_nvrhiColorTextureSampler = device.createSampler(samplerDesc);

        nvrhi::TextureDesc oneByOneTextureDesc{};
        oneByOneTextureDesc.dimension = nvrhi::TextureDimension::Texture2D;
        oneByOneTextureDesc.format = nvrhi::Format::RGBA16_FLOAT;
        oneByOneTextureDesc.initialState = nvrhi::ResourceStates::ShaderResource;
        oneByOneTextureDesc.keepInitialState = true;
        oneByOneTextureDesc.debugName = "SOGE geometry entity 1x1 texture";
        m_nvrhiOneByOneTexture = device.createTexture(oneByOneTextureDesc);

        CreateBindingSet();
    }

    const Transform& GeometryEntity::GetTransform() const
    {
        return m_transform;
    }

    Transform& GeometryEntity::GetTransform()
    {
        m_shouldWriteConstantBuffer = true;
        return m_transform;
    }

    auto GeometryEntity::GetMaterial() const -> const Material&
    {
        return m_material;
    }

    auto GeometryEntity::GetMaterial() -> Material&
    {
        m_shouldWriteConstantBuffer = true;
        return m_material;
    }

    auto GeometryEntity::GetVertices() const -> eastl::span<const Vertex>
    {
        return m_vertices;
    }

    auto GeometryEntity::GetVertices() -> eastl::vector<Vertex>&
    {
        m_shouldWriteVertexBuffer = true;
        return m_vertices;
    }

    auto GeometryEntity::GetIndices() const -> eastl::span<const Index>
    {
        return m_indices;
    }

    auto GeometryEntity::GetIndices() -> eastl::vector<Index>&
    {
        m_shouldWriteIndexBuffer = true;
        return m_indices;
    }

    glm::vec3 GeometryEntity::GetColor() const
    {
        return m_color;
    }

    glm::vec3& GeometryEntity::GetColor()
    {
        m_shouldWriteConstantBuffer = true;
        return m_color;
    }

    nvrhi::ITexture* GeometryEntity::GetColorTexture() const
    {
        return m_nvrhiColorTexture;
    }

    nvrhi::TextureHandle& GeometryEntity::GetColorTexture()
    {
        m_shouldWriteConstantBuffer = true;
        return m_nvrhiColorTexture;
    }

    nvrhi::BindingSetHandle GeometryEntity::GetBindingSet(Tag)
    {
        return m_nvrhiBindingSet;
    }

    nvrhi::BufferHandle GeometryEntity::GetVertexBuffer(Tag)
    {
        return m_nvrhiVertexBuffer;
    }

    nvrhi::BufferHandle GeometryEntity::GetIndexBuffer(Tag)
    {
        return m_nvrhiIndexBuffer;
    }

    void GeometryEntity::WriteResources(nvrhi::ICommandList& aCommandList)
    {
        WriteConstantBuffer(aCommandList);
        WriteVertexBuffer(aCommandList);
        WriteIndexBuffer(aCommandList);
    }

    void GeometryEntity::CreateBindingSet()
    {
        SOGE_INFO_LOG("Creating NVRHI binding set for geometry entity...");
        nvrhi::BindingSetDesc bindingSetDesc;
        bindingSetDesc.trackLiveness = true;
        bindingSetDesc.bindings = {
            nvrhi::BindingSetItem::Texture_SRV(0, m_nvrhiColorTexture != nullptr ? m_nvrhiColorTexture
                                                                                 : m_nvrhiOneByOneTexture),
            nvrhi::BindingSetItem::Sampler(0, m_nvrhiColorTextureSampler),
            nvrhi::BindingSetItem::ConstantBuffer(1, m_nvrhiConstantBuffer),
        };
        nvrhi::IDevice& device = m_core.get().GetRawDevice();
        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, &m_pipeline.get().GetEntityBindingLayout());
    }

    void GeometryEntity::WriteConstantBuffer(nvrhi::ICommandList& aCommandList)
    {
        if (!m_shouldWriteConstantBuffer)
        {
            return;
        }
        m_shouldWriteConstantBuffer = false;

        CreateBindingSet();

        SOGE_INFO_LOG("Updating NVRHI constant buffer for geometry entity...");
        const ConstantBufferData constantBufferData{
            .m_model = m_transform.WorldMatrix(),
            .m_material = m_material,
            .m_color = m_color,
            .m_hasColorTexture = m_nvrhiColorTexture != nullptr,
        };
        aCommandList.writeBuffer(m_nvrhiConstantBuffer, &constantBufferData, sizeof(constantBufferData));
    }

    void GeometryEntity::WriteVertexBuffer(nvrhi::ICommandList& aCommandList)
    {
        if (!m_shouldWriteVertexBuffer)
        {
            return;
        }
        m_shouldWriteVertexBuffer = false;

        const eastl::span vertices{m_vertices};
        if (vertices.empty())
        {
            m_nvrhiVertexBuffer = nullptr;
            return;
        }

        if (m_nvrhiVertexBuffer == nullptr || m_nvrhiVertexBuffer->getDesc().byteSize != vertices.size_bytes())
        {
            SOGE_INFO_LOG("Creating NVRHI vertex buffer for geometry entity...");
            nvrhi::BufferDesc bufferDesc{};
            bufferDesc.byteSize = vertices.size_bytes();
            bufferDesc.isVertexBuffer = true;
            bufferDesc.initialState = nvrhi::ResourceStates::VertexBuffer;
            bufferDesc.keepInitialState = true;
            bufferDesc.debugName = "SOGE geometry entity vertex buffer";
            m_nvrhiVertexBuffer = m_core.get().GetRawDevice().createBuffer(bufferDesc);
        }

        SOGE_INFO_LOG("Updating NVRHI vertex buffer for geometry entity...");
        aCommandList.writeBuffer(m_nvrhiVertexBuffer, vertices.data(), vertices.size_bytes());
    }

    void GeometryEntity::WriteIndexBuffer(nvrhi::ICommandList& aCommandList)
    {
        if (!m_shouldWriteIndexBuffer)
        {
            return;
        }
        m_shouldWriteIndexBuffer = false;

        const eastl::span indices{m_indices};
        if (indices.empty())
        {
            m_nvrhiIndexBuffer = nullptr;
            return;
        }

        if (m_nvrhiIndexBuffer == nullptr || m_nvrhiIndexBuffer->getDesc().byteSize != indices.size_bytes())
        {
            SOGE_INFO_LOG("Creating NVRHI index buffer for geometry entity...");
            nvrhi::BufferDesc bufferDesc{};
            bufferDesc.byteSize = indices.size_bytes();
            bufferDesc.isIndexBuffer = true;
            bufferDesc.initialState = nvrhi::ResourceStates::IndexBuffer;
            bufferDesc.keepInitialState = true;
            bufferDesc.debugName = "SOGE geometry entity index buffer";
            m_nvrhiIndexBuffer = m_core.get().GetRawDevice().createBuffer(bufferDesc);
        }

        SOGE_INFO_LOG("Updating NVRHI index buffer for geometry entity...");
        aCommandList.writeBuffer(m_nvrhiIndexBuffer, indices.data(), indices.size_bytes());
    }
}
