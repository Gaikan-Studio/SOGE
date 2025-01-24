#ifndef SOGE_AI_AIAGENT_HPP
#define SOGE_AI_AIAGENT_HPP

#include <flecs.h>
#include <flecs/addons/cpp/entity.hpp>


namespace soge
{
    class AiAgent
    {
    private:
        friend class AiModule;

        explicit AiAgent(flecs::entity aEntity);

        flecs::entity m_entity;

    public:
        AiAgent(const AiAgent&) = delete;
        AiAgent& operator=(const AiAgent&) = delete;

        AiAgent(AiAgent&&) noexcept = default;
        AiAgent& operator=(AiAgent&&) noexcept = default;

        ~AiAgent() noexcept;

        [[nodiscard]]
        flecs::entity GetEntity() const;

        [[nodiscard]]
        eastl::string_view GetName() const;
        void SetName(eastl::string_view aName);
    };
}

#endif // SOGE_AI_AIAGENT_HPP
