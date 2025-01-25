#include "sogepch.hpp"

#include "SOGE/Graphics/Entities/StaticMeshEntity.hpp"

#include "SOGE/Graphics/Resources/SimpleTextureResource.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


namespace soge
{
    class StaticMeshEntity::Hierarchy
    {
    public:
        static constexpr std::size_t s_invalidParent = std::numeric_limits<std::size_t>::max();

        struct Item
        {
            Transform m_localTransform;
            std::size_t m_parent = s_invalidParent;
        };
        eastl::vector<Item> m_transforms{1, Item{}};

        eastl::hash_map<UUIDv4::UUID, std::size_t> m_geometryToTransform;
        eastl::vector<UUIDv4::UUID> m_textures;

        void Reset()
        {
            m_geometryToTransform.clear();
            m_textures.clear();
            m_transforms.resize(1);
        }

        Transform GetWorldTransform(const std::size_t aIndex) const
        {
            if (aIndex == s_invalidParent)
            {
                return {};
            }
            auto [current, parentIndex] = m_transforms[aIndex];

            const auto [position, rotation, scale] = GetWorldTransform(parentIndex);
            current = Transform{
                .m_position = glm::rotate(rotation, current.m_position * scale) + position,
                .m_rotation = current.m_rotation * rotation,
                .m_scale = current.m_scale * scale,
            };

            return current;
        }
    };
}

namespace
{
    [[nodiscard]]
    soge::Transform TransformFromNode(const aiNode& aNode)
    {
        aiVector3D position, scale;
        aiQuaternion rotation;
        aNode.mTransformation.Decompose(scale, rotation, position);

        return soge::Transform{
            .m_position = glm::vec3{position.x, position.y, position.z},
            .m_rotation = glm::quat{rotation.x, rotation.y, rotation.z, rotation.w},
            .m_scale = glm::vec3{scale.x, scale.y, scale.z},
        };
    }

    [[nodiscard]]
    eastl::pair<soge::GeometryEntity, eastl::optional<soge::SimpleTextureResource>> CreateEntityFromMesh(
        const cppfs::FilePath& aStaticMeshPath, const aiScene& aScene, const aiMesh& aMesh, soge::GraphicsCore& aCore,
        soge::GeometryGraphicsPipeline& aPipeline)
    {
        using Material = soge::GeometryEntity::Material;

        Material material;
        eastl::optional<cppfs::FilePath> colorTexturePath;
        if (const aiMaterial* aiMaterial = aScene.mNumMaterials > 0 ? aScene.mMaterials[aMesh.mMaterialIndex] : nullptr)
        {
            if (aiColor3D aiAmbient{1.0f, 1.0f, 1.0f};
                aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbient) == aiReturn_SUCCESS)
            {
                material.m_ambient = glm::vec3{aiAmbient.r, aiAmbient.g, aiAmbient.b};
            }

            if (aiColor3D aiDiffuse{1.0f, 1.0f, 1.0f};
                aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse) == aiReturn_SUCCESS)
            {
                material.m_diffuse = glm::vec3{aiDiffuse.r, aiDiffuse.g, aiDiffuse.b};
            }

