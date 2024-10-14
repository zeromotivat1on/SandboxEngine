#include "sndpch.h"
#include "SandboxEngine/Threading/WorkQueue.h"

namespace snd::thread
{
    WorkQueue::WorkQueue(SemaphoreHandle semaphore)
        : m_Semaphore(semaphore), m_Entries(),
          m_EntryToAdd(0), m_EntryToProcess(0),
          m_AddedEntryCount(0), m_ProcessedEntryCount(0)
    {
    }

    bool WorkQueue::InProgress() const
    {
        return m_ProcessedEntryCount != m_AddedEntryCount;
    }

    void WorkQueue::AddEntry(const OnEntryProcess& delegate, void* data)
    {
        const u32 entryToAdd = m_EntryToAdd;
        const u32 nextEntryToAdd = (entryToAdd + 1) % ARRAY_COUNT(m_Entries);

        // SND_ASSERT(nextEntryToAdd != m_EntryToProcess);
        if (nextEntryToAdd == m_EntryToProcess)
        {
            SND_CORE_ERROR(
                "Wrong read/write indices, m_EntryToAdd = {}, entryToAdd = {}, nextEntryToAdd = {}, m_EntryToProcess = {}, char data = {}",
                m_EntryToAdd, entryToAdd, nextEntryToAdd, m_EntryToProcess, (const char*)data);
        }

        const u32 idx = AtomicCompareExchange((volatile s32*)&m_EntryToAdd, nextEntryToAdd, entryToAdd);
        if (idx == entryToAdd)
        {
            Entry& entry = m_Entries[entryToAdd];
            entry.Delegate = delegate;
            entry.Data = data;

            AtomicIncrement((volatile s32*)&m_ProcessedEntryCount);
            ReleaseSemaphore(m_Semaphore);
        }
        else
        {
            SND_CORE_WARNING(
                "Add comparand miss, m_EntryToAdd = {}, nextEntryToAdd = {}, entryToAdd = {}",
                m_EntryToAdd, nextEntryToAdd, entryToAdd);
        }
    }

    bool WorkQueue::ProcessEntry()
    {
        const u32 entryToProcess = m_EntryToProcess;
        if (entryToProcess == m_EntryToAdd)
        {
            return false;
        }

        const u32 nextEntryToProcess = (entryToProcess + 1) % ARRAY_COUNT(m_Entries);
        const u32 idx = AtomicCompareExchange((volatile s32*)&m_EntryToProcess, nextEntryToProcess, entryToProcess);
        if (idx == entryToProcess)
        {
            Entry& entry = m_Entries[entryToProcess];
            entry.Delegate.Execute(this, entry.Data);
            AtomicIncrement((volatile s32*)&m_AddedEntryCount);
        }
        else
        {
            SND_CORE_WARNING(
                "Process comparand miss, m_EntryToProcess = {}, nextEntryToProcess = {}, entryToProcess = {}",
                m_EntryToProcess, nextEntryToProcess, entryToProcess);
        }

        return true;
    }

    void WorkQueue::WaitForWork(u32 ms) const
    {
        WaitSemaphore(m_Semaphore, ms);
    }
}
