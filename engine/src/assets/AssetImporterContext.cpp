//// AssetImporterContext.cpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        02/03/2025
//  Description: Implementation file for the asset importer context class
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetImporterContext.hpp"

namespace nexo::assets {

    /**
     * @brief Sets the main asset for the importer context.
     *
     * This method assigns the provided asset pointer as the main asset, updating the context for further asset operations.
     *
     * @param asset Pointer to the asset to be set as the main asset.
     */
    void AssetImporterContext::setMainAsset(IAsset* asset)
    {
        m_mainAsset = asset;
    }

    /**
     * @brief Retrieves the main asset.
     *
     * Returns a pointer to the primary asset managed by the context. This asset is expected
     * to have been set previously using the setMainAsset method. If no asset has been assigned,
     * the function returns a null pointer.
     *
     * @return A pointer to the main IAsset instance, or nullptr if unset.
     */
    IAsset* AssetImporterContext::getMainAsset() const
    {
        return m_mainAsset;
    }

    /**
     * @brief Appends a dependency reference to the asset's dependency list.
     *
     * Adds the provided dependency reference to the internal list of dependencies.
     *
     * @param dependency Reference to the asset dependency to include.
     */
    void AssetImporterContext::addDependency(const GenericAssetRef& dependency)
    {
        m_dependencies.push_back(dependency);
    }

    /**
     * @brief Retrieves all asset dependencies in the context.
     *
     * Returns a constant reference to the vector containing all dependencies registered
     * with the asset importer context.
     *
     * @return const std::vector<GenericAssetRef>& Reference to the vector of asset dependencies.
     */
    const std::vector<GenericAssetRef>& AssetImporterContext::getDependencies() const
    {
        return m_dependencies;
    }

    /**
     * @brief Sets the asset importer context parameters.
     *
     * Assigns the provided JSON object to the internal parameters used for asset importing.
     *
     * @param params A JSON object containing the configuration parameters.
     */
    void AssetImporterContext::setParameters(const nlohmann::json& params)
    {
        m_jsonParameters = params;
    }

    /**
     * @brief Retrieves the JSON configuration parameters.
     *
     * Returns the current JSON object that stores the asset importer's configuration parameters.
     *
     * @return nlohmann::json The JSON parameters.
     */
    nlohmann::json AssetImporterContext::getParameters() const
    {
        return m_jsonParameters;
    }

} // namespace nexo::assets
