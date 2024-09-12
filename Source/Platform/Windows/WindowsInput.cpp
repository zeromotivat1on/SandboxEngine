#include "sndpch.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Core/Error.h"
#include "Platform/Windows/WindowsWindow.h"
#include <GLFW/glfw3.h>

static snd::WindowsWindow* s_Window = nullptr;
static GLFWwindow* s_GlfwWindow = nullptr;

static snd::input::Keyboard s_KeyboardState;
static snd::input::Gamepad s_GamepadState;
static snd::input::Mouse s_MouseState;

static int32_t s_ConvertedKeyboardBits[static_cast<uint32_t>(snd::KeyboardBit::Count)];
static int32_t s_ConvertedGamepadBits[static_cast<uint32_t>(snd::GamepadBit::Count)];
static int32_t s_ConvertedMouseBits[static_cast<uint32_t>(snd::MouseBit::Count)];

template<typename TInputBit>
static void ConvertBits(int32_t* outConvertedBits)
{
	for (uint32_t i = 0; i < static_cast<uint32_t>(TInputBit::Count); ++i)
	{
		outConvertedBits[i] = snd::input::ConvertBit(static_cast<TInputBit>(i));
	}
}

template<typename T>
static void DetectButtonsUpDown(const T& thisFrameButtons, const T& prevFrameButtons, T& outDowns, T& outUps)
{
	const auto buttonChanges = thisFrameButtons ^ prevFrameButtons;
	outDowns = buttonChanges & thisFrameButtons;
	outUps = buttonChanges & (~thisFrameButtons);
}

void snd::input::Init(Window* window)
{
	s_Window = static_cast<WindowsWindow*>(window);
	s_GlfwWindow = static_cast<GLFWwindow*>(s_Window->GetHandle());
	SND_ASSERT(s_Window && s_GlfwWindow);

	// Prepare converted input key bits from engine system to platform specific.
	ConvertBits<KeyboardBit>(s_ConvertedKeyboardBits);
	ConvertBits<GamepadBit>(s_ConvertedGamepadBits);
	ConvertBits<MouseBit>(s_ConvertedMouseBits);
}

void snd::input::Shutdown()
{
	s_Window = nullptr;
	s_GlfwWindow = nullptr;
}

void snd::input::Update()
{
	// Update keyboard state.
	{
		// Update button states.
		for (uint8_t i = 0; i < static_cast<uint8_t>(KeyboardBit::Count); ++i)
		{
			const int32_t keyState = glfwGetKey(s_GlfwWindow, s_ConvertedKeyboardBits[i]);
			s_KeyboardState.Buttons.set(i, keyState == GLFW_PRESS || keyState == GLFW_REPEAT);
		}

		// Detect pressed/released buttons this frame.
		DetectButtonsUpDown(s_KeyboardState.Buttons, s_KeyboardState.PrevButtons, s_KeyboardState.ButtonsDown, s_KeyboardState.ButtonsUp);

		// Cache prev button states.
		s_KeyboardState.PrevButtons = s_KeyboardState.Buttons;
	}
	
	// Update gamepad state.
	{
		GLFWgamepadstate gamepadstate;
		glfwGetGamepadState(0, &gamepadstate); // support only 1 gamepad for now

		// Update button states.
		for (uint8_t i = 0; i < static_cast<uint8_t>(GamepadBit::Count); ++i)
		{
			if (gamepadstate.buttons[s_ConvertedGamepadBits[i]])
			{
				SET_BIT(s_GamepadState.Buttons, i);
			}
			else
			{
				CLEAR_BIT(s_GamepadState.Buttons, i);
			}
		}

		// Detect pressed/released buttons this frame.
		DetectButtonsUpDown(s_GamepadState.Buttons, s_GamepadState.PrevButtons, s_GamepadState.ButtonsDown, s_GamepadState.ButtonsUp);

		// Update axis buttons.
		s_GamepadState.ThumbLX	= gamepadstate.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
		s_GamepadState.ThumbLY	= gamepadstate.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
		s_GamepadState.ThumbRX	= gamepadstate.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
		s_GamepadState.ThumbRY	= gamepadstate.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
		s_GamepadState.TriggerL = gamepadstate.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
		s_GamepadState.TriggerR = gamepadstate.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];

		// Cache prev button states.
		s_GamepadState.PrevButtons = s_GamepadState.Buttons;
	}
	
	// Update mouse state.
	{
		// Update button states.
		for (uint8_t i = 0; i < static_cast<uint8_t>(MouseBit::Count); ++i)
		{
			const int32_t keyState = glfwGetMouseButton(s_GlfwWindow, s_ConvertedMouseBits[i]);

			if (keyState == GLFW_PRESS)
			{
				SET_BIT(s_MouseState.Buttons, i);
			}
			else
			{
				CLEAR_BIT(s_MouseState.Buttons, i);
			}
		}

		// Detect pressed/released buttons this frame.
		DetectButtonsUpDown(s_MouseState.Buttons, s_MouseState.PrevButtons, s_MouseState.ButtonsDown, s_MouseState.ButtonsUp);

		// Update mouse position.
		double mouseX = 0;
		double mouseY = 0;
		glfwGetCursorPos(s_GlfwWindow, &mouseX, &mouseY);
		
		s_MouseState.X = static_cast<float>(mouseX);
		s_MouseState.Y = static_cast<float>(mouseY);

		// Cache prev button states.
		s_MouseState.PrevButtons = s_MouseState.Buttons;
	}
}

