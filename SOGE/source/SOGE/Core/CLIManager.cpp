#include "sogepch.hpp"
#include "SOGE/Core/CLIManager.hpp"
#include "SOGE/System/Memory.hpp"

#include <CLI/CLI.hpp>


namespace soge
{
    CLIManager::CLIManager()
    {
        m_cliApp = CreateUnique<CLI::App>("General purpose game engine", "SOGE");
    }

    CLIManager::~CLIManager()
    {
        m_cliApp.release();
    }

    void CLIManager::InitCLI()
    {
        auto hello = m_cliApp->add_subcommand("hello", "just say hello");
        bool verbose = false;
        hello->add_flag("-n,--name", verbose, "Enable verbose");
    }

    void CLIManager::ParseArgs(int argc, char** argv)
    {
        argv = m_cliApp->ensure_utf8(argv);

        try
        {
            m_cliApp->parse(argc, argv);
        }
        catch (const CLI::ParseError& e)
        {
            if (!m_cliApp->parsed())
            {
                SOGE_ERROR_LOG("Failed to parse command line arguments");
            }

            m_cliApp->exit(e);
        }
    }
}
