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

    /**
     * @brief AssetLocation is a wrapper of std::string to represent the location of an asset
     *
     * It is used to apply our own rules on naming
     */
    class AssetLocation {
        public:
            explicit AssetLocation(const std::string& fullLocation)
            {
                setLocation(fullLocation);
            }

            AssetLocation& setName(const AssetName& name)
            {
                _name = name;
                return *this;
            }

            AssetLocation& setPath(const std::string& path)
            {
                _path = path;
                return *this;
            }

            AssetLocation& setPackName(const std::optional<std::reference_wrapper<const AssetPackName>>& packName)
            {
                _packName = packName;
                return *this;
            }

            /**
             * @brief Get the asset's name
             * @return The asset's AssetName
             */
            [[nodiscard]] const AssetName& getName() const { return _name; }

            /**
             * @brief Get the asset's pack name
             * @return The asset's AssetPackName
             */
            [[nodiscard]] std::optional<std::reference_wrapper<const AssetPackName>> getPackName() const { return _packName; }

            /**
             * @brief Get the asset's path
             * @return The asset's path
             */
            [[nodiscard]] const std::string& getPath() const { return _path; }

            /**
             * @brief Get the asset's full location
             * @return The asset's full location as string (e.g.: packName::name@path/to/asset)
             */
            [[nodiscard]] std::string getFullLocation() const
            {
                std::string fullLocation;
                if (_packName)
                    fullLocation += _packName->data() + "::";
                fullLocation += _name.data();
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

            bool operator==(const AssetLocation& asset_location) const
            {
                return _name == asset_location._name && _packName == asset_location._packName && _path == asset_location._path;
            }

            bool operator==(const std::string& fullLocation) const
            {
                std::string extractedPackName;
                std::string extractedAssetName;
                std::string extractedPath;

                parseFullLocation(fullLocation, extractedAssetName, extractedPath, extractedPackName);

                return _name == AssetName(extractedAssetName) && _packName == AssetPackName(extractedPackName) && _path == extractedPath;
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
                // TODO: maybe trim spaces?
                extractedAssetName = fullLocation;
            }

        private:
            AssetName _name{"Unnamed"};            //< The name of the asset
            std::optional<AssetPackName> _packName; //< The package containing the asset
            std::string _path;                      //< The path to the asset
    };


} // namespace nexo::assets
