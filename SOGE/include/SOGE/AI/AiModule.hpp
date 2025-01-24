#ifndef SOGE_AI_AIMODULE_HPP
#define SOGE_AI_AIMODULE_HPP

#include "SOGE/AI/AiAction.hpp"
#include "SOGE/AI/AiAgent.hpp"
#include "SOGE/AI/AiConsideration.hpp"
#include "SOGE/Core/Module.hpp"

#include <flecs.h>
#include <flecs/addons/cpp/world.hpp>

#include <EASTL/optional.h>


namespace soge
{
    class AiModule : public Module
    {
    private:
        struct Agent
        {
        };

        struct Action
        {
        };

        flecs::entity m_relationAgentToAction;
        flecs::entity m_relationActionToAgent;

        flecs::entity m_relationAgentToBestAction;
        flecs::entity m_relationBestActionToAgent;

        flecs::entity m_relationActionToConsideration;
        flecs::entity m_relationConsiderationToAction;

        flecs::world m_world;

    public:
        struct Consideration
        {
            float m_score{};
        };

        explicit AiModule();

        [[nodiscard]]
        AiAgent CreateAgent(eastl::string_view aName = {});
        [[nodiscard]]
        eastl::optional<AiAgent> GetAgent(flecs::entity aEntity) const;
        void DestroyAgent(AiAgent aAgent);

        [[nodiscard]]
        AiAction CreateAction(eastl::string_view aName = {});
        [[nodiscard]]
        eastl::optional<AiAction> GetAction(flecs::entity aEntity) const;
        void DestroyAction(AiAction aAction);

        [[nodiscard]]
        AiConsideration CreateConsideration(eastl::string_view aName = {});
        [[nodiscard]]
        eastl::optional<AiConsideration> GetConsideration(flecs::entity aEntity) const;
        void DestroyConsideration(AiConsideration aAgent);

        void AttachActionToAgent(AiAction& aAction, AiAgent& aAgent);
        void DetachActionFromAgent(AiAction& aAction, AiAgent& aAgent);

        void AttachConsiderationToAction(AiConsideration& aConsideration, AiAction& aAction);
        void DetachConsiderationFromAction(AiConsideration& aConsideration, AiAction& aAction);

        [[nodiscard]]
        eastl::optional<AiAgent> GetAttachedAgent(const AiAction& aAction) const;
        [[nodiscard]]
        eastl::optional<AiAction> GetAttachedAction(const AiAgent& aAgent) const;
        [[nodiscard]]
        eastl::optional<AiConsideration> GetAttachedConsideration(const AiAction& aAction) const;
        [[nodiscard]]
        eastl::optional<AiAction> GetAttachedAction(const AiConsideration& aConsideration) const;

        [[nodiscard]]
        eastl::optional<AiAction> GetBestAction(const AiAgent& aAgent) const;
        [[nodiscard]]
        eastl::optional<AiAgent> GetAgentFromBest(const AiAction& aAction) const;

        template <typename... Components>
        [[nodiscard]]
        flecs::system_builder<Consideration, Components...> CreateConsiderationSystem(eastl::string_view aName = {});

        template <typename... Components>
        [[nodiscard]]
        flecs::system_builder<Components...> CreateActionSystem(eastl::string_view aName = {});

        virtual void Update();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;
    };

    template <typename... Components>
    auto AiModule::CreateConsiderationSystem(const eastl::string_view aName)
        -> flecs::system_builder<Consideration, Components...>
    {
        return m_world.system<Consideration, Components...>(aName.data())
            .kind(flecs::PreUpdate)
            .with(m_relationConsiderationToAction, flecs::Wildcard);
    }

    template <typename... Components>
    flecs::system_builder<Components...> AiModule::CreateActionSystem(const eastl::string_view aName)
    {
        return m_world.system<Components...>(aName.data())
            .kind(flecs::PostUpdate)
            .with(m_relationBestActionToAgent, flecs::Wildcard);
    }
}

SOGE_DI_REGISTER_MODULE_NS(soge, AiModule)

#endif // SOGE_AI_AIMODULE_HPP
