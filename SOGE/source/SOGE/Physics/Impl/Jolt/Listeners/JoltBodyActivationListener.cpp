#include "sogepch.hpp"
#include "SOGE/Physics/Impl/Jolt/Listeners/JoltBodyActivationListener.hpp"


namespace soge
{
    void JoltBodyActivationListener::OnBodyActivated(const JPH::BodyID& aInBodyID, JPH::uint64 aInBodyUserData)
    {
        SOGE_INFO_LOG("A body got activated");
    }

    void JoltBodyActivationListener::OnBodyDeactivated(const JPH::BodyID& aInBodyID, JPH::uint64 aInBodyUserData)
    {
        SOGE_INFO_LOG("A body went to sleep");
    }
}
