#ifndef SOGE_AI_AICORE_HPP
#define SOGE_AI_AICORE_HPP

#include "SOGE/DI/Dependency.hpp"


namespace soge
{
    class AiCore
    {
    public:
        constexpr explicit AiCore() noexcept = default;

        constexpr AiCore(const AiCore&) noexcept = delete;
        constexpr AiCore& operator=(const AiCore&) noexcept = delete;

        constexpr AiCore(AiCore&&) noexcept = default;
        constexpr AiCore& operator=(AiCore&&) noexcept = default;

        constexpr virtual ~AiCore() noexcept = default;

        constexpr virtual void Update() = 0;
    };
}

SOGE_DI_REGISTER_NS(soge, AiCore, df::Abstract<AiCore>)

#endif // SOGE_AI_AICORE_HPP
