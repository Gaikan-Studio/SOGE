#include "sogepch.hpp"
#include "SOGE/Sound/Impl/FMOD/FMODException.hpp"


namespace soge
{
    FMODException::FMODException(FMOD_RESULT aFmodResult) : m_fmodResult(aFmodResult)
    {
    }

    const char* FMODException::what() const noexcept
    {
        static char s_str[64] = {};
        sprintf_s(s_str, FMOD_ErrorString(m_fmodResult));
        return s_str;
    }
}
