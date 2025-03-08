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
#include "AssetImporterRegistry.hpp"
#include "AssetLocation.hpp"
#include "Asset.hpp"
#include "Assets/Model/Model.hpp"
#include "Assets/Model/ModelImporter.hpp"

#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <filesystem>

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

            template <typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            AssetRef<AssetType> importAsset(const AssetLocation& location, const ImporterInputVariant& inputVariant);


            GenericAssetRef importAsset(const AssetLocation& location, const ImporterInputVariant& inputVariant);
            GenericAssetRef importAssetUsingImporter(const AssetLocation& location, const ImporterInputVariant& inputVariant, const std::shared_ptr<AssetImporter>& importer);
            GenericAssetRef importAssetTryImporters(const AssetLocation& location, const ImporterInputVariant& inputVariant, const std::vector<std::shared_ptr<AssetImporter>>& importers);

            template <typename AssetType, typename... Args>
                requires std::derived_from<AssetType, IAsset>
            AssetRef<AssetType> createEmptyAsset(const AssetLocation& location, Args&&... args);

        private:
            void setupImporterInstances();

            std::unordered_map<AssetID, std::shared_ptr<IAsset>> m_assets;
            AssetImporterRegistry m_importers;
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

    template<typename AssetType>
        requires std::derived_from<AssetType, IAsset>
    AssetRef<AssetType> AssetCatalog::importAsset(const AssetLocation& location, const ImporterInputVariant& inputVariant)
    {
        auto importers = m_importers.getImportersForType<AssetType>();
        return importAssetTryImporters(location, inputVariant, importers).template as<AssetType>();
    }

    template<typename AssetType, typename ... Args>
        requires std::derived_from<AssetType, IAsset>
    AssetRef<AssetType> AssetCatalog::createEmptyAsset(const AssetLocation& location, Args&&... args)
    {
        auto shared_ptr = std::make_shared<AssetType>(std::forward<Args>(args)...);
        shared_ptr->m_metadata.location = location;
        m_assets[shared_ptr->getID()] = shared_ptr;
        return AssetRef<AssetType>(shared_ptr);
    }

} // namespace nexo::assets
