#include "sogepch.hpp"
#include "SOGE/Physics/Impl/Jolt/Layers/JoltPhysicsLayer.hpp"


namespace soge
{
    JoltPhysicsLayer::JoltPhysicsLayer(std::uint16_t aIndex, const eastl::string_view& aLayerName)
    {
        m_pair.first = aIndex;
        m_pair.second = JPH::BroadPhaseLayer(aIndex);
        m_layerName = aLayerName;
    }

    const JoltPhysicsPair& JoltPhysicsLayer::GetLayers() const
    {
        return m_pair;
    }

    std::uint16_t JoltPhysicsLayer::GetIndex() const
    {
        return m_index;
    }
}
