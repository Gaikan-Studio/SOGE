#include "sogepch.hpp"

#include "SOGE/AI/AiModule.hpp"

#include "SOGE/Core/Timestep.hpp"


namespace soge
{
    AiModule::AiModule()
    {
        m_relationAgentToAction = m_world.entity("Agent to action relation");
        m_relationActionToAgent = m_world.entity("Action to agent relation");

        m_relationAgentToBestAction = m_world.entity("Agent to best action relation");
        m_relationBestActionToAgent = m_world.entity("Best action to agent relation");

        m_relationActionToConsideration = m_world.entity("Action to consideration relation");
        m_relationConsiderationToAction = m_world.entity("Consideration to action relation");

        m_world.system<Agent>("Pick the best action for agent")
            .kind(flecs::OnUpdate)
            .immediate()
            .with(m_relationAgentToAction, flecs::Any)
            .each([this](const flecs::entity aAgent, Agent) {
                SOGE_INFO_LOG(R"([PICK] Agent name is "{}")", aAgent.name().c_str());
                (void)aAgent.remove(m_relationAgentToBestAction, flecs::Wildcard);

                flecs::entity bestAction;
                decltype(Consideration::m_score) bestScore{};

                std::int32_t index{};
                while (flecs::entity action = aAgent.target(m_relationAgentToAction, index++))
                {
                    SOGE_INFO_LOG(R"([PICK] Action name is "{}")", action.name().c_str());
                    (void)action.remove(m_relationBestActionToAgent, flecs::Wildcard);

                    if (flecs::entity consideration = action.target_for<Consideration>(m_relationActionToConsideration))
                    {
                        const auto& [score] = *consideration.get<Consideration>();
                        SOGE_INFO_LOG(R"([PICK] Consideration "{}" score is {})", consideration.name().c_str(), score);
                        if (score > bestScore)
                        {
                            bestScore = score;
                            bestAction = action;
                        }
                    }
                }

                if (!bestAction)
                {
                    return;
                }
                (void)aAgent.add(m_relationAgentToBestAction, bestAction);
                (void)bestAction.add(m_relationBestActionToAgent, aAgent);
            });

        m_world.system<Agent>("Print the best action for agent")
            .kind(flecs::OnUpdate)
            .each([this](const flecs::entity aAgent, Agent) {
                if (const flecs::entity bestAction = aAgent.target(m_relationAgentToBestAction))
                {
                    SOGE_INFO_LOG(R"([PICKED] Best action for agent "{}" is "{}")", aAgent.name().c_str(),
                                  bestAction.name().c_str());
                }
                else
                {
                    SOGE_INFO_LOG(R"([PICKED] No best action for agent "{}")", aAgent.name().c_str());
                }
            });
    }

