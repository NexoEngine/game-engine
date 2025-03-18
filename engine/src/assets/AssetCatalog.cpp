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
     * @brief Constructs an AssetCatalog instance.
     *
     * Initializes an empty collection of assets.
     */
    AssetCatalog::AssetCatalog()
    {
    }

    /**
     * @brief Deletes an asset from the catalog by its identifier.
     *
     * Checks whether an asset with the specified @p id exists in the catalog. If found, the asset is removed from the internal collection.
     *
     * @param id The unique identifier of the asset to delete.
     */
    void AssetCatalog::deleteAsset(AssetID id)
    {
        if (!m_assets.contains(id))
            return;
        m_assets.erase(id);
    }

    /**
     * @brief Deletes an asset from the catalog using a weak reference.
     *
     * Attempts to lock the provided weak asset reference. If successful, retrieves the asset's ID
     * and deletes the asset from the catalog.
     *
     * @param asset A weak reference to the asset to be deleted.
     */
    void AssetCatalog::deleteAsset(const GenericAssetRef& asset)
    {
        if (const auto assetData = asset.lock()) {
            deleteAsset(assetData->getID());
        }
    }

    /**
     * @brief Retrieves an asset by its unique identifier.
     *
     * Checks if an asset with the provided identifier exists in the catalog.
     * If the asset is found, returns a GenericAssetRef to it; otherwise, returns a null reference.
     *
     * @param id The unique identifier of the asset to retrieve.
     * @return GenericAssetRef A reference to the asset if found, or a null reference if no matching asset exists.
     */
    GenericAssetRef AssetCatalog::getAsset(AssetID id) const
    {
        if (!m_assets.contains(id))
            return GenericAssetRef::null();
        return GenericAssetRef(m_assets.at(id));
    }

    /**
     * @brief Retrieves an asset by location.
     *
     * Searches the catalog for an asset whose metadata location matches the specified location.
     * Returns a reference to the found asset or a null reference if no such asset exists.
     *
     * @param location The location used to identify the asset.
     * @return GenericAssetRef Reference to the matching asset, or a null reference if not found.
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
     * @brief Retrieves all assets in the catalog.
     *
     * Iterates over the stored asset collection and compiles a vector of references to each asset.
     *
     * @return std::vector<GenericAssetRef> A vector of references to all registered assets.
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
     * @brief Returns a view of asset references for all assets in the catalog.
     *
     * This function creates a lazy view that transforms the internal asset collection into
     * a sequence of GenericAssetRef objects, enabling efficient iteration over the asset references.
     *
     * @return A view that yields GenericAssetRef for each asset in the catalog.
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
     * Transfers ownership of the provided asset into a managed shared pointer, sets its location metadata,
     * and assigns a new unique identifier if it does not already have one. The asset is then stored in the
     * internal asset collection. If the asset pointer is null, the function returns a null asset reference.
     *
     * @param location The asset's location metadata.
     * @param asset Pointer to the asset instance to register. Ownership is transferred if asset is non-null.
     * @return GenericAssetRef A reference to the registered asset, or a null reference if the provided asset is null.
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
