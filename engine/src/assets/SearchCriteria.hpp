//// SearchCriteria.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date:        09/25/2025
//  Description: Search criteria structure for asset filtering
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <set>
#include "Asset.hpp"

namespace nexo::assets {

    enum class SearchMode {
        NAME_ONLY,      // Search only in asset names
        TAGS_ONLY,      // Search only in tags
        DESCRIPTION,    // Search in descriptions
        ALL            // Search in all text fields
    };

    struct SearchCriteria {
        // Text search
        std::string searchText;
        SearchMode searchMode = SearchMode::ALL;
        bool caseSensitive = false;
        bool useRegex = false;

        // Type filtering
        std::set<AssetType> allowedTypes;  // Empty means all types

        // Date range filtering
        std::optional<std::chrono::system_clock::time_point> createdAfter;
        std::optional<std::chrono::system_clock::time_point> createdBefore;
        std::optional<std::chrono::system_clock::time_point> modifiedAfter;
        std::optional<std::chrono::system_clock::time_point> modifiedBefore;

        // Size filtering (in bytes)
        std::optional<size_t> minSize;
        std::optional<size_t> maxSize;

        // Tag filtering
        std::vector<std::string> requiredTags;    // Must have all these tags
        std::vector<std::string> excludedTags;    // Must not have any of these

        // Path filtering
        std::string pathFilter;  // Only show assets in this path (and subpaths if includeSubfolders is true)
        bool includeSubfolders = true;

        // Quick filters
        bool onlyLoaded = false;      // Only show loaded assets
        bool onlyUnloaded = false;    // Only show unloaded assets
        bool onlyErrored = false;     // Only show errored assets

        /**
         * @brief Clear all search criteria
         */
        void clear() {
            searchText.clear();
            searchMode = SearchMode::ALL;
            caseSensitive = false;
            useRegex = false;
            allowedTypes.clear();
            createdAfter.reset();
            createdBefore.reset();
            modifiedAfter.reset();
            modifiedBefore.reset();
            minSize.reset();
            maxSize.reset();
            requiredTags.clear();
            excludedTags.clear();
            pathFilter.clear();
            includeSubfolders = true;
            onlyLoaded = false;
            onlyUnloaded = false;
            onlyErrored = false;
        }

        /**
         * @brief Check if any criteria is set
         */
        [[nodiscard]] bool isEmpty() const {
            return searchText.empty() &&
                   allowedTypes.empty() &&
                   !createdAfter.has_value() &&
                   !createdBefore.has_value() &&
                   !modifiedAfter.has_value() &&
                   !modifiedBefore.has_value() &&
                   !minSize.has_value() &&
                   !maxSize.has_value() &&
                   requiredTags.empty() &&
                   excludedTags.empty() &&
                   pathFilter.empty() &&
                   !onlyLoaded &&
                   !onlyUnloaded &&
                   !onlyErrored;
        }
    };

} // namespace nexo::assets