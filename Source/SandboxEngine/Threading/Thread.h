#pragma once

namespace snd
{
    using ThreadEntryPoint = u32 (void* userdata);

    enum class ThreadCreateType : u8
    {
        Immediate = 0,  // start thread immediately after creation
        Suspended = 1,  // create and suspend thread until resume
    };

    u64     ThreadCurrentId();
    void    ThreadSleep(u32 ms);
    bool    ThreadActive(void* handle);

    void*   ThreadCreate(ThreadCreateType type, ThreadEntryPoint entry, void* userdata);
    void    ThreadResume(void* handle);
    void    ThreadSuspend(void* handle);
    void    ThreadTerminate(void* handle);

    // Create semaphore with initial val count and maximum max count.
    void*   MakeSemaphore(s32 val, s32 max);
    void    ReleaseSemaphore(void* handle);
    void    WaitSemaphore(void* handle, u32 ms);

    // Hint compiler to not reorder reads, happened before barrier.
    // Basically disable read/load concerned optimizations.
    void    ReadBarrier();

    // Same as read barrier, but for write/store operations.
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
