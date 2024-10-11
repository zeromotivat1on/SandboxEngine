#include "sndpch.h"
#include "SandboxEngine/Threading/Thread.h"

#define INVALID_THREAD_OP_RES ((DWORD)-1)

namespace snd::thread
{
    DWORD GetCreationFlags(CreateType type)
    {
        switch(type)
        {
            case CreateType::Immediate: return 0;
            case CreateType::Suspended: return CREATE_SUSPENDED;
            default:
                SND_CORE_CRITICAL("Unknown thread create type '{}'", (u8)type);
                return 0;
        }
    }

    Handle Current()
    {
        return GetCurrentThread();
    }

    u64 CurrentId()
    {
        return GetCurrentThreadId();
    }

    void Sleep(u32 ms)
    {
        ::Sleep(ms);
    }

    bool Active(Handle handle)
    {
        DWORD exitCode;
        GetExitCodeThread(handle, &exitCode);
        return exitCode == STILL_ACTIVE;
    }

    Handle Create(CreateType type, EntryPoint entry, void* userdata)
    {
        DWORD id;
        return CreateThread(0, 0, (LPTHREAD_START_ROUTINE)entry, userdata, GetCreationFlags(type), &id);
    }

    void Resume(Handle handle)
    {
        const DWORD res = ResumeThread(handle);
        SND_ASSERT(res != INVALID_THREAD_OP_RES);
    }

    void Suspend(Handle handle)
    {
        const DWORD res = SuspendThread(handle);
        SND_ASSERT(res != INVALID_THREAD_OP_RES);
    }

    void Terminate(Handle handle)
    {
        DWORD exitCode;
        GetExitCodeThread(handle, &exitCode);
        const BOOL res = TerminateThread(handle, exitCode);
        SND_ASSERT(res);
    }

    SemaphoreHandle MakeSemaphore(s32 val, s32 max)
    {
        return CreateSemaphore(nullptr, val, max, nullptr);
    }

    void ReleaseSemaphore(SemaphoreHandle handle)
    {
        LONG prevCount;
        ::ReleaseSemaphore(handle, 1, &prevCount);
    }

    void WaitSemaphore(SemaphoreHandle handle, u32 ms)
    {
        WaitForSingleObjectEx(handle, ms, FALSE);
    }

    void ReadBarrier()
    {
        _ReadBarrier();
    }

    void WriteBarrier()
    {
        _WriteBarrier();
    }

    void ReadWriteBarrier()
    {
        _ReadWriteBarrier();
    }

    void ReadFence()
    {
        _mm_lfence();
    }

    void WriteFence()
    {
        _mm_sfence();
    }

    void ReadWriteFence()
    {
        _mm_mfence();
    }

    s32 AtomicExchange(volatile s32* dst, s32 val)
    {
        return InterlockedExchange((volatile LONG*)dst, val);
    }

    s32 AtomicCompareExchange(volatile s32* dst, s32 val, s32 cmp)
    {
        return InterlockedCompareExchange((volatile LONG*)dst, val, cmp);
    }

    s32 AtomicAdd(volatile s32* dst, s32 val)
    {
        return InterlockedAdd((volatile LONG*)dst, val);
    }

    s32 AtomicIncrement(volatile s32* dst)
    {
        return InterlockedIncrement((volatile LONG*)dst);
    }

    s32 AtomicDecrement(volatile s32* dst)
    {
        return InterlockedDecrement((volatile LONG*)dst);
    }
}
