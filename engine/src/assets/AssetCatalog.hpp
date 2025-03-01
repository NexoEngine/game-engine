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
             * @breif Get all assets of a specific type in the catalog.
             * @tparam AssetType The type of asset to get. (e.g. Model, Texture)
             * @return A vector of all assets of the specified type in the catalog.
             */
            template <typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            [[nodiscard]] std::vector<AssetRef<AssetType>> getAssetsOfType() const;

            template <typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            AssetRef<AssetType> importAsset(const AssetLocation& location, const std::filesystem::path& fsPath);

            GenericAssetRef importAsset(const AssetLocation& location, const std::filesystem::path& fsPath);

            template <typename AssetType, typename... Args>
                requires std::derived_from<AssetType, IAsset>
            AssetRef<AssetType> createEmptyAsset(const AssetLocation& location, Args&&... args);

        private:
            void setupImporterInstances();

            template <typename AssetType, typename TAssetImporter>
                requires std::derived_from<AssetType, IAsset>
                      && std::derived_from<TAssetImporter, AssetImporter>
            void registerImporter();

            std::unordered_map<AssetID, std::shared_ptr<IAsset>> m_assets;
            std::unordered_map<std::type_index, std::shared_ptr<AssetImporter>> m_importers;
    };

    template<typename AssetType>
        requires std::derived_from<AssetType, IAsset>
    std::vector<AssetRef<AssetType>> AssetCatalog::getAssetsOfType() const
    {
        std::vector<AssetRef<AssetType>> assets;
        for (const auto& [id, asset] : m_assets) {
            if (asset->getType() == AssetType::TYPE)
                assets.emplace_back(std::static_pointer_cast<AssetType>(asset));
        }
        return assets;
    }

    template<typename AssetType>
        requires std::derived_from<AssetType, IAsset>
    AssetRef<AssetType> AssetCatalog::importAsset(const AssetLocation& location, const std::filesystem::path& fsPath)
    {
        const auto importer = m_importers.at(std::type_index(typeid(AssetType)));
        IAsset *asset = importer->import(fsPath);
        if (asset->getID().is_nil())
            asset->m_metadata.id = boost::uuids::random_generator()();
        asset->m_metadata.location = location;

        std::shared_ptr<AssetType> shared_ptr(dynamic_cast<AssetType*>(asset));
        assert(shared_ptr != nullptr);
        m_assets[asset->getID()] = shared_ptr;
        return AssetRef<AssetType>(shared_ptr);
    }

    template<typename AssetType, typename ... Args>
        requires std::derived_from<AssetType, IAsset>
    AssetRef<AssetType> AssetCatalog::createEmptyAsset(const AssetLocation& location, Args&&... args)
    {
        auto shared_ptr = std::make_shared<AssetType>(std::forward<Args>(args)...);
        shared_ptr->m_metadata.location = location;
        return AssetRef<AssetType>(shared_ptr);
    }

    template<typename AssetType, typename TAssetImporter>
        requires std::derived_from<AssetType, IAsset>
              && std::derived_from<TAssetImporter, AssetImporter>
    void AssetCatalog::registerImporter()
    {
        auto importer = std::make_shared<TAssetImporter>();
        m_importers[std::type_index(typeid(AssetType))] = importer;
    }

} // namespace nexo::assets
