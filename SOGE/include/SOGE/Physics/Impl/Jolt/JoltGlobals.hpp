#ifndef SOGE_PHYSICS_IMPL_JOLT_JOLTGLOBALS_HPP
#define SOGE_PHYSICS_IMPL_JOLT_JOLTGLOBALS_HPP

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>


namespace soge
{
    // To handle return of jolt uintX types
    using uint      = unsigned int;
    using uint8     = std::uint8_t;
    using uint16    = std::uint16_t;
    using uint32    = std::uint32_t;
    using uint64    = std::uint64_t;

    struct JoltGlobals
    {
        // Layer that objects can be in, determines which other objects it can collide with
        // Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have
        // more layers if you want. E.g. you could have a layer for high detail collision (which is not used by the
        // physics simulation but only if you do collision testing).

        static constexpr JPH::ObjectLayer NON_MOVING_OBJECTS_LAYER = 0;
        static constexpr JPH::ObjectLayer MOVING_OBJECTS_LAYER = 1;
        static constexpr JPH::ObjectLayer LAYER_COUNT = 2;

        static constexpr JPH::BroadPhaseLayer NON_MOVING_BROADPHASE_LAYER =
            JPH::BroadPhaseLayer(NON_MOVING_OBJECTS_LAYER);
        static constexpr JPH::BroadPhaseLayer MOVING_BROADPHASE_LAYER =
            JPH::BroadPhaseLayer(MOVING_OBJECTS_LAYER);
        static constexpr uint BORADPHASE_LAYER_COUNT = 2;
    };
}

#endif // !SOGE_PHYSICS_IMPL_JOLT_JOLTGLOBALS_HPP
