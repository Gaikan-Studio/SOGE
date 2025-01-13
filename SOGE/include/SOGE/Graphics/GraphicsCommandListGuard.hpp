#ifndef SOGE_GRAPHICS_GRAPHICSCOMMANDLISTGUARD_HPP
#define SOGE_GRAPHICS_GRAPHICSCOMMANDLISTGUARD_HPP

#include <nvrhi/nvrhi.h>

#include <functional>


namespace soge
{
    class GraphicsCommandListGuard
    {
    private:
        std::reference_wrapper<nvrhi::ICommandList> m_commandList;

    public:
        constexpr explicit GraphicsCommandListGuard(nvrhi::ICommandList& aCommandList) noexcept;

        constexpr GraphicsCommandListGuard(const GraphicsCommandListGuard&) noexcept = default;
        constexpr GraphicsCommandListGuard& operator=(const GraphicsCommandListGuard&) noexcept = default;

        constexpr GraphicsCommandListGuard(GraphicsCommandListGuard&&) = default;
        constexpr GraphicsCommandListGuard& operator=(GraphicsCommandListGuard&&) noexcept = default;

        constexpr ~GraphicsCommandListGuard() noexcept;

        constexpr nvrhi::ICommandList* operator->() noexcept;
        constexpr const nvrhi::ICommandList* operator->() const noexcept;

        constexpr operator nvrhi::ICommandList&() noexcept;
        constexpr operator const nvrhi::ICommandList&() const noexcept;

        constexpr operator nvrhi::ICommandList*() noexcept;
        constexpr operator const nvrhi::ICommandList*() const noexcept;
    };

    constexpr GraphicsCommandListGuard::GraphicsCommandListGuard(nvrhi::ICommandList& aCommandList) noexcept
        : m_commandList(aCommandList)
    {
        m_commandList.get().open();
    }

    constexpr GraphicsCommandListGuard::~GraphicsCommandListGuard() noexcept
    {
        m_commandList.get().close();
    }

    constexpr nvrhi::ICommandList* GraphicsCommandListGuard::operator->() noexcept
    {
        return &m_commandList.get();
    }

    constexpr const nvrhi::ICommandList* GraphicsCommandListGuard::operator->() const noexcept
    {
        return &m_commandList.get();
    }

    constexpr GraphicsCommandListGuard::operator nvrhi::ICommandList&() noexcept
    {
        return m_commandList.get();
    }

    constexpr GraphicsCommandListGuard::operator const nvrhi::ICommandList&() const noexcept
    {
        return m_commandList.get();
    }

    constexpr GraphicsCommandListGuard::operator nvrhi::ICommandList*() noexcept
    {
        return &m_commandList.get();
    }

    constexpr GraphicsCommandListGuard::operator const nvrhi::ICommandList*() const noexcept
    {
        return &m_commandList.get();
    }
}

#endif // SOGE_GRAPHICS_GRAPHICSCOMMANDLISTGUARD_HPP
