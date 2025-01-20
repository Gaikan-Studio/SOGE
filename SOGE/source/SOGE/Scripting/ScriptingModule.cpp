#include "sogepch.hpp"
#include "SOGE/Scripting/ScriptingModule.hpp"
#include "SOGE/Scripting/ScriptingCore.hpp"

#include "SOGE/DI/Container.hpp"
#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/Event/EventModule.hpp"

#include "SOGE/Utils/PreprocessorHelpers.hpp"
#include "SOGE/Scripting/Dev/ScriptingCompilePreproc.hpp"


#include SOGE_ABS_COMPILED_SCRIPTING_IMPL_HEADER(SOGE/Scripting, ScriptingCore.hpp);


namespace soge
{
    ScriptingModule::ScriptingModule() : m_scriptingCore(nullptr)
    {
    }

    void ScriptingModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        SOGE_LOG_TITLE_DIVIDER(SCRIPTING MODULE)

        aModuleManager.CreateModule<EventModule>();
        aContainer.Create<ImplScriptingCore>();
        m_scriptingCore = &aContainer.Provide<ScriptingCore>();

        SOGE_INFO_LOG("Scripting module loaded...");
    }

    void ScriptingModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_scriptingCore = nullptr;
        SOGE_INFO_LOG("Scripting module unloaded...");
    }
}
