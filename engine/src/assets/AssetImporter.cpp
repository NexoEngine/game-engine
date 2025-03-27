//// AssetImporter.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/03/2025
//  Description: Registry for asset importers implementation
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetImporter.hpp"
#include "AssetImporterBase.hpp"
#include "AssetCatalog.hpp"

#include "Assets/Model/ModelImporter.hpp"
#include "Assets/Texture/TextureImporter.hpp"

namespace nexo::assets {
    AssetImporter::AssetImporter()
    {
        registerImporter<Texture, TextureImporter>(100);
        registerImporter<Model, ModelImporter>(90);
    }

    AssetImporter::~AssetImporter()
    {
        for (auto& importers: m_importers | std::views::values) {
            for (const auto& importer: importers) {
                delete importer;
            }
        }
    }

    /**
     * @brief Automatically imports an asset using available importers.
     *
     * Iterates through all registered importer groups, invoking each group's importer(s)
     * to attempt importing an asset from the specified location and input data variant.
     * Returns the first successfully imported asset, or a null reference if none succeed.
     *
     * @param location The location of the asset to be imported.
     * @param inputVariant The input data variant providing information for asset import.
     * @return GenericAssetRef A reference to the imported asset, or GenericAssetRef::null() if import fails.
     */
    GenericAssetRef AssetImporter::importAssetAuto(const AssetLocation& location, const ImporterInputVariant& inputVariant)
    {
        for (const auto& importers: m_importers | std::views::values) {
            if (importers.empty())
                continue;
            if (const auto asset = importAssetTryImporters(location, inputVariant, importers))
                return asset;
        }
        return GenericAssetRef::null();
    }

    /**
     * @brief Imports an asset using a specified importer and registers it.
     *
     * This function attempts to import an asset by invoking the given importer. It utilizes a custom import context if one
     * is configured; otherwise, it creates a temporary context initialized with the provided input data and location.
     * After importing, if the asset is valid, the function ensures that the asset has a unique identifier and updates its location
     * metadata if it is set to "default". The asset is then registered in the AssetCatalog. If the import fails, a null asset reference is returned.
     *
     * @param location The asset location used for input configuration and asset registration.
     * @param inputVariant Input data required by the importer for the asset import operation.
     * @param importer The importer instance responsible for performing the asset import.
     * @return GenericAssetRef A reference to the imported and registered asset, or a null reference if the import fails.
     */
    GenericAssetRef AssetImporter::importAssetUsingImporter(const AssetLocation& location,
        const ImporterInputVariant& inputVariant, AssetImporterBase* importer) const
    {
        AssetImporterContext* ctx = m_customCtx;
        AssetImporterContext ctxOnStack;
        if (!m_customCtx) {
            ctx = &ctxOnStack;
            ctx->input = inputVariant;
            ctx->location = location;
        }


        importer->import(*ctx);
        const auto asset = ctx->getMainAsset();
        if (!asset)
            return GenericAssetRef::null();
        if (asset->getID().is_nil())
            asset->m_metadata.id = boost::uuids::random_generator()();
        if (asset->m_metadata.location == AssetLocation("default"))
            asset->m_metadata.location = location;

        return AssetCatalog::getInstance().registerAsset(location, asset);
    }

    /**
     * @brief Attempts to import an asset using a prioritized list of importers.
     *
     * This function iterates over the provided importers in two phases. In the first phase, it attempts to import the asset
     * using importers that are capable of reading the given input variant. If none of these succeed, it then tries the remaining
     * importers regardless of compatibility. The function returns immediately upon the first successful import, or a null asset
     * reference if all attempts fail.
     *
     * @param location The asset's location information.
     * @param inputVariant A variant that encapsulates the data and configuration for asset import.
     * @param importers A list of asset importers to attempt the import operation with.
     * @return GenericAssetRef A reference to the successfully imported asset, or a null reference if the import fails.
     */
    GenericAssetRef AssetImporter::importAssetTryImporters(const AssetLocation& location,
        const ImporterInputVariant& inputVariant, const std::vector<AssetImporterBase*>& importers) const
    {
        std::vector<AssetImporterBase *> untriedImporters;
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

    const std::vector<AssetImporterBase *>& AssetImporter::
    getImportersForType(const std::type_index& typeIdx) const
    {
        if (const auto it = m_importers.find(typeIdx) ; it == m_importers.end()) {
            static const std::vector<AssetImporterBase *> empty;
            return empty;
        }
        return m_importers.at(typeIdx);
    }

    bool AssetImporter::hasImportersForType(const std::type_index& typeIdx) const
    {
        const auto it = m_importers.find(typeIdx);
        return (it != m_importers.end() && !it->second.empty());
    }

    void AssetImporter::unregisterAllImportersForType(const std::type_index& typeIdx)
    {
        m_importers.erase(typeIdx);
        m_importersDetails.erase(typeIdx);
    }

} // namespace nexo::assets