const snd::input::Keyboard& snd::input::KeyboardState()
{
	return s_KeyboardState;
}

const snd::input::Gamepad& snd::input::GamepadState()
{
	return s_GamepadState;
}

const snd::input::Mouse& snd::input::MouseState()
{
	return s_MouseState;
}

bool snd::input::ButtonDown(KeyboardBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);
	return s_KeyboardState.Buttons.test(pos);
}

bool snd::input::ButtonDown(GamepadBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);
	return CHECK_BIT(s_GamepadState.Buttons, pos);
}

bool snd::input::ButtonDown(MouseBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);
	return CHECK_BIT(s_MouseState.Buttons, pos);
}

bool snd::input::ButtonJustWentDown(KeyboardBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);
	return s_KeyboardState.ButtonsDown.test(pos);
}

bool snd::input::ButtonJustWentDown(GamepadBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);
	return CHECK_BIT(s_GamepadState.ButtonsDown, pos);
}

bool snd::input::ButtonJustWentDown(MouseBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);
	return CHECK_BIT(s_MouseState.ButtonsDown, pos);
}

bool snd::input::AnyButtonJustWentDownExcept(KeyboardBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);
	return s_KeyboardState.ButtonsDown.any() && !s_KeyboardState.ButtonsDown.test(pos);
}

bool snd::input::AnyButtonJustWentDownExcept(GamepadBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);

	for (uint8_t i = 0; i < static_cast<uint8_t>(GamepadBit::Count); ++i)
	{
		if (i == pos)
		{
			continue;
		}

		if (CHECK_BIT(s_GamepadState.Buttons, i))
		{
			return true;
		}
	}

	return false;
}

bool snd::input::AnyButtonJustWentDownExcept(MouseBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);

	for (uint8_t i = 0; i < static_cast<uint8_t>(MouseBit::Count); ++i)
	{
		if (i == pos)
		{
			continue;
		}

		if (CHECK_BIT(s_MouseState.Buttons, i))
		{
			return true;
		}
	}

	return false;
}

bool snd::input::ButtonJustWentUp(KeyboardBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);
	return s_KeyboardState.ButtonsUp.test(pos);
}

bool snd::input::ButtonJustWentUp(GamepadBit bit)
{
	const uint8_t pos = static_cast<uint8_t>(bit);
	return CHECK_BIT(s_GamepadState.ButtonsUp, pos);
}

bool snd::input::ButtonJustWentUp(MouseBit bit)
{
	const uint32_t pos = static_cast<uint32_t>(bit);
	return CHECK_BIT(s_MouseState.ButtonsUp, pos);
}

