#pragma once

#include "SandboxEngine/Core/CoreMacros.h"
#include "SandboxEngine/Events/Event.h"
#include <glm/glm.hpp>

namespace snd
{
	class Window
	{
	public:
		using EventCallbackFn		= std::function<void(Event&)>;

		struct Props
		{
			std::string				Title = "Sandbox Engine";
			uint32_t				Width = 1280;
			uint32_t				Height = 720;
		};

	public:
		static Window*				Create(const Props& props = {});

		virtual						~Window() = default;

		virtual void* 				GetHandle() const = 0;
		virtual void* 				GetNativeHandle() const = 0;

		inline virtual uint32_t		GetWidth() const = 0;
		inline virtual uint32_t		GetHeight() const = 0;
		inline virtual const char*	GetTitle() const = 0;
		
		virtual void				SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual bool 				IsVsync() const = 0;
		virtual void 				SetVsync(bool enable) = 0;

		virtual bool				ShouldClose() const = 0;

		virtual void				Update() = 0;

	public:
		float 						GetWidthF32() const;
		float 						GetHeightF32() const;

		float						GetAspectRatio() const;

		// Get [left, right, bottom, top] orthographic data with origin at window center.
		glm::vec4					GetOrthoDataCentered() const;

		// Get [left, right, bottom, top] orthographic data with direct origin at window top-left.
		glm::vec4					GetOrthoDataDirect() const;
	};

	SND_INLINE float Window::GetWidthF32() const
	{
		return static_cast<float>(GetWidth());
	}

	SND_INLINE float Window::GetHeightF32() const
	{
		return static_cast<float>(GetHeight());
	}

	SND_INLINE float Window::GetAspectRatio() const
	{ 
		return GetWidthF32() / GetHeightF32();
	}

	SND_INLINE glm::vec4 Window::GetOrthoDataCentered() const
	{ 
		const float halfWidth = GetWidthF32() * 0.5f;
		const float halfHeight = GetHeightF32() * 0.5f;

		const float left = -halfWidth;
		const float right = halfWidth;
		const float bottom = -halfHeight;
		const float top = halfHeight;

		return glm::vec4(left, right, bottom, top); 
	}

	SND_INLINE glm::vec4 Window::GetOrthoDataDirect() const
	{
		const float left = 0.0f;
		const float right = GetWidthF32();
		const float bottom = GetHeightF32();
		const float top = 0.0f;

		return glm::vec4(left, right, bottom, top);
	}
}
