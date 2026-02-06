//// String.hpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Guillaume HEIN
//  Date:        23/11/2024
//  Description: Utils for strings
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string_view>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>

namespace nexo {
    /**
     * @brief Compare two strings case-insensitively.
     *
     * @param a The first string.
     * @param b The second string.
     * @return true if the strings are equal (case-insensitive), false otherwise.
     */
    [[nodiscard]] constexpr bool iequals(const std::string_view& a, const std::string_view& b) {
        return a.size() == b.size() &&
            std::equal(a.begin(), a.end(), b.begin(), [](const char _a, const char _b) {
                return std::tolower(static_cast<unsigned char>(_a)) ==
                       std::tolower(static_cast<unsigned char>(_b));
            });
    }

    /**
     * @brief Convert a string to lowercase.
     *
     * @param str The string to convert.
     * @return A new string with all characters in lowercase.
     */
    [[nodiscard]] inline std::string toLower(std::string_view str) {
        std::string result;
        result.reserve(str.size());
        std::ranges::transform(str, std::back_inserter(result), [](const char c) {
            return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        });
        return result;
    }

    /**
     * @brief Convert a string to uppercase.
     *
     * @param str The string to convert.
     * @return A new string with all characters in uppercase.
     */
    [[nodiscard]] inline std::string toUpper(std::string_view str) {
        std::string result;
        result.reserve(str.size());
        std::ranges::transform(str, std::back_inserter(result), [](const char c) {
            return static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        });
        return result;
    }

    /**
     * @brief Check if a string starts with a given prefix.
     *
     * @param str The string to check.
     * @param prefix The prefix to search for.
     * @return true if str starts with prefix, false otherwise.
     */
    [[nodiscard]] constexpr bool startsWith(const std::string_view& str, const std::string_view& prefix) {
        return str.size() >= prefix.size() && str.substr(0, prefix.size()) == prefix;
    }

    /**
     * @brief Check if a string starts with a given prefix (case-insensitive).
     *
     * @param str The string to check.
     * @param prefix The prefix to search for.
     * @return true if str starts with prefix (case-insensitive), false otherwise.
     */
    [[nodiscard]] constexpr bool istartsWith(const std::string_view& str, const std::string_view& prefix) {
        if (str.size() < prefix.size()) return false;
        return iequals(str.substr(0, prefix.size()), prefix);
    }

    /**
     * @brief Check if a string ends with a given suffix.
     *
     * @param str The string to check.
     * @param suffix The suffix to search for.
     * @return true if str ends with suffix, false otherwise.
     */
    [[nodiscard]] constexpr bool endsWith(const std::string_view& str, const std::string_view& suffix) {
        return str.size() >= suffix.size() && str.substr(str.size() - suffix.size()) == suffix;
    }

    /**
     * @brief Check if a string ends with a given suffix (case-insensitive).
     *
     * @param str The string to check.
     * @param suffix The suffix to search for.
     * @return true if str ends with suffix (case-insensitive), false otherwise.
     */
    [[nodiscard]] constexpr bool iendsWith(const std::string_view& str, const std::string_view& suffix) {
        if (str.size() < suffix.size()) return false;
        return iequals(str.substr(str.size() - suffix.size()), suffix);
    }

    /**
     * @brief Trim whitespace from the left side of a string.
     *
     * @param str The string to trim.
     * @return A new string with leading whitespace removed.
     */
    [[nodiscard]] inline std::string ltrim(std::string_view str) {
        const auto start = std::ranges::find_if(str, [](const char c) {
            return !std::isspace(static_cast<unsigned char>(c));
        });
        return std::string(start, str.end());
    }

    /**
     * @brief Trim whitespace from the right side of a string.
     *
     * @param str The string to trim.
     * @return A new string with trailing whitespace removed.
     */
    [[nodiscard]] inline std::string rtrim(std::string_view str) {
        const auto end = std::ranges::find_if(str | std::views::reverse, [](const char c) {
            return !std::isspace(static_cast<unsigned char>(c));
        }).base();
        return std::string(str.begin(), end);
    }

    /**
     * @brief Trim whitespace from both sides of a string.
     *
     * @param str The string to trim.
     * @return A new string with leading and trailing whitespace removed.
     */
    [[nodiscard]] inline std::string trim(std::string_view str) {
        return ltrim(rtrim(str));
    }

    /**
     * @brief Check if a string contains a substring.
     *
     * @param str The string to search in.
     * @param substr The substring to search for.
     * @return true if str contains substr, false otherwise.
     */
    [[nodiscard]] constexpr bool contains(const std::string_view& str, const std::string_view& substr) {
        return str.find(substr) != std::string_view::npos;
    }

    /**
     * @brief Check if a string contains a substring (case-insensitive).
     *
     * @param str The string to search in.
     * @param substr The substring to search for.
     * @return true if str contains substr (case-insensitive), false otherwise.
     */
    [[nodiscard]] inline bool icontains(const std::string_view& str, const std::string_view& substr) {
        if (substr.empty()) return true;
        if (str.size() < substr.size()) return false;

        for (size_t i = 0; i <= str.size() - substr.size(); ++i) {
            if (iequals(str.substr(i, substr.size()), substr)) {
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Replace all occurrences of a substring with another string.
     *
     * @param str The original string.
     * @param from The substring to replace.
     * @param to The replacement string.
     * @return A new string with all occurrences replaced.
     */
    [[nodiscard]] inline std::string replaceAll(std::string_view str, std::string_view from, std::string_view to) {
        if (from.empty()) return std::string(str);

        std::string result;
        result.reserve(str.size());
        size_t start_pos = 0;
        size_t pos;

        while ((pos = str.find(from, start_pos)) != std::string_view::npos) {
            result.append(str.substr(start_pos, pos - start_pos));
            result.append(to);
            start_pos = pos + from.size();
        }
        result.append(str.substr(start_pos));
        return result;
    }

    /**
     * @brief Split a string by a delimiter.
     *
     * @param str The string to split.
     * @param delimiter The delimiter to split by.
     * @return A vector of substrings.
     */
    [[nodiscard]] inline std::vector<std::string> split(std::string_view str, std::string_view delimiter) {
        std::vector<std::string> result;
        if (delimiter.empty()) {
            result.emplace_back(str);
            return result;
        }

        size_t start = 0;
        size_t end;

        while ((end = str.find(delimiter, start)) != std::string_view::npos) {
            result.emplace_back(str.substr(start, end - start));
            start = end + delimiter.size();
        }
        result.emplace_back(str.substr(start));
        return result;
    }

    /**
     * @brief Join a collection of strings with a delimiter.
     *
     * @param strings The strings to join.
     * @param delimiter The delimiter to use.
     * @return A single string with all elements joined.
     */
    template<typename Container>
    [[nodiscard]] inline std::string join(const Container& strings, std::string_view delimiter) {
        if (strings.empty()) return "";

        std::string result;
        auto it = strings.begin();
        result.append(*it);
        ++it;

        for (; it != strings.end(); ++it) {
            result.append(delimiter);
            result.append(*it);
        }
        return result;
    }

} // namespace nexo
