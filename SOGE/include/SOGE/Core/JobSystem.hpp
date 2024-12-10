#ifndef SOGE_CORE_JOBSYSTEM_HPP
#define SOGE_CORE_JOBSYSTEM_HPP

#include "SOGE/DI/Dependency.hpp"
#include "SOGE/System/Memory.hpp"


namespace soge
{
    class JobSystem final
    {
    private:
        class Impl;

        UniquePtr<Impl> m_impl;

    public:
        explicit JobSystem(std::uint16_t aThreadCount = 0);

        explicit JobSystem(const JobSystem&) = delete;
        JobSystem& operator=(const JobSystem&) = delete;

        explicit JobSystem(JobSystem&&) noexcept = default;
        JobSystem& operator=(JobSystem&&) noexcept = default;

        ~JobSystem();

        [[nodiscard]]
        std::uint16_t GetThreadCount();

        void Schedule(std::function<void()> aJob);
        void Wait();
    };
}

SOGE_DI_REGISTER_NS(soge, JobSystem, df::Single<JobSystem>, tag::Final<JobSystem>)

#endif // SOGE_CORE_JOBSYSTEM_HPP
