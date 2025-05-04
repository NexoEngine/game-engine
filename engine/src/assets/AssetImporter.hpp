//// AssetImporter.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/03/2025
//  Description: Registry for asset importers
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <typeindex>
#include <map>
#include <vector>
#include <filesystem>

#include "Asset.hpp"
#include "AssetImporterInput.hpp"
#include "json.hpp"

namespace nexo::assets {

    class AssetImporterBase;

    struct AssetImporterContext;

    /**
     * @class AssetImporter
     *
     * @brief Registry for asset importers, allowing to import assets from various sources.
     */
    class AssetImporter {
        public:
            using ImporterMap = std::map<std::type_index, std::vector<AssetImporterBase *>>;

            AssetImporter();
            ~AssetImporter();

            template<typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            AssetRef<AssetType> importAsset(const AssetLocation& location, const ImporterInputVariant& inputVariant);

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
            GenericAssetRef importAssetAuto(const AssetLocation& location, const ImporterInputVariant& inputVariant);

            /**
             * @brief Imports an asset using a specified importer and registers it.
             *
             * This function attempts to import an asset by invoking the given importer. It utilizes a custom import context if one
             * is configured (see setCustomContext()); otherwise, it creates a temporary context initialized with the provided input data and location.
             * After importing, if the asset is valid, the function ensures that the asset has a unique identifier and updates its location
             * metadata if it is set to "default". The asset is then registered in the AssetCatalog. If the import fails, a null asset reference is returned.
             *
             * @param location The asset location used for input configuration and asset registration.
             * @param inputVariant Input data required by the importer for the asset import operation.
             * @param importer The importer instance responsible for performing the asset import.
             * @return GenericAssetRef A reference to the imported and registered asset, or a null reference if the import fails.
             */
            GenericAssetRef importAssetUsingImporter(const AssetLocation& location, const ImporterInputVariant& inputVariant, AssetImporterBase *importer) const;

            /**
             * @brief Attempts to import an asset using a prioritized list of importers.
             *
             * This function iterates over the provided importers in two phases:
             * 1. In the first phase, it attempts to import the asset
             *    using importers that are capable of reading the given input variant. (checking with canRead())
             * 2. If none of these succeed, it then tries the remaining
             *    importers regardless of compatibility. The function returns immediately
             *    upon the first successful import, or a null asset reference if all attempts fail.
             *
             * @param location The asset's location information.
             * @param inputVariant A variant that encapsulates the data and configuration for asset import.
             * @param importers A list of asset importers to attempt the import operation with.
             * @return GenericAssetRef A reference to the successfully imported asset, or a null reference if the import fails.
             */
            [[nodiscard]] GenericAssetRef importAssetTryImporters(const AssetLocation& location, const ImporterInputVariant& inputVariant, const std::vector<AssetImporterBase *>&
                importers) const;

            /**
             * @brief Get all registered importers for an asset type
             *
             * @tparam AssetType The type of asset
             * @return Vector of importers in priority order (highest first)
             */
            template<typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            [[nodiscard]] const std::vector<AssetImporterBase *>& getImportersForType() const;

            /**
             * @brief Get all registered importers for an asset type
             *
             * @param typeIdx The type index of the asset
             * @return Vector of importers in priority order (highest first)
             */
            [[nodiscard]] const std::vector<AssetImporterBase *>& getImportersForType(const std::type_index& typeIdx) const;

            /**
             * @brief Get all registered importers
             *
             * @return Map of importers by asset type
             */
            [[nodiscard]] const ImporterMap& getImporters() const { return m_importers; }

            /**
             * @brief Check if any importers are registered for an asset type
             *
             * @tparam AssetType The type of asset
             * @return True if importers are registered, false otherwise
             */
            template<typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            [[nodiscard]] bool hasImportersForType() const;

            /**
             * @brief Check if any importers are registered for an asset type
             *
             * @param typeIdx The type index of the asset
             * @return True if importers are registered, false otherwise
             */
            [[nodiscard]] bool hasImportersForType(const std::type_index& typeIdx) const;

            void setCustomContext(AssetImporterContext *ctx) { m_customCtx = ctx; }

            /**
             * @brief Clears the custom context.
             *
             * Resets the internal custom context pointer to indicate that no custom context is in use.
             */
            void clearCustomContext() { m_customCtx = nullptr; }

            /**
             * @brief Retrieves the current custom asset importer context.
             *
             * This function returns a pointer to the custom context associated with the asset importer.
             * The returned context can provide custom configurations or behaviors if one has been set.
             *
             * @return A pointer to the current AssetImporterContext, or nullptr if no custom context is set.
             */
            [[nodiscard]] AssetImporterContext *getCustomContext() const { return m_customCtx; }

