#include "sogepch.hpp"

#include "SOGE/Input/InputModule.hpp"

#include "SOGE/Core/ModuleManager.hpp"
#include "SOGE/DI/Container.hpp"
#include "SOGE/Input/InputCompilePreproc.hpp"

#include SOGE_ABS_COMPILED_INPUT_IMPL_HEADER(SOGE/Input, InputCore.hpp)


namespace soge
{
    InputModule::InputModule() : m_inputCore(nullptr)
    {
        Keys::Initialize();
    }

    void InputModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        aModuleManager.CreateModule<EventModule>();
        m_inputCore = &aContainer.Provide<ImplInputCore>();

        SOGE_INFO_LOG("Input module loaded...");
    }

    void InputModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_inputCore = nullptr;

        SOGE_INFO_LOG("Input module unloaded...");
    }

    bool InputModule::IsKeyPressed(const Key& aKey) const
    {
        return m_inputCore->IsKeyPressed(aKey);
    }

    bool InputModule::IsKeyReleased(const Key& aKey) const
    {
        return m_inputCore->IsKeyReleased(aKey);
    }

    void InputModule::Update() const
    {
        m_inputCore->BeginUpdateInput();
    }

    Keyboard* InputModule::GetKeyboard() const
    {
        return m_inputCore->GetKeyboard();
    }

    Gamepad* InputModule::GetGamepad() const
    {
        return m_inputCore->GetGamepad();
    }

    Mouse* InputModule::GetMouse() const
    {
        return m_inputCore->GetMouse();
    }
}
