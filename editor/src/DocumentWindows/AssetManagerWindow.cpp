//// AssetManagerWindow.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        18/11/2024
//  Description: Source file for the AssetManagerWindow class
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include <algorithm>

// AssetManagerWindow.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        18/11/2024
//  Description: Source file for the AssetManagerWindow class
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include <algorithm>

namespace nexo::editor {

    void AssetManagerWindow::setup() {
        // Initialize assets
        for (int i = 0; i < 100; ++i) {
            assets.push_back({ "Asset " + std::to_string(i), i % 3 }); // Alternate types
        }
    }

    void AssetManagerWindow::shutdown() {
        assets.clear();
    }

    void AssetManagerWindow::show() {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("Asset Manager", nullptr, ImGuiWindowFlags_MenuBar)) {
            ImGui::End();
            return;
        }

        drawMenuBar();

        float availWidth = ImGui::GetContentRegionAvail().x;
        calculateLayout(availWidth);

        drawAssetsGrid();

        ImGui::End();
    }

    void AssetManagerWindow::update() {
        // Update logic if necessary
    }

    void AssetManagerWindow::calculateLayout(float availWidth) {
        // Sizes
        layout.size.columnCount = std::max(static_cast<int>(availWidth / (layout.size.iconSize + layout.size.iconSpacing)), 1);
        layout.size.itemSize = ImVec2(layout.size.iconSize + ImGui::GetFontSize() * 1.5f, layout.size.iconSize + ImGui::GetFontSize() * 1.7f);
        layout.size.itemStep = ImVec2(layout.size.itemSize.x + layout.size.iconSpacing, layout.size.itemSize.y + layout.size.iconSpacing);

        // Colors
		layout.color.thumbnailBg = ImGui::GetColorU32(ImGuiCol_Button);
		layout.color.thumbnailBgHovered = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
		layout.color.thumbnailBgSelected = ImGui::GetColorU32(ImGuiCol_Header);
		layout.color.thumbnailBgSelectedHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);

		layout.color.titleBg = ImGui::GetColorU32(ImGuiCol_Header);
		layout.color.titleBgHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);
		layout.color.titleBgSelected = ImGui::GetColorU32(ImGuiCol_Header);
		layout.color.titleBgSelectedHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);

		layout.color.titleText = ImGui::GetColorU32(ImGuiCol_Text);
    }

    void AssetManagerWindow::drawMenuBar() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                ImGui::SliderFloat("Icon Size", &layout.size.iconSize, 32.0f, 128.0f, "%.0f");
                ImGui::SliderInt("Icon Spacing", &layout.size.iconSpacing, 0, 32);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void AssetManagerWindow::drawAssetsGrid() {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 startPos = ImGui::GetCursorScreenPos();

        ImGuiListClipper clipper;
        clipper.Begin(assets.size(), layout.size.itemStep.y);
        while (clipper.Step()) {
            for (int lineIdx = clipper.DisplayStart; lineIdx < clipper.DisplayEnd; ++lineIdx) {
                int startIdx = lineIdx * layout.size.columnCount;
                int endIdx = std::min(startIdx + layout.size.columnCount, static_cast<int>(assets.size()));

                for (int i = startIdx; i < endIdx; ++i) {
                    ImVec2 itemPos = ImVec2(startPos.x + (i % layout.size.columnCount) * layout.size.itemStep.x,
                        startPos.y + (i / layout.size.columnCount) * layout.size.itemStep.y);
                    drawAsset(assets[i], i, itemPos, layout.size.itemSize);
                }
            }
        }
        clipper.End();
    }

    void AssetManagerWindow::drawAsset(const Asset& asset, int index, const ImVec2& itemPos, const ImVec2& itemSize) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 itemEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y);

        ImGui::PushID(index);

        // Highlight selection
        bool isSelected = std::find(selectedAssets.begin(), selectedAssets.end(), index) != selectedAssets.end();
        ImU32 bgColor = isSelected ? layout.color.thumbnailBgSelected : layout.color.thumbnailBg;
        drawList->AddRectFilled(itemPos, itemEnd, bgColor, layout.size.cornerRadius);

        // Draw thumbnail
        ImVec2 thumbnailEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y * layout.size.thumbnailHeightRatio);
        drawList->AddRectFilled(itemPos, thumbnailEnd, layout.color.thumbnailBg);

        // Draw type overlay
        ImVec2 overlayPos = ImVec2(thumbnailEnd.x - layout.size.overlayPadding, itemPos.y + layout.size.overlayPadding);
        ImU32 overlayColor = getAssetTypeOverlayColor(asset.type);
        drawList->AddRectFilled(overlayPos, ImVec2(overlayPos.x + layout.size.overlaySize, overlayPos.y + layout.size.overlaySize), overlayColor);

        // Draw title
        ImVec2 textPos = ImVec2(itemPos.x + (itemSize.x - ImGui::CalcTextSize(asset.name.c_str()).x) * 0.5f,
            thumbnailEnd.y + layout.size.titlePadding);
        // Background rectangle for text
        drawList->AddRectFilled(ImVec2(itemPos.x, thumbnailEnd.y), ImVec2(itemEnd.x, itemEnd.y), layout.color.titleBg);
        drawList->AddText(textPos, layout.color.titleText, asset.name.c_str());

        // Handle input
        if (ImGui::InvisibleButton("##item", itemSize)) {
            handleSelection(index, isSelected);
        }

        ImGui::PopID();
    }

    void AssetManagerWindow::handleSelection(int index, bool isSelected) {
        LOG(NEXO_INFO, "Asset {} {}", index, isSelected ? "deselected" : "selected");
        if (ImGui::GetIO().KeyCtrl) {
            if (isSelected)
                selectedAssets.erase(std::remove(selectedAssets.begin(), selectedAssets.end(), index), selectedAssets.end());
            else
                selectedAssets.push_back(index);
        }
        else {
            selectedAssets.clear();
            selectedAssets.push_back(index);
        }
    }

    ImU32 AssetManagerWindow::getAssetTypeOverlayColor(int type) const {
        switch (type) {
        case 1: return IM_COL32(200, 70, 70, 255);
        case 2: return IM_COL32(70, 170, 70, 255);
        default: return IM_COL32(0, 0, 0, 0);
        }
    }

} // namespace nexo::editor
