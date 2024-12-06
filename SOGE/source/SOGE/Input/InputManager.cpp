#include "sogepch.hpp"
#include "SOGE/Input/InputManager.hpp"
#include "SOGE/Utils/PreprocessorHelpers.hpp"

#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, Keyboard.hpp)
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, Gamepad.hpp)
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, Mouse.hpp)
#include SG_ABS_COMPILED_IMPL_HEADER(SOGE/Input, InputCore.hpp)


namespace soge
{
    InputManager::InputManager()
    {
        Keys::Initialize();

        m_inputCore = SharedPtr<ImplInputCore>(new ImplInputCore());
        //m_keyboard  = eastl::make_shared<ImplKeyboard>(m_inputCore->CreateKeyboard());
        //m_mouse     = eastl::make_shared<ImplMouse>(m_inputCore->CreateMouse());
        m_keyboard.reset(m_inputCore->CreateKeyboard());
        m_mouse.reset(m_inputCore->CreateMouse());
    }

    void InputManager::Update()
    {
        m_inputCore->BeginUpdateInput();
        m_keyboard->Update();
        m_mouse->Update();
    }

    Keyboard* InputManager::GetKeyboard() const
    {
        return m_keyboard.get();
    }

    Mouse* InputManager::GetMouse() const
    {
        return m_mouse.get();
    }
}
