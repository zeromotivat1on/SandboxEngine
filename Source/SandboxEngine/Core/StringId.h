#pragma once

#include "SandboxEngine/Core/CoreMacros.h"

#define SID(str) ::snd::StringId(str)

namespace snd
{
    // StringId hash type.
    using sid_hash = uint64_t;

    // Global string id table.
    inline std::unordered_map<sid_hash, std::string> g_StringIdTable;

    class StringId
    {
    public:
                            StringId(const char* str);

        bool                operator==(const StringId& other) const;
        bool                operator==(sid_hash hash) const;
        
        bool                operator!=(const StringId& other) const;
        bool                operator!=(sid_hash hash) const;

        sid_hash            Id() const;
        const char*         String() const;
        
    private:
        sid_hash            m_Hash;
    };

    // String id hash details.
    namespace sid_details
    {
        constexpr sid_hash g_FnvBasis = 14695981039346656037ull;
        constexpr sid_hash g_FnvPrime = 1099511628211ull;

        // FNV-1a 64 bit hash.
        constexpr sid_hash SidHashFnv(const char* str, sid_hash hash = g_FnvBasis)
        {
            return *str ? SidHashFnv(str + 1, (hash ^ *str) * g_FnvPrime) : hash;
        }
    }

    // Convenient custom literal to generate sid hash.
    SND_INLINE constexpr sid_hash operator ""_sid(const char* str, size_t)
    {
        return sid_details::SidHashFnv(str);
    }

    // StringId
    
    SND_INLINE StringId::StringId(const char* str)
        : m_Hash(sid_details::SidHashFnv(str))
    {
        g_StringIdTable.try_emplace(m_Hash, str);
    }

    SND_INLINE sid_hash StringId::Id() const
    {
        return m_Hash;
    }

    SND_INLINE const char* StringId::String() const
    {
        return g_StringIdTable.at(m_Hash).c_str();
    }

    SND_INLINE bool StringId::operator==(const StringId& other) const
    {
        return m_Hash == other.m_Hash;
    }

    SND_INLINE bool StringId::operator==(sid_hash hash) const
    {
        return m_Hash == hash;
    }
    
    SND_INLINE bool StringId::operator!=(const StringId& other) const
    {
        return !(*this == other);
    }

    SND_INLINE bool StringId::operator!=(sid_hash hash) const
    {
        return !(*this == hash);
    }
}

