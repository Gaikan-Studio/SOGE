#ifndef SOGE_CORE_JOBSYSTEM_HPP
#define SOGE_CORE_JOBSYSTEM_HPP

#include "SOGE/DI/Dependency.hpp"


namespace soge
{
    class JobSystem final
    {
    public:
        explicit JobSystem(std::uint16_t aThreadCount = 0);

        explicit JobSystem(const JobSystem&) = delete;
        JobSystem& operator=(const JobSystem&) = delete;

        explicit JobSystem(JobSystem&&) = default;
        JobSystem& operator=(JobSystem&&) = default;

        ~JobSystem();
    };
}

SOGE_DI_REGISTER_NS(soge, JobSystem, df::Single<JobSystem>, tag::Final<JobSystem>)

#endif // SOGE_CORE_JOBSYSTEM_HPP
