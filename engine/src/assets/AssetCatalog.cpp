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
    AssetCatalog::AssetCatalog()
    {
        setupImporterInstances();
        const auto asset = createEmptyAsset<Model>(AssetLocation("my_package::My_Model@foo/bar/"));
        if (const auto assetData = asset.lock()) {
            if (!assetData->isLoaded())
                assetData->data = new ModelData();
            assetData->data->scene = nullptr;
        }
        if (const auto assetData = asset.lock()) {
            m_assets[assetData->getID()] = assetData;
        }
        std::filesystem::path path = nexo::Path::resolvePathRelativeToExe("../assets/models/9mn/scene.gltf");
        importAsset<Model>(AssetLocation("my_package::9mn@foo/bar/"), ImporterFileInput{path});
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
        for (const auto& asset: m_assets | std::views::values) {
            assets.emplace_back(asset);
        }
        return assets;
    }

    std::ranges::view auto AssetCatalog::getAssetsView() const
    {
        return m_assets
               | std::views::values
               | std::views::transform([](const auto& asset) {
                   return GenericAssetRef(asset);
               });
    }

    GenericAssetRef AssetCatalog::importAsset(const AssetLocation& location, const ImporterInputVariant& inputVariant)
    {
        auto importersMap = m_importers.getImporters();
        for (const auto& importers: importersMap | std::views::values) {
            if (importers.empty())
                continue;
            if (const auto asset = importAssetTryImporters(location, inputVariant, importers))
                return asset;
        }
        return GenericAssetRef::null();
    }

    GenericAssetRef AssetCatalog::importAssetUsingImporter(const AssetLocation& location,
        const ImporterInputVariant& inputVariant, const std::shared_ptr<AssetImporter>& importer)
    {
        AssetImporterContext ctx;
        ctx.input = inputVariant;
        ctx.location = location;
        importer->import(ctx);
        auto asset = ctx.getMainAsset();
        if (!asset)
            return GenericAssetRef::null();
        if (asset->getID().is_nil())
            asset->m_metadata.id = boost::uuids::random_generator()();
        if (asset->m_metadata.location == AssetLocation("default"))
            asset->m_metadata.location = location;

        m_assets[asset->getID()] = asset;

        for (const auto& dep: ctx.getDependencies()) {
            GenericAssetRef depAssetRef = GenericAssetRef::null();
            if (dep.importer != nullptr) {
                depAssetRef = importAssetUsingImporter(ctx.location, ctx.input, dep.importer);
            } else {
                const auto& importerList = m_importers.getImportersForType(dep.typeIdx);
                depAssetRef = importAssetTryImporters(dep.ctx.location, dep.ctx.input, importerList);
            }
            if (const auto depAsset = depAssetRef.lock()) {
                depAsset->m_metadata.location = dep.ctx.location;
            }
        }

        return GenericAssetRef(asset);
    }

    GenericAssetRef AssetCatalog::importAssetTryImporters(const AssetLocation& location,
        const ImporterInputVariant& inputVariant, const std::vector<std::shared_ptr<AssetImporter>>& importers)
    {
        std::vector<std::shared_ptr<AssetImporter>> untriedImporters;
        for (const auto& importer : importers) {
            if (importer->canRead(inputVariant)) {
                auto asset = importAssetUsingImporter(location, inputVariant, importer);
                if (asset)
                    return asset;
            } else {
                untriedImporters.push_back(importer);
            }
        }
        // If "compatibles" importers failed, try even "incompatibles" ones
        for (const auto& importer : untriedImporters) {
            auto asset = importAssetUsingImporter(location, inputVariant, importer);
            if (asset)
                return asset;
        }
        return GenericAssetRef::null();
    }

    void AssetCatalog::setupImporterInstances()
    {
        m_importers.registerImporter<Model, ModelImporter>(100);
    }

} // namespace nexo::assets
