#include "sogepch.hpp"

#include "SOGE/Core/JobSystem.hpp"

#include <VGJS.h>


namespace soge
{
    JobSystem::JobSystem(const std::uint16_t aThreadCount)
    {
        if (aThreadCount == 0)
        {
            SOGE_INFO_LOG("Initializing job system with default thread count...");
        }
        else
        {
            SOGE_INFO_LOG("Initializing job system with thread count of {}...", aThreadCount);
        }

        const vgjs::thread_count_t threadCount(aThreadCount);
        vgjs::JobSystem jobSystem{threadCount};

        SOGE_INFO_LOG("Job system initialized successfully with thread count of {}!",
                      jobSystem.get_thread_count().value);
    }

    JobSystem::~JobSystem()
    {
        SOGE_INFO_LOG("Cleaning up job system...");

        vgjs::terminate();
        vgjs::wait_for_termination();
    }
}
