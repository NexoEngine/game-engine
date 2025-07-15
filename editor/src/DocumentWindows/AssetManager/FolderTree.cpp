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

namespace nexo::editor {
    void AssetManagerWindow::drawFolderTreeItem(const std::string& name, const std::string& path)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

        // Check if this is the selected folder
        if (path == m_currentFolder)
            flags |= ImGuiTreeNodeFlags_Selected;

        // Check if this is a leaf node (no subfolders)
        bool hasSubfolders = false;
        for (const auto& [subPath, _] : m_folderStructure) {
            if (subPath != path && subPath.find(path) == 0 &&
                subPath.find('/', path.length() + 1) == std::string::npos) {
                hasSubfolders = true;
                break;
            }
        }

        if (!hasSubfolders)
            flags |= ImGuiTreeNodeFlags_Leaf;

        // Folder icon
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(230, 180, 80, 255));
        ImGui::Text(ICON_FA_FOLDER);
        ImGui::PopStyleColor();
        ImGui::SameLine();

        bool opened = ImGui::TreeNodeEx(name.c_str(), flags);

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            m_currentFolder = path;

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("New Folder")) {
                m_folderCreationState.parentPath = path;
                m_folderCreationState.isCreatingFolder = true;
                ImGui::OpenPopup("Create New Folder");
                snprintf(m_folderCreationState.folderName,
                         sizeof(m_folderCreationState.folderName),
                         "%s", "New Folder");
            }
            ImGui::EndPopup();
        }

        if (opened) {
            for (const auto& [subPath, subName] : m_folderStructure) {
                if (subPath != path &&
                    subPath.find(path) == 0 &&
                    subPath.length() > path.length() &&
                    subPath.find('/', path.length() + 1) == std::string::npos) {
                    drawFolderTreeItem(subName, subPath);
                }
            }
            ImGui::TreePop();
        }
    }

    void AssetManagerWindow::handleNewFolderCreation()
    {
        if (m_folderCreationState.isCreatingFolder) {
            ImGui::OpenPopup("Create New Folder");

            // Center the popup
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal("Create New Folder", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Enter name for new folder:");
                ImGui::InputText("##FolderName", m_folderCreationState.folderName, sizeof(m_folderCreationState.folderName));

                ImGui::Separator();

                if (ImGui::Button("Create", ImVec2(120, 0))) {
                    if (strnlen(m_folderCreationState.folderName, sizeof(m_folderCreationState.folderName)) > 0) {
                        std::string newFolderPath;
                        if (m_folderCreationState.parentPath.empty())
                            newFolderPath = m_folderCreationState.folderName;
                        else
                            newFolderPath = m_folderCreationState.parentPath + "/" + m_folderCreationState.folderName;

                        // Check if folder already exists
                        bool folderExists = false;
                        for (const auto& [path, _] : m_folderStructure) {
                            if (path == newFolderPath) {
                                folderExists = true;
                                break;
                            }
                        }

                        if (!folderExists) {
                            m_folderStructure.emplace_back(newFolderPath, m_folderCreationState.folderName);
                            LOG(NEXO_INFO, "Created new folder: {}", newFolderPath);

                            m_folderCreationState.isCreatingFolder = false;
                            ImGui::CloseCurrentPopup();
                        } else {
                            m_folderCreationState.showError = true;
                            m_folderCreationState.errorMessage = "Folder already exists";
                        }
                    } else {
                        m_folderCreationState.showError = true;
                        m_folderCreationState.errorMessage = "Folder name cannot be empty";
                    }
                }

                ImGui::SameLine();
                if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                    m_folderCreationState.isCreatingFolder = false;
                    ImGui::CloseCurrentPopup();
                }

                // Display error message if needed
                if (m_folderCreationState.showError) {
                    ImGui::Separator();
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    ImGui::Text("%s", m_folderCreationState.errorMessage.c_str());
                    ImGui::PopStyleColor();

                    // Clear error after a few seconds
                    if (m_folderCreationState.errorTimer <= 0.0f) {
                        m_folderCreationState.showError = false;
                        m_folderCreationState.errorTimer = 3.0f; // Reset timer
                    } else {
                        m_folderCreationState.errorTimer -= ImGui::GetIO().DeltaTime;
                    }
                }

                ImGui::EndPopup();
            }
        }
    }

    void AssetManagerWindow::buildFolderStructure()
    {
        m_folderStructure.clear();
        m_folderStructure.emplace_back("", "Assets");

        const auto assets = assets::AssetCatalog::getInstance().getAssets();
        for (const auto& asset : assets) {
            if (auto assetData = asset.lock()) {
                std::string fullPath = assetData->getMetadata().location.getPath();
                size_t pos = 0;
                std::string currentPath;

                while ((pos = fullPath.find('/', pos)) != std::string::npos) {
                    std::string folderPath = fullPath.substr(0, pos);
                    std::string folderName = folderPath;

                    // Extract just the folder name from the path
                    size_t lastSlash = folderPath.find_last_of('/');
                    if (lastSlash != std::string::npos)
                        folderName = folderPath.substr(lastSlash + 1);

                    bool found = false;
                    for (const auto& [path, _] : m_folderStructure) {
                        if (path == folderPath) {
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                        m_folderStructure.emplace_back(folderPath, folderName);

                    pos++;
                }
            }
        }
    }

    void AssetManagerWindow::drawFolderTree()
    {
        handleNewFolderCreation();

        ImGui::PushItemWidth(-1);
        ImGui::InputTextWithHint("##search", "Search...", m_searchBuffer, sizeof(m_searchBuffer));
        ImGui::PopItemWidth();
        ImGui::Separator();

        // Draw favorites section
        {
            ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;
            bool favoritesOpen = ImGui::TreeNodeEx(ICON_FA_STAR " Favorites", headerFlags);

            if (favoritesOpen) {
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
                    bool isSelected = (fav.type == m_selectedType);

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

        // Draw folder structure
        {
            ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_OpenOnDoubleClick;

            if (m_currentFolder.empty()) {
                headerFlags |= ImGuiTreeNodeFlags_Selected;
            }

            bool assetsOpen = ImGui::TreeNodeEx(ICON_FA_FOLDER " Assets", headerFlags);

            // Handle right-click on Assets root
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("New Folder")) {
                    m_folderCreationState.parentPath = "";
                    m_folderCreationState.isCreatingFolder = true;
                    snprintf(m_folderCreationState.folderName,
                             sizeof(m_folderCreationState.folderName),
                             "%s", "New Folder");
                }
                ImGui::EndPopup();
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
