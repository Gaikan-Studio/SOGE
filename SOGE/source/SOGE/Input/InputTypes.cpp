#include "sogepch.hpp"

#include "SOGE/Input/InputTypes.hpp"


namespace soge
{
    ////////////////////////
    // Keys init
    ////////////////////////

    const Key Keys::UndefinedKey("Undefined");
    const Key Keys::AnyKey("AnyKey");

    const Key Keys::MouseX("MouseX");
    const Key Keys::MouseY("MouseY");
    const Key Keys::Mouse2D("Mouse2D");
    const Key Keys::MouseScrollUp("MouseScrollUp");
    const Key Keys::MouseScrollDown("MouseScrollDown");
    const Key Keys::MouseWheelAxis("MouseWheelAxis");

    const Key Keys::LeftMouseButton("LeftMouseButton");
    const Key Keys::RightMouseButton("RightMouseButton");
    const Key Keys::MiddleMouseButton("MiddleMouseButton");
    const Key Keys::ThumbMouseButton("ThumbMouseButton");
    const Key Keys::ThumbMouseButton2("ThumbMouseButton2");

    const Key Keys::BackSpace("BackSpace");
    const Key Keys::Tab("Tab");
    const Key Keys::Enter("Enter");
    const Key Keys::Pause("Pause");

    const Key Keys::CapsLock("CapsLock");
    const Key Keys::Escape("Escape");
    const Key Keys::SpaceBar("SpaceBar");
    const Key Keys::PageUp("PageUp");
    const Key Keys::PageDown("PageDown");
    const Key Keys::End("End");
    const Key Keys::Home("Home");

    const Key Keys::Left("Left");
    const Key Keys::Up("Up");
    const Key Keys::Right("Right");
    const Key Keys::Down("Down");

    const Key Keys::Insert("Insert");
    const Key Keys::Delete("Delete");

    const Key Keys::Zero("Zero");
    const Key Keys::One("One");
    const Key Keys::Two("Two");
    const Key Keys::Three("Three");
    const Key Keys::Four("Four");
    const Key Keys::Five("Five");
    const Key Keys::Six("Six");
    const Key Keys::Seven("Seven");
    const Key Keys::Eight("Eight");
    const Key Keys::Nine("Nine");

    const Key Keys::A("A");
    const Key Keys::B("B");
    const Key Keys::C("C");
    const Key Keys::D("D");
    const Key Keys::E("E");
    const Key Keys::F("F");
    const Key Keys::G("G");
    const Key Keys::H("H");
    const Key Keys::I("I");
    const Key Keys::J("J");
    const Key Keys::K("K");
    const Key Keys::L("L");
    const Key Keys::M("M");
    const Key Keys::N("N");
    const Key Keys::O("O");
    const Key Keys::P("P");
    const Key Keys::Q("Q");
    const Key Keys::R("R");
    const Key Keys::S("S");
    const Key Keys::T("T");
    const Key Keys::U("U");
    const Key Keys::V("V");
    const Key Keys::W("W");
    const Key Keys::X("X");
    const Key Keys::Y("Y");
    const Key Keys::Z("Z");

    const Key Keys::NumPadZero("NumPadZero");
    const Key Keys::NumPadOne("NumPadOne");
    const Key Keys::NumPadTwo("NumPadTwo");
    const Key Keys::NumPadThree("NumPadThree");
    const Key Keys::NumPadFour("NumPadFour");
    const Key Keys::NumPadFive("NumPadFive");
    const Key Keys::NumPadSix("NumPadSix");
    const Key Keys::NumPadSeven("NumPadSeven");
    const Key Keys::NumPadEight("NumPadEight");
    const Key Keys::NumPadNine("NumPadNine");

    const Key Keys::Multiply("Multiply");
    const Key Keys::Add("Add");
    const Key Keys::Subtract("Subtract");
    const Key Keys::Decimal("Decimal");
    const Key Keys::Divide("Divide");

    const Key Keys::F1("F1");
    const Key Keys::F2("F2");
    const Key Keys::F3("F3");
    const Key Keys::F4("F4");
    const Key Keys::F5("F5");
    const Key Keys::F6("F6");
    const Key Keys::F7("F7");
    const Key Keys::F8("F8");
    const Key Keys::F9("F9");
    const Key Keys::F10("F10");
    const Key Keys::F11("F11");
    const Key Keys::F12("F12");

