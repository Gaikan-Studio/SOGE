#ifndef SOGE_INPUT_INPUTMANAGER_HPP
#define SOGE_INPUT_INPUTMANAGER_HPP

// Abstract
#include "SOGE/Input/Device/Keyboard.hpp"
#include "SOGE/Input/Device/Gamepad.hpp"
#include "SOGE/Input/Device/Mouse.hpp"
#include "SOGE/Input/InputCore.hpp"

#include "SOGE/System/Memory.hpp"


namespace soge
{


    class InputManager
    {
    private:
        SharedPtr<Keyboard> m_keyboard;
        SharedPtr<Mouse> m_mouse;

        eastl::list<SharedPtr<Gamepad>> m_gamepadList;
        eastl::list<SharedPtr<InputDevice>> m_deviceList;
        SharedPtr<InputCore> m_inputCore;

    public:
        InputManager();
        ~InputManager() = default;

        void Update();

        Keyboard* GetKeyboard() const;
        Mouse* GetMouse() const;

    };
}

#endif // SOGE_INPUT_INPUTMANAGER_HPP
