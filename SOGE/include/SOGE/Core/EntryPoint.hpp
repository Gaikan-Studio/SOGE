#ifndef SOGE_CORE_ENTRYPOINT_HPP
#define SOGE_CORE_ENTRYPOINT_HPP

#include "SOGE/Utils/Logger.hpp"


extern soge::Engine* soge::CreateApplication();

#ifdef SOGE_WINDOWS

#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nShowCmd)
{

#ifdef SOGE_DEBUG
    AllocConsole();
    freopen("CONOUT$", "w+", stdout);
#endif // SOGE_DEBUG

    soge::Logger::Init();

    const auto app = soge::CreateApplication();
    app->HandleCLIArgs(__argc, __argv);
    app->Run();
    delete app;

    return 0;
}

#else

int main(int argc, char** argv)
{
    soge::Logger::Init();

    auto app = soge::CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif // SOGE_WINDOWS
#endif // SOGE_CORE_ENTRYPOINT_HPP
