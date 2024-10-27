#include "sndpch.h"
#include "Engine/Threading/Thread.h"

#define INVALID_THREAD_OP_RES ((DWORD)-1)

static DWORD GetCreationFlags(snd::ThreadCreateType type)
{
    switch(type)
    {
        case snd::ThreadCreateType::Immediate: return 0;
        case snd::ThreadCreateType::Suspended: return CREATE_SUSPENDED;
        default:
            SND_ERROR("Unknown thread create type (%d)", (u8)type);
            return 0;
    }
}

u64 snd::ThreadCurrentId()
{
    return GetCurrentThreadId();
}

void snd::ThreadSleep(u32 ms)
{
    Sleep(ms);
}

bool snd::ThreadActive(void* handle)
{
    DWORD exitCode;
    GetExitCodeThread(handle, &exitCode);
    return exitCode == STILL_ACTIVE;
}

void* snd::ThreadCreate(ThreadCreateType type, ThreadEntryPoint entry, void* userdata)
{
    DWORD id;
    return CreateThread(0, 0, (LPTHREAD_START_ROUTINE)entry, userdata, GetCreationFlags(type), &id);
}

void snd::ThreadResume(void* handle)
{
    const DWORD res = ResumeThread(handle);
    SND_ASSERT(res != INVALID_THREAD_OP_RES);
}

void snd::ThreadSuspend(void* handle)
{
    const DWORD res = SuspendThread(handle);
    SND_ASSERT(res != INVALID_THREAD_OP_RES);
}

void snd::ThreadTerminate(void* handle)
{
    DWORD exitCode;
    GetExitCodeThread(handle, &exitCode);
    const BOOL res = TerminateThread(handle, exitCode);
    SND_ASSERT(res);
}

void* snd::MakeSemaphore(s32 val, s32 max)
{
    return CreateSemaphore(nullptr, val, max, nullptr);
}

void snd::ReleaseSemaphore(void* handle)
{
    LONG prevCount;
    ::ReleaseSemaphore(handle, 1, &prevCount);
}

void snd::WaitSemaphore(void* handle, u32 ms)
{
    WaitForSingleObjectEx(handle, ms, FALSE);
}

void snd::ReadBarrier()
{
    _ReadBarrier();
}

void snd::WriteBarrier()
{
    _WriteBarrier();
}

void snd::ReadWriteBarrier()
{
    _ReadWriteBarrier();
}

void snd::ReadFence()
{
    _mm_lfence();
}

void snd::WriteFence()
{
    _mm_sfence();
}

void snd::ReadWriteFence()
{
    _mm_mfence();
}

s32 snd::AtomicExchange(volatile s32* dst, s32 val)
{
    return InterlockedExchange((volatile LONG*)dst, val);
}

s32 snd::AtomicCompareExchange(volatile s32* dst, s32 val, s32 cmp)
{
    return InterlockedCompareExchange((volatile LONG*)dst, val, cmp);
}

s32 snd::AtomicAdd(volatile s32* dst, s32 val)
{
    return InterlockedAdd((volatile LONG*)dst, val);
}

s32 snd::AtomicIncrement(volatile s32* dst)
{
    return InterlockedIncrement((volatile LONG*)dst);
}

s32 snd::AtomicDecrement(volatile s32* dst)
{
    return InterlockedDecrement((volatile LONG*)dst);
}
