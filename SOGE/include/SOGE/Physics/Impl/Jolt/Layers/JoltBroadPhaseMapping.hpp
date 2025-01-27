#ifndef SOGE_PHYSICS_IMPL_JOLT_LAYERS_JOLTBROADPHASEMAPPING_HPP
#define SOGE_PHYSICS_IMPL_JOLT_LAYERS_JOLTBROADPHASEMAPPING_HPP

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>

#include "SOGE/Physics/Impl/Jolt/JoltGlobals.hpp"


namespace soge
{
    /**
    * @class BroadPhaseLayers
    * @brief Defines a mapping between object and broadphase layers.
    */

    class JoltBroadPhaseMapping final : public JPH::BroadPhaseLayerInterface
    {
    private:
        JPH::BroadPhaseLayer m_objectToBoradPhase[JoltGlobals::LAYER_COUNT];

    public:
        JoltBroadPhaseMapping();
        ~JoltBroadPhaseMapping();

        virtual uint GetNumBroadPhaseLayers() const override;
        virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer aInLayer) const override;

        #if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
        virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer aInLayer) const override
        {
            switch (static_cast<JPH::BroadPhaseLayer::Type>(aInLayer))
            {
                case static_cast<JPH::BroadPhaseLayer::Type>(JoltGlobals::NON_MOVING_BROADPHASE_LAYER):
                    return "NON_MOVING_OBJECTS_LAYER";
                case static_cast<JPH::BroadPhaseLayer::Type>(JoltGlobals::MOVING_BROADPHASE_LAYER):
                    return "MOVING_OBJECTS_LAYER";
                default:
                    JPH_ASSERT(false);
                    return "INVALID";
            }

            return "INVALID";
        }
        #endif

    };
}

#endif // SOGE_PHYSICS_IMPL_JOLT_LAYERS_JOLTBROADPHASEMAPPING_HPP
