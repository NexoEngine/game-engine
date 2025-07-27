//// FolderTree.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/05/2025
//  Description: Source file for the folder tree rendering
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include "IconsFontAwesome.h"
#include "assets/Asset.hpp"
#include "assets/AssetCatalog.hpp"

#include <filesystem>
#include <imgui.h>
#include <set>

namespace nexo::editor {

    void AssetManagerWindow::folderTreeContextMenu()
    {
        if (ImGui::MenuItem("New Folder"))
            m_popupManager.openPopup("Create new folder");

        PopupManager::closePopup();
    }

    void AssetManagerWindow::drawFolderTreeItem(const std::string& name, const std::string& path)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (path == m_currentFolder)
            flags |= ImGuiTreeNodeFlags_Selected;
        if (!m_folderChildren.contains(path) || m_folderChildren.at(path).empty())
            flags |= ImGuiTreeNodeFlags_Leaf;

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(230, 180, 80, 255));
        ImGui::Text(ICON_FA_FOLDER);
        ImGui::PopStyleColor();
        ImGui::SameLine();

        bool opened = ImGui::TreeNodeEx(name.c_str(), flags);

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen())
            m_currentFolder = path;
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            m_folderCreationState.reset();
            m_folderCreationState.parentPath = path;
            m_popupManager.openPopup("Folder Tree Context Menu");
        }

        if (!opened)
            return;

        if (const auto it = m_folderChildren.find(path); it != m_folderChildren.end()) {
            for (const auto& childPath : it->second) {
                const std::string childName = std::filesystem::path(childPath).filename().string();
                drawFolderTreeItem(childName, childPath);
            }
        }
        ImGui::TreePop();
    }

    void AssetManagerWindow::drawFolderTree()
    {
        ImGui::PushItemWidth(-1);
        ImGui::InputTextWithHint("##search", "Search...", m_searchBuffer, sizeof(m_searchBuffer));
        ImGui::PopItemWidth();
        ImGui::Separator();

        // favorites section
        {
            if (ImGui::TreeNodeEx(ICON_FA_STAR " Favorites", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick)) {
                struct FavoriteItem {
                    std::string label;
                    assets::AssetType type;
                };

                static const FavoriteItem favorites[] = {
                    {ICON_FA_ADJUST " Materials", assets::AssetType::MATERIAL},
                    {ICON_FA_CUBE " Models", assets::AssetType::MODEL},
                    {ICON_FA_SQUARE " Textures", assets::AssetType::TEXTURE}
                };

                for (const auto& fav : favorites) {
                    const bool isSelected = (fav.type == m_selectedType);

                    ImGuiTreeNodeFlags itemFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                    if (isSelected)
                        itemFlags |= ImGuiTreeNodeFlags_Selected;

                    const std::string labelName = fav.label + (isSelected ? "   " ICON_FA_CHECK : "");
                    ImGui::TreeNodeEx(labelName.c_str(), itemFlags);

                    if (ImGui::IsItemClicked()) {
                        if (isSelected)
                            m_selectedType = assets::AssetType::UNKNOWN;
                        else
                            m_selectedType = fav.type;
                    }
                }
                ImGui::TreePop();
            }
        }

        // folder structure
        {
            ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_OpenOnDoubleClick;

            if (m_currentFolder.empty()) {
                headerFlags |= ImGuiTreeNodeFlags_Selected;
            }

            bool assetsOpen = ImGui::TreeNodeEx(ICON_FA_FOLDER " Assets", headerFlags);

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                m_folderCreationState.reset();
                m_popupManager.openPopup("Folder Tree Context Menu");
            }

            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                m_currentFolder = "";

            if (assetsOpen) {
                for (const auto& [path, name] : m_folderStructure) {
                    if (!path.empty() && path.find('/') == std::string::npos) {
                        drawFolderTreeItem(name, path);
                    }
                }
                ImGui::TreePop();
            }
        }
    }
}
