#include "sndpch.h"
#include "SandboxEngine/Filesystem/AssetRegistry.h"
#include "SandboxEngine/Filesystem/Filesystem.h"

namespace snd::filesystem
{
    AssetRegistry::AssetRegistry()
    {
        for (u8 i = 0; i < static_cast<u8>(AssetType::Count); ++i)
        {
            m_Registry[i] = Buffer(1024, sizeof(Asset));
        }
    }

    AssetHandle AssetRegistry::Store(const char* filepath, AssetType type)
    {
        switch (type)
        {
            case AssetType::Shader:
            {
                const AssetHandle handle = ReadShader(filepath).idx;
                new (AssetBuffer(type).Get(handle)) Asset(filepath, type);
                return handle;
            }

            default: return INVALID_ASSET_HANDLE;
        }
    }

    Asset* AssetRegistry::Request(AssetHandle handle, AssetType type)
    {
        if (void* data = AssetBuffer(type).Get(handle))
        {
            return static_cast<Asset*>(data);
        }

        return nullptr;
    }
    
    Buffer& AssetRegistry::AssetBuffer(AssetType type)
    {
        return m_Registry[static_cast<u8>(type)];
    }
}
