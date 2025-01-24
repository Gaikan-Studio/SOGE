#ifndef SOGE_CORE_CLI_CLIARGUMENT_HPP
#define SOGE_CORE_CLI_CLIARGUMENT_HPP

#include <string>


namespace soge
{
    class CLIArgument
    {
    private:
        std::string m_argName;
        std::string m_optNames;

    public:
        virtual ~CLIArgument() = default;

    };
}

#endif // !SOGE_CORE_CLI_CLIARGUMENT_HPP
