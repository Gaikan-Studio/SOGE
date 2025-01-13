#ifndef SOGE_WINDOW_WINDOWMODULE_HPP
#define SOGE_WINDOW_WINDOWMODULE_HPP

#include "SOGE/Core/Module.hpp"
#include "SOGE/System/Memory.hpp"
#include "SOGE/Utils/UUID.hpp"
#include "SOGE/Window/WindowCore.hpp"

#pragma push_macro("CreateWindow")
#undef CreateWindow


namespace soge
{
    class WindowModule : public Module
    {
    private:
        using Key = UUIDv4::UUID;
        using UniqueWindow = UniquePtr<Window>;

        using Windows = eastl::hash_map<Key, UniqueWindow>;
        Windows m_windows;

        WindowCore* m_core;

    public:
        explicit WindowModule();

        [[nodiscard]]
        eastl::pair<Window&, Key> CreateWindow(const WindowDesc& aDesc = WindowDesc());

        [[nodiscard]]
        Window* GetWindow(const Key& aKey) const;

        UniqueWindow DestroyWindow(const Key& aKey);

        void Load(di::Container& aContainer, ModuleManager& aModuleManager) override;
        void Unload(di::Container& aContainer, ModuleManager& aModuleManager) override;
    };
}

SOGE_DI_REGISTER_MODULE_NS(soge, WindowModule)


#pragma pop_macro("CreateWindow")

#endif // SOGE_WINDOW_WINDOWMODULE_HPP
