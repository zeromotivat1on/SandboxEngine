#pragma once

#define INVALID_ASSET_HANDLE (AssetHandle)INDEX_NONE

using AssetHandle = uid;

enum class AssetType : u8
{
    None    = 0,
    Shader  = 1,
    Texture = 2,
    Model   = 3,

    Count
};

struct Asset
{
                    Asset();
    explicit        Asset(const char* path, AssetType type);

    const char*     Path;
    AssetType       Type;

    bool            Valid() const;
};

// Asset

inline Asset::Asset()
    : Path(nullptr), Type(AssetType::None)
{
}

inline Asset::Asset(const char* path, AssetType type)
    : Path(path), Type(type)
{
}

inline bool Asset::Valid() const
{
    return Path && Type != AssetType::None;
}
