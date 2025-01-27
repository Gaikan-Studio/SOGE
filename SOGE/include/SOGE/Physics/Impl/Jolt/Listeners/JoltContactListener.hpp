#ifndef SOGE_PHYSICS_IMPL_JOLT_LISTENERS_JOLTCONTACTLISTENER_HPP
#define SOGE_PHYSICS_IMPL_JOLT_LISTENERS_JOLTCONTACTLISTENER_HPP

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>


namespace soge
{
    class JoltContactListener final : public JPH::ContactListener
    {
    public:
        virtual JPH::ValidateResult OnContactValidate(const JPH::Body& aInBody1, const JPH::Body& aInBody2,
                                                      JPH::RVec3Arg aInBaseOffset,
                                                      const JPH::CollideShapeResult& aInCollisionResult) override;

        virtual void OnContactAdded(const JPH::Body& aInBody1, const JPH::Body& aInBody2,
                                    const JPH::ContactManifold& aInManifold,
                                    JPH::ContactSettings& aIoSettings) override;

        virtual void OnContactPersisted(const JPH::Body& aInBody1, const JPH::Body& aInBody2,
                                        const JPH::ContactManifold& aInManifold,
                                        JPH::ContactSettings& aIoSettings) override;

        virtual void OnContactRemoved(const JPH::SubShapeIDPair& aInSubShapePair) override;

    };
}

#endif // !SOGE_PHYSICS_IMPL_JOLT_LISTENERS_JOLTCONTACTLISTENER_HPP
