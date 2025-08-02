//// ActionOnFolder.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        27/07/2025
//  Description: Source file for the method used to create a new folder
//
///////////////////////////////////////////////////////////////////////////////

#include <gmock/internal/gmock-internal-utils.h>

#include "AssetManagerWindow.hpp"
#include "ImNexo/Elements.hpp"

namespace nexo::editor {

    void AssetManagerWindow::handleRightClickOnFolder()
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !m_hoveredFolder.empty()) {
            m_folderActionState.folderName       = m_hoveredFolder;
            m_folderActionState.isManagingFolder = true;
            std::cout << "Right click on folder: " << m_hoveredFolder << std::endl;
            m_popupManager.openPopup("Folder Right Click Menu");
        }
    }

    bool AssetManagerWindow::handleFolderCreation()
    {
        if (m_folderActionState.folderName.empty()) {
            m_folderActionState.showError    = true;
            m_folderActionState.errorMessage = "Folder name cannot be empty";
            return false;
        }

        if (!m_folderManager.createFolder(m_folderActionState.parentPath, m_folderActionState.folderName)) {
            m_folderActionState.showError    = true;
            m_folderActionState.errorMessage = "Failed to create the folder (may already exist)";
            return false;
        }

        return true;
    }

    bool AssetManagerWindow::handleFolderRenaming(const std::string &newName)
    {
        if (m_folderActionState.folderName.empty()) {
            m_folderActionState.showError    = true;
            m_folderActionState.errorMessage = "Folder name cannot be empty";
            return false;
        }

        const std::string actualPath = m_folderActionState.parentPath.empty() ?
                                           m_folderActionState.folderName :
                                           m_folderActionState.parentPath;
        if (!m_folderManager.renameFolder(actualPath, newName)) {
            m_folderActionState.showError    = true;
            m_folderActionState.errorMessage = "Failed to rename the folder (may already exist)";
            return false;
        }

        return true;
    }

    void AssetManagerWindow::createFolderMenu()
    {
        ImGui::Text("Enter name for the new folder:");
        constexpr size_t MAX_FOLDER_NAME_LENGTH = 256;
        m_folderActionState.folderName.resize(MAX_FOLDER_NAME_LENGTH);
        ImGui::InputText("##FolderName", m_folderActionState.folderName.data(),
                         m_folderActionState.folderName.capacity());
        m_folderActionState.folderName.resize(strlen(m_folderActionState.folderName.c_str()));
        ImGui::Separator();

        if (ImNexo::Button("Create") && handleFolderCreation()) {
            m_folderActionState.reset();
            PopupManager::closePopupInContext();
        }
        ImGui::SameLine();
        if (ImNexo::Button("Cancel")) {
            m_folderActionState.reset();
            PopupManager::closePopupInContext();
        }

        if (m_folderActionState.showError) {
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::Text("%s", m_folderActionState.errorMessage.c_str());
            ImGui::PopStyleColor();

            if (m_folderActionState.errorTimer <= 0.0f) {
                m_folderActionState.showError  = false;
                m_folderActionState.errorTimer = ERROR_DISPLAY_TIMEOUT; // Reset timer
            } else
                m_folderActionState.errorTimer -= ImGui::GetIO().DeltaTime;
        }
        PopupManager::closePopup();
    }

    void AssetManagerWindow::rightClickMenu()
    {
        // Handle the right click on any asset type
    }

    void AssetManagerWindow::folderRightClickMenu()
    {
        if (ImGui::MenuItem("Rename Folder")) {
            m_popupManager.openPopup("Rename folder");
        }
        if (ImGui::MenuItem("Delete Folder")) {
            m_popupManager.openPopup("Delete folder");
        }
        if (ImGui::MenuItem("Détails")) {
            m_popupManager.openPopup("Details about folder");
        }
        PopupManager::closePopup();
    }

    void AssetManagerWindow::deleteFolderMenu()
    {
        ImGui::Text("Are you sure you want to delete %s?", m_folderActionState.folderName.c_str());
        ImGui::Separator();
        if (ImNexo::Button("Delete")) {
            // TODO: Check if folder contains assets - you might want to prevent deletion
            const std::vector<assets::GenericAssetRef> assets =
                m_folderManager.getFolderAssets(m_folderActionState.parentPath);
            if (!assets.empty()) {
                m_folderActionState.showError    = true;
                m_folderActionState.errorMessage = "Folder is not empty, deletion not allowed";
            } else if (m_folderManager.deleteFolder(m_folderActionState.parentPath)) {
                m_folderActionState.reset();
                PopupManager::closePopupInContext();
            } else {
                m_folderActionState.showError    = true;
                m_folderActionState.errorMessage = "Failed to delete the folder (may not be empty)";
            }
        }
        ImGui::SameLine();
        if (ImNexo::Button("Cancel")) {
            m_folderActionState.reset();
            PopupManager::closePopupInContext();
        }

        if (m_folderActionState.showError) {
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::Text("%s", m_folderActionState.errorMessage.c_str());
            ImGui::PopStyleColor();

            if (m_folderActionState.errorTimer <= 0.0f) {
                m_folderActionState.showError  = false;
                m_folderActionState.errorTimer = ERROR_DISPLAY_TIMEOUT; // Reset timer
            } else
                m_folderActionState.errorTimer -= ImGui::GetIO().DeltaTime;
        }
        PopupManager::closePopup();
    }

    void AssetManagerWindow::renameFolderMenu()
    {
        ImGui::Text("Enter a new name for the folder:");
        constexpr size_t MAX_FOLDER_NAME_LENGTH = 256;
        static std::string newName = m_folderActionState.folderName;
        if (newName.empty()) {
            newName = m_folderActionState.folderName;
        }
        m_folderActionState.folderName.resize(MAX_FOLDER_NAME_LENGTH);
        ImGui::InputText("##FolderName", newName.data(), m_folderActionState.folderName.capacity());
        newName.resize(strlen(newName.c_str()));
        ImGui::Separator();

        if (ImNexo::Button("Rename") && handleFolderRenaming(newName)) {
            m_folderActionState.reset();
            newName = "";
            PopupManager::closePopupInContext();
        }
        ImGui::SameLine();
        if (ImNexo::Button("Cancel")) {
            m_folderActionState.reset();
            newName = "";
            PopupManager::closePopupInContext();
        }

        if (m_folderActionState.showError) {
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::Text("%s", m_folderActionState.errorMessage.c_str());
            ImGui::PopStyleColor();

            if (m_folderActionState.errorTimer <= 0.0f) {
                m_folderActionState.showError  = false;
                m_folderActionState.errorTimer = ERROR_DISPLAY_TIMEOUT; // Reset timer
            } else
                m_folderActionState.errorTimer -= ImGui::GetIO().DeltaTime;
        }
        PopupManager::closePopup();
    }

    void AssetManagerWindow::folderDetailsMenu() const
    {
        ImGui::Text("Details of: %s", m_folderActionState.folderName.c_str());
        ImGui::Separator();
        ImGui::Text("Name: %s", m_folderActionState.folderName.c_str());
        const std::string& folderPath = m_folderActionState.parentPath.empty() ? m_folderActionState.folderName :
                                            m_folderActionState.parentPath + "/" + m_folderActionState.folderName;
        ImGui::Text("Path: %s", folderPath);
        ImGui::Text("Child: %zu", m_folderManager.getChildCount(m_folderActionState.parentPath + "/" + m_folderActionState.folderName));
        ImGui::Separator();
        std::vector<std::string> allPaths = m_folderManager.getAllPaths();
        std::ranges::sort(allPaths);
        ImGui::Text("All Paths:");
        for (const auto& path : allPaths) {
            ImGui::BulletText("%s", path.c_str());
        }
        ImGui::Separator();
        ImGui::Separator();
        if (ImNexo::Button("Close")) {
            PopupManager::closePopupInContext();
        }
        PopupManager::closePopup();
    }
} // namespace nexo::editor
