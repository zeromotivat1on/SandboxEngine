#pragma once

namespace snd
{
    // Preallocated block of memory from which you can 'allocate' and 'release' data.
    // It's client responsibility to take care of data manipulations, fragmentation avoidance etc.
    struct Arena final
    {
    public:
        explicit    Arena(u64 size);
                    NOT_COPYABLE(Arena);
                    NOT_MOVABLE(Arena);
                    ~Arena();

        void*       Push(u64 size);     // push bytes
        void*       PushZero(u64 size); // push zero bytes
        void        Pop(u64 count);     // pop bytes
        void        Fit(u64 pos);       // set new position
        void        Clear();            // reset position
        void        Free();             // release allocated data

        u64         Pos();
        u64         Size();

    private:
        u8*         m_Data;             // memory block pointer
        u64         m_Pos;              // current number of bytes
        u64         m_Size;             // max number of bytes
    };

    SND_INLINE Arena::Arena(u64 size)
        : m_Pos(0), m_Size(size)
    {
        m_Data = (u8*)malloc(size);
        SND_ASSERT(m_Data, "Failed to allocate memory");
    }

    SND_INLINE Arena::~Arena()
    {
        Free();
    }

    SND_INLINE void* Arena::Push(u64 count)
    {
        SND_ASSERT(m_Pos + count <= m_Size);
        m_Pos += count;
        return m_Data + m_Pos;
    }

    SND_INLINE void* Arena::PushZero(u64 count)
    {
        void* p = Push(count);
        memset(p, 0, count);
        return p;
    }

    SND_INLINE void Arena::Pop(u64 count)
    {
        SND_ASSERT(m_Pos >= count);
        m_Pos -= count;
    }

    SND_INLINE void Arena::Fit(u64 pos)
    {
        SND_ASSERT(pos < m_Pos);
        m_Pos = pos;
    }

    SND_INLINE void Arena::Clear()
    {
        m_Pos = 0;
    }

    SND_INLINE void Arena::Free()
    {
        if (m_Data)
        {
            free(m_Data);
            m_Data = nullptr;
            m_Pos = 0;
            m_Size = 0;
        }
    }

    SND_INLINE u64 Arena::Pos()
    {
        return m_Pos;
    }

    SND_INLINE u64 Arena::Size()
    {
        return m_Size;
    }
}
