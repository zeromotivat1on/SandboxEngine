#pragma once

namespace snd::thread
{
    // Multithreading work queue with synced entry process.
    // Each entry data should be unique in order to avoid undefined behavior.
    class WorkQueue
    {
    public:
        DECLARE_DELEGATE_Params(OnEntryProcess, const WorkQueue*, void*);

        struct Entry
        {
            OnEntryProcess      Delegate;
            void*               Data;
        };

    public:
        explicit            WorkQueue(SemaphoreHandle semaphore);

        bool                InProgress() const;
        void                AddEntry(OnEntryProcess delegate, void* data);

        // Process next entry, return true if processed,
        // false if no work to do, also hints to put thread to wait for more work.
        bool                ProcessNextEntry();

        // Wait for the semaphore to be signaled.
        void                Wait(u32 ms) const;
        void                Reset();

    private:
        SemaphoreHandle     m_Semaphore;
        volatile u32        m_EntryCount;
        volatile u32        m_NextEntry;
        volatile u32        m_DoneEntryCount;
        Entry               m_Entries[256];
    };
}
