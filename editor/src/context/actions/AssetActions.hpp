//// AssetActions.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        25-07-2025
//  Description: Header file for asset-related actions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Action.hpp"
#include "assets/Asset.hpp"
#include <string>

namespace nexo::editor {

    /**
     * @brief Action for moving assets between folders in the Asset Manager
     *
     * Handles undo/redo for asset location changes
     */
    class AssetMoveAction final : public Action {
    public:
        AssetMoveAction(assets::AssetID assetId,
                       const std::string& fromPath,
                       const std::string& toPath);

        void redo() override;
        void undo() override;

    private:
        assets::AssetID m_assetId;
        std::string m_fromPath;
        std::string m_toPath;
    };

} // namespace nexo::editor