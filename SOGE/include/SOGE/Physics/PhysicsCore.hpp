#ifndef SOGE_PHYSICS_PHYSICSCORE_HPP
#define SOGE_PHYSICS_PHYSICSCORE_HPP

#include "SOGE/DI/Dependency.hpp"


namespace soge
{
    class PhysicsCore
    {
    public:
        constexpr explicit PhysicsCore() noexcept                       = default;

        constexpr PhysicsCore(const PhysicsCore&) noexcept              = delete;
        constexpr PhysicsCore& operator=(const PhysicsCore&) noexcept   = delete;

        constexpr PhysicsCore(PhysicsCore&&) noexcept                   = default;
        constexpr PhysicsCore& operator=(PhysicsCore&&) noexcept        = default;

        constexpr virtual ~PhysicsCore() noexcept                       = default;

        virtual void Update() = 0;

    };
}

SOGE_DI_REGISTER_NS(soge, PhysicsCore, df::Abstract<PhysicsCore>)

#endif // !SOGE_PHYSICS_PHYSICSCORE_HPP
