#include "sogepch.hpp"

#include "SOGE/Graphics/TriangleEntity.hpp"

#include "SOGE/Graphics/GraphicsCommandListGuard.hpp"


namespace soge
{
    TriangleEntity::TriangleEntity(GraphicsCore& aCore, TriangleGraphicsPipeline& aPipeline)
        : m_core{aCore}, m_pipeline{aPipeline}
    {
        nvrhi::IDevice& device = aCore.GetRawDevice();

        SOGE_INFO_LOG("Creating NVRHI constant buffer...");
        nvrhi::BufferDesc bufferDesc{};
        bufferDesc.byteSize = sizeof(glm::mat4x4);
        bufferDesc.isConstantBuffer = true;
        bufferDesc.initialState = nvrhi::ResourceStates::ConstantBuffer;
        bufferDesc.keepInitialState = true;
        bufferDesc.debugName = "SOGE constant buffer";
        m_nvrhiConstantBuffer = device.createBuffer(bufferDesc);

        SOGE_INFO_LOG("Creating NVRHI binding set...");
        nvrhi::BindingSetDesc bindingSetDesc{};
        bindingSetDesc.trackLiveness = true;
        bindingSetDesc.addItem(nvrhi::BindingSetItem::ConstantBuffer(0, m_nvrhiConstantBuffer));
        m_nvrhiBindingSet = device.createBindingSet(bindingSetDesc, &aPipeline.GetBindingLayout());

        UpdateMatrix(glm::mat4x4{1.0f});
    }

    void TriangleEntity::UpdateVertices(const Vertices aVertices)
    {
        if (aVertices.empty())
        {
            m_nvrhiVertexBuffer = nullptr;
            return;
        }

        nvrhi::IDevice& device = m_core.get().GetRawDevice();
        const auto inputByteSize = static_cast<std::uint64_t>(aVertices.size_bytes());

        if (m_nvrhiVertexBuffer == nullptr || m_nvrhiVertexBuffer->getDesc().byteSize != inputByteSize)
        {
            SOGE_INFO_LOG("Creating NVRHI vertex buffer...");
            nvrhi::BufferDesc bufferDesc{};
            bufferDesc.byteSize = inputByteSize;
            bufferDesc.isVertexBuffer = true;
            bufferDesc.initialState = nvrhi::ResourceStates::VertexBuffer;
            bufferDesc.keepInitialState = true;
            bufferDesc.debugName = "SOGE vertex buffer";
            m_nvrhiVertexBuffer = device.createBuffer(bufferDesc);
        }

        SOGE_INFO_LOG("Updating NVRHI vertex buffer...");
        const nvrhi::CommandListHandle updateCommandList = device.createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};
            commandList->writeBuffer(m_nvrhiVertexBuffer, aVertices.data(), inputByteSize);
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    void TriangleEntity::UpdateMatrix(const glm::mat4x4& aMatrix)
    {
        SOGE_INFO_LOG("Updating NVRHI constant buffer...");
        const nvrhi::CommandListHandle updateCommandList = m_core.get().GetRawDevice().createCommandList();
        {
            GraphicsCommandListGuard commandList{*updateCommandList};
            commandList->writeBuffer(m_nvrhiConstantBuffer, &aMatrix, sizeof(glm::mat4x4));
        }
        m_core.get().ExecuteCommandList(updateCommandList, nvrhi::CommandQueue::Graphics);
    }

    nvrhi::CommandListHandle TriangleEntity::Update(GraphicsRenderPass& aRenderPass, GraphicsPipeline& aPipeline)
    {
        if (m_nvrhiVertexBuffer == nullptr)
        {
            return {};
        }

        // check if render passes are the same
        if (&aRenderPass != &m_pipeline.get().GetRenderPass())
        {
            return {};
        }

        // check for pipeline framebuffer compatibility
        if (aPipeline.GetGraphicsPipeline().getFramebufferInfo() !=
            m_pipeline.get().GetGraphicsPipeline().getFramebufferInfo())
        {
            return {};
        }

        nvrhi::CommandListParameters commandListDesc{};
        commandListDesc.enableImmediateExecution = false;

        nvrhi::IDevice& device = m_core.get().GetRawDevice();
        const nvrhi::CommandListHandle drawCommandList = device.createCommandList(commandListDesc);
        {
            GraphicsCommandListGuard commandList{*drawCommandList};

            nvrhi::IFramebuffer& currentFramebuffer = aRenderPass.GetFramebuffer();
            const nvrhi::FramebufferInfoEx& framebufferInfo = currentFramebuffer.getFramebufferInfo();

            nvrhi::GraphicsState graphicsState{};
            graphicsState.pipeline = &aPipeline.GetGraphicsPipeline();
            graphicsState.framebuffer = &currentFramebuffer;
            graphicsState.bindings = {m_nvrhiBindingSet};
            graphicsState.vertexBuffers = {
                nvrhi::VertexBufferBinding{.buffer = m_nvrhiVertexBuffer, .slot = 0, .offset = 0},
            };
            graphicsState.viewport.addViewportAndScissorRect(framebufferInfo.getViewport());
            commandList->setGraphicsState(graphicsState);

            const auto& vertexBufferDesc = m_nvrhiVertexBuffer->getDesc();
            const auto vertexCount = static_cast<std::uint32_t>(vertexBufferDesc.byteSize / sizeof(Vertex));

            nvrhi::DrawArguments drawArguments{};
            drawArguments.vertexCount = vertexCount;
            commandList->draw(drawArguments);
        }

        return drawCommandList;
    }
}
