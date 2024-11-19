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
inline constexpr u64 ENGINE_BLOCK_SIZE = MB(16);
inline constexpr u64 CORE_STACK_SIZE = MB(2);
inline constexpr u64 FRAME_STACK_SIZE = MB(2);

inline constexpr u64 VIRT_SPACE_SIZE = GB(8);
inline constexpr u64 PERS_ARENA_SIZE = MB(16);
inline constexpr u64 TRAN_ARENA_SIZE = GB(1);
inline constexpr u64 FRAME_ARENA_SIZE = MB(4);
inline constexpr u64 PHYS_HEAP_SIZE = (PERS_ARENA_SIZE + TRAN_ARENA_SIZE + FRAME_ARENA_SIZE);

inline void* g_virt_space;
inline void* g_phys_heap;

inline Arena g_arena_persistent;
inline Arena g_arena_transient;
inline Arena g_arena_frame;

// Frame
inline u64 g_frame_counter;
