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
            const auto [start, end] = std::minmax(latestSelected, index);
            const auto range = std::views::iota(start, end + 1);
            m_selectedAssets.insert(range.begin(), range.end());
            return;
        }

        m_selectedAssets.clear();
        m_selectedAssets.insert(index);
    }
}
