#pragma once

#if PLATFORM_WIN32
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
#include "Engine/Core/Error.h"
#include "Engine/Core/Delegate.h"

// Memory
#define ENGINE_BLOCK_SIZE   MB(16)
#define CORE_STACK_SIZE     MB(2)
#define FRAME_STACK_SIZE    MB(2)

#define VIRT_SPACE_SIZE     GB(8)
#define PERS_ARENA_SIZE     MB(16)
#define TRAN_ARENA_SIZE     GB(1)
#define FRAME_ARENA_SIZE    MB(4)
#define PHYS_HEAP_SIZE      (PERS_ARENA_SIZE + TRAN_ARENA_SIZE + FRAME_ARENA_SIZE)

inline void* g_virt_space;
inline void* g_phys_heap;

inline Arena g_arena_persistent;
inline Arena g_arena_transient;
inline Arena g_arena_frame;
