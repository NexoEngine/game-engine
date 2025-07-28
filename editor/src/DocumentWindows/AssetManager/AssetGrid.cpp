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
#include "assets/Asset.hpp"
#include "assets/AssetCatalog.hpp"
#include "context/ThumbnailCache.hpp"
#include "ImNexo/Elements.hpp"

namespace nexo::editor {

    static constexpr ImU32 getAssetTypeOverlayColor(const assets::AssetType type)
    {
        switch (type) {
            case assets::AssetType::TEXTURE: return IM_COL32(60, 40, 40, 255);
            case assets::AssetType::MODEL: return IM_COL32(40, 60, 40, 255);
            case assets::AssetType::MATERIAL: return IM_COL32(40, 40, 60, 255);
            default: return IM_COL32(0, 0, 0, 0);
        }
    }

    static void calculateGridLayout(LayoutSettings &layout)
    {
        const float availWidth = ImGui::GetContentRegionAvail().x;

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

    static void drawAssetThumbnail(
        const assets::GenericAssetRef& asset,
        const LayoutSettings &layout,
        const AssetLayoutParams& params,
        const bool isSelected
    ) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        const ImU32 bgColor = isSelected ? layout.color.selectedBoxColor : layout.color.thumbnailBg;
        ImNexo::ButtonBorder(bgColor, bgColor, bgColor);

        if (const ImTextureID textureId = ThumbnailCache::getInstance().getThumbnail(asset); !textureId) {
            drawList->AddRectFilled(params.itemPos, params.thumbnailEnd, layout.color.thumbnailBg);
        } else {
            constexpr float padding = 4.0f;
            const ImVec2 imageStart(params.itemPos.x + padding, params.itemPos.y + padding);
            const ImVec2 imageEnd(params.thumbnailEnd.x - padding, params.thumbnailEnd.y - padding);

            drawList->AddImage(
                textureId,
                imageStart,
                imageEnd,
                ImVec2(0, 1),
                ImVec2(1, 0),
                IM_COL32(255, 255, 255, 255)
            );
        }
    }

    static void cropText(const std::string& assetName, std::string& displayText, float availableTextWidth)
    {
        const std::string ellipsis = "...";
        if (ImGui::CalcTextSize(assetName.c_str()).x <= availableTextWidth) {
            displayText = assetName;
            return;
        }

        for (size_t length = assetName.size(); length > 0; --length) {
            displayText = assetName.substr(0, length) + ellipsis;
            if (ImGui::CalcTextSize(displayText.c_str()).x <= availableTextWidth)
                return;
        }

        displayText = ellipsis;
    }

    void AssetManagerWindow::drawAssetTitle(
        const std::shared_ptr<assets::IAsset>& assetData,
        const AssetLayoutParams& params,
        bool isHovered
    ) const {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        const float titleAreaHeight = params.itemSize.y * (1.0f - m_layout.size.THUMBNAIL_HEIGHT_RATIO);
        const float titlePadding = std::max(2.0f, titleAreaHeight * 0.1f);
        const float availableTextWidth = params.itemSize.x - (titlePadding * 2);

        ImU32 titleBgColor = (isHovered) ?
                m_layout.color.titleBgHovered :
                getAssetTypeOverlayColor(assetData->getType());

        // title background
        drawList->AddRectFilled(
            ImVec2(params.itemPos.x, params.thumbnailEnd.y),
            ImVec2(params.itemEnd.x, params.itemEnd.y),
            titleBgColor
        );

        const std::string assetName = assetData->getMetadata().location.getName().data();
        const ImVec2 fullTextSize = ImGui::CalcTextSize(assetName.c_str());
        std::string displayText = assetName;

        // Crop text if it's too wide
        if (fullTextSize.x > availableTextWidth)
            cropText(assetName, displayText, availableTextWidth);

        const ImVec2 displayTextSize = ImGui::CalcTextSize(displayText.c_str());
        const ImVec2 textPos(
            params.itemPos.x + (params.itemSize.x - displayTextSize.x) * 0.5f,
            params.thumbnailEnd.y + (titleAreaHeight - displayTextSize.y) * 0.5f
        );
        drawList->AddText(textPos, m_layout.color.titleText, displayText.c_str());

        if (isHovered) {
            if (fullTextSize.x > availableTextWidth)
                ImGui::SetTooltip("%s\n%s", assetName.c_str(), assetData->getMetadata().location.getFullLocation().c_str());
            else
                ImGui::SetTooltip("%s", assetData->getMetadata().location.getFullLocation().c_str());
        }
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

        ImGui::PushID(static_cast<int>(index));
        ImGui::SetCursorScreenPos(itemPos);

        const bool clicked = ImGui::InvisibleButton("##item", itemSize);
        const bool isHovered = ImGui::IsItemHovered();
        const bool isSelected = m_selectedAssets.contains(index);
        const auto itemEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y);
        const auto thumbnailEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y * m_layout.size.THUMBNAIL_HEIGHT_RATIO);
        const AssetLayoutParams assetLayoutParams{itemPos, itemSize, itemEnd, thumbnailEnd};

        drawAssetThumbnail(asset, m_layout, assetLayoutParams, isSelected);
        drawAssetTitle(assetData, assetLayoutParams, isHovered);

        if (clicked)
            handleSelection(index, isSelected);

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            AssetDragDropPayload payload;
            payload.type = assetData->getType();
            payload.id = assetData->getID();
            payload.path = assetData->getMetadata().location.getFullLocation();
            payload.name = assetData->getMetadata().location.getName().data();

            ImGui::SetDragDropPayload("ASSET_DRAG", &payload, sizeof(payload));
            ImTextureID textureID = ThumbnailCache::getInstance().getThumbnail(asset);
            if (textureID)
                ImGui::Image(textureID, {64, 64}, ImVec2(0, 1), ImVec2(1, 0));

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

        if (const ImTextureID folderIconTexture = getIconTexture(m_folderIcon)) {
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
        auto subfolders = m_folderManager.getChildren(m_currentFolder);

        std::vector<assets::GenericAssetRef> filtered;
        for (auto& ref : assets::AssetCatalog::getInstance().getAssets()) {
            const auto d = ref.lock();
            if (!d)
                continue;
            const auto& folder = d->getMetadata().location.getPath();
            if (folder == "_internal")
                continue;
            if (m_selectedType != assets::AssetType::UNKNOWN && d->getType() != m_selectedType)
                continue;
            if (folder == m_currentFolder)
                filtered.push_back(ref);
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
