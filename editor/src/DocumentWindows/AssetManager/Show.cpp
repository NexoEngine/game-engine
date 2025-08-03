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

#include <imgui.h>
#include "AssetManagerWindow.hpp"
#include "IconsFontAwesome.h"
#include "ImNexo/Elements.hpp"
#include "Path.hpp"
#include "assets/AssetCatalog.hpp"
#include "context/ThumbnailCache.hpp"
#include "context/ActionManager.hpp"
#include "context/actions/AssetActions.hpp"
#include "ImNexo/Elements.hpp"
#include <cstring>
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

    void AssetManagerWindow::drawPanelSplitter()
    {
        constexpr float splitterWidth = 5.0f;

        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Separator));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_SeparatorHovered));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_SeparatorActive));

        ImGui::Button("##Splitter", ImVec2(splitterWidth, -1));
        ImGui::PopStyleColor(3);

        if (ImGui::IsItemActive()) m_layout.leftPanelWidth += ImGui::GetIO().MouseDelta.x;
    }

    void AssetManagerWindow::drawBreadcrumbs()
    {
        ImGui::PushID("breadcrumb_root");
        if (ImGui::Button("Assets")) m_currentFolder.clear();

        handleAssetDrop("");
        ImGui::PopID();

        std::string path                = m_currentFolder;
        std::vector<std::string> crumbs = splitPath(m_currentFolder);
        std::string fullPath;
        for (const auto &crumb : crumbs) {
            fullPath += (fullPath.empty() ? "" : "/") + crumb;
            ImGui::SameLine();
            ImGui::Text(" > ");
            ImGui::SameLine();
            ImGui::PushID(("breadcrumb_" + crumb).c_str());
            if (crumb == *std::prev(crumbs.end()))
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "%s", crumb.c_str());
            else if (ImNexo::Button(crumb))
                m_currentFolder = fullPath;

            handleAssetDrop(fullPath);
            ImGui::PopID();
        }
    }

    void AssetManagerWindow::show()
    {
        ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin(ICON_FA_FOLDER_OPEN " Asset Manager" NEXO_WND_USTRID_ASSET_MANAGER, &m_opened,
                     ImGuiWindowFlags_MenuBar);

        beginRender(NEXO_WND_USTRID_ASSET_MANAGER);

        drawMenuBar();

        // Left panel
        {
            ImGui::BeginChild("LeftPanel", ImVec2(m_layout.leftPanelWidth, 0), true);
            handleRightClickOnAssetManager();
            drawFolderTree();
            ImGui::EndChild();
        }

        drawPanelSplitter();
        ImGui::SameLine();

        // Right panel
        {
            ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
            ImGui::Text(ICON_FA_FOLDER " ");
            ImGui::SameLine();
            handleRightClickOnAssetManager();
            drawBreadcrumbs();
            ImGui::Separator();
            drawAssetsGrid();
            ImGui::EndChild();
        }

        // Popups & right-click menus
        {
            // Right-click menus
            if (m_popupManager.showPopup("Right click on Asset Manager")) rightClickOnAssetManagerMenu();
            if (m_popupManager.showPopup("Right click on Folder")) rightClickOnFolderMenu();
            if (m_popupManager.showPopup("Right click on Asset")) rightClickOnAssetMenu();

            // Folder popups
            if (m_popupManager.showPopupModal("Create Folder Popup")) createFolderPopup();
            if (m_popupManager.showPopupModal("Rename Folder Popup")) renameFolderPopup();
            if (m_popupManager.showPopupModal("Delete Folder Popup")) deleteFolderPopup();
            if (m_popupManager.showPopupModal("Delete Not Empty Folder Popup")) deleteNotEmptyFolderPopup();
            if (m_popupManager.showPopupModal("Details Folder Popup")) folderDetailsPopup();

            // Asset popups
            if (m_popupManager.showPopupModal("Rename Asset Popup")) renameAssetPopup();
            if (m_popupManager.showPopupModal("Delete Asset Popup")) deleteAssetPopup();
            if (m_popupManager.showPopupModal("Delete Not Empty Asset Popup")) deleteUsedAssetPopup();
            if (m_popupManager.showPopupModal("Details Asset Popup")) assetDetailsPopup();
        }

        ImGui::End();
    }
} // namespace nexo::editor
