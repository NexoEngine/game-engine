//// AssetLocation.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        07/12/2024
//  Description: AssetLocation is a wrapper of std::string to represent the
//               location of an asset. It is used to apply our own rules on
//               naming.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <optional>
#include <string>
#include <filesystem>

#include "AssetName.hpp"
#include "AssetPackName.hpp"

namespace nexo::assets {

    class InvalidAssetLocation final : public Exception {
        public:
        explicit InvalidAssetLocation(
            std::string_view assetLocation,
            std::string_view message,
            const std::source_location loc = std::source_location::current()
        ) : Exception(std::format("Invalid asset location '{}': {}", assetLocation, message), loc) {};
    };

    class AssetLocation {
        public:
            explicit AssetLocation(const std::string& fullLocation)
            {
                setLocation(fullLocation);
            }

            [[nodiscard]] std::optional<std::reference_wrapper<const AssetPackName>> getPackName() const { return _packName; }
            [[nodiscard]] const AssetName& getAssetName() const { return _name; }
            [[nodiscard]] const std::string& getPath() const { return _path; }

            [[nodiscard]] std::string getFullLocation() const
            {
                std::string fullLocation;
                if (_packName)
                    fullLocation += _packName->getName() + "::";
                fullLocation += _name.getName();
                if (!_path.empty())
                    fullLocation += "@" + _path;
                return fullLocation;
            }

            void setLocation(
                const AssetName& name,
                const std::string& path,
                const std::optional<std::reference_wrapper<const AssetPackName>>& packName = std::nullopt
            );

            void setLocation(const std::string& fullLocation)
            {
                std::string extractedPackName;
                std::string extractedAssetName;
                std::string extractedPath;

                parseFullLocation(fullLocation, extractedAssetName, extractedPath, extractedPackName);

                try {
                    _name = AssetName(extractedAssetName);
                    if (!extractedPackName.empty())
                        _packName = AssetPackName(extractedPackName);
                } catch (const InvalidName& e) {
                    THROW_EXCEPTION(InvalidAssetLocation, fullLocation, e.getMessage());
                }
                _path = extractedPath;
            }

            /**
            * @brief Parse a full asset location string into its components
            * @param fullLocation The full location string to parse
            * @param extractedAssetName The extracted asset name
            * @param extractedPath The extracted path
            * @param extractedPackName The extracted package name
            * @note This function is static and can be used to parse a full location string into its components
            * @warning Does not validate the extracted names
            */
            static void parseFullLocation(
                std::string_view fullLocation,
                std::string& extractedAssetName,
                std::string& extractedPath,
                std::string& extractedPackName
            )
            {
                if (auto packNameEndPos = fullLocation.find("::"); packNameEndPos != std::string::npos) {
                    extractedPackName = fullLocation.substr(0, packNameEndPos);
                    fullLocation.remove_prefix(packNameEndPos + 2);
                } else {
                    extractedPackName.clear();
                }
                if (auto pathStartPos = fullLocation.find('@'); pathStartPos != std::string::npos) {
                    extractedPath = fullLocation.substr(pathStartPos + 1);
                    fullLocation.remove_suffix(fullLocation.size() - pathStartPos);
                } else {
                    extractedPath.clear();
                }
                extractedAssetName = fullLocation;
            }

        private:
            AssetName _name{"Unnamed"};            //< The name of the asset
            std::optional<AssetPackName> _packName; //< The package containing the asset
            std::string _path;                      //< The path to the asset
    };


} // namespace nexo::assets
