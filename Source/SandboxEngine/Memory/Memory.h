#pragma once

namespace snd
{
    struct Memory
    {
        u8*     Data;
        u32     Size;
    };

    SND_INLINE bool Valid(const Memory& mem)
    {
        return mem.Data && mem.Size > 0;
    }

    SND_INLINE void MemoryRelease(void* data, void* userData)
    {
        SND_ASSERT(data);
        free(data);
    }
}
