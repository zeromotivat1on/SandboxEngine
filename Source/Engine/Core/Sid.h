#pragma once

#define SID(str) snd::sid(str)

namespace snd
{
    // String identifier hash type.
    using sid_hash = u64;

    // Global string identifier table.
    inline std::unordered_map<sid_hash, const char*> gSidTable;

    // Unique string identifier.
    class sid
    {
    public:
                        sid(const char* str);

        bool            operator==(const sid& other) const;
        bool            operator==(sid_hash hash) const;

        bool            operator!=(const sid& other) const;
        bool            operator!=(sid_hash hash) const;

        sid_hash        Id() const;
        const char*     String() const;

    private:
        sid_hash        mHash;
    };

    // String hash details.
    namespace sid_details
    {
        constexpr u64 gFnvBasis = 14695981039346656037ull;
        constexpr u64 gFnvPrime = 1099511628211ull;

        // FNV-1a 64 bit hash.
        constexpr u64 SidHashFnv(const char* str, u64 hash = gFnvBasis)
        {
            return *str ? SidHashFnv(str + 1, (hash ^ *str) * gFnvPrime) : hash;
        }
    }

    // Convenient custom literal to generate sid hash.
    SND_INLINE constexpr sid_hash operator ""_sid(const char* str, size_t)
    {
        return sid_details::SidHashFnv(str);
    }

    // Check if given string is not stored in global table.
    SND_INLINE bool SidUnique(const char* str)
    {
        const sid_hash hash = sid_details::SidHashFnv(str);
        return gSidTable.find(hash) == gSidTable.end();
    }

    // sid

    SND_INLINE sid::sid(const char* str)
        : mHash(sid_details::SidHashFnv(str))
    {
        gSidTable.try_emplace(mHash, str);
    }

    SND_INLINE sid_hash sid::Id() const
    {
        return mHash;
    }

    SND_INLINE const char* sid::String() const
    {
        return gSidTable.at(mHash);
    }

    SND_INLINE bool sid::operator==(const sid& other) const
    {
        return mHash == other.mHash;
    }

    SND_INLINE bool sid::operator==(sid_hash hash) const
    {
        return mHash == hash;
    }

    SND_INLINE bool sid::operator!=(const sid& other) const
    {
        return !(*this == other);
    }

    SND_INLINE bool sid::operator!=(sid_hash hash) const
    {
        return !(*this == hash);
    }
}

