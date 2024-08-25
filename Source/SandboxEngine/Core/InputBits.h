#pragma once

// The header describes HID abstract input keys. 

namespace snd::input
{
    // Describes key code and the position of bit in button state.
    enum class KeyboardBit : uint8_t
    {
        Space,
        Apostrophe,		/* ' */
        Comma,			/* , */
        Minus,			/* - */
        Period,			/* . */
        Slash,			/* / */
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        Semicolon,		/* ; */
        Equal,			/* = */
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        LeftBracket,	/* [ */
        Backslash,		/* \ */
        RightBracket,	/* ] */
        GraveAccent,	/* ` */
        Escape,
        Enter,
        Tab,
        Backspace,
        Insert,
        Delete,
        Right,
        Left,
        Down,
        Up,
        PageUp,
        PageDown,
        Home,
        End,
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        KP0,
        KP1,
        KP2,
        KP3,
        KP4,
        KP5,
        KP6,
        KP7,
        KP8,
        KP9,
        KPDecimal,
        KPDivide,
        KPMultiply,
        KPSubtract,
        KPAdd,
        KPEnter,
        KPEqual,
        LeftShift,
        LeftControl,
        LeftAlt,
        RightShift,
        RightControl,
        RightAlt,
        Count
    };

    // Describes key code and the position of bit in gamepad state.
    enum class GamepadBit : uint8_t
    {
        DpadUp,
        DpadDown,
        DpadLeft,
        DpadRight,
        FaceUp,
        FaceDown,
        FaceLeft,
        FaceRight,
        SpecialLeft,
        SpecialRight,
        ThumbLeft,
        ThumbRight,
        BumperLeft,
        BumperRight,
        Count
    };

    // Describes key code and the position of bit in mouse state.
    enum class MouseBit : uint8_t
    {
        Left,
        Right,
        Middle,
        Count
    };
}
