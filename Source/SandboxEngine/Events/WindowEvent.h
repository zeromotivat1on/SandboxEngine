#pragma once

#include "SandboxEngine/Events/Event.h"

namespace snd
{
	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		inline std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << "x" << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResized);
		EVENT_CLASS_CATEGORY(uint32_t(EventCategory::Window));

	private:
		uint32_t m_Width; 
		uint32_t m_Height;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		EVENT_CLASS_TYPE(WindowClosed);
		EVENT_CLASS_CATEGORY(uint32_t(EventCategory::Window));
	};
}
