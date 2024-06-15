#pragma once

#include "SandboxEngine/Events/Event.h"

namespace snd
{
	// From GLFW
	enum class MouseCode : uint8_t
	{
		Button0			= 0,
		Button1			= 1,
		Button2			= 2,
		Button3			= 3,
		Button4			= 4,
		Button5			= 5,
		Button6			= 6,
		Button7			= 7,
		ButtonLast		= Button7,
		ButtonLeft		= Button0,
		ButtonRight		= Button1,
		ButtonMiddle	= Button2
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: m_PosX(x), m_PosY(y) {}

		inline float GetX() const { return m_PosX; }
		inline float GetY() const { return m_PosY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << m_PosX << ", " << m_PosY << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved);
		EVENT_CLASS_CATEGORY(uint32_t(EventCategory::Input) | uint32_t(EventCategory::Mouse));

	private:
		float m_PosX = 0.0f;
		float m_PosY = 0.0f;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_OffsetX(xOffset), m_OffsetY(yOffset) {}

		inline float GetOffsetX() const { return m_OffsetX; }
		inline float GetOffsetY() const { return m_OffsetY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << GetOffsetX() << ", " << GetOffsetY() << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(uint32_t(EventCategory::Input) | uint32_t(EventCategory::Mouse));

	private:
		float m_OffsetX = 0.0f; 
		float m_OffsetY = 0.0f;
	};

	class MouseKeyEvent : public Event
	{
	public:
		inline MouseCode GetMouseCode() const { return m_MouseCode; }

		EVENT_CLASS_CATEGORY(uint32_t(EventCategory::Input) | uint32_t(EventCategory::Mouse) | uint32_t(EventCategory::MouseButton));

	protected:
		MouseKeyEvent(MouseCode mouseCode) : m_MouseCode(mouseCode) {}

	protected:
		MouseCode m_MouseCode;
	};

	class MouseKeyPressedEvent : public MouseKeyEvent
	{
	public:
		MouseKeyPressedEvent(const MouseCode button)
			: MouseKeyEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << uint32_t(m_MouseCode);
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseKeyPressed);
	};

	class MouseKeyReleasedEvent : public MouseKeyEvent
	{
	public:
		MouseKeyReleasedEvent(const MouseCode button)
			: MouseKeyEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << uint32_t(m_MouseCode);
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseKeyReleased);
	};
}
