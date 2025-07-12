//// FileDrop.cpp ///////////////////////////////////////////////////////////////
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
#include "assets/AssetImporter.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include "Logger.hpp"
#include <filesystem>
#include <algorithm>

namespace nexo::editor {

    void AssetManagerWindow::handleEvent(event::EventFileDrop& event)
    {
        // Queue dropped files for processing in the next frame
        m_pendingDroppedFiles.insert(m_pendingDroppedFiles.end(),
                                    event.files.begin(),
                                    event.files.end());
    }

    void AssetManagerWindow::handleDroppedFiles()
    {
        if (m_pendingDroppedFiles.empty())
            return;

        // Process each dropped file
        for (const auto& filePath : m_pendingDroppedFiles)
        {
            importDroppedFile(filePath);
        }

        m_pendingDroppedFiles.clear();

        // Rebuild folder structure to include new assets
        m_folderStructure.clear();
        buildFolderStructure();
    }

    void AssetManagerWindow::importDroppedFile(const std::string& filePath)
    {
        std::filesystem::path path(filePath);

        if (!std::filesystem::exists(path))
        {
            LOG(NEXO_WARN, "Dropped file does not exist: {}", filePath);
            return;
        }

        // Get file extension
        std::string extension = path.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

        // Determine asset type based on extension
        assets::AssetType assetType = assets::AssetType::UNKNOWN;

        // Image extensions
        static const std::vector<std::string> imageExtensions = {
            ".png", ".jpg", ".jpeg", ".bmp", ".tga", ".gif", ".psd", ".hdr", ".pic", ".pnm", ".ppm", ".pgm"
        };

        // Model extensions (common ones supported by Assimp)
        static const std::vector<std::string> modelExtensions = {
            ".gltf", ".glb", ".fbx", ".obj", ".dae", ".3ds", ".stl", ".ply", ".blend", ".x3d", ".ifc"
        };

        if (std::find(imageExtensions.begin(), imageExtensions.end(), extension) != imageExtensions.end())
        {
            assetType = assets::AssetType::TEXTURE;
        }
        else if (std::find(modelExtensions.begin(), modelExtensions.end(), extension) != modelExtensions.end())
        {
            assetType = assets::AssetType::MODEL;
        }
        else
        {
            LOG(NEXO_WARN, "Unsupported file type: {}", extension);
            return;
        }

        // Generate asset location
        std::string filename = path.filename().string();
        std::string assetName = path.stem().string();

        // Create location based on current folder
        // The path after @ should just be the folder path, not include the asset name
        std::string assetPath = m_currentFolder.empty() ? "" : m_currentFolder;
        assetPath += m_hoveredFolder.empty() ? "" : "/" + m_hoveredFolder;
        std::string locationString = assetName + "@" + assetPath;

        LOG(NEXO_DEV, "Creating asset location: {} (current folder: '{}')", locationString, m_currentFolder);
        assets::AssetLocation location(locationString);

        // Import the asset
        assets::AssetImporter importer;
        assets::ImporterFileInput fileInput{path};

        try
        {
            if (assetType == assets::AssetType::TEXTURE)
            {
                auto assetRef = importer.importAsset<assets::Texture>(location, fileInput);
                if (assetRef)
                {
                    LOG(NEXO_INFO, "Successfully imported texture: {}", filename);
                }
                else
                {
                    LOG(NEXO_ERROR, "Failed to import texture: {}", filename);
                }
            }
            else if (assetType == assets::AssetType::MODEL)
            {
                auto assetRef = importer.importAsset<assets::Model>(location, fileInput);
                if (assetRef)
                {
                    LOG(NEXO_INFO, "Successfully imported model: {}", filename);
                }
                else
                {
                    LOG(NEXO_ERROR, "Failed to import model: {}", filename);
                }
            }
        }
        catch (const std::exception& e)
        {
            LOG(NEXO_ERROR, "Exception while importing {}: {}", filename, e.what());
        }
    }

} // namespace nexo::editor
