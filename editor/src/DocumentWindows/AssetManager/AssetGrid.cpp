//// AssetGrid.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        27/07/2025
//  Description: Source file for the asset grid
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include "assets/AssetCatalog.hpp"
#include "context/ThumbnailCache.hpp"

namespace nexo::editor {

    static constexpr ImU32 getAssetTypeOverlayColor(const assets::AssetType type)
    {
        switch (type) {
            case assets::AssetType::TEXTURE: return IM_COL32(200, 70, 70, 255);
            case assets::AssetType::MODEL: return IM_COL32(70, 170, 70, 255);
            default: return IM_COL32(0, 0, 0, 0);
        }
    }

    static void calculateGridLayout(LayoutSettings &layout)
    {
        const float availWidth = ImGui::GetContentRegionAvail().x;

        // Sizes
        layout.size.columnCount = std::max(
            static_cast<int>(availWidth / layout.size.itemStep.x), 1
        );
        layout.size.itemSize = ImVec2(
            layout.size.iconSize + ImGui::GetFontSize() * 1.5f, // width
            layout.size.iconSize + ImGui::GetFontSize() * 1.7f  // height
        );
        layout.size.itemStep = ImVec2(
            layout.size.itemSize.x + static_cast<float>(layout.size.iconSpacing),
            layout.size.itemSize.y + static_cast<float>(layout.size.iconSpacing)
        );
    }

    void AssetManagerWindow::drawAsset(
        const assets::GenericAssetRef& asset,
        const unsigned int index,
        const ImVec2& itemPos,
        const ImVec2& itemSize
    ) {
        const auto assetData = asset.lock();
        if (!assetData)
            return;
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const auto itemEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y);

        ImGui::PushID(static_cast<int>(index));

        ImGui::SetCursorScreenPos(itemPos);

        const bool clicked = ImGui::InvisibleButton("##item", itemSize);
        const bool isHovered = ImGui::IsItemHovered();

        const bool isSelected = std::ranges::find(m_selectedAssets, index) != m_selectedAssets.end();
        const ImU32 bgColor = isSelected ? m_layout.color.thumbnailBgSelected : m_layout.color.thumbnailBg;
        drawList->AddRectFilled(itemPos, itemEnd, bgColor, m_layout.size.CORNER_RADIUS);

        if (isSelected) {
            // Draw a distinctive border around selected items
            drawList->AddRect(
                ImVec2(itemPos.x - 1, itemPos.y - 1),
                ImVec2(itemEnd.x + 1, itemEnd.y + 1),
                m_layout.color.selectedBoxColor,
                m_layout.size.CORNER_RADIUS,
                0,
                m_layout.size.SELECTED_BOX_THICKNESS
            );
        }

