#pragma once

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
        u32                 mSparseCapacity;   // max amount of sparse indices
        u32                 mDenseCount;       // current amount of dense elements
        u32*                mSparseIndices;    // sparse indices data
        Buffer              mDenseData;        // dense elements data
    };

    SND_INLINE SparseBuffer::SparseBuffer()
        : mSparseCapacity(0), mDenseCount(0), mSparseIndices(nullptr)
    {
    }

    SND_INLINE SparseBuffer::SparseBuffer(u32 sparseCapacity, u16 denseElementSize)
        : mSparseCapacity(sparseCapacity),
          mDenseCount(0),
          mSparseIndices(new u32[sparseCapacity]),
          mDenseData(0, denseElementSize)
    {
        memset(mSparseIndices, INVALID_UINDEX, mSparseCapacity * sizeof(u32));
    }

    SND_INLINE SparseBuffer::SparseBuffer(SparseBuffer&& other) noexcept
        : mSparseCapacity(other.mSparseCapacity),
          mDenseCount(other.mDenseCount),
          mSparseIndices(other.mSparseIndices),
          mDenseData(std::move(other.mDenseData))
    {
        other.mSparseIndices = nullptr; // ensure the old object does not try to free memory
        other.mSparseCapacity = 0;
        other.mDenseCount = 0;
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
            mSparseCapacity = other.mSparseCapacity;
            mDenseCount = other.mDenseCount;
            mSparseIndices = other.mSparseIndices;
            mDenseData = std::move(other.mDenseData);

            // Nullify the other object.
            other.mSparseIndices = nullptr;
            other.mSparseCapacity = 0;
            other.mDenseCount = 0;
        }

        return *this;
    }

    SND_INLINE void SparseBuffer::Realloc(u32 sparseCapacity)
    {
        if (sparseCapacity <= mSparseCapacity)
        {
            return;
        }

        u32* sparseIndices = new u32[sparseCapacity];

        memset(sparseIndices + mSparseCapacity, INVALID_UINDEX, (sparseCapacity - mSparseCapacity) * sizeof(u32));
        memcpy(sparseIndices, mSparseIndices, mSparseCapacity * sizeof(u32));

        delete[] mSparseIndices;

        mSparseIndices = sparseIndices;
        mSparseCapacity = sparseCapacity;
    }

    SND_INLINE void SparseBuffer::Free()
    {
        delete[] mSparseIndices;

        mSparseIndices = nullptr;
        mSparseCapacity = 0;

        mDenseData.Free();
        mDenseCount = 0;
    }

    SND_INLINE void* SparseBuffer::Get(u32 index) const
    {
        if (index >= mSparseCapacity)
        {
            return nullptr;
        }

        const u32 denseIndex = mSparseIndices[index];

        if (denseIndex >= mDenseCount)
        {
            return nullptr;
        }

        return mDenseData.Get(denseIndex);
    }

    SND_INLINE void* SparseBuffer::Allocate(u32 index)
    {
        if (index >= mSparseCapacity || mSparseIndices[index] != INVALID_UINDEX)
        {
            return nullptr;
        }

        const u32 denseIndex = mDenseCount++;
        mSparseIndices[index] = denseIndex;

        if (denseIndex >= mDenseData.Capacity())
        {
            const u32 newCapacity = mDenseData.Capacity() * 2 + 1;
            mDenseData.Realloc(newCapacity);
        }

        return mDenseData.Get(denseIndex);
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
        if (index >= mSparseCapacity || mSparseIndices[index] == INVALID_UINDEX)
        {
            return false;
        }

        const u32 denseIndex     = mSparseIndices[index];
        const u32 lastDenseIndex = mDenseCount - 1;

        mSparseIndices[index] = INVALID_UINDEX;

        if (denseIndex == lastDenseIndex)
        {
            mDenseCount--;
            return true;
        }

        void* lastElement   = mDenseData.Get(lastDenseIndex);
        void* targetElement = mDenseData.Get(denseIndex);

        memcpy(targetElement, lastElement, mDenseData.ElementSize());

        for (u32 i = 0; i < mSparseCapacity; ++i)
        {
            if (mSparseIndices[i] == lastDenseIndex)
            {
                mSparseIndices[i] = denseIndex;
                break;
            }
        }

        mDenseCount--;
        return true;
    }

    SND_INLINE bool SparseBuffer::Valid() const
    {
        return mSparseIndices && mSparseCapacity > 0 && mDenseData.ElementSize() > 0;
    }
}
