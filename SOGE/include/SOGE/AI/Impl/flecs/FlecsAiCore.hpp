#ifndef SOGE_AI_IMPL_FLECS_FLECSAICORE_HPP
#define SOGE_AI_IMPL_FLECS_FLECSAICORE_HPP

#include "SOGE/AI/AiCore.hpp"

#include <flecs.h>
#include <flecs/addons/cpp/world.hpp>


namespace soge
{
    class FlecsAiCore : public AiCore
    {
    private:
        flecs::world m_world;

    public:
        explicit FlecsAiCore();

        [[nodiscard]]
        UniquePtr<AiAgent> CreateAgent() override;

        void Update() override;
    };
}

SOGE_DI_REGISTER_NS(soge, FlecsAiCore, df::Single<FlecsAiCore>, tag::Overrides<FlecsAiCore, AiCore>)

#endif // SOGE_AI_IMPL_FLECS_FLECSAICORE_HPP
