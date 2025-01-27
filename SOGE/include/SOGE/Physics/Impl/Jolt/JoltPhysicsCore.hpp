#ifndef SOGE_PHYSICS_IMPL_JOLT_JOLTPHYSICSCORE_HPP
#define SOGE_PHYSICS_IMPL_JOLT_JOLTPHYSICSCORE_HPP

#include "SOGE/Physics/PhysicsCore.hpp"

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Memory.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>


//#define JPH_FLOATING_POINT_EXCEPTIONS_ENABLED
//#define JPH_DEBUG_RENDERER
//#define JPH_PROFILE_ENABLED
//#define JPH_OBJECT_STREAM
//#define JPH_USE_AVX2
//#define JPH_USE_AVX
//#define JPH_USE_SSE4_1
//#define JPH_USE_SSE4_2
//#define JPH_USE_LZCNT
//#define JPH_USE_TZCNT
//#define JPH_USE_F16C
//#define JPH_USE_FMADD


namespace soge
{
    class JoltPhysicsCore final : public PhysicsCore
    {
        struct JoltConfig
        {
            const std::uint64_t m_maxBodies               = 65536;
            const std::uint64_t m_numBodyMutexes          = 0;
            const std::uint64_t m_maxBodyPairs            = 65536;
            const std::uint64_t m_maxContactConstraints   = 65536;
        };

    private:
        JPH::PhysicsSystem m_physicsSystem;
        JPH::TempAllocatorImpl* m_tempAllocator;
        JPH::JobSystemThreadPool* m_jobSystemThreadPool;
        JoltConfig m_config;

    public:
        explicit JoltPhysicsCore();

        JoltPhysicsCore(const JoltPhysicsCore&) = delete;
        JoltPhysicsCore& operator=(const JoltPhysicsCore&) = delete;

        JoltPhysicsCore(JoltPhysicsCore&& aOther) noexcept;
        JoltPhysicsCore& operator=(JoltPhysicsCore&& aOther) noexcept;

        ~JoltPhysicsCore() override;

        void Update() override;

    };

    using ImplPhysicsCore = JoltPhysicsCore;

}

SOGE_DI_REGISTER_NS(soge, JoltPhysicsCore, df::Single<JoltPhysicsCore>, tag::Overrides<JoltPhysicsCore, PhysicsCore>)

#endif // !SOGE_PHYSICS_IMPL_JOLT_JOLTPHYSICSCORE_HPP
