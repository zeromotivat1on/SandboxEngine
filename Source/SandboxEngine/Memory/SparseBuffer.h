#pragma once

#include "SandboxEngine/Memory/Buffer.h"

namespace snd
{
    // Buffer that allocates memory only for actual elements to store.
    // All possible indices [0, capacity) are stored in sparse index array.
    struct SparseBuffer
    {
    public:
                            SparseBuffer();
                            SparseBuffer(u32 sparseCapacity, u16 denseElementSize);
                            SparseBuffer(const SparseBuffer&) = delete;
                            SparseBuffer(SparseBuffer&& other) noexcept;
                            ~SparseBuffer();

        SparseBuffer&       operator=(const SparseBuffer&) = delete;
        SparseBuffer&       operator=(SparseBuffer&& other) noexcept;
        
        void                Realloc(u32 sparseCapacity);
        void                Free();
        void*               Get(u32 index) const;
        void*               Allocate(u32 index);
        void*               GetOrAllocate(u32 index);
        bool                Remove(u32 index);

        bool                Valid() const;      // is buffer initialized

    private:
        u32                 m_SparseCapacity;   // max amount of sparse indices
        u32                 m_DenseCount;       // current amount of dense elements
        u32*                m_SparseIndices;    // sparse indices data
        Buffer              m_DenseData;        // dense elements data
    };

    SND_INLINE SparseBuffer::SparseBuffer()
        : m_SparseCapacity(0), m_DenseCount(0), m_SparseIndices(nullptr)
    {
    }

    SND_INLINE SparseBuffer::SparseBuffer(u32 sparseCapacity, u16 denseElementSize)
        : m_SparseCapacity(sparseCapacity),
          m_DenseCount(0),
          m_SparseIndices(new u32[sparseCapacity]),
          m_DenseData(0, denseElementSize)
    {
        memset(m_SparseIndices, INVALID_INDEX, m_SparseCapacity * sizeof(u32));
    }

    SND_INLINE SparseBuffer::SparseBuffer(SparseBuffer&& other) noexcept
        : m_SparseCapacity(other.m_SparseCapacity),
          m_DenseCount(other.m_DenseCount),
          m_SparseIndices(other.m_SparseIndices),
          m_DenseData(std::move(other.m_DenseData))
    {
        other.m_SparseIndices = nullptr; // ensure the old object does not try to free memory
        other.m_SparseCapacity = 0;
        other.m_DenseCount = 0;
    }

    SND_INLINE SparseBuffer::~SparseBuffer()
    {
        Free();
    }

    SND_INLINE SparseBuffer& SparseBuffer::operator=(SparseBuffer&& other) noexcept
    {
        if (this != &other)
        {
            // Free the current resources.
            Free();

            // Transfer ownership.
            m_SparseCapacity = other.m_SparseCapacity;
            m_DenseCount = other.m_DenseCount;
            m_SparseIndices = other.m_SparseIndices;
            m_DenseData = std::move(other.m_DenseData);

            // Nullify the other object.
            other.m_SparseIndices = nullptr;
            other.m_SparseCapacity = 0;
            other.m_DenseCount = 0;
        }
        
        return *this;
    }

    SND_INLINE void SparseBuffer::Realloc(u32 sparseCapacity)
    {
        if (sparseCapacity <= m_SparseCapacity)
        {
            return;
        }

        u32* sparseIndices = new u32[sparseCapacity];

        memset(sparseIndices + m_SparseCapacity, INVALID_INDEX, (sparseCapacity - m_SparseCapacity) * sizeof(u32));
        memcpy(sparseIndices, m_SparseIndices, m_SparseCapacity * sizeof(u32));

        delete[] m_SparseIndices;

        m_SparseIndices = sparseIndices;
        m_SparseCapacity = sparseCapacity;
    }

    SND_INLINE void SparseBuffer::Free()
    {
        delete[] m_SparseIndices;

        m_SparseIndices = nullptr;
        m_SparseCapacity = 0;

        m_DenseData.Free();
        m_DenseCount = 0;
    }

    SND_INLINE void* SparseBuffer::Get(u32 index) const
    {
        if (index >= m_SparseCapacity)
        {
            return nullptr;
        }
        
        const u32 denseIndex = m_SparseIndices[index];

        if (denseIndex >= m_DenseCount)
        {
            return nullptr;
        }

        return m_DenseData.Get(denseIndex);
    }

    SND_INLINE void* SparseBuffer::Allocate(u32 index)
    {
        if (index >= m_SparseCapacity || m_SparseIndices[index] != INVALID_UINDEX)
        {
            return nullptr;
        }

        const u32 denseIndex = m_DenseCount++;
        m_SparseIndices[index] = denseIndex;

        if (denseIndex >= m_DenseData.Capacity())
        {
            const u32 newCapacity = m_DenseData.Capacity() * 2 + 1; 
            m_DenseData.Realloc(newCapacity);
        }
        
        return m_DenseData.Get(denseIndex);
    }

    SND_INLINE void* SparseBuffer::GetOrAllocate(u32 index)
    {
        if (void* data = Get(index))
        {
            return data;
        }

        return Allocate(index);
    }

    SND_INLINE bool SparseBuffer::Remove(u32 index)
    {
        if (index >= m_SparseCapacity || m_SparseIndices[index] == INVALID_UINDEX)
        {
            return false;
        }

        const u32 denseIndex     = m_SparseIndices[index];
        const u32 lastDenseIndex = m_DenseCount - 1;
        
        m_SparseIndices[index] = INVALID_UINDEX;

        if (denseIndex == lastDenseIndex)
        {
            m_DenseCount--;
            return true;
        }
        
        void* lastElement   = m_DenseData.Get(lastDenseIndex);
        void* targetElement = m_DenseData.Get(denseIndex);

        memcpy(targetElement, lastElement, m_DenseData.ElementSize());

        for (u32 i = 0; i < m_SparseCapacity; ++i)
        {
            if (m_SparseIndices[i] == lastDenseIndex)
            {
                m_SparseIndices[i] = denseIndex;
                break;
            }
        }
        
        m_DenseCount--;
        return true;
    }

    SND_INLINE bool SparseBuffer::Valid() const
    {
        return m_SparseIndices && m_SparseCapacity > 0 && m_DenseData.ElementSize() > 0;
    }
}
