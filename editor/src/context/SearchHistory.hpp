//// SearchHistory.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date:        09/25/2025
//  Description: Search history management for asset manager
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <deque>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include "assets/SearchCriteria.hpp"
#include "json.hpp"

namespace nexo::editor {

    struct SearchHistoryEntry {
        std::string searchText;
        assets::SearchCriteria criteria;
        std::chrono::system_clock::time_point timestamp;
        size_t useCount = 1;  // Track frequency of use

        SearchHistoryEntry() = default;

        SearchHistoryEntry(const std::string& text, const assets::SearchCriteria& crit)
            : searchText(text), criteria(crit), timestamp(std::chrono::system_clock::now()), useCount(1) {}
    };

    class SearchHistory {
    public:
        static constexpr size_t MAX_HISTORY_SIZE = 50;
        static constexpr size_t MAX_RECENT_SEARCHES = 20;
        static constexpr size_t MAX_SUGGESTIONS = 10;

        SearchHistory() {
            loadHistory();
        }

        ~SearchHistory() {
            saveHistory();
        }

        /**
         * @brief Add a search to history
         * @param searchText The search text
         * @param criteria The full search criteria used
         */
        void addSearch(const std::string& searchText, const assets::SearchCriteria& criteria) {
            if (searchText.empty()) return;

            // Check if search already exists
            auto it = std::find_if(m_history.begin(), m_history.end(),
                [&searchText](const SearchHistoryEntry& entry) {
                    return entry.searchText == searchText;
                });

            if (it != m_history.end()) {
                // Update existing entry
                it->useCount++;
                it->timestamp = std::chrono::system_clock::now();
                it->criteria = criteria;
            } else {
                // Add new entry
                m_history.emplace_front(searchText, criteria);
                if (m_history.size() > MAX_HISTORY_SIZE) {
                    m_history.pop_back();
                }
            }

            saveHistory();
        }

        /**
         * @brief Get recent searches
         * @param maxCount Maximum number of recent searches to return
         * @return Vector of recent search strings
         */
        [[nodiscard]] std::vector<std::string> getRecentSearches(size_t maxCount = MAX_RECENT_SEARCHES) const {
            std::vector<SearchHistoryEntry> sorted = sortByRecency();

            std::vector<std::string> recent;
            recent.reserve(std::min(maxCount, sorted.size()));

            for (size_t i = 0; i < std::min(maxCount, sorted.size()); ++i) {
                recent.push_back(sorted[i].searchText);
            }

            return recent;
        }

        /**
         * @brief Get popular searches (most frequently used)
         * @param maxCount Maximum number of popular searches to return
         * @return Vector of popular search strings
         */
        [[nodiscard]] std::vector<std::string> getPopularSearches(size_t maxCount = 10) const {
            std::vector<SearchHistoryEntry> sorted(m_history.begin(), m_history.end());
            std::sort(sorted.begin(), sorted.end(),
                [](const SearchHistoryEntry& a, const SearchHistoryEntry& b) {
                    return a.useCount > b.useCount;
                });

            std::vector<std::string> popular;
            popular.reserve(std::min(maxCount, sorted.size()));

            for (size_t i = 0; i < std::min(maxCount, sorted.size()); ++i) {
                popular.push_back(sorted[i].searchText);
            }

            return popular;
        }

        /**
         * @brief Get search suggestions based on partial text
         * @param partialText The partial text to get suggestions for
         * @param maxSuggestions Maximum number of suggestions to return
         * @return Vector of suggested search terms from history
         */
        [[nodiscard]] std::vector<std::string> getSuggestions(const std::string& partialText,
                                                              size_t maxSuggestions = MAX_SUGGESTIONS) const {
            if (partialText.empty()) {
                return getRecentSearches(maxSuggestions);
            }

            std::vector<std::string> suggestions;
            suggestions.reserve(maxSuggestions);

            std::string lowerPartial = toLower(partialText);
            std::vector<SearchHistoryEntry> sorted = sortByRecency();

            for (const auto& entry : sorted) {
                if (suggestions.size() >= maxSuggestions) break;

                if (toLower(entry.searchText).find(lowerPartial) == 0) {
                    suggestions.push_back(entry.searchText);
                }
            }

            return suggestions;
        }

        /**
         * @brief Clear all search history
         */
        void clearHistory() {
            m_history.clear();
            saveHistory();
        }

