#pragma once

#ifdef SND_BUILD_DLL
	#define SND_API __declspec(dllexport)
#else
	#define SND_API __declspec(dllimport)
#endif

#define SND_EXPAND_MACRO(x) x
#define SND_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << (x))										// create bit shifted value
#define BIT_MASK(pos, n) (((1U << (n)) - 1) << (pos))			// create a mask with 'n' bits set starting from a position
#define SET_BIT(value, pos) ((value) |= (1U << (pos)))			// set a bit at a specific position
#define CLEAR_BIT(value, pos) ((value) &= ~(1U << (pos)))		// clear a bit at a specific position
#define TOGGLE_BIT(value, pos) ((value) ^= (1U << (pos)))		// toggle a bit at a specific position
#define CHECK_BIT(value, pos) (((value) & (1U << (pos))) != 0)	// check if a bit is set at a specific position

#define GLUE_INNER(a, b) a ## b
#define GLUE(a, b) GLUE_INNER(a, b)

#define SND_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include "SandboxEngine/Core/Log.h"
#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/Core/Dirty.h"
#include "SandboxEngine/Core/Timer.h"
#include "SandboxEngine/Core/Profile.h"
#include "SandboxEngine/Core/FileSystem.h"
