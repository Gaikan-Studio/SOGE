#include "sogepch.hpp"
#include "SOGE/Scripting/Impl/Mono/MonoScriptingCore.hpp"


namespace soge
{
    MonoScriptingCore::MonoScriptingCore()
    {
        mono_set_dirs("mono/4.5", "mono/4.5");
        //m_monoDomain = SharedPtr<MonoDomain>(mono_jit_init("ProjectName"));
        m_monoDomain = mono_jit_init("GAME");
        if (!m_monoDomain)
            SOGE_ERROR_LOG("Failed to initialize MONO domain...");
    }

    MonoScriptingCore::~MonoScriptingCore()
    {
        if (m_monoDomain)
            mono_jit_cleanup(m_monoDomain);
    }

    void MonoScriptingCore::Init()
    {
    }
}
