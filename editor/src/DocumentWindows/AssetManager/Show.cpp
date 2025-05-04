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
#include "assets/AssetCatalog.hpp"
#include "IconsFontAwesome.h"

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

        // Draw thumbnail
        const auto thumbnailEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y * m_layout.size.thumbnailHeightRatio);
        drawList->AddRectFilled(itemPos, thumbnailEnd, m_layout.color.thumbnailBg);

        // Draw type overlay
        const auto overlayPos = ImVec2(thumbnailEnd.x - m_layout.size.overlayPadding, itemPos.y + m_layout.size.overlayPadding);
        const ImU32 overlayColor = getAssetTypeOverlayColor(assetData->getType());
        drawList->AddRectFilled(overlayPos, ImVec2(overlayPos.x + m_layout.size.overlaySize, overlayPos.y + m_layout.size.overlaySize), overlayColor);

        // Draw title
        const char *assetName = assetData->getMetadata().location.getName().c_str();
        const auto textPos = ImVec2(itemPos.x + (itemSize.x - ImGui::CalcTextSize(assetName).x) * 0.5f,
            thumbnailEnd.y + m_layout.size.titlePadding);
        // Background rectangle for text
        drawList->AddRectFilled(ImVec2(itemPos.x, thumbnailEnd.y), ImVec2(itemEnd.x, itemEnd.y), m_layout.color.titleBg);
        drawList->AddText(textPos, m_layout.color.titleText, assetName);

        // Position the cursor for the invisible button
        ImGui::SetCursorScreenPos(itemPos);

        // Handle input
        if (ImGui::InvisibleButton("##item", itemSize)) {
            handleSelection(index, isSelected);
        }

        // On Hover show asset location
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip)) {
            ImGui::SetTooltip("%s", assetData->getMetadata().location.getFullLocation().c_str());
        }

        ImGui::PopID();
    }

    void AssetManagerWindow::drawAssetsGrid()
    {
        ImVec2 startPos = ImGui::GetCursorScreenPos();

        ImGuiListClipper clipper;
        const auto assets = assets::AssetCatalog::getInstance().getAssets();
        clipper.Begin(static_cast<int>(assets.size()), m_layout.size.itemStep.y);
        while (clipper.Step()) {
            for (int lineIdx = clipper.DisplayStart; lineIdx < clipper.DisplayEnd; ++lineIdx) {
                int startIdx = lineIdx * m_layout.size.columnCount;
                int endIdx = std::min(startIdx + m_layout.size.columnCount, static_cast<int>(assets.size()));

                int columns = m_layout.size.columnCount;
                float stepX  = m_layout.size.itemStep.x;
                float stepY  = m_layout.size.itemStep.y;

                for (int i = startIdx; i < endIdx; ++i) {
                    float idx     = static_cast<float>(i);
                    float col     = std::fmod(idx, static_cast<float>(columns));
                    float row     = std::floor(idx / static_cast<float>(columns));
                    ImVec2 itemPos{
                        startPos.x + col * stepX,
                        startPos.y + row * stepY
                    };
                    drawAsset(assets[i], i, itemPos, m_layout.size.itemSize);
                }
            }
        }
        clipper.End();
    }

    void AssetManagerWindow::show()
    {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin(ICON_FA_FOLDER_OPEN " Asset Manager" NEXO_WND_USTRID_ASSET_MANAGER, &m_opened, ImGuiWindowFlags_MenuBar);
        beginRender(NEXO_WND_USTRID_ASSET_MANAGER);

        drawMenuBar();

        float availWidth = ImGui::GetContentRegionAvail().x;
        calculateLayout(availWidth);

        drawAssetsGrid();

        ImGui::End();
    }
}
