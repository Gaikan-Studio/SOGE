#ifndef SOGE_SCRIPTING_SCRIPTINGMODULE_HPP
#define SOGE_SCRIPTING_SCRIPTINGMODULE_HPP

#include "SOGE/Core/Module.hpp"


namespace soge
{
    class ScriptingCore;

    class ScriptingModule final : public Module
    {
    private:
        ScriptingCore* m_scriptingCore;

    public:
        explicit ScriptingModule();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;

    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, ScriptingModule)

#endif // !SOGE_SCRIPTING_SCRIPTINGMODULE_HPP
