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
     * @brief Removes an asset from the catalog by its identifier.
     *
     * If an asset with the specified AssetID exists, it is removed from the asset collection.
     * If no such asset exists, the function does nothing.
     *
     * @param id The unique identifier of the asset to remove.
     */
    void AssetCatalog::deleteAsset(AssetID id)
    {
        if (!m_assets.contains(id))
            return;
        m_assets.erase(id);
    }

    /**
     * @brief Deletes an asset using its generic reference.
     *
     * Attempts to lock the provided asset reference to access the underlying asset data.
     * If successful, retrieves the asset's ID and deletes the asset by invoking the ID-based deletion method.
     *
     * @param asset A generic reference to the asset to be deleted.
     */
    void AssetCatalog::deleteAsset(const GenericAssetRef& asset)
    {
        if (const auto assetData = asset.lock()) {
            deleteAsset(assetData->getID());
        }
    }

    /**
     * @brief Retrieves an asset from the catalog by its ID.
     *
     * Checks if an asset with the specified ID exists in the catalog and returns a reference
     * to it. If the asset is not found, a null reference is returned.
     *
     * @param id The unique identifier of the asset.
     * @return GenericAssetRef A reference to the asset if found; otherwise, a null reference.
     */
    GenericAssetRef AssetCatalog::getAsset(AssetID id) const
    {
        if (!m_assets.contains(id))
            return GenericAssetRef::null();
        return GenericAssetRef(m_assets.at(id));
    }

    /**
     * @brief Retrieves an asset that matches the specified location.
     *
     * Iterates over the catalog's assets and returns a reference to the first asset whose metadata
     * location is equal to the provided location. If no matching asset is found, a null asset reference
     * is returned.
     *
     * @param location The location identifier to search for.
     * @return GenericAssetRef A reference to the asset with the matching location, or a null reference if not found.
     */
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
     * @brief Retrieves all asset references from the catalog.
     *
     * Iterates over the stored assets and compiles them into a vector, returning a reference to each asset.
     *
     * @return std::vector<GenericAssetRef> A vector containing references to all assets in the catalog.
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
     * @brief Registers an asset with the specified location.
     *
     * Associates an asset with a given location and ensures it has a unique identifier. If the asset's identifier is not set,
     * the function generates a new UUID. The asset is then added to the internal catalog. If the asset pointer is null,
     * this function returns a null asset reference.
     *
     * @param location The metadata location to assign to the asset.
     * @param asset Pointer to the asset to register.
     * @return GenericAssetRef A reference to the registered asset, or a null reference if the asset pointer is null.
     *
     * @note Duplicate asset handling is not implemented yet.
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
