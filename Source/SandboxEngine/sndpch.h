#pragma once

// Intrinsics
#include <intrin.h>

// STD
#include <chrono>
#include <algorithm>
#include <filesystem>

// STL
#include <string>
#include <bitset>
#include <vector>
#include <sstream>
#include <unordered_map>

// CSTD
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <stdarg.h>

// Fundamental types
using int8  = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using s8  = int8;
using s16 = int16;
using s32 = int32;
using s64 = int64;

using u8  = uint8;
using u16 = uint16;
using u32 = uint32;
using u64 = uint64;

using f32       = float;
using float32   = float;

using f64       = double;
using float64   = double;

// Common macros
#define SND_EXPAND_MACRO(x)     x
#define SND_STRINGIFY_MACRO(x)  #x

#define SND_CORE_SUCCESS        0
#define SND_CORE_FAILURE        1

#define GLUE_INNER(a, b)        a ## b
#define GLUE(a, b)              GLUE_INNER(a, b)

#define BIT(x)                  (1 << (x))                          // create bit shifted value
#define BIT_MASK(pos, n)        (((1U << (n)) - 1) << (pos))        // create a mask with 'n' bits set starting from a position
#define SET_BIT(value, pos)     ((value) |= (1U << (pos)))			// set a bit at a specific position
#define CLEAR_BIT(value, pos)   ((value) &= ~(1U << (pos)))		    // clear a bit at a specific position
#define TOGGLE_BIT(value, pos)  ((value) ^= (1U << (pos)))		    // toggle a bit at a specific position
#define CHECK_BIT(value, pos)   (((value) & (1U << (pos))) != 0)    // check if a bit is set at a specific position

#define INVALID_INDEX           (-1)
#define INVALID_UINDEX          (-1u)

#define ARRAY_COUNT(arr)        (sizeof(arr) / sizeof(arr[0]))

#define SND_INLINE              __forceinline

#define KB(n)                   (n * 1024ULL)
#define MB(n)                   (KB(n) * 1024ULL)
#define GB(n)                   (MB(n) * 1024ULL)
#define TB(n)                   (GB(n) * 1024ULL)

#define FRAMERATE(n)            (1.0f / n)

#define NO_COPY_CTOR(T)         T(const T&) = delete
#define NO_MOVE_CTOR(T)         T(T&&) noexcept = delete
#define NO_COPY_OP(T)           T& operator=(const T&) = delete
#define NO_MOVE_OP(T)           T& operator=(T&&) noexcept = delete

#define NOT_COPYABLE(T)         NO_COPY_CTOR(T); NO_COPY_OP(T)
#define NOT_MOVABLE(T)          NO_MOVE_CTOR(T); NO_MOVE_OP(T)

#define DEF_COPY_CTOR(T)        T(const T&) = default
#define DEF_MOVE_CTOR(T)        T(T&&) noexcept = default
#define DEF_COPY_OP(T)          T& operator=(const T&) = default
#define DEF_MOVE_OP(T)          T& operator=(T&&) noexcept = default

#define DEF_COPYABLE(T)         DEF_COPY_CTOR(T); DEF_COPY_OP(T)
#define DEF_MOVABLE(T)          DEF_MOVE_CTOR(T); DEF_MOVE_OP(T)

// Core
#include "SandboxEngine/Core/Log.h"
#include "SandboxEngine/Core/Error.h"
#include "SandboxEngine/Core/Time.h"
#include "SandboxEngine/Core/String.h"
#include "SandboxEngine/Core/Delegate.h"
#include "SandboxEngine/Core/SID.h"
#include "SandboxEngine/Core/UID.h"

// Math
#include "SandboxEngine/Math/Math.h"
#include "SandboxEngine/Math/Euler.h"
#include "SandboxEngine/Math/Vector.h"
#include "SandboxEngine/Math/Matrix.h"
#include "SandboxEngine/Math/Quat.h"

// Memory
#include "SandboxEngine/Memory/Memory.h"
#include "SandboxEngine/Memory/Buffer.h"
#include "SandboxEngine/Memory/SparseBuffer.h"

// Threading
#include "SandboxEngine/Threading/Thread.h"
#include "SandboxEngine/Threading/WorkQueue.h"

// Core extra
#include "SandboxEngine/Core/Input.h"
#include "SandboxEngine/Core/Window.h"

#ifdef SND_PLATFORM_WINDOWS
	#define NOMINMAX // disable windows min/max macro definitions to avoid possible name conflicts
	#include <Windows.h>
#endif
