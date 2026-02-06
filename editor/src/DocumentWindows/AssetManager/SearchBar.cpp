//// SearchBar.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date:        09/25/2025
//  Description: Search bar implementation for asset manager
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include "IconsFontAwesome.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <array>

namespace nexo::editor {

    void AssetManagerWindow::drawSearchBar() {
        ImGui::PushItemWidth(-1);

        // Search input with icon
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 6));

        // Search icon
        ImGui::Text(ICON_FA_SEARCH " ");
        ImGui::SameLine();

        // Input field
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 100);

        constexpr size_t MAX_SEARCH_LENGTH = 256;
        m_searchBuffer.resize(MAX_SEARCH_LENGTH);

        ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_CallbackEdit |
                                         ImGuiInputTextFlags_CallbackHistory |
                                         ImGuiInputTextFlags_EnterReturnsTrue;

        if (ImGui::InputTextWithHint("##SearchInput", "Search assets...",
                                     m_searchBuffer.data(), m_searchBuffer.capacity(),
                                     inputFlags,
                                     [](ImGuiInputTextCallbackData* data) {
                                         auto* window = static_cast<AssetManagerWindow*>(data->UserData);

                                         if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
                                             window->m_searchBuffer = std::string(data->Buf, data->BufTextLen);
                                             window->handleSearchInput();
                                             return 0;
                                         }

                                         if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory && data->EventKey == ImGuiKey_UpArrow) {
                                             auto history = window->m_searchHistory.getRecentSearches(1);
                                             if (!history.empty()) {
                                                 data->DeleteChars(0, data->BufTextLen);
                                                 data->InsertChars(0, history[0].c_str());
                                             }
                                         }

                                         return 0;
                                     }, this)) {
            applySearch();
        }

        m_searchBuffer.resize(strlen(m_searchBuffer.c_str()));

        // Clear button
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TIMES)) {
            clearSearch();
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Clear search (Esc)");
        }

        // Advanced search toggle
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_FILTER)) {
            m_showAdvancedSearch = !m_showAdvancedSearch;
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Advanced search (Ctrl+Shift+F)");
        }

        ImGui::PopStyleVar();

        // Show search result count
        if (!m_searchBuffer.empty()) {
            auto filtered = getFilteredAssets();
            ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f),
                             "%zu results found", filtered.size());
        }

        // Draw search suggestions dropdown
        if (m_showSearchSuggestions) {
            drawSearchSuggestions();
        }

        // Draw advanced search panel if enabled
        if (m_showAdvancedSearch) {
            drawAdvancedSearchPanel();
        }

        ImGui::PopItemWidth();
        ImGui::Separator();
    }

    void AssetManagerWindow::drawAdvancedSearchPanel() {
        if (!ImGui::CollapsingHeader("Advanced Search", ImGuiTreeNodeFlags_DefaultOpen)) {
            return;
        }

        ImGui::Indent();

        // Search mode
        ImGui::Text("Search in:");
        ImGui::SameLine();
        std::array<const char*, 4> searchModes = { "Name Only", "Tags Only", "Description", "All Fields" };
        if (auto currentMode = static_cast<int>(m_searchCriteria.searchMode);
            ImGui::Combo("##SearchMode", &currentMode, searchModes.data(), searchModes.size())) {
            m_searchCriteria.searchMode = static_cast<assets::SearchMode>(currentMode);
            updateSearchFilter();
        }

        // Asset type filter
        ImGui::Text("Asset Types:");
        ImGui::Indent();
        for (int i = 1; i < static_cast<int>(assets::AssetType::_COUNT); ++i) {
            auto type = static_cast<assets::AssetType>(i);
            if (bool isSelected = m_searchCriteria.allowedTypes.contains(type);
                ImGui::Checkbox(assets::getAssetTypeName(type), &isSelected)) {
                if (isSelected) {
                    m_searchCriteria.allowedTypes.insert(type);
                } else {
                    m_searchCriteria.allowedTypes.erase(type);
                }
                updateSearchFilter();
            }
            if ((i - 1) % 3 < 2 && i < static_cast<int>(assets::AssetType::_COUNT) - 1) {
                ImGui::SameLine();
            }
        }
        ImGui::Unindent();

        // File size filter
        ImGui::Separator();
        ImGui::Text("File Size:");

        float minSize = static_cast<float>(m_searchCriteria.minSize.value_or(0)) / 1024.0f;
        float maxSize = static_cast<float>(m_searchCriteria.maxSize.value_or(0)) / 1024.0f;

        ImGui::PushItemWidth(100);
        if (ImGui::DragFloat("Min (KB)##MinSize", &minSize, 1.0f, 0.0f, FLT_MAX, "%.1f")) {
            m_searchCriteria.minSize = static_cast<size_t>(minSize * 1024);
            updateSearchFilter();
        }
        ImGui::SameLine();
        if (ImGui::DragFloat("Max (KB)##MaxSize", &maxSize, 1.0f, 0.0f, FLT_MAX, "%.1f")) {
            m_searchCriteria.maxSize = static_cast<size_t>(maxSize * 1024);
            updateSearchFilter();
        }
        ImGui::PopItemWidth();

        drawStatusFilters();

        // Options
        ImGui::Separator();
        ImGui::Text("Options:");

        if (ImGui::Checkbox("Case Sensitive", &m_searchCriteria.caseSensitive)) {
            updateSearchFilter();
        }
        ImGui::SameLine();
        if (ImGui::Checkbox("Use Regex", &m_searchCriteria.useRegex)) {
            updateSearchFilter();
        }

        // Clear filters button
        ImGui::Separator();
        if (ImGui::Button("Clear All Filters")) {
            clearSearch();
        }
        ImGui::SameLine();
        if (ImGui::Button("Apply")) {
            applySearch();
        }

        ImGui::Unindent();
        ImGui::Separator();
    }

    void AssetManagerWindow::drawSearchSuggestions() {
        // Get suggestions from history and current assets
        auto historySuggestions = m_searchHistory.getSuggestions(m_searchBuffer, 5);
        auto assetSuggestions = m_searchFilter.getSuggestions(
            m_searchBuffer,
            assets::AssetCatalog::getInstance().getAssets(),
            5
        );

        // Combine suggestions
        m_searchSuggestions.clear();
        m_searchSuggestions.insert(m_searchSuggestions.end(),
                                   historySuggestions.begin(),
                                   historySuggestions.end());
        m_searchSuggestions.insert(m_searchSuggestions.end(),
                                   assetSuggestions.begin(),
                                   assetSuggestions.end());

        if (m_searchSuggestions.empty()) {
            m_showSearchSuggestions = false;
            return;
        }

        // Draw dropdown
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetItemRectSize().x, 0));

        if (ImGui::Begin("##SearchSuggestions", nullptr,
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                        ImGuiWindowFlags_Tooltip)) {

            for (size_t i = 0; i < m_searchSuggestions.size(); ++i) {
                bool isSelected = (static_cast<int>(i) == m_selectedSuggestion);

                if (ImGui::Selectable(m_searchSuggestions[i].c_str(), isSelected)) {
                    m_searchBuffer = m_searchSuggestions[i];
                    applySearch();
                    m_showSearchSuggestions = false;
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
        }
        ImGui::End();
    }

    void AssetManagerWindow::updateSearchFilter() {
        // Properly set the search text from the buffer (trim null characters)
        std::string searchText = m_searchBuffer;
        // Trim to actual string length (remove padding nulls)
        searchText = searchText.c_str();

        m_searchCriteria.searchText = searchText;
        m_searchCriteria.pathFilter = m_currentFolder;
        m_searchFilter.setCriteria(m_searchCriteria);
    }

    void AssetManagerWindow::applySearch() {
        updateSearchFilter();

        // Add to search history if not empty
        if (!m_searchBuffer.empty()) {
            m_searchHistory.addSearch(m_searchBuffer, m_searchCriteria);
        }

        m_showSearchSuggestions = false;
    }

    void AssetManagerWindow::clearSearch() {
        m_searchBuffer.clear();
        m_searchCriteria.clear();
        m_searchFilter.setCriteria(m_searchCriteria);
        m_showSearchSuggestions = false;
        m_selectedSuggestion = -1;
    }

    void AssetManagerWindow::handleSearchInput() {
        // Debounce search input
        auto now = std::chrono::steady_clock::now();
        auto timeSinceLastSearch = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - m_lastSearchTime).count();

        if (timeSinceLastSearch >= SEARCH_DEBOUNCE_MS) {
            updateSearchFilter();
            m_lastSearchTime = now;

            // Show suggestions if text is not empty
            m_showSearchSuggestions = !m_searchBuffer.empty();
        }
    }

    void AssetManagerWindow::handleSearchKeyboardShortcuts() {
        // Ctrl+F: Focus search
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_F)) {
            ImGui::SetKeyboardFocusHere(-1);
        }

        // Escape: Clear search
        if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
            clearSearch();
        }

        // Ctrl+Shift+F: Toggle advanced search
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) &&
            ImGui::IsKeyDown(ImGuiKey_LeftShift) &&
            ImGui::IsKeyPressed(ImGuiKey_F)) {
            m_showAdvancedSearch = !m_showAdvancedSearch;
        }

        // Handle suggestion navigation
        if (m_showSearchSuggestions) {
            if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
                m_selectedSuggestion++;
                if (m_selectedSuggestion >= static_cast<int>(m_searchSuggestions.size())) {
                    m_selectedSuggestion = 0;
                }
            } else if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
                m_selectedSuggestion--;
                if (m_selectedSuggestion < 0) {
                    m_selectedSuggestion = static_cast<int>(m_searchSuggestions.size()) - 1;
                }
            } else if (ImGui::IsKeyPressed(ImGuiKey_Enter) && m_selectedSuggestion >= 0) {
                m_searchBuffer = m_searchSuggestions[m_selectedSuggestion];
                applySearch();
            }
        }
    }

    void AssetManagerWindow::drawStatusFilters() {
        ImGui::Separator();
        ImGui::Text("Asset Status:");

        if (ImGui::Checkbox("Loaded Only", &m_searchCriteria.onlyLoaded)) {
            if (m_searchCriteria.onlyLoaded) {
                m_searchCriteria.onlyUnloaded = false;
                m_searchCriteria.onlyErrored = false;
            }
            updateSearchFilter();
        }
        ImGui::SameLine();
        if (ImGui::Checkbox("Unloaded Only", &m_searchCriteria.onlyUnloaded)) {
            if (m_searchCriteria.onlyUnloaded) {
                m_searchCriteria.onlyLoaded = false;
                m_searchCriteria.onlyErrored = false;
            }
            updateSearchFilter();
        }
        ImGui::SameLine();
        if (ImGui::Checkbox("Errors Only", &m_searchCriteria.onlyErrored)) {
            if (m_searchCriteria.onlyErrored) {
                m_searchCriteria.onlyLoaded = false;
                m_searchCriteria.onlyUnloaded = false;
            }
            updateSearchFilter();
        }
    }

    std::vector<assets::GenericAssetRef> AssetManagerWindow::getFilteredAssets() const {
        std::vector<assets::GenericAssetRef> allAssets = assets::AssetCatalog::getInstance().getAssets();

        // First filter by current folder
        std::vector<assets::GenericAssetRef> folderFiltered;
        for (const auto& asset : allAssets) {
            auto assetPtr = asset.lock();
            if (!assetPtr) continue;

            const auto& path = assetPtr->getMetadata().location.getPath();
            if (path == INTERNAL_FOLDER_PREFIX) continue;

            if (m_currentFolder.empty() || path == m_currentFolder ||
                (m_searchCriteria.includeSubfolders && path.find(m_currentFolder) == 0)) {
                folderFiltered.push_back(asset);
            }
        }

        // Apply search filter
        return m_searchFilter.filter(folderFiltered);
    }

} // namespace nexo::editor