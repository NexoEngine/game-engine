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
        PopupManager::endPopup();
    }

    void AssetManagerWindow::deleteAssetPopup()
    {
        const std::string assetName = m_assetActionState.assetData->m_metadata.location.getName().c_str();

        ImGui::Text("Are you sure you want to delete %s?", assetName.c_str());
        ImGui::Separator();
        if (Button("Delete", ImNexo::VALIDATION)) {
            // TODO: Check if the asset is used before deleting
            // if (m_assetActionState.assetData->isUsed()) {
            //     m_popupManager.openPopup("Delete Used Asset Popup");
            //     m_assetActionState.showError    = true;
            //     m_assetActionState.errorMessage = "Are you sure you want to delete this asset? It is currently
            //     used.";
            // } else {
            if (assets::AssetCatalog::getInstance().deleteAsset(m_assetActionState.assetData->getID())) {
                m_assetActionState.reset();
                PopupManager::closePopup();
            } else {
                m_assetActionState.showError    = true;
                m_assetActionState.errorMessage = "Failed to delete the asset (may currently be in use)";
            }
            // }
            ImGui::SameLine();
            if (Button("Cancel", ImNexo::CANCEL)) {
                m_assetActionState.reset();
                PopupManager::closePopup();
            }
        }

        // Display error message if any
        drawErrorMessageInPopup(m_assetActionState);

        PopupManager::endPopup();
    }

    void AssetManagerWindow::deleteUsedAssetPopup()
    {
        const std::string assetName = m_assetActionState.assetData->m_metadata.location.getName().c_str();

        ImGui::Text("%s is used by one or more entities.\nAre you sure you want to delete it?", assetName.c_str());
        ImGui::Separator();
        if (Button("Delete", ImNexo::VALIDATION) &&
            assets::AssetCatalog::getInstance().deleteAsset(m_assetActionState.assetData->getID())) {
            m_assetActionState.reset();
            PopupManager::closePopup();
        } else {
            m_assetActionState.showError    = true;
            m_assetActionState.errorMessage = "Failed to delete the asset (may currently be in use)";
        }
        ImGui::SameLine();
        if (Button("Cancel", ImNexo::CANCEL)) {
            m_assetActionState.reset();
            PopupManager::closePopup();
        }

        // Display error message if any
        drawErrorMessageInPopup(m_assetActionState);

        PopupManager::endPopup();
    }

    bool AssetManagerWindow::handleAssetRenaming(const std::string &newName)
    {
        const std::string assetName = m_assetActionState.assetData->m_metadata.location.getName().c_str();

        if (assetName.empty() || newName.empty()) {
            m_assetActionState.showError    = true;
            m_assetActionState.errorMessage = "Asset name cannot be empty";
            return false;
        }

        if (!FolderManager::isNameValid(newName)) {
            m_assetActionState.showError    = true;
            m_assetActionState.errorMessage = "Asset name is invalid";
            return false;
        }

        const assets::GenericAssetRef assetRef =
            assets::AssetCatalog::getInstance().getAsset(m_assetActionState.assetData->getID());
        if (!assets::AssetCatalog::getInstance().renameAsset(assetRef, newName)) {
            m_assetActionState.showError    = true;
            m_assetActionState.errorMessage = "Failed to rename the asset (may already exist)";
            return false;
        }
        return true;
    }

    void AssetManagerWindow::renameAssetPopup()
    {
        static bool isFocus = true;
        ImGui::Text("Enter a new name for the asset:");

        // Input text for the new asset name
        std::string assetName                  = m_assetActionState.assetData->m_metadata.location.getName().c_str();
        constexpr size_t MAX_ASSET_NAME_LENGTH = 256;
        static std::string newName             = assetName;
        if (newName.empty()) {
            newName = assetName;
        }
        assetName.resize(MAX_ASSET_NAME_LENGTH);
        if (isFocus) {
            ImGui::SetKeyboardFocusHere();
            isFocus = false;
        }
        ImGui::InputText("##AssetName", newName.data(), assetName.capacity(), ImGuiInputTextFlags_AutoSelectAll);
        newName.resize(strlen(newName.c_str()));
        ImGui::Separator();

        // Buttons for renaming or canceling the action
        if (Button("Rename", ImNexo::VALIDATION) && handleAssetRenaming(newName)) {
            m_assetActionState.reset();
            newName = "";
            PopupManager::closePopup();
            isFocus = true;
        }
        ImGui::SameLine();
        if (Button("Cancel", ImNexo::CANCEL)) {
            m_assetActionState.reset();
            newName = "";
            PopupManager::closePopup();
            isFocus = true;
        }

        // Display error message if any
        drawErrorMessageInPopup(m_assetActionState);

        PopupManager::endPopup();
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
        if (Button("Close", ImNexo::CANCEL)) {
            m_assetActionState.reset();
            PopupManager::closePopup();
        }
        PopupManager::endPopup();
    }

} // namespace nexo::editor
