#include "sndpch.h"
#include "SandboxEngine/Filesystem/AssetRegistry.h"
#include "SandboxEngine/Filesystem/Filesystem.h"

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
        const Asset asset = Asset(filepath, AssetType::Shader);
        m_Registry.try_emplace(handle, asset);
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
