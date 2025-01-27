#include "sogepch.hpp"
#include "SOGE/Physics/Impl/Jolt/Layers/JoltBroadPhaseMapping.hpp"


namespace soge
{
    JoltBroadPhaseMapping::JoltBroadPhaseMapping()
    {
        m_objectToBoradPhase[JoltGlobals::NON_MOVING_OBJECTS_LAYER] = JoltGlobals::NON_MOVING_BROADPHASE_LAYER;
        m_objectToBoradPhase[JoltGlobals::MOVING_OBJECTS_LAYER] = JoltGlobals::MOVING_BROADPHASE_LAYER;
    }

    JoltBroadPhaseMapping::~JoltBroadPhaseMapping()
    {

    }

    uint JoltBroadPhaseMapping::GetNumBroadPhaseLayers() const
    {
        return JoltGlobals::BORADPHASE_LAYER_COUNT;
    }

    JPH::BroadPhaseLayer JoltBroadPhaseMapping::GetBroadPhaseLayer(JPH::ObjectLayer aInLayer) const
    {
        JPH_ASSERT(aInLayer < JoltGlobals::LAYER_COUNT);
        return m_objectToBoradPhase[aInLayer];
    }
}
