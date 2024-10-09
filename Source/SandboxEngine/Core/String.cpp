#include "sndpch.h"
#include "SandboxEngine/Core/String.h"

const char* snd::FloatArrayToString(const f32* arr, u16 size, s8 precision)
{
    static s32 s_Index = 0;
    static char s_Buffer[4][16384];	// in case called by nested functions

    char  format[16];
    char* string = s_Buffer[s_Index];
    s_Index = (s_Index + 1) & 3;

    // First formatted element.
    sprintf_s(format, sizeof(format), "(%%.%df,", precision);
    s32 n = sprintf_s(string, sizeof(s_Buffer[0]), format, arr[0]);
    if (precision > 0)
    {
        while(n > 0 && string[n - 1] == '0') string[--n] = '\0';
        while(n > 0 && string[n - 1] == '.') string[--n] = '\0';
    }

    // Intermediate formatted elements.
    sprintf_s(format, sizeof(format), " %%.%df,", precision);
    for (s32 i = 1; i < size - 1; i++)
    {
        n += sprintf_s(string + n, sizeof(s_Buffer[0]) - n, format, arr[i]);
        if (precision > 0)
        {
            while(n > 0 && string[n - 1] == '0') string[--n] = '\0';
            while(n > 0 && string[n - 1] == '.') string[--n] = '\0';
        }
    }

    // Last formatted element.
    sprintf_s(format, sizeof(format), " %%.%df)", precision);
    n += sprintf_s(string + n, sizeof(s_Buffer[0]) - n, format, arr[size - 1]);
    if (precision > 0)
    {
        while(n > 0 && string[n - 1] == '0') string[--n] = '\0';
        while(n > 0 && string[n - 1] == '.') string[--n] = '\0';
    }
    
    return string;
}
