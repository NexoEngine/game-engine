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

namespace nexo::editor {

    static void drawSearchBar(std::string& searchBuffer)
    {
        constexpr size_t MAX_SEARCH_LENGTH = 256;
        searchBuffer.resize(MAX_SEARCH_LENGTH);
        ImGui::PushItemWidth(-1);
        ImGui::InputTextWithHint("##search", "Search...", searchBuffer.data(), searchBuffer.capacity());
        searchBuffer.resize(strlen(searchBuffer.c_str()));
        ImGui::PopItemWidth();
        ImGui::Separator();
    }

    struct FavoriteItem {
        std::string_view icon;
        std::string_view name;
        assets::AssetType type;

        [[nodiscard]] std::string getLabel(bool selected) const
        {
            return std::format("{} {}{}", icon, name, selected ? "   " ICON_FA_CHECK : "");
        }
    };

    static void drawFavorites(assets::AssetType& selectedType)
    {
        ImGuiTreeNodeFlags rootFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (!ImGui::TreeNodeEx(ICON_FA_STAR " Favorites", rootFlags)) return;

        static constexpr FavoriteItem favorites[]{{ICON_FA_ADJUST, "Materials", assets::AssetType::MATERIAL},
                                                  {ICON_FA_CUBE, "Models", assets::AssetType::MODEL},
                                                  {ICON_FA_SQUARE, "Textures", assets::AssetType::TEXTURE}};

        for (const auto& fav : favorites) {
            const bool isSelected = (fav.type == selectedType);

            ImGuiTreeNodeFlags itemFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            if (isSelected) itemFlags |= ImGuiTreeNodeFlags_Selected;

            const auto label = fav.getLabel(isSelected);
            ImGui::TreeNodeEx(label.c_str(), itemFlags);

            if (ImGui::IsItemClicked()) {
                selectedType = isSelected ? assets::AssetType::UNKNOWN : fav.type;
            }
        }
        ImGui::TreePop();
    }

    /**
     * @brief Displays the context menu for the folder tree.
     *
     * This method provides options to create a new folder or import assets.
     * It is triggered by a right-click on the folder tree.
     */
    void AssetManagerWindow::folderTreeContextMenu()
    {
        if (ImGui::MenuItem("New Folder")) m_popupManager.openPopup("Create folder");
        if (ImGui::MenuItem("Import")) LOG(NEXO_INFO, "Importing assets is not implemented yet");

        PopupManager::closePopup();
    }

    void AssetManagerWindow::drawFolderTreeItem(const std::string& name, const std::string& path)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (path == m_currentFolder) flags |= ImGuiTreeNodeFlags_Selected;

        auto children = m_folderManager.getChildren(path);
        if (children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;

        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(230, 180, 80, 255));
        ImGui::Text(ICON_FA_FOLDER);
        ImGui::PopStyleColor();
        ImGui::SameLine();

        bool opened = ImGui::TreeNodeEx(name.c_str(), flags);

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen()) m_currentFolder = path;
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            m_folderActionState.reset();
            m_folderActionState.parentPath = path;
            m_popupManager.openPopup("Folder Tree Context Menu");
        }

        if (!opened) return;

        for (const auto& [childPath, childName] : children) {
            drawFolderTreeItem(childName, childPath);
        }
        ImGui::TreePop();
    }

    void AssetManagerWindow::drawFolderTree()
    {
        drawSearchBar(m_searchBuffer);
        drawFavorites(m_selectedType);

        ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_OpenOnDoubleClick;

        if (m_currentFolder.empty()) headerFlags |= ImGuiTreeNodeFlags_Selected;

        const bool assetsOpen = ImGui::TreeNodeEx(ICON_FA_FOLDER " Assets", headerFlags);

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) {
            m_folderActionState.reset();
            m_popupManager.openPopup("Folder Tree Context Menu");
        }
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) m_currentFolder = "";

        if (!assetsOpen) return;

        auto rootChildren = m_folderManager.getChildren("");
        for (const auto& [path, name] : rootChildren) {
            drawFolderTreeItem(name, path);
        }
        ImGui::TreePop();
    }
} // namespace nexo::editor
