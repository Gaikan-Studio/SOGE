#ifndef SOGE_PHYSICS_PHYSICSMODULE_HPP
#define SOGE_PHYSICS_PHYSICSMODULE_HPP

#include "SOGE/Event/EventModule.hpp"


namespace soge
{
    class PhysicsCore;
    class PhysicsModule final : public Module
    {
    private:
        PhysicsCore* m_physicsCore;

    public:
        explicit PhysicsModule();
        ~PhysicsModule();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Update() const;

    };

}

SOGE_DI_REGISTER_MODULE_NS(soge, PhysicsModule);

#endif // !SOGE_PHYSICS_PHYSICSMODULE_HPP
