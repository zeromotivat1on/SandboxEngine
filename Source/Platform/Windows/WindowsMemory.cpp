#include "sndpch.h"
#include "SandboxEngine/Memory/Memory.h"

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

void* snd::vmreserve(void* addr, u64 size)
{
    return VirtualAlloc(addr, size, MEM_RESERVE, PAGE_READWRITE);
}

void* snd::vmcommit(void* vm, u64 size)
{
    return VirtualAlloc(vm, size, MEM_COMMIT, PAGE_READWRITE);
}

bool snd::vmdecommit(void* vm, u64 size)
{
    return VirtualFree(vm, size, MEM_DECOMMIT);
}

bool snd::vmrelease(void* vm)
{
    return VirtualFree(vm, 0, MEM_RELEASE);
}
