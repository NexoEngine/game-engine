//// FilenameValidator.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        13/12/2024
//  Description: FilenameValidator is a struct that provides a static method to
//               validate a filename (most OS compatible).
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <regex>
#include <optional>
#include <string_view>

namespace nexo::assets {
    struct FilenameValidator {
        static inline const auto ValidationRegex = std::regex("^[a-zA-Z0-9._-]*$");
        static constexpr auto MaxLength = 255;
        static constexpr auto ForbiddenKeywords = {
            "CON", "PRN", "AUX", "NUL",
            "COM1", "COM2", "COM3", "COM4",
            "COM5", "COM6", "COM7", "COM8",
            "COM9", "LPT1", "LPT2", "LPT3",
            "LPT4", "LPT5", "LPT6", "LPT7",
            "LPT8", "LPT9"
        };

        /**
         * @brief Validates a filename against predefined criteria.
         *
         * The function checks that the filename is not empty, does not exceed 255 characters,
         * contains only allowed characters (alphanumeric, '.', '_', and '-'), and is not a reserved keyword.
         * It returns an error message describing the first encountered issue if the filename is invalid;
         * otherwise, it returns std::nullopt.
         *
         * @param name The filename to be validated.
         * @return std::optional<std::string> An error message if the filename is invalid, or std::nullopt if valid.
         */
        [[nodiscard]] static std::optional<std::string> validate(std::string_view name)
        {
            if (name.empty())
                return "Cannot be empty.";
            if (name.size() > MaxLength)
                return "Cannot exceed 255 characters.";
            if (!std::regex_match(name.data(), ValidationRegex))
                return "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.";
            for (const auto& keyword : ForbiddenKeywords) {
                if (name == keyword)
                    return "Cannot be a reserved keyword.";
            }
            return std::nullopt; // Valid name
        }
    };
}
