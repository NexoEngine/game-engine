//// AssetCatalog.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        03/12/2024
//  Description: Header file for the AssetCatalog class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AssetImporter.hpp"
#include "AssetLocation.hpp"
#include "Asset.hpp"

#include <unordered_map>
#include <memory>

namespace nexo::assets {

    /**
    * @class AssetCatalog
    *
    * @brief Singleton class that holds all the assets in the engine.
    */
    class AssetCatalog {
        private:
            // Singleton: private constructor and destructor
            AssetCatalog();
            /**
 * @brief Default destructor for AssetCatalog.
 *
 * Uses the default implementation since AssetCatalog relies on automatic
 * resource management via smart pointers.
 */
~AssetCatalog() = default;

        public:
            /**
             * @brief Retrieves the singleton instance of the AssetCatalog.
             *
             * Implements Meyers' Singleton Pattern, ensuring that only one instance of
             * AssetCatalog exists throughout the application's lifetime. The unique
             * instance is created on the first call and returned in subsequent calls.
             *
             * @return AssetCatalog& Reference to the singleton AssetCatalog instance.
             */
            static AssetCatalog& getInstance()
            {
                static AssetCatalog s_instance;
                return s_instance;
            }

            /**
 * @brief Deleted copy constructor to enforce singleton semantics.
 *
 * The copy constructor is explicitly deleted to prevent copying of the AssetCatalog instance,
 * ensuring that only one instance exists throughout the application.
 */
            AssetCatalog(AssetCatalog const&)   = delete;
            /**
 * @brief Deleted copy assignment operator.
 *
 * This operator is explicitly deleted to prevent assignment of the AssetCatalog instance,
 * thereby enforcing the Singleton design pattern.
 */
void operator=(AssetCatalog const&) = delete;

            /**
             * @brief Delete an asset from the catalog.
             * @param id The ID of the asset to delete.
             */
            void deleteAsset(AssetID id);

            /**
             * @brief Delete an asset from the catalog.
             * @param asset The asset to delete.
             */
            void deleteAsset(const GenericAssetRef& asset);

            /**
             * @brief Get an asset by its ID.
             * @param id The ID of the asset to get.
             * @return A reference to the asset, or a null reference if the asset does not exist.
             */
            [[nodiscard]] GenericAssetRef getAsset(AssetID id) const;

            /**
             * @brief Get an asset by its location.
             * @param location The location of the asset to get.
             * @return A reference to the asset, or a null reference if the asset does not exist.
             */
            [[nodiscard]] GenericAssetRef getAsset(const AssetLocation& location) const;

            /**
             * @brief Get all assets in the catalog.
             * @return A vector of all assets in the catalog.
             */
            [[nodiscard]] std::vector<GenericAssetRef> getAssets() const;

            /**
             * @breif Get all assets in the catalog as a view.
             * @return A view of all assets in the catalog.
             */
            [[nodiscard]] std::ranges::view auto getAssetsView() const;

            /**
             * @breif Get all assets of a specific type in the catalog.
             * @tparam AssetType The type of asset to get. (e.g. Model, Texture)
             * @return A vector of all assets of the specified type in the catalog.
             */
            template <typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            [[nodiscard]] std::vector<AssetRef<AssetType>> getAssetsOfType() const;

            /**
             * @brief Get all assets of a specific type in the catalog as a view.
             * @tparam AssetType The type of asset to get. (e.g. Model, Texture)
             * @return A view of all assets of the specified type in the catalog.
             */
            template <typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            [[nodiscard]] std::ranges::view auto getAssetsOfTypeView() const;

            GenericAssetRef registerAsset(const AssetLocation& location, IAsset *asset);
        private:
            std::unordered_map<AssetID, std::shared_ptr<IAsset>> m_assets;
    };

    template<typename AssetType>
        requires std::derived_from<AssetType, IAsset>
    /**
     * @brief Retrieves asset references of a specified type.
     *
     * Iterates over all assets in the catalog and returns those whose type, determined by IAsset::getType(),
     * matches the placeholder type identifier AssetType::TYPE. Note that the filtering logic is not fully
     * implemented, as indicated by the static assertion.
     *
     * @tparam AssetType The asset type to retrieve; must derive from IAsset.
     * @return A vector of asset references of type AssetType.
     */
    std::vector<AssetRef<AssetType>> AssetCatalog::getAssetsOfType() const
    {
        // TODO: AssetType::TYPE is not a thing, need to find a way to get the type of the asset
        static_assert(true, "Filtering not implemented yet");
        std::vector<AssetRef<AssetType>> assets;
        for (const auto& asset: m_assets | std::views::values) {
            if (asset->getType() == AssetType::TYPE)
                assets.emplace_back(std::static_pointer_cast<AssetType>(asset));
        }
        return assets;
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    /**
     * @brief Returns a view of assets filtered by a specific asset type.
     *
     * This template method is intended to provide a view over those assets in the catalog
     * that match the specified type (AssetType), where AssetType must derive from IAsset.
     * Filtering is based on comparing each asset's type with AssetType::TYPE. However, note
     * that the filtering functionality is currently not implemented.
     *
     * @tparam AssetType The asset type used for filtering; must derive from IAsset.
     *
     * @return A range view of assets, transformed to AssetType.
     *
     * @todo Implement proper filtering logic to identify assets of the correct type.
     */
    std::ranges::view auto AssetCatalog::getAssetsOfTypeView() const
    {
        // TODO: AssetType::TYPE is not a thing, need to find a way to get the type of the asset
        static_assert(true, "Filtering not implemented yet");
        return m_assets
               | std::views::values
               | std::views::filter([](const auto& asset) {
                   return asset->getType() == AssetType::TYPE;
               })
               | std::views::transform([](const auto& asset) {
                   return std::static_pointer_cast<AssetType>(asset);
               });
    }

} // namespace nexo::assets
