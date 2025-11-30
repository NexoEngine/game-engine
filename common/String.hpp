//// String.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        23/11/2024
//  Description: Utils for strings
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string_view>
#include <boost/core/demangle.hpp>

#include <string>
#include <utility> // std::index_sequence

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

    template<typename T>
    [[nodiscard]] constexpr std::string type_name()
    {
        std::string nameString = boost::core::demangle(typeid(T).name());
        std::string_view name{nameString};

        // Remove any leading compiler-specific prefixes
        #if defined(_MSC_VER)
        // MSVC: Remove leading "class " or "struct "
        if (name.starts_with("class ")) {
            name.remove_prefix(strlen("class "));
        } else if (name.starts_with("struct ")) {
            name.remove_prefix(strlen("struct "));
        }
        #endif

        // skip space
        size_t endPos = name.find_first_of(' ');
        if (endPos != std::string_view::npos) {
            name = name.substr(0, endPos);
        }
        return std::string{name};
    }
} // namespace nexo
