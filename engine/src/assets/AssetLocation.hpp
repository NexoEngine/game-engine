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
        /**
         * @brief Constructs an exception indicating an invalid asset location.
         *
         * This constructor builds an InvalidAssetLocation exception by formatting an error
         * message that includes the provided asset location and an explanatory message.
         * It optionally takes a source location to mark where the exception is generated.
         *
         * @param assetLocation The invalid asset location string.
         * @param message Additional context describing why the asset location is invalid.
         * @param loc The source location at which the exception is thrown (defaults to the current call site).
         */
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
            /**
             * @brief Constructs an AssetLocation by parsing the provided full location string.
             *
             * This explicit constructor initializes an AssetLocation object using the given fullLocation string,
             * which should include the asset name, path, and optionally the asset pack name. The parsing is
             * performed by calling setLocation(fullLocation), and an InvalidAssetLocation exception is thrown
             * if the asset name is invalid.
             *
             * @param fullLocation The complete asset location string to be parsed.
             */
            explicit AssetLocation(const std::string& fullLocation)
            {
                setLocation(fullLocation);
            }

            /**
             * @brief Sets the asset's name.
             *
             * Updates the asset's name with the provided value and returns a reference
             * to the current object to facilitate method chaining.
             *
             * @param name The new name for the asset.
             * @return Reference to the modified AssetLocation instance.
             */
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
                _path = path;
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
 * @brief Retrieves the asset's pack name.
 *
 * Returns an optional reference wrapper to a constant AssetPackName. If the asset is not associated
 * with any pack, this function returns an empty optional.
 *
 * @return An optional reference to the asset's pack name.
 */
            [[nodiscard]] std::optional<std::reference_wrapper<const AssetPackName>> getPackName() const { return _packName; }

            /**
             * @brief Get the asset's path
             * @return The asset's path
             */
            [[nodiscard]] const std::string& getPath() const { return _path; }

            /**
             * @brief Constructs and returns the full asset location.
             *
             * The full location is formatted as "packName::name@path", where the "packName::" prefix is included only if a pack name is set and the "@path" suffix is appended only when the path is non-empty.
             *
             * @return A string representing the asset's complete location.
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
             * @brief Parses a full asset location string into its constituent components.
             *
             * This static function divides the provided location string into an asset name, an asset path, and an optional package name based on the "::" and "@" delimiters. If "::" is present, the substring before it is assigned as the package name and removed from the string. If "@" is found in the remaining string, the substring after it is used as the asset path, and the preceding part is considered the asset name. If a delimiter is absent, the corresponding output string is cleared.
             *
             * @param fullLocation The full asset location string to parse.
             * @param extractedAssetName Output parameter for the parsed asset name.
             * @param extractedPath Output parameter for the parsed asset path.
             * @param extractedPackName Output parameter for the parsed package name.
             *
             * @warning No validation is performed on the extracted components.
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
