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

namespace nexo::assets {


    /**
    * @class AssetCatalog
    *
    * @brief Singleton class that holds all the assets in the engine.
    */
    class AssetCatalog {
        public:
            AssetCatalog()
            {
                setupImporterInstances();
                auto asset = createEmptyAsset<Model>(AssetLocation("/test.model"));
                if (auto assetData = asset.lock()) {
                    if (!assetData->isLoaded())
                        assetData->data = new ModelData();
                    assetData->data->scene = nullptr;
                }
            }

            /*void addAsset(AssetID id, AssetRef<Asset> asset);
            void removeAsset(AssetID id);
            void removeAsset(AssetRef<Asset> asset);

            [[nodiscard]] AssetRef<Asset> getAsset(AssetID id) const;
            [[nodiscard]] AssetRef<Asset> getAsset(const std::string &name) const;

            [[nodiscard]] bool hasAsset(AssetID id) const;
            [[nodiscard]] bool hasAsset(const std::string &name) const;

            [[nodiscard]] std::size_t size() const;
*/
            /*void importAsset(std::string_view location);
            void importAsset(const AssetLocation& location);*/

            template <typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            AssetRef<AssetType> importAsset(const AssetLocation& location, const std::filesystem::path& fsPath)
            {
                const auto importer = m_importers.at(std::type_index(typeid(AssetType)));
                IAsset *asset = importer->import(fsPath);
                if (asset->getID().is_nil())
                    asset->m_metadata.id = boost::uuids::random_generator()();
                asset->m_metadata.location = location;

                auto shared_ptr = std::make_shared<AssetType>(importer->import(fsPath));
                m_assets[asset->getID()] = shared_ptr;
                return AssetRef<AssetType>(shared_ptr);
            }

            template <typename AssetType, typename... Args>
                requires std::derived_from<AssetType, IAsset>
            AssetRef<AssetType> createEmptyAsset(const AssetLocation& location, Args&&... args)
            {
                auto shared_ptr = std::make_shared<AssetType>(std::forward<Args>(args)...);
                shared_ptr->m_metadata.location = location;
                return AssetRef<AssetType>(shared_ptr);
            }

        private:

            void setupImporterInstances()
            {
                registerImporter<Model, ModelImporter>();
            }
            ~AssetCatalog() = default;

            template <typename AssetType, typename TAssetImporter>
            void registerImporter()
            {
                auto importer = std::make_shared<TAssetImporter>();
                m_importers[std::type_index(typeid(AssetType))] = importer;
            }

            std::unordered_map<AssetID, std::shared_ptr<IAsset>> m_assets;
            //std::unordered_map<AssetLocation, AssetID> m_assetIdByLocation;

            std::unordered_map<std::type_index, std::shared_ptr<AssetImporter>> m_importers;


    };


} // namespace nexo::assets
