//// FileDrop.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date:        30/06/2025
//  Description: Implementation of file drop handling for asset manager
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include "assets/AssetImporter.hpp"
#include "assets/AssetLocation.hpp"
#include "Logger.hpp"
#include <filesystem>
#include <algorithm>

namespace nexo::editor {

    assets::AssetLocation AssetManagerWindow::getAssetLocation(const std::filesystem::path &path) const
    {
        const std::string assetName = path.stem().string();
        std::filesystem::path folderPath;
        const std::string targetFolder = !m_hoveredFolder.empty() ? m_hoveredFolder : m_currentFolder;

        std::string locationString = assetName + "@" + targetFolder;

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

    void AssetManagerWindow::importDroppedFile(const std::string& filePath) const
    {
        const std::filesystem::path path(filePath);

        if (!std::filesystem::exists(path)) {
            LOG(NEXO_WARN, "Dropped file does not exist: {}", filePath);
            return;
        }

        std::string extension = path.extension().string();
        std::ranges::transform(extension, extension.begin(), ::tolower);

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
}
