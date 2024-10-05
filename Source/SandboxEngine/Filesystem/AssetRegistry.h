#pragma once

#include "SandboxEngine/Filesystem/Asset.h"

namespace snd::filesystem
{
    using AssetTable = std::unordered_map<AssetHandle, Asset>;

    // General purpose class for managing different types of assets.
    class AssetRegistry
    {
    public:
                        AssetRegistry();

        // Load and save given file in registry.
        AssetHandle     Store(const char* filepath, AssetType type);
        AssetHandle     StoreShader(const char* filepath);

        // Obtain asset from registry.
        Asset*          Request(AssetHandle handle);

    private:
        AssetTable      m_Registry;
    };
}
