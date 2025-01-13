#include "sogepch.hpp"

#include "SOGE/Graphics/GraphicsEntityManager.hpp"


namespace soge
{
    auto GraphicsEntityManager::CreateEntity(UniqueEntity aEntity) -> Key
    {
        const auto key = UUID::Generate();

        const auto [iter, created] = m_entities.try_emplace(key, std::move(aEntity));
        if (created)
        {
            m_entityRefs.push_back(*iter->second);
        }

        return key;
    }

    auto GraphicsEntityManager::GetEntity(const Key& aKey) const -> GraphicsEntity*
    {
        const auto iter = m_entities.find(aKey);
        if (iter == m_entities.end())
        {
            return nullptr;
        }

        return iter->second.get();
    }

    auto GraphicsEntityManager::DestroyEntity(const Key& aKey) -> UniqueEntity
    {
        const auto iter = m_entities.find(aKey);
        if (iter == m_entities.end())
        {
            return UniqueEntity{};
        }

        UniqueEntity entity = std::move(iter->second);
        m_entities.erase(iter);

        auto predicate = [&entity](auto&& aItem) { return entity.get() == &aItem.get(); };
        m_entityRefs.erase(eastl::remove_if(m_entityRefs.begin(), m_entityRefs.end(), predicate), m_entityRefs.end());

        return entity;
    }

    void GraphicsEntityManager::Clear()
    {
        m_entityRefs.clear();
        m_entities.clear();
    }

    auto GraphicsEntityManager::GetEntities() noexcept -> EntitiesSpan
    {
        return m_entityRefs;
    }
}
