//// AssetCatalog.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        01/03/2025
//  Description: Source file for the AssetCatalog class
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetCatalog.hpp"

#include <boost/uuid/random_generator.hpp>

namespace nexo::assets {

    bool AssetCatalog::deleteAsset(const AssetID id)
    {
        if (!m_assets.contains(id)) return false;
        m_assets.erase(id);
        return true;
    }

    bool AssetCatalog::deleteAsset(const GenericAssetRef& asset)
    {
        if (const auto assetData = asset.lock()) {
            return deleteAsset(assetData->getID());
        }
        return false;
    }

    bool AssetCatalog::renameAsset(const AssetID id, const std::string& newName) const
    {
        if (!m_assets.contains(id) || newName.empty()) return false;
        const auto asset = m_assets.at(id);
        asset->m_metadata.location.setName(newName);
        return true;
    }

    bool AssetCatalog::renameAsset(const GenericAssetRef& asset, const std::string& newName) const
    {
        if (const auto assetData = asset.lock()) return renameAsset(assetData->getID(), newName);
        return false;
    }

    void AssetCatalog::moveAsset(const GenericAssetRef& asset, const std::string& path) const
    {
        if (const auto assetData = asset.lock()) moveAsset(assetData->getID(), path);
    }

    void AssetCatalog::moveAsset(const AssetID id, const std::string& path) const
    {
        if (!m_assets.contains(id)) return;
        const auto asset = m_assets.at(id);
        asset->m_metadata.location.setPath(path);
    }

    GenericAssetRef AssetCatalog::getAsset(const AssetID id) const
    {
        if (!m_assets.contains(id)) return GenericAssetRef::null();
        return GenericAssetRef(m_assets.at(id));
    }

    GenericAssetRef AssetCatalog::getAsset(const AssetLocation& location) const
    {
        // TODO: implement a tree for folders and assets instead of doing O(n) search
        for (const auto& asset : m_assets | std::views::values) {
            if (asset->m_metadata.location == location) return GenericAssetRef(asset);
        }
        return GenericAssetRef::null();
    }

    std::vector<GenericAssetRef> AssetCatalog::getAssets() const
    {
        std::vector<GenericAssetRef> assets;
        for (const auto& asset : m_assets | std::views::values) {
            assets.emplace_back(asset);
        }
        return assets;
    }

    GenericAssetRef AssetCatalog::registerAsset(const AssetLocation& location, std::unique_ptr<IAsset> asset)
    {
        if (!asset) return GenericAssetRef::null();
        // TODO: implement error handling if already exists (once we have the folder tree)
        const std::shared_ptr<IAsset> shared_ptr = std::move(asset);
        shared_ptr->m_metadata.location          = location;
        if (shared_ptr->m_metadata.id.is_nil()) shared_ptr->m_metadata.id = boost::uuids::random_generator()();
        m_assets[shared_ptr->m_metadata.id] = shared_ptr;
        return GenericAssetRef(shared_ptr);
    }

} // namespace nexo::assets
