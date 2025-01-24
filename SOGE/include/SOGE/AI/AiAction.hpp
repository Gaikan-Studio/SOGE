#ifndef SOGE_AI_AIACTION_HPP
#define SOGE_AI_AIACTION_HPP

#include <flecs.h>
#include <flecs/addons/cpp/entity.hpp>


namespace soge
{
    class AiAction
    {
    private:
        friend class AiModule;

        explicit AiAction(flecs::entity aEntity);

        flecs::entity m_entity;

    public:
        [[nodiscard]]
        flecs::entity GetEntity() const;

        [[nodiscard]]
        eastl::string_view GetName() const;
        void SetName(eastl::string_view aName);

        bool operator==(const AiAction& aOther) const;
    };
}

#endif // SOGE_AI_AIACTION_HPP
