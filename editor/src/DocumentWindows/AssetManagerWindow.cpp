//// AssetManagerWindow.cpp ///////////////////////////////////////////////////
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
#include <Path.hpp>
#include <assets/Asset.hpp>
#include <assets/AssetCatalog.hpp>
#include <assets/AssetRef.hpp>
#include <assets/Assets/Model/Model.hpp>
#include <assets/Assets/Texture/Texture.hpp>

namespace nexo::editor {

    void AssetManagerWindow::setup() {
        auto& catalog = assets::AssetCatalog::getInstance();

        const auto asset = new assets::Model();

        catalog.registerAsset(assets::AssetLocation("my_package::My_Model@foo/bar/"), asset);



        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/9mn/scene.gltf");
            assets::ImporterFileInput fileInput{path};
            auto assetRef9mn = importer.importAssetAuto(assets::AssetLocation("my_package::9mn@foo/bar/"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/logo_nexo.png");
            assets::ImporterFileInput fileInput{path};
            auto textureRef = importer.importAsset<assets::Texture>(assets::AssetLocation("nexo_logo@foo/bar/"), fileInput);
        }
    }

    void AssetManagerWindow::shutdown() {
    }

    void AssetManagerWindow::show() {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin("Asset Manager", nullptr, ImGuiWindowFlags_MenuBar);

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
        m_layout.size.columnCount = std::max(static_cast<int>(availWidth / (m_layout.size.iconSize + m_layout.size.iconSpacing)), 1);
        m_layout.size.itemSize = ImVec2(m_layout.size.iconSize + ImGui::GetFontSize() * 1.5f, m_layout.size.iconSize + ImGui::GetFontSize() * 1.7f);
        m_layout.size.itemStep = ImVec2(m_layout.size.itemSize.x + m_layout.size.iconSpacing, m_layout.size.itemSize.y + m_layout.size.iconSpacing);

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

    void AssetManagerWindow::drawMenuBar() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Options")) {
                ImGui::SliderFloat("Icon Size", &m_layout.size.iconSize, 32.0f, 128.0f, "%.0f");
                ImGui::SliderInt("Icon Spacing", &m_layout.size.iconSpacing, 0, 32);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void AssetManagerWindow::drawAssetsGrid() {
        ImVec2 startPos = ImGui::GetCursorScreenPos();

        ImGuiListClipper clipper;
        const auto assets = assets::AssetCatalog::getInstance().getAssets();
        clipper.Begin(assets.size(), m_layout.size.itemStep.y);
        while (clipper.Step()) {
            for (int lineIdx = clipper.DisplayStart; lineIdx < clipper.DisplayEnd; ++lineIdx) {
                int startIdx = lineIdx * m_layout.size.columnCount;
                int endIdx = std::min(startIdx + m_layout.size.columnCount, static_cast<int>(assets.size()));

                for (int i = startIdx; i < endIdx; ++i) {
                    ImVec2 itemPos = ImVec2(startPos.x + (i % m_layout.size.columnCount) * m_layout.size.itemStep.x,
                        startPos.y + (i / m_layout.size.columnCount) * m_layout.size.itemStep.y);
                    drawAsset(assets[i], i, itemPos, m_layout.size.itemSize);
                }
            }
        }
        clipper.End();
    }

    void AssetManagerWindow::drawAsset(const assets::GenericAssetRef& asset, int index, const ImVec2& itemPos, const ImVec2& itemSize) {
        auto assetData = asset.lock();
        if (!assetData)
            return;
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 itemEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y);

        ImGui::PushID(index);


        // Highlight selection
        bool isSelected = std::find(m_selectedAssets.begin(), m_selectedAssets.end(), index) != m_selectedAssets.end();
        ImU32 bgColor = isSelected ? m_layout.color.thumbnailBgSelected : m_layout.color.thumbnailBg;
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
        ImVec2 thumbnailEnd = ImVec2(itemPos.x + itemSize.x, itemPos.y + itemSize.y * m_layout.size.thumbnailHeightRatio);
        drawList->AddRectFilled(itemPos, thumbnailEnd, m_layout.color.thumbnailBg);

        // Draw type overlay
        ImVec2 overlayPos = ImVec2(thumbnailEnd.x - m_layout.size.overlayPadding, itemPos.y + m_layout.size.overlayPadding);
        ImU32 overlayColor = getAssetTypeOverlayColor(assetData->getType());
        drawList->AddRectFilled(overlayPos, ImVec2(overlayPos.x + m_layout.size.overlaySize, overlayPos.y + m_layout.size.overlaySize), overlayColor);

        // Draw title
        const char *assetName = assetData->getMetadata().location.getName().c_str();
        ImVec2 textPos = ImVec2(itemPos.x + (itemSize.x - ImGui::CalcTextSize(assetName).x) * 0.5f,
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

    void AssetManagerWindow::handleSelection(int index, bool isSelected)
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

    ImU32 AssetManagerWindow::getAssetTypeOverlayColor(assets::AssetType type) const {
        switch (type) {
            case assets::AssetType::TEXTURE: return IM_COL32(200, 70, 70, 255);
            case assets::AssetType::MODEL: return IM_COL32(70, 170, 70, 255);
            default: return IM_COL32(0, 0, 0, 0);
        }
    }

} // namespace nexo::editor
