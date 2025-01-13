#ifndef SOGE_GRAPHICS_GRAPHICSENTITYMANAGER_HPP
#define SOGE_GRAPHICS_GRAPHICSENTITYMANAGER_HPP

#include "SOGE/Graphics/GraphicsEntity.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Utils/UUID.hpp"

#include <EASTL/functional.h>
#include <EASTL/hash_map.h>
#include <EASTL/span.h>
#include <EASTL/vector.h>


namespace soge
{
    class GraphicsEntityManager
    {
    private:
        using Key = UUIDv4::UUID;
        using UniqueEntity = UniquePtr<GraphicsEntity>;

        Key CreateEntity(UniqueEntity aEntity);

        using Entities = eastl::hash_map<Key, UniqueEntity>;
        Entities m_entities;

        using EntityRefs = eastl::vector<eastl::reference_wrapper<GraphicsEntity>>;
        EntityRefs m_entityRefs;

    public:
        template <typename T, typename... Args>
        [[nodiscard]]
        eastl::pair<T&, Key> CreateEntity(Args&&... args);

        [[nodiscard]]
        GraphicsEntity* GetEntity(const Key& aKey) const;

        UniqueEntity DestroyEntity(const Key& aKey);

        void Clear();

        using EntitiesSpan = eastl::span<eastl::reference_wrapper<GraphicsEntity>>;
        [[nodiscard]]
        EntitiesSpan GetEntities() noexcept;
    };

    template <typename T, typename... Args>
    auto GraphicsEntityManager::CreateEntity(Args&&... args) -> eastl::pair<T&, Key>
    {
        UniquePtr<T> entity = CreateUnique<T>(std::forward<Args>(args)...);
        T* entityPtr = entity.get();

        const auto key = CreateEntity(std::move(entity));
        return {*entityPtr, key};
    }
}

#endif // SOGE_GRAPHICS_GRAPHICSENTITYMANAGER_HPP