glm::vec2 snd::input::MousePosition()
{
	return glm::vec2(s_MouseState.X, s_MouseState.Y);
}

snd::KeyboardBit snd::input::ConvertKeyboardCode(int32_t keycode)
{
	switch (keycode)
	{
		case GLFW_KEY_SPACE:			return KeyboardBit::Space;
		case GLFW_KEY_APOSTROPHE:		return KeyboardBit::Apostrophe;
		case GLFW_KEY_COMMA:			return KeyboardBit::Comma;
		case GLFW_KEY_MINUS:			return KeyboardBit::Minus;
		case GLFW_KEY_PERIOD:			return KeyboardBit::Period;
		case GLFW_KEY_SLASH:			return KeyboardBit::Slash;
		case GLFW_KEY_0:				return KeyboardBit::_0;
		case GLFW_KEY_1:				return KeyboardBit::_1;
		case GLFW_KEY_2:				return KeyboardBit::_2;
		case GLFW_KEY_3:				return KeyboardBit::_3;
		case GLFW_KEY_4:				return KeyboardBit::_4;
		case GLFW_KEY_5:				return KeyboardBit::_5;
		case GLFW_KEY_6:				return KeyboardBit::_6;
		case GLFW_KEY_7:				return KeyboardBit::_7;
		case GLFW_KEY_8:				return KeyboardBit::_8;
		case GLFW_KEY_9:				return KeyboardBit::_9;
		case GLFW_KEY_SEMICOLON:		return KeyboardBit::Semicolon;
		case GLFW_KEY_EQUAL:			return KeyboardBit::Equal;
		case GLFW_KEY_A:				return KeyboardBit::A;
		case GLFW_KEY_B:				return KeyboardBit::B;
		case GLFW_KEY_C:				return KeyboardBit::C;
		case GLFW_KEY_D:				return KeyboardBit::D;
		case GLFW_KEY_E:				return KeyboardBit::E;
		case GLFW_KEY_F:				return KeyboardBit::F;
		case GLFW_KEY_G:				return KeyboardBit::G;
		case GLFW_KEY_H:				return KeyboardBit::H;
		case GLFW_KEY_I:				return KeyboardBit::I;
		case GLFW_KEY_J:				return KeyboardBit::J;
		case GLFW_KEY_K:				return KeyboardBit::K;
		case GLFW_KEY_L:				return KeyboardBit::L;
		case GLFW_KEY_M:				return KeyboardBit::M;
		case GLFW_KEY_N:				return KeyboardBit::N;
		case GLFW_KEY_O:				return KeyboardBit::O;
		case GLFW_KEY_P:				return KeyboardBit::P;
		case GLFW_KEY_Q:				return KeyboardBit::Q;
		case GLFW_KEY_R:				return KeyboardBit::R;
		case GLFW_KEY_S:				return KeyboardBit::S;
		case GLFW_KEY_T:				return KeyboardBit::T;
		case GLFW_KEY_U:				return KeyboardBit::U;
		case GLFW_KEY_V:				return KeyboardBit::V;
		case GLFW_KEY_W:				return KeyboardBit::W;
		case GLFW_KEY_X:				return KeyboardBit::X;
		case GLFW_KEY_Y:				return KeyboardBit::Y;
		case GLFW_KEY_Z:				return KeyboardBit::Z;
		case GLFW_KEY_LEFT_BRACKET:		return KeyboardBit::LeftBracket;
		case GLFW_KEY_BACKSLASH:		return KeyboardBit::Backslash;
		case GLFW_KEY_RIGHT_BRACKET:	return KeyboardBit::RightBracket;
		case GLFW_KEY_GRAVE_ACCENT:		return KeyboardBit::GraveAccent;
		case GLFW_KEY_ESCAPE:			return KeyboardBit::Escape;
		case GLFW_KEY_ENTER:			return KeyboardBit::Enter;
		case GLFW_KEY_TAB:				return KeyboardBit::Tab;
		case GLFW_KEY_BACKSPACE:		return KeyboardBit::Backspace;
		case GLFW_KEY_INSERT:			return KeyboardBit::Insert;
		case GLFW_KEY_DELETE:			return KeyboardBit::Delete;
		case GLFW_KEY_RIGHT:			return KeyboardBit::Right;
		case GLFW_KEY_LEFT:				return KeyboardBit::Left;
		case GLFW_KEY_DOWN:				return KeyboardBit::Down;
		case GLFW_KEY_UP:				return KeyboardBit::Up;
		case GLFW_KEY_PAGE_UP:			return KeyboardBit::PageUp;
		case GLFW_KEY_PAGE_DOWN:		return KeyboardBit::PageDown;
		case GLFW_KEY_HOME:				return KeyboardBit::Home;
		case GLFW_KEY_END:				return KeyboardBit::End;
		case GLFW_KEY_CAPS_LOCK:		return KeyboardBit::CapsLock;
		case GLFW_KEY_SCROLL_LOCK:		return KeyboardBit::ScrollLock;
		case GLFW_KEY_NUM_LOCK:			return KeyboardBit::NumLock;
		case GLFW_KEY_PRINT_SCREEN:		return KeyboardBit::PrintScreen;
		case GLFW_KEY_PAUSE:			return KeyboardBit::Pause;
		case GLFW_KEY_F1:				return KeyboardBit::F1;
		case GLFW_KEY_F2:				return KeyboardBit::F2;
		case GLFW_KEY_F3:				return KeyboardBit::F3;
		case GLFW_KEY_F4:				return KeyboardBit::F4;
		case GLFW_KEY_F5:				return KeyboardBit::F5;
		case GLFW_KEY_F6:				return KeyboardBit::F6;
		case GLFW_KEY_F7:				return KeyboardBit::F7;
		case GLFW_KEY_F8:				return KeyboardBit::F8;
		case GLFW_KEY_F9:				return KeyboardBit::F9;
		case GLFW_KEY_F10:				return KeyboardBit::F10;
		case GLFW_KEY_F11:				return KeyboardBit::F11;
		case GLFW_KEY_F12:				return KeyboardBit::F12;
		case GLFW_KEY_KP_0:				return KeyboardBit::KP0;
		case GLFW_KEY_KP_1:				return KeyboardBit::KP1;
		case GLFW_KEY_KP_2:				return KeyboardBit::KP2;
		case GLFW_KEY_KP_3:				return KeyboardBit::KP3;
		case GLFW_KEY_KP_4:				return KeyboardBit::KP4;
		case GLFW_KEY_KP_5:				return KeyboardBit::KP5;
		case GLFW_KEY_KP_6:				return KeyboardBit::KP6;
		case GLFW_KEY_KP_7:				return KeyboardBit::KP7;
		case GLFW_KEY_KP_8:				return KeyboardBit::KP8;
		case GLFW_KEY_KP_9:				return KeyboardBit::KP9;
		case GLFW_KEY_KP_DECIMAL:		return KeyboardBit::KPDecimal;
		case GLFW_KEY_KP_DIVIDE:		return KeyboardBit::KPDivide;
		case GLFW_KEY_KP_MULTIPLY:		return KeyboardBit::KPMultiply;
		case GLFW_KEY_KP_SUBTRACT:		return KeyboardBit::KPSubtract;
		case GLFW_KEY_KP_ADD:			return KeyboardBit::KPAdd;
		case GLFW_KEY_KP_ENTER:			return KeyboardBit::KPEnter;
		case GLFW_KEY_KP_EQUAL:			return KeyboardBit::KPEqual;
		case GLFW_KEY_LEFT_SHIFT:		return KeyboardBit::LeftShift;
		case GLFW_KEY_LEFT_CONTROL:		return KeyboardBit::LeftControl;
		case GLFW_KEY_LEFT_ALT:			return KeyboardBit::LeftAlt;
		case GLFW_KEY_RIGHT_SHIFT:		return KeyboardBit::RightShift;
		case GLFW_KEY_RIGHT_CONTROL:	return KeyboardBit::RightControl;
		case GLFW_KEY_RIGHT_ALT:		return KeyboardBit::RightAlt;
		default:
			SND_LOG_ERROR("Unknown keycode {}", keycode);
			return KeyboardBit::Count;
	}

}

