//// Thumbnail.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/06/2025
//  Description: Source file for the thumbnail rendering of the different asset types
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"

namespace nexo::editor {

    void AssetManagerWindow::drawTextureThumbnail(ImDrawList *drawList, ImTextureID textureId, const ImVec2 &itemPos, const ImVec2& thumbnailEnd)
    {
        if (textureId) {
            const float padding = 4.0f;
            ImVec2 imageStart(itemPos.x + padding, itemPos.y + padding);
            ImVec2 imageEnd(thumbnailEnd.x - padding, thumbnailEnd.y - padding);

            drawList->AddImage(
                textureId,
                imageStart,
                imageEnd,
                ImVec2(0, 1),     // UV0 (top-left)
                ImVec2(1, 0),     // UV1 (bottom-right)
                IM_COL32(255, 255, 255, 255) // White tint
            );
        } else
            drawList->AddRectFilled(itemPos, thumbnailEnd, m_layout.color.thumbnailBg);
    }
}
