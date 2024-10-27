#pragma once

namespace snd
{
    inline u8               gWindowCount;
    inline struct Window*   gWindow;

    struct Window
    {
        // Platform specific.
        void* Handle;
        void* NativeHandle;

        // Window data.
        const char* Title;
		u16         Width;
        u16         Height;

        // Input state.
        s8          Buttons[INPUT_BUTTON_COUNT];        // general state
        s8          ButtonsUp[INPUT_BUTTON_COUNT];      // released this frame
        s8          ButtonsDown[INPUT_BUTTON_COUNT];    // pressed this frame
        f32         Axes[INPUT_AXIS_COUNT];

        void Update();
        void ShouldClose(bool close);
        void EnableCursor(bool enable);

        bool ShouldClose() const;

        vec2 MousePos();
        vec2 MouseOffset();
        vec2 ScrollOffset();

        // Get [left, right, bottom, top] orthographic data with origin at window center.
        vec4 OrthoDataCentered() const;
        f32 AspectRatio() const;
    };

    Window* OpenWindow(const char* title, u16 width, u16 height);
    void CloseWindow(Window* wnd);

    // Window

    SND_INLINE vec2 Window::MousePos()
    {
        return vec2(Axes[INPUT_MOUSE_X], Axes[INPUT_MOUSE_Y]);
    }

    SND_INLINE vec2 Window::MouseOffset()
    {
        return vec2(Axes[INPUT_MOUSE_OFFSET_X], Axes[INPUT_MOUSE_OFFSET_Y]);
    }

    SND_INLINE vec2 Window::ScrollOffset()
    {
        return vec2(Axes[INPUT_MOUSE_SCROLL_X], Axes[INPUT_MOUSE_SCROLL_Y]);
    }

    SND_INLINE vec4 Window::OrthoDataCentered() const
    {
    	const f32 halfWidth  = (f32)Width * 0.5f;
    	const f32 halfHeight = (f32)Height * 0.5f;

        const f32 left	    = -halfWidth;
        const f32 right	    =  halfWidth;
        const f32 bottom    = -halfHeight;
        const f32 top       =  halfHeight;

    	return vec4(left, right, bottom, top);
    }

    SND_INLINE f32 Window::AspectRatio() const
    {
        return (f32)Width / (f32)Height;
    }
}

