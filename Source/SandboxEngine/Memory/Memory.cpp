#include "sndpch.h"
#include "SandboxEngine/Memory/Memory.h"

void snd::InitAppMemory(void* vm, u64 vmsize)
{
    SND_ASSERT(vm);
    SND_ASSERT(vmsize > PHYS_CORE_ALLOC_SIZE);

    void* physcore = vmcommit(vm, PHYS_CORE_ALLOC_SIZE);
    SND_ASSERT(physcore);

    gPersistentStack.Data = (u8*)physcore;
    gPersistentStack.Size = PERSISTENT_BLOCK_SIZE;
    gPersistentStack.Pos = 0;

    gFrameStack.Data = gPersistentStack.Data + gPersistentStack.Size;
    gFrameStack.Size = FRAME_BLOCK_SIZE;
    gFrameStack.Pos = 0;

    gTransientStack.Data = gPersistentStack.Data + gPersistentStack.Size;
    gTransientStack.Size = TRANSIENT_BLOCK_SIZE;
    gTransientStack.Pos = 0;
}

void snd::FreeAppMemory(void* vm)
{
    SND_ASSERT(vm);
    const bool res = vmrelease(vm);
    if (!res)
    {
        SND_ERROR("Failed to release app memory");
    }
}
