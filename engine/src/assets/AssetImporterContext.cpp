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
     * @brief Sets the main asset for the asset importer context.
     *
     * Assigns the provided asset pointer as the primary asset managed by the context.
     *
     * @param asset Pointer to the asset to be set as the main asset.
     */
    void AssetImporterContext::setMainAsset(IAsset* asset)
    {
        m_mainAsset = asset;
    }

    /**
     * @brief Retrieves the main asset for the context.
     *
     * Returns a pointer to the primary asset managed within the context. If no main asset has been set, the returned pointer is nullptr.
     *
     * @return IAsset* Pointer to the main asset.
     */
    IAsset* AssetImporterContext::getMainAsset() const
    {
        return m_mainAsset;
    }

    /**
     * @brief Adds a dependency to the asset importer context.
     *
     * Appends the specified dependency reference to the internal list of dependencies,
     * ensuring that additional asset references required during asset import are tracked.
     *
     * @param dependency A reference to the asset dependency to be added.
     */
    void AssetImporterContext::addDependency(const GenericAssetRef& dependency)
    {
        m_dependencies.push_back(dependency);
    }

    /**
     * @brief Retrieves the asset dependency collection.
     *
     * Returns a constant reference to the vector of asset dependencies (stored as GenericAssetRef)
     * managed by this context. These dependencies are added via the addDependency method.
     *
     * @return A constant reference to the vector of asset dependencies.
     */
    const std::vector<GenericAssetRef>& AssetImporterContext::getDependencies() const
    {
        return m_dependencies;
    }

    /**
     * @brief Sets the configuration parameters for the asset importer context.
     *
     * Overwrites the current parameters with the provided JSON settings, which are used to configure asset importing.
     *
     * @param params A JSON object containing the configuration and metadata for asset importing.
     */
    void AssetImporterContext::setParameters(const nlohmann::json& params)
    {
        m_jsonParameters = params;
    }

    /**
     * @brief Retrieves the parameters for the asset importer context.
     *
     * This method returns the JSON object containing the current parameters, which can include configuration or settings related to asset management.
     *
     * @return A JSON object with the context parameters.
     */
    nlohmann::json AssetImporterContext::getParameters() const
    {
        return m_jsonParameters;
    }

} // namespace nexo::assets
