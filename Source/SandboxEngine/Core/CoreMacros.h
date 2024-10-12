#pragma once

// Core macros

#define SND_EXPAND_MACRO(x)     x
#define SND_STRINGIFY_MACRO(x)  #x

#define GLUE_INNER(a, b)        a ## b
#define GLUE(a, b)              GLUE_INNER(a, b)

// Bitmask

#define BIT(x)                  (1 << (x))                          // create bit shifted value
#define BIT_MASK(pos, n)        (((1U << (n)) - 1) << (pos))        // create a mask with 'n' bits set starting from a position
#define SET_BIT(value, pos)     ((value) |= (1U << (pos)))			// set a bit at a specific position
#define CLEAR_BIT(value, pos)   ((value) &= ~(1U << (pos)))		    // clear a bit at a specific position
#define TOGGLE_BIT(value, pos)  ((value) ^= (1U << (pos)))		    // toggle a bit at a specific position
#define CHECK_BIT(value, pos)   (((value) & (1U << (pos))) != 0)    // check if a bit is set at a specific position

// Miscellaneous

#define INVALID_INDEX           (-1)
#define INVALID_UINDEX          (-1u)

#define ARRAY_COUNT(arr)        sizeof(arr) / sizeof(arr[0])

#define SND_INLINE              __forceinline

#define SND_BIND_EVENT_FN(fn)   [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// Memory

#define KB(n)   n * 1000
#define MB(n)   n * 1000 * 1000
#define GB(n)   n * 1000 * 1000 * 1000

// Constructors and operators

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
