#ifndef ASSET_H
#define ASSET_H

typedef unsigned long long AssetHandle;

class Asset
{
public:
    Asset();
    virtual ~Asset() = default;

    AssetHandle assetHandle_;

    std::string path_;
};

#endif // !ASSET_H
