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

#ifdef NEXO_TESTING
class AssetCatalogTest;
#endif

namespace nexo::assets {

    /**
    * @class AssetCatalog
    *
    * @brief Singleton class that holds all the assets in the engine.
    */
    class AssetCatalog {
#ifdef NEXO_TESTING
        friend class AssetCatalogTest;
#endif

        private:
            // Singleton: private constructor and destructor
            AssetCatalog() = default;
            ~AssetCatalog() = default;

        public:
            // Singleton: Meyers' Singleton Pattern
            static AssetCatalog& getInstance()
            {
                static AssetCatalog s_instance;
                return s_instance;
            }

            // Singleton: delete copy constructor and assignment operator
            AssetCatalog(AssetCatalog const&)   = delete;
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
             * @brief Get all assets in the catalog as a view.
             * @return A view of all assets in the catalog.
             */
            [[nodiscard]] auto getAssetsView() const
            {
                return m_assets
                    | std::views::values
                    | std::views::transform([](const auto& asset) {
                        return GenericAssetRef(asset);
                    });
            }

            /**
             * @brief Get all assets of a specific type in the catalog.
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
