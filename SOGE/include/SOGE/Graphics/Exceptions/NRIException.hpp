#ifndef SOGE_GRAPHICS_EXCEPTIONS_NRIEXCEPTION_HPP
#define SOGE_GRAPHICS_EXCEPTIONS_NRIEXCEPTION_HPP

#include <exception>

#include <NRIDescs.h>


namespace soge
{
    class NRIException : public std::exception
    {
    private:
        nri::Result m_nriResult;

    public:
        explicit NRIException(nri::Result aNriResult);

        [[nodiscard]]
        const char* what() const noexcept override;
    };

    void NRIThrowIfFailed(nri::Result aNriResult);
    void NRIThrowIfFailed(nri::Result aNriResult, eastl::string_view aContext);
}

#endif // SOGE_GRAPHICS_EXCEPTIONS_NRIEXCEPTION_HPP
