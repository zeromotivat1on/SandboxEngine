#pragma once

#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Core/InputBits.h"

namespace snd::input
{
	// State of keyboard input.
	struct Keyboard
	{
		std::bitset<static_cast<uint8_t>(KeyboardBit::Count)> Buttons = 0;		// current frame's button states
		std::bitset<static_cast<uint8_t>(KeyboardBit::Count)> PrevButtons = 0;	// previous frame's button states
		std::bitset<static_cast<uint8_t>(KeyboardBit::Count)> ButtonsDown = 0;	// pressed buttons this frame
		std::bitset<static_cast<uint8_t>(KeyboardBit::Count)> ButtonsUp = 0;	// released buttons this frame
	};

	// State of mouse input.
	struct Mouse
	{
		float X = 0.0f;
		float Y = 0.0f;
		
		uint8_t Buttons = 0;
		uint8_t PrevButtons = 0;
		uint8_t ButtonsDown = 0;
		uint8_t ButtonsUp = 0;
	};

	// State of gamepad input. Thumbstick and trigger values range from -1.0 to 1.0 inclusive.
	struct Gamepad
	{
		uint32_t Buttons = 0;
		uint32_t PrevButtons = 0;
		uint32_t ButtonsDown = 0;
		uint32_t ButtonsUp = 0;
		
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

	// Get input state.
	const Keyboard&	KeyboardState();
	const Gamepad&	GamepadState();
	const Mouse&	MouseState();

	glm::vec2 MousePosition();

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
	KeyboardBit ConvertKeyboardCode(int32_t keycode);
	GamepadBit	ConvertGamepadCode(int32_t keycode);
	MouseBit	ConvertMouseCode(int32_t keycode);

	// Convert engine system key to platform keycode.
	int32_t ConvertBit(KeyboardBit bit);
	int32_t ConvertBit(GamepadBit bit);
	int32_t ConvertBit(MouseBit bit);
}
