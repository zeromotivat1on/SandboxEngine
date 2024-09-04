#include "sndpch.h"
#include "StringId.h"
#include "Log.h"

static std::unordered_map<snd::sid_hash, std::string> s_StringIdTable;

snd::StringId::StringId(const char* str)
    : m_Hash(sid_details::SidHash(str))
{
    s_StringIdTable.try_emplace(m_Hash, str);
}

snd::sid_hash snd::StringId::Id() const
{
    return m_Hash;
}

const char* snd::StringId::String() const
{
    return s_StringIdTable.at(m_Hash).c_str();
}
