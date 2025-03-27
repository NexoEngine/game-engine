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

#include <Path.hpp>

namespace nexo::assets {

    /**
     * @brief Removes the asset associated with the given ID from the catalog.
     *
     * Checks if an asset with the specified ID exists in the catalog and, if so,
     * deletes it.
     *
     * @param id The unique identifier of the asset to be removed.
     */
    void AssetCatalog::deleteAsset(AssetID id)
    {
        if (!m_assets.contains(id))
            return;
        m_assets.erase(id);
    }

    void AssetCatalog::deleteAsset(const GenericAssetRef& asset)
    {
        if (const auto assetData = asset.lock()) {
            deleteAsset(assetData->getID());
        }
    }

    GenericAssetRef AssetCatalog::getAsset(AssetID id) const
    {
        if (!m_assets.contains(id))
            return GenericAssetRef::null();
        return GenericAssetRef(m_assets.at(id));
    }

    GenericAssetRef AssetCatalog::getAsset(const AssetLocation& location) const
    {
        // TODO: implement a tree for folders and assets instead of doing O(n) search
        for (const auto& asset: m_assets | std::views::values) {
            if (asset->m_metadata.location == location)
                return GenericAssetRef(asset);
        }
        return GenericAssetRef::null();
    }

    /**
     * @brief Retrieves all asset references registered in the catalog.
     *
     * Iterates through the stored assets and collects them into a vector of GenericAssetRef objects.
     *
     * @return A vector containing a GenericAssetRef for each managed asset.
     */
    std::vector<GenericAssetRef> AssetCatalog::getAssets() const
    {
        std::vector<GenericAssetRef> assets;
        for (const auto& asset: m_assets | std::views::values) {
            assets.emplace_back(asset);
        }
        return assets;
    }

    /**
     * @brief Registers an asset in the catalog.
     *
     * This method verifies that the provided asset pointer is valid, then creates a shared pointer
     * to the asset. It updates the asset's metadata by setting its location and assigning a new unique
     * identifier if one is not already set. The asset is stored in the catalog and a reference to the
     * asset is returned.
     *
     * @param location The asset's location metadata.
     * @param asset Pointer to the asset to be registered.
     * @return GenericAssetRef A reference to the registered asset, or a null reference if the asset pointer was null.
     */
    GenericAssetRef AssetCatalog::registerAsset(const AssetLocation& location, IAsset* asset)
    {
        if (!asset)
            return GenericAssetRef::null();
        // TODO: implement error handling if already exists (once we have the folder tree)
        auto shared_ptr = std::shared_ptr<IAsset>(asset);
        shared_ptr->m_metadata.location = location;
        if (shared_ptr->m_metadata.id.is_nil())
            shared_ptr->m_metadata.id = boost::uuids::random_generator()();
        m_assets[shared_ptr->m_metadata.id] = shared_ptr;
        return GenericAssetRef(shared_ptr);
    }

} // namespace nexo::assets
