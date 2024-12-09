#ifndef SOGE_WINDOW_IMPL_SDL_SDLWINDOW_HPP
#define SOGE_WINDOW_IMPL_SDL_SDLWINDOW_HPP

#include "SOGE/Window/Window.hpp"

#include <SDL3/SDL_video.h>


namespace soge
{
    class SDLWindow final : public Window
    {
    private:
        SDL_Window* m_sdlWindow;
        WindowDesc m_windowDesc;

    public:
        explicit SDLWindow(const WindowDesc& aWindowDesc);

        explicit SDLWindow(const SDLWindow&) = delete;
        SDLWindow& operator=(const SDLWindow&) = delete;

        explicit SDLWindow(SDLWindow&&) noexcept = default;
        SDLWindow& operator=(SDLWindow&&) noexcept = default;

        ~SDLWindow() override;

        [[nodiscard]]
        void* GetNativeHandler() const override;

        [[nodiscard]]
        eastl::wstring GetTitle() const override;
        [[nodiscard]]
        std::uint32_t GetWidth() const override;
        [[nodiscard]]
        std::uint32_t GetHeight() const override;
    };

    using ImplWindow = SDLWindow;
}

#endif // SOGE_WINDOW_IMPL_SDL_SDLWINDOW_HPP
