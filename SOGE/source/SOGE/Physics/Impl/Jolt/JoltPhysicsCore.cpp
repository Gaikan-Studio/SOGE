#include "sogepch.hpp"
#include "SOGE/Physics/Impl/Jolt/JoltPhysicsCore.hpp"
#include "SOGE/Physics/Impl/Jolt/Layers/JoltBroadPhaseMapping.hpp"
#include "SOGE/Physics/Impl/Jolt/Layers/JoltVSBroadPhaseLayerFilter.hpp"
#include "SOGE/Physics/Impl/Jolt/Layers/JoltObjectLayerPairFilter.hpp"

#include "SOGE/Physics/Impl/Jolt/Listeners/JoltBodyActivationListener.hpp"
#include "SOGE/Physics/Impl/Jolt/Listeners/JoltContactListener.hpp"

#include <thread>
#include <Jolt/Core/Core.h>


namespace soge
{
    using uint64 = std::uint64_t;
    JoltPhysicsCore::JoltPhysicsCore()
    {
        JPH::RegisterDefaultAllocator();
        JPH::Factory::sInstance = new JPH::Factory();

        std::uint64_t joltVersionId = JPH_VERSION_ID;
        SOGE_INFO_LOG("Jolt version id: {}", joltVersionId);

        JPH::RegisterTypes();
        m_tempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
        m_jobSystemThreadPool = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers,
                                                             std::thread::hardware_concurrency() - 1);

        JoltBroadPhaseMapping broadPhaseLayerInterface;
        JoltVSBroadPhaseLayerFilter vsBroadPhaseFilter;
        JoltObjectLayerPairFilter vsObjectLayerFilter;

        m_physicsSystem.Init(m_config.m_maxBodies, m_config.m_numBodyMutexes, m_config.m_maxBodyPairs,
                             m_config.m_maxContactConstraints, broadPhaseLayerInterface, vsBroadPhaseFilter,
                             vsObjectLayerFilter);

        JoltBodyActivationListener bodyActivationListener;
        m_physicsSystem.SetBodyActivationListener(&bodyActivationListener);

        JoltContactListener contactListener;
        m_physicsSystem.SetContactListener(&contactListener);

        JPH::BodyInterface& bodyInterface = m_physicsSystem.GetBodyInterface();

    }

    JoltPhysicsCore::JoltPhysicsCore(JoltPhysicsCore&& aOther) noexcept
    {
    }

    JoltPhysicsCore& JoltPhysicsCore::operator=(JoltPhysicsCore&& aOther) noexcept
    {
        return *this;
    }

    JoltPhysicsCore::~JoltPhysicsCore()
    {
    }

    void JoltPhysicsCore::Update()
    {
    }
}
