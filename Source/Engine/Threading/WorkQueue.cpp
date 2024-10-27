#include "sndpch.h"
#include "Engine/Threading/WorkQueue.h"

snd::WorkQueue::WorkQueue(void* semaphore)
    : mSemaphore(semaphore), mEntries(),
      mEntryToAdd(0), mEntryToProcess(0),
      mAddedEntryCount(0), mProcessedEntryCount(0)
{
}

bool snd::WorkQueue::InProgress() const
{
    return mProcessedEntryCount != mAddedEntryCount;
}

void snd::WorkQueue::AddEntry(const OnEntryProcess& delegate, void* data)
{
    const u32 entryToAdd = mEntryToAdd;
    const u32 nextEntryToAdd = (entryToAdd + 1) % ARRAY_COUNT(mEntries);

    SND_ASSERT(nextEntryToAdd != mEntryToProcess);

    const u32 idx = AtomicCompareExchange((volatile s32*)&mEntryToAdd, nextEntryToAdd, entryToAdd);
    if (idx == entryToAdd)
    {
        Entry& entry = mEntries[entryToAdd];
        entry.Delegate = delegate;
        entry.Data = data;

        AtomicIncrement((volatile s32*)&mProcessedEntryCount);
        ReleaseSemaphore(mSemaphore);
    }
}

bool snd::WorkQueue::ProcessEntry()
{
    const u32 entryToProcess = mEntryToProcess;
    if (entryToProcess == mEntryToAdd)
    {
        return false;
    }

    const u32 nextEntryToProcess = (entryToProcess + 1) % ARRAY_COUNT(mEntries);
    const u32 idx = AtomicCompareExchange((volatile s32*)&mEntryToProcess, nextEntryToProcess, entryToProcess);
    if (idx == entryToProcess)
    {
        Entry& entry = mEntries[entryToProcess];
        entry.Delegate.Execute(this, entry.Data);
        AtomicIncrement((volatile s32*)&mAddedEntryCount);
    }

    return true;
}

void snd::WorkQueue::WaitForWork(u32 ms) const
{
    WaitSemaphore(mSemaphore, ms);
}