snd::GamepadBit snd::input::ConvertGamepadCode(int32_t keycode)
{
	switch (keycode)
	{
		case GLFW_GAMEPAD_BUTTON_A:				return GamepadBit::FaceDown;
		case GLFW_GAMEPAD_BUTTON_B:				return GamepadBit::FaceRight;
		case GLFW_GAMEPAD_BUTTON_X:				return GamepadBit::FaceLeft;
		case GLFW_GAMEPAD_BUTTON_Y:				return GamepadBit::FaceUp;
		case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:	return GamepadBit::BumperLeft;
		case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:	return GamepadBit::BumperRight;
		case GLFW_GAMEPAD_BUTTON_BACK:			return GamepadBit::SpecialLeft;
		case GLFW_GAMEPAD_BUTTON_START:			return GamepadBit::SpecialRight;
		//case GLFW_GAMEPAD_BUTTON_GUIDE:			return GamepadBit::Count;
		case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:	return GamepadBit::ThumbLeft;
		case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:	return GamepadBit::ThumbRight;
		case GLFW_GAMEPAD_BUTTON_DPAD_UP:		return GamepadBit::DpadUp;
		case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:	return GamepadBit::DpadRight;
		case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:		return GamepadBit::DpadDown;
		case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:		return GamepadBit::DpadLeft;
		default:
			SND_LOG_ERROR("Unknown keycode {}", keycode);
			return GamepadBit::Count;
	}
}