        /**
         * @brief Remove a specific search from history
         * @param searchText The search text to remove
         */
        void removeSearch(const std::string& searchText) {
            m_history.erase(
                std::remove_if(m_history.begin(), m_history.end(),
                    [&searchText](const SearchHistoryEntry& entry) {
                        return entry.searchText == searchText;
                    }),
                m_history.end()
            );
            saveHistory();
        }

        /**
         * @brief Get full criteria for a previous search
         * @param searchText The search text to get criteria for
         * @return Optional containing the criteria if found
         */
        [[nodiscard]] std::optional<assets::SearchCriteria> getCriteriaForSearch(const std::string& searchText) const {
            auto it = std::find_if(m_history.begin(), m_history.end(),
                [&searchText](const SearchHistoryEntry& entry) {
                    return entry.searchText == searchText;
                });

            if (it != m_history.end()) {
                return it->criteria;
            }
            return std::nullopt;
        }

    private:
        std::deque<SearchHistoryEntry> m_history;
        static constexpr const char* HISTORY_FILE = ".nexo/search_history.json";

        [[nodiscard]] std::vector<SearchHistoryEntry> sortByRecency() const {
            std::vector<SearchHistoryEntry> sorted(m_history.begin(), m_history.end());
            std::sort(sorted.begin(), sorted.end(),
                [](const SearchHistoryEntry& a, const SearchHistoryEntry& b) {
                    return a.timestamp > b.timestamp;
                });
            return sorted;
        }

        void loadHistory() {
            std::filesystem::path historyPath = getHistoryFilePath();
            if (!std::filesystem::exists(historyPath)) {
                return;
            }

            try {
                std::ifstream file(historyPath);
                if (!file.is_open()) return;

                nlohmann::json j;
                file >> j;

                if (j.contains("history") && j["history"].is_array()) {
                    for (const auto& item : j["history"]) {
                        SearchHistoryEntry entry;
                        entry.searchText = item.value("searchText", "");
                        entry.useCount = item.value("useCount", 1);

                        if (item.contains("timestamp")) {
                            auto time_t = item["timestamp"].get<std::time_t>();
                            entry.timestamp = std::chrono::system_clock::from_time_t(time_t);
                        }

                        // Load criteria if available
                        if (item.contains("criteria")) {
                            const auto& crit = item["criteria"];
                            entry.criteria.searchText = crit.value("searchText", "");
                            entry.criteria.caseSensitive = crit.value("caseSensitive", false);
                            entry.criteria.useRegex = crit.value("useRegex", false);
                            // ... load other criteria fields as needed
                        }

                        m_history.push_back(entry);
                        if (m_history.size() >= MAX_HISTORY_SIZE) break;
                    }
                }
            } catch (const std::exception&) {
                // Ignore errors loading history
            }
        }

        void saveHistory() const {
            try {
                std::filesystem::path historyPath = getHistoryFilePath();
                std::filesystem::create_directories(historyPath.parent_path());

                nlohmann::json j;
                nlohmann::json historyArray = nlohmann::json::array();

                for (const auto& entry : m_history) {
                    nlohmann::json item;
                    item["searchText"] = entry.searchText;
                    item["useCount"] = entry.useCount;
                    item["timestamp"] = std::chrono::system_clock::to_time_t(entry.timestamp);

                    // Save basic criteria
                    nlohmann::json criteria;
                    criteria["searchText"] = entry.criteria.searchText;
                    criteria["caseSensitive"] = entry.criteria.caseSensitive;
                    criteria["useRegex"] = entry.criteria.useRegex;
                    item["criteria"] = criteria;

                    historyArray.push_back(item);
                }

                j["history"] = historyArray;

                std::ofstream file(historyPath);
                if (file.is_open()) {
                    file << j.dump(2);
                }
            } catch (const std::exception&) {
                // Ignore errors saving history
            }
        }

        [[nodiscard]] static std::filesystem::path getHistoryFilePath() {
            return std::filesystem::current_path() / HISTORY_FILE;
        }

        [[nodiscard]] static std::string toLower(const std::string& str) {
            std::string lower = str;
            std::transform(lower.begin(), lower.end(), lower.begin(),
                [](unsigned char c) { return std::tolower(c); });
            return lower;
        }
    };

} // namespace nexo::editor