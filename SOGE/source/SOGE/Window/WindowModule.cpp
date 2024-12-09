#include "sogepch.hpp"

#include "SOGE/Window/WindowModule.hpp"

#include "SOGE/DI/Container.hpp"
#include "SOGE/Window/WindowCompilePreproc.hpp"

#include SOGE_ABS_COMPILED_WINDOW_IMPL_HEADER(SOGE/Window, WindowCore.hpp)

#undef CreateWindow


namespace soge
{
    WindowModule::WindowModule() : m_core(nullptr)
    {
    }

    void WindowModule::Load(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_core = &aContainer.Provide<ImplWindowCore>();

        SOGE_INFO_LOG("Window module loaded...");
    }

    void WindowModule::Unload(di::Container& aContainer, ModuleManager& aModuleManager)
    {
        m_windows.clear();
        m_core = nullptr;

        SOGE_INFO_LOG("Window module unloaded...");
    }

    auto WindowModule::CreateWindow(const WindowDesc& aDesc) -> eastl::pair<Window&, Key>
    {
        const auto key = UUID::Generate();

        auto window = m_core->CreateWindow(aDesc);
        const auto [iter, _] = m_windows.try_emplace(key, std::move(window));

        return {*iter->second, key};
    }

    Window* WindowModule::GetWindow(const Key& aKey) const
    {
        const auto iter = m_windows.find(aKey);
        if (iter == m_windows.end())
        {
            return nullptr;
        }

        return iter->second.get();
    }

    auto WindowModule::DestroyWindow(const Key& aKey) -> UniqueWindow
    {
        const auto iter = m_windows.find(aKey);
        if (iter == m_windows.end())
        {
            return UniqueWindow{};
        }

        UniqueWindow window = std::move(iter->second);
        m_windows.erase(iter);

        return window;
    }
}
