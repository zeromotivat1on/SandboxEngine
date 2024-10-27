#include "sndpch.h"
#include "Engine/Filesystem/AssetRegistry.h"
#include "Engine/Filesystem/Filesystem.h"

namespace snd::filesystem
{
    AssetRegistry::AssetRegistry()
    {
        m_Registry.reserve(1024);
    }

    AssetHandle AssetRegistry::Store(const char* filepath, AssetType type)
    {
        switch (type)
        {
            case AssetType::Shader: return StoreShader(filepath);
            default:                return INVALID_ASSET_HANDLE;
        }
    }

    AssetHandle AssetRegistry::StoreShader(const char* filepath)
    {
        const AssetHandle handle = UID(ReadShader(filepath).idx);
        m_Registry.try_emplace(handle, filepath, AssetType::Shader);
        return handle;
    }

    Asset* AssetRegistry::Request(AssetHandle handle)
    {
        if (auto it = m_Registry.find(handle); it != m_Registry.end())
        {
            return &it->second;
        }

        return nullptr;
    }
}
