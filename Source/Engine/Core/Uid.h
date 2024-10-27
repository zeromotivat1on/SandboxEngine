#pragma once

// Generate completely new uid.
#define UID()   snd::uid()

// Generate uid based on input, determined.
#define UID(n)  snd::uid(n)

namespace snd
{
    // Unique identifier hash type.
    using uid_hash = u64;

    // Unique numeric identifier.
    class uid
    {
    public:
                    uid();
        explicit    uid(u64 input);

        bool        operator==(const uid& other) const;
        bool        operator==(uid_hash hash) const;

        bool        operator!=(const uid& other) const;
        bool        operator!=(uid_hash hash) const;

        uid_hash    Id() const;

    private:
        uid_hash    mHash;
    };

    // Numeric hash details.
    namespace uid_details
    {
        inline u64 g_HashPcgCounter = 1;

        constexpr u32 UidHashPcg32(u32 input)
        {
            u32 state = input * 747796405u + 2891336453u;
			u32 word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
			return (word >> 22u) ^ word;
        }

        constexpr u64 UidHashPcg64(u64 input)
        {
            return UidHashPcg32(input) + UidHashPcg32(input >> 32);
        }
    }

    // uid

    SND_INLINE uid::uid()
    {
        const u64 ctime = (u64)(CurrentTime() << 32);
        const u64 stime = (u64)(TimeSinceSystemBoot() << 8);
        const u64 gpcgc = uid_details::g_HashPcgCounter++;

        mHash = uid_details::UidHashPcg64(ctime + stime + gpcgc);
    }

    SND_INLINE uid::uid(u64 input)
        : mHash(uid_details::UidHashPcg64(input))
    {
    }

    SND_INLINE uid_hash uid::Id() const
    {
        return mHash;
    }

    SND_INLINE bool uid::operator==(const uid& other) const
    {
        return mHash == other.mHash;
    }

    SND_INLINE bool uid::operator==(uid_hash hash) const
    {
        return mHash == hash;
    }

    SND_INLINE bool uid::operator!=(const uid& other) const
    {
        return !(*this == other);
    }

    SND_INLINE bool uid::operator!=(uid_hash hash) const
    {
        return !(*this == hash);
    }
}

namespace std
{
    template <typename T> struct hash;

    template<>
    struct hash<snd::uid>
    {
        std::size_t operator()(const snd::uid& uid) const noexcept
        {
            return uid.Id();
        }
    };

}
