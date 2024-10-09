#pragma once

namespace snd
{
    // Return array of floats as formatted string.
    // Example output: "(0.12, 3.45, 6.78)"
    const char* FloatArrayToString(const f32* arr, u16 size, s8 precision = 2);
}
