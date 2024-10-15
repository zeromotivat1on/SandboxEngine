#pragma once

#include "SandboxEngine/Events/Event.h"

namespace snd
{
	class Window
	{
	public:
        DECLARE_DELEGATE_Params(EventCallback, Event&);

		struct Props
		{
			const char*				Title;
			u16						Width;
			u16						Height;
		};

	public:
		static Window*				Create(const Props& props);

		virtual						~Window() = default;

		virtual void* 				Handle() const = 0;
		virtual void* 				NativeHandle() const = 0;

		virtual u16					Width() const = 0;
		virtual u16					Height() const = 0;
		virtual const char*			Title() const = 0;
		virtual bool 				Vsync() const = 0;
		virtual bool				Focused() const = 0;

		virtual void				SetEventCallback(const EventCallback& callback) = 0;
		virtual void 				SetVsync(bool enable) = 0;
		virtual bool				ShouldClose() const = 0;

		virtual void				Update() = 0;

	public:
		f32							AspectRatio() const;

		// Get [left, right, bottom, top] orthographic data with origin at window center.
		vec4						OrthoDataCentered() const;

		// Get [left, right, bottom, top] orthographic data with direct origin at window top-left.
		vec4						OrthoDataDirect() const;
	};

	SND_INLINE f32 Window::AspectRatio() const
	{
		return static_cast<f32>(Width()) / static_cast<f32>(Height());
	}

	SND_INLINE vec4 Window::OrthoDataCentered() const
	{
		const f32 halfWidth  = static_cast<f32>(Width())  * 0.5f;
		const f32 halfHeight = static_cast<f32>(Height()) * 0.5f;

		const f32 left		 = -halfWidth;
		const f32 right		 = halfWidth;
		const f32 bottom	 = -halfHeight;
		const f32 top		 = halfHeight;

		return vec4(left, right, bottom, top);
	}

	SND_INLINE vec4 Window::OrthoDataDirect() const
	{
		const f32 left   = 0.0f;
		const f32 right  = Width();
		const f32 bottom = Height();
		const f32 top    = 0.0f;

		return vec4(left, right, bottom, top);
	}
}
