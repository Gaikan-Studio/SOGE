#ifndef SOGE_PHYSICS_IMPL_JOL_LAYERS_JOLTPHYSICSLAYER_HPP
#define SOGE_PHYSICS_IMPL_JOL_LAYERS_JOLTPHYSICSLAYER_HPP

#include "SOGE/System/Memory.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>


namespace soge
{
    using JoltPhysicsPair = eastl::pair<JPH::ObjectLayer, JPH::BroadPhaseLayer>;

    class JoltPhysicsLayer final
    {
    private:
        JoltPhysicsPair m_pair;
        eastl::string_view m_layerName;
        std::uint16_t m_index;

    public:
        JoltPhysicsLayer(std::uint16_t aIndex, const eastl::string_view& aLayerName);
        ~JoltPhysicsLayer() = default;

        const JoltPhysicsPair& GetLayers() const;
        std::uint16_t GetIndex() const;

    };

    struct JoltLayers
    {
        static const JoltPhysicsLayer NON_MOVING;
        static const JoltPhysicsLayer MOVING;

        static const std::uint16_t LAYER_COUNT;
    };

    const JoltPhysicsLayer JoltLayers::NON_MOVING(0, "NON_MOVING");
    const JoltPhysicsLayer JoltLayers::MOVING(0, "MOVING");
    const std::uint16_t JoltLayers::LAYER_COUNT = 2;

}

#endif // !SOGE_PHYSICS_IMPL_JOL_LAYERS_JOLTPHYSICSLAYER_HPP