            if (aiColor3D aiSpecular{1.0f, 1.0f, 1.0f};
                aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular) == aiReturn_SUCCESS)
            {
                material.m_specular = glm::vec3{aiSpecular.r, aiSpecular.g, aiSpecular.b};
            }

            if (aiColor3D aiEmissive{0.0f, 0.0f, 0.0f};
                aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmissive) == aiReturn_SUCCESS)
            {
                material.m_emissive = glm::vec3{aiEmissive.r, aiEmissive.g, aiEmissive.b};
            }

            if (float shininess = 8.0f; aiMaterial->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS)
            {
                material.m_shininess = shininess;
            }

            if (aiString aiTextureDiffuse;
                aiMaterial->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), aiTextureDiffuse) == aiReturn_SUCCESS)
            {
                colorTexturePath = cppfs::FilePath{
                    fmt::format("{}{}", aStaticMeshPath.directoryPath(), aiTextureDiffuse.C_Str()),
                };
            }
        }

        using Vertex = soge::GeometryEntity::Vertex;
        using Index = soge::GeometryEntity::Index;

        eastl::vector<Vertex> vertices;
        vertices.reserve(aMesh.mNumVertices);
        for (std::uint32_t index = 0; index < aMesh.mNumVertices; ++index)
        {
            const auto [x, y, z] = aMesh.mVertices[index];
            const glm::vec3 position{x, y, z};

            glm::vec3 normal{};
            if (const aiVector3D* aiNormals = aMesh.mNormals)
            {
                const auto [nx, ny, nz] = aiNormals[index];
                normal = glm::vec3{nx, ny, nz};
            }

            glm::vec3 color{1.0f};
            if (const aiColor4D* aiColor = aMesh.mColors[0])
            {
                const auto [r, g, b, a] = aiColor[index];
                color *= glm::vec3{r, g, b};
            }

            glm::vec2 texCoord;
            if (const aiVector3D* aiTexCoord = aMesh.mTextureCoords[0])
            {
                const auto [cx, cy, cz] = aiTexCoord[index];
                texCoord = glm::vec2{cx, cy};
            }

            vertices.emplace_back(Vertex{
                .m_position = position,
                .m_normal = normal,
                .m_color = color,
                .m_texCoord = texCoord,
            });
        }

        eastl::vector<Index> indices;
        indices.reserve(static_cast<std::size_t>(aMesh.mNumFaces) * 3u);
        for (const aiFace& face : eastl::span{aMesh.mFaces, aMesh.mNumFaces})
        {
            for (const eastl::span aiIndices{face.mIndices, face.mNumIndices};
                 const std::uint32_t index : aiIndices | std::views::take(3))
            {
                indices.emplace_back(index);
            }
        }

        soge::GeometryEntity geometryEntity{aCore, aPipeline, soge::Transform{}, material, vertices, indices};
        eastl::optional<soge::SimpleTextureResource> simpleTextureResource;
        if (colorTexturePath.has_value())
        {
            simpleTextureResource = soge::SimpleTextureResource{aCore, "", colorTexturePath.value()};
        }
        return {std::move(geometryEntity), std::move(simpleTextureResource)};
    }

    void TraverseNode(const cppfs::FilePath& aStaticMeshPath, const aiScene& aScene, const aiNode& aNode,
                      soge::GraphicsCore& aCore, soge::GeometryGraphicsPipeline& aPipeline,
                      soge::GraphicsEntityManager& aEntityManager, soge::StaticMeshEntity::Hierarchy& aHierarchy,
                      const std::size_t aHierarchyParent)
    {
        aHierarchy.m_transforms.push_back({
            .m_localTransform = TransformFromNode(aNode),
            .m_parent = aHierarchyParent,
        });
        const auto currentHierarchyIndex = aHierarchy.m_transforms.size() - 1;

        for (const std::size_t meshIndex : eastl::span{aNode.mMeshes, aNode.mNumMeshes})
        {
            const aiMesh* aiMesh = aScene.mMeshes[meshIndex];
            if (aiMesh == nullptr)
            {
                continue;
            }

            auto entityFromMesh = CreateEntityFromMesh(aStaticMeshPath, aScene, *aiMesh, aCore, aPipeline);

            const auto [entity, entityUuid] =
                aEntityManager.CreateEntity<soge::GeometryEntity>(std::move(entityFromMesh.first));
            SOGE_INFO_LOG("Created static mesh (current node index is {}) with UUID: {}", currentHierarchyIndex,
                          entityUuid.str());
            aHierarchy.m_geometryToTransform[entityUuid] = currentHierarchyIndex;

            if (entityFromMesh.second.has_value())
            {
                const auto [texture, textureUuid] =
                    aEntityManager.CreateEntity<soge::SimpleTextureResource>(std::move(entityFromMesh.second).value());
                SOGE_INFO_LOG("Created texture (current node index is {}) with UUID: {}", currentHierarchyIndex,
                              textureUuid.str());
                entity.GetColorTexture() = texture.GetTextureResource();
                aHierarchy.m_textures.push_back(textureUuid);
            }
        }

        for (const aiNode* childNode : eastl::span{aNode.mChildren, aNode.mNumChildren})
        {
            TraverseNode(aStaticMeshPath, aScene, *childNode, aCore, aPipeline, aEntityManager, aHierarchy,
                         currentHierarchyIndex);
        }
    }
}

