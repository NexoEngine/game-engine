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

#include <ImNexo/Elements.hpp>

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
        // Draw the right-click menu for assets
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

    bool AssetManagerWindow::handleAssetRenaming(const std::string &newName)
    {
        const std::string assetName = m_assetActionState.assetData->m_metadata.location.getName().c_str();

        if (assetName.empty() || newName.empty()) {
            m_assetActionState.showError    = true;
            m_assetActionState.errorMessage = "Asset name cannot be empty";
            return false;
        }

        assets::GenericAssetRef assetRef =
            assets::AssetCatalog::getInstance().getAsset(m_assetActionState.assetData->getID());
        if (assets::AssetCatalog::getInstance().renameAsset(assetRef, newName)) {
            return true;
        }
        m_assetActionState.showError    = true;
        m_assetActionState.errorMessage = "Failed to rename the asset (may already exist)";
        return false;
    }

    void AssetManagerWindow::renameAssetPopup()
    {
        ImGui::Text("Enter a new name for the asset:");

        // Input text for the new folder name
        std::string assetName = m_assetActionState.assetData->m_metadata.location.getName().c_str();
        constexpr size_t MAX_ASSET_NAME_LENGTH = 256;
        static std::string newName = assetName;
        if (newName.empty()) {
            newName = assetName;
        }
        assetName.resize(MAX_ASSET_NAME_LENGTH);
        ImGui::InputText("##AssetName", newName.data(), assetName.capacity());
        newName.resize(strlen(newName.c_str()));
        ImGui::Separator();

        // Buttons for renaming or canceling the action
        if (ImNexo::Button("Rename", true) && handleAssetRenaming(newName)) {
            m_assetActionState.reset();
            newName = "";
            PopupManager::closePopupInContext();
        }
        ImGui::SameLine();
        if (ImNexo::Button("Cancel")) {
            m_assetActionState.reset();
            newName = "";
            PopupManager::closePopupInContext();
        }

        // Display error message if any
        drawErrorMessageInPopup();

        PopupManager::closePopup();
    }

    void AssetManagerWindow::assetDetailsPopup()
    {
        const std::string assetName = m_assetActionState.assetData->m_metadata.location.getName().c_str();
        ImGui::Text("Details of: %s", assetName.c_str());
        ImGui::Separator();
        ImGui::Text("Name: %s", assetName.c_str());
        ImGui::Text("Path: %s", m_assetActionState.assetData->m_metadata.location.getPath().c_str());
        ImGui::Text("Type: %s", getAssetTypeName(m_assetActionState.assetData->m_metadata.type));
        ImGui::Text("Status: %s", m_assetActionState.assetData->isLoaded() ? "Loaded" : "Not Loaded");

        ImGui::Separator();
        if (ImNexo::Button("Close")) {
            PopupManager::closePopupInContext();
            m_folderActionState.reset();
        }
        PopupManager::closePopup();
    }

} // namespace nexo::editor
