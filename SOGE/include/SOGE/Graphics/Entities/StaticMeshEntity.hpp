﻿#ifndef SOGE_GRAPHICS_ENTITIES_STATICMESHENTITY_HPP
#define SOGE_GRAPHICS_ENTITIES_STATICMESHENTITY_HPP

#include "SOGE/Graphics/Entities/GeometryEntity.hpp"
#include "SOGE/Graphics/GraphicsModule.hpp"

#include <cppfs/FilePath.h>


namespace soge
{
    class StaticMeshEntity : public GraphicsEntity
    {
    public:
        class Hierarchy;

    private:
        eastl::reference_wrapper<GraphicsCore> m_core;
        eastl::reference_wrapper<GeometryGraphicsPipeline> m_pipeline;
        eastl::reference_wrapper<GraphicsEntityManager> m_entityManager;
        eastl::reference_wrapper<GraphicsResourceManager> m_resourceManager;

        cppfs::FilePath m_filePath;
        UniquePtr<Hierarchy> m_hierarchy;
        bool m_shouldReadFromFile;
        bool m_shouldUpdateTransforms;

    public:
        explicit StaticMeshEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                  GraphicsModule& aGraphicsModule);

        StaticMeshEntity(const StaticMeshEntity&) = delete;
        StaticMeshEntity& operator=(const StaticMeshEntity&) = delete;

        StaticMeshEntity(StaticMeshEntity&& aOther) noexcept;
        StaticMeshEntity& operator=(StaticMeshEntity&& aOther) noexcept;

        ~StaticMeshEntity() override;

        [[nodiscard]]
        Transform GetTransform() const;
        Transform& GetTransform();

        [[nodiscard]]
        const cppfs::FilePath& GetFilePath() const;
        cppfs::FilePath& GetFilePath();

        void Load();
    };
}

SOGE_DI_REGISTER_NS(soge, StaticMeshEntity,
                    df::Factory<StaticMeshEntity, GraphicsCore, GeometryGraphicsPipeline, GraphicsModule>)

#endif // SOGE_GRAPHICS_ENTITIES_STATICMESHENTITY_HPP