    void AiModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        SOGE_INFO_LOG("AI module loaded...");
    }

    void AiModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        SOGE_INFO_LOG("AI module unloaded...");
    }

    AiAgent AiModule::CreateAgent(const eastl::string_view aName)
    {
        const auto entity = m_world.entity(aName.data()).add<Agent>();
        return AiAgent{entity};
    }

    eastl::optional<AiAgent> AiModule::GetAgent(const flecs::entity aEntity) const
    {
        if (!aEntity.has<Agent>())
        {
            return eastl::nullopt;
        }
        return AiAgent{aEntity};
    }

    void AiModule::DestroyAgent(AiAgent aAgent)
    {
        const auto entity = aAgent.GetEntity();
        entity.destruct();
    }

    AiAction AiModule::CreateAction(const eastl::string_view aName)
    {
        const auto entity = m_world.entity(aName.data()).add<Action>();
        return AiAction{entity};
    }

    eastl::optional<AiAction> AiModule::GetAction(const flecs::entity aEntity) const
    {
        if (!aEntity.has<Action>())
        {
            return eastl::nullopt;
        }
        return AiAction{aEntity};
    }

    void AiModule::DestroyAction(AiAction aAction)
    {
        const auto entity = aAction.GetEntity();
        entity.destruct();
    }

    AiConsideration AiModule::CreateConsideration(const eastl::string_view aName)
    {
        const auto entity = m_world.entity(aName.data()).set(Consideration{});
        return AiConsideration{entity};
    }

    eastl::optional<AiConsideration> AiModule::GetConsideration(const flecs::entity aEntity) const
    {
        if (!aEntity.has<Consideration>())
        {
            return eastl::nullopt;
        }
        return AiConsideration{aEntity};
    }

    void AiModule::DestroyConsideration(AiConsideration aAgent)
    {
        const auto entity = aAgent.GetEntity();
        entity.destruct();
    }

    void AiModule::AttachActionToAgent(AiAction& aAction, AiAgent& aAgent)
    {
        const auto agent = aAgent.GetEntity();
        const auto action = aAction.GetEntity();

        (void)agent.add(m_relationAgentToAction, action);
        (void)action.add(m_relationActionToAgent, agent);
    }

    void AiModule::DetachActionFromAgent(AiAction& aAction, AiAgent& aAgent)
    {
        const auto agent = aAgent.GetEntity();
        const auto action = aAction.GetEntity();

        (void)agent.remove(m_relationAgentToAction, action);
        (void)action.remove(m_relationActionToAgent, agent);
    }

    void AiModule::AttachConsiderationToAction(AiConsideration& aConsideration, AiAction& aAction)
    {
        const auto consideration = aConsideration.GetEntity();
        const auto action = aAction.GetEntity();

        (void)action.add(m_relationActionToConsideration, consideration);
        (void)consideration.add(m_relationConsiderationToAction, action);
    }

    void AiModule::DetachConsiderationFromAction(AiConsideration& aConsideration, AiAction& aAction)
    {
        const auto consideration = aConsideration.GetEntity();
        const auto action = aAction.GetEntity();

        (void)action.remove(m_relationActionToConsideration, consideration);
        (void)consideration.remove(m_relationConsiderationToAction, action);
    }

    eastl::optional<AiAgent> AiModule::GetAttachedAgent(const AiAction& aAction) const
    {
        if (const flecs::entity agent = aAction.GetEntity().target(m_relationActionToAgent))
        {
            return GetAgent(agent);
        }
        return eastl::nullopt;
    }

    eastl::optional<AiAction> AiModule::GetAttachedAction(const AiAgent& aAgent) const
    {
        if (const flecs::entity action = aAgent.GetEntity().target(m_relationAgentToAction))
        {
            return GetAction(action);
        }
        return eastl::nullopt;
    }

    eastl::optional<AiConsideration> AiModule::GetAttachedConsideration(const AiAction& aAction) const
    {
        if (const flecs::entity consideration = aAction.GetEntity().target(m_relationActionToConsideration))
        {
            return GetConsideration(consideration);
        }
        return eastl::nullopt;
    }

    eastl::optional<AiAction> AiModule::GetAttachedAction(const AiConsideration& aConsideration) const
    {
        if (const flecs::entity action = aConsideration.GetEntity().target(m_relationConsiderationToAction))
        {
            return GetAction(action);
        }
        return eastl::nullopt;
    }

    eastl::optional<AiAction> AiModule::GetBestAction(const AiAgent& aAgent) const
    {
        if (const flecs::entity bestAction = aAgent.GetEntity().target(m_relationAgentToBestAction))
        {
            return GetAction(bestAction);
        }
        return eastl::nullopt;
    }

    eastl::optional<AiAgent> AiModule::GetAgentFromBest(const AiAction& aAction) const
    {
        if (const flecs::entity agent = aAction.GetEntity().target(m_relationBestActionToAgent))
        {
            return GetAgent(agent);
        }
        return eastl::nullopt;
    }

    void AiModule::Update()
    {
        if (!m_world.progress(Timestep::DeltaTime()))
        {
            SOGE_WARN_LOG("AI module ECS world processing failed...");
        }
    }
}
