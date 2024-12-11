#ifndef SOGE_INPUT_DEVICE_KEYBOARD_HPP
#define SOGE_INPUT_DEVICE_KEYBOARD_HPP

#include "SOGE/Input/Device/InputDevice.hpp"


namespace soge
{
    class Keyboard : public InputDevice
    {
    public:
        explicit Keyboard(eastl::string_view aKeyboardName);

        virtual void Update() = 0;
    };
}

#endif // SOGE_INPUT_DEVICE_KEYBOARD_HPP
