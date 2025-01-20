#ifndef SOGE_SCRIPTING_SCRIPTINGCORE_HPP
#define SOGE_SCRIPTING_SCRIPTINGCORE_HPP

#include "SOGE/DI/Dependency.hpp"


namespace soge
{
    class ScriptingCore
    {
    public:
        constexpr explicit ScriptingCore() noexcept = default;

        constexpr ScriptingCore(const ScriptingCore&) noexcept = delete;
        constexpr ScriptingCore& operator=(const ScriptingCore&) noexcept = delete;

        constexpr ScriptingCore(ScriptingCore&&) noexcept = default;
        constexpr ScriptingCore& operator=(ScriptingCore&&) noexcept = default;

        constexpr virtual ~ScriptingCore() noexcept = default;

        virtual void Init() = 0;

    };
}

SOGE_DI_REGISTER_NS(soge, ScriptingCore, df::Abstract<ScriptingCore>)

#endif // !SOGE_SCRIPTING_SCRIPTINGCORE_HPP
