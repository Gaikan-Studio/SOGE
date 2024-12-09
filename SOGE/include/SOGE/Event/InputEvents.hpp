#ifndef SOGE_EVENT_INPUTEVENTS_HPP
#define SOGE_EVENT_INPUTEVENTS_HPP

#include "SOGE/Event/Event.hpp"
#include "SOGE/Event/EventHelpers.hpp"
#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    //////////////////////
    // Input Event types
    /////////////////////

    // NOLINTNEXTLINE(readability-identifier-naming)
    namespace EventTypes
    {
        // NOLINTNEXTLINE(readability-identifier-naming)
        namespace InputEvents
        {
            constexpr EventType g_keyPressedEvent{StringId("KeyPressedEvent"), EventCategory::Input};
            constexpr EventType g_keyReleasedEvent{StringId("KeyReleasedEvent"), EventCategory::Input};

            constexpr EventType g_mouseButtonPressedEvent{StringId("MouseButtonPressedEvent"), EventCategory::Input};
            constexpr EventType g_mouseButtonReleasedEvent{StringId("MouseButtonReleasedEvent"), EventCategory::Input};
            constexpr EventType g_mouseMovedEvent{StringId("MouseMovedEvent"), EventCategory::Input};
            constexpr EventType g_mouseWheelEvent{StringId("MouseWheelEvent"), EventCategory::Input};
        }
    }

    class KeyEventBase : public StaticEvent<KeyEventBase>
    {
    private:
        Key m_keyObj;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        explicit KeyEventBase(const Key& aKeyObj) noexcept;

        [[nodiscard]]
        virtual Key GetKey() const;
    };

    //////////////////////
    // Keyboard Event
    /////////////////////

    class KeyPressedEvent : public KeyEventBase
    {
    private:
        std::uint32_t m_repeatCount;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        explicit KeyPressedEvent(const Key& aPressedKey, std::uint32_t aRepeatCount) noexcept;

        [[nodiscard]]
        std::uint32_t GetRepeatCount() const;

        // FIXME: this is needed because static event class overrides this method
        //  only from the first derived class, not the last one
        constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }
    };

    class KeyReleasedEvent : public KeyEventBase
    {
    public:
        static constexpr EventType GetStaticEventType() noexcept;

        explicit KeyReleasedEvent(const Key& aReleasedKey) noexcept;

        // FIXME: this is needed because static event class overrides this method
        //  only from the first derived class, not the last one
        constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }
    };

    //////////////////////
    // Mouse Event
    /////////////////////

    // Pretty much similar to keyboard key pressed event.
    // But we need set exactly g_mouseButtonKeyPressedEvent.
    //
    // So here is some code duplication.
    // Probably this can be moved to some kind of multifunction for all button events.
    //
    // TODO: Figure out this garbage in the future

    class MouseButtonPressedEvent : public KeyEventBase
    {
    private:
        std::uint32_t m_repeatCount;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        explicit MouseButtonPressedEvent(const Key& aPressedButton, std::uint32_t aRepeatCount) noexcept;

        [[nodiscard]]
        std::uint32_t GetRepeatCount() const;

        // FIXME: this is needed because static event class overrides this method
        //  only from the first derived class, not the last one
        constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }
    };

    class MouseButtonReleasedEvent : public KeyEventBase
    {
    public:
        static constexpr EventType GetStaticEventType() noexcept;

        explicit MouseButtonReleasedEvent(const Key& aReleasedButton) noexcept;

        // FIXME: this is needed because static event class overrides this method
        //  only from the first derived class, not the last one
        constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }
    };

    class MouseMovedEvent : public StaticEvent<MouseMovedEvent>
    {
    private:
        float m_relX;
        float m_relY;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        explicit MouseMovedEvent(float aRelX, float aRelY) noexcept;

        [[nodiscard]]
        float GetRelativeX() const;
        [[nodiscard]]
        float GetRelativeY() const;

        [[nodiscard]]
        constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }
    };

    class MouseWheelEvent : public StaticEvent<MouseWheelEvent>
    {
    private:
        float m_xOffset;
        float m_yOffset;

    public:
        static constexpr EventType GetStaticEventType() noexcept;

        explicit MouseWheelEvent(float aXOffset, float aYOffset) noexcept;

        [[nodiscard]]
        float GetXOffset() const;
        [[nodiscard]]
        float GetYOffset() const;

        [[nodiscard]]
        constexpr EventType GetEventType() const override
        {
            return GetStaticEventType();
        }
    };

    //////////////////////
    // Assertions
    /////////////////////

    SOGE_DERIVED_FROM_STATIC_EVENT_ASSERT(KeyEventBase);
    SOGE_DERIVED_FROM_STATIC_EVENT_ASSERT(KeyPressedEvent);
    SOGE_DERIVED_FROM_STATIC_EVENT_ASSERT(KeyReleasedEvent);
    SOGE_DERIVED_FROM_STATIC_EVENT_ASSERT(MouseButtonPressedEvent);
    SOGE_DERIVED_FROM_STATIC_EVENT_ASSERT(MouseButtonReleasedEvent);
    SOGE_DERIVED_FROM_STATIC_EVENT_ASSERT(MouseMovedEvent);
    SOGE_DERIVED_FROM_STATIC_EVENT_ASSERT(MouseWheelEvent);

    ////////////////////////////
    // Event declarations
    ///////////////////////////

    constexpr EventType KeyEventBase::GetStaticEventType() noexcept
    {
        return EventTypes::Dummy::g_dummyEvent;
    }

    constexpr EventType KeyPressedEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_keyPressedEvent;
    }

    constexpr EventType KeyReleasedEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_keyReleasedEvent;
    }

    constexpr EventType MouseButtonPressedEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_mouseButtonPressedEvent;
    }

    constexpr EventType MouseButtonReleasedEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_mouseButtonReleasedEvent;
    }

    constexpr EventType MouseMovedEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_mouseMovedEvent;
    }

    constexpr EventType MouseWheelEvent::GetStaticEventType() noexcept
    {
        return EventTypes::InputEvents::g_mouseWheelEvent;
    }
}

#endif // SOGE_EVENT_INPUTEVENTS_HPP
