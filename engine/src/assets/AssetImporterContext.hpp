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

#include "Asset.hpp"
#include "AssetCatalog.hpp"
#include "AssetImporterInput.hpp"
#include "json.hpp"

namespace nexo::assets {
    struct AssetDependency;

    class AssetImporterBase;
    class AssetImporter;

    /**
     * @class AssetImporterContext
     *
     * @brief Context class for asset importers.
     */
    struct AssetImporterContext {
        public:
            ImporterInputVariant input;                          //< Input data for the importer
            AssetLocation location = AssetLocation("default"); //< Future location of the asset in the catalog

            AssetImporterContext() = default;
            ~AssetImporterContext() = default;

            /**
             * @brief Set the main asset for this context
             * @param asset The main asset
             * @note This method must be called by the importer to set the main asset data
             */
            void setMainAsset(std::unique_ptr<IAsset> asset);

            /**
             * @brief Get the main asset data for this context
             * @return The main asset data
             */
            [[nodiscard]] const std::unique_ptr<IAsset>& getMainAsset() const;

            /**
             * @brief Release the main asset data for this context
             * @warning This function will take ownership of the mainAsset ptr
             *          The mainAsset ptr will become NULL in the context
             * @return The main asset data
             */
            [[nodiscard]] std::unique_ptr<IAsset> releaseMainAsset();

            /**
             * @brief Add dependency to main asset.
             *
             * Main asset will be considered parent of these dependencies.
             * @param dependency The dependency to add
             */
            void addDependency(const GenericAssetRef& dependency);

            /**
             * @brief Get a vector of all dependencies for this context
             * @return A vector of all dependencies' asset references
             */
            [[nodiscard]] const std::vector<GenericAssetRef>& getDependencies() const;

            template <typename ParamType>
                requires JSONSerializable<ParamType>
            void setParameters(const ParamType& params);

            void setParameters(const json& params);

            template <typename ParamType>
                requires JSONSerializable<ParamType>
            [[nodiscard]] ParamType getParameters() const;

            [[nodiscard]] json getParameters() const;


            /**
             * @brief Generates a unique dependency asset location.
             *
             * This method creates a candidate asset location by using the current location as a base, then sets a unique name
             * by incrementing an internal dependency counter and formatting the name with a dedicated formatting function.
             * If the generated location already exists in the asset catalog, the method continues to update the candidate location
             * until a unique one is found or the maximum allowed dependency count is exceeded. In the latter case, an error is logged,
             * and the last candidate is returned.
             *
             * @tparam AssetType The type of the asset for which the location is being generated.
             * @return AssetLocation A unique location for the dependency asset.
             */
            template <typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            AssetLocation genUniqueDependencyLocation();

            /**
             * @brief Formats a unique asset name.
             *
             * Constructs a unique asset name by combining a base name, the asset type name, and a unique identifier.
             * The resulting format is: `<base_name>_<asset_type_name><unique_id>`, where the asset type name is
             * derived from the provided asset type.
             *
             * @param name The base name of the asset.
             * @param type The type of the asset.
             * @param id A unique identifier appended to ensure the name is distinct.
             * @return AssetName The uniquely formatted asset name.
             */
            static AssetName formatUniqueName(const std::string& name, const AssetType type, unsigned int id)
            {
                return AssetName(std::format("{}_{}{}", name, getAssetTypeName(type), id));
            }

        private:
            std::unique_ptr<IAsset> m_mainAsset = nullptr;         //< Main asset being imported, resulting asset (MUST be set by importer)
            std::vector<GenericAssetRef> m_dependencies; //< Dependencies to import
            json m_jsonParameters;             //< JSON parameters for the importer
            unsigned int m_depUniqueId = 0;            //< Unique ID for the dependency name
    };


    template <typename AssetType>
        requires std::derived_from<AssetType, IAsset>
    AssetLocation AssetImporterContext::genUniqueDependencyLocation()
    {
        auto depLoc = AssetLocation(location.getFullLocation());
        depLoc.setName(formatUniqueName(location.getName().data(), AssetType::TYPE, ++m_depUniqueId));

        if (!AssetCatalog::getInstance().getAsset(depLoc))
            return depLoc;

        // If the location already exists, we need to generate a new one
        while (AssetCatalog::getInstance().getAsset(depLoc)) {
            depLoc.setName(formatUniqueName(location.getName().data(), AssetType::TYPE, ++m_depUniqueId));
            if (m_depUniqueId > ASSET_MAX_DEPENDENCIES) {
                // Prevent infinite loop
                LOG(NEXO_ERROR, "Failed to generate unique name for asset: {}: couldn't find unique id", depLoc.getFullLocation());
                break;
            }
        }
        return depLoc;
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
