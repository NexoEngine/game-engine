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
    /**
     * @brief Constructs an AssetImporter and registers default asset importers.
     *
     * Initializes the AssetImporter by registering the TextureImporter with a priority of 100 
     * and the ModelImporter with a priority of 90, ensuring that the default mechanisms for 
     * importing texture and model assets are in place.
     */
    AssetImporter::AssetImporter()
    {
        registerImporter<Texture, TextureImporter>(100);
        registerImporter<Model, ModelImporter>(90);
    }

    /**
     * @brief Destroys the AssetImporter and frees associated importer memory.
     *
     * Iterates over all importer vectors in the registry and deletes each importer,
     * ensuring all dynamically allocated asset importer objects are properly released.
     */
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
     * Iterates through the registered importer groups to attempt asset importation. For each non-empty group, 
     * it calls the function that tries all importers in that group and returns the asset reference on the first successful import.
     * If none of the importers succeed, a null asset reference is returned.
     *
     * @param location The source location of the asset to be imported.
     * @param inputVariant The data variant containing information needed for the asset import.
     * @return GenericAssetRef A reference to the imported asset if successful; otherwise, a null reference.
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
     * This function performs an asset import operation by invoking the provided importer with an import
     * context. If a custom context is not available, a temporary context is created using the supplied
     * asset location and input data. After importing, the function ensures the asset has a valid identifier
     * (generating one if necessary) and that its location is updated from the default value. The asset is then
     * registered with the AssetCatalog.
     *
     * @param location The target location for the asset.
     * @param inputVariant The input data variant used for the import.
     * @param importer The importer responsible for handling the asset import.
     * @return GenericAssetRef A reference to the registered asset, or a null reference if the import fails.
     */
    GenericAssetRef AssetImporter::importAssetUsingImporter(const AssetLocation& location,
        const ImporterInputVariant& inputVariant, AssetImporterBase* importer)
    {
        AssetImporterContext* ctx = m_customCtx;
        AssetImporterContext ctxOnStack;
        if (!m_customCtx) {
            ctx = &ctxOnStack;
            ctx->input = inputVariant;
            ctx->location = location;
        }


        importer->import(*ctx);
        auto asset = ctx->getMainAsset();
        if (!asset)
            return GenericAssetRef::null();
        if (asset->getID().is_nil())
            asset->m_metadata.id = boost::uuids::random_generator()();
        if (asset->m_metadata.location == AssetLocation("default"))
            asset->m_metadata.location = location;

        return AssetCatalog::getInstance().registerAsset(location, asset);
    }

    /**
     * @brief Attempts to import an asset using a sequence of importers.
     *
     * This function first iterates over the provided importers, attempting an asset import with those
     * that can read the given input variant. If no importer that supports the input succeeds, it then
     * retries with the remaining importers. It returns the first successfully imported asset or a null
     * reference if all attempts fail.
     *
     * @param location The location of the asset to import.
     * @param inputVariant The variant containing input data for the import operation.
     * @param importers The list of asset importers to try.
     * @return GenericAssetRef The imported asset if successful, otherwise a null reference.
     */
    GenericAssetRef AssetImporter::importAssetTryImporters(const AssetLocation& location,
        const ImporterInputVariant& inputVariant, const std::vector<AssetImporterBase*>& importers)
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

    /**
     * @brief Retrieves the registered asset importers for a specified asset type.
     *
     * This function returns a constant reference to the vector of asset importer pointers associated with the given type index.
     * If no importers are registered for the specified type, it returns a reference to a static empty vector.
     *
     * @param typeIdx The type index representing the asset type.
     * @return const std::vector<AssetImporterBase *>& A reference to the vector of registered asset importers, or an empty vector if none exist.
     */
    const std::vector<AssetImporterBase *>& AssetImporter::
    getImportersForType(const std::type_index& typeIdx) const
    {
        if (const auto it = m_importers.find(typeIdx) ; it == m_importers.end()) {
            static const std::vector<AssetImporterBase *> empty;
            return empty;
        }
        return m_importers.at(typeIdx);
    }

    /**
     * @brief Checks if any importers are registered for a specific asset type.
     *
     * This function determines whether the asset importer registry contains at least one
     * importer associated with the provided asset type index.
     *
     * @param typeIdx The type index representing the asset type to check.
     * @return true if there is at least one importer for the given type; false otherwise.
     */
    bool AssetImporter::hasImportersForType(const std::type_index& typeIdx) const
    {
        const auto it = m_importers.find(typeIdx);
        return (it != m_importers.end() && !it->second.empty());
    }

    /**
     * @brief Unregisters all importers for a specific asset type.
     *
     * Removes all importer entries and their associated details from the registry
     * for the specified asset type.
     *
     * @param typeIdx The type index representing the asset whose importers will be removed.
     */
    void AssetImporter::unregisterAllImportersForType(const std::type_index& typeIdx)
    {
        m_importers.erase(typeIdx);
        m_importersDetails.erase(typeIdx);
    }

} // namespace nexo::assets
