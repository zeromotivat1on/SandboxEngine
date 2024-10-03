#pragma once

namespace snd
{
    // General buffer with an ability to store elements of given size.
    // Max size this buffer can hold ~= 4GB * ElementSize.
    struct Buffer
    {
    public:
                            Buffer();
                            Buffer(u32 capacity, u16 elementSize);
                            Buffer(const Buffer&) = delete;
                            Buffer(Buffer&& other) noexcept;
                            ~Buffer();
        
        Buffer&             operator=(const Buffer&) = delete;
        Buffer&             operator=(Buffer&& other) noexcept;
        
        void                Realloc(u32 capacity);
        void                Realloc(u32 capacity, u16 elementSize);
        void                Free();
        void*               Get(u32 index) const;
        
        bool                Valid() const;          // is buffer initialized
        void*               Data() const;           // internal data pointer
        void*               Last() const;           // pointer to last buffer element
        u64                 Size() const;           // buffer size in bytes
        u32                 Capacity() const;       // amount of buffer elements it can hold
        u64                 Capacity64() const;     // capacity as uint64
        u16                 ElementSize() const;    // buffer element size in bytes

    private:
        u8*                 m_Data;
        u32                 m_Capacity;
        u16                 m_ElementSize;
    };

    SND_INLINE Buffer::Buffer()
        : m_Data(nullptr), m_Capacity(0), m_ElementSize(0)
    {
    }

    SND_INLINE Buffer::Buffer(u32 capacity, u16 elementSize)
        : m_Data(nullptr), m_Capacity(capacity), m_ElementSize(elementSize)
    {
        Realloc(capacity, elementSize);
    }

    SND_INLINE Buffer::Buffer(Buffer&& other) noexcept
        : m_Data(other.m_Data),
          m_Capacity(other.m_Capacity),
          m_ElementSize(other.m_ElementSize)
    {
        other.m_Data = nullptr;
        other.m_Capacity = 0;
        other.m_ElementSize = 0;
    }

    SND_INLINE Buffer::~Buffer()
    {
        Free();
    }

    SND_INLINE Buffer& Buffer::operator=(Buffer&& other) noexcept
    {
        if (this != &other)
        {
            // Free current resources.
            Free();

            // Transfer ownership of the other object's resources.
            m_Data = other.m_Data;
            m_Capacity = other.m_Capacity;
            m_ElementSize = other.m_ElementSize;

            // Nullify the other object.
            other.m_Data = nullptr;
            other.m_Capacity = 0;
            other.m_ElementSize = 0;
        }
        
        return *this;
    }

    SND_INLINE void Buffer::Realloc(u32 capacity)
    {
        Realloc(capacity, m_ElementSize);
    }

    SND_INLINE void Buffer::Realloc(u32 capacity, u16 elementSize)
    {
        if (capacity == 0 || elementSize == 0)
        {
            Free();
            return;
        }
        
        if (void* newData = realloc(m_Data, static_cast<u64>(capacity) * elementSize))
        {
            m_Data = static_cast<u8*>(newData);

            if (capacity > m_Capacity)
            {
                memset(m_Data + Capacity64() * elementSize, 0, static_cast<u64>(capacity - m_Capacity) * elementSize);
            }
            
            m_Capacity = capacity;
            m_ElementSize = elementSize;
        }
        else
        {
            SND_CRITICAL("Realloc failed (data = {}, buffer = {})", static_cast<void*>(m_Data), static_cast<void*>(this));
        }
    }

    SND_INLINE void Buffer::Free()
    {
        if (m_Data)
        {
            free(m_Data);
            m_Data = nullptr;
            m_Capacity = 0;
            m_ElementSize = 0;
        }
    }

    SND_INLINE void* Buffer::Get(u32 index) const
    {
        if (index >= m_Capacity)
        {
            return nullptr;            
        }
        
        return m_Data + static_cast<u64>(index) * m_ElementSize;
    }

    SND_INLINE bool Buffer::Valid() const
    {
        return m_Data && m_Capacity > 0 && m_ElementSize > 0;
    }

    SND_INLINE void* Buffer::Data() const
    {
        return m_Data;
    }

    SND_INLINE void* Buffer::Last() const
    {
        return m_Data + (Capacity64() - 1) * m_ElementSize;
    }

    SND_INLINE u64 Buffer::Size() const
    {
        return Capacity64() * m_ElementSize;
    }

    SND_INLINE u32 Buffer::Capacity() const
    {
        return m_Capacity;
    }

    SND_INLINE u64 Buffer::Capacity64() const
    {
        return m_Capacity;
    }

    SND_INLINE u16 Buffer::ElementSize() const
    {
        return m_ElementSize;
    }
}
