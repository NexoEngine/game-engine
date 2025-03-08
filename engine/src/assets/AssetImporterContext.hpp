//// AssetImporterContext.hpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        02/03/2025
//  Description: Header file for the asset importer context class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <filesystem>
#include <vector>
#include <variant>
#include <unordered_map>

#include "AssetImportParameters.hpp"

#include "Asset.hpp"
#include "json.hpp"

namespace nexo::assets {
    struct AssetDependency;

    class AssetCatalog;
    class AssetImporter;

    // Import from file, importer should open and read the file
    struct ImporterFileInput {
        std::filesystem::path filePath; //< File path to import
    };

    // Import from memory buffer, importer should read from the buffer
    struct ImporterMemoryInput {
        std::vector<uint8_t> memoryData;          //< Memory buffer
        std::optional<std::string> fileExtension; //< For format detection with memory sources (MUST start with a dot, e.g.: .png)
    };

    using ImporterInputVariant = std::variant<ImporterFileInput, ImporterMemoryInput>;

    /**
     * @class AssetImporterContext
     *
     * @brief Context class for asset importers.
     */
    struct AssetImporterContext {
        public:
            ImporterInputVariant input;                          //< Input data for the importer
            std::shared_ptr<IAsset> parentAsset;                 //< Parent asset (if any)
            AssetLocation location = AssetLocation("default"); //< Future location of the asset in the catalog

            AssetImporterContext() = default;
            ~AssetImporterContext() = default;

            /**
             * @brief Set the main asset data for this context
             * @param data The main asset data
             * @note This method must be called by the importer to set the main asset data
             */
            void setMainAssetData(const std::shared_ptr<IAsset>& data);

            /**
             * @brief Get the main asset data for this context
             * @return The main asset data
             */
            [[nodiscard]] const std::shared_ptr<IAsset>& getMainAsset() const;

            /**
             * @brief Add a dependency to the context
             * @tparam AssetType The type of asset to add as a dependency
             * @param input The input data for the importer
             * @param importer The importer to use for this dependency (default: nullptr) (if null, auto-select)
             * @return A reference to the dependency asset
             */
            template <typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            GenericAssetRef addDependency(const ImporterInputVariant& input, std::shared_ptr<AssetImporter> importer = nullptr);

            [[nodiscard]] const std::vector<AssetDependency>& getDependencies() const;

            template <typename ParamType>
                requires JSONSerializable<ParamType>
            void setParameters(const ParamType& params);

            void setParameters(const json& params);

            template <typename ParamType>
                requires JSONSerializable<ParamType>
            [[nodiscard]] ParamType getParameters() const;

            [[nodiscard]] json getParameters() const;

        private:
            std::shared_ptr<IAsset> m_mainAsset;         //< Main asset being imported, resulting asset (MUST be set by importer)
            std::vector<AssetDependency> m_dependencies; //< Dependencies to import
            json m_jsonParameters;             //< JSON parameters for the importer
    };

    /**
     * @brief Represents a dependency asset to be imported
     */
    struct AssetDependency {
        std::shared_ptr<IAsset> asset;           //< Ptr to this dependency asset (to later update the assetData inside)
        std::shared_ptr<AssetImporter> importer; //< Ptr to the importer for this dependency, if null, auto-select
        std::type_index typeIdx;                 //< Type index of the dependency asset
        AssetImporterContext ctx;                //< Import context for this dependency
    };

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    GenericAssetRef AssetImporterContext::addDependency(const ImporterInputVariant& input, std::shared_ptr<AssetImporter> importer)
    {
        auto asset = std::make_shared<AssetType>();
        auto ref = GenericAssetRef(asset);

        std::string depLocation = std::format("{}_{}{}",
            location.getFullLocation(), AssetTypeNames[asset->getType()], std::to_string(m_dependencies.size()));

        AssetImporterContext ctx;
        ctx.input = input;
        ctx.parentAsset = m_mainAsset;
        ctx.location = AssetLocation(depLocation);

        const AssetDependency dep {
            .asset = asset,
            .ctx = ctx,
            .typeIdx = std::type_index(typeid(AssetType)),
            .importer = importer
        };
        m_dependencies.push_back(dep);
        return ref;
    }

    template<typename ParamType> requires JSONSerializable<ParamType>
    void AssetImporterContext::setParameters(const ParamType& params)
    {
        to_json(m_jsonParameters, params);
    }

    template<typename ParamType> requires JSONSerializable<ParamType>
    ParamType AssetImporterContext::getParameters() const
    {
        ParamType params;
        if (!m_jsonParameters.is_null()) {
            from_json(m_jsonParameters, params);
        }
        return params;
    }



} // namespace nexo::assets
