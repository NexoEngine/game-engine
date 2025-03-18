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
     * This constructor registers the TextureImporter for Texture assets with a priority of 100 and the ModelImporter for Model assets with a priority of 90.
     */
    AssetImporter::AssetImporter()
    {
        registerImporter<Texture, TextureImporter>(100);
        registerImporter<Model, ModelImporter>(90);
    }

    /**
     * @brief Destructor that deallocates all registered asset importers.
     *
     * Iterates over each list of importers in the internal registry and deletes each pointer,
     * ensuring that dynamically allocated memory is properly freed.
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
     * @brief Automatically imports an asset using registered importers.
     *
     * Iterates through all registered importer sets and attempts to import the asset located at the
     * specified location using the provided input variant. Returns the first successfully imported asset,
     * or a null reference if all attempts fail.
     *
     * @param location The asset's location.
     * @param inputVariant The input configuration used for the import process.
     * @return GenericAssetRef The imported asset reference if successful, or a null reference otherwise.
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
     * @brief Imports an asset using the specified importer.
     *
     * Sets up an asset importer context, either using a custom context or a temporary stack-allocated one,
     * with the provided location and input variant. The importer is then used to import the asset. If the import
     * is successful, the function ensures the asset has a unique ID and non-default location before registering
     * it in the AssetCatalog.
     *
     * @param location The source location of the asset.
     * @param inputVariant The input data variant used for importing the asset.
     * @param importer The specific importer instance used to perform the import operation.
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
     * @brief Attempts to import an asset using a list of asset importers.
     *
     * This function iterates over the provided asset importers and attempts to import an asset from the specified
     * location and input data. It first tries those importers that indicate compatibility with the input variant via
     * their `canRead()` method. If an import attempt is successful, it returns the imported asset immediately.
     * If no compatible importer succeeds, the function retries with the remaining importers regardless of compatibility.
     * If all attempts fail, a null asset reference is returned.
     *
     * @param location Information about the asset's location used during the import.
     * @param inputVariant Variant encapsulating the input data for the import operation.
     * @param importers List of candidate asset importers to try.
     * @return GenericAssetRef A reference to the imported asset if successful, or a null reference otherwise.
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
     * @brief Retrieves registered importers for a specified asset type.
     *
     * This function returns a constant reference to a vector containing pointers to asset importers
     * that are associated with the provided asset type index. If no importers are registered for the
     * specified type, a static empty vector is returned.
     *
     * @param typeIdx The type index representing the asset type for which importers are queried.
     * @return const std::vector<AssetImporterBase *>& A reference to the vector of registered importers,
     * or an empty vector if none are found.
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
     * @brief Determines if importers are registered for a given asset type.
     *
     * Checks whether any asset importers have been registered for the asset type specified by the given type index.
     *
     * @param typeIdx The type index representing the asset type.
     * @return true if at least one importer is registered for the asset type, false otherwise.
     */
    bool AssetImporter::hasImportersForType(const std::type_index& typeIdx) const
    {
        const auto it = m_importers.find(typeIdx);
        return (it != m_importers.end() && !it->second.empty());
    }

    /**
     * @brief Unregisters all asset importers associated with a specific asset type.
     *
     * This method removes all entries corresponding to the given asset type from the internal importer registry,
     * erasing both the list of importers and their associated details.
     *
     * @param typeIdx The type index representing the asset type to be unregistered.
     */
    void AssetImporter::unregisterAllImportersForType(const std::type_index& typeIdx)
    {
        m_importers.erase(typeIdx);
        m_importersDetails.erase(typeIdx);
    }

} // namespace nexo::assets
