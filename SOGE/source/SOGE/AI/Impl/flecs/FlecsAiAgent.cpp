#include "sogepch.hpp"

#include "SOGE/AI/Impl/flecs/FlecsAiAgent.hpp"


namespace soge
{
    FlecsAiAgent::FlecsAiAgent(const flecs::world& aWorld) : m_entity{aWorld.entity()}
    {
    }

    FlecsAiAgent::~FlecsAiAgent()
    {
        auto entity = m_entity;
        m_entity.world().defer([entity] { entity.destruct(); });
    }

    flecs::entity FlecsAiAgent::GetEntity() const
    {
        return m_entity;
    }
}
