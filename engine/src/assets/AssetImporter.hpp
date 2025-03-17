//// AssetImporterRegistry.hpp ////////////////////////////////////////////////
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

            void setCustomContext(AssetImporterContext *ctx) { m_customCtx = ctx; }

            void clearCustomContext() { m_customCtx = nullptr; }

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
    void AssetImporter::registerImporter(int priority)
    {
        auto importer = new ImporterType();
        registerImporter<AssetType>(importer, priority);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
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
    AssetRef<AssetType> AssetImporter::importAsset(const AssetLocation& location,
        const ImporterInputVariant& inputVariant)
    {
        auto importers = getImportersForType<AssetType>();
        if (importers.empty()) {
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
        m_importers.erase(typeIdx);
        m_importersDetails.erase(typeIdx);
    }
} // namespace nexo::assets
