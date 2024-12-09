#include "sogepch.hpp"

#include "SOGE/Window/Impl/SDL/SDLWindow.hpp"

#include "SOGE/Utils/StringHelpers.hpp"


namespace soge
{
    SDLWindow::SDLWindow(const WindowDesc& aWindowDesc) : Window(aWindowDesc), m_windowDesc(aWindowDesc)
    {
        const auto title = EAToNarrow(m_windowDesc.m_title);
        const auto width = static_cast<int>(m_windowDesc.m_width);
        const auto height = static_cast<int>(m_windowDesc.m_height);
        m_sdlWindow = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_INPUT_FOCUS);
        // SDL_SetWindowRelativeMouseMode(m_sdlWindow, true);
    }

    SDLWindow::~SDLWindow()
    {
        SDL_DestroyWindow(m_sdlWindow);
    }

    void* SDLWindow::GetNativeHandler() const
    {
        // https://github.com/libsdl-org/SDL/blob/main/docs/README-migration.md#sdl_syswmh
        const SDL_PropertiesID properties = SDL_GetWindowProperties(m_sdlWindow);
        return SDL_GetPointerProperty(properties, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
    }

    eastl::wstring SDLWindow::GetTitle() const
    {
        const auto title = SDL_GetWindowTitle(m_sdlWindow);
        if (!title)
        {
            SOGE_ERROR_LOG("Cannot get window title: {}", SDL_GetError());
            return {};
        }

        return EAToWide(title);
    }

    std::uint32_t SDLWindow::GetWidth() const
    {
        int width;
        if (!SDL_GetWindowSize(m_sdlWindow, &width, nullptr))
        {
            SOGE_ERROR_LOG("Cannot get window width: {}", SDL_GetError());
            return 0;
        }

        return static_cast<std::uint32_t>(width);
    }

    std::uint32_t SDLWindow::GetHeight() const
    {
        int height;
        if (!SDL_GetWindowSize(m_sdlWindow, nullptr, &height))
        {
            SOGE_ERROR_LOG("Cannot get window height: {}", SDL_GetError());
            return 0;
        }

        return static_cast<std::uint32_t>(height);
    }
}
