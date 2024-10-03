#pragma once

#include "SandboxEngine/Filesystem/Asset.h"

namespace snd::filesystem
{
    // General purpose class for managing different types of assets.
    // Stores buffers for each asset type.
    class AssetRegistry
    {
    public:
                        AssetRegistry();

        // Load and save given file in registry.
        AssetHandle     Store(const char* filepath, AssetType type);

        // Obtain asset from registry.
        Asset*          Request(AssetHandle handle, AssetType type);

    private:
        // Get asset buffer of given type.
        Buffer&         AssetBuffer(AssetType type);

    private:
        Buffer          m_Registry[static_cast<u8>(AssetType::Count)];  // asset buffers per type
    };
}
