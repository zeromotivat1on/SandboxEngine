#pragma once

namespace snd::thread
{
    using Handle            = void*;
    using SemaphoreHandle   = void*;
    using EntryPoint        = u64 (void* userdata);

    enum class CreateType : u8
    {
        Immediate = 0,  // start thread immediately after creation
        Suspended = 1,  // create and suspend thread until resume
    };

    Handle  Current();
    u64     CurrentId();
    void    Sleep(u32 ms);
    bool    Active(Handle handle);

    Handle  Create(CreateType type, EntryPoint entry, void* userdata);
    void    Resume(Handle handle);
    void    Suspend(Handle handle);
    void    Terminate(Handle handle);

    // Create semaphore with initial val count and maximum max count.
    SemaphoreHandle MakeSemaphore(s32 val, s32 max);
    void            ReleaseSemaphore(SemaphoreHandle handle);
    void            WaitSemaphore(SemaphoreHandle handle, u32 ms);

    // Hint compiler to not reorder reads, happened before barrier.
    // Basically disable read/store concerned optimizations.
    void    ReadBarrier();

    // Same as read barrier, but for write/load operations.
    void    WriteBarrier();

    // Read and write barriers at once.
    void    ReadWriteBarrier();

    // Ensure the order of read/load CPU instructions.
    void    ReadFence();

    // Ensure the order of write/store CPU instructions.
    void    WriteFence();

    // Ensure the order of read/load and write/store CPU instructions.
    void    ReadWriteFence();

    // Set dst to val and return dst value before exchange.
    s32     AtomicExchange(volatile s32* dst, s32 val);

    // Set dst to val if current dst is equal to cmp, return dst value before exchange.
    // In case if dst value and cmp is not the same, nothing happens.
    s32     AtomicCompareExchange(volatile s32* dst, s32 val, s32 cmp);

    // Add dst value and val, store result in dst and return it as well.
    s32     AtomicAdd(volatile s32* dst, s32 val);

    s32     AtomicIncrement(volatile s32* dst);
    s32     AtomicDecrement(volatile s32* dst);
}
