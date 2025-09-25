//// AssetSearchFilter.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date:        09/25/2025
//  Description: Asset search filter for advanced asset searching
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <regex>
#include <algorithm>
#include <cctype>
#include "SearchCriteria.hpp"
#include "Asset.hpp"
#include "AssetRef.hpp"

namespace nexo::assets {

    class AssetSearchFilter {
    public:
        AssetSearchFilter() = default;
        ~AssetSearchFilter() = default;

        /**
         * @brief Set the search criteria
         */
        void setCriteria(const SearchCriteria& criteria) {
            m_criteria = criteria;
            updateSearchPattern();
        }

        /**
         * @brief Get the current search criteria
         */
        [[nodiscard]] const SearchCriteria& getCriteria() const {
            return m_criteria;
        }

        /**
         * @brief Check if an asset matches the current search criteria
         * @param asset The asset to check
         * @return true if the asset matches all criteria
         */
        [[nodiscard]] bool matches(const GenericAssetRef& asset) const {
            auto assetPtr = asset.lock();
            if (!assetPtr) return false;

            const auto& metadata = assetPtr->getMetadata();

            // Check status filters
            if (!matchesStatusFilter(metadata.status)) {
                return false;
            }

            // Check type filter
            if (!matchesTypeFilter(metadata.type)) {
                return false;
            }

            // Check path filter
            if (!matchesPathFilter(metadata.location.getPath())) {
                return false;
            }

            // Check date filters
            if (!matchesDateFilters(metadata)) {
                return false;
            }

            // Check size filter
            if (!matchesSizeFilter(metadata.fileSize)) {
                return false;
            }

            // Check tag filters
            if (!matchesTagFilters(metadata.tags)) {
                return false;
            }

            // Check text search
            if (!matchesTextSearch(metadata)) {
                return false;
            }

            return true;
        }

        /**
         * @brief Filter a list of assets based on current criteria
         * @param assets The list of assets to filter
         * @return Vector of assets that match the criteria
         */
        [[nodiscard]] std::vector<GenericAssetRef> filter(const std::vector<GenericAssetRef>& assets) const {
            if (m_criteria.isEmpty()) {
                return assets;
            }

            std::vector<GenericAssetRef> filtered;
            filtered.reserve(assets.size());

            for (const auto& asset : assets) {
                if (matches(asset)) {
                    filtered.push_back(asset);
                }
            }

            return filtered;
        }

        /**
         * @brief Get suggestions based on partial search text
         * @param partialText The partial text to get suggestions for
         * @param assets The assets to search through
         * @param maxSuggestions Maximum number of suggestions to return
         * @return Vector of suggested search terms
         */
        [[nodiscard]] std::vector<std::string> getSuggestions(
            const std::string& partialText,
            const std::vector<GenericAssetRef>& assets,
            size_t maxSuggestions = 10) const {

            std::vector<std::string> suggestions;
            if (partialText.empty()) return suggestions;

            std::string lowerPartial = toLower(partialText);
            std::set<std::string> uniqueSuggestions;

            for (const auto& asset : assets) {
                if (suggestions.size() >= maxSuggestions) break;

                auto assetPtr = asset.lock();
                if (!assetPtr) continue;

                const auto& metadata = assetPtr->getMetadata();
                const std::string assetName = metadata.location.getName().data();

                // Check asset name
                if (toLower(assetName).find(lowerPartial) != std::string::npos) {
                    uniqueSuggestions.insert(assetName);
                }

                // Check tags
                for (const auto& tag : metadata.tags) {
                    if (toLower(tag).find(lowerPartial) != std::string::npos) {
                        uniqueSuggestions.insert(tag);
                    }
                }
            }

            suggestions.assign(uniqueSuggestions.begin(), uniqueSuggestions.end());
            if (suggestions.size() > maxSuggestions) {
                suggestions.resize(maxSuggestions);
            }

            return suggestions;
        }

    private:
        SearchCriteria m_criteria;
        mutable std::optional<std::regex> m_searchRegex;

