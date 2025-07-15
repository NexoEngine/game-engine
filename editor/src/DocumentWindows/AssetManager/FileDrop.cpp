//// FileDrop.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date:        2025-06-30
//  Description: Implementation of file drop handling for asset manager
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include "assets/Asset.hpp"
#include "assets/AssetImporter.hpp"
#include "assets/AssetLocation.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include "Logger.hpp"
#include <filesystem>
#include <algorithm>

namespace nexo::editor {

    static assets::AssetType getAssetTypeFromExtension(const std::string &extension)
    {
        static const std::set<std::string> imageExtensions = {
            ".png", ".jpg", ".jpeg", ".bmp", ".tga", ".gif", ".psd", ".hdr", ".pic", ".pnm", ".ppm", ".pgm"
        };
        if (imageExtensions.contains(extension))
            return assets::AssetType::TEXTURE;
        static const std::set<std::string> modelExtensions = {
            ".gltf", ".glb", ".fbx", ".obj", ".dae", ".3ds", ".stl", ".ply", ".blend", ".x3d", ".ifc"
        };
        if (modelExtensions.contains(extension))
            return assets::AssetType::MODEL;
        return assets::AssetType::UNKNOWN;
    }

    const assets::AssetLocation AssetManagerWindow::getAssetLocation(const std::filesystem::path &path) const
    {
        std::string assetName = path.stem().string();
        std::filesystem::path folderPath;
        if (!m_currentFolder.empty())
            folderPath /= m_currentFolder;
        if (!m_hoveredFolder.empty())
            folderPath /= m_hoveredFolder;

        std::string assetPath = folderPath.string();
        std::string locationString = assetName + "@" + assetPath;

        LOG(NEXO_DEV,
            "Creating asset location: {} (current folder: '{}', hovered: '{}')",
            locationString,
            m_currentFolder,
            m_hoveredFolder);

        assets::AssetLocation location(locationString);
        return location;
    }

    void AssetManagerWindow::handleEvent(event::EventFileDrop& event)
    {
        m_pendingDroppedFiles.insert(m_pendingDroppedFiles.end(),
                                    event.files.begin(),
                                    event.files.end());
    }

    void AssetManagerWindow::handleDroppedFiles()
    {
        if (m_pendingDroppedFiles.empty())
            return;

        for (const auto& filePath : m_pendingDroppedFiles)
            importDroppedFile(filePath);
        m_pendingDroppedFiles.clear();

        m_folderStructure.clear();
        buildFolderStructure();
    }

    void AssetManagerWindow::importDroppedFile(const std::string& filePath)
    {
        std::filesystem::path path(filePath);

        if (!std::filesystem::exists(path)) {
            LOG(NEXO_WARN, "Dropped file does not exist: {}", filePath);
            return;
        }

        std::string extension = path.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        assets::AssetType assetType = getAssetTypeFromExtension(extension);
        if (assetType == assets::AssetType::UNKNOWN) {
            LOG(NEXO_WARN, "Unsupported file type: {}", extension);
            return;
        }

        assets::AssetLocation location = getAssetLocation(path);

        assets::AssetImporter importer;
        assets::ImporterFileInput fileInput{path};
        try {
            if (assetType == assets::AssetType::TEXTURE) {
                auto assetRef = importer.importAsset<assets::Texture>(location, fileInput);
                if (assetRef)
                    LOG(NEXO_INFO, "Successfully imported texture: {}", location.getName());
                else
                    LOG(NEXO_ERROR, "Failed to import texture: {}", location.getPath());
            } else if (assetType == assets::AssetType::MODEL) {
                auto assetRef = importer.importAsset<assets::Model>(location, fileInput);
                if (assetRef)
                    LOG(NEXO_INFO, "Successfully imported model: {}", location.getName());
                else
                    LOG(NEXO_ERROR, "Failed to import model: {}", location.getPath());
            }
        } catch (const std::exception& e) {
            LOG(NEXO_ERROR, "Exception while importing {}: {}", location.getPath(), e.what());
        }
    }
}
