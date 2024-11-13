#include "pch.h"
#include "Engine/Filesystem/AssetRegistry.h"
#include "Engine/Filesystem/Filesystem.h"

AssetRegistry::AssetRegistry()
{
    mRegistry.reserve(1024);
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
    const AssetHandle handle = UID(file_shader_load(filepath).idx);
    mRegistry.try_emplace(handle, filepath, AssetType::Shader);
    return handle;
}

Asset* AssetRegistry::Request(AssetHandle handle)
{
    if (auto it = mRegistry.find(handle); it != mRegistry.end())
    {
        return &it->second;
    }

    return nullptr;
}
