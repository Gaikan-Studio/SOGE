#ifndef SOGE_PHYSICS_IMPL_JOLT_LAYERS_JOLTVSBROADPHASELAYERFILTER_HPP
#define SOGE_PHYSICS_IMPL_JOLT_LAYERS_JOLTVSBROADPHASELAYERFILTER_HPP

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>


namespace soge
{
    class JoltVSBroadPhaseLayerFilter final : public JPH::ObjectVsBroadPhaseLayerFilter
    {
    public:
        JoltVSBroadPhaseLayerFilter() = default;
        ~JoltVSBroadPhaseLayerFilter() = default;

        virtual bool ShouldCollide(JPH::ObjectLayer aInLayer1, JPH::BroadPhaseLayer aInLayer2) const override;

    };
}

#endif // !SOGE_PHYSICS_IMPL_JOLT_LAYERS_JOLTVSBROADPHASELAYERFILTER_HPP
