#ifndef SOGE_SYSTEM_IMPL_SDL_SDLCONTEXT_HPP
#define SOGE_SYSTEM_IMPL_SDL_SDLCONTEXT_HPP

#include "SOGE/DI/Dependency.hpp"


namespace soge
{
    // This class exists just to share one-time SDL initialization and cleanup
    // with multiple modules which depend on SDL (such as input and window).
    //
    // Feel free to initialize your libraries in the module constructor,
    // but for C libraries without explicit context object this way is preferred.
    class SDLContext final
    {
    public:
        explicit SDLContext();

        explicit SDLContext(const SDLContext&) = delete;
        SDLContext& operator=(const SDLContext&) = delete;

        explicit SDLContext(SDLContext&&) noexcept = default;
        SDLContext& operator=(SDLContext&&) noexcept = default;

        ~SDLContext();
    };
}

SOGE_DI_REGISTER_NS(soge, SDLContext, df::Single<SDLContext>, tag::Final<SDLContext>)

#endif // SOGE_SYSTEM_IMPL_SDL_SDLCONTEXT_HPP
