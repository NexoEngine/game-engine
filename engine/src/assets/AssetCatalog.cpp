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

namespace nexo::assets {
    AssetCatalog::AssetCatalog()
    {
        setupImporterInstances();
        auto asset = createEmptyAsset<Model>(AssetLocation("/test.model"));
        if (auto assetData = asset.lock()) {
            if (!assetData->isLoaded())
                assetData->data = new ModelData();
            assetData->data->scene = nullptr;
        }
    }

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

    std::vector<GenericAssetRef> AssetCatalog::getAssets() const
    {
        std::vector<GenericAssetRef> assets;
        for (const auto& [id, asset] : m_assets) {
            assets.emplace_back(asset);
        }
        return assets;
    }

    GenericAssetRef AssetCatalog::importAsset(const AssetLocation& location, const std::filesystem::path& fsPath)
    {
        for (const auto& [type, importer] : m_importers) {
            if (importer->canRead(fsPath)) {
                IAsset *asset = importer->import(fsPath);
                if (asset->getID().is_nil())
                    asset->m_metadata.id = boost::uuids::random_generator()();
                asset->m_metadata.location = location;

                const std::shared_ptr<IAsset> shared_ptr(asset);
                m_assets[asset->getID()] = shared_ptr;
                return GenericAssetRef(shared_ptr);
            }
        }
        return GenericAssetRef::null();
    }

    void AssetCatalog::setupImporterInstances()
    {
        registerImporter<Model, ModelImporter>();
    }

} // namespace nexo::assets
