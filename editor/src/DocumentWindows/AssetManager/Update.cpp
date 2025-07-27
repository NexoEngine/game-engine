//// Update.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the update function of the asset manager window
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include "assets/AssetCatalog.hpp"

namespace nexo::editor {

    void AssetManagerWindow::updateFolderChildren()
    {
        m_folderChildren.clear();

        for (const auto& [path, name] : m_folderStructure) {
            if (!path.empty()) { // Skip root entry
                m_folderChildren[path] = {};
            }
        }

        // Build parent-child relationships
        for (const auto& [path, name] : m_folderStructure) {
            if (path.empty()) continue; // Skip root

            const size_t lastSlash = path.find_last_of('/');

            if (lastSlash == std::string::npos) {
                // Top-level folder - child of root
                m_folderChildren[""].push_back(path);
            } else {
                // Nested folder - child of parent
                const std::string parentPath = path.substr(0, lastSlash);
                m_folderChildren[parentPath].push_back(path);
            }
        }

        // Sort children for consistent display order
        for (auto& [parent, children] : m_folderChildren) {
            std::ranges::sort(children);
        }
    }

    void AssetManagerWindow::buildFolderStructure()
    {
        m_folderStructure.clear();
        // Root entry
        m_folderStructure.emplace_back("", "Assets");
        m_folderChildren.clear(); // Clear the folder children map

        // First pass: build the folder structure
        std::set<std::string, std::less<>> uniqueFolderPaths;

        std::unordered_set<std::string> seen{""};

        for (const auto assets = assets::AssetCatalog::getInstance().getAssets(); auto& ref : assets) {
            if (const auto assetData = ref.lock()) {
                // normalized path: e.g. "Random/Sub"
                std::filesystem::path p{ assetData->getMetadata().location.getPath() };
                std::filesystem::path curr;
                for (auto const& part : p) {
                    // skip empty or “_internal” style parts
                    if (auto s = part.string(); s.empty() || s.front() == '_')
                        continue;
                    curr /= part;
                    if (auto folderPath = curr.string(); seen.emplace(folderPath).second) {
                        m_folderStructure.emplace_back(
                            folderPath,
                            curr.filename().string()
                        );
                    }
                }
            }
        }

        std::sort(
            m_folderStructure.begin() + 1,
            m_folderStructure.end(),
            [](auto const& a, auto const& b){
                return a.first < b.first;
            }
        );
    }

    void AssetManagerWindow::update()
    {
        handleDroppedFiles();
        // Nothing to do for now
    }

}
