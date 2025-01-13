#include "sogepch.hpp"

#include "SOGE/Event/InputEvents.hpp"


namespace soge
{
    KeyEventBase::KeyEventBase(const Key& aKeyObj) noexcept : m_keyObj(aKeyObj)
    {
    }

    Key KeyEventBase::GetKey() const
    {
        return m_keyObj;
    }

    //////////////////////
    // Keyboard Event
    /////////////////////

    KeyPressedEvent::KeyPressedEvent(const Key& aPressedKey, const std::uint32_t aRepeatCount) noexcept
        : KeyEventBase(aPressedKey), m_repeatCount(aRepeatCount)
    {
    }

    std::uint32_t KeyPressedEvent::GetRepeatCount() const
    {
        return m_repeatCount;
    }

    KeyReleasedEvent::KeyReleasedEvent(const Key& aReleasedKey) noexcept : KeyEventBase(aReleasedKey)
    {
    }

    //////////////////////
    // Mouse Event
    /////////////////////

    MouseButtonPressedEvent::MouseButtonPressedEvent(const Key& aPressedButton,
                                                     const std::uint32_t aRepeatCount) noexcept
        : KeyEventBase(aPressedButton), m_repeatCount(aRepeatCount)
    {
    }

    std::uint32_t MouseButtonPressedEvent::GetRepeatCount() const
    {
        return m_repeatCount;
    }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(const Key& aReleasedButton) noexcept
        : KeyEventBase(aReleasedButton)
    {
    }

    MouseMovedEvent::MouseMovedEvent(const float aXOffset, const float aYOffset) noexcept : m_xOffset(aXOffset), m_yOffset(aYOffset)
    {
    }

    float MouseMovedEvent::GetXOffset() const
    {
        return m_xOffset;
    }

    float MouseMovedEvent::GetYOffset() const
    {
        return m_yOffset;
    }

    MouseWheelEvent::MouseWheelEvent(const float aXOffset, const float aYOffset) noexcept
        : m_xOffset(aXOffset), m_yOffset(aYOffset)
    {
    }

    float MouseWheelEvent::GetXOffset() const
    {
        return m_xOffset;
    }

    float MouseWheelEvent::GetYOffset() const
    {
        return m_yOffset;
    }
}
