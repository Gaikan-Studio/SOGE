#ifndef SOGE_INPUT_IMPL_SDL_SDLKEYBOARD_HPP
#define SOGE_INPUT_IMPL_SDL_SDLKEYBOARD_HPP

#include "SOGE/Input/Impl/SDL/SDLInputCore.hpp"
#include "SOGE/Input/Device/Keyboard.hpp"


namespace soge
{
    class SDLKeyboard final : public Keyboard
    {
    private:
        SharedPtr<SDLInputCore> m_inputCoreSDL;
        int m_repeatCounter;

    public:
        SDLKeyboard(SharedPtr<SDLInputCore> aInputCore);
        ~SDLKeyboard() = default;

        void Update() override;
        bool IsKeyPressed(const Key aKeyName) override;
        bool IsKeyReleased(const Key aKeyName) override;

    };

    typedef SDLKeyboard ImplKeyboard;

}

#endif // SOGE_INPUT_IMPL_SDL_SDLKEYBOARD_HPP
