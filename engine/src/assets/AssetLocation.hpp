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
         * @brief Constructs an InvalidAssetLocation exception.
         *
         * Initializes an exception with a formatted error message that combines the invalid asset location
         * with a descriptive message. An optional source location parameter allows tracking the origin of the error,
         * defaulting to the current call site.
         *
         * @param assetLocation The asset location string that failed validation.
         * @param message A detail message describing the reason for the invalid asset location.
         * @param loc The source location where the exception is thrown (defaults to the call site).
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
             * @brief Constructs an AssetLocation object from a full location string.
             *
             * This constructor initializes the AssetLocation by parsing the provided full location string into its
             * component parts (asset name, path, and optional asset pack name) using the setLocation method.
             * The input must conform to the expected asset location format.
             *
             * @param fullLocation The full asset location string to parse.
             * @throws InvalidAssetLocation if the provided string does not represent a valid asset location.
             */
            explicit AssetLocation(const std::string& fullLocation)
            {
                setLocation(fullLocation);
            }

            /**
             * @brief Sets the asset name.
             *
             * Updates the asset's name with the provided value and returns a reference to the current object, enabling method chaining.
             *
             * @param name The asset name to assign.
             * @return A reference to this AssetLocation instance.
             */
            AssetLocation& setName(const AssetName& name)
            {
                _name = name;
                return *this;
            }

            /**
             * @brief Sets the asset path.
             *
             * Updates the asset location's path with the specified value and returns a reference
             * to the current instance to allow for method chaining.
             *
             * @param path The new path of the asset.
             * @return Reference to the current AssetLocation instance.
             */
            AssetLocation& setPath(const std::string& path)
            {
                _path = path;
                return *this;
            }

            /**
             * @brief Sets the optional asset pack name.
             *
             * Updates the asset pack name for the asset, allowing it to be associated with a specific pack.
             * If no pack name is provided, the asset will not be associated with any pack.
             *
             * @param packName Optional reference to a constant AssetPackName.
             * @return AssetLocation& Reference to the current instance for method chaining.
             */
            AssetLocation& setPackName(const std::optional<std::reference_wrapper<const AssetPackName>>& packName)
            {
                _packName = packName;
                return *this;
            }

            /**
 * @brief Retrieves the asset name.
 *
 * Returns a constant reference to the asset name associated with this AssetLocation.
 *
 * @return A constant reference to the AssetName.
 */
[[nodiscard]] const AssetName& getName() const { return _name; }
            /**
 * @brief Retrieves the asset's pack name, if available.
 *
 * This method returns an optional reference wrapper to the asset pack name. If no pack name is
 * associated with the asset, the returned optional will be empty.
 *
 * @return std::optional<std::reference_wrapper<const AssetPackName>> Optional reference to the asset pack name.
 */
[[nodiscard]] std::optional<std::reference_wrapper<const AssetPackName>> getPackName() { return _packName; }

            /**
 * @brief Retrieves the optional asset pack name.
 *
 * Returns an optional constant reference to the asset pack name if it is set; otherwise, returns an empty optional.
 *
 * @return std::optional<std::reference_wrapper<const AssetPackName>> containing the asset pack name if set.
 */
[[nodiscard]] std::optional<std::reference_wrapper<const AssetPackName>> getPackName() const { return _packName; }
            /**
 * @brief Retrieves the asset name.
 *
 * Returns a constant reference to the internal AssetName representing the asset's name.
 *
 * @return const AssetName& A reference to the asset name.
 */
[[nodiscard]] const AssetName& getAssetName() const { return _name; }
            /**
 * @brief Retrieves the asset's path.
 *
 * Returns the path component of the asset location.
 *
 * @return const std::string& A reference to the asset's path string.
 */
[[nodiscard]] const std::string& getPath() const { return _path; }

            /**
             * @brief Retrieves the formatted full asset location string.
             *
             * Constructs the full asset location by optionally prefixing the asset pack name (if available)
             * followed by "::", appending the asset name, and then appending the asset path (if non-empty)
             * prefixed by "@".
             *
             * @return std::string The full asset location string.
             */
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

            /**
             * @brief Initializes the asset's components by parsing a full location string.
             *
             * This method decomposes the provided full location string into its constituent parts:
             * asset name, path, and an optional pack name. It attempts to construct an AssetName from 
             * the extracted asset name and, if a pack name is present, initializes the asset pack name.
             * If the asset name is invalid, an InvalidAssetLocation exception is thrown.
             *
             * @param fullLocation The full string representing the asset location.
             *
             * @throws InvalidAssetLocation if the extracted asset name is invalid.
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
             * @brief Compares this AssetLocation with another for equality.
             *
             * This operator checks whether the asset name, optional pack name, and asset path of both objects are identical.
             *
             * @param asset_location The AssetLocation instance to compare with.
             * @return true if all components match; false otherwise.
             */
            bool operator==(const AssetLocation& asset_location) const
            {
                return _name == asset_location._name && _packName == asset_location._packName && _path == asset_location._path;
            }

            /**
             * @brief Compares the AssetLocation with a full location string.
             *
             * Parses the provided full location string into its asset name, path, and optional pack name,
             * then checks whether these components match the corresponding members of the current object.
             *
             * @param fullLocation The string representing the complete asset location.
             * @return true if the parsed components equal the asset's name, pack name, and path; false otherwise.
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
             * @brief Splits a full asset location string into its constituent components.
             *
             * This static function parses the input string to extract an optional package name,
             * an asset name, and an optional path. If the string contains the "::" delimiter, the substring
             * before it is extracted as the package name and removed from further processing. Then, if the
             * remaining string contains the '@' delimiter, the substring following '@' is extracted as the path,
             * leaving the preceding portion as the asset name. If either delimiter is absent, the corresponding
             * output is set to an empty string.
             *
             * @param fullLocation The full asset location string to parse.
             * @param extractedAssetName Output parameter that will receive the extracted asset name.
             * @param extractedPath Output parameter that will receive the extracted asset path.
             * @param extractedPackName Output parameter that will receive the extracted package name.
             *
             * @warning This function does not validate the extracted values.
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
