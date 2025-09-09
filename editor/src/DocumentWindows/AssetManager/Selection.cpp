//// Selection.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        27/07/2025
//  Description: Source file for the selection handling in the asset manager
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"

namespace nexo::editor {
    void AssetManagerWindow::handleSelection(const unsigned int index, const bool isSelected)
    {
        if (ImGui::IsKeyDown(ImGuiKey_ModCtrl)) {
            if (isSelected)
                m_selectedAssets.erase(index);
            else
                m_selectedAssets.insert(index);
            return;
        }

        if (ImGui::IsKeyDown(ImGuiKey_ModShift) && !m_selectedAssets.empty()) {
            const unsigned int latestSelected = *m_selectedAssets.rbegin();
            const auto [start, end]           = std::minmax(latestSelected, index);
            const auto range                  = std::views::iota(start, end + 1);
            m_selectedAssets.insert(range.begin(), range.end());
            return;
        }

        m_selectedAssets.clear();
        m_selectedAssets.insert(index);
    }

    void AssetManagerWindow::handleSelection(const std::string& folderPath, const bool isSelected)
    {
        if (ImGui::IsKeyDown(ImGuiKey_ModCtrl)) {
            if (isSelected)
                m_selectedFolders.erase(folderPath);
            else
                m_selectedFolders.insert(folderPath);
            return;
        }

        if (ImGui::IsKeyDown(ImGuiKey_ModShift) && !m_selectedFolders.empty()) {
            std::vector<std::pair<std::string, std::string>> allFolder =
                m_folderManager.getChildren(m_folderActionState.parentPath); // Ensure the folder exists in the manager
            std::vector<std::string> folderPaths;
            for (const auto& path : allFolder | std::views::keys) {
                folderPaths.push_back(path);
            }
            std::ranges::sort(folderPaths);

            const std::string& latestSelected = *m_selectedFolders.rbegin();
            auto startIt                      = std::ranges::find(folderPaths, latestSelected);
            auto endIt                        = std::ranges::find(folderPaths, folderPath);
            // If start is after end, reverse the range
            if (startIt > endIt) {
                std::swap(startIt, endIt);
            }
            m_selectedFolders.clear();

            for (auto it = startIt; it != endIt + 1; ++it) {
                m_selectedFolders.insert(
                    folderPaths[it - folderPaths.begin()]); // Insert the folder path into the selection
            }

            return;
        }

        m_selectedFolders.clear();
        m_selectedFolders.insert(folderPath);
    }
} // namespace nexo::editor
