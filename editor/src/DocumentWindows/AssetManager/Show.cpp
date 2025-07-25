//// Show.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the rendering of the asset manager window
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include "assets/Asset.hpp"
#include "assets/AssetCatalog.hpp"
#include "IconsFontAwesome.h"
#include "Path.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include "context/ThumbnailCache.hpp"
#include <cstring>
#include "Logger.hpp"
#include <imgui.h>

namespace nexo::editor {
    void AssetManagerWindow::drawMenuBar()
    {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                ImGui::SliderFloat("Icon Size", &m_layout.size.iconSize, 32.0f, 128.0f, "%.0f");
                ImGui::SliderInt("Icon Spacing", &m_layout.size.iconSpacing, 0, 32);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void AssetManagerWindow::calculateLayout(const float availWidth)
    {
        // Sizes
        m_layout.size.columnCount = std::max(static_cast<int>(availWidth / (m_layout.size.iconSize + static_cast<float>(m_layout.size.iconSpacing))), 1);
        m_layout.size.itemSize = ImVec2(m_layout.size.iconSize + ImGui::GetFontSize() * 1.5f, m_layout.size.iconSize + ImGui::GetFontSize() * 1.7f);
        m_layout.size.itemStep = ImVec2(m_layout.size.itemSize.x + static_cast<float>(m_layout.size.iconSpacing), m_layout.size.itemSize.y + static_cast<float>(m_layout.size.iconSpacing));

        // Colors
        m_layout.color.thumbnailBg = ImGui::GetColorU32(ImGuiCol_Button);
        m_layout.color.thumbnailBgHovered = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
        m_layout.color.thumbnailBgSelected = ImGui::GetColorU32(ImGuiCol_Header);
        m_layout.color.thumbnailBgSelectedHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);

        m_layout.color.selectedBoxColor = ImGui::GetColorU32(ImGuiCol_TabSelectedOverline);

        m_layout.color.titleBg = ImGui::GetColorU32(ImGuiCol_Header);
        m_layout.color.titleBgHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);
        m_layout.color.titleBgSelected = ImGui::GetColorU32(ImGuiCol_Header);
        m_layout.color.titleBgSelectedHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);

        m_layout.color.titleText = ImGui::GetColorU32(ImGuiCol_Text);
    }

    void AssetManagerWindow::handleSelection(int index, const bool isSelected)
    {
        if (ImGui::GetIO().KeyCtrl) {
            if (isSelected)
                m_selectedAssets.erase(index);
            else
                m_selectedAssets.insert(index);
        } else if (ImGui::GetIO().KeyShift) {
            const int latestSelected = m_selectedAssets.empty() ? 0 : *m_selectedAssets.rbegin();
            if (latestSelected <= index) {
                for (int i = latestSelected ; i <= index; ++i) {
                    m_selectedAssets.insert(i);
                }
            } else {
                for (int i = index; i <= latestSelected; ++i) {
                    m_selectedAssets.insert(i);
                }
            }
        } else {
            m_selectedAssets.clear();
            m_selectedAssets.insert(index);
        }
    }

    static ImU32 getAssetTypeOverlayColor(const assets::AssetType type)
    {
        switch (type) {
            case assets::AssetType::TEXTURE: return IM_COL32(200, 70, 70, 255);
            case assets::AssetType::MODEL: return IM_COL32(70, 170, 70, 255);
            default: return IM_COL32(0, 0, 0, 0);
        }
    }

    void AssetManagerWindow::drawAsset(
        const assets::GenericAssetRef& asset,
        const int index,
        const ImVec2& itemPos,
        const ImVec2& itemSize
    ) {
        auto assetData = asset.lock();
        if (!assetData)
            return;
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const auto itemEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y);

        ImGui::PushID(index);

        ImGui::SetCursorScreenPos(itemPos);

        bool clicked = ImGui::InvisibleButton("##item", itemSize);
        bool isHovered = ImGui::IsItemHovered();

        const bool isSelected = std::ranges::find(m_selectedAssets, index) != m_selectedAssets.end();
        const ImU32 bgColor = isSelected ? m_layout.color.thumbnailBgSelected : m_layout.color.thumbnailBg;
        drawList->AddRectFilled(itemPos, itemEnd, bgColor, m_layout.size.cornerRadius);

        if (isSelected) {
            // Draw a distinctive border around selected items
            drawList->AddRect(
                ImVec2(itemPos.x - 1, itemPos.y - 1),
                ImVec2(itemEnd.x + 1, itemEnd.y + 1),
                m_layout.color.selectedBoxColor,
                m_layout.size.cornerRadius,
                0,
                m_layout.size.selectedBoxThickness
            );
        }

        // Draw thumbnail area
        const auto thumbnailEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y * m_layout.size.thumbnailHeightRatio);

        ImTextureID textureId = ThumbnailCache::getInstance().getThumbnail(asset);
        if (!textureId) {
            drawList->AddRectFilled(itemPos, thumbnailEnd, m_layout.color.thumbnailBg);
        } else {
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
        }

        // Draw type overlay (maybe later modify it to an icon)
        const auto overlayPos = ImVec2(thumbnailEnd.x - m_layout.size.overlayPadding, itemPos.y + m_layout.size.overlayPadding);
        const ImU32 overlayColor = getAssetTypeOverlayColor(assetData->getType());
        drawList->AddRectFilled(overlayPos, ImVec2(overlayPos.x + m_layout.size.overlaySize, overlayPos.y + m_layout.size.overlaySize), overlayColor);

        // Draw title
        const char *assetName = assetData->getMetadata().location.getName().c_str();
        const auto textPos = ImVec2(itemPos.x + (itemSize.x - ImGui::CalcTextSize(assetName).x) * 0.5f,
            thumbnailEnd.y + m_layout.size.titlePadding);

        // Background rectangle for text
        ImU32 titleBgColor = isHovered ? m_layout.color.titleBgHovered : m_layout.color.titleBg;
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
                auto textureAsset = asset.as<assets::Texture>();
                auto textureData = textureAsset.lock();
                if (textureData && textureData->getData() && textureData->getData()->texture) {
                    ImTextureID textureId = textureData->getData()->texture->getId();
                    ImGui::Image(textureId, {64, 64});
                }
            }

            ImGui::EndDragDropSource();
        }

        ImGui::PopID();
    }

    ImTextureID AssetManagerWindow::getFolderIconTexture()
    {
        if (auto texRef = m_folderIcon.lock()) {
            auto &texData = texRef->getData();
            if (texData && texData->texture) {
                return texData->texture->getId();
            }
        }
        return 0;
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

        bool clicked = ImGui::InvisibleButton("##folder", itemSize);
        bool isHovered = ImGui::IsItemHovered();

        if (isHovered) {
            m_hoveredFolder = folderPath;
        } else if (m_hoveredFolder == folderPath) {
            m_hoveredFolder.clear();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG"))
            {
                const AssetDragDropPayload* data = (const AssetDragDropPayload*)payload->Data;
                assets::AssetCatalog::getInstance().moveAsset(data->id, folderPath);
            }
            ImGui::EndDragDropTarget();
        }

        // Background - use hover color when hovered
        ImU32 bgColor = isHovered ? m_layout.color.thumbnailBgHovered : IM_COL32(0, 0, 0, 0);
        drawList->AddRectFilled(itemPos, itemEnd, bgColor, m_layout.size.cornerRadius);

        const auto thumbnailEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y * m_layout.size.thumbnailHeightRatio);

        // Calculate padding for the icon
        const float padding = 10.0f;

        // Calculate available area dimensions
        float availWidth = thumbnailEnd.x - itemPos.x - (padding * 2.0f);
        float availHeight = thumbnailEnd.y - itemPos.y - (padding * 2.0f);

        // Maintain aspect ratio by using the smaller dimension
        float displaySize = std::min(availWidth, availHeight);

        // Calculate centered position
        float xOffset = (availWidth - displaySize) * 0.5f + padding;
        float yOffset = (availHeight - displaySize) * 0.5f + padding;

        // Final image coordinates maintaining aspect ratio
        ImVec2 imageStart(
            itemPos.x + xOffset,
            itemPos.y + yOffset
        );
        ImVec2 imageEnd(
            imageStart.x + displaySize,
            imageStart.y + displaySize
        );

        // Draw folder PNG icon
        ImTextureID folderIconTexture = getFolderIconTexture();

        if (folderIconTexture) {
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
        ImVec2 textSize = ImGui::CalcTextSize(folderName.c_str());

        // Draw title background
        ImU32 titleBgColor = isHovered ? m_layout.color.titleBgHovered : IM_COL32(0, 0, 0, 0);
        float titleAreaHeight = itemSize.y * (1.0f - m_layout.size.thumbnailHeightRatio);

        drawList->AddRectFilled(
            ImVec2(itemPos.x, thumbnailEnd.y),
            ImVec2(itemEnd.x, itemEnd.y),
            titleBgColor
        );

        // Position text with proper vertical alignment
        float textY = thumbnailEnd.y + ((titleAreaHeight - textSize.y) * 0.5f);
        float textX = itemPos.x + (itemSize.x - textSize.x) * 0.5f;

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
        ImVec2 startPos = ImGui::GetCursorScreenPos();

        // 1) Collect immediate subfolders of the current folder
        std::vector<std::pair<std::string,std::string>> subfolders;
        for (auto& [path,name] : m_folderStructure) {
            if (path.empty() || path.front() == '_')
                continue;

            if (m_currentFolder.empty()) {
                // root level = no slash in path
                if (path.find('/') == std::string::npos)
                    subfolders.emplace_back(path, name);
            } else {
                std::string prefix = m_currentFolder + "/";
                // immediate child: starts with "curr/" but has no further '/'
                if (path.rfind(prefix, 0) == 0 &&
                    path.find('/', prefix.size()) == std::string::npos)
                {
                    subfolders.emplace_back(path, path.substr(prefix.size()));
                }
            }
        }

        // 2) Collect assets exactly in the current folder
        std::vector<assets::GenericAssetRef> filtered;
        for (auto& ref : assets::AssetCatalog::getInstance().getAssets()) {
            if (auto d = ref.lock()) {
                const auto& folder = d->getMetadata().location.getPath();
                if (folder == "_internal")           continue;
                if (m_selectedType != assets::AssetType::UNKNOWN &&
                    d->getType() != m_selectedType)  continue;

                // **here’s the fix**: just compare the whole normalized folder
                if (folder == m_currentFolder)
                    filtered.push_back(ref);
            }
        }

        // 3) Layout & draw both subfolders and assets
        size_t totalItems = subfolders.size() + filtered.size();
        ImGuiListClipper clipper;
        int rows = int((totalItems + m_layout.size.columnCount - 1) / m_layout.size.columnCount);
        clipper.Begin(rows, m_layout.size.itemStep.y);

        while (clipper.Step()) {
            for (int line = clipper.DisplayStart; line < clipper.DisplayEnd; ++line) {
                int startIdx = line * m_layout.size.columnCount;
                int endIdx   = std::min(startIdx + m_layout.size.columnCount, (int)totalItems);

                for (int i = startIdx; i < endIdx; ++i) {
                    float col = float(i % m_layout.size.columnCount);
                    float row = float(i / m_layout.size.columnCount);
                    ImVec2 itemPos{
                        startPos.x + col * m_layout.size.itemStep.x,
                        startPos.y + row * m_layout.size.itemStep.y
                    };

                    if (i < (int)subfolders.size()) {
                        // draw folder thumbnail
                        drawFolder(
                            subfolders[i].first,
                            subfolders[i].second,
                            itemPos,
                            m_layout.size.itemSize
                        );
                    } else {
                        // draw asset thumbnail
                        int assetIdx = i - (int)subfolders.size();
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

    void AssetManagerWindow::show()
    {
        m_hoveredFolder.clear();
        if (m_folderStructure.empty())
            buildFolderStructure();

        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin(ICON_FA_FOLDER_OPEN " Asset Manager" NEXO_WND_USTRID_ASSET_MANAGER, &m_opened, ImGuiWindowFlags_MenuBar);
        beginRender(NEXO_WND_USTRID_ASSET_MANAGER);

        drawMenuBar();

        // Calculate sizes for splitter
        const float splitterWidth = 5.0f;
        static float leftPanelWidth = 200.0f; // Default width

        // Left panel (folder hierarchy)
        ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
        drawFolderTree();
        ImGui::EndChild();

        // Splitter
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Separator));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_SeparatorHovered));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_SeparatorActive));

        ImGui::Button("##Splitter", ImVec2(splitterWidth, -1));
        ImGui::PopStyleColor(3);

        // Handle splitter drag
        if (ImGui::IsItemActive())
            leftPanelWidth += ImGui::GetIO().MouseDelta.x;

        // Right panel (asset grid)
        ImGui::SameLine();
        ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);

        // Handle file drops
        if (ImGui::BeginDragDropTarget())
        {
            m_showDropIndicator = true;

            // Accept external file drops (this is a placeholder - ImGui doesn't directly support OS file drops)
            // The actual files come through the EventFileDrop event

            ImGui::EndDragDropTarget();
        }
        else
        {
            m_showDropIndicator = false;
        }

        // Draw drop indicator
        if (m_showDropIndicator || !m_pendingDroppedFiles.empty())
        {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();

            // Draw semi-transparent overlay
            drawList->AddRectFilled(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
                                  IM_COL32(100, 100, 255, 50));

            // Draw border
            drawList->AddRect(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + windowSize.y),
                            IM_COL32(100, 100, 255, 200), 0.0f, 0, 3.0f);

            // Draw text
            const char* dropText = "Drop files here to import";
            ImVec2 textSize = ImGui::CalcTextSize(dropText);
            ImVec2 textPos = ImVec2(windowPos.x + (windowSize.x - textSize.x) * 0.5f,
                                  windowPos.y + (windowSize.y - textSize.y) * 0.5f);
            drawList->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 1.5f, textPos,
                            IM_COL32(255, 255, 255, 255), dropText);
        }

        ImGui::Text(ICON_FA_FOLDER " ");
        ImGui::SameLine();

        {
            ImGui::PushID("breadcrumb_root");
            if (ImGui::Button("Assets"))
                m_currentFolder.clear();

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG"))
                {
                    const AssetDragDropPayload* data = (const AssetDragDropPayload*)payload->Data;
                    assets::AssetCatalog::getInstance().moveAsset(data->id, "");
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::PopID();
        }

        // Intermediate breadcrumbs
        std::string path = m_currentFolder;
        size_t pos = 0;
        std::string segment;
        std::string fullPath;
        while ((pos = path.find('/')) != std::string::npos) {
            segment = path.substr(0, pos);
            fullPath += (fullPath.empty() ? "" : "/") + segment;
            ImGui::SameLine(); ImGui::Text(" > "); ImGui::SameLine();

            ImGui::PushID(("breadcrumb_" + fullPath).c_str());
            if (ImGui::Button(segment.c_str()))
                m_currentFolder = fullPath;

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG"))
                {
                    const AssetDragDropPayload* data = (const AssetDragDropPayload*)payload->Data;
                    assets::AssetCatalog::getInstance().moveAsset(data->id, fullPath);
                }
                ImGui::EndDragDropTarget();
            }
            ImGui::PopID();

            path.erase(0, pos + 1);
        }

        if (!path.empty()) {
            ImGui::SameLine(); ImGui::Text(" > "); ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "%s", path.c_str());
        }

        ImGui::Separator();

        calculateLayout(ImGui::GetContentRegionAvail().x);
        drawAssetsGrid();
        ImGui::EndChild();

        ImGui::End();
    }
}
