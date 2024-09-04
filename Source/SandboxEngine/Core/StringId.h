#pragma once

#define SID(str) ::snd::StringId(str)

namespace snd
{
    using sid_hash = uint64_t;
    
    class StringId
    {
    public:
        StringId(const char* str);

        sid_hash Id() const;
        const char* String() const;

        friend bool operator==(StringId a, StringId b)                  { return a.m_Hash == b.m_Hash; }
        friend bool operator==(uint32_t a, const StringId& b)           { return a == b.m_Hash; }
        friend bool operator==(const StringId& a, uint32_t b)           { return a.m_Hash == b; }
        
        friend bool operator!=(const StringId& a, const StringId& b)    { return !(a == b); }
        friend bool operator!=(uint32_t a, const StringId& b)           { return !(a == b); }
        friend bool operator!=(const StringId& a, uint32_t b)           { return !(a == b); }
        
    private:
        sid_hash m_Hash = 0;
    };

    // String id hash details.
    namespace sid_details
    {
        constexpr sid_hash g_FnvBasis = 14695981039346656037ull;
        constexpr sid_hash g_FnvPrime = 1099511628211ull;

        // FNV-1a 64 bit hash.
        constexpr sid_hash SidHash(const char* str, sid_hash hash = g_FnvBasis)
        {
            return *str ? SidHash(str + 1, (hash ^ *str) * g_FnvPrime) : hash;
        }
    }

    // Convenient custom literal to generate sid hash.
    constexpr sid_hash operator ""_sid(const char* str, size_t)
    {
        return sid_details::SidHash(str);
    }
}

