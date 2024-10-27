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

        Buffer&         operator=(const Buffer&) = delete;
        Buffer&         operator=(Buffer&& other) noexcept;

        void            Realloc(u32 capacity);
        void            Realloc(u32 capacity, u16 elementSize);
        void            Free();
        void*           Get(u32 index) const;

        bool            Valid() const;          // is buffer initialized
        void*           Data() const;           // internal data pointer
        void*           Last() const;           // pointer to last buffer element
        u64             Size() const;           // buffer size in bytes
        u32             Capacity() const;       // amount of buffer elements it can hold
        u64             Capacity64() const;     // capacity as uint64
        u16             ElementSize() const;    // buffer element size in bytes

    private:
        u8*             mData;
        u32             mCapacity;
        u16             mElementSize;
    };

    SND_INLINE Buffer::Buffer()
        : mData(nullptr), mCapacity(0), mElementSize(0)
    {
    }

    SND_INLINE Buffer::Buffer(u32 capacity, u16 elementSize)
        : mData(nullptr), mCapacity(capacity), mElementSize(elementSize)
    {
        Realloc(capacity, elementSize);
    }

    SND_INLINE Buffer::Buffer(Buffer&& other) noexcept
        : mData(other.mData),
          mCapacity(other.mCapacity),
          mElementSize(other.mElementSize)
    {
        other.mData = nullptr;
        other.mCapacity = 0;
        other.mElementSize = 0;
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
            mData = other.mData;
            mCapacity = other.mCapacity;
            mElementSize = other.mElementSize;

            // Nullify the other object.
            other.mData = nullptr;
            other.mCapacity = 0;
            other.mElementSize = 0;
        }

        return *this;
    }

    SND_INLINE void Buffer::Realloc(u32 capacity)
    {
        Realloc(capacity, mElementSize);
    }

    SND_INLINE void Buffer::Realloc(u32 capacity, u16 elementSize)
    {
        if (capacity == 0 || elementSize == 0)
        {
            Free();
            return;
        }

        if (void* newData = realloc(mData, static_cast<u64>(capacity) * elementSize))
        {
            mData = static_cast<u8*>(newData);

            if (capacity > mCapacity)
            {
                memset(mData + Capacity64() * elementSize, 0, static_cast<u64>(capacity - mCapacity) * elementSize);
            }

            mCapacity = capacity;
            mElementSize = elementSize;
        }
    }

    SND_INLINE void Buffer::Free()
    {
        if (mData)
        {
            free(mData);
            mData = nullptr;
            mCapacity = 0;
            mElementSize = 0;
        }
    }

    SND_INLINE void* Buffer::Get(u32 index) const
    {
        if (index >= mCapacity)
        {
            return nullptr;
        }

        return mData + static_cast<u64>(index) * mElementSize;
    }

    SND_INLINE bool Buffer::Valid() const
    {
        return mData && mCapacity > 0 && mElementSize > 0;
    }

    SND_INLINE void* Buffer::Data() const
    {
        return mData;
    }

    SND_INLINE void* Buffer::Last() const
    {
        return mData + (Capacity64() - 1) * mElementSize;
    }

    SND_INLINE u64 Buffer::Size() const
    {
        return Capacity64() * mElementSize;
    }

    SND_INLINE u32 Buffer::Capacity() const
    {
        return mCapacity;
    }

    SND_INLINE u64 Buffer::Capacity64() const
    {
        return mCapacity;
    }

    SND_INLINE u16 Buffer::ElementSize() const
    {
        return mElementSize;
    }
}
