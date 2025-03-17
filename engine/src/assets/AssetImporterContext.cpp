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

    void AssetImporterContext::setMainAsset(IAsset* asset)
    {
        m_mainAsset = asset;
    }

    IAsset* AssetImporterContext::getMainAsset() const
    {
        return m_mainAsset;
    }

    void AssetImporterContext::addDependency(const GenericAssetRef& dependency)
    {
        m_dependencies.push_back(dependency);
    }

    const std::vector<GenericAssetRef>& AssetImporterContext::getDependencies() const
    {
        return m_dependencies;
    }

    void AssetImporterContext::setParameters(const nlohmann::json& params)
    {
        m_jsonParameters = params;
    }

    nlohmann::json AssetImporterContext::getParameters() const
    {
        return m_jsonParameters;
    }

} // namespace nexo::assets
