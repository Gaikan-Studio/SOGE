#ifndef SOGE_AI_AIMODULE_HPP
#define SOGE_AI_AIMODULE_HPP

#include "SOGE/AI/AiAgent.hpp"
#include "SOGE/Core/Module.hpp"

#include <flecs.h>
#include <flecs/addons/cpp/world.hpp>


namespace soge
{
    class AiModule : public Module
    {
    private:
        flecs::world m_world;

    public:
        explicit AiModule();

        [[nodiscard]]
        AiAgent CreateAgent(eastl::string_view aName = "");
        void DestroyAgent(AiAgent aAgent);

        template <typename... Components, typename Func>
        [[nodiscard]]
        flecs::system CreateAction(const eastl::string_view aName, const Func& aFunc)
        {
            return m_world.system<Components...>(aName.data()).kind(flecs::PostUpdate).each(aFunc);
        }

        virtual void Update();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;
    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, AiModule)

#endif // SOGE_AI_AIMODULE_HPP
