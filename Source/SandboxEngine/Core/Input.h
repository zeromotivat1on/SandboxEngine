#pragma once

namespace snd
{
    enum ButtonState : s8
    {
        INPUT_RELEASE,
        INPUT_PRESS,
        INPUT_RELEASE_FRAME,
        INPUT_PRESS_FRAME,
    };

    enum Button : s16
    {
        INPUT_KEY_INVALID = -1,

        // Printable keys.
        INPUT_KEY_SPACE,
        INPUT_KEY_FIRST = INPUT_KEY_SPACE,
        INPUT_BUTTON_FIRST = INPUT_KEY_FIRST,
        INPUT_KEY_APOSTROPHE,
        INPUT_KEY_COMMA,
        INPUT_KEY_MINUS,
        INPUT_KEY_PERIOD,
        INPUT_KEY_SLASH,
        INPUT_KEY_0,
        INPUT_KEY_1,
        INPUT_KEY_2,
        INPUT_KEY_3,
        INPUT_KEY_4,
        INPUT_KEY_5,
        INPUT_KEY_6,
        INPUT_KEY_7,
        INPUT_KEY_8,
        INPUT_KEY_9,
        INPUT_KEY_SEMICOLON,
        INPUT_KEY_EQUAL,
        INPUT_KEY_A,
        INPUT_KEY_B,
        INPUT_KEY_C,
        INPUT_KEY_D,
        INPUT_KEY_E,
        INPUT_KEY_F,
        INPUT_KEY_G,
        INPUT_KEY_H,
        INPUT_KEY_I,
        INPUT_KEY_J,
        INPUT_KEY_K,
        INPUT_KEY_L,
        INPUT_KEY_M,
        INPUT_KEY_N,
        INPUT_KEY_O,
        INPUT_KEY_P,
        INPUT_KEY_Q,
        INPUT_KEY_R,
        INPUT_KEY_S,
        INPUT_KEY_T,
        INPUT_KEY_U,
        INPUT_KEY_V,
        INPUT_KEY_W,
        INPUT_KEY_X,
        INPUT_KEY_Y,
        INPUT_KEY_Z,
        INPUT_KEY_LEFT_BRACKET,
        INPUT_KEY_BACKSLASH,
        INPUT_KEY_RIGHT_BRACKET,
        INPUT_KEY_GRAVE_ACCENT,

        // Function keys.
        INPUT_KEY_ESCAPE,
        INPUT_KEY_ENTER,
        INPUT_KEY_TAB,
        INPUT_KEY_BACKSPACE,
        INPUT_KEY_INSERT,
        INPUT_KEY_DELETE,
        INPUT_KEY_RIGHT,
        INPUT_KEY_LEFT,
        INPUT_KEY_DOWN,
        INPUT_KEY_UP,
        INPUT_KEY_PAGE_UP,
        INPUT_KEY_PAGE_DOWN,
        INPUT_KEY_HOME,
        INPUT_KEY_END,
        INPUT_KEY_CAPS_LOCK,
        INPUT_KEY_SCROLL_LOCK,
        INPUT_KEY_NUM_LOCK,
        INPUT_KEY_PRINT_SCREEN,
        INPUT_KEY_PAUSE,
        INPUT_KEY_F1,
        INPUT_KEY_F2,
        INPUT_KEY_F3,
        INPUT_KEY_F4,
        INPUT_KEY_F5,
        INPUT_KEY_F6,
        INPUT_KEY_F7,
        INPUT_KEY_F8,
        INPUT_KEY_F9,
        INPUT_KEY_F10,
        INPUT_KEY_F11,
        INPUT_KEY_F12,
        INPUT_KEY_F13,
        INPUT_KEY_F14,
        INPUT_KEY_F15,
        INPUT_KEY_F16,
        INPUT_KEY_F17,
        INPUT_KEY_F18,
        INPUT_KEY_F19,
        INPUT_KEY_F20,
        INPUT_KEY_F21,
        INPUT_KEY_F22,
        INPUT_KEY_F23,
        INPUT_KEY_F24,
        INPUT_KEY_F25,
        INPUT_KEY_KP_0,
        INPUT_KEY_KP_1,
        INPUT_KEY_KP_2,
        INPUT_KEY_KP_3,
        INPUT_KEY_KP_4,
        INPUT_KEY_KP_5,
        INPUT_KEY_KP_6,
        INPUT_KEY_KP_7,
        INPUT_KEY_KP_8,
        INPUT_KEY_KP_9,
        INPUT_KEY_KP_DECIMAL,
        INPUT_KEY_KP_DIVIDE,
        INPUT_KEY_KP_MULTIPLY,
        INPUT_KEY_KP_SUBTRACT,
        INPUT_KEY_KP_ADD,
        INPUT_KEY_KP_ENTER,
        INPUT_KEY_KP_EQUAL,
        INPUT_KEY_LEFT_SHIFT,
        INPUT_KEY_LEFT_CTRL,
        INPUT_KEY_LEFT_ALT,
        INPUT_KEY_LEFT_SUPER,
        INPUT_KEY_RIGHT_SHIFT,
        INPUT_KEY_RIGHT_CTRL,
        INPUT_KEY_RIGHT_ALT,
        INPUT_KEY_RIGHT_SUPER,
        INPUT_KEY_MENU,
        INPUT_KEY_LAST = INPUT_KEY_MENU,

