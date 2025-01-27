#include "sogepch.hpp"
#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"

#include "SOGE/Physics/Impl/SoundCompilePreproc.hpp"
#include "SOGE/Physics/PhysicsCore.hpp"
#include "SOGE/Physics/PhysicsModule.hpp"

#include SOGE_ABS_COMPILED_PHYSICS_IMPL_HEADER(SOGE/Physics, PhysicsCore.hpp)


namespace soge
{
    PhysicsModule::PhysicsModule() : m_physicsCore(nullptr)
    {
    }

    PhysicsModule::~PhysicsModule()
    {
    }

    void PhysicsModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        aModuleManager.CreateModule<EventModule>();
        m_physicsCore = &aContainer.Provide<ImplPhysicsCore>();
        SOGE_INFO_LOG("Physics module loaded...");
    }

    void PhysicsModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_physicsCore = nullptr;
        SOGE_INFO_LOG("Physics module unloaded...");
    }

    void PhysicsModule::Update() const
    {
        m_physicsCore->Update();
    }
}