            void setParameters(const json& params);


        protected:

            /**
             * @brief Constructs an AssetImporter with a custom context.
             *
             * Initializes the AssetImporter using the provided custom context, allowing
             * derived classes and unit tests to override the default importer context.
             *
             * @param ctx Pointer to the custom AssetImporterContext used for asset importing.
             */
            explicit AssetImporter(AssetImporterContext *ctx) : m_customCtx(ctx)
            {
            }

            /**
             * @brief Instantiates and registers a new importer.
             *
             * This templated function creates a new importer instance of type ImporterType using default construction and
             * registers it to handle assets of type AssetType with the provided priority. For equal priority values,
             * the order of registration determines the processing sequence.
             *
             * @tparam AssetType The asset type associated with the importer.
             * @tparam ImporterType The type of the importer to be registered.
             * @param priority The registration priority. Importers with equal priorities retain the order in which they were added.
             */
            template<typename AssetType, typename ImporterType>
                requires std::derived_from<AssetType, IAsset>
                      && std::derived_from<ImporterType, AssetImporterBase>
            void registerImporter(int priority = 0);

            /**
             * @brief Registers an importer instance for a specific asset type.
             *
             * This function inserts the provided importer for the asset type into the internal registry,
             * maintaining a descending order based on the importers' priority. Importers with higher priority
             * are placed before those with lower priority, and if equal priorities exist, the new importer is
             * appended after previously registered ones.
             *
             * @tparam AssetType The asset type associated with the importer.
             * @param importer Pointer to the importer instance.
             * @param priority An integer representing the importer's priority; higher values denote higher precedence. If equal, insertion order.
             */
            template<typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            void registerImporter(AssetImporterBase *importer, int priority = 0);

            /**
             * @brief Unregisters all importers associated with a specific asset type.
             *
             * Determines the runtime type index of the asset type using RTTI and delegates the unregistration
             * to the overload that handles type index unregistration.
             *
             * @tparam AssetType The asset type for which all registered importers will be unregistered.
             */
            template<typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            void unregisterAllImportersForType();

            /**
             * @brief Unregister all importers for an asset type
             *
             * @param typeIdx The type index of the asset
             */
            void unregisterAllImportersForType(const std::type_index& typeIdx);


            struct ImporterDetails {
                int priority;
            };
            using ImporterDetailsMap = std::map<std::type_index, std::vector<ImporterDetails>>;

            // Map from asset type to prioritized list of importers
            ImporterMap m_importers;
            ImporterDetailsMap m_importersDetails;

            AssetImporterContext *m_customCtx = nullptr;
    };

    template<typename AssetType, typename ImporterType> requires std::derived_from<AssetType, IAsset> && std::
        derived_from<ImporterType, AssetImporterBase>
    void AssetImporter::registerImporter(int priority)
    {
        auto importer = new ImporterType();
        registerImporter<AssetType>(importer, priority);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    void AssetImporter::registerImporter(AssetImporterBase *importer, const int priority)
    {
        const auto typeIdx = std::type_index(typeid(AssetType));

        if (!m_importers.contains(typeIdx)) {
            m_importers[typeIdx] = {};
        }
        if (!m_importersDetails.contains(typeIdx)) {
            m_importersDetails[typeIdx] = {};
        }

        auto& importersVec = m_importers[typeIdx];
        auto& importersDetailsVec = m_importersDetails[typeIdx];

        size_t i = 0;
        for (; i < importersVec.size() && priority <= importersDetailsVec[i].priority ; ++i);
        importersVec.insert(importersVec.begin() + static_cast<long>(i), importer);
        importersDetailsVec.insert(importersDetailsVec.begin() + static_cast<long>(i), {priority});
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    AssetRef<AssetType> AssetImporter::importAsset(const AssetLocation& location,
        const ImporterInputVariant& inputVariant)
    {
        auto importers = getImportersForType<AssetType>();
        if (importers.empty()) {
            LOG(NEXO_ERROR, "No importers registered for asset type {}", typeid(AssetType).name());
            return GenericAssetRef::null().as<AssetType>();
        }
        return importAssetTryImporters(location, inputVariant, importers).template as<AssetType>();
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    const std::vector<AssetImporterBase *>& AssetImporter::getImportersForType() const
    {
        const auto typeIdx = std::type_index(typeid(AssetType));

        return getImportersForType(typeIdx);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    bool AssetImporter::hasImportersForType() const
    {
        const auto typeIdx = std::type_index(typeid(AssetType));
        return hasImportersForType(typeIdx);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    void AssetImporter::unregisterAllImportersForType()
    {
        const auto typeIdx = std::type_index(typeid(AssetType));
        unregisterAllImportersForType(typeIdx);
    }
} // namespace nexo::assets
