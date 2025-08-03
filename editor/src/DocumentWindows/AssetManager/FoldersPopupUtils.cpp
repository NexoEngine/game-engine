//// AssetManagerPopup.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        03/08/2025
//  Description: Source file for the popup used in asset manager
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include "ImNexo/Elements.hpp"

namespace nexo::editor {

    /**
     * @brief Displays the right-click menu for folders.
     *
     * This method provides options for renaming, deleting, or viewing details
     * about a folder. It opens the corresponding popup based on the selected action.
     */
    void AssetManagerWindow::rightClickOnFolderMenu()
    {
        // Ensure the folder action state is set up correctly
        const std::string folderPath = m_folderActionState.parentPath.empty() ?
                                           m_folderActionState.folderName :
                                           m_folderActionState.parentPath + "/" + m_folderActionState.folderName;
        m_hoveredFolder              = folderPath;

        // Draw the right-click menu for folders
        if (ImGui::MenuItem("Rename Folder")) {
            m_popupManager.openPopup("Rename Folder Popup");
        }
        if (ImGui::MenuItem("Delete Folder")) {
            m_popupManager.openPopup("Delete Folder Popup");
        }
        if (ImGui::MenuItem("Details")) {
            m_popupManager.openPopup("Details Folder Popup");
        }
        PopupManager::closePopup();
    }

    /**
     * @brief Handles the creation of a new folder.
     *
     * This method validates the folder name and attempts to create a new folder
     * using the folder manager. If the folder name is empty or the creation fails,
     * an error message is displayed.
     *
     * @return `true` if the folder is successfully created, `false` otherwise.
     */
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

    /**
     * @brief Displays the menu for creating a new folder.
     *
     * This method provides a user interface for entering a folder name and creating
     * a new folder. It validates the input and handles errors if the creation fails.
     */
    void AssetManagerWindow::createFolderPopup()
    {
        // Ensure the folder action state is set up correctly
        const std::string folderPath = m_folderActionState.parentPath.empty() ?
                                           m_folderActionState.folderName :
                                           m_folderActionState.parentPath + "/" + m_folderActionState.folderName;
        m_hoveredFolder              = folderPath;

        // Draw the popup for creating a new folder
        ImGui::Text("Enter name for the new folder:");
        constexpr size_t MAX_FOLDER_NAME_LENGTH = 256;
        m_folderActionState.folderName.resize(MAX_FOLDER_NAME_LENGTH);
        ImGui::InputText("##FolderName", m_folderActionState.folderName.data(),
                         m_folderActionState.folderName.capacity());
        m_folderActionState.folderName.resize(strlen(m_folderActionState.folderName.c_str()));
        ImGui::Separator();

        if (ImNexo::Button("Create", true) && handleFolderCreation()) {
            m_folderActionState.reset();
            PopupManager::closePopupInContext();
        }
        ImGui::SameLine();
        if (ImNexo::Button("Cancel")) {
            m_folderActionState.reset();
            PopupManager::closePopupInContext();
        }

        // Display error message if any
        drawErrorMessageInPopup();

        PopupManager::closePopup();
    }

