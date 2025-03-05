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

    void AssetImporterContext::setMainAssetData(IAsset* data)
    {
        m_mainAsset = std::shared_ptr<IAsset>(data);
    }

    GenericAssetRef AssetImporterContext::getMainAsset() const
    {
        return GenericAssetRef(m_mainAsset);
    }

    const std::vector<AssetDependency>& AssetImporterContext::getDependencies() const
    {
        return m_dependencies;
    }

    template<typename ParamType> requires std::derived_from<ParamType, ImportParametersBase>
    void AssetImporterContext::setParameters(const ParamType& params)
    {
        m_jsonParameters = params.toJson();
    }

    void AssetImporterContext::setParameters(const nlohmann::json& params)
    {
        m_jsonParameters = params;
    }

    template<typename ParamType> requires std::derived_from<ParamType, ImportParametersBase>
    ParamType AssetImporterContext::getParameters() const
    {
        ParamType params;
        if (!m_jsonParameters.is_null()) {
            params.fromJson(m_jsonParameters);
        }
        return params;
    }

    nlohmann::json AssetImporterContext::getParameters() const
    {
        return m_jsonParameters;
    }

} // namespace nexo::assets