    const Key Keys::NumLock("NumLock");
    const Key Keys::ScrollLock("ScrollLock");

    const Key Keys::LeftShift("LeftShift");
    const Key Keys::RightShift("RightShift");
    const Key Keys::LeftControl("LeftControl");
    const Key Keys::RightControl("RightControl");
    const Key Keys::LeftAlt("LeftAlt");
    const Key Keys::RightAlt("RightAlt");
    const Key Keys::LeftCommand("LeftCommand");
    const Key Keys::RightCommand("RightCommand");

    const Key Keys::Semicolon("Semicolon");
    const Key Keys::Equals("Equals");
    const Key Keys::Comma("Comma");
    const Key Keys::Underscore("Underscore");
    const Key Keys::Hyphen("Hyphen");
    const Key Keys::Period("Period");
    const Key Keys::Slash("Slash");
    const Key Keys::Tilde("Tilde");
    const Key Keys::LeftBracket("LeftBracket");
    const Key Keys::RightBracket("RightBracket");
    const Key Keys::LeftParenthesis("LeftParenthesis");
    const Key Keys::RightParenthesis("RightParenthesis");
    const Key Keys::Apostrophe("Apostrophe");
    const Key Keys::Quote("Quote");

    const Key Keys::Dollar("Dollar");
    const Key Keys::Exclamation("Exclamation");
    const Key Keys::Colon("Colon");
    const Key Keys::Caret("Caret");
    const Key Keys::Backslash("BackSlash");
    const Key Keys::Asterix("Asterix");
    const Key Keys::Ampersand("Ampersand");

    ////////////////////////
    // Key class
    ////////////////////////

    void Key::LookupForDetails() const
    {
        if (m_keyDetails.get() == nullptr)
        {
            m_keyDetails = Keys::GetKeyDetails(*this);
        }
    }

    Key::Key(const char* aKeyName) : m_keyName(aKeyName)
    {
    }

    Key::Key(const LWString& aKeyName) : m_keyName(aKeyName)
    {
    }

    bool Key::IsValid() const
    {
        if (m_keyName != "")
        {
            LookupForDetails();
            return m_keyDetails.get() != nullptr;
        }

        return false;
    }

    bool Key::IsModifierKey() const
    {
        LookupForDetails();
        return (m_keyDetails.get() != nullptr ? m_keyDetails->IsModifierKey() : false);
    }

    bool Key::IsGamepadKey() const
    {
        LookupForDetails();
        return (m_keyDetails.get() != nullptr ? m_keyDetails->IsGamepadKey() : false);
    }

    bool Key::IsMouseButton() const
    {
        LookupForDetails();
        return (m_keyDetails.get() != nullptr ? m_keyDetails->IsMouseButton() : false);
    }

    KeyDetails* Key::GetDetails() const
    {
        LookupForDetails();
        return m_keyDetails.get();
    }

    const LWString& Key::GetAlternateName() const
    {
        LookupForDetails();
        return (m_keyDetails.get() != nullptr ? m_keyDetails->GetAlternateName() : m_keyName);
    }

    KeyState Key::GetKeyState() const
    {
        LookupForDetails();
        return (m_keyDetails.get() != nullptr ? m_keyDetails->GetKeyState() : KeyState::KeyState_Undefined);
    }

    const char* Key::ToCString() const
    {
        return m_keyName.c_str();
    }

    eastl::string Key::ToString() const
    {
        return m_keyName.eastl_str();
    }

    bool operator==(const Key& aKeyA, const Key& aKeyB)
    {
        return aKeyA.m_keyName == aKeyB.m_keyName;
    }

    bool operator!=(const Key& aKeyA, const Key& aKeyB)
    {
        return aKeyA.m_keyName != aKeyB.m_keyName;
    }

    bool operator<(const Key& aKeyA, const Key& aKeyB)
    {
        return aKeyA.m_keyName.LexicalLess(aKeyB.m_keyName.c_str());
    }

    ////////////////////////
    // KeyDetails class
    ////////////////////////

