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
        std::string targetFolder = !m_hoveredFolder.empty() ? m_hoveredFolder : m_currentFolder;

        std::string assetPath = targetFolder;
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
            auto assetRef = importer.importAssetAuto(location, fileInput);
            if (!assetRef)
                LOG(NEXO_ERROR, "Failed to import asset: {}", location.getPath().data());
        } catch (const std::exception& e) {
            LOG(NEXO_ERROR, "Exception while importing {}: {}", location.getPath().data(), e.what());
        }
    }
}
