#pragma once

#include "sndpch.h"
#include "Core.h"

namespace snd
{
	class SND_API Window
	{
	public:
		struct Props
		{
			std::string Title = "Sandbox Engine";
			uint32_t Width = 1280;
			uint32_t Height = 720;
		};

	public:
		static Window* Create(const Window::Props& props = {});

		virtual ~Window() = default;

		virtual void* GetNativeHandle() const = 0;

		inline virtual uint32_t GetWidth() const = 0;
		inline virtual uint32_t GetHeight() const = 0;
		
		virtual bool IsVsync() const = 0;
		virtual void SetVsync(bool enable) = 0;

		virtual bool ShouldClose() const = 0;

		virtual void OnUpdate() const = 0;
	};
}
