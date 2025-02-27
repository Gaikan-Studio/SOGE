#ifndef SOGE_INPUT_INPUTMODULE_HPP
#define SOGE_INPUT_INPUTMODULE_HPP

#include "SOGE/Event/EventModule.hpp"
#include "SOGE/Input/InputCore.hpp"


namespace soge
{
    class Keyboard;
    class Gamepad;
    class Mouse;

    class InputModule : public Module
    {
    private:
        InputCore* m_inputCore;

    public:
        explicit InputModule();

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;

        [[nodiscard]]
        bool IsKeyPressed(const Key& aKey) const;
        [[nodiscard]]
        bool IsKeyReleased(const Key& aKey) const;

        [[nodiscard]]
        Keyboard* GetKeyboard() const;
        [[nodiscard]]
        Gamepad* GetGamepad() const;
        [[nodiscard]]
        Mouse* GetMouse() const;

        void Update() const;
    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, InputModule)

#endif // SOGE_INPUT_INPUTMODULE_HPP
