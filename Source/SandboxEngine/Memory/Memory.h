#pragma once

namespace snd
{
    struct Memory
    {
                Memory();
                Memory(u8* data, u32 size);
        
        u8*     Data;
        u32     Size;

        bool    Valid() const;
    };

    SND_INLINE Memory::Memory()
        : Data(nullptr), Size(0)
    {
    }

    SND_INLINE Memory::Memory(u8* data, u32 size)
        : Data(data), Size(size)
    {
    }

    SND_INLINE bool Memory::Valid() const
    {
        return Data && Size > 0;
    }

    SND_INLINE void MemoryRelease(void* data, void* userData)
    {
        SND_ASSERT(data);
        free(data);
    }
}
