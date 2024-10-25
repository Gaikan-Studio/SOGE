#ifndef SOGE_CORE_ENTRYPOINT_HPP
#define SOGE_CORE_ENTRYPOINT_HPP

#include "SOGE/System/Memory.hpp"
#include "SOGE/Utils/Logger.hpp"


namespace soge
{
    extern Engine* CreateApplication();

    [[nodiscard]]
    bool ConsoleInit(Span<char*> args);

    [[nodiscard]]
    inline int Launch(const Span<char*> args)
    {
        if (!ConsoleInit(args))
        {
            return EXIT_FAILURE;
        }

        Logger::Init();

        const auto app = CreateApplication();
        app->Run();

        return EXIT_SUCCESS;
    }
}

#ifdef SOGE_WINDOWS

#include <Windows.h>

inline bool soge::ConsoleInit(Span<char*> args)
{
#ifdef SOGE_DEBUG
    // By using console ctrl handler, we can gracefully shut down the engine

    // Hack to define local handler function
    struct CtrlHandler
    {
        // Prevent opened console from closing before the engine has finished shutting down
        // https://stackoverflow.com/questions/3640633/setconsolectrlhandler-routine-issue
        static BOOL WINAPI Handle(DWORD aCtrlType)
        {
            // Wait for the engine to gracefully shutdown
            if (const auto engine = Engine::GetInstance())
            {
                engine->RequestShutdown();
                while (engine->IsRunning())
                {
                }
            }

            // Maximum amount of time to wait until main thread exits
            std::this_thread::sleep_for(std::chrono::seconds(10));
            return TRUE;
        }
    };

    if (!AllocConsole() || !SetConsoleCtrlHandler(CtrlHandler::Handle, TRUE))
    {
        SOGE_ERROR_LOG("Failed to initialize console");
        return false;
    }

    FILE* unusedStream;
    if (freopen_s(&unusedStream, "CONOUT$", "w+", stdout) != 0)
    {
        SOGE_ERROR_LOG("Failed to reopen stdout");
        return false;
    }

#endif // SOGE_DEBUG

    return true;
}

inline int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nShowCmd)
{
    using namespace soge;

    const Span args{__argv, static_cast<USize>(__argc)};
    return Launch(args);
}

#else

inline bool soge::ConsoleInit(Span<char*> args)
{
    return true;
}

inline int main(int argc, char** argv)
{
    using namespace soge;

    const Span args{argv, static_cast<USize>(argc)};
    return Launch(args);
}

#endif // SOGE_WINDOWS

#endif // SOGE_CORE_ENTRYPOINT_HPP
