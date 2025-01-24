#include "sogepch.hpp"

#include "SOGE/AI/AiAgent.hpp"


namespace soge
{
    AiAgent::AiAgent(const flecs::entity aEntity) : m_entity{aEntity}
    {
    }

    AiAgent::~AiAgent() noexcept = default;

    flecs::entity AiAgent::GetEntity() const
    {
        return m_entity;
    }

    eastl::string_view AiAgent::GetName() const
    {
        const auto name = m_entity.name();
        return eastl::string_view{name.c_str(), name.size()};
    }

    void AiAgent::SetName(const eastl::string_view aName)
    {
        (void)m_entity.set_name(aName.data());
    }
}
