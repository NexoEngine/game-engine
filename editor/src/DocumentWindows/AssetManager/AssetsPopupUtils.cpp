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
     * @brief Displays the right-click menu for assets.
     *
     * This method provides options for renaming, deleting, or viewing details
     * about an asset. It opens the corresponding popup based on the selected action.
     */
    void AssetManagerWindow::rightClickOnAssetMenu()
    {
        if (ImGui::MenuItem("Rename Asset")) {
            m_popupManager.openPopup("Rename Asset Popup");
        }
        if (ImGui::MenuItem("Delete Asset")) {
            m_popupManager.openPopup("Delete Asset Popup");
        }
        if (ImGui::MenuItem("Details")) {
            m_popupManager.openPopup("Details Asset Popup");
        }
        PopupManager::closePopup();
    }

    void AssetManagerWindow::deleteAssetPopup()
    {
        ImGui::Text("Are you sure you want to delete this asset?");
        // TODO: Implement the delete asset popup
        PopupManager::closePopup();
    }

    void AssetManagerWindow::deleteUsedAssetPopup()
    {
        ImGui::Text("This asset is used by one or more entities.\nAre you sure you want to delete it?");
        // TODO: Implement the delete used asset popup
        PopupManager::closePopup();
    }

    void AssetManagerWindow::renameAssetPopup()
    {
        ImGui::Text("Enter a new name for the asset:");
        constexpr size_t MAX_ASSET_NAME_LENGTH = 256;
        // TODO: Implement the rename asset popup
        PopupManager::closePopup();
    }

    void AssetManagerWindow::assetDetailsPopup() const
    {
        // TODO: Implement the asset details popup
        PopupManager::closePopup();
    }

} // namespace nexo::editor
