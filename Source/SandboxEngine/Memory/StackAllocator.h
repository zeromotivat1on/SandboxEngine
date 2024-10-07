#pragma once

namespace snd
{
    // Simple allocator that simulates stack.
    // Data is operated in first-allocated-last-freed order.
    // New size must lie at the boundary between two memory blocks.
    class StackAllocator final
    {
    public:
        explicit    StackAllocator(u32 capacity);
                    NOT_COPYABLE(StackAllocator);
                    NOT_MOVABLE(StackAllocator);
                    ~StackAllocator();

        void*       Alloc(u32 size);    // request memory block of size bytes
        void        Fit(u32 size);      // move stack pointer down to fit size bytes
        void        Clear();            // set stack pointer to start of memory block
        void        Free();             // release memory block data

        void*       Data();
        u32         Size();
        u32         Capacity();

    private:
        u8*         m_Data;             // start of memory block
        u32         m_Size;             // current number of bytes
        u32         m_Capacity;         // max number of bytes
    };

    SND_INLINE StackAllocator::StackAllocator(u32 capacity)
        : m_Data(nullptr), m_Size(0), m_Capacity(capacity)
    {
        m_Data = static_cast<u8*>(malloc(capacity));
        memset(m_Data, 0, capacity);
        SND_ASSERT(m_Data, "Failed to allocate memory");
    }

    SND_INLINE StackAllocator::~StackAllocator()
    {
        Free();
    }

    SND_INLINE void* StackAllocator::Alloc(u32 size)
    {
        const u32 allocSize = m_Size + size;
        if (allocSize > m_Capacity)
        {
            return nullptr;
        }

        void* ptr = m_Data + m_Size;
        m_Size = allocSize;

        return ptr;
    }

    SND_INLINE void StackAllocator::Fit(u32 size)
    {
        if (size >= m_Size)
        {
            return;
        }

        memset(m_Data + size, 0, m_Capacity - size);
        m_Size = size;
    }

    SND_INLINE void StackAllocator::Clear()
    {
        memset(m_Data, 0, m_Capacity);
        m_Size = 0;
    }

    SND_INLINE void StackAllocator::Free()
    {
        if (m_Data)
        {
            free(m_Data);
            m_Data = nullptr;
            m_Size = 0;
            m_Capacity = 0;
        }
    }

    SND_INLINE void* StackAllocator::Data()
    {
        return m_Data;
    }

    SND_INLINE u32 StackAllocator::Size()
    {
        return m_Size;
    }

    SND_INLINE u32 StackAllocator::Capacity()
    {
        return m_Capacity;
    }
}
