#pragma once

#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Core/InputBits.h"

namespace snd::input
{
	// State of keyboard input.
	struct Keyboard
	{
		std::bitset<static_cast<u8>(KeyboardBit::Count)> Buttons;		// current frame's button states
		std::bitset<static_cast<u8>(KeyboardBit::Count)> PrevButtons;	// previous frame's button states
		std::bitset<static_cast<u8>(KeyboardBit::Count)> ButtonsDown;	// pressed buttons this frame
		std::bitset<static_cast<u8>(KeyboardBit::Count)> ButtonsUp;		// released buttons this frame
	};

	// State of mouse input.
	struct Mouse
	{
		f32 X;
		f32 Y;
		
		f32 ScrollX;
		f32 ScrollY;
		
		u8 Buttons;
		u8 PrevButtons;
		u8 ButtonsDown;
		u8 ButtonsUp;
	};

	// State of gamepad input. Thumbstick and trigger values range from -1.0 to 1.0 inclusive.
	struct Gamepad
	{
		u32 Buttons;
		u32 PrevButtons;
		u32 ButtonsDown;
		u32 ButtonsUp;
		
		f32 ThumbLX;
		f32 ThumbLY;
		f32 ThumbRX;
		f32 ThumbRY;
		f32 TriggerL;
		f32 TriggerR;
	};

	void Init(Window* window);
	void Shutdown();
	void Update();

	// Get input state.
	const Keyboard&	KeyboardState();
	const Gamepad&	GamepadState();
	const Mouse&	MouseState();

	vec2 MousePosition();
	vec2 MouseOffset();
	vec2 MouseScroll();

	void OnMouseScroll(f32 x, f32 y);
	
	// Check if given button is currently pressed.
	bool ButtonDown(KeyboardBit bit);
	bool ButtonDown(GamepadBit bit);
	bool ButtonDown(MouseBit bit);
	
	// Check if given button was pressed this frame.
	bool ButtonJustWentDown(KeyboardBit bit);
	bool ButtonJustWentDown(GamepadBit bit);
	bool ButtonJustWentDown(MouseBit bit);

	// Check if any button was pressed this frame except for a given one.
	bool AnyButtonJustWentDownExcept(KeyboardBit bit);
	bool AnyButtonJustWentDownExcept(GamepadBit bit);
	bool AnyButtonJustWentDownExcept(MouseBit bit);
	
	// Check if given button was released this frame.
	bool ButtonJustWentUp(KeyboardBit bit);
	bool ButtonJustWentUp(GamepadBit bit);
	bool ButtonJustWentUp(MouseBit bit);
	
	// Convert platform keycode to engine system key.
	KeyboardBit ConvertKeyboardCode(s32 keycode);
	GamepadBit	ConvertGamepadCode(s32 keycode);
	MouseBit	ConvertMouseCode(s32 keycode);

	// Convert engine system key to platform keycode.
	s32 ConvertBit(KeyboardBit bit);
	s32 ConvertBit(GamepadBit bit);
	s32 ConvertBit(MouseBit bit);
}
