#pragma once

#define ENGINE_BLOCK_SIZE   MB(16)
#define CORE_STACK_SIZE     MB(2)
#define FRAME_STACK_SIZE    MB(2)

#define VM_CORE_ALLOC_SIZE      GB(8)
#define PERSISTENT_BLOCK_SIZE   MB(16)
#define FRAME_BLOCK_SIZE        MB(4)
#define TRANSIENT_BLOCK_SIZE    GB(1)
#define PHYS_CORE_ALLOC_SIZE    (PERSISTENT_BLOCK_SIZE + FRAME_BLOCK_SIZE + TRANSIENT_BLOCK_SIZE)

namespace snd
{
    struct OsInfo
    {
        u64 TotalPhys;
        u64 TotalVirt;
        u64 AvailPhys;
        u64 AvailVirt;
        u32 PageSize;
        u32 AllocGran;
        u32 LgCoreNum;
    };

    struct AppMemory
    {
        u64 PermSize;
        u64 TransSize;
        u8* PermData;
        u8* TransData;
    };

    struct mstack // memory stack
    {
        u8*     Data;
        u64     Size;
        u64     Pos;

        void*   Push(u64 bytes);
        void*   PushZero(u64 bytes);
        void    Pop(u64 bytes);
        void    Clear();
    };

    inline mstack gPersistentStack;
    inline mstack gFrameStack;
    inline mstack gTwoFrameStack;
    inline mstack gTransientStack;

    void GetOsInfo(OsInfo* info);
    void InitAppMemory(void* vm, u64 vmsize);
    void FreeAppMemory(void* vm);

    // Reserve virtual memory of a given size.
    // Returned memory and alloc size are aligned based on system granulairty.
    // Initial address can be specified, leave null to let os decide alloc address.
    void* vmreserve(void* addr, u64 size);

    // Allocate physical memory from given range of virtual addresses.
    void* vmcommit(void* vm, u64 size);

    // Release physical memory from given range of virtual addresses.
    bool vmdecommit(void* vm, u64 size);

    // Free both physical and virtual memory.
    bool vmrelease(void* vm);

    // mstack

    SND_INLINE void* mstack::Push(u64 bytes)
    {
        SND_ASSERT(Pos + bytes <= Size);
        void* data = Data + Pos;
        Pos += bytes;
        return data;
    }

    SND_INLINE void* mstack::PushZero(u64 bytes)
    {
        void* data = Push(bytes);
        memset(data, 0, bytes);
        return data;
    }

    SND_INLINE void mstack::Pop(u64 bytes)
    {
        SND_ASSERT(Pos >= bytes);
        Pos -= bytes;
    }

    SND_INLINE void mstack::Clear()
    {
        Pos = 0;
    }
}
