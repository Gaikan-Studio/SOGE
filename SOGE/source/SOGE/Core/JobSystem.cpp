#include "sogepch.hpp"

#include "SOGE/Core/JobSystem.hpp"

#include <VGJS.h>
#include <VGJSCoro.h>


namespace
{
    constexpr std::size_t g_jobSystemMaxBlocksPerChunk = 1 << 10;
    constexpr std::size_t g_jobSystemLargestRequiredPoolBlock = 1 << 10;

    std::pmr::synchronized_pool_resource g_jobSystemMemoryResource{
        std::pmr::pool_options{
            .max_blocks_per_chunk = g_jobSystemMaxBlocksPerChunk,
            .largest_required_pool_block = g_jobSystemLargestRequiredPoolBlock,
        },
        std::pmr::new_delete_resource(),
    };
}

namespace soge
{
    class JobSystem::Impl
    {
    private:
        friend JobSystem;

        static std::uint64_t CreateInstanceId()
        {
            static std::uint64_t instanceId = 0;
            return instanceId++;
        }

        const std::uint64_t m_instanceId;
        std::atomic_uint64_t m_jobCount;
        std::atomic_bool m_allJobsCompleted;

    public:
        explicit Impl();

        explicit Impl(const Impl&) = delete;
        Impl& operator=(const Impl&) = delete;

        explicit Impl(Impl&&) = delete;
        Impl& operator=(Impl&&) = delete;

        ~Impl() = default;
    };

    JobSystem::Impl::Impl() : m_instanceId(CreateInstanceId())
    {
    }

    void JobSystem::Initialize(const std::uint16_t aThreadCount)
    {
        if (aThreadCount == 0)
        {
            SOGE_INFO_LOG("Initializing job system with default thread count...");
        }
        else
        {
            SOGE_INFO_LOG("Initializing job system with thread count of {}...", aThreadCount);
        }

        const vgjs::thread_count_t threadCount{aThreadCount};
        const vgjs::thread_index_t threadIndex{0};
        vgjs::JobSystem jobSystem{threadCount, threadIndex, &g_jobSystemMemoryResource};

        SOGE_INFO_LOG("Job system initialized successfully with thread count of {}!",
                      jobSystem.get_thread_count().value);
    }

    void JobSystem::Terminate()
    {
        SOGE_INFO_LOG("Cleaning up job system...");

        vgjs::JobSystem jobSystem;
        jobSystem.terminate();
        jobSystem.wait_for_termination();
    }

    JobSystem::JobSystem() : m_impl(CreateUnique<Impl>())
    {
    }

    JobSystem::~JobSystem()
    {
        Wait();
    }

    std::uint16_t JobSystem::GetThreadCount()
    {
        vgjs::JobSystem jobSystem;
        return static_cast<std::uint16_t>(jobSystem.get_thread_count().value);
    }

    void JobSystem::Schedule(std::function<void()> aJob)
    {
        // For better debugging experience
        Impl& impl = *m_impl;

        // Register job for this job system instance
        ++impl.m_jobCount;

        vgjs::Function job{
            [job = std::move(aJob), &impl] {
                job();

                // Unregister job from this job system instance
                --impl.m_jobCount;

                // Notify all waiting threads that all jobs were completed
                if (impl.m_jobCount == 0)
                {
                    impl.m_allJobsCompleted = true;
                    impl.m_allJobsCompleted.notify_all();
                }
            },
            vgjs::thread_index_t{}, // could be useful
        };

        vgjs::JobSystem jobSystem;
        jobSystem.schedule(std::move(job));
    }

    void JobSystem::Wait()
    {
        // For better debugging experience
        Impl& impl = *m_impl;

        // Fast path: nothing to wait
        if (impl.m_jobCount == 0)
        {
            return;
        }

        // Slow path: atomic wait
        impl.m_allJobsCompleted.wait(false);
        impl.m_allJobsCompleted = false;
    }
}
