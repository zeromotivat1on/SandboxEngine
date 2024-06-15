#pragma once

#include "SandboxEngine/Events/Event.h"

namespace snd
{
	// From GLFW
	enum class KeyCode : uint16_t
	{
		Space			= 32,
		Apostrophe		= 39,	/* ' */
		Comma			= 44,	/* , */
		Minus			= 45,	/* - */
		Period			= 46,	/* . */
		Slash			= 47,	/* / */
		_0				= 48,
		_1				= 49,
		_2				= 50,
		_3				= 51,
		_4				= 52,
		_5				= 53,
		_6				= 54,
		_7				= 55,
		_8				= 56,
		_9				= 57,
		Semicolon		= 59,	/* ; */
		Equal			= 61,	/* = */
		A				= 65,
		B				= 66,
		C				= 67,
		D				= 68,
		E				= 69,
		F				= 70,
		G				= 71,
		H				= 72,
		I				= 73,
		J				= 74,
		K				= 75,
		L				= 76,
		M				= 77,
		N				= 78,
		O				= 79,
		P				= 80,
		Q				= 81,
		R				= 82,
		S				= 83,
		T				= 84,
		U				= 85,
		V				= 86,
		W				= 87,
		X				= 88,
		Y				= 89,
		Z				= 90,
		LeftBracket		= 91,  /* [ */
		Backslash		= 92,  /* \ */
		RightBracket	= 93,  /* ] */
		GraveAccent		= 96,  /* ` */
		Escape			= 256,
		Enter			= 257,
		Tab				= 258,
		Backspace		= 259,
		Insert			= 260,
		Delete			= 261,
		Right			= 262,
		Left			= 263,
		Down			= 264,
		Up				= 265,
		PageUp			= 266,
		PageDown		= 267,
		Home			= 268,
		End				= 269,
		CapsLock		= 280,
		ScrollLock		= 281,
		NumLock			= 282,
		PrintScreen		= 283,
		Pause			= 284,
		F1				= 290,
		F2				= 291,
		F3				= 292,
		F4				= 293,
		F5				= 294,
		F6				= 295,
		F7				= 296,
		F8				= 297,
		F9				= 298,
		F10				= 299,
		F11				= 300,
		F12				= 301,
		KP0				= 320,
		KP1				= 321,
		KP2				= 322,
		KP3				= 323,
		KP4				= 324,
		KP5				= 325,
		KP6				= 326,
		KP7				= 327,
		KP8				= 328,
		KP9				= 329,
		KPDecimal		= 330,
		KPDivide		= 331,
		KPMultiply		= 332,
		KPSubtract		= 333,
		KPAdd			= 334,
		KPEnter			= 335,
		KPEqual			= 336,
		LeftShift		= 340,
		LeftControl		= 341,
		LeftAlt			= 342,
		RightShift		= 344,
		RightControl	= 345,
		RightAlt		= 346,
	};

	class KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(uint32_t(EventCategory::Input) | uint32_t(EventCategory::Keyboard));
	
	protected:
		KeyEvent(KeyCode code) : m_KeyCode(code) {}

	protected:
		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode code, bool isRepeat)
			: KeyEvent(code), m_IsRepeat(isRepeat)
		{}

	public:
		inline bool IsRepeat() const { return m_IsRepeat; }

		EVENT_CLASS_TYPE(KeyPressed);

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << uint32_t(m_KeyCode) << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

	private:
		bool m_IsRepeat = false;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << uint32_t(m_KeyCode);
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << uint32_t(m_KeyCode);
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped);
	};
}
