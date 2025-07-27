//// Utils.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        27/05/2025
//  Description: Source file for the utils function of the asset manager
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"

namespace nexo::editor {

    ImTextureID AssetManagerWindow::getFolderIconTexture() const
    {
        if (const auto texRef = m_folderIcon.lock()) {
            const auto &texData = texRef->getData();
            if (texData && texData->texture) {
                return texData->texture->getId();
            }
        }
        return 0;
    }
}
