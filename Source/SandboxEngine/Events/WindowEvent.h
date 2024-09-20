#pragma once

#include "SandboxEngine/Events/Event.h"

namespace snd
{
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(u32 width, u32 height)
			: m_Width(width), m_Height(height) {}

		inline u32 GetWidth() const { return m_Width; }
		inline u32 GetHeight() const { return m_Height; }

		inline std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << "x" << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResized);
		EVENT_CLASS_CATEGORY(u32(EventCategory::Window));

	private:
		u32 m_Width; 
		u32 m_Height;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		EVENT_CLASS_TYPE(WindowClosed);
		EVENT_CLASS_CATEGORY(u32(EventCategory::Window));
	};
}
