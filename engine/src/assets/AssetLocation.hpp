//// AssetLocation.hpp ////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
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
#include "Path.hpp"

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

            /**
             * @brief Sets the asset path.
             *
             * Assigns the given path string to the asset location.
             *
             * @param path The new asset path.
             * @return A reference to this AssetLocation instance for chaining.
             */
            AssetLocation& setPath(const std::string& path)
            {
                _path = normalizePathAndRemovePrefixSlash(path);
                return *this;
            }

            /**
             * @brief Sets the asset's pack name.
             *
             * Assigns the provided pack name to the current asset location and returns a reference to
             * the modified object, allowing for method chaining.
             *
             * @param packName The pack name to associate with the asset.
             * @return AssetLocation& Reference to the updated AssetLocation.
             */
            AssetLocation& setPackName(const AssetPackName& packName)
            {
                _packName = packName;
                return *this;
            }

            /**
             * @brief Clears the pack name associated with the asset.
             *
             * Resets the pack name, effectively marking the asset as not belonging to any pack.
             * Returns a reference to the current instance to facilitate method chaining.
             *
             * @return AssetLocation& A reference to the current AssetLocation instance.
             */
            AssetLocation& clearPackName()
            {
                _packName.reset();
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
                if (!_path.empty()) {
                    fullLocation += "@";
                    fullLocation += _path;
                }
                return fullLocation;
            }

            void setLocation(
                const AssetName& name,
                const std::string& path,
                const std::optional<std::reference_wrapper<const AssetPackName>>& packName = std::nullopt
            );

            /**
             * @brief Parses and sets the asset's location from a full location string.
             *
             * Extracts the asset name, path, and optional pack name from the provided string and updates
             * the corresponding internal members. The expected format is "packName::name@path" or "name@path" if
             * no pack name is included. If the extracted asset name is invalid, an InvalidAssetLocation
             * exception is thrown.
             *
             * @param fullLocation Full asset location string.
             *
             * @throws InvalidAssetLocation if the asset name in the provided string is invalid.
             */
            void setLocation(const std::string& fullLocation)
            {
                std::string extractedPackName;
                std::string extractedAssetName;
                std::string extractedPath;

                parseFullLocation(fullLocation, extractedAssetName, extractedPath, extractedPackName);
                extractedPath = normalizePathAndRemovePrefixSlash(extractedPath);

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
             * @brief Compares two AssetLocation objects for equality.
             *
             * Determines if this AssetLocation has the same name, pack name, and path as the given object.
             *
             * @param assetLocation The asset location to compare with.
             * @return true if both asset locations contain equivalent values; false otherwise.
             */
            bool operator==(const AssetLocation& assetLocation) const
            {
                return _name == assetLocation._name && _packName == assetLocation._packName && _path == assetLocation._path;
            }

            /**
             * @brief Compares the current asset location with a full location string.
             *
             * Parses the provided full location string into its asset name, path, and pack name components and
             * compares them with the object's corresponding values. Returns true if all components match, indicating
             * that the asset location is equivalent to the provided string.
             *
             * @param fullLocation The full asset location string, expected to follow the format "packName::name@path".
             * @return true if the extracted asset name, pack name, and path match those of this asset location; false otherwise.
             */
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
                if (const auto packNameEndPos = fullLocation.find("::"); packNameEndPos != std::string::npos) {
                    extractedPackName = fullLocation.substr(0, packNameEndPos);
                    fullLocation.remove_prefix(packNameEndPos + 2);
                } else {
                    extractedPackName.clear();
                }
                if (const auto pathStartPos = fullLocation.find('@'); pathStartPos != std::string::npos) {
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
