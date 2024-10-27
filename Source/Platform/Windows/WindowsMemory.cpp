#include "sndpch.h"
#include "Engine/Memory/Memory.h"

void snd::GetOsInfo(OsInfo* info)
{
    if (!info) return;

    MEMORYSTATUSEX sysmem;
    sysmem.dwLength = sizeof(sysmem);
    if (GlobalMemoryStatusEx(&sysmem))
    {
        info->TotalPhys = sysmem.ullTotalPhys;
        info->TotalVirt = sysmem.ullTotalVirtual;
        info->AvailPhys = sysmem.ullAvailPhys;
        info->AvailVirt = sysmem.ullAvailVirtual;
    }
    else
    {
        SND_ERROR("Failed to retreive system memory status (%d)", GetLastError());
    }

    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    info->PageSize = sysinfo.dwPageSize;
    info->AllocGran = sysinfo.dwAllocationGranularity;
    info->LgCoreNum = sysinfo.dwNumberOfProcessors;
}

void* snd::ReserveVirtSpace(void* addr, u64 size)
{
    SND_ASSERT(size > 0);
    return VirtualAlloc(addr, size, MEM_RESERVE, PAGE_READWRITE);
}

void* snd::AllocPhysHeap(void* vm, u64 size)
{
    SND_ASSERT(vm);
    SND_ASSERT(size > 0);

    void* physHeap = VirtualAlloc(vm, size, MEM_COMMIT, PAGE_READWRITE);
    SND_ASSERT(physHeap);

    return physHeap;
}

void snd::FreePhysHeap(void* heap, u64 size)
{
    SND_ASSERT(heap);
    SND_ASSERT(size > 0);
    VirtualFree(heap, size, MEM_DECOMMIT);
}

void snd::ReleaseVirtSpace(void* vm)
{
    SND_ASSERT(vm);
    VirtualFree(vm, 0, MEM_RELEASE);
}
