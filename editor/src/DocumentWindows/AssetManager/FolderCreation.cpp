//// FolderCreation.cpp ///////////////////////////////////////////////////////////////
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

#include "AssetManagerWindow.hpp"
#include "ImNexo/Elements.hpp"

namespace nexo::editor {

    bool AssetManagerWindow::handleNewFolderCreation()
    {
        if (m_folderCreationState.folderName.empty()) {
            m_folderCreationState.showError = true;
            m_folderCreationState.errorMessage = "Folder name cannot be empty";
            return false;
        }

        if (!m_folderManager.createFolder(m_folderCreationState.parentPath, m_folderCreationState.folderName)) {
            m_folderCreationState.showError = true;
            m_folderCreationState.errorMessage = "Failed to create folder (may already exist)";
            return false;
        }

        return true;
    }

    void AssetManagerWindow::newFolderMenu()
    {
        ImGui::Text("Enter name for the new folder:");
        constexpr size_t MAX_FOLDER_NAME_LENGTH = 256;
        m_folderCreationState.folderName.resize(MAX_FOLDER_NAME_LENGTH);
        ImGui::InputText("##FolderName", m_folderCreationState.folderName.data(), m_folderCreationState.folderName.capacity());
        m_folderCreationState.folderName.resize(strlen(m_folderCreationState.folderName.c_str()));
        ImGui::Separator();

        if (ImNexo::Button("Create") && handleNewFolderCreation()) {
            m_folderCreationState.reset();
            PopupManager::closePopupInContext();
        }
        ImGui::SameLine();
        if (ImNexo::Button("Cancel")) {
            m_folderCreationState.reset();
            PopupManager::closePopupInContext();
        }

        if (m_folderCreationState.showError) {
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
            ImGui::Text("%s", m_folderCreationState.errorMessage.c_str());
            ImGui::PopStyleColor();

            if (m_folderCreationState.errorTimer <= 0.0f) {
                m_folderCreationState.showError = false;
                m_folderCreationState.errorTimer = ERROR_DISPLAY_TIMEOUT; // Reset timer
            } else
                m_folderCreationState.errorTimer -= ImGui::GetIO().DeltaTime;
        }
        PopupManager::closePopup();
    }

}
