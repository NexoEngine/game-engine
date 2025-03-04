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


namespace nexo::assets {
    struct AssetDependency;

    class AssetCatalog;




    /**
     * @class AssetImporterContext
     *
     * @brief Context class for asset importers.
     */
    struct AssetImporterContext {
        // Import from file, importer should open and read the file
        struct ImporterFileInput {
            std::filesystem::path filePath; //< File path to import
        };

        // Import from memory buffer, importer should read from the buffer
        struct ImporterMemoryInput {
            std::vector<uint8_t> memoryData; //< Memory buffer
            std::optional<std::string> fileExtension;       //< For format detection with memory sources
        };

        using ImporterInputVariant = std::variant<ImporterFileInput, ImporterMemoryInput>;

        ImporterInputVariant input;                        //< Input data for the importer
        std::shared_ptr<const IAsset> parentAsset; //< Parent asset (if any)
        AssetLocation location;                    //< Future location of the asset in the catalog

        private:
            std::shared_ptr<IAsset> m_mainAsset; //< Main asset being imported, resulting asset (MUST be set by importer)
            std::vector<AssetDependency> m_dependencies; //< Dependencies to import
            nlohmann::json m_jsonParameters; //< JSON parameters for the importer


        public:

            AssetImporterContext() = default;
            ~AssetImporterContext() = default;


            void setMainAssetData(IAsset *data) {
                m_mainAsset = std::shared_ptr<IAsset>(data);
            }

            [[nodiscard]] GenericAssetRef getMainAsset() const {
                return GenericAssetRef(m_mainAsset);
            }

            template <typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            [[nodiscard]] GenericAssetRef addDependency(const ImporterInputVariant& input);

            [[nodiscard]] const std::vector<AssetDependency>& getDependencies() const {
                return m_dependencies;
            }

        template <typename ParamType>
        requires std::derived_from<ParamType, ImportParametersBase>
        void setParameters(const ParamType& params) {
                        m_jsonParameters = params.toJson();
                    }

        void setParameters(const nlohmann::json& params) {
            m_jsonParameters = params;
        }

        template <typename ParamType>
            requires std::derived_from<ParamType, ImportParametersBase>
        [[nodiscard]] ParamType getParameters() const {
                ParamType params;
                if (!m_jsonParameters.is_null()) {
                    params.fromJson(m_jsonParameters);
                }
                return params;
            }

        [[nodiscard]] nlohmann::json getParameters() const {
            return m_jsonParameters;
        }



    };

    /**
     * @brief Represents a dependency asset to be imported
     */
    struct AssetDependency {
        GenericAssetRef asset;    //< Reference to this dependency asset (to later update the assetData inside)
        AssetImporterContext ctx; //< Import context for this dependency
    };

    template<typename AssetType> requires std::derived_from<AssetType, IAsset>
    GenericAssetRef AssetImporterContext::addDependency(const ImporterInputVariant& input)
    {
        auto asset = std::make_shared<AssetType>();
        auto ref = GenericAssetRef(asset);

        std::string depLocation = std::format("{}_{}{}",
            location.getFullLocation(), AssetTypeNames[asset->getType()], std::to_string(m_dependencies.size()));

        const AssetImporterContext ctx {
            .input = input,
            .parentAsset = m_mainAsset,
            .location = AssetLocation(depLocation),
        };

        const AssetDependency dep {
            .asset = ref,
            .ctx = ctx
        };
        m_dependencies.push_back(dep);
        return ref;
    }



} // namespace nexo::assets
