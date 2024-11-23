#pragma once

#ifdef WIN32
// Remove win32 min and max coz bgfx is upset with them.
#define NOMINMAX
#endif

// Intrinsics
#include <intrin.h>

// CSTD
#include <cstdlib>
#include <stdarg.h>

// GDL
#include <gdl.h>

// Core
#include "Engine/Core/Delegate.h"
#include "Engine/Core/File.h"
#include "Engine/Core/Profile.h"

// Memory
inline void* g_virt_space;
inline void* g_phys_heap;

inline Arena g_arena_persistent;
inline Arena g_arena_transient;
inline Arena g_arena_frame;

// Frame
inline u64 g_frame_counter;