snd::MouseBit snd::input::ConvertMouseCode(int32_t keycode)
{
	switch (keycode)
	{
		case GLFW_MOUSE_BUTTON_LEFT:	return MouseBit::Left;
		case GLFW_MOUSE_BUTTON_RIGHT:	return MouseBit::Right;
		case GLFW_MOUSE_BUTTON_MIDDLE:	return MouseBit::Middle;
		default:
			SND_LOG_ERROR("Unknown keycode {}", keycode);
			return MouseBit::Count;
	}
}

int32_t snd::input::ConvertBit(KeyboardBit bit)
{
	switch (bit)
	{
		case KeyboardBit::Space:		return GLFW_KEY_SPACE;
		case KeyboardBit::Apostrophe:	return GLFW_KEY_APOSTROPHE;
		case KeyboardBit::Comma:		return GLFW_KEY_COMMA;
		case KeyboardBit::Minus:		return GLFW_KEY_MINUS;
		case KeyboardBit::Period:		return GLFW_KEY_PERIOD;
		case KeyboardBit::Slash:		return GLFW_KEY_SLASH;
		case KeyboardBit::_0:			return GLFW_KEY_0;
		case KeyboardBit::_1:			return GLFW_KEY_1;
		case KeyboardBit::_2:			return GLFW_KEY_2;
		case KeyboardBit::_3:			return GLFW_KEY_3;
		case KeyboardBit::_4:			return GLFW_KEY_4;
		case KeyboardBit::_5:			return GLFW_KEY_5;
		case KeyboardBit::_6:			return GLFW_KEY_6;
		case KeyboardBit::_7:			return GLFW_KEY_7;
		case KeyboardBit::_8:			return GLFW_KEY_8;
		case KeyboardBit::_9:			return GLFW_KEY_9;
		case KeyboardBit::Semicolon:	return GLFW_KEY_SEMICOLON;
		case KeyboardBit::Equal:		return GLFW_KEY_EQUAL;
		case KeyboardBit::A:			return GLFW_KEY_A;
		case KeyboardBit::B:			return GLFW_KEY_B;
		case KeyboardBit::C:			return GLFW_KEY_C;
		case KeyboardBit::D:			return GLFW_KEY_D;
		case KeyboardBit::E:			return GLFW_KEY_E;
		case KeyboardBit::F:			return GLFW_KEY_F;
		case KeyboardBit::G:			return GLFW_KEY_G;
		case KeyboardBit::H:			return GLFW_KEY_H;
		case KeyboardBit::I:			return GLFW_KEY_I;
		case KeyboardBit::J:			return GLFW_KEY_J;
		case KeyboardBit::K:			return GLFW_KEY_K;
		case KeyboardBit::L:			return GLFW_KEY_L;
		case KeyboardBit::M:			return GLFW_KEY_M;
		case KeyboardBit::N:			return GLFW_KEY_N;
		case KeyboardBit::O:			return GLFW_KEY_O;
		case KeyboardBit::P:			return GLFW_KEY_P;
		case KeyboardBit::Q:			return GLFW_KEY_Q;
		case KeyboardBit::R:			return GLFW_KEY_R;
		case KeyboardBit::S:			return GLFW_KEY_S;
		case KeyboardBit::T:			return GLFW_KEY_T;
		case KeyboardBit::U:			return GLFW_KEY_U;
		case KeyboardBit::V:			return GLFW_KEY_V;
		case KeyboardBit::W:			return GLFW_KEY_W;
		case KeyboardBit::X:			return GLFW_KEY_X;
		case KeyboardBit::Y:			return GLFW_KEY_Y;
		case KeyboardBit::Z:			return GLFW_KEY_Z;
		case KeyboardBit::LeftBracket:	return GLFW_KEY_LEFT_BRACKET;
		case KeyboardBit::Backslash:	return GLFW_KEY_BACKSLASH;
		case KeyboardBit::RightBracket:	return GLFW_KEY_RIGHT_BRACKET;
		case KeyboardBit::GraveAccent:	return GLFW_KEY_GRAVE_ACCENT;
		case KeyboardBit::Escape:		return GLFW_KEY_ESCAPE;
		case KeyboardBit::Enter:		return GLFW_KEY_ENTER;
		case KeyboardBit::Tab:			return GLFW_KEY_TAB;
		case KeyboardBit::Backspace:	return GLFW_KEY_BACKSPACE;
		case KeyboardBit::Insert:		return GLFW_KEY_INSERT;
		case KeyboardBit::Delete:		return GLFW_KEY_DELETE;
		case KeyboardBit::Right:		return GLFW_KEY_RIGHT;
		case KeyboardBit::Left:			return GLFW_KEY_LEFT;
		case KeyboardBit::Down:			return GLFW_KEY_DOWN;
		case KeyboardBit::Up:			return GLFW_KEY_UP;
		case KeyboardBit::PageUp:		return GLFW_KEY_PAGE_UP;
		case KeyboardBit::PageDown:		return GLFW_KEY_PAGE_DOWN;
		case KeyboardBit::Home:			return GLFW_KEY_HOME;
		case KeyboardBit::End:			return GLFW_KEY_END;
		case KeyboardBit::CapsLock:		return GLFW_KEY_CAPS_LOCK;
		case KeyboardBit::ScrollLock:	return GLFW_KEY_SCROLL_LOCK;
		case KeyboardBit::NumLock:		return GLFW_KEY_NUM_LOCK;
		case KeyboardBit::PrintScreen:	return GLFW_KEY_PRINT_SCREEN;
		case KeyboardBit::Pause:		return GLFW_KEY_PAUSE;
		case KeyboardBit::F1:			return GLFW_KEY_F1;
		case KeyboardBit::F2:			return GLFW_KEY_F2;
		case KeyboardBit::F3:			return GLFW_KEY_F3;
		case KeyboardBit::F4:			return GLFW_KEY_F4;
		case KeyboardBit::F5:			return GLFW_KEY_F5;
		case KeyboardBit::F6:			return GLFW_KEY_F6;
		case KeyboardBit::F7:			return GLFW_KEY_F7;
		case KeyboardBit::F8:			return GLFW_KEY_F8;
		case KeyboardBit::F9:			return GLFW_KEY_F9;
		case KeyboardBit::F10:			return GLFW_KEY_F10;
		case KeyboardBit::F11:			return GLFW_KEY_F11;
		case KeyboardBit::F12:			return GLFW_KEY_F12;
		case KeyboardBit::KP0:			return GLFW_KEY_KP_0;
		case KeyboardBit::KP1:			return GLFW_KEY_KP_1;
		case KeyboardBit::KP2:			return GLFW_KEY_KP_2;
		case KeyboardBit::KP3:			return GLFW_KEY_KP_3;
		case KeyboardBit::KP4:			return GLFW_KEY_KP_4;
		case KeyboardBit::KP5:			return GLFW_KEY_KP_5;
		case KeyboardBit::KP6:			return GLFW_KEY_KP_6;
		case KeyboardBit::KP7:			return GLFW_KEY_KP_7;
		case KeyboardBit::KP8:			return GLFW_KEY_KP_8;
		case KeyboardBit::KP9:			return GLFW_KEY_KP_9;
		case KeyboardBit::KPDecimal:	return GLFW_KEY_KP_DECIMAL;
		case KeyboardBit::KPDivide:		return GLFW_KEY_KP_DIVIDE;
		case KeyboardBit::KPMultiply:	return GLFW_KEY_KP_MULTIPLY;
		case KeyboardBit::KPSubtract:	return GLFW_KEY_KP_SUBTRACT;
		case KeyboardBit::KPAdd:		return GLFW_KEY_KP_ADD;
		case KeyboardBit::KPEnter:		return GLFW_KEY_KP_ENTER;
		case KeyboardBit::KPEqual:		return GLFW_KEY_KP_EQUAL;
		case KeyboardBit::LeftShift:	return GLFW_KEY_LEFT_SHIFT;
		case KeyboardBit::LeftControl:	return GLFW_KEY_LEFT_CONTROL;
		case KeyboardBit::LeftAlt:		return GLFW_KEY_LEFT_ALT;
		case KeyboardBit::RightShift:	return GLFW_KEY_RIGHT_SHIFT;
		case KeyboardBit::RightControl:	return GLFW_KEY_RIGHT_CONTROL;
		case KeyboardBit::RightAlt:		return GLFW_KEY_RIGHT_ALT;
		default:
			SND_LOG_ERROR("Unknown keyboard bit {}", static_cast<int32_t>(bit));
			return -1;
	}
}