        void updateSearchPattern() {
            if (m_criteria.useRegex && !m_criteria.searchText.empty()) {
                try {
                    auto flags = std::regex::ECMAScript;
                    if (!m_criteria.caseSensitive) {
                        flags |= std::regex::icase;
                    }
                    m_searchRegex = std::regex(m_criteria.searchText, flags);
                } catch (const std::regex_error&) {
                    m_searchRegex.reset();
                }
            } else {
                m_searchRegex.reset();
            }
        }

        [[nodiscard]] bool matchesStatusFilter(AssetStatus status) const {
            if (m_criteria.onlyLoaded && status != AssetStatus::LOADED) return false;
            if (m_criteria.onlyUnloaded && status != AssetStatus::UNLOADED) return false;
            if (m_criteria.onlyErrored && status != AssetStatus::ERROR) return false;
            return true;
        }

        [[nodiscard]] bool matchesTypeFilter(AssetType type) const {
            if (m_criteria.allowedTypes.empty()) return true;
            return m_criteria.allowedTypes.contains(type);
        }

        [[nodiscard]] bool matchesPathFilter(const std::string& path) const {
            if (m_criteria.pathFilter.empty()) return true;

            if (m_criteria.includeSubfolders) {
                return path.find(m_criteria.pathFilter) == 0;
            } else {
                return path == m_criteria.pathFilter;
            }
        }

        [[nodiscard]] bool matchesDateFilters(const AssetMetadata& metadata) const {
            if (m_criteria.createdAfter && metadata.creationTime < *m_criteria.createdAfter) return false;
            if (m_criteria.createdBefore && metadata.creationTime > *m_criteria.createdBefore) return false;
            if (m_criteria.modifiedAfter && metadata.modificationTime < *m_criteria.modifiedAfter) return false;
            if (m_criteria.modifiedBefore && metadata.modificationTime > *m_criteria.modifiedBefore) return false;
            return true;
        }

        [[nodiscard]] bool matchesSizeFilter(size_t size) const {
            if (m_criteria.minSize && size < *m_criteria.minSize) return false;
            if (m_criteria.maxSize && size > *m_criteria.maxSize) return false;
            return true;
        }

        [[nodiscard]] bool matchesTagFilters(const std::vector<std::string>& tags) const {
            // Check required tags
            for (const auto& requiredTag : m_criteria.requiredTags) {
                if (std::find(tags.begin(), tags.end(), requiredTag) == tags.end()) {
                    return false;
                }
            }

            // Check excluded tags
            for (const auto& excludedTag : m_criteria.excludedTags) {
                if (std::find(tags.begin(), tags.end(), excludedTag) != tags.end()) {
                    return false;
                }
            }

            return true;
        }

        [[nodiscard]] bool matchesTextSearch(const AssetMetadata& metadata) const {
            if (m_criteria.searchText.empty()) return true;

            const std::string assetName = metadata.location.getName().data();

            switch (m_criteria.searchMode) {
                case SearchMode::NAME_ONLY:
                    return matchesText(assetName);

                case SearchMode::TAGS_ONLY:
                    for (const auto& tag : metadata.tags) {
                        if (matchesText(tag)) return true;
                    }
                    return false;

                case SearchMode::DESCRIPTION:
                    return matchesText(metadata.description);

                case SearchMode::ALL:
                default:
                    if (matchesText(assetName)) return true;
                    if (matchesText(metadata.description)) return true;
                    for (const auto& tag : metadata.tags) {
                        if (matchesText(tag)) return true;
                    }
                    return false;
            }
        }

        [[nodiscard]] bool matchesText(const std::string& text) const {
            if (m_searchRegex) {
                return std::regex_search(text, *m_searchRegex);
            }

            if (m_criteria.caseSensitive) {
                return text.find(m_criteria.searchText) != std::string::npos;
            } else {
                return toLower(text).find(toLower(m_criteria.searchText)) != std::string::npos;
            }
        }

        [[nodiscard]] static std::string toLower(const std::string& str) {
            std::string lower = str;
            std::transform(lower.begin(), lower.end(), lower.begin(),
                [](unsigned char c) { return std::tolower(c); });
            return lower;
        }
    };

} // namespace nexo::assets