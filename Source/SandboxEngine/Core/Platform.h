#pragma once

#if defined(SND_PLATFORM_WIN32)
    #define SND_PLATFORM_WINDOW_WIN32
#endif

namespace snd
{
    struct Platform
    {
        s16 Keycodes[INPUT_KEY_LAST + 1];
        s16 Scancodes[512];
    };

    inline Platform gPlatform;

    void InitPlatform();
}
