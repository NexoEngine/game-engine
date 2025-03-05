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

#include "AssetImporter.hpp"
#include "Asset.hpp"

namespace nexo::assets {

    /**
     * @class AssetImporterRegistry
     *
     * @brief Manages registration and selection of asset importers
     */
    class AssetImporterRegistry {
        public:
            using ImporterMap = std::map<std::type_index, std::vector<std::shared_ptr<AssetImporter>>>;

            /**
             * @brief Register an importer for a specific asset type
             *
             * @tparam AssetType The type of asset the importer can handle
             * @tparam ImporterType The type of importer to register
             * @param priority Optional priority value (higher values = higher priority)
             */
            template<typename AssetType, typename ImporterType>
                requires std::derived_from<AssetType, IAsset>
                      && std::derived_from<ImporterType, AssetImporter>
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
            void registerImporter(const std::shared_ptr<AssetImporter>& importer, int priority = 0);

            /**
             * @brief Get all registered importers for an asset type
             *
             * @tparam AssetType The type of asset
             * @return Vector of importers in priority order (highest first)
             */
            template<typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            [[nodiscard]] const std::vector<std::shared_ptr<AssetImporter>>& getImportersForType() const;

            /**
             * @brief Get all registered importers for an asset type
             *
             * @param typeIdx The type index of the asset
             * @return Vector of importers in priority order (highest first)
             */
            [[nodiscard]] const std::vector<std::shared_ptr<AssetImporter>>& getImportersForType(const std::type_index& typeIdx) const;

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

        private:
            struct ImporterDetails {
                int priority;
            };
            using ImporterDetailsMap = std::map<std::type_index, std::vector<ImporterDetails>>;

            // Map from asset type to prioritized list of importers
            ImporterMap m_importers;
            ImporterDetailsMap m_importersDetails;
    };

    template<typename AssetType, typename ImporterType> requires std::derived_from<AssetType, IAsset> && std::
        derived_from<ImporterType, AssetImporter>
    void AssetImporterRegistry::registerImporter(int priority)
    {
        auto importer = std::make_shared<ImporterType>();
        registerImporter<AssetType>(importer, priority);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    void AssetImporterRegistry::registerImporter(const std::shared_ptr<AssetImporter>& importer, int priority)
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

        for (size_t i = 0; i < importersVec.size(); ++i) {
            if (importersDetailsVec[i].priority < priority) {
                importersVec.insert(importersVec.begin() + static_cast<long>(i), importer);
                importersDetailsVec.insert(importersDetailsVec.begin() + static_cast<long>(i), {priority});
                return;
            }
        }

    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    const std::vector<std::shared_ptr<AssetImporter>>& AssetImporterRegistry::getImportersForType() const
    {
        const auto typeIdx = std::type_index(typeid(AssetType));

        return getImportersForType(typeIdx);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    bool AssetImporterRegistry::hasImportersForType() const
    {
        const auto typeIdx = std::type_index(typeid(AssetType));
        return hasImportersForType(typeIdx);
    }

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    void AssetImporterRegistry::unregisterAllImportersForType()
    {
        const auto typeIdx = std::type_index(typeid(AssetType));
        m_importers.erase(typeIdx);
        m_importersDetails.erase(typeIdx);
    }
} // namespace nexo::assets
