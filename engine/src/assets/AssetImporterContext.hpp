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
            AssetLocation location = AssetLocation("default"); /**
 * @brief Default constructor for AssetImporterContext.
 *
 * Initializes an AssetImporterContext instance with default values.
 */

            AssetImporterContext() = default;
            /**
 * @brief Default destructor for AssetImporterContext.
 *
 * This destructor is defaulted, using the compiler-generated implementation.
 * No custom cleanup or additional operations are performed.
 */
~AssetImporterContext() = default;

            /**
             * @brief Set the main asset for this context
             * @param asset The main asset
             * @note This method must be called by the importer to set the main asset data
             */
            void setMainAsset(IAsset* asset);

            /**
             * @brief Get the main asset data for this context
             * @return The main asset data
             */
            [[nodiscard]] IAsset* getMainAsset() const;

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

            template <typename AssetType>
                requires std::derived_from<AssetType, IAsset>
            AssetLocation genUniqueDependencyName();


        private:


            IAsset *m_mainAsset = nullptr;         //< Main asset being imported, resulting asset (MUST be set by importer)
            std::vector<GenericAssetRef> m_dependencies; //< Dependencies to import
            json m_jsonParameters;             //< JSON parameters for the importer
            unsigned int m_depUniqueId = 0;            //< Unique ID for the dependency name
    };


    template <typename AssetType>
        requires std::derived_from<AssetType, IAsset>
    /**
     * @brief Generates a unique asset location for a dependency.
     *
     * Combines the current asset location, asset type, and an incremented unique identifier to construct
     * a new asset location. The method checks the asset catalog for conflicts and iteratively adjusts
     * the name if an existing asset is found. If the maximum number of allowed dependencies is exceeded,
     * an error is logged and the last attempted location is returned.
     *
     * @return AssetLocation A unique asset location for the dependency.
     */
    AssetLocation AssetImporterContext::genUniqueDependencyName()
    {
        auto depLoc = AssetLocation(
            std::format("{}_{}{}", location.getFullLocation(), AssetTypeNames[AssetType::getType()], ++m_depUniqueId)
        );
        if (!AssetCatalog::getInstance().getAsset(depLoc))
            return depLoc;

        // If the location already exists, we need to generate a new one
        auto name = std::string(location.getName());
        while (AssetCatalog::getInstance().getAsset(depLoc)) {
            std::string newName = name + std::to_string(++m_depUniqueId);
            depLoc.setName(newName);
            if (m_depUniqueId > ASSET_MAX_DEPENDENCIES) {
                // Prevent infinite loop
                LOG(NEXO_ERROR, "Failed to generate unique name for asset: {}: couldn't find unique id", depLoc.getFullLocation());
                break;
            }
        }
        return depLoc;
    }

    template<typename ParamType> requires JSONSerializable<ParamType>
    /**
     * @brief Serializes the provided parameters to JSON format.
     *
     * Converts the given JSON serializable object into a JSON representation and stores it in the asset importer's parameter storage.
     *
     * @param params An object containing import parameters that is JSON serializable.
     */
    void AssetImporterContext::setParameters(const ParamType& params)
    {
        to_json(m_jsonParameters, params);
    }

    template<typename ParamType> requires JSONSerializable<ParamType>
    /**
     * @brief Retrieves the asset importer parameters.
     *
     * If JSON parameters have been set and are not null, this method converts them
     * to the templated parameter type using the `from_json` function. Otherwise, it
     * returns a default-constructed instance.
     *
     * @return ParamType The deserialized parameters.
     */
    ParamType AssetImporterContext::getParameters() const
    {
        ParamType params;
        if (!m_jsonParameters.is_null()) {
            from_json(m_jsonParameters, params);
        }
        return params;
    }



} // namespace nexo::assets
