#include "sndpch.h"
#include "SandboxEngine/Core/Window.h"
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Render/Render.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

static void DetectButtonState(const s8 prevState, const s8 currState, s8& outUp, s8& outDown)
{
    const s8 changed = currState ^ prevState;
    outUp = changed & (~currState);
    outDown = changed & currState;
}

static void GLFWErrorCallback(int error, const char* description)
{
	SND_ERROR("GLFW (%d): %s", error, description);
}

static void GLFWCloseCallback(GLFWwindow* window)
{
    snd::Window* wnd = (snd::Window*)glfwGetWindowUserPointer(window);
    SND_LOG("GLFW window (%s) close callback", wnd->Title);
}

static void GLFWSizeCallback(GLFWwindow* window, int width, int height)
{
    snd::Window* wnd = (snd::Window*)glfwGetWindowUserPointer(window);
    wnd->Width = width;
    wnd->Height = height;
    snd::gRenderer->Reset(width, height);
}

static void GLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    snd::Window* wnd = (snd::Window*)glfwGetWindowUserPointer(window);
    wnd->Axes[snd::INPUT_MOUSE_SCROLL_X] = (f32)xoffset;
    wnd->Axes[snd::INPUT_MOUSE_SCROLL_Y] = (f32)yoffset;
}

snd::Window* snd::OpenWindow(const char* title, u16 width, u16 height)
{
    if (gWindowCount == 0)
    {
        const s32 success = glfwInit();
		SND_ASSERT(success);
		glfwSetErrorCallback(GLFWErrorCallback);
    }

    Window* wnd = (Window*)gPersistentStack.PushZero(sizeof(Window));
    memset(wnd, 0, sizeof(Window));

    wnd->Handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    SND_ASSERT(wnd->Handle);

    wnd->NativeHandle = glfwGetWin32Window((GLFWwindow*)wnd->Handle);
    SND_ASSERT(wnd->NativeHandle);

    glfwSetWindowUserPointer((GLFWwindow*)wnd->Handle, wnd);
    glfwMakeContextCurrent((GLFWwindow*)wnd->Handle);
	glfwSetWindowCloseCallback((GLFWwindow*)wnd->Handle, GLFWCloseCallback);
	glfwSetWindowSizeCallback((GLFWwindow*)wnd->Handle, GLFWSizeCallback);

	glfwSetScrollCallback((GLFWwindow*)wnd->Handle, GLFWScrollCallback);

    wnd->Title = title;
    wnd->Width = width;
    wnd->Height = height;

    gWindowCount++;

    return wnd;
}

void snd::CloseWindow(Window* wnd)
{
    SND_ASSERT(wnd);
    glfwDestroyWindow((GLFWwindow*)wnd->Handle);
    gWindowCount--;

    if (gWindowCount == 0)
    {
        glfwTerminate();
    }
}

void snd::Window::Update()
{
    static f32 PrevMouseX, PrevMouseY;

    Axes[INPUT_MOUSE_SCROLL_X] = 0.0f;
    Axes[INPUT_MOUSE_SCROLL_Y] = 0.0f;

	PrevMouseX = Axes[INPUT_MOUSE_X];
    PrevMouseY = Axes[INPUT_MOUSE_Y];

    glfwPollEvents();

    f64 xmouse, ymouse;
    glfwGetCursorPos((GLFWwindow*)Handle, &xmouse, &ymouse);

    Axes[INPUT_MOUSE_X] = (f32)xmouse;
    Axes[INPUT_MOUSE_Y] = (f32)ymouse;

    Axes[INPUT_MOUSE_OFFSET_X] = Axes[INPUT_MOUSE_X] - PrevMouseX;
    Axes[INPUT_MOUSE_OFFSET_Y] = PrevMouseY - Axes[INPUT_MOUSE_Y]; // y-axis is reversed since it goes from top to bottom

    for (s32 i = INPUT_KEY_FIRST; i <= INPUT_KEY_LAST; ++i)
    {
        const s8 prev = Buttons[i];
        Buttons[i] = glfwGetKey((GLFWwindow*)Handle, gButtonMap[i]);
        DetectButtonState(prev, Buttons[i], ButtonsUp[i], ButtonsDown[i]);
    }

    for (s32 i = INPUT_MOUSE_FIRST; i <= INPUT_MOUSE_LAST; ++i)
    {
        const s8 prev = Buttons[i];
        Buttons[i] = glfwGetMouseButton((GLFWwindow*)Handle, gButtonMap[i]);
        DetectButtonState(prev, Buttons[i], ButtonsUp[i], ButtonsDown[i]);
    }

    GLFWgamepadstate gamepadState;
	glfwGetGamepadState(0, &gamepadState); // TODO: make several gamepads available

    for (s32 i = INPUT_GAMEPAD_FIRST; i <= INPUT_GAMEPAD_LAST; ++i)
    {
        const s8 prev = Buttons[i];
        Buttons[i] = gamepadState.buttons[gButtonMap[i]];
        DetectButtonState(prev, Buttons[i], ButtonsUp[i], ButtonsDown[i]);
    }

    for (s32 i = INPUT_GAMEPAD_AXIS_FIRST; i <= INPUT_GAMEPAD_AXIS_LAST; ++i)
    {
        Axes[i] = gamepadState.axes[gAxisMap[i]];
    }
}

void snd::Window::ShouldClose(bool close)
{
    glfwSetWindowShouldClose((GLFWwindow*)Handle, close);
}

bool snd::Window::ShouldClose() const
{
    return glfwWindowShouldClose((GLFWwindow*)Handle);
}

void snd::Window::EnableCursor(bool enable)
{
    if (enable) glfwSetInputMode((GLFWwindow*)Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else        glfwSetInputMode((GLFWwindow*)Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
