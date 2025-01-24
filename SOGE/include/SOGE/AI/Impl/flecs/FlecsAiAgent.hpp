#ifndef SOGE_AI_IMPL_FLECS_FLECSAIAGENT_HPP
#define SOGE_AI_IMPL_FLECS_FLECSAIAGENT_HPP

#include "SOGE/AI/AiAgent.hpp"

#include <flecs.h>
#include <flecs/addons/cpp/world.hpp>


namespace soge
{
    class FlecsAiAgent : public AiAgent
    {
    private:
        flecs::entity m_entity;

    public:
        explicit FlecsAiAgent(const flecs::world& aWorld);

        FlecsAiAgent(const FlecsAiAgent&) = delete;
        FlecsAiAgent& operator=(const FlecsAiAgent&) = delete;

        FlecsAiAgent(FlecsAiAgent&&) noexcept = default;
        FlecsAiAgent& operator=(FlecsAiAgent&&) noexcept = default;

        ~FlecsAiAgent() override;

        [[nodiscard]]
        flecs::entity GetEntity() const;
    };
}

#endif // SOGE_AI_IMPL_FLECS_FLECSAIAGENT_HPP
