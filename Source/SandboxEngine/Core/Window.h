#pragma once

#include "SandboxEngine/Core/CoreMacros.h"
#include "SandboxEngine/Events/Event.h"
#include <glm/glm.hpp>

namespace snd
{
	class Window
	{
	public:
		using EventCallback			= std::function<void(Event&)>;

		struct Props
		{
			std::string				Title;
			u16						Width;
			u16						Height;
		};

	public:
		static Window*				Create(const Props& props);
		
		virtual						~Window() = default;

		virtual void* 				GetHandle() const = 0;
		virtual void* 				GetNativeHandle() const = 0;

		inline virtual u16			GetWidth() const = 0;
		inline virtual u16			GetHeight() const = 0;
		inline virtual const char*	GetTitle() const = 0;
		
		virtual void				SetEventCallback(const EventCallback& callback) = 0;

		virtual bool 				IsVsync() const = 0;
		virtual void 				SetVsync(bool enable) = 0;

		virtual bool				ShouldClose() const = 0;

		virtual void				Update() = 0;

	public:
		f32							GetAspectRatio() const;

		// Get [left, right, bottom, top] orthographic data with origin at window center.
		glm::vec4					GetOrthoDataCentered() const;

		// Get [left, right, bottom, top] orthographic data with direct origin at window top-left.
		glm::vec4					GetOrthoDataDirect() const;
	};

	SND_INLINE f32 Window::GetAspectRatio() const
	{ 
		return static_cast<f32>(GetWidth()) / static_cast<f32>(GetHeight());
	}

	SND_INLINE glm::vec4 Window::GetOrthoDataCentered() const
	{ 
		const f32 halfWidth  = static_cast<f32>(GetWidth())  * 0.5f;
		const f32 halfHeight = static_cast<f32>(GetHeight()) * 0.5f;

		const f32 left		 = -halfWidth;
		const f32 right		 = halfWidth;
		const f32 bottom	 = -halfHeight;
		const f32 top		 = halfHeight;

		return glm::vec4(left, right, bottom, top); 
	}

	SND_INLINE glm::vec4 Window::GetOrthoDataDirect() const
	{
		const f32 left   = 0.0f;
		const f32 right  = GetWidth();
		const f32 bottom = GetHeight();
		const f32 top    = 0.0f;

		return glm::vec4(left, right, bottom, top);
	}
}
