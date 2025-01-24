#include "sogepch.hpp"

#include "SOGE/AI/AiAction.hpp"


namespace soge
{
    AiAction::AiAction(const flecs::entity aEntity) : m_entity{aEntity}
    {
    }

    flecs::entity AiAction::GetEntity() const
    {
        return m_entity;
    }

    eastl::string_view AiAction::GetName() const
    {
        const auto name = m_entity.name();
        return eastl::string_view{name.c_str(), name.size()};
    }

    void AiAction::SetName(const eastl::string_view aName)
    {
        (void)m_entity.set_name(aName.data());
    }

    bool AiAction::operator==(const AiAction& aOther) const
    {
        return m_entity.id() == aOther.m_entity.id() && m_entity.world() == aOther.m_entity.world();
    }
}
