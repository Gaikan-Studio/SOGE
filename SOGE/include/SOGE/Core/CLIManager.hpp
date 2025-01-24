#ifndef SOGE_CORE_CLIMANAGER_HPP
#define SOGE_CORE_CLIMANAGER_HPP


namespace CLI
{
    class App;
}

namespace soge
{
    class CLIManager
    {
    private:
        eastl::unique_ptr<CLI::App> m_cliApp;


    public:
        CLIManager();
        ~CLIManager();

        void InitCLI();
        void ParseArgs(int argc, char** argv);

    };
}

#endif // !SOGE_CORE_CLIMANAGER_HPP