        // Mouse buttons.
        INPUT_MOUSE_LEFT,
        INPUT_MOUSE_FIRST = INPUT_MOUSE_LEFT,
        INPUT_MOUSE_RIGHT,
        INPUT_MOUSE_MIDDLE,
        INPUT_MOUSE_LAST = INPUT_MOUSE_MIDDLE,

        // Gamepad buttons.
        INPUT_GAMEPAD_FACE_UP,
        INPUT_GAMEPAD_FIRST = INPUT_GAMEPAD_FACE_UP,
        INPUT_GAMEPAD_FACE_RIGHT,
        INPUT_GAMEPAD_FACE_DOWN,
        INPUT_GAMEPAD_FACE_LEFT,
        INPUT_GAMEPAD_LB,
        INPUT_GAMEPAD_RB,
        INPUT_GAMEPAD_BACK,
        INPUT_GAMEPAD_START,
        INPUT_GAMEPAD_GUIDE,
        INPUT_GAMEPAD_LTHUMB,
        INPUT_GAMEPAD_RTHUMB,
        INPUT_GAMEPAD_DPAD_UP,
        INPUT_GAMEPAD_DPAD_RIGHT,
        INPUT_GAMEPAD_DPAD_DOWN,
        INPUT_GAMEPAD_DPAD_LEFT,
        INPUT_GAMEPAD_LAST = INPUT_GAMEPAD_DPAD_LEFT,

        INPUT_GAMEPAD_TRIANGLE = INPUT_GAMEPAD_FACE_UP,
        INPUT_GAMEPAD_CIRCLE = INPUT_GAMEPAD_FACE_RIGHT,
        INPUT_GAMEPAD_CROSS = INPUT_GAMEPAD_FACE_DOWN,
        INPUT_GAMEPAD_SQUARE = INPUT_GAMEPAD_FACE_LEFT,

        INPUT_GAMEPAD_Y = INPUT_GAMEPAD_FACE_UP,
        INPUT_GAMEPAD_B = INPUT_GAMEPAD_FACE_RIGHT,
        INPUT_GAMEPAD_A = INPUT_GAMEPAD_FACE_DOWN,
        INPUT_GAMEPAD_X = INPUT_GAMEPAD_FACE_LEFT,

        INPUT_BUTTON_LAST = INPUT_GAMEPAD_LAST,
        INPUT_BUTTON_COUNT,
    };

    enum Axis : s16
    {
        // Mouse axes.
        INPUT_MOUSE_X,
        INPUT_MOUSE_AXIS_FIRST = INPUT_MOUSE_X,
        INPUT_AXIS_FIRST = INPUT_MOUSE_AXIS_FIRST,
        INPUT_MOUSE_Y,
        INPUT_MOUSE_OFFSET_X,
        INPUT_MOUSE_OFFSET_Y,
        INPUT_MOUSE_SCROLL_X,
        INPUT_MOUSE_SCROLL_Y,
        INPUT_MOUSE_AXIS_LAST = INPUT_MOUSE_SCROLL_Y,

        // Gamepad axes.
        INPUT_GAMEPAD_LX,
        INPUT_GAMEPAD_AXIS_FIRST = INPUT_GAMEPAD_LX,
        INPUT_GAMEPAD_LY,
        INPUT_GAMEPAD_RX,
        INPUT_GAMEPAD_RY,
        INPUT_GAMEPAD_LT,
        INPUT_GAMEPAD_RT,
        INPUT_GAMEPAD_AXIS_LAST = INPUT_GAMEPAD_RT,

        INPUT_AXIS_LAST = INPUT_GAMEPAD_AXIS_LAST,
        INPUT_AXIS_COUNT = (INPUT_AXIS_LAST - INPUT_AXIS_FIRST + 1)
    };

    inline s16 gButtonMap[INPUT_BUTTON_COUNT];
    inline s16 gAxisMap[INPUT_AXIS_COUNT];

    void InitInputMaps();
}
