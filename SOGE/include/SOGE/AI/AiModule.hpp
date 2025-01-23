#ifndef SOGE_AI_AIMODULE_HPP
#define SOGE_AI_AIMODULE_HPP

#include "SOGE/AI/AiCore.hpp"
#include "SOGE/Core/Module.hpp"


namespace soge
{
    class AiModule : public Module
    {
    private:
        AiCore* m_aiCore;

    public:
        explicit AiModule();

        virtual void Update();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;
    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, AiModule)

#endif // SOGE_AI_AIMODULE_HPP
