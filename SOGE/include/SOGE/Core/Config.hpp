#ifndef SOGE_CORE_CONFIG_HPP
#define SOGE_CORE_CONFIG_HPP


namespace soge
{
    class Config final
    {
    private:
        bool m_graphicsModuleRequired;
        bool m_windowModuleRequired;
        bool m_soundModuleRequired;
        bool m_eventModuleRequired;
        bool m_inputModuleRequired;

    public:
        Config();
        ~Config();

    };
}

#endif // !SOGE_CORE_CONFIG_HPP
