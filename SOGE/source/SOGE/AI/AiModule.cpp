#include "sogepch.hpp"

#include "SOGE/AI/AiModule.hpp"

#include "SOGE/Core/Timestep.hpp"


namespace soge
{
    AiModule::AiModule() = default;

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
        const auto entity = m_world.entity();
        if (!aName.empty())
        {
            (void)entity.set_name(aName.data());
        }

        return AiAgent{entity};
    }

    void AiModule::DestroyAgent(AiAgent aAgent)
    {
        const auto entity = aAgent.GetEntity();
        entity.destruct();
    }

    void AiModule::Update()
    {
        (void)m_world.progress(Timestep::DeltaTime());
    }
}
