#pragma once

#include "SandboxEngine/Core/CoreMacros.h"

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::type; }				\
								virtual EventType GetEventType() const override { return GetStaticType(); }	\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual uint32_t GetCategoryFlags() const override { return category; }

namespace snd
{
	enum class EventType : uint32_t
	{
		None = 0,
		WindowClosed, WindowResized, WindowFocused, WindowFocusLost, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseKeyPressed, MouseKeyReleased, MouseMoved, MouseScrolled
	};

	enum class EventCategory : uint32_t
	{
		None		= 0,
		Window		= BIT(0),
		Input		= BIT(1),
		Keyboard	= BIT(2),
		Mouse		= BIT(3),
		MouseButton = BIT(4)
	};

	// For now events act in a blocking manner - dispatch and process immediately.
	// TODO: add event queue to process all events at specific part of frame.

	class Event
	{
		friend class EventDispatcher;

	public:
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual uint32_t GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsHandled() const { return m_Handled; }
		inline bool IsInCategory(EventCategory category) const { return GetCategoryFlags() & uint32_t(category); }

	private:
		bool m_Handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
