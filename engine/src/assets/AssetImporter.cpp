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
     * This constructor registers a TextureImporter for Texture assets with a priority of 100
     * and a ModelImporter for Model assets with a priority of 90, ensuring that these asset types
     * have dedicated importers available for asset processing.
     */
    AssetImporter::AssetImporter()
    {
        registerImporter<Texture, TextureImporter>(100);
        registerImporter<Model, ModelImporter>(90);
    }

    /**
     * @brief Destroys the AssetImporter instance and cleans up all registered importers.
     *
     * Iterates over the registered importer collections and deletes each dynamically 
     * allocated asset importer to free memory.
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
     * @brief Attempts to automatically import an asset using registered importers.
     *
     * This function iterates over all lists of importers registered in the internal registry. For each list,
     * it calls the importAssetTryImporters method to try and import the asset using the provided location and input data.
     * It returns the first successfully imported asset, or a null asset reference if no importer is able to perform the import.
     *
     * @param location The location information identifying the asset to be imported.
     * @param inputVariant The input data required for asset import.
     * @return GenericAssetRef A reference to the imported asset, or a null reference if the import fails.
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
     * Sets up an asset importer context—using a custom context if available or a temporary one otherwise—and delegates
     * the import operation to the given importer. If the import is successful, the asset's metadata is updated by assigning
     * a new unique identifier if needed and replacing a default location with the provided one before the asset is registered
     * in the asset catalog.
     *
     * @param location The location where the asset is expected to be found.
     * @param inputVariant The input data variant used for importing the asset.
     * @param importer The asset importer instance to perform the import operation.
     * @return GenericAssetRef A reference to the registered asset, or a null reference if the import fails.
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
     * @brief Attempts to import an asset by trying a list of importers.
     *
     * This function first iterates over the provided importers and uses those that are compatible with the given input variant (i.e., their `canRead()` method returns true) to try importing the asset via `importAssetUsingImporter()`. If none of the compatible importers successfully import the asset, it retries with the remaining, originally incompatible importers. It returns the first successfully imported asset, or a null reference if all attempts fail.
     *
     * @param location The location descriptor of the asset to import.
     * @param inputVariant The input data variant used by the importers to read the asset.
     * @param importers A list of asset importers to attempt for the import.
     * @return A reference to the successfully imported asset, or a null reference if the import fails.
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

    /**
     * @brief Retrieves the asset importers registered for a specific asset type.
     *
     * Returns a constant reference to a vector containing pointers to importers associated with the given type index. 
     * If no importers are registered for the specified type, an empty vector is returned.
     *
     * @param typeIdx The type index representing the asset type.
     * @return const std::vector<AssetImporterBase*>& A reference to the vector of importers for the given type.
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
     * @brief Determines if there are any registered importers for a given asset type.
     *
     * Searches the internal importer registry for the specified type index and checks that the associated importer list is not empty.
     *
     * @param typeIdx The type index identifying the asset type.
     * @return true if at least one importer is registered for the specified asset type, false otherwise.
     */
    bool AssetImporter::hasImportersForType(const std::type_index& typeIdx) const
    {
        const auto it = m_importers.find(typeIdx);
        return (it != m_importers.end() && !it->second.empty());
    }

    /**
     * @brief Unregisters all asset importers for the specified asset type.
     *
     * Removes both the importers and their associated details corresponding to the provided type index.
     *
     * @param typeIdx The type index identifying the asset type to unregister.
     */
    void AssetImporter::unregisterAllImportersForType(const std::type_index& typeIdx)
    {
        m_importers.erase(typeIdx);
        m_importersDetails.erase(typeIdx);
    }

} // namespace nexo::assets