namespace soge
{
    StaticMeshEntity::StaticMeshEntity(GraphicsCore& aCore, GeometryGraphicsPipeline& aPipeline,
                                       GraphicsModule& aGraphicsModule)
        : m_core{aCore}, m_pipeline{aPipeline}, m_entityManager{aGraphicsModule.GetEntityManager()},
          m_hierarchy{CreateUnique<Hierarchy>()}, m_color{1.0f}, m_shouldReadFromFile{false},
          m_shouldUpdateTransforms{false}
    {
    }

    StaticMeshEntity::StaticMeshEntity(StaticMeshEntity&& aOther) noexcept = default;
    StaticMeshEntity& StaticMeshEntity::operator=(StaticMeshEntity&& aOther) noexcept = default;

    StaticMeshEntity::~StaticMeshEntity() = default;

    Transform StaticMeshEntity::GetTransform() const
    {
        return m_hierarchy->m_transforms[0].m_localTransform;
    }

    Transform& StaticMeshEntity::GetTransform()
    {
        m_shouldUpdateTransforms = true;
        return m_hierarchy->m_transforms[0].m_localTransform;
    }

    glm::vec3 StaticMeshEntity::GetColor() const
    {
        return m_color;
    }

    glm::vec3& StaticMeshEntity::GetColor()
    {
        m_shouldUpdateTransforms = true;
        return m_color;
    }

    const cppfs::FilePath& StaticMeshEntity::GetFilePath() const
    {
        return m_filePath;
    }

    cppfs::FilePath& StaticMeshEntity::GetFilePath()
    {
        m_shouldReadFromFile = true;
        m_shouldUpdateTransforms = true;
        return m_filePath;
    }

    void StaticMeshEntity::Load()
    {
        if (!m_shouldReadFromFile)
        {
            return;
        }
        m_shouldReadFromFile = false;

        Assimp::Importer importer;

        constexpr std::uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
        const aiScene* scene = importer.ReadFile(m_filePath.path(), flags);
        if (scene == nullptr)
        {
            const char* message = importer.GetErrorString();
            SOGE_WARN_LOG("{}", message);
            return;
        }

        const aiNode* node = scene->mRootNode;
        if (node == nullptr)
        {
            SOGE_WARN_LOG(R"(Mesh loaded by path "{}" has no root node)", m_filePath.path());
            return;
        }

        for (auto&& [entityUuid, hierarchyIndex] : m_hierarchy->m_geometryToTransform)
        {
            m_entityManager.get().DestroyEntity(entityUuid);
        }
        for (auto&& entityUuid : m_hierarchy->m_textures)
        {
            m_entityManager.get().DestroyEntity(entityUuid);
        }
        m_hierarchy->Reset();
        TraverseNode(m_filePath, *scene, *node, m_core, m_pipeline, m_entityManager, *m_hierarchy, 0);
    }

    void StaticMeshEntity::WriteResources(nvrhi::ICommandList& aCommandList)
    {
        if (!m_shouldUpdateTransforms)
        {
            return;
        }
        m_shouldUpdateTransforms = false;

        for (auto&& [entityUuid, hierarchyIndex] : m_hierarchy->m_geometryToTransform)
        {
            const auto entity = dynamic_cast<GeometryEntity*>(m_entityManager.get().GetEntity(entityUuid));
            if (entity == nullptr)
            {
                return;
            }
            entity->GetTransform() = m_hierarchy->GetWorldTransform(hierarchyIndex);
            entity->GetColor() = m_color;
        }
    }
}
