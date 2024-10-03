#pragma once

#define INVALID_ASSET_HANDLE    INVALID_UINDEX

namespace snd::filesystem
{
    using AssetHandle = u16;

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

    SND_INLINE Asset::Asset()
        : Path(nullptr), Type(AssetType::None)
    {
    }

    SND_INLINE Asset::Asset(const char* path, AssetType type)
        : Path(path), Type(type)
    {
    }

    SND_INLINE bool Asset::Valid() const
    {
        return Path && Type != AssetType::None && Type != AssetType::Count;
    }
}
