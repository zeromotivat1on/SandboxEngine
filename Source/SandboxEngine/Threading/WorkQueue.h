#pragma once

namespace snd::thread
{
    // Multithreading circular work queue with synced entry addition and process.
    // Supported thread model is multiple producers multiple consumers.
    // At some situations when entry has not been processed but returned true,
    // this means that internal atomic operation was not completed as expected,
    // but it still counts as success because read and write indices are correct.
    class WorkQueue
    {
    public:
        DECLARE_DELEGATE_Params(OnEntryProcess, const WorkQueue*, void*);

        struct Entry
        {
            OnEntryProcess  Delegate;
            void*           Data;
        };

    public:
        explicit            WorkQueue(SemaphoreHandle semaphore);

        bool                InProgress() const;

        // Add entry for future process, assert if overflowed.
        void                AddEntry(const OnEntryProcess& delegate, void* data);

        // Process entry, return true if processed or can be processed,
        // false if no work to do, therefore hints to put thread to wait for more work.
        bool                ProcessEntry();

        // Idle calling thread untill new work is available.
        void                WaitForWork(u32 ms) const;

    private:
        SemaphoreHandle     m_Semaphore;
        Entry               m_Entries[256];
        volatile u32        m_EntryToAdd;
        volatile u32        m_EntryToProcess;
        volatile u32        m_AddedEntryCount;
        volatile u32        m_ProcessedEntryCount;
    };
}
