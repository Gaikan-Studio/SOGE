#ifndef SOGE_SCRIPTING_IMPL_MONO_MONOSCRIPTINGCORE_HPP
#define SOGE_SCRIPTING_IMPL_MONO_MONOSCRIPTINGCORE_HPP

#include "SOGE/Scripting/ScriptingCore.hpp"
#include "SOGE/System/Memory.hpp"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>


namespace soge
{
    class MonoScriptingCore final : public ScriptingCore
    {
    private:
        //SharedPtr<MonoDomain> m_monoDomain;
        //SharedPtr<MonoAssembly> m_monoAssembly;
        //SharedPtr<MonoImage> m_monoImage;
        MonoDomain* m_monoDomain;

    public:
        explicit MonoScriptingCore();

        MonoScriptingCore(const MonoScriptingCore&) = delete;
        MonoScriptingCore& operator=(const MonoScriptingCore&) = delete;
        ~MonoScriptingCore() override;

        void Init() override;

    };

    using ImplScriptingCore = MonoScriptingCore;

}

SOGE_DI_REGISTER_NS(soge, MonoScriptingCore, df::Single<MonoScriptingCore>,
                    tag::Overrides<MonoScriptingCore, ScriptingCore>)

#endif // !SOGE_SCRIPTING_IMPL_MONO_MONOSCRIPTINGCORE_HPP
