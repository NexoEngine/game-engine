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
#include "ImNexo/Elements.hpp"
#include "assets/Asset.hpp"
#include "assets/AssetCatalog.hpp"
#include "assets/AssetRef.hpp"
#include "context/ThumbnailCache.hpp"

namespace nexo::editor {

    static constexpr ImU32 getAssetTypeOverlayColor(const assets::AssetType type)
    {
        switch (type) {
            case assets::AssetType::TEXTURE:
                return IM_COL32(60, 40, 40, 255);
            case assets::AssetType::MODEL:
                return IM_COL32(40, 60, 40, 255);
            case assets::AssetType::MATERIAL:
                return IM_COL32(40, 40, 60, 255);
            case assets::AssetType::UNKNOWN:
            case assets::AssetType::FONT:
            case assets::AssetType::MUSIC:
            case assets::AssetType::SCRIPT:
            case assets::AssetType::SHADER:
            case assets::AssetType::SOUND:
            default:
                return IM_COL32(0, 0, 0, 0);
        }
    }

    static void calculateGridLayout(LayoutSettings& layout)
    {
        const float availWidth = ImGui::GetContentRegionAvail().x;

        layout.size.columnCount = std::max(static_cast<int>(availWidth / layout.size.itemStep.x), 1);
        layout.size.itemSize    = ImVec2(layout.size.iconSize + ImGui::GetFontSize() * 1.5f, // width
                                         layout.size.iconSize + ImGui::GetFontSize() * 1.7f  // height
           );
        layout.size.itemStep    = ImVec2(layout.size.itemSize.x + static_cast<float>(layout.size.iconSpacing),
                                         layout.size.itemSize.y + static_cast<float>(layout.size.iconSpacing));
    }

    static void drawAssetThumbnail(const assets::GenericAssetRef& asset, const LayoutSettings& layout,
                                   const AssetLayoutParams& params, const bool isSelected)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        const ImU32 bgColor = isSelected ? layout.color.selectedBoxColor : layout.color.thumbnailBg;
        ImNexo::ButtonBorder(bgColor, bgColor, bgColor);

        if (const ImTextureID textureId = ThumbnailCache::getInstance().getThumbnail(asset); !textureId) {
            drawList->AddRectFilled(params.itemPos, params.thumbnailEnd, layout.color.thumbnailBg);
        } else {
            constexpr float padding = 4.0f;
            const ImVec2 imageStart(params.itemPos.x + padding, params.itemPos.y + padding);
            const ImVec2 imageEnd(params.thumbnailEnd.x - padding, params.thumbnailEnd.y - padding);

            drawList->AddImage(textureId, imageStart, imageEnd, ImVec2(0, 1), ImVec2(1, 0),
                               IM_COL32(255, 255, 255, 255));
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
            if (ImGui::CalcTextSize(displayText.c_str()).x <= availableTextWidth) return;
        }

