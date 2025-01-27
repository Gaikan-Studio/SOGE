#ifndef SOGE_PHYSICS_IMPL_JOLT_LISTENERS_JOLTBODYACTIVATIONLISTENER_HPP
#define SOGE_PHYSICS_IMPL_JOLT_LISTENERS_JOLTBODYACTIVATIONLISTENER_HPP

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>


namespace soge
{
    class JoltBodyActivationListener final : public JPH::BodyActivationListener
    {
    public:
        JoltBodyActivationListener() = default;
        ~JoltBodyActivationListener() = default;

        virtual void OnBodyActivated(const JPH::BodyID& aInBodyID, JPH::uint64 aInBodyUserData) override;
        virtual void OnBodyDeactivated(const JPH::BodyID& aInBodyID, JPH::uint64 aInBodyUserData) override;

    };
}

#endif // SOGE_PHYSICS_IMPL_JOLT_LISTENERS_JOLTBODYACTIVATIONLISTENER_HPP
