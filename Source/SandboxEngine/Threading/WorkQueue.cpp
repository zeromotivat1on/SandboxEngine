#include "sndpch.h"
#include "SandboxEngine/Threading/WorkQueue.h"

namespace snd::thread
{
    WorkQueue::WorkQueue(SemaphoreHandle semaphore)
        : m_Semaphore(semaphore), m_EntryCount(0), m_NextEntry(0), m_DoneEntryCount(0), m_Entries()
    {
    }

    bool WorkQueue::InProgress() const
    {
        return m_DoneEntryCount <  m_EntryCount;
    }

    void WorkQueue::AddEntry(OnEntryProcess delegate, void* data)
    {
        SND_ASSERT(m_EntryCount < 256);

        m_Entries[m_EntryCount].Delegate = delegate;
        m_Entries[m_EntryCount].Data = data;

        WriteBarrier();
        WriteFence();

        m_EntryCount++;

        ReleaseSemaphore(m_Semaphore);
    }

    bool WorkQueue::ProcessNextEntry()
    {
        const u32 originalNextEntry = m_NextEntry;
        if (originalNextEntry >= m_EntryCount)
        {
            return false;
        }

        const u32 idx = AtomicCompareExchange((volatile s32*)&m_NextEntry, originalNextEntry + 1, originalNextEntry);
        if (idx == originalNextEntry)
        {
            Entry& entry = m_Entries[idx];
            entry.Delegate.Execute(this, entry.Data);
            AtomicIncrement((volatile s32*)&m_DoneEntryCount);
        }

        return true;
    }

    void WorkQueue::Wait(u32 ms) const
    {
        WaitSemaphore(m_Semaphore, ms);
    }

    void WorkQueue::Reset() const
    {
        m_EntryCount = 0;
        m_NextEntry = 0;
        m_DoneEntryCount = 0;
    }
}
