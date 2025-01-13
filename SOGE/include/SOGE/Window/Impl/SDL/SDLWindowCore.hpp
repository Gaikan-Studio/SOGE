#ifndef SOGE_WINDOW_IMPL_SDL_SDLWINDOWCORE_HPP
#define SOGE_WINDOW_IMPL_SDL_SDLWINDOWCORE_HPP

#include "SOGE/System/Impl/SDL/SDLContext.hpp"
#include "SOGE/Window/Impl/SDL/SDLWindow.hpp"
#include "SOGE/Window/WindowCore.hpp"

#pragma push_macro("CreateWindow")
#undef CreateWindow


namespace soge
{
    class SDLWindowCore : public WindowCore
    {
    public:
        explicit SDLWindowCore(SDLContext&);

        UniquePtr<Window> CreateWindow(const WindowDesc& aWindowDesc) override;
    };

    using ImplWindowCore = SDLWindowCore;
}

SOGE_DI_REGISTER_NS(soge, SDLWindowCore, df::Single<SDLWindowCore, SDLContext>,
                    tag::Overrides<SDLWindowCore, WindowCore>)


#pragma pop_macro("CreateWindow")

#endif // SOGE_WINDOW_IMPL_SDL_SDLWINDOWCORE_HPP
