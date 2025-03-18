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
 * @brief Sets the custom context for asset importing.
 *
 * This function assigns a custom context that can hold additional configuration or state used during asset import operations.
 *
 * @param ctx Pointer to an AssetImporterContext. Providing a null pointer will effectively clear any previously set context.
 */
void setCustomContext(AssetImporterContext *ctx) { m_customCtx = ctx; }

            /**
 * @brief Clears the current custom context.
 *
 * Resets the internal custom context pointer to null, effectively removing any previously set context for asset importing operations.
 */
void clearCustomContext() { m_customCtx = nullptr; }

            /**
 * @brief Retrieves the custom context associated with the asset importer.
 *
 * Returns the custom context pointer that has been set for the importer, enabling asset-import
 * operations to utilize additional user-defined settings or state. If no custom context has been
 * established, the function returns nullptr.
 *
 * @return AssetImporterContext* Pointer to the current custom context, or nullptr if unset.
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
     * @brief Registers a new importer for the specified asset type.
     *
     * Instantiates an importer of type ImporterType and registers it to handle assets
     * of type AssetType with the provided priority. A higher priority value indicates
     * a higher preference during asset import selection.
     *
     * @tparam AssetType The asset type for which the importer is registered.
     * @tparam ImporterType The concrete importer class to instantiate.
     * @param priority The priority assigned to the importer.
     */
    void AssetImporter::registerImporter(int priority)
    {
        auto importer = new ImporterType();
        registerImporter<AssetType>(importer, priority);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    /**
     * @brief Registers an importer instance for a specified asset type.
     *
     * Associates the provided importer with the asset type specified by the template parameter by inserting it into
     * the internal registry. The importer is positioned in an ordered list based on its priority, where lower numeric
     * values take precedence. New registry entries are created if none exist for the asset type.
     *
     * @param importer Pointer to the importer instance to register.
     * @param priority Priority used to order the importer relative to others of the same asset type.
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
     * @brief Imports an asset of the specified type using registered importers.
     *
     * This function retrieves the list of importers registered for the given asset type and
     * attempts to import the asset from the provided location using the input variant. If no
     * importers are found for the asset type, an error is logged and a null asset reference is returned.
     *
     * @tparam AssetType The type of asset to import.
     * @param location The source from which to load the asset.
     * @param inputVariant Variant holding additional data required for importing the asset.
     * @return AssetRef<AssetType> A reference to the imported asset, or a null reference if no suitable importer exists.
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
     * @brief Retrieves registered importers for the specified asset type.
     *
     * This templated function returns a constant reference to a vector of importer instances associated
     * with the asset type provided as the template parameter. The importers are typically ordered based on
     * their assigned priority.
     *
     * @tparam AssetType The asset type for which to retrieve importer instances.
     * @return A constant reference to a vector of pointers to AssetImporterBase associated with the specified asset type.
     */
    const std::vector<AssetImporterBase *>& AssetImporter::getImportersForType() const
    {
        const auto typeIdx = std::type_index(typeid(AssetType));

        return getImportersForType(typeIdx);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    /**
     * @brief Checks if any importers are registered for the specified asset type.
     *
     * This templated method computes the unique type identifier for AssetType and delegates
     * the check to the overloaded function that queries the importer registry.
     *
     * @tparam AssetType The asset type for which to check for registered importers.
     * @return true if at least one importer is registered for AssetType; false otherwise.
     */
    bool AssetImporter::hasImportersForType() const
    {
        const auto typeIdx = std::type_index(typeid(AssetType));
        return hasImportersForType(typeIdx);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    /**
     * @brief Unregisters all importers for the specified asset type.
     *
     * Removes all registered importers and their priority details associated with the asset type `AssetType`
     * from the internal registry. If no importers for the asset type exist, the function performs no action.
     *
     * @tparam AssetType The asset type whose importers are to be unregistered.
     */
    void AssetImporter::unregisterAllImportersForType()
    {
        const auto typeIdx = std::type_index(typeid(AssetType));
        m_importers.erase(typeIdx);
        m_importersDetails.erase(typeIdx);
    }
} // namespace nexo::assets
