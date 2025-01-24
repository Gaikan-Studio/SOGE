#ifndef SOGE_CORE_ENTRYPOINT_HPP
#define SOGE_CORE_ENTRYPOINT_HPP

#include "SOGE/Utils/Logger.hpp"

#include <span>


namespace soge
{
    extern Engine* CreateApplication();

    [[nodiscard]]
    bool ConsoleInit(std::span<char*> args);

    [[nodiscard]]
    inline int Launch(const std::span<char*> args)
    {
        if (!ConsoleInit(args))
        {
            return EXIT_FAILURE;
        }

        Logger::Init();

        const auto app = CreateApplication();
        app->HandleCLIArgs(args.size(), args.data());
        app->Run();

        return EXIT_SUCCESS;
    }
}

#ifdef SOGE_WINDOWS

#include <Windows.h>

inline bool soge::ConsoleInit(std::span<char*> args)
{
#ifdef SOGE_DEBUG
    // By using console ctrl handler, we can gracefully shut down the engine

    // Hack to define local handler function
    struct CtrlHandler
    {
        // Prevent opened console from closing before the engine has finished shutting down
        // https://stackoverflow.com/questions/3640633/setconsolectrlhandler-routine-issue
        static BOOL WINAPI Handle(const DWORD aCtrlType)
        {
            // Wait for the engine to gracefully shutdown
            if (const auto engine = Engine::GetInstance())
            {
                engine->RequestShutdown();
                while (engine->IsRunning())
                {
                }
            }

            // Ctrl+C or Ctrl+Break events are not terminating the program, but close event does
            // https://learn.microsoft.com/en-us/windows/console/handlerroutine
            const bool shouldYield = aCtrlType == CTRL_CLOSE_EVENT;
            // NOLINTNEXTLINE(bugprone-infinite-loop) reason: intended behaviour
            while (shouldYield)
            {
                // Allow for other threads (such as the main thread) to finish their work
                // This should work because Windows calls this handler in a separate thread
                std::this_thread::yield();
            }
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
    const std::span args{__argv, static_cast<std::size_t>(__argc)};
    return Launch(args);
}

#else

inline bool soge::ConsoleInit(std::span<char*> args)
{
    return true;
}

inline int main(int argc, char** argv)
{
    using namespace soge;

    const std::span args{argv, static_cast<std::size_t>(argc)};
    return Launch(args);
}

#endif // SOGE_WINDOWS

#endif // SOGE_CORE_ENTRYPOINT_HPP
