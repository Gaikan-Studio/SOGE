#include "sogepch.hpp"
#include "SOGE/Physics/Impl/Jolt/Layers/JoltObjectLayerPairFilter.hpp"
#include "SOGE/Physics/Impl/Jolt/JoltGlobals.hpp"


namespace soge
{
    bool JoltObjectLayerPairFilter::ShouldCollide(JPH::ObjectLayer aInObject1, JPH::ObjectLayer aInObject2) const
    {
        switch (aInObject1)
        {
        case JoltGlobals::NON_MOVING_OBJECTS_LAYER:
            return aInObject2 == JoltGlobals::MOVING_OBJECTS_LAYER;
        case JoltGlobals::MOVING_OBJECTS_LAYER:
            return true;
        default:
            JPH_ASSERT(false);
            return false;
        }
    }
}