    KeyDetails::KeyDetails(Key aKey, const LWString& aAlternateName, const std::uint32_t aKeyFlags = 0)
        : m_keyObj(std::move(aKey)), m_alternateName(aAlternateName), m_keyState(KeyState::KeyState_KeyReleased)
    {
        CommonInit(aKeyFlags);
    }

    void KeyDetails::CommonInit(const std::uint32_t aKeyFlags)
    {
        m_isModifierKey = ((aKeyFlags & KeyFlags::keyFlag_ModifierKey) != 0);
        m_isGamepadKey = ((aKeyFlags & KeyFlags::keyFlag_GamepadKey) != 0);
        m_isMouseButton = ((aKeyFlags & KeyFlags::keyFlag_MouseButton) != 0);
    }

    bool KeyDetails::IsModifierKey() const
    {
        return m_isModifierKey != 0;
    }

    bool KeyDetails::IsGamepadKey() const
    {
        return m_isGamepadKey != 0;
    }

    bool KeyDetails::IsMouseButton() const
    {
        return m_isMouseButton != 0;
    }

    const Key& KeyDetails::GetKey() const
    {
        return m_keyObj;
    }

    const LWString& KeyDetails::GetAlternateName() const
    {
        return m_alternateName;
    }

    KeyState KeyDetails::GetKeyState() const
    {
        return m_keyState;
    }

    void KeyDetails::SetKeyState(const KeyState aKeyState)
    {
        m_keyState = aKeyState;
    }

    ////////////////////////
    // Keys class
    ////////////////////////

    eastl::map<Key, SharedPtr<KeyDetails>> Keys::s_inputKeys;
    bool Keys::s_isInitialized = false;

