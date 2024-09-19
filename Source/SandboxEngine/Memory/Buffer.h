#pragma once

#include "SandboxEngine/Core/CoreMacros.h"
#include "SandboxEngine/Core/Log.h"

namespace snd
{
    // General buffer with an ability to store elements of given size.
    struct Buffer
    {
    public:
                            Buffer();
                            Buffer(size_t capacity, size_t elementSize);
                            Buffer(const Buffer&) = delete;
                            Buffer(Buffer&& other) noexcept;
                            ~Buffer();
        
        Buffer&             operator=(const Buffer&) = delete;
        Buffer&             operator=(Buffer&& other) noexcept;
        
        void                Realloc(size_t capacity);
        void                Realloc(size_t capacity, size_t elementSize);
        void                Free();
        void*               Get(size_t index) const;
        
        bool                Valid() const;          // is buffer initialized
        void*               Data() const;           // internal data pointer
        void*               Last() const;           // pointer to last buffer element
        size_t              Size() const;           // buffer size in bytes
        size_t              Capacity() const;       // amount of buffer elements it can hold
        size_t              ElementSize() const;    // buffer element size in bytes

        void                Meminit(uint8_t val) const;
        
    private:
        uint8_t*            m_Data;
        size_t              m_Capacity;
        size_t              m_ElementSize;
    };

    SND_INLINE Buffer::Buffer()
        : m_Data(nullptr), m_Capacity(0), m_ElementSize(0)
    {
    }

    SND_INLINE Buffer::Buffer(size_t capacity, size_t elementSize)
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

    SND_INLINE void Buffer::Realloc(size_t capacity)
    {
        Realloc(capacity, m_ElementSize);
    }

    SND_INLINE void Buffer::Realloc(size_t capacity, size_t elementSize)
    {
        if (capacity == 0 || elementSize == 0)
        {
            Free();
            return;
        }
        
        if (void* newData = realloc(m_Data, capacity * elementSize))
        {
            m_Data = static_cast<uint8_t*>(newData);
            m_Capacity = capacity;
            m_ElementSize = elementSize;
        }
        else
        {
            SND_LOG_CRITICAL("Realloc failed (data = {}, buffer = {})", static_cast<void*>(m_Data), static_cast<void*>(this));
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

    SND_INLINE void* Buffer::Get(size_t index) const
    {
        if (index >= m_Capacity)
        {
            return nullptr;            
        }
        
        return m_Data + index * m_ElementSize;
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
        return m_Data + (m_Capacity - 1);
    }

    SND_INLINE size_t Buffer::Size() const
    {
        return m_Capacity * m_ElementSize;
    }

    SND_INLINE size_t Buffer::Capacity() const
    {
        return m_Capacity;
    }

    SND_INLINE size_t Buffer::ElementSize() const
    {
        return m_ElementSize;
    }

    SND_INLINE void Buffer::Meminit(uint8_t val) const
    {
        if (m_Data)
        {
            memset(m_Data, val, m_Capacity * m_ElementSize);
        }
    }
}
