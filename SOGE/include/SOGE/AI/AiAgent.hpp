#ifndef SOGE_AI_AIAGENT_HPP
#define SOGE_AI_AIAGENT_HPP


namespace soge
{
    class AiAgent
    {
    public:
        constexpr explicit AiAgent() noexcept = default;

        constexpr AiAgent(const AiAgent&) noexcept = delete;
        constexpr AiAgent& operator=(const AiAgent&) noexcept = delete;

        constexpr AiAgent(AiAgent&&) noexcept = default;
        constexpr AiAgent& operator=(AiAgent&&) noexcept = default;

        constexpr virtual ~AiAgent() = default;
    };
}

#endif // SOGE_AI_AIAGENT_HPP