        // Draw thumbnail area
        const auto thumbnailEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y * m_layout.size.THUMBNAIL_HEIGHT_RATIO);

        if (const ImTextureID textureId = ThumbnailCache::getInstance().getThumbnail(asset); !textureId) {
            drawList->AddRectFilled(itemPos, thumbnailEnd, m_layout.color.thumbnailBg);
        } else {
            constexpr float padding = 4.0f;
            const ImVec2 imageStart(itemPos.x + padding, itemPos.y + padding);
            const ImVec2 imageEnd(thumbnailEnd.x - padding, thumbnailEnd.y - padding);

            drawList->AddImage(
                textureId,
                imageStart,
                imageEnd,
                ImVec2(0, 1),     // UV0 (top-left)
                ImVec2(1, 0),     // UV1 (bottom-right)
                IM_COL32(255, 255, 255, 255) // White tint
            );
        }

        // Draw type overlay (maybe later modify it to an icon)
        const auto overlayPos = ImVec2(thumbnailEnd.x - m_layout.size.OVERLAY_PADDING, itemPos.y + m_layout.size.OVERLAY_PADDING);
        const ImU32 overlayColor = getAssetTypeOverlayColor(assetData->getType());
        drawList->AddRectFilled(overlayPos, ImVec2(overlayPos.x + m_layout.size.OVERLAY_SIZE, overlayPos.y + m_layout.size.OVERLAY_SIZE), overlayColor);

        // Draw title
        const char *assetName = assetData->getMetadata().location.getName().c_str();
        const auto textPos = ImVec2(itemPos.x + (itemSize.x - ImGui::CalcTextSize(assetName).x) * 0.5f,
            thumbnailEnd.y + m_layout.size.TITLE_PADDING);

        // Background rectangle for text
        const ImU32 titleBgColor = isHovered ? m_layout.color.titleBgHovered : m_layout.color.titleBg;
        drawList->AddRectFilled(ImVec2(itemPos.x, thumbnailEnd.y), ImVec2(itemEnd.x, itemEnd.y), titleBgColor);
        drawList->AddText(textPos, m_layout.color.titleText, assetName);

        // Handle selection when clicked
        if (clicked)
            handleSelection(index, isSelected);

        // On Hover show asset location
        if (isHovered)
            ImGui::SetTooltip("%s", assetData->getMetadata().location.getFullLocation().c_str());

        // Handle drag source for assets
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            AssetDragDropPayload payload;
            payload.type = assetData->getType();
            payload.id = assetData->getID();
            payload.path = assetData->getMetadata().location.getFullLocation();
            payload.name = assetName;

            ImGui::SetDragDropPayload("ASSET_DRAG", &payload, sizeof(payload));

            // Show preview while dragging
            //TODO: Add asset preview thanks to thumbnail cache after rebasing
            if (assetData->getType() == assets::AssetType::TEXTURE) {
                const auto textureAsset = asset.as<assets::Texture>();
                if (const auto textureData = textureAsset.lock();
                    textureData && textureData->getData() && textureData->getData()->texture) {
                    const ImTextureID textureId = textureData->getData()->texture->getId();
                    ImGui::Image(textureId, {64, 64});
                }
            }

            ImGui::EndDragDropSource();
        }

        ImGui::PopID();
    }

    void AssetManagerWindow::drawFolder(
        const std::string& folderPath,
        const std::string& folderName,
        const ImVec2& itemPos,
        const ImVec2& itemSize
    ) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const auto itemEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y);

        ImGui::PushID(("folder_" + folderPath).c_str());

        ImGui::SetCursorScreenPos(itemPos);

        const bool clicked = ImGui::InvisibleButton("##folder", itemSize);
        const bool isHovered = ImGui::IsItemHovered();

        if (isHovered) {
            m_hoveredFolder = folderPath;
        } else if (m_hoveredFolder == folderPath) {
            m_hoveredFolder.clear();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG"))
            {
                const auto* data = static_cast<const AssetDragDropPayload *>(payload->Data);
                assets::AssetCatalog::getInstance().moveAsset(data->id, folderPath);
            }
            ImGui::EndDragDropTarget();
        }

        // Background - use hover color when hovered
        const ImU32 bgColor = isHovered ? m_layout.color.thumbnailBgHovered : IM_COL32(0, 0, 0, 0);
        drawList->AddRectFilled(itemPos, itemEnd, bgColor, m_layout.size.CORNER_RADIUS);

        const auto thumbnailEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y * m_layout.size.THUMBNAIL_HEIGHT_RATIO);

        // Calculate padding for the icon
        constexpr float padding = 10.0f;

        // Calculate available area dimensions
        const float availWidth = thumbnailEnd.x - itemPos.x - (padding * 2.0f);
        const float availHeight = thumbnailEnd.y - itemPos.y - (padding * 2.0f);

        // Maintain aspect ratio by using the smaller dimension
        const float displaySize = std::min(availWidth, availHeight);

        // Calculate centered position
        const float xOffset = (availWidth - displaySize) * 0.5f + padding;
        const float yOffset = (availHeight - displaySize) * 0.5f + padding;

        // Final image coordinates maintaining aspect ratio
        const ImVec2 imageStart(
            itemPos.x + xOffset,
            itemPos.y + yOffset
        );
        const ImVec2 imageEnd(
            imageStart.x + displaySize,
            imageStart.y + displaySize
        );

        // Draw folder PNG icon

        if (const ImTextureID folderIconTexture = getFolderIconTexture()) {
            drawList->AddImage(
                folderIconTexture,
                imageStart,
                imageEnd,
                ImVec2(0, 1),     // UV0 (top-left)
                ImVec2(1, 0),     // UV1 (bottom-right)
                IM_COL32(255, 255, 255, 255) // White tint for default color
            );
        }

        // Calculate text size to ensure it fits
        const ImVec2 textSize = ImGui::CalcTextSize(folderName.c_str());

        // Draw title background
        const ImU32 titleBgColor = isHovered ? m_layout.color.titleBgHovered : IM_COL32(0, 0, 0, 0);
        const float titleAreaHeight = itemSize.y * (1.0f - m_layout.size.THUMBNAIL_HEIGHT_RATIO);

        drawList->AddRectFilled(
            ImVec2(itemPos.x, thumbnailEnd.y),
            ImVec2(itemEnd.x, itemEnd.y),
            titleBgColor
        );

        // Position text with proper vertical alignment
        const float textY = thumbnailEnd.y + ((titleAreaHeight - textSize.y) * 0.5f);
        const float textX = itemPos.x + (itemSize.x - textSize.x) * 0.5f;

        drawList->AddText(
            ImVec2(textX, textY),
            m_layout.color.titleText,
            folderName.c_str()
        );

        if (clicked)
            m_currentFolder = folderPath; // Navigate into this folder

        ImGui::PopID();
    }

    void AssetManagerWindow::drawAssetsGrid()
    {
        calculateGridLayout(m_layout);

        const ImVec2 startPos = ImGui::GetCursorScreenPos();

        std::vector<std::pair<std::string,std::string>> subfolders;
        for (auto& [path,name] : m_folderStructure) {
            if (path.empty() || path.front() == '_')
                continue;

            if (m_currentFolder.empty()) {
                if (path.find('/') == std::string::npos)
                    subfolders.emplace_back(path, name);
            } else {
                if (std::string prefix = m_currentFolder + "/"; path.rfind(prefix, 0) == 0 &&
                                                                path.find('/', prefix.size()) == std::string::npos)
                {
                    subfolders.emplace_back(path, path.substr(prefix.size()));
                }
            }
        }

        std::vector<assets::GenericAssetRef> filtered;
        for (auto& ref : assets::AssetCatalog::getInstance().getAssets()) {
            if (const auto d = ref.lock()) {
                const auto& folder = d->getMetadata().location.getPath();
                if (folder == "_internal")
                    continue;
                if (m_selectedType != assets::AssetType::UNKNOWN &&
                    d->getType() != m_selectedType)  continue;

                if (folder == m_currentFolder)
                    filtered.push_back(ref);
            }
        }

        const size_t totalItems = subfolders.size() + filtered.size();
        ImGuiListClipper clipper;
        const auto rows = static_cast<int>((totalItems + m_layout.size.columnCount - 1) / m_layout.size.columnCount);
        clipper.Begin(rows, m_layout.size.itemStep.y);

        while (clipper.Step()) {
            for (int line = clipper.DisplayStart; line < clipper.DisplayEnd; ++line) {
                const unsigned int startIdx = line * m_layout.size.columnCount;
                const unsigned int endIdx   = std::min(startIdx + m_layout.size.columnCount, static_cast<unsigned int>(totalItems));

                for (unsigned int i = startIdx; i < endIdx; ++i) {
                    unsigned int col = i % m_layout.size.columnCount;
                    unsigned int row = i / m_layout.size.columnCount;
                    ImVec2 itemPos{
                        startPos.x + static_cast<float>(col) * m_layout.size.itemStep.x,
                        startPos.y + static_cast<float>(row) * m_layout.size.itemStep.y
                    };

                    if (i < static_cast<unsigned int>(subfolders.size())) {
                        // draw folder thumbnail
                        drawFolder(
                            subfolders[i].first,
                            subfolders[i].second,
                            itemPos,
                            m_layout.size.itemSize
                        );
                    } else {
                        // draw asset thumbnail
                        const auto assetIdx = i - static_cast<unsigned int>(subfolders.size());
                        drawAsset(
                            filtered[assetIdx],
                            assetIdx,
                            itemPos,
                            m_layout.size.itemSize
                        );
                    }
                }
            }
        }
        clipper.End();
    }
}
