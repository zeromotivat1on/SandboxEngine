#pragma once

#define ENGINE_BLOCK_SIZE   MB(16)
#define CORE_STACK_SIZE     MB(2)
#define FRAME_STACK_SIZE    MB(2)

#define VIRT_SPACE_SIZE     GB(8)
#define PERS_ARENA_SIZE     MB(16)
#define TRAN_ARENA_SIZE     GB(1)
#define FRAME_ARENA_SIZE    MB(4)
#define PHYS_HEAP_SIZE      (PERS_ARENA_SIZE + TRAN_ARENA_SIZE + FRAME_ARENA_SIZE)

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

    struct MemoryArena;

    struct TempArena
    {
        MemoryArena*    Arena;
        u64             Used;
    };

    struct MemoryArena
    {
        u8*         Base;
        u64         Size;
        u64         Used;
        u32         TempCount;

        void*       Push(u64 size);
        void*       PushZero(u64 size);
        void        Pop(u64 size);
        void        Clear();

        TempArena   BeginTemp();
        void        EndTemp(const TempArena& tmp);

        void        Subarena(MemoryArena& sub, u64 size);
    };

    inline void* gVirtSpace;
    inline void* gPhysHeap;

    inline MemoryArena gPersArena;
    inline MemoryArena gTranArena;
    inline MemoryArena gFrameArena;

    void GetOsInfo(OsInfo* info);

    void*   ReserveVirtSpace(void* addr, u64 size);
    void*   AllocPhysHeap(void* vm, u64 size);
    void    FreePhysHeap(void* heap, u64 size);
    void    ReleaseVirtSpace(void* vm);

    SND_INLINE void InitArena(MemoryArena& arena, void* base, u64 size, u64 offset = 0)
    {
        arena.Base = (u8*)base + offset;
        arena.Size = size;
        arena.Used = 0;
        arena.TempCount = 0;
    }

    // MemoryArena

    #define PushSize(arena, size)           (u8*)arena.PushZero(size)
    #define PushStruct(arena, type)         (type*)arena.PushZero(sizeof(type))
    #define PushArray(arena, type, count)   (type*)arena.PushZero(sizeof(type) * count)

    SND_INLINE void* MemoryArena::Push(u64 size)
    {
        SND_ASSERT(Used + size <= Size);
        void* data = Base + Used;
        Used += size;
        return data;
    }

    SND_INLINE void* MemoryArena::PushZero(u64 size)
    {
        void* data = Push(size);
        memset(data, 0, size);
        return data;
    }

    SND_INLINE void MemoryArena::Pop(u64 size)
    {
        SND_ASSERT(Used >= size);
        Used -= size;
    }

    SND_INLINE void MemoryArena::Clear()
    {
        Used = 0;
    }

    SND_INLINE TempArena MemoryArena::BeginTemp()
    {
        TempArena tmp;
        tmp.Arena = this;
        tmp.Used = Used;
        TempCount++;
        return tmp;
    }

    SND_INLINE void MemoryArena::EndTemp(const TempArena& tmp)
    {
        SND_ASSERT(Used >= tmp.Used);
        Used = tmp.Used;
        TempCount--;
    }

    SND_INLINE void MemoryArena::Subarena(MemoryArena& sub, u64 size)
    {
        sub.Base = (u8*)PushZero(size);
        sub.Size = size;
        sub.Used = 0;
        sub.TempCount = 0;
    }
}