        displayText = ellipsis;
    }

    void AssetManagerWindow::drawAssetTitle(const std::shared_ptr<assets::IAsset>& assetData,
                                            const AssetLayoutParams& params) const
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        const float titleAreaHeight    = params.itemSize.y * (1.0f - GridLayoutSizes::THUMBNAIL_HEIGHT_RATIO);
        const float titlePadding       = std::max(2.0f, titleAreaHeight * 0.1f);
        const float availableTextWidth = params.itemSize.x - (titlePadding * 2);

        const ImU32 titleBgColor = getAssetTypeOverlayColor(assetData->getType());

        // title background
        drawList->AddRectFilled(ImVec2(params.itemPos.x, params.thumbnailEnd.y),
                                ImVec2(params.itemEnd.x, params.itemEnd.y), titleBgColor);

        const std::string assetName = assetData->getMetadata().location.getName().data();
        const ImVec2 fullTextSize   = ImGui::CalcTextSize(assetName.c_str());
        std::string displayText     = assetName;

        // Crop text if it's too wide
        if (fullTextSize.x > availableTextWidth) cropText(assetName, displayText, availableTextWidth);

        const ImVec2 displayTextSize = ImGui::CalcTextSize(displayText.c_str());
        const ImVec2 textPos(params.itemPos.x + (params.itemSize.x - displayTextSize.x) * 0.5f,
                             params.thumbnailEnd.y + (titleAreaHeight - displayTextSize.y) * 0.5f);
        drawList->AddText(textPos, m_layout.color.titleText, displayText.c_str());
    }

    void AssetManagerWindow::drawAsset(const assets::GenericAssetRef& asset, const unsigned int index,
                                       const ImVec2& itemPos, const ImVec2& itemSize)
    {
        const auto assetData = asset.lock();
        if (!assetData) return;

        ImGui::PushID(static_cast<int>(index));
        ImGui::SetCursorScreenPos(itemPos);

        const bool isSelected = m_selectedAssets.contains(index);
        const auto itemEnd    = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y);
        const auto thumbnailEnd =
            ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y * GridLayoutSizes::THUMBNAIL_HEIGHT_RATIO);
        const AssetLayoutParams assetLayoutParams{itemPos, itemSize, itemEnd, thumbnailEnd};

        drawAssetThumbnail(asset, m_layout, assetLayoutParams, isSelected);
        drawAssetTitle(assetData, assetLayoutParams);


        static bool clicked = false; // ImGui::InvisibleButton("##item", itemSize);
        ImGui::Selectable("###asset", clicked, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_Highlight,
                          itemSize);
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            clicked = !clicked;
        }
        if (clicked) {
            handleSelection(index, isSelected);
            clicked = false;
        }

        const bool isHovered  = ImGui::IsItemHovered();
        if (isHovered) {
            m_hoveredAsset = assetData;
        } else {
            m_hoveredAsset.reset();
        }

        handleRightClickOnAsset();

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            AssetDragDropPayload payload;
            payload.type         = assetData->getType();
            payload.id           = assetData->getID();
            const auto& fullPath = assetData->getMetadata().location.getFullLocation();
            const auto& name     = assetData->getMetadata().location.getName().data();
            std::strncpy(payload.path, fullPath.c_str(), sizeof(payload.path) - 1);
            payload.path[sizeof(payload.path) - 1] = '\0';
            std::strncpy(payload.name, name.c_str(), sizeof(payload.name) - 1);
            payload.name[sizeof(payload.name) - 1] = '\0';

            ImGui::SetDragDropPayload("ASSET_DRAG", &payload, sizeof(payload));
            ImTextureID textureID = ThumbnailCache::getInstance().getThumbnail(asset);
            if (textureID) ImGui::Image(textureID, {64, 64}, ImVec2(0, 1), ImVec2(1, 0));

            ImGui::EndDragDropSource();
        }

        ImGui::PopID();
    }

    void AssetManagerWindow::drawFolderIcon(const AssetLayoutParams& params) const
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        constexpr float padding = 10.0f;

        const float availWidth  = params.thumbnailEnd.x - params.itemPos.x - (padding * 2.0f);
        const float availHeight = params.thumbnailEnd.y - params.itemPos.y - (padding * 2.0f);

        const float displaySize = std::min(availWidth, availHeight);

        const float xOffset = (availWidth - displaySize) * 0.5f + padding;
        const float yOffset = (availHeight - displaySize) * 0.5f + padding;

        const ImVec2 imageStart(params.itemPos.x + xOffset, params.itemPos.y + yOffset);
        const ImVec2 imageEnd(imageStart.x + displaySize, imageStart.y + displaySize);

        if (const ImTextureID folderIconTexture = getIconTexture(m_folderIcon)) {
            drawList->AddImage(folderIconTexture, imageStart, imageEnd, ImVec2(0, 1), // UV0 (top-left)
                               ImVec2(1, 0),                                          // UV1 (bottom-right)
                               IM_COL32(255, 255, 255, 255)                           // White tint for default color
            );
        }
    }

    static void drawFolderTitle(const std::string& folderName, const LayoutSettings& layout,
                                const AssetLayoutParams& params)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        constexpr ImU32 titleBgColor = IM_COL32(0, 0, 0, 0);
        const float titleAreaHeight  = params.itemSize.y * (1.0f - GridLayoutSizes::THUMBNAIL_HEIGHT_RATIO);

        drawList->AddRectFilled(ImVec2(params.itemPos.x, params.thumbnailEnd.y),
                                ImVec2(params.itemEnd.x, params.itemEnd.y), titleBgColor);

        const ImVec2 textSize = ImGui::CalcTextSize(folderName.c_str());
        const float textY     = params.thumbnailEnd.y + ((titleAreaHeight - textSize.y) * 0.5f);
        const float textX     = params.itemPos.x + (params.itemSize.x - textSize.x) * 0.5f;

        drawList->AddText(ImVec2(textX, textY), layout.color.titleText, folderName.c_str());
    }

    void AssetManagerWindow::drawFolder(const std::string& folderPath, const std::string& folderName,
                                        const ImVec2& itemPos, const ImVec2& itemSize)
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const auto itemEnd   = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y);
        const auto thumbnailEnd =
            ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y * GridLayoutSizes::THUMBNAIL_HEIGHT_RATIO);
        const AssetLayoutParams folderLayoutParams{itemPos, itemSize, itemEnd, thumbnailEnd};

        ImGui::PushID(("folder_" + folderPath).c_str());
        ImGui::SetCursorScreenPos(itemPos);

        static bool isDoubleClicked = false;
        // Create an invisible button for the folder
        ImGui::Selectable("###folder", isDoubleClicked,
                          ImGuiSelectableFlags_AllowDoubleClick |
                              (m_folderActionState.folderName == folderName && m_hoveredFolder == folderName ?
                                   ImGuiSelectableFlags_Highlight :
                                   0),
                          itemSize);
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            isDoubleClicked = !isDoubleClicked;
        }
        if (isDoubleClicked) { // If double-clicked, navigate to the folder
            m_currentFolder = folderPath;
            isDoubleClicked = false;
            m_hoveredFolder.clear();
        }

        const bool isHovered = ImGui::IsItemHovered();
        if (isHovered) {
            // If hovered, set the hovered folder
            m_hoveredFolder = folderPath;
        } else if (m_hoveredFolder == folderPath) {
            // If not hovered and was previously hovered, clear it
            m_hoveredFolder.clear();
        }

        handleRightClickOnFolder();

        handleAssetDrop(folderPath);

        constexpr ImU32 bgColor = IM_COL32(0, 0, 0, 0);
        drawList->AddRectFilled(itemPos, itemEnd, bgColor, GridLayoutSizes::CORNER_RADIUS);
        drawFolderIcon(folderLayoutParams);
        drawFolderTitle(folderName, m_layout, folderLayoutParams);

        ImGui::PopID();
    }

    static std::vector<assets::GenericAssetRef> getFilteredAsset(const std::string_view currentFolder,
                                                                 const assets::AssetType selectedType)
    {
        std::vector<assets::GenericAssetRef> filtered;
        std::vector<assets::GenericAssetRef> assets = assets::AssetCatalog::getInstance().getAssets();

        for (const auto& ref : assets) {
            const auto d = ref.lock();
            if (!d) continue;
            const auto& assetPath = d->getMetadata().location.getPath();
            if (assetPath == INTERNAL_FOLDER_PREFIX) continue;
            if (selectedType != assets::AssetType::UNKNOWN && d->getType() != selectedType) continue;
            if (assetPath == currentFolder) filtered.push_back(ref);
        }

        return filtered;
    }

    void AssetManagerWindow::drawAssetsGrid()
    {
        calculateGridLayout(m_layout);

        const ImVec2 startPos                               = ImGui::GetCursorScreenPos();
        const auto subfolders                               = m_folderManager.getChildren(m_currentFolder);
        const std::vector<assets::GenericAssetRef> filtered = getFilteredAsset(m_currentFolder, m_selectedType);
        if (filtered.empty() && subfolders.empty()) {
            ImGui::Text("This folder is empty.");
            return;
        }

        const size_t totalItems = subfolders.size() + filtered.size();
        const int columnCount   = m_layout.size.columnCount;
        const auto rows         = static_cast<int>((totalItems + columnCount - 1) / columnCount);

        ImGuiListClipper clipper;
        clipper.Begin(rows, m_layout.size.itemStep.y);

        while (clipper.Step()) {
            const unsigned int visibleStart = clipper.DisplayStart * columnCount;
            const unsigned int visibleEnd   = std::min(clipper.DisplayEnd * columnCount, static_cast<int>(totalItems));

            for (unsigned int i = visibleStart; i < visibleEnd; ++i) {
                const unsigned int col = i % columnCount;
                const unsigned int row = i / columnCount;
                ImVec2 itemPos{startPos.x + static_cast<float>(col) * m_layout.size.itemStep.x,
                               startPos.y + static_cast<float>(row) * m_layout.size.itemStep.y};

                if (i < subfolders.size()) {
                    drawFolder(subfolders[i].first, subfolders[i].second, itemPos, m_layout.size.itemSize);
                } else {
                    const auto assetIdx = i - static_cast<unsigned int>(subfolders.size());
                    drawAsset(filtered[assetIdx], assetIdx, itemPos, m_layout.size.itemSize);
                }
            }
        }
        clipper.End();
    }
} // namespace nexo::editor
