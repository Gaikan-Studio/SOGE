#include "sogepch.hpp"
#include "SOGE/Physics/Impl/Jolt/Listeners/JoltContactListener.hpp"


namespace soge
{
    JPH::ValidateResult JoltContactListener::OnContactValidate(const JPH::Body& aInBody1, const JPH::Body& aInBody2,
                                                               JPH::RVec3Arg aInBaseOffset,
                                                               const JPH::CollideShapeResult& aInCollisionResult)
    {
        SOGE_TRACE_LOG("Contact validate callback");
        return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
    }

    void JoltContactListener::OnContactAdded(const JPH::Body& aInBody1, const JPH::Body& aInBody2,
                                             const JPH::ContactManifold& aInManifold, JPH::ContactSettings& aIoSettings)
    {
        SOGE_TRACE_LOG("A contact was added");
    }

    void JoltContactListener::OnContactPersisted(const JPH::Body& aInBody1, const JPH::Body& aInBody2,
                                                 const JPH::ContactManifold& aInManifold,
                                                 JPH::ContactSettings& aIoSettings)
    {
        SOGE_TRACE_LOG("A contact was persisted");
    }

    void JoltContactListener::OnContactRemoved(const JPH::SubShapeIDPair& aInSubShapePair)
    {
        SOGE_TRACE_LOG("A contact was removed");
    }
}
