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

        std::string newFolderPath = (m_folderCreationState.parentPath.empty()) ? "" : m_folderCreationState.parentPath + "/";
        newFolderPath += m_folderCreationState.folderName;

        const bool folderExists = std::ranges::any_of(m_folderStructure,
            [&newFolderPath](const auto& folder) {
                return folder.first == newFolderPath;
            }
        );

        if (folderExists) {
            m_folderCreationState.showError = true;
            m_folderCreationState.errorMessage = "Folder already exists";
            return false;
        }

        m_folderStructure.emplace_back(newFolderPath, m_folderCreationState.folderName);

        std::sort(
            m_folderStructure.begin() + 1,
            m_folderStructure.end(),
            [](const auto& a, const auto& b) {
                return a.first < b.first;
            }
        );
        updateFolderChildren();
        return true;
    }

    void AssetManagerWindow::newFolderContextMenu()
    {


        ImGui::Text("Enter name for the new folder:");
        ImGui::InputText("##FolderName", m_folderCreationState.folderName.data(), m_folderCreationState.folderName.size());

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
                m_folderCreationState.errorTimer = 3.0f; // Reset timer
            } else
                m_folderCreationState.errorTimer -= ImGui::GetIO().DeltaTime;
        }
        PopupManager::closePopup();
    }

}
