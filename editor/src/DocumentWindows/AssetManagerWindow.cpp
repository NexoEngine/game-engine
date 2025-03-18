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
#include <Path.hpp>
#include <assets/Asset.hpp>
#include <assets/AssetCatalog.hpp>
#include <assets/AssetRef.hpp>
#include <assets/Assets/Model/Model.hpp>
#include <assets/Assets/Texture/Texture.hpp>

namespace nexo::editor {

    /**
     * @brief Initializes the asset manager with default and imported assets.
     *
     * Populates the asset list with 100 default assets, assigning alternating asset types.
     * Retrieves the singleton asset catalog and registers a new model asset under a predefined location.
     * Additionally, imports a model and a texture from file paths relative to the executable,
     * setting up asset references for use within the manager.
     */
    void AssetManagerWindow::setup() {
        // Initialize assets
        for (int i = 0; i < 100; ++i) {
            m_assets.push_back({ "Asset " + std::to_string(i), i % 3 }); // Alternate types
        }

        auto& catalog = assets::AssetCatalog::getInstance();

        const auto asset = new assets::Model();

        catalog.registerAsset(assets::AssetLocation("my_package::My_Model@foo/bar/"), asset);



        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../assets/models/9mn/scene.gltf");
            assets::ImporterFileInput fileInput{path};
            auto assetRef9mn = importer.importAssetAuto(assets::AssetLocation("my_package::9mn@foo/bar/"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../assets/textures/logo_nexo.png");
            assets::ImporterFileInput fileInput{path};
            auto textureRef = importer.importAsset<assets::Texture>(assets::AssetLocation("nexo_logo@foo/bar/"), fileInput);
        }


    }

    /**
     * @brief Clears all managed asset references.
     *
     * This method clears the internal list of assets, effectively releasing any references held
     * by the AssetManagerWindow. It is typically invoked during the shutdown process to ensure that
     * all assets are properly released.
     */
    void AssetManagerWindow::shutdown() {
        m_assets.clear();
    }

    /**
     * @brief Displays the asset manager window.
     *
     * Sets the initial window size, opens the "Asset Manager" window, and renders its contents including
     * the menu bar and the grid of assets. If the window cannot be opened, it ends the session early.
     */
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

    /**
     * @brief Placeholder for update operations in the asset manager.
     *
     * This method is designed for future expansion to handle updating the asset manager's state.
     */
    void AssetManagerWindow::update() {
        // Update logic if necessary
    }

    /**
     * @brief Calculates layout parameters for asset display based on available width.
     *
     * This function computes the number of columns that can fit in the current available width and determines
     * the size and spacing for each asset item. It also updates various color settings for UI elements such as thumbnails
     * and titles using ImGui's color scheme.
     *
     * @param availWidth The total available width for laying out asset items.
     */
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

    /**
     * @brief Renders the menu bar with layout customization options.
     *
     * This function creates an "Options" menu within the asset manager's menu bar,
     * providing sliders to adjust the icon size (ranging from 32 to 128) and icon spacing
     * (ranging from 0 to 32) used in the asset grid layout.
     */
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

    /**
     * @brief Renders the asset grid within the asset manager window.
     *
     * Retrieves the current list of assets from the asset catalog and uses an ImGuiListClipper
     * to efficiently process and display only the visible asset rows according to the layout settings.
     * For each asset in the visible grid segment, it calculates the screen position based on the current
     * cursor position and layout parameters, and delegates rendering to drawAsset().
     */
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

    /**
     * @brief Renders an asset within the asset grid.
     *
     * Draws the asset’s thumbnail, type overlay, title background, and selection indicators in the specified display area.
     * The function sets up an invisible button to handle user interaction for selection and shows a tooltip with the asset's full location on hover.
     * If the asset reference is expired, no rendering occurs.
     *
     * @param asset Reference to the asset to be rendered.
     * @param index Index of the asset in the grid, used for unique identification.
     * @param itemPos Screen coordinates for the top-left corner of the asset's display area.
     * @param itemSize Dimensions of the asset's display area.
     */
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
        const char *assetName = assetData->getMetadata().location.getAssetName().c_str();
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

    /**
     * @brief Updates the selection state of an asset based on user interaction.
     *
     * This function modifies the set of selected assets using the asset's index and the
     * current state of modifier keys:
     * - With Ctrl held, it toggles the selection state of the asset.
     * - With Shift held, it selects all assets in the range from the last selected asset to
     *   the current asset.
     * - Without any modifier keys, it clears previous selections and selects only the current asset.
     *
     * @param index The index of the asset to update.
     * @param isSelected Indicates whether the asset is already selected.
     */
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

    /**
     * @brief Returns the overlay color for a given asset type.
     *
     * This method maps a specific asset type to a predefined 32-bit color value used for overlay rendering.
     * For recognized asset types such as TEXTURE and MODEL, it returns their corresponding colors;
     * for other types, it returns a fully transparent color.
     *
     * @param type The asset type for which to determine the overlay color.
     * @return ImU32 The packed 32-bit color value corresponding to the asset type.
     */
    ImU32 AssetManagerWindow::getAssetTypeOverlayColor(assets::AssetType type) const {
        switch (type) {
            case assets::AssetType::TEXTURE: return IM_COL32(200, 70, 70, 255);
            case assets::AssetType::MODEL: return IM_COL32(70, 170, 70, 255);
            default: return IM_COL32(0, 0, 0, 0);
        }
        /*case 1: return IM_COL32(200, 70, 70, 255);
        case 2: return IM_COL32(70, 170, 70, 255);
        default: return IM_COL32(0, 0, 0, 0);
        }*/
    }

} // namespace nexo::editor
