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
#include "DocumentWindows/PopupManager.hpp"
#include "assets/AssetRef.hpp"
#include "Logger.hpp"

#include <imgui.h>
#include <nlohmann/json.hpp>

#include "assets/AssetCatalog.hpp"
#include "assets/AssetImporterInput.hpp"
#include "assets/AssetImporterContext.hpp"
#include "assets/AssetImporter.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include "assets/Assets/Texture/TextureParameters.hpp"

#ifdef NX_GRAPHICS_API_OPENGL
  #include "renderer/Texture.hpp"
#endif

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
        if (Button("Delete", ImNexo::ButtonTypes::VALIDATION)) {
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
            if (Button("Cancel", ImNexo::ButtonTypes::CANCEL)) {
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
        if (Button("Delete", ImNexo::ButtonTypes::VALIDATION) &&
            assets::AssetCatalog::getInstance().deleteAsset(m_assetActionState.assetData->getID())) {
            m_assetActionState.reset();
            PopupManager::closePopup();
        } else {
            m_assetActionState.showError    = true;
            m_assetActionState.errorMessage = "Failed to delete the asset (may currently be in use)";
        }
        ImGui::SameLine();
        if (Button("Cancel", ImNexo::ButtonTypes::CANCEL)) {
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
        constexpr size_t MAX_ASSET_NAME_LENGTH = 256;
        static std::string newName;
        std::string assetName = m_assetActionState.assetData->m_metadata.location.getName().c_str();
        if (newName.empty()) {
            newName = assetName;
        }
        assetName.resize(MAX_ASSET_NAME_LENGTH);
        if (isFocus) {
            ImGui::SetKeyboardFocusHere();
            isFocus = false;
        }
        ImGui::InputText("##AssetName", newName.data(), MAX_ASSET_NAME_LENGTH, ImGuiInputTextFlags_AutoSelectAll);
        newName.resize(strlen(newName.c_str()));
        ImGui::Separator();

        // Buttons for renaming or canceling the action
        if (Button("Rename", ImNexo::ButtonTypes::VALIDATION) && handleAssetRenaming(newName)) {
            m_assetActionState.reset();
            newName = "";
            PopupManager::closePopup();
            isFocus = true;
        }
        ImGui::SameLine();
        if (Button("Cancel", ImNexo::ButtonTypes::CANCEL)) {
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
        if (Button("Close", ImNexo::ButtonTypes::CANCEL)) {
            m_assetActionState.reset();
            PopupManager::closePopup();
        }
        PopupManager::endPopup();
    }

    void AssetManagerWindow::textureImporterPopup()
    {
        using nexo::assets::AssetCatalog;
        using nexo::assets::AssetImporter;
        using nexo::assets::AssetImporterContext;
        using nexo::assets::GenericAssetRef;
        using nexo::assets::Texture;
        using nexo::assets::TextureImportParameters;

        if (m_pendingImportedAsset == GenericAssetRef::null()) {
            LOG(NEXO_ERROR, "No asset to import");
            PopupManager::closePopup();
            return;
        }

        // -------- persistent UI state (local to this popup) --------
        static bool                    s_inited   = false;
        static TextureImportParameters s_params{};      // tweakable params
        static std::string             s_errorMsg;      // last error

        if (!s_inited) {
            s_params  = TextureImportParameters{};      // defaults
            s_errorMsg.clear();
            s_inited  = true;
        }

        auto deletePreviewIfAny = [&]() {
            if (m_pendingImportedAsset != GenericAssetRef::null()) {
                AssetCatalog::getInstance().deleteAsset(m_pendingImportedAsset);
                m_pendingImportedAsset = GenericAssetRef::null();
            }
        };

        // Re-import helper (used by Reload/Save)
        auto reimportWithParams = [&](bool keepOldOnFailure) -> bool {
            s_errorMsg.clear();

            GenericAssetRef old = m_pendingImportedAsset;

            // Build a context that carries params
            AssetImporterContext ctx;
            ctx.location = old.lock()->getMetadata().location;
            ctx.input    = assets::ImporterFileInput(m_pendingImportedAssetPath);
            ctx.setParameters(s_params); // <-- this uses your JSON path internally

            AssetImporter importer;
            importer.setCustomContext(&ctx); // ensure our params are used
            auto newly = importer.importAssetAuto(ctx.location, ctx.input);
            importer.clearCustomContext();

            if (newly == GenericAssetRef::null()) {
                s_errorMsg = "Import failed with the current parameters.";
                if (!keepOldOnFailure) {
                    // keep the old preview so user still sees something
                    m_pendingImportedAsset = old;
                }
                return false;
            }

            // swap preview and delete the old one
            m_pendingImportedAsset = newly;
            if (old != GenericAssetRef::null()) AssetCatalog::getInstance().deleteAsset(old);
            return true;
        };

    #ifdef NX_GRAPHICS_API_OPENGL
        auto previewImTextureId = [&]() -> ImTextureID {
            if (m_pendingImportedAsset == GenericAssetRef::null()) return 0;
            auto texRef = m_pendingImportedAsset.as<Texture>();
            auto tex    = texRef.lock();
            if (!tex || !tex->getData() || !tex->getData()->texture) return 0;
            return tex->getData()->texture->getId();
        };
    #else
        auto previewImTextureId = [&]() -> ImTextureID { return nullptr; };
    #endif

        // -------- UI: two panels --------
        const float leftWidth = 380.0f;

        ImGui::BeginChild("##texImp_left", ImVec2(leftWidth, 0), true);
        {
            ImGui::TextUnformatted("Texture Import Settings");
            ImGui::Separator();

            ImGui::Checkbox("Flip vertically", &s_params.flipVertically);
            ImGui::Checkbox("Convert to sRGB", &s_params.convertToSRGB);
            ImGui::Checkbox("Generate mipmaps", &s_params.generateMipmaps);

            // Minification filter (your “simple knob”)
            {
                static const char* kFilters[] = { "Nearest", "Bilinear", "Trilinear" };
                int idx = static_cast<int>(s_params.minFilter);
                if (ImGui::Combo("Min filter", &idx, kFilters, IM_ARRAYSIZE(kFilters))) {
                    s_params.minFilter = static_cast<TextureImportParameters::MinFilter>(idx);
                }
            }

            // Target format (importer may ignore some until you wire compression)
            {
                static const char* kFmts[] = { "Preserve", "RGB", "RGBA", "BC1", "BC3", "BC7" };
                int fidx = static_cast<int>(s_params.format);
                if (ImGui::Combo("Target format", &fidx, kFmts, IM_ARRAYSIZE(kFmts))) {
                    s_params.format = static_cast<TextureImportParameters::Format>(fidx);
                }
            }

            ImGui::SliderInt("Max dimension", &s_params.maxSize, 64, 8192);
            ImGui::SliderFloat("Compression quality", &s_params.compressionQuality, 0.0f, 1.0f);

            if (!s_errorMsg.empty()) {
                ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.35f, 0.35f, 1.0f));
                ImGui::TextWrapped("%s", s_errorMsg.c_str());
                ImGui::PopStyleColor();
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("##texImp_right", ImVec2(0, 0), true);
        {
            ImGui::TextUnformatted("Preview");
            ImGui::Separator();

            if (auto id = previewImTextureId()) {
                // Original texture size
                auto texPtr = m_pendingImportedAsset.as<assets::Texture>().lock();
                const float texW = (float)texPtr->getData()->texture->getWidth();
                const float texH = (float)texPtr->getData()->texture->getHeight();

                // How much space we have in the right panel
                ImVec2 avail = ImGui::GetContentRegionAvail();

                // Compute fitted size keeping aspect
                ImVec2 size = avail;
                if (texW > 0.f && texH > 0.f) {
                    const float aspect = texW / texH;
                    size.x = avail.x;
                    size.y = size.x / aspect;
                    if (size.y > avail.y) {            // too tall, fit by height instead
                        size.y = avail.y;
                        size.x = size.y * aspect;
                    }
                }

                // (optional) prevent upscaling: uncomment if you don’t want bigger than native
                size.x = std::min(size.x, texW);
                size.y = std::min(size.y, texH);

                // Center it in the available region
                const ImVec2 cursor = ImGui::GetCursorPos();
                const ImVec2 offset = ImVec2((avail.x - size.x) * 0.5f, (avail.y - size.y) * 0.5f);
                ImGui::SetCursorPos({cursor.x + offset.x, cursor.y + offset.y});

                ImGui::Image(id, size, ImVec2(0,1), ImVec2(1,0));
            } else {
                ImGui::TextDisabled("No preview available.");
            }
        }
        ImGui::EndChild();

        // -------- bottom actions --------
        ImGui::Separator();
        if (ImGui::Button("Reload")) {
            reimportWithParams(/*keepOldOnFailure=*/true);
        }
        ImGui::SameLine();
        if (ImGui::Button("Save")) {
            // Ensure stored asset matches current params, then close.
            if (reimportWithParams(/*keepOldOnFailure=*/false)) {
                s_inited = false; s_errorMsg.clear();
                PopupManager::closePopup();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            // delete preview & close
            deletePreviewIfAny();
            s_inited = false; s_errorMsg.clear();
            PopupManager::closePopup();
        }

        PopupManager::endPopup();
    }


} // namespace nexo::editor
