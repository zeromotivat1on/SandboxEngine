#pragma once

#ifdef SND_BUILD_DLL
	#define SND_API __declspec(dllexport)
#else
	#define SND_API __declspec(dllimport)
#endif

#define SND_EXPAND_MACRO(x) x
#define SND_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << (x))

#define GLUE_INNER(a, b) a ## b
#define GLUE(a, b) GLUE_INNER(a, b)

#define SND_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include "SandboxEngine/Core/Log.h"
#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/Core/Timer.h"
#include "SandboxEngine/Core/Profile.h"
