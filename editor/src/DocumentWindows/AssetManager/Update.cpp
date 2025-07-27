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

    void AssetManagerWindow::update()
    {
        handleDroppedFiles();
        // Nothing to do for now
    }

}
