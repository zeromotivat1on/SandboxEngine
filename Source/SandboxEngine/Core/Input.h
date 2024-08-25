#pragma once

#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Events/KeyEvent.h"
#include "SandboxEngine/Events/MouseEvent.h"

namespace snd::input
{
	// Describes key code and the position of bit in button state.
	enum class KeyboardBit : uint32_t
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
	enum class GamepadBit : uint32_t
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
	enum class MouseBit : uint32_t
	{
		Left,
		Right,
		Middle,
		Count
	};

	// Current frame state of keyboard input.
	struct Keyboard
	{
		std::bitset<static_cast<uint32_t>(KeyboardBit::Count)> Buttons = 0;
	};

	// Current frame state of mouse input.
	struct Mouse
	{
		float X = 0.0f;
		float Y = 0.0f;
		uint8_t Buttons = 0;
	};

	// Current frame state of gamepad input.
	// Thumbstick and trigger values range from -1.0 to 1.0 inclusive.
	struct Gamepad
	{
		uint32_t Buttons = 0;
		float ThumbLX = 0;
		float ThumbLY = 0;
		float ThumbRX = 0;
		float ThumbRY = 0;
		float TriggerL = 0;
		float TriggerR = 0;
	};

	void Init(Window* window);
	void Shutdown();
	void Update();

	const Keyboard&	KeyboardState();
	const Gamepad&	GamepadState();
	const Mouse&	MouseState();
	
	KeyboardBit ConvertKeyboardCode(int32_t keycode);
	GamepadBit	ConvertGamepadCode(int32_t keycode);
	MouseBit	ConvertMouseCode(int32_t keycode);

	int32_t ConvertBit(KeyboardBit bit);
	int32_t ConvertBit(GamepadBit bit);
	int32_t ConvertBit(MouseBit bit);

	bool IsButtonDown(KeyboardBit bit);
	bool IsButtonDown(GamepadBit bit);
	bool IsButtonDown(MouseBit bit);

	glm::vec2 GetMousePosition();
}
