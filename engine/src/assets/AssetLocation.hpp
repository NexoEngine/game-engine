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
         * Initializes the exception with a formatted error message that includes the invalid asset location and a custom message.
         *
         * @param assetLocation The asset location string that is deemed invalid.
         * @param message Additional detail describing why the asset location is invalid.
         * @param loc Source location where the error occurred (defaults to the current source location).
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
             * @brief Constructs an AssetLocation object from a complete location string.
             *
             * Parses the supplied full location string to extract the asset name, path, and optional pack name,
             * and initializes the object accordingly. Throws an InvalidAssetLocation exception if the extracted
             * asset name is invalid.
             *
             * @param fullLocation A complete asset location string.
             *
             * @throws InvalidAssetLocation if the asset name extracted from fullLocation is invalid.
             */
            explicit AssetLocation(const std::string& fullLocation)
            {
                setLocation(fullLocation);
            }

            /**
             * @brief Sets the asset name.
             *
             * Assigns the provided asset name to the current AssetLocation instance and returns a reference to it
             * to enable method chaining.
             *
             * @param name The new asset name.
             * @return A reference to the updated AssetLocation object.
             */
            AssetLocation& setName(const AssetName& name)
            {
                _name = name;
                return *this;
            }

            /**
             * @brief Updates the asset's path.
             *
             * Assigns the provided string as the asset's new path and returns a reference to the current instance,
             * facilitating method chaining.
             *
             * @param path The new asset path.
             * @return A reference to the modified AssetLocation instance.
             */
            AssetLocation& setPath(const std::string& path)
            {
                _path = path;
                return *this;
            }

            /**
             * @brief Sets the optional asset pack name.
             *
             * Updates the asset location to include the provided asset pack name.
             *
             * @param packName Optional reference to the asset pack name.
             * @return AssetLocation& Reference to the current object for method chaining.
             */
            AssetLocation& setPackName(const std::optional<std::reference_wrapper<const AssetPackName>>& packName)
            {
                _packName = packName;
                return *this;
            }

            /**
 * @brief Retrieves the asset's name.
 *
 * Provides read-only access to the AssetName associated with this AssetLocation.
 *
 * @return const AssetName& Constant reference to the asset name.
 */
[[nodiscard]] const AssetName& getName() const { return _name; }
            /**
 * @brief Retrieves the optional asset pack name associated with this asset location.
 *
 * Returns an optional reference to the constant AssetPackName if one has been set; otherwise, returns an empty optional.
 *
 * @return An optional reference to the asset pack name.
 */
[[nodiscard]] std::optional<std::reference_wrapper<const AssetPackName>> getPackName() { return _packName; }

            /**
 * @brief Retrieves the asset pack name if available.
 *
 * Returns an optional reference wrapper to the associated asset pack name.
 * If no asset pack name has been set, the returned optional will be empty.
 *
 * @return std::optional<std::reference_wrapper<const AssetPackName>> Optional containing the asset pack name.
 */
[[nodiscard]] std::optional<std::reference_wrapper<const AssetPackName>> getPackName() const { return _packName; }
            /**
 * @brief Retrieves the asset's name.
 *
 * Returns the asset name stored within this AssetLocation.
 *
 * @return A constant reference to the AssetName.
 */
[[nodiscard]] const AssetName& getAssetName() const { return _name; }
            /**
 * @brief Retrieves the asset path.
 *
 * Returns a constant reference to the string representing the asset's path.
 *
 * @return A constant reference to the asset's path.
 */
[[nodiscard]] const std::string& getPath() const { return _path; }

            /**
             * @brief Constructs the complete asset location string.
             *
             * Combines the optional pack name, asset name, and asset path into a single string. If a pack name is present,
             * it is prefixed followed by "::". The asset name is always included, and if a non-empty path exists, it is appended
             * with "@".
             *
             * @return std::string The full asset location in the format "[packName::]name[@path]".
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
             * @brief Parses a full asset location string and updates the asset's components.
             *
             * This function decomposes the provided full location string into its constituent parts—asset name, asset path,
             * and an optional asset pack name—using a helper parser. It updates the AssetLocation object's internal state accordingly.
             * An InvalidAssetLocation exception is thrown if the extracted asset name is invalid.
             *
             * @param fullLocation The complete asset location string.
             * @throws InvalidAssetLocation If the asset name extracted from fullLocation is invalid.
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
             * Returns true if the asset name, optional pack name, and asset path of both objects match.
             *
             * @param asset_location The AssetLocation to compare against.
             * @return true if all components are equal, false otherwise.
             */
            bool operator==(const AssetLocation& asset_location) const
            {
                return _name == asset_location._name && _packName == asset_location._packName && _path == asset_location._path;
            }

            /**
             * @brief Compares the asset location with a full location string.
             *
             * Parses the provided string to extract the asset name, path, and pack name, and then checks if
             * these components match the corresponding members of the asset location.
             *
             * @param fullLocation Full asset location string to compare.
             * @return true if the parsed asset name, pack name, and path match the asset location; false otherwise.
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
             * Given a full asset location string, this function extracts up to three parts:
             * an optional pack name, an asset name, and an asset path. If the string contains "::",
             * the substring before "::" is treated as the pack name. If it contains an '@', the substring
             * after the '@' is treated as the asset path. The remaining substring is considered the asset name.
             *
             * @param fullLocation The complete asset location string to parse.
             * @param extractedAssetName Output parameter for storing the parsed asset name.
             * @param extractedPath Output parameter for storing the parsed asset path; cleared if '@' is not found.
             * @param extractedPackName Output parameter for storing the parsed pack name; cleared if "::" is not found.
             *
             * @note This function does not perform validation or trimming on the extracted components.
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
