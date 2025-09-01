//// FileDrop.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date:        30/06/2025
//  Description: Implementation of file drop handling for asset manager
//
///////////////////////////////////////////////////////////////////////////////

#include <filesystem>
#include "AssetManagerWindow.hpp"
#include "Logger.hpp"
#include "assets/AssetCatalog.hpp"
#include "assets/AssetImporter.hpp"
#include "assets/AssetLocation.hpp"
#include "context/ThumbnailCache.hpp"

namespace nexo::editor {

    void AssetManagerWindow::handleAssetDrop(const std::string& path)
    {
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG")) {
                const auto data = static_cast<const AssetDragDropPayload*>(payload->Data);
                assets::AssetCatalog::getInstance().moveAsset(data->id, path);
                m_selectedAssets.clear();
            }
            ImGui::EndDragDropTarget();
        }
    }

    void AssetManagerWindow::handleFolderDrop(const std::string& folderPath, const std::string &folderName)
    {
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FOLDER_DRAG")) {
                const auto data        = static_cast<const FolderDragDropPayload*>(payload->Data);
                m_folderManager.moveFolder(data->path, folderPath);
                m_selectedFolders.clear();
            }
            ImGui::EndDragDropTarget();
        }
    }

    void AssetManagerWindow::handleAssetDrag(const assets::GenericAssetRef& asset)
    {
        const auto assetData = asset.lock();

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            AssetDragDropPayload payload;
            payload.type         = assetData->getType();
            payload.id           = assetData->getID();
            const auto& fullPath = assetData->getMetadata().location.getFullLocation();
            const auto& name     = assetData->getMetadata().location.getName().data();
            std::memcpy(payload.path, fullPath.c_str(), std::min(fullPath.size(), sizeof(payload.path) - 1));
            payload.path[sizeof(payload.path) - 1] = '\0';
            std::memcpy(payload.name, name.c_str(), std::min(name.size(), sizeof(payload.name) - 1));
            payload.name[sizeof(payload.name) - 1] = '\0';

            ImGui::SetDragDropPayload("ASSET_DRAG", &payload, sizeof(payload));
            ImTextureID textureID = ThumbnailCache::getInstance().getThumbnail(asset);
            if (textureID) ImGui::Image(textureID, {64, 64}, ImVec2(0, 1), ImVec2(1, 0));

            ImGui::EndDragDropSource();
        }
    }

    void AssetManagerWindow::handleFolderDrag(const std::string& folderPath, const std::string& folderName)
    {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            FolderDragDropPayload payload;
            std::memcpy(payload.path, folderPath.c_str(), std::min(folderPath.size(), sizeof(payload.path) - 1));
            payload.path[sizeof(payload.path) - 1] = '\0';
            std::memcpy(payload.name, folderName.c_str(), std::min(folderName.size(), sizeof(payload.name) - 1));
            payload.name[sizeof(payload.name) - 1] = '\0';

            ImGui::SetDragDropPayload("FOLDER_DRAG", &payload, sizeof(payload));
            // ImTextureID textureID = ThumbnailCache::getInstance().getThumbnail(asset);
            // if (textureID) ImGui::Image(textureID, {64, 64}, ImVec2(0, 1), ImVec2(1, 0));
            const ImTextureID folderIconTexture = getIconTexture(m_folderIcon);
            if (folderIconTexture) {
                ImGui::Image(folderIconTexture, {64, 64}, ImVec2(0, 1), ImVec2(0, 1));                           // White tint for default color
            }
            ImGui::EndDragDropSource();
        }
    }

    assets::AssetLocation AssetManagerWindow::getAssetLocation(const std::filesystem::path& path) const
    {
        const std::string assetName = path.stem().string();
        std::filesystem::path folderPath;
        const std::string targetFolder = !m_hoveredFolder.empty() ? m_hoveredFolder : m_currentFolder;

        std::string locationString = assetName + "@" + targetFolder;

        LOG(NEXO_DEV, "Creating asset location: {} (current folder: '{}', hovered: '{}')", locationString,
            m_currentFolder, m_hoveredFolder);

        assets::AssetLocation location(locationString);
        return location;
    }

    void AssetManagerWindow::handleEvent(event::EventFileDrop& event)
    {
        m_pendingDroppedFiles.insert(m_pendingDroppedFiles.end(), event.files.begin(), event.files.end());
    }

    void AssetManagerWindow::handleDroppedFiles()
    {
        if (m_pendingDroppedFiles.empty()) return;

        for (const auto& filePath : m_pendingDroppedFiles) importDroppedFile(filePath);
        m_pendingDroppedFiles.clear();
    }

    void AssetManagerWindow::importDroppedFile(const std::string& filePath) const
    {
        const std::filesystem::path path(filePath);

        if (!exists(path)) {
            LOG(NEXO_WARN, "Dropped file does not exist: {}", filePath);
            return;
        }

        const assets::AssetLocation location = getAssetLocation(path);

        assets::ImporterFileInput fileInput{path};
        try {
            assets::AssetImporter importer;
            if (const auto assetRef = importer.importAssetAuto(location, fileInput); !assetRef)
                LOG(NEXO_ERROR, "Failed to import asset: {}", location.getPath().data());
        } catch (const std::exception& e) {
            LOG(NEXO_ERROR, "Exception while importing {}: {}", location.getPath().data(), e.what());
        }
    }
} // namespace nexo::editor
