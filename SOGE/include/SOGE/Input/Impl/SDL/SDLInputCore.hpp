#ifndef SOGE_INPUT_IMPL_SDL_SDLINPUTCORE_HPP
#define SOGE_INPUT_IMPL_SDL_SDLINPUTCORE_HPP

#include "SOGE/Input/InputCore.hpp"
#include "SOGE/System/Impl/SDL/SDLContext.hpp"

#include <SDL3/SDL_events.h>


namespace soge
{
    class SDLInputCore final : public InputCore
    {
    private:
        friend class SDLKeyboard;
        friend class SDLGamepad;
        friend class SDLMouse;
        friend class SDLKeyMapManager;

        eastl::list<SDL_Event> m_sdlEventList;

        bool m_isPauseUpdateRequested;
        bool m_isEndUpdateRequested;
        bool m_isAnyButtonPressed;

    public:
        explicit SDLInputCore(EventModule& aEventModule, SDLContext&);

        void LockInput(bool aLockInput) override;
        void UseRelativeMouseMode(bool aRelMouse) override;
        bool IsAnyButtonPressed() override;

        void BeginUpdateInput() override;
        void EndUpdateInput() override;
        void SetPauseUpdate(bool aIsPauseNeeded) override;

        [[nodiscard]]
        bool IsKeyPressed(const Key& aKey) const override;
        [[nodiscard]]
        bool IsKeyReleased(const Key& aKey) const override;

        [[nodiscard]]
        Keyboard* GetKeyboard() const override;
        [[nodiscard]]
        Gamepad* GetGamepad() const override;
        [[nodiscard]]
        Mouse* GetMouse() const override;
    };

    using ImplInputCore = SDLInputCore;
}

SOGE_DI_REGISTER_NS(soge, SDLInputCore, df::Single<SDLInputCore, EventModule, SDLContext>,
                    tag::Overrides<SDLInputCore, InputCore>)

#endif // SOGE_INPUT_IMPL_SDL_SDLINPUTCORE_HPP
