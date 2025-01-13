#include "sogepch.hpp"

#include "SOGE/Core/Timestep.hpp"


namespace soge
{
    std::uint64_t Timestep::s_frameTime = 0;
    std::uint64_t Timestep::s_frameTimeForRealDelta = 0;
    float Timestep::s_deltaTime = 0.0f;
    float Timestep::s_realDeltaTime = 0.0f;
    float Timestep::s_globalTime = 0.0f;


    void Timestep::StartFrame()
    {
        if (s_frameTime == 0)
        {
            s_frameTime = GetMicroseconds();
            s_frameTimeForRealDelta = s_frameTime;
        }

        std::uint64_t timestamp = GetMicroseconds();
        float delta = static_cast<float>((timestamp - s_frameTime) / 1000000.0f);

        s_realDeltaTime = delta;
        s_deltaTime = eastl::min(0.1f, eastl::max(0.0004f, delta));

        s_frameTime = timestamp;
    }

    void Timestep::CalculateDelta()
    {
        s_realDeltaTime = static_cast<float>((s_frameTime - s_frameTimeForRealDelta) / 1000000.0f);
        s_frameTimeForRealDelta = s_frameTime;
    }

    void Timestep::SetDelta(float aDelta)
    {
        s_deltaTime = aDelta;
    }

    float Timestep::DeltaTime()
    {
        return s_deltaTime;
    }

    float Timestep::RealDeltaTime()
    {
        return s_realDeltaTime;
    }

    float Timestep::FrameTime()
    {
        return static_cast<float>(s_frameTime);
    }

    std::uint64_t Timestep::GetMilliseconds()
    {
        return chrono::duration_cast<chrono::milliseconds>(ClockEngine::now().time_since_epoch()).count();
    }

    std::uint64_t Timestep::GetMicroseconds()
    {
        return chrono::duration_cast<chrono::microseconds>(ClockEngine::now().time_since_epoch()).count();
    }

    std::uint64_t Timestep::GetNanoseconds()
    {
        return chrono::duration_cast<chrono::nanoseconds>(ClockEngine::now().time_since_epoch()).count();
    }

    std::uint64_t Timestep::GetSeconds()
    {
        return chrono::duration_cast<chrono::seconds>(ClockEngine::now().time_since_epoch()).count();
    }
}
