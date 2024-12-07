//// AssetName.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        07/12/2024
//  Description: AssetName is a wrapper of std::string to represent the name
//               of an asset. It is used to apply our own rules on naming.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <format>
#include <string>
#include <string_view>

#include "Exception.hpp"

namespace nexo::assets {

    class InvalidAssetName final : public Exception {
        public:
        explicit InvalidAssetName(
            std::string_view assetName,
            std::string_view message,
            const std::source_location loc = std::source_location::current()
        ) : Exception(std::format("Invalid asset name '{}': {}", assetName, message), loc) {};
    };

    class AssetName {
        public:
            constexpr static auto InvalidCharList = "@/\\";

            // Explicit(false) constructor to allow implicit conversion from std::string
            explicit(false) AssetName(const std::string_view name)
            {
                checkName(name);
                _assetNameValue = name;
            }
            explicit(false) AssetName(const std::string& name) : AssetName(std::string_view(name)) {}
            explicit(false) AssetName(const char* name) : AssetName(std::string_view(name)) {}

            // Get the underlying name as a string
            [[nodiscard]] const std::string& getName() const { return _assetNameValue; }

            // Implicit conversion to std::string for convenience
            explicit(false) operator std::string() const { return _assetNameValue; } // NOLINT(google-explicit-constructor)
            explicit(false) operator std::string_view() const { return _assetNameValue; } // NOLINT(google-explicit-constructor)
            explicit(false) operator const char*() const { return _assetNameValue.c_str(); } // NOLINT(google-explicit-constructor)

            // Equality comparison
            bool operator==(const AssetName& Other) const { return _assetNameValue == Other._assetNameValue; }
            bool operator!=(const AssetName& Other) const { return !(*this == Other); }

            /**
             * @brief Check if a name is valid
             * @param name The name to check
             * @throws InvalidAssetName if the name is invalid
             */
            static void checkName(std::string_view name);

            /**
             * @brief Check if a name is valid
             * @param name The name to check
             * @note This function is used to check if a name is valid without throwing an exception
             */
            static bool isValid(std::string_view name);

        private:
            std::string _assetNameValue;
    };

    inline void test()
    {
        const auto a = "a@";
        AssetName test = a;
    }
} // namespace nexo::assets