    /**
     * @brief Displays the menu for deleting a folder.
     *
     * This method confirms the deletion of a folder and checks if the folder
     * contains any assets. If the folder is not empty or the deletion fails,
     * an error message is displayed.
     */
    void AssetManagerWindow::deleteFolderPopup()
    {
        // Ensure the folder action state is set up correctly
        const std::string folderPath = m_folderActionState.parentPath.empty() ?
                                           m_folderActionState.folderName :
                                           m_folderActionState.parentPath + "/" + m_folderActionState.folderName;
        m_hoveredFolder              = folderPath;

        // Draw the popup for deleting a folder
        ImGui::Text("Are you sure you want to delete %s?", m_folderActionState.folderName.c_str());
        ImGui::Separator();
        if (ImNexo::Button("Delete", true)) {
            const std::vector<assets::GenericAssetRef> assets = m_folderManager.getFolderAssets(folderPath);
            if (!assets.empty()) {
                m_popupManager.openPopup("Delete Not Empty Folder Popup");
                m_folderActionState.showError    = true;
                m_folderActionState.errorMessage = "Are you sure you want to delete this folder? It contains assets.";
            } else if (m_folderManager.deleteFolder(folderPath)) {
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

        // Display error message if any
        drawErrorMessageInPopup();

        PopupManager::closePopup();
    }

    void AssetManagerWindow::deleteNotEmptyFolderPopup()
    {
        // Ensure the folder action state is set up correctly
        const std::string folderPath = m_folderActionState.parentPath.empty() ?
                                           m_folderActionState.folderName :
                                           m_folderActionState.parentPath + "/" + m_folderActionState.folderName;
        m_hoveredFolder              = folderPath;

        // Draw the popup for deleting a folder that contains assets
        ImGui::Text("Are you sure you want to delete %s? It contains assets.", m_folderActionState.folderName.c_str());
        ImGui::Separator();
        if (ImNexo::Button("Delete", true) && m_folderManager.deleteFolder(folderPath)) {
            m_folderActionState.reset();
            PopupManager::closePopupInContext();
        } else {
            m_folderActionState.showError    = true;
            m_folderActionState.errorMessage = "Failed to delete the folder (may not be empty)";
        }
        ImGui::SameLine();
        if (ImNexo::Button("Cancel")) {
            m_folderActionState.reset();
            PopupManager::closePopupInContext();
        }

        // Display error message if any
        drawErrorMessageInPopup();

        PopupManager::closePopup();
    }

    /**
     * @brief Handles the renaming of a folder.
     *
     * This method validates the current folder name and attempts to rename the folder
     * to the specified new name using the folder manager. If the folder name is empty
     * or the renaming fails, an error message is displayed.
     *
     * @param newName The new name for the folder.
     * @return `true` if the folder is successfully renamed, `false` otherwise.
     */
    bool AssetManagerWindow::handleFolderRenaming(const std::string &newName)
    {
        if (m_folderActionState.folderName.empty()) {
            m_folderActionState.showError    = true;
            m_folderActionState.errorMessage = "Folder name cannot be empty";
            return false;
        }

        std::string actualPath = m_folderActionState.parentPath.empty() ?
                                     m_folderActionState.folderName :
                                     m_folderActionState.parentPath + "/" + m_folderActionState.folderName;
        actualPath.resize(strlen(actualPath.c_str())); // Ensure the path is properly resized

        const std::string resizedActualPath = actualPath;
        if (!m_folderManager.renameFolder(resizedActualPath, newName)) {
            m_folderActionState.showError    = true;
            m_folderActionState.errorMessage = "Failed to rename the folder (may already exist)";
            return false;
        } else {
            const std::string newFolderPath =
                m_folderActionState.parentPath.empty() ? newName : m_folderActionState.parentPath + "/" + newName;
            // Get all assets
            const assets::AssetCatalog &assetCatalog    = assets::AssetCatalog::getInstance();
            std::vector<assets::GenericAssetRef> assets = assetCatalog.getAssets();

            // Update all assets
            for (const auto &ref : assets) {
                const auto d = ref.lock();
                if (!d) continue;
                std::string assetPath = d->getMetadata().location.getPath();
                if (assetPath == actualPath || assetPath.starts_with(actualPath + "/")) {
                    std::string newAssetPath = newFolderPath + assetPath.substr(actualPath.size());
                    d->getMetadata().location.setPath(newAssetPath);
                }
            }
        }

        return true;
    }

    /**
     * @brief Displays the menu for renaming a folder.
     *
     * This method provides a user interface for entering a new name for a folder
     * and handles the renaming process. It validates the input and displays errors
     * if the renaming fails.
     */
    void AssetManagerWindow::renameFolderPopup()
    {
        // Ensure the folder action state is set up correctly
        const std::string folderPath = m_folderActionState.parentPath.empty() ?
                                           m_folderActionState.folderName :
                                           m_folderActionState.parentPath + "/" + m_folderActionState.folderName;
        m_hoveredFolder = folderPath;

        // Draw the popup for renaming a folder
        ImGui::Text("Enter a new name for the folder:");

        // Input text for the new folder name
        constexpr size_t MAX_FOLDER_NAME_LENGTH = 256;
        static std::string newName              = m_folderActionState.folderName;
        if (newName.empty()) {
            newName = m_folderActionState.folderName;
        }
        m_folderActionState.folderName.resize(MAX_FOLDER_NAME_LENGTH);
        ImGui::InputText("##FolderName", newName.data(), m_folderActionState.folderName.capacity());
        newName.resize(strlen(newName.c_str()));
        ImGui::Separator();

        // Buttons for renaming or canceling the action
        if (ImNexo::Button("Rename", true) && handleFolderRenaming(newName)) {
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

        // Display error message if any
        drawErrorMessageInPopup();

        PopupManager::closePopup();
    }

    /**
     * @brief Displays the details of a folder.
     *
     * This method shows information about the selected folder, including its name,
     * path, and child count. It also lists all available folder paths.
     */
    void AssetManagerWindow::folderDetailsPopup()
    {
        // Ensure the folder action state is set up correctly
        const std::string folderPath = m_folderActionState.parentPath.empty() ?
                                           m_folderActionState.folderName :
                                           m_folderActionState.parentPath + "/" + m_folderActionState.folderName;
        m_hoveredFolder = folderPath;

        // Draw the popup for folder details
        ImGui::Text("Details of: %s", m_folderActionState.folderName.c_str());
        ImGui::Separator();
        ImGui::Text("Name: %s", m_folderActionState.folderName.c_str());
        ImGui::Text("Path: %s", folderPath.c_str());
        ImGui::Text("Child: %zu", m_folderManager.getChildCount(folderPath));
        ImGui::Separator();
        if (ImNexo::Button("Close")) {
            PopupManager::closePopupInContext();
        }
        PopupManager::closePopup();
    }

} // namespace nexo::editor
