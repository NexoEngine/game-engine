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
            GenericAssetRef importAssetAuto(const AssetLocation& location, const ImporterInputVariant& inputVariant);
            GenericAssetRef importAssetUsingImporter(const AssetLocation& location, const ImporterInputVariant& inputVariant, AssetImporterBase *importer);
            GenericAssetRef importAssetTryImporters(const AssetLocation& location, const ImporterInputVariant& inputVariant, const std::vector<AssetImporterBase *>&
                importers);

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

            /**
 * @brief Sets the custom context for asset importing operations.
 *
 * This method assigns a new custom context that can be used by the asset importer to
 * alter or extend its behavior during asset import processes.
 *
 * @param ctx Pointer to the new custom context.
 */
void setCustomContext(AssetImporterContext *ctx) { m_customCtx = ctx; }

            /**
 * @brief Clears the custom context for asset importing.
 *
 * Resets the custom context pointer to nullptr so that subsequent asset import
 * operations will use the default context.
 */
void clearCustomContext() { m_customCtx = nullptr; }

            /**
 * @brief Retrieves the custom context set for asset importing.
 *
 * Returns the pointer to the custom context that has been configured for this importer.
 * If no custom context has been set, the function returns nullptr.
 *
 * @return AssetImporterContext* Pointer to the current custom context, or nullptr if not set.
 */
AssetImporterContext *getCustomContext() const { return m_customCtx; }

            void setParameters(const json& params);


        private:

            /**
             * @brief Register an importer for a specific asset type
             *
             * @tparam AssetType The type of asset the importer can handle
             * @tparam ImporterType The type of importer to register
             * @param priority Optional priority value (higher values = higher priority)
             */
            template<typename AssetType, typename ImporterType>
                requires std::derived_from<AssetType, IAsset>
                      && std::derived_from<ImporterType, AssetImporterBase>
            void registerImporter(int priority = 0);

            /**
             * @brief Register an existing importer instance for a specific asset type
             * @note Registered in order of priority and then insertion
             *
             * @tparam AssetType The type of asset the importer can handle
             * @param importer The importer instance to register
             * @param priority Optional priority value (higher values = higher priority)
             */
            template<typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            void registerImporter(AssetImporterBase *importer, int priority = 0);

            /**
             * @brief Unregister all importers for an asset type
             *
             * @tparam AssetType The type of asset
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
    /**
     * @brief Registers a new importer instance for a specific asset type.
     *
     * Creates a new instance of the importer (of type ImporterType) and registers it for the asset type defined by AssetType.
     * The importer is added with the specified priority, which influences the order in which importers are invoked during asset importing.
     *
     * @tparam AssetType The asset type with which the importer is associated.
     * @tparam ImporterType The concrete importer type to instantiate and register.
     * @param priority The registration priority for the importer (default is 0).
     */
    void AssetImporter::registerImporter(int priority)
    {
        auto importer = new ImporterType();
        registerImporter<AssetType>(importer, priority);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    /**
     * @brief Registers an asset importer instance for a specified asset type.
     *
     * This function registers an importer for the asset type provided as a template parameter. It ensures
     * that the importer is inserted into the registry's list in sorted order based on the given priority.
     * If no importer list or details exist for the asset type, the corresponding containers are created.
     *
     * @tparam AssetType The asset type for which the importer is being registered.
     * @param importer Pointer to the asset importer instance.
     * @param priority Priority level that determines the importer's order; importers with lower priority values
     * are placed before those with higher values.
     */
    void AssetImporter::registerImporter(AssetImporterBase *importer, int priority)
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
        for (; i < importersVec.size() && importersDetailsVec[i].priority < priority; ++i);
        importersVec.insert(importersVec.begin() + static_cast<long>(i), importer);
        importersDetailsVec.insert(importersDetailsVec.begin() + static_cast<long>(i), {priority});
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    /**
     * @brief Imports an asset of type AssetType from a specified location.
     *
     * Retrieves the registered importers for the given asset type and attempts to import the asset using them.
     * If no importer is found, the function logs an error and returns a null asset reference.
     *
     * @tparam AssetType The type of asset to import.
     * @param location The source location from which to import the asset.
     * @param inputVariant Additional data required for the import process.
     * @return AssetRef<AssetType> A reference to the imported asset or a null reference if no suitable importer is registered.
     */
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
    /**
     * @brief Retrieves the registered asset importers for the specified asset type.
     *
     * This template function deduces the runtime type of the asset using `typeid` and returns a constant reference
     * to the vector of importers associated with that type. It delegates the retrieval to the overload that
     * uses a `std::type_index` for the lookup.
     *
     * @tparam AssetType The asset type for which the importers are being retrieved.
     * @return A constant reference to a vector containing pointers to the asset importer instances for the specified type.
     */
    const std::vector<AssetImporterBase *>& AssetImporter::getImportersForType() const
    {
        const auto typeIdx = std::type_index(typeid(AssetType));

        return getImportersForType(typeIdx);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    /**
     * @brief Checks if any importer is registered for a specified asset type.
     *
     * This templated function converts the asset type provided as a template parameter to a runtime type index
     * and determines if any importer is registered for that asset type.
     *
     * @tparam AssetType The asset type to check for registered importers.
     * @return true if at least one importer is registered for the specified asset type, false otherwise.
     */
    bool AssetImporter::hasImportersForType() const
    {
        const auto typeIdx = std::type_index(typeid(AssetType));
        return hasImportersForType(typeIdx);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    /**
     * @brief Unregisters all importers associated with the specified asset type.
     *
     * This method removes all registered importer instances and their corresponding details for the asset type determined by the template parameter.
     *
     * @tparam AssetType The asset type for which importers should be unregistered.
     */
    void AssetImporter::unregisterAllImportersForType()
    {
        const auto typeIdx = std::type_index(typeid(AssetType));
        m_importers.erase(typeIdx);
        m_importersDetails.erase(typeIdx);
    }
} // namespace nexo::assets
