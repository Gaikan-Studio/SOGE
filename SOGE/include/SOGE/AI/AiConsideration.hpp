#ifndef SOGE_AI_AICONSIDERATION_HPP
#define SOGE_AI_AICONSIDERATION_HPP

#include <flecs.h>
#include <flecs/addons/cpp/entity.hpp>


namespace soge
{
    class AiConsideration
    {
    private:
        friend class AiModule;

        explicit AiConsideration(flecs::entity aEntity);

        flecs::entity m_entity;

    public:
        [[nodiscard]]
        flecs::entity GetEntity() const;

        [[nodiscard]]
        eastl::string_view GetName() const;
        void SetName(eastl::string_view aName);

        bool operator==(const AiConsideration& aOther) const;
    };
}

#endif // SOGE_AI_AICONSIDERATION_HPP