    void Keys::Initialize()
    {
        if (s_isInitialized)
        {
            return;
        }
        s_isInitialized = true;

        // clang-format off
        AddKey(KeyDetails(Keys::UndefinedKey, "Undefined"));
        AddKey(KeyDetails(Keys::AnyKey, "Any Key"));

        AddKey(KeyDetails(Keys::MouseX, "Mouse X", KeyDetails::keyFlag_Axis1D | KeyDetails::keyFlag_MouseButton));
        AddKey(KeyDetails(Keys::MouseY, "Mouse Y", KeyDetails::keyFlag_Axis1D | KeyDetails::keyFlag_MouseButton));
        AddKey(KeyDetails(Keys::MouseWheelAxis, "Mouse Wheel Axis", KeyDetails::keyFlag_Axis1D | KeyDetails::keyFlag_MouseButton));
        AddKey(KeyDetails(Keys::MouseScrollUp, "Mouse Scroll Up", KeyDetails::keyFlag_MouseButton | KeyDetails::keyFlag_ButtonAxis));
        AddKey(KeyDetails(Keys::MouseScrollDown, "Mouse Scroll Down", KeyDetails::keyFlag_MouseButton | KeyDetails::keyFlag_ButtonAxis));
        AddKey(KeyDetails(Keys::LeftMouseButton, "Left Mouse button", KeyDetails::keyFlag_MouseButton));
        AddKey(KeyDetails(Keys::RightMouseButton, "Right Mouse Button", KeyDetails::keyFlag_MouseButton));
        AddKey(KeyDetails(Keys::MiddleMouseButton, "Middle Mouse Button", KeyDetails::keyFlag_MouseButton));
        AddKey(KeyDetails(Keys::ThumbMouseButton, "Thumb Mouse Button", KeyDetails::keyFlag_MouseButton));
        AddKey(KeyDetails(Keys::ThumbMouseButton2, "Thumb Mouse Button 2", KeyDetails::keyFlag_MouseButton));

        AddKey(KeyDetails(Keys::Tab, "Tab"));
        AddKey(KeyDetails(Keys::Enter, "Enter"));
        AddKey(KeyDetails(Keys::Pause, "Pause"));
        AddKey(KeyDetails(Keys::CapsLock, "Caps Lock"));
        AddKey(KeyDetails(Keys::Escape, "Escape"));
        AddKey(KeyDetails(Keys::SpaceBar, "Space Bar"));
        AddKey(KeyDetails(Keys::PageUp, "Page Up"));
        AddKey(KeyDetails(Keys::PageDown, "Page Down"));
        AddKey(KeyDetails(Keys::End, "End"));
        AddKey(KeyDetails(Keys::Home, "Home"));

        AddKey(KeyDetails(Keys::Left, "Left"));
        AddKey(KeyDetails(Keys::Right, "Right"));
        AddKey(KeyDetails(Keys::Up, "Up"));
        AddKey(KeyDetails(Keys::Down, "Down"));

        AddKey(KeyDetails(Keys::Insert, "Insert"));
        AddKey(KeyDetails(Keys::BackSpace, "Back Space"));
        AddKey(KeyDetails(Keys::Delete, "Delete"));

        AddKey(KeyDetails(Keys::Zero, "0"));
        AddKey(KeyDetails(Keys::One, "1"));
        AddKey(KeyDetails(Keys::Two, "2"));
        AddKey(KeyDetails(Keys::Three, "3"));
        AddKey(KeyDetails(Keys::Four, "4"));
        AddKey(KeyDetails(Keys::Five, "5"));
        AddKey(KeyDetails(Keys::Six, "6"));
        AddKey(KeyDetails(Keys::Seven, "7"));
        AddKey(KeyDetails(Keys::Eight, "8"));
        AddKey(KeyDetails(Keys::Nine, "9"));

        AddKey(KeyDetails(Keys::A, "A"));
        AddKey(KeyDetails(Keys::B, "B"));
        AddKey(KeyDetails(Keys::C, "C"));
        AddKey(KeyDetails(Keys::D, "D"));
        AddKey(KeyDetails(Keys::E, "E"));
        AddKey(KeyDetails(Keys::F, "F"));
        AddKey(KeyDetails(Keys::G, "G"));
        AddKey(KeyDetails(Keys::H, "H"));
        AddKey(KeyDetails(Keys::I, "I"));
        AddKey(KeyDetails(Keys::J, "J"));
        AddKey(KeyDetails(Keys::K, "K"));
        AddKey(KeyDetails(Keys::L, "L"));
        AddKey(KeyDetails(Keys::M, "M"));
        AddKey(KeyDetails(Keys::N, "N"));
        AddKey(KeyDetails(Keys::O, "O"));
        AddKey(KeyDetails(Keys::P, "P"));
        AddKey(KeyDetails(Keys::Q, "Q"));
        AddKey(KeyDetails(Keys::R, "R"));
        AddKey(KeyDetails(Keys::S, "S"));
        AddKey(KeyDetails(Keys::T, "T"));
        AddKey(KeyDetails(Keys::U, "U"));
        AddKey(KeyDetails(Keys::V, "V"));
        AddKey(KeyDetails(Keys::W, "W"));
        AddKey(KeyDetails(Keys::X, "X"));
        AddKey(KeyDetails(Keys::Y, "Y"));
        AddKey(KeyDetails(Keys::Z, "Z"));

        AddKey(KeyDetails(Keys::NumPadZero, "Num 0"));
        AddKey(KeyDetails(Keys::NumPadOne, "Num 1"));
        AddKey(KeyDetails(Keys::NumPadTwo, "Num 2"));
        AddKey(KeyDetails(Keys::NumPadThree, "Num 3"));
        AddKey(KeyDetails(Keys::NumPadFour, "Num 4"));
        AddKey(KeyDetails(Keys::NumPadFive, "Num 5"));
        AddKey(KeyDetails(Keys::NumPadSix, "Num 6"));
        AddKey(KeyDetails(Keys::NumPadSeven, "Num 7"));
        AddKey(KeyDetails(Keys::NumPadEight, "Num 8"));
        AddKey(KeyDetails(Keys::NumPadNine, "Num 9"));

        AddKey(KeyDetails(Keys::Multiply, "Num *"));
        AddKey(KeyDetails(Keys::Add, "Num +"));
        AddKey(KeyDetails(Keys::Subtract, "Num -"));
        AddKey(KeyDetails(Keys::Decimal, "Num ."));
        AddKey(KeyDetails(Keys::Divide, "Num /"));

        AddKey(KeyDetails(Keys::F1, "F1"));
        AddKey(KeyDetails(Keys::F2, "F2"));
        AddKey(KeyDetails(Keys::F3, "F3"));
        AddKey(KeyDetails(Keys::F4, "F4"));
        AddKey(KeyDetails(Keys::F5, "F5"));
        AddKey(KeyDetails(Keys::F6, "F6"));
        AddKey(KeyDetails(Keys::F7, "F7"));
        AddKey(KeyDetails(Keys::F8, "F8"));
        AddKey(KeyDetails(Keys::F9, "F9"));
        AddKey(KeyDetails(Keys::F10, "F10"));
        AddKey(KeyDetails(Keys::F11, "F11"));
        AddKey(KeyDetails(Keys::F12, "F12"));

        AddKey(KeyDetails(Keys::NumLock, "Num Lock"));
        AddKey(KeyDetails(Keys::ScrollLock, "Scroll Lock"));
        AddKey(KeyDetails(Keys::LeftShift, "Left Shift"));
        AddKey(KeyDetails(Keys::RightShift, "Right Shift"));
        AddKey(KeyDetails(Keys::LeftControl, "Left Ctrl"));
        AddKey(KeyDetails(Keys::RightControl, "Right Ctrl"));
        AddKey(KeyDetails(Keys::LeftCommand, "Left Cmd"));
        AddKey(KeyDetails(Keys::RightCommand, "Right Cmd"));
        AddKey(KeyDetails(Keys::LeftAlt, "Left Alt"));
        AddKey(KeyDetails(Keys::RightAlt, "Right Alt"));

        AddKey(KeyDetails(Keys::Semicolon, ";"));
        AddKey(KeyDetails(Keys::Equals, "="));
        AddKey(KeyDetails(Keys::Comma, ","));
        AddKey(KeyDetails(Keys::Hyphen, "-"));
        AddKey(KeyDetails(Keys::Underscore, "_"));
        AddKey(KeyDetails(Keys::Period, "."));
        AddKey(KeyDetails(Keys::Slash, "/"));
        AddKey(KeyDetails(Keys::Tilde, "`")); // Yes this is not actually a tilde, it is a long, sad, and old story https://github.com/EpicGames/UnrealEngine/blob/40eea367040d50aadd9f030ed5909fc890c159c2/Engine/Source/Runtime/InputCore/Private/InputCoreTypes.cpp
        AddKey(KeyDetails(Keys::LeftBracket, "["));
        AddKey(KeyDetails(Keys::RightBracket, "]"));
        AddKey(KeyDetails(Keys::Backslash, "\\"));
        AddKey(KeyDetails(Keys::Apostrophe, "'"));
        AddKey(KeyDetails(Keys::Quote, "\""));

        AddKey(KeyDetails(Keys::LeftParenthesis, "("));
        AddKey(KeyDetails(Keys::RightParenthesis, ")"));
        AddKey(KeyDetails(Keys::Ampersand, "&"));
        AddKey(KeyDetails(Keys::Asterix, "*"));
        AddKey(KeyDetails(Keys::Caret, "^"));
        AddKey(KeyDetails(Keys::Dollar, "$"));
        AddKey(KeyDetails(Keys::Exclamation, "!"));
        AddKey(KeyDetails(Keys::Colon, ":"));
        // clang-format on
    }

    void Keys::AddKey(const KeyDetails& aKeyDetails)
    {
        // TODO: check if key already exists

        const Key& key = aKeyDetails.GetKey();
        key.m_keyDetails = eastl::shared_ptr<KeyDetails>(new KeyDetails(aKeyDetails));
        s_inputKeys.insert(eastl::pair<const Key, SharedPtr<KeyDetails>>(key, key.m_keyDetails));
    }

    SharedPtr<KeyDetails> Keys::GetKeyDetails(const Key& aKey)
    {
        SharedPtr<KeyDetails> keyDetails = s_inputKeys[aKey];
        // TODO: Add assertion
        return keyDetails;
    }

    bool Keys::IsModifierKey(const Key& aKey)
    {
        return aKey.IsModifierKey();
    }

    bool Keys::IsGamepadKey(const Key& aKey)
    {
        return aKey.IsGamepadKey();
    }

    bool Keys::IsMouseButton(const Key& aKey)
    {
        return aKey.IsMouseButton();
    }
}
