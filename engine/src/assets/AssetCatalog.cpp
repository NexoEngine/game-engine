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
     * @brief Constructs an empty AssetCatalog instance.
     *
     * Initializes a new AssetCatalog for managing assets.
     */
    AssetCatalog::AssetCatalog()
    {
    }

    /**
     * @brief Deletes an asset by its unique identifier from the catalog.
     *
     * If an asset with the provided ID exists, it is removed from the asset collection.
     *
     * @param id The unique identifier of the asset to be deleted.
     */
    void AssetCatalog::deleteAsset(AssetID id)
    {
        if (!m_assets.contains(id))
            return;
        m_assets.erase(id);
    }

    /**
     * @brief Deletes an asset from the catalog using a generic asset reference.
     *
     * Attempts to lock the provided asset reference. If the asset is valid (i.e., still accessible),
     * retrieves its unique identifier and removes it from the catalog.
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
     * @brief Retrieves an asset reference by its unique ID.
     *
     * Checks if an asset with the specified ID exists in the catalog. If found, returns a valid
     * GenericAssetRef to the asset; otherwise, returns a null reference.
     *
     * @param id The unique identifier of the asset.
     * @return GenericAssetRef A reference to the asset if present, or a null reference if not found.
     */
    GenericAssetRef AssetCatalog::getAsset(AssetID id) const
    {
        if (!m_assets.contains(id))
            return GenericAssetRef::null();
        return GenericAssetRef(m_assets.at(id));
    }

    /**
     * @brief Retrieves an asset by its specified location.
     *
     * This method searches the asset collection for an asset whose metadata location matches the provided location.
     * It returns a reference to the matching asset, or a null reference if no such asset is found.
     *
     * @param location The asset location to search for.
     * @return GenericAssetRef A reference to the found asset, or a null reference if none exists.
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
     * @brief Retrieves references for all assets in the catalog.
     *
     * Iterates over all stored assets and returns a vector containing a reference to each asset.
     *
     * @return A vector of GenericAssetRef objects representing all assets in the catalog.
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
     * @brief Returns a lazy-evaluated view of the asset collection as GenericAssetRef objects.
     *
     * This method creates a view of the internal asset map by retrieving its values and transforming each
     * asset into a GenericAssetRef. The returned view provides efficient, on-demand access to the assets,
     * reflecting any modifications to the underlying collection.
     *
     * @return A range view where each element is a GenericAssetRef corresponding to a registered asset.
     */
    std::ranges::view auto AssetCatalog::getAssetsView() const
    {
        return m_assets
               | std::views::values
               | std::views::transform([](const auto& asset) {
                   return GenericAssetRef(asset);
               });
    }

    /**
     * @brief Registers an asset in the catalog.
     *
     * If the provided asset pointer is valid, this function wraps the asset in a shared pointer,
     * updates its metadata with the specified location, and assigns it a new unique identifier if it
     * does not already have one. The asset is then added to the catalog, and a reference to it is returned.
     * If the asset pointer is null, the function returns a null asset reference.
     *
     * @param location The location metadata to assign to the asset.
     * @param asset Pointer to the asset to register. Ownership is transferred upon registration.
     * @return GenericAssetRef A reference to the registered asset, or a null reference if the asset pointer is invalid.
     */
    GenericAssetRef AssetCatalog::registerAsset(const AssetLocation& location, IAsset* asset)
    {
        if (!asset)
            return GenericAssetRef::null();
        auto shared_ptr = std::shared_ptr<IAsset>(asset);
        shared_ptr->m_metadata.location = location;
        if (shared_ptr->m_metadata.id.is_nil())
            shared_ptr->m_metadata.id = boost::uuids::random_generator()();
        m_assets[shared_ptr->m_metadata.id] = shared_ptr;
        return GenericAssetRef(shared_ptr);
    }

} // namespace nexo::assets
