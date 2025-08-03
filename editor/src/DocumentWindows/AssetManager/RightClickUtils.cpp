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

namespace nexo::editor {

    /**
     * @brief Handles the right-click event on the asset manager window.
     *
     * This method checks if the right mouse button is clicked while hovering over the
     * asset manager window. If so, it opens the "Right click on AssetManager" popup
     * with the current folder path set in the folder action state.
     */
    void AssetManagerWindow::handleRightClickOnAssetManager()
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered()) {
            m_folderActionState.parentPath = m_currentFolder;
            m_popupManager.openPopup("Right click on AssetManager");
        }
    }

    /**
     * @brief Handles a right-click event on a folder.
     *
     * This method checks if the right mouse button is clicked while hovering over a folder.
     * If so, it sets the folder action state to manage the selected folder and opens the
     * "Right click on Folder" popup.
     */
    void AssetManagerWindow::handleRightClickOnFolder()
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !m_hoveredFolder.empty()) {
            m_folderActionState.folderName       = m_folderManager.getName(m_hoveredFolder);
            m_folderActionState.isManagingFolder = true;
            m_popupManager.openPopup("Right click on Folder");
        }
    }

    void AssetManagerWindow::handleRightClickOnAsset()
    {
        // TODO: Implement right-click handling for assets
        // if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        //     m_popupManager.openPopup("Right click on Asset");
        // }
    }

} // namespace nexo::editor
