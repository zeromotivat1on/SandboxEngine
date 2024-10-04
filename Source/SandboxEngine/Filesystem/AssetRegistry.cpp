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
            case AssetType::Shader: return StoreShader(filepath);
            default:                return INVALID_ASSET_HANDLE;
        }
    }

    AssetHandle AssetRegistry::StoreShader(const char* filepath)
    {
        const AssetType shaderType = AssetType::Shader;
        const AssetHandle handle = AssetHandle(ReadShader(filepath).idx, shaderType);
        new (AssetBuffer(shaderType).Get(handle.Index)) Asset(filepath, shaderType);
        return handle;
    }

    Asset* AssetRegistry::Request(AssetHandle handle)
    {
        if (void* data = AssetBuffer(handle.Type).Get(handle.Index))
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
