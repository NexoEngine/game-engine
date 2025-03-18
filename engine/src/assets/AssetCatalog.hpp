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
 * The AssetCatalog destructor is defaulted to automatically manage resource cleanup,
 * ensuring that all member assets and internal data structures are properly released.
 */
~AssetCatalog() = default;

        public:
            /**
             * @brief Retrieves the singleton instance of the AssetCatalog.
             *
             * This method implements Meyers' Singleton Pattern, ensuring that a single instance
             * of AssetCatalog exists throughout the application's lifetime. The instance is lazily
             * initialized on its first invocation.
             *
             * @return AssetCatalog& A reference to the singleton instance.
             */
            static AssetCatalog& getInstance()
            {
                static AssetCatalog s_instance;
                return s_instance;
            }

            /**
 * @brief Deleted copy constructor for AssetCatalog.
 *
 * Prevents copying of the singleton instance to enforce unique access.
 */
            AssetCatalog(AssetCatalog const&)   = delete;
            /**
 * @brief Deleted copy assignment operator.
 *
 * Disallows assignment of the AssetCatalog singleton to ensure only a single instance exists.
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
     * @brief Retrieves all assets of the specified derived type.
     *
     * This template method iterates over all stored assets and attempts to filter those that
     * match the type identifier defined by the asset type (AssetType::TYPE). Note that asset type
     * filtering is not implemented yet; invoking this function will trigger a static assertion.
     *
     * @tparam AssetType The asset type to filter by. Must derive from IAsset.
     * @return std::vector<AssetRef<AssetType>> A vector of asset references of the specified type.
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
     * @brief Returns a view of assets cast to the specified type.
     *
     * This template method filters the asset catalog to produce a view of assets that are expected
     * to be of the type specified by the template parameter. Each asset is statically cast to the desired type.
     * 
     * @tparam AssetType The asset type to filter for, which must derive from IAsset.
     * @return A view of shared pointers to assets of type AssetType.
     *
     * @note Filtering by asset type is currently a placeholder and is not fully implemented.
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
