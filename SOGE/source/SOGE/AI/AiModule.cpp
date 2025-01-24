#include "sogepch.hpp"

#include "SOGE/AI/AiModule.hpp"

#include "SOGE/AI/Impl/flecs/FlecsAiCore.hpp"
#include "SOGE/DI/Container.hpp"


namespace soge
{
    AiModule::AiModule() : m_aiCore{nullptr}
    {
    }

    void AiModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        SOGE_INFO_LOG("AI module loaded...");

        aContainer.Create<FlecsAiCore>();

        m_aiCore = &aContainer.Provide<AiCore>();
    }

    void AiModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        SOGE_INFO_LOG("AI module unloaded...");

        m_aiCore = nullptr;
    }

    void AiModule::Update()
    {
        if (m_aiCore == nullptr)
        {
            return;
        }

        m_aiCore->Update();
    }

    UniquePtr<AiAgent> AiModule::CreateAgent()
    {
        if (m_aiCore == nullptr)
        {
            return UniquePtr<AiAgent>{};
        }
        return m_aiCore->CreateAgent();
    }
}
