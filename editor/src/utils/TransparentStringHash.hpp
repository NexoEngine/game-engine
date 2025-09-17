//// TransparentStringHash.hpp ////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/03/2025
//  Description: Header file containing the transparent string hash used for maps
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

namespace nexo::editor {
    /**
     * @struct TransparentStringHash
     * @brief A custom hash functor that supports heterogeneous lookup for string types.
     *
     * This struct provides hash functions for std::string, std::string_view, and C-style strings (const char*).
     * It enables the use of these different string types as keys in hash-based containers like std::unordered_map
     * without requiring conversions to a single type, thus improving performance and usability.
     *
     * The presence of the `is_transparent` type alias indicates to standard library containers that this hasher
     * supports heterogeneous lookup, allowing for more flexible key types.
     */
    struct TransparentStringHash {
        // This alias tells the container that our hasher supports heterogeneous lookup.
        using is_transparent = void;

        /**
         * @brief Computes the hash value for a given std::string.
         *
         * This function leverages the standard library's std::hash to generate a hash value
         * from the provided std::string, facilitating its use as a key in hash-based containers.
         *
         * @param s The input string to hash.
         * @return size_t The computed hash value.
         */
        size_t operator()(const std::string& s) const noexcept
        {
            return std::hash<std::string>{}(s);
        }

        /**
         * @brief Computes the hash value for a std::string_view.
         *
         * Uses the standard library's std::hash implementation for std::string_view to compute
         * the hash of the provided string view.
         *
         * @param s The string view whose hash is to be computed.
         * @return The computed hash value.
         */
        size_t operator()(std::string_view s) const noexcept
        {
            return std::hash<std::string_view>{}(s);
        }

        /**
         * @brief Computes the hash value for a C-style null-terminated string.
         *
         * This function converts the provided C-string into a std::string_view and then computes its hash
         * using the standard library's hash for string views, supporting heterogeneous lookup.
         *
         * @param s A null-terminated C-string to be hashed.
         * @return size_t The computed hash value for the input string.
         */
        size_t operator()(const char* s) const noexcept
        {
            if (!s) return 0;
            return std::hash<std::string_view>{}(s);
        }
    };
} // namespace nexo::editor
