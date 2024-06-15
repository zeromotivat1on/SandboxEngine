#pragma once

#include "Core.h"
#include "SandboxEngine/Events/Event.h"

namespace snd
{
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		struct Props
		{
			std::string Title = "Sandbox Engine";
			uint32_t Width = 1280;
			uint32_t Height = 720;
		};

	public:
		static Window* Create(const Window::Props& props = {});

		virtual ~Window() = default;

		virtual void* GetHandle() const = 0;
		virtual void* GetNativeHandle() const = 0;

		inline virtual uint32_t GetWidth() const = 0;
		inline virtual uint32_t GetHeight() const = 0;
		inline virtual const char* GetTitle() const = 0;
		
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

		virtual bool IsVsync() const = 0;
		virtual void SetVsync(bool enable) = 0;

		virtual bool ShouldClose() const = 0;

		virtual void Tick(float dt) const = 0;
	};
}
