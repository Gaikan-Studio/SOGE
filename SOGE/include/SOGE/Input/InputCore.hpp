#ifndef SOGE_INPUT_INPUTCORE_HPP
#define SOGE_INPUT_INPUTCORE_HPP

#include "SOGE/System/Memory.hpp"


namespace soge
{
    class Keyboard;
    class Gamepad;
    class Mouse;

    class InputCore
    {
    public:
        InputCore()             = default;
        virtual ~InputCore()    = default;

        virtual void LockInput(bool aLockInput)             = 0;
        virtual void UseRelativeMouseMode(bool aLockMouse)  = 0;
        virtual bool IsAnyButtonPressed()                   = 0;

        virtual void BeginUpdateInput() = 0;
        virtual void EndUpdateInput()   = 0;
        virtual void SetPauseUpdate(bool aIsPauseNeeded) = 0;

        virtual Mouse* CreateMouse()        = 0;
        virtual Gamepad* CreateGamepad()    = 0;
        virtual Keyboard* CreateKeyboard()  = 0;

    };
}

#endif // SOGE_INPUT_INPUTCORE_HPP
