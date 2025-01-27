#include "sogepch.hpp"
#include "SOGE/Physics/Impl/Jolt/Layers/JoltVSBroadPhaseLayerFilter.hpp"
#include "SOGE/Physics/Impl/Jolt/JoltGlobals.hpp"


namespace soge
{
    bool JoltVSBroadPhaseLayerFilter::ShouldCollide(JPH::ObjectLayer aInLayer1, JPH::BroadPhaseLayer aInLayer2) const
    {
        switch (aInLayer1)
        {
        case JoltGlobals::NON_MOVING_OBJECTS_LAYER:
            return aInLayer2 == JoltGlobals::MOVING_BROADPHASE_LAYER;
        case JoltGlobals::MOVING_OBJECTS_LAYER:
            return true;
        default:
            JPH_ASSERT(false);
            return false;
        }
    }
}
