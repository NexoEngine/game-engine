//// AssetActions.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        25-07-2025
//  Description: Source file for asset-related actions
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetActions.hpp"
#include <utility>
#include "assets/AssetCatalog.hpp"

namespace nexo::editor {

    AssetMoveAction::AssetMoveAction(const assets::AssetID assetId, std::string fromPath, std::string toPath)
        : m_assetId(assetId), m_fromPath(std::move(fromPath)), m_toPath(std::move(toPath))
    {}

    void AssetMoveAction::redo()
    {
        const auto& catalog = assets::AssetCatalog::getInstance();
        catalog.moveAsset(m_assetId, m_toPath);
    }

    void AssetMoveAction::undo()
    {
        const auto& catalog = assets::AssetCatalog::getInstance();
        catalog.moveAsset(m_assetId, m_fromPath);
    }

} // namespace nexo::editor
