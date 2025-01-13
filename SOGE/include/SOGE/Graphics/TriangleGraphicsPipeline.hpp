#ifndef SOGE_GRAPHICS_TRIANGLEGRAPHICSPIPELINE_HPP
#define SOGE_GRAPHICS_TRIANGLEGRAPHICSPIPELINE_HPP

#include "SOGE/Graphics/FinalGraphicsRenderPass.hpp"
#include "SOGE/Graphics/GraphicsCore.hpp"
#include "SOGE/Graphics/GraphicsPipeline.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


namespace soge
{
    class TriangleGraphicsPipeline : public GraphicsPipeline
    {
    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<FinalGraphicsRenderPass> m_renderPass;

        nvrhi::ShaderHandle m_nvrhiVertexShader;
        nvrhi::InputLayoutHandle m_nvrhiInputLayout;
        nvrhi::ShaderHandle m_nvrhiPixelShader;
        nvrhi::BindingLayoutHandle m_nvrhiBindingLayout;
        nvrhi::GraphicsPipelineHandle m_nvrhiGraphicsPipeline;

    public:
        struct Vertex
        {
            alignas(16) glm::vec3 m_position;
            glm::vec4 m_color;
        };

        explicit TriangleGraphicsPipeline(GraphicsCore& aCore, FinalGraphicsRenderPass& aRenderPass);

        TriangleGraphicsPipeline(const TriangleGraphicsPipeline&) = delete;
        TriangleGraphicsPipeline& operator=(const TriangleGraphicsPipeline&) = delete;

        TriangleGraphicsPipeline(TriangleGraphicsPipeline&& aOther) noexcept;
        TriangleGraphicsPipeline& operator=(TriangleGraphicsPipeline&& aOther) noexcept;

        ~TriangleGraphicsPipeline() override;

        // NOLINTNEXTLINE(readability-identifier-naming) reason: ADL support
        void swap(TriangleGraphicsPipeline& aOther) noexcept;

        [[nodiscard]]
        FinalGraphicsRenderPass& GetRenderPass() noexcept;
        [[nodiscard]]
        nvrhi::IShader& GetVertexShader() noexcept;
        [[nodiscard]]
        nvrhi::IInputLayout& GetInputLayout() noexcept;
        [[nodiscard]]
        nvrhi::IShader& GetPixelShader() noexcept;
        [[nodiscard]]
        nvrhi::IBindingLayout& GetBindingLayout() noexcept;
        [[nodiscard]]
        nvrhi::IGraphicsPipeline& GetGraphicsPipeline() noexcept override;
    };
}

SOGE_DI_REGISTER_NS(soge, TriangleGraphicsPipeline,
                    df::Single<TriangleGraphicsPipeline, GraphicsCore, FinalGraphicsRenderPass>)

#endif // SOGE_GRAPHICS_TRIANGLEGRAPHICSPIPELINE_HPP
