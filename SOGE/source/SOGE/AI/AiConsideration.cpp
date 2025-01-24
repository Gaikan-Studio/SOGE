#include "sogepch.hpp"

#include "SOGE/AI/AiConsideration.hpp"


namespace soge
{
    AiConsideration::AiConsideration(const flecs::entity aEntity) : m_entity{aEntity}
    {
    }

    flecs::entity AiConsideration::GetEntity() const
    {
        return m_entity;
    }

    eastl::string_view AiConsideration::GetName() const
    {
        const auto name = m_entity.name();
        return eastl::string_view{name.c_str(), name.size()};
    }

    void AiConsideration::SetName(const eastl::string_view aName)
    {
        (void)m_entity.set_name(aName.data());
    }

    bool AiConsideration::operator==(const AiConsideration& aOther) const
    {
        return m_entity.id() == aOther.m_entity.id() && m_entity.world() == aOther.m_entity.world();
    }
}