int32_t snd::input::ConvertBit(GamepadBit bit)
{
	switch (bit)
	{
		case GamepadBit::FaceDown:		return GLFW_GAMEPAD_BUTTON_A;
		case GamepadBit::FaceRight:		return GLFW_GAMEPAD_BUTTON_B;
		case GamepadBit::FaceLeft:		return GLFW_GAMEPAD_BUTTON_X;
		case GamepadBit::FaceUp:		return GLFW_GAMEPAD_BUTTON_Y;
		case GamepadBit::BumperLeft:	return GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
		case GamepadBit::BumperRight:	return GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
		case GamepadBit::SpecialLeft:	return GLFW_GAMEPAD_BUTTON_BACK;
		case GamepadBit::SpecialRight:	return GLFW_GAMEPAD_BUTTON_START;
		//case GamepadBit::Count:			return GLFW_GAMEPAD_BUTTON_GUIDE;
		case GamepadBit::ThumbLeft:		return GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
		case GamepadBit::ThumbRight:	return GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;
		case GamepadBit::DpadUp:	 	return GLFW_GAMEPAD_BUTTON_DPAD_UP;
		case GamepadBit::DpadRight:	 	return GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
		case GamepadBit::DpadDown:	 	return GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
		case GamepadBit::DpadLeft:	 	return GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
		default:
			SND_LOG_ERROR("Unknown gamepad bit {}", static_cast<int32_t>(bit));
			return -1;
	}
}

int32_t snd::input::ConvertBit(MouseBit bit)
{
	switch (bit)
	{
		case MouseBit::Left:	return GLFW_MOUSE_BUTTON_LEFT;
		case MouseBit::Right:	return GLFW_MOUSE_BUTTON_RIGHT;
		case MouseBit::Middle:	return GLFW_MOUSE_BUTTON_MIDDLE;
		default:
			SND_LOG_ERROR("Unknown mouse bit {}", static_cast<int32_t>(bit));
			return -1;
	}
}
