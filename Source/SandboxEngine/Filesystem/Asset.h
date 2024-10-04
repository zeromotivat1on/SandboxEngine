#pragma once

#define INVALID_ASSET_INDEX     INVALID_UINDEX
#define INVALID_ASSET_HANDLE    AssetHandle(INVALID_ASSET_INDEX, snd::filesystem::AssetType::None)

namespace snd::filesystem
{
    enum class AssetType : u8
    {
        None    = 0,
        Shader  = 1,
        Texture = 2,
        Model   = 3,

        Count
    };

    struct AssetHandle
    {
                        AssetHandle();
        explicit        AssetHandle(u16 index, AssetType type);

        u16             Index;
        AssetType       Type;

        bool            Valid() const;
    };

    struct Asset
    {
                        Asset();
        explicit        Asset(const char* path, AssetType type);

        const char*     Path;
        AssetType       Type;

        bool            Valid() const;
    };

    // AssetHandle

    SND_INLINE AssetHandle::AssetHandle()
        : Index(INVALID_UINDEX), Type(AssetType::None)
    {
    }

    SND_INLINE AssetHandle::AssetHandle(u16 index, AssetType type)
        : Index(index), Type(type)
    {
    }

    SND_INLINE bool AssetHandle::Valid() const
    {
        return Index != INVALID_ASSET_INDEX && Type != AssetType::Count;
    }

    // Asset

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
