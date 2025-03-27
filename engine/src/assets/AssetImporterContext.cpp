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
     * Assigns the specified asset as the main asset used within the context.
     *
     * @param asset Pointer to the asset to be designated as the main asset.
     */
    void AssetImporterContext::setMainAsset(IAsset* asset)
    {
        m_mainAsset = asset;
    }

    /**
     * @brief Retrieves the main asset from the context.
     *
     * Returns a pointer to the main asset currently stored in the context.
     *
     * @return IAsset* Pointer to the main asset, or nullptr if not set.
     */
    IAsset* AssetImporterContext::getMainAsset() const
    {
        return m_mainAsset;
    }

    /**
     * @brief Adds an asset dependency to the context.
     *
     * Appends the provided asset dependency reference to the list of dependencies managed by the importer context.
     *
     * @param dependency The dependency to be added.
     */
    void AssetImporterContext::addDependency(const GenericAssetRef& dependency)
    {
        m_dependencies.push_back(dependency);
    }

    /**
     * @brief Retrieves the list of asset dependencies.
     *
     * Returns a constant reference to the vector containing the asset dependencies managed by the context.
     *
     * @return const std::vector<GenericAssetRef>& Reference to the vector of dependencies.
     */
    const std::vector<GenericAssetRef>& AssetImporterContext::getDependencies() const
    {
        return m_dependencies;
    }

    /**
     * @brief Sets the JSON parameters for the asset importer context.
     *
     * Assigns the provided JSON object to configure the parameters used for asset importing.
     *
     * @param params The JSON object containing the configuration parameters.
     */
    void AssetImporterContext::setParameters(const nlohmann::json& params)
    {
        m_jsonParameters = params;
    }

    /**
     * @brief Retrieves the JSON parameters for the asset importer context.
     *
     * Returns a JSON object representing the current configuration parameters.
     *
     * @return A JSON object containing the parameters.
     */
    nlohmann::json AssetImporterContext::getParameters() const
    {
        return m_jsonParameters;
    }

} // namespace nexo::assets
