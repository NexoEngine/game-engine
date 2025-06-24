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
        LOG(NEXO_INFO, "Asset {} {}", index, isSelected ? "deselected" : "selected");
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

        // Set cursor position for hover/click detection
        ImGui::SetCursorScreenPos(itemPos);

        // Create button for click handling
        bool clicked = ImGui::InvisibleButton("##item", itemSize);
        bool isHovered = ImGui::IsItemHovered();

        // Highlight selection
        const bool isSelected = std::ranges::find(m_selectedAssets, index) != m_selectedAssets.end();
        const ImU32 bgColor = isSelected ? m_layout.color.thumbnailBgSelected : m_layout.color.thumbnailBg;
        drawList->AddRectFilled(itemPos, itemEnd, bgColor, m_layout.size.cornerRadius);

        // Add selection border
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

        // Draw thumbnail content based on asset type
        if (assetData->getType() == assets::AssetType::TEXTURE) {
            // Get the texture ID for rendering
            auto textureAsset = asset.as<assets::Texture>();
            auto textureData = textureAsset.lock();
            ImTextureID textureId = textureData->getData().get()->texture->getId();
            drawTextureThumbnail(drawList, textureId, itemPos, thumbnailEnd);
        } else {
            // For non-texture assets, use a standard background
            drawList->AddRectFilled(itemPos, thumbnailEnd, m_layout.color.thumbnailBg);
        }

        // Draw type overlay
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
        if (clicked) {
            handleSelection(index, isSelected);
        }

        // On Hover show asset location
        if (isHovered) {
            ImGui::SetTooltip("%s", assetData->getMetadata().location.getFullLocation().c_str());
        }

        ImGui::PopID();
    }

    ImTextureID AssetManagerWindow::getFolderIconTexture()
    {
        // Check if folder texture is already loaded
        if (m_folderIconTexture == 0) {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/icon_folder.png");
            assets::ImporterFileInput fileInput{path};
            auto textureRef = importer.importAsset<assets::Texture>(assets::AssetLocation("folder_icon@_internal"), fileInput);
            if (textureRef) {
                auto textureData = textureRef.lock();
                // Get the ImGui texture ID from your engine's texture
                m_folderIconTexture = textureData->getData()->texture->getId();
            } else {
                // Fallback or error handling
                LOG(NEXO_WARN, "Failed to load folder icon texture");
            }
        }

        return m_folderIconTexture;
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

        // Set cursor position for hover/click detection
        ImGui::SetCursorScreenPos(itemPos);

        // Create invisible button first for proper hover detection
        bool clicked = ImGui::InvisibleButton("##folder", itemSize);
        bool isHovered = ImGui::IsItemHovered();

        // Background - use hover color when hovered
        ImU32 bgColor = isHovered ? m_layout.color.thumbnailBgHovered : IM_COL32(0, 0, 0, 0);
        drawList->AddRectFilled(itemPos, itemEnd, bgColor, m_layout.size.cornerRadius);

        // Draw thumbnail area
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

        // Handle navigation when clicked
        if (clicked) {
            m_currentFolder = folderPath; // Navigate into this folder
        }

        ImGui::PopID();
    }

    void AssetManagerWindow::drawAssetsGrid()
    {
        ImVec2 startPos = ImGui::GetCursorScreenPos();
        std::vector<std::pair<std::string, std::string>> subfolders;

        // First, collect all immediate subfolders of the current folder
        for (const auto& [path, name] : m_folderStructure) {
            // Skip the current folder itself
            if (path == m_currentFolder) {
                continue;
            }

            // Check if this is a direct subfolder of the current folder
            if (path.find(m_currentFolder) == 0) {
                // For root folder (empty current folder)
                if (m_currentFolder.empty()) {
                    if (path.find('/') == std::string::npos) {
                        subfolders.emplace_back(path, name);
                    }
                }
                // For non-root folders
                else {
                    // Check if it's a direct child (only one more / after current folder)
                    std::string pathAfterCurrent = path.substr(m_currentFolder.length());
                    if (pathAfterCurrent[0] == '/') {
                        pathAfterCurrent = pathAfterCurrent.substr(1);
                    }

                    if (pathAfterCurrent.find('/') == std::string::npos && !pathAfterCurrent.empty()) {
                        subfolders.emplace_back(path, pathAfterCurrent);
                    }
                }
            }
        }

        // Get assets to display
        const std::vector<assets::GenericAssetRef> assets = assets::AssetCatalog::getInstance().getAssets();

        // Filter assets by currently selected folder
        std::vector<assets::GenericAssetRef> filteredAssets;
        for (const auto& asset : assets) {
            if (auto assetData = asset.lock()) {
                if (assetData->getMetadata().location.getPath() == "_internal")
                    continue;

                if (m_selectedType != assets::AssetType::UNKNOWN && assetData->getType() != m_selectedType)
                    continue;

                // Check if asset is in current folder exactly (not in subfolders)
                std::string assetPath = assetData->getMetadata().location.getPath();
                std::string assetDir = assetPath.substr(0, assetPath.find_last_of('/'));

                // If there's no slash, asset is in root
                if (assetPath.find('/') == std::string::npos) {
                    assetDir = "";
                }

                if (assetDir == m_currentFolder) {
                    filteredAssets.push_back(asset);
                }
            }
        }

        // Calculate total items (folders + assets)
        size_t totalItems = subfolders.size() + filteredAssets.size();

        // Create a virtual list for clipping
        ImGuiListClipper clipper;
        int rowCount = (totalItems + m_layout.size.columnCount - 1) / m_layout.size.columnCount;
        clipper.Begin(rowCount, m_layout.size.itemStep.y);

        while (clipper.Step()) {
            for (int lineIdx = clipper.DisplayStart; lineIdx < clipper.DisplayEnd; ++lineIdx) {
                int startIdx = lineIdx * m_layout.size.columnCount;
                int endIdx = std::min(startIdx + m_layout.size.columnCount, static_cast<int>(totalItems));

                for (int i = startIdx; i < endIdx; ++i) {
                    float col = static_cast<float>(i % m_layout.size.columnCount);
                    float row = static_cast<float>(i / m_layout.size.columnCount);
                    ImVec2 itemPos{
                        startPos.x + col * m_layout.size.itemStep.x,
                        startPos.y + row * m_layout.size.itemStep.y
                    };

                    // Draw folder if index is in subfolder range
                    if (i < static_cast<int>(subfolders.size())) {
                        drawFolder(
                            subfolders[i].first,
                            subfolders[i].second,
                            itemPos,
                            m_layout.size.itemSize
                        );
                    }
                    // Otherwise draw asset
                    else {
                        int assetIdx = i - static_cast<int>(subfolders.size());
                        if (assetIdx < static_cast<int>(filteredAssets.size())) {
                            drawAsset(
                                filteredAssets[assetIdx],
                                assetIdx,
                                itemPos,
                                m_layout.size.itemSize
                            );
                        }
                    }
                }
            }
        }
        clipper.End();
    }

    void AssetManagerWindow::show()
    {
        // Ensure folder structure is built
        if (m_folderStructure.empty()) {
            buildFolderStructure();
        }

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
        if (ImGui::IsItemActive()) {
            leftPanelWidth += ImGui::GetIO().MouseDelta.x;

            // Clamp to reasonable values
            //leftPanelWidth = ImClamp(leftPanelWidth, 100.0f, ImGui::GetWindowContentRegionWidth() - 200.0f);
        }

        // Right panel (asset grid)
        ImGui::SameLine();
        ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);

        // Show path breadcrumb
        if (m_currentFolder.empty()) {
            ImGui::Text("Assets");
        } else {
            // Display clickable breadcrumbs
            ImGui::Text(ICON_FA_FOLDER " ");
            ImGui::SameLine();

            // Start with root level "Assets"
            if (ImGui::Button("Assets")) {
                m_currentFolder = "";
            }

            // Split the current path into components
            std::string path = m_currentFolder;
            size_t pos = 0;
            std::string segment;
            std::string fullPath = "";

            while ((pos = path.find('/')) != std::string::npos) {
                segment = path.substr(0, pos);
                if (!segment.empty()) {
                    fullPath += (fullPath.empty() ? "" : "/") + segment;
                    ImGui::SameLine();
                    ImGui::Text(" > ");
                    ImGui::SameLine();
                    if (ImGui::Button(segment.c_str())) {
                        m_currentFolder = fullPath;
                    }
                }
                path.erase(0, pos + 1);
            }

            // Last segment
            if (!path.empty()) {
                ImGui::SameLine();
                ImGui::Text(" > ");
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "%s", path.c_str());
            }
        }

        ImGui::Separator();

        // Calculate layout for asset grid
        calculateLayout(ImGui::GetContentRegionAvail().x);
        drawAssetsGrid();
        ImGui::EndChild();

        ImGui::End();
    }
}
