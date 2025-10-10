//// FolderManager.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/07/2025
//  Description: Header file for the folder manager
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "assets/AssetCatalog.hpp"

namespace nexo::editor {

    class FolderManager {
       private:
        std::unordered_map<std::string, std::string> m_pathToName;            // path -> display name
        std::unordered_map<std::string, std::vector<std::string>> m_children; // path -> direct children paths

       public:
        FolderManager();

        /** @brief Builds the folder structure from the available assets.
         * This method scans through all assets in the catalog and constructs the folder hierarchy.
         * It populates the internal maps (m_pathToName and m_children) based on asset paths.
         */
        void buildFromAssets();

        /** @brief Retrieves the children of a specified folder path with their display names.
         * @param path The folder path for which to retrieve children.
         * @return A vector of pairs containing path and display name for each child folder.
         */
        [[nodiscard]] std::vector<std::pair<std::string, std::string>> getChildren(const std::string& path) const;

        /** @brief Gets the display name for a folder path.
         * @param path The folder path for which to retrieve the display name.
         * @return The display name of the folder. Returns empty string if path doesn't exist.
         */
        [[nodiscard]] std::string getName(const std::string& path) const;

        /** @brief Checks if a folder path exists in the folder structure.
         * @param path The folder path to check.
         * @return `true` if the path exists, `false` otherwise.
         */
        [[nodiscard]] bool exists(const std::string& path) const;

        /** @brief Retrieves all assets contained within a specified folder.
         * @param folderPath The folder path for which to retrieve assets.
         * @return A vector of asset references located in the specified folder.
         */
        [[nodiscard]] std::vector<assets::GenericAssetRef> getFolderAssets(const std::string& folderPath) const;

        /** @brief Creates a new folder within a parent folder.
         * @param parentPath The path of the parent folder where the new folder will be created.
         * @param folderName The name of the new folder to create.
         * @return `true` if folder creation was successful, `false` otherwise.
         */
        bool createFolder(const std::string& parentPath, const std::string& folderName);

        /** @brief Deletes an existing folder and potentially its contents.
         * @param folderPath The path of the folder to delete.
         * @return `true` if folder deletion was successful, `false` otherwise.
         */
        bool deleteFolder(const std::string& folderPath);

        /** @brief Renames an existing folder.
         * @param folderPath The path of the folder to rename.
         * @param newName The new name to assign to the folder.
         * @return `true` if folder renaming was successful, `false` otherwise.
         */
        bool renameFolder(const std::string& folderPath, const std::string& newName);

        /** @brief Moves a folder to a new location in the folder structure.
         * @param currentFolderPath The current path of the folder to move.
         * @param path The destination path where the folder should be moved.
         * @return `true` if folder was successfully moved, `false` otherwise.
         */
        bool moveFolder(const std::string& currentFolderPath, const std::string& path);

        /** @brief Retrieves all folder paths in the folder structure.
         * @return A vector containing all folder paths in the structure.
         */
        [[nodiscard]] std::vector<std::string> getAllPaths() const;

        /** @brief Retrieves the number of direct child folders for a specified folder path.
         * This method checks if the given folder path exists in the `m_children` map.
         * If it does, it returns the count of direct children (subfolders) associated with that path.
         * If the folder path does not exist, it returns 0.
         * @param path The folder path for which to count the direct children.
         * @return The number of direct child folders. Returns 0 if the folder does not exist.
         */
        [[nodiscard]] size_t getChildCount(const std::string& path) const;

        /**
         * @brief Calculates the total size of all assets in a specified folder.
         * This static method computes the cumulative size of all assets located within the given folder path.
         * It iterates through the assets catalog, summing the sizes of assets that are either directly
         * in the specified folder or in its subfolders.
         * @param folderPath The path of the folder for which to calculate the total asset size.
         * @return The total size of all assets in the folder, in bytes. Returns 0 if the folder does not exist
         * or contains no assets.
         */
        [[nodiscard]] static float getFolderSize(std::string_view folderPath);

        /** @brief Validates a folder name.
         * This static method checks if the provided folder name is valid. A valid name is non-empty,
         * does not contain slashes ('/' or '\'), and is not "." or "..".
         * @note This method does not check for name uniqueness within a specific folder.
         * @param name The folder name to validate.
         * @return `true` if the name is valid, `false` otherwise.
         */
        [[nodiscard]] static bool isNameValid(const std::string& name);

       private:
        void clear();

        static void addPathAndParents(const std::string& fullPath, std::unordered_set<std::string>& allPaths);

        void buildMapsFromPaths(const std::unordered_set<std::string>& allPaths);

        [[nodiscard]] static std::string extractNameFromPath(const std::string& path);

        [[nodiscard]] static std::string getParentPath(const std::string& path);
    };

} // namespace nexo::editor
