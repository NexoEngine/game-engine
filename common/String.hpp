//// String.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        23/11/2024
//  Description: Utils for strings
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string_view>
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


} // namespace nexo
