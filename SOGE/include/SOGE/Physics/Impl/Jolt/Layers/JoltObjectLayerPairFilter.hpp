#ifndef SOGE_PHYSICS_IMPL_JOLT_LAYERS_JOLTOBJECTLAYERPAIRFILTER_HPP
#define SOGE_PHYSICS_IMPL_JOLT_LAYERS_JOLTOBJECTLAYERPAIRFILTER_HPP

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>


namespace soge
{
    class JoltObjectLayerPairFilter final : public JPH::ObjectLayerPairFilter
    {
    public:
        JoltObjectLayerPairFilter() = default;
        ~JoltObjectLayerPairFilter() = default;

        virtual bool ShouldCollide(JPH::ObjectLayer aInObject1, JPH::ObjectLayer aInObject2) const override;

    };
}

#endif // !SOGE_PHYSICS_IMPL_JOLT_LAYERS_JOLTOBJECTLAYERPAIRFILTER_HPP
