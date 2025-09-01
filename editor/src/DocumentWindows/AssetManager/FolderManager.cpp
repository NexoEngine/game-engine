//// FolderManager.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/07/2025
//  Description: Source file for the folder manager
//
///////////////////////////////////////////////////////////////////////////////

#include "FolderManager.hpp"
#include "AssetManagerWindow.hpp"

namespace nexo::editor {

    /**
     * @brief Constructs a FolderManager instance.
     *
     * Initializes the folder manager with a root folder named "Assets".
     * The root folder has no children at the start.
     */
    FolderManager::FolderManager()
    {
        m_pathToName[""] = "Assets";
        m_children[""]   = {};
    }

    /** @brief Builds the folder structure from the assets catalog.
     *
     * This method clears the current folder structure and populates it
     * with paths derived from the assets catalog. It iterates through all
     * assets, extracts their folder paths, and adds them along with their
     * parent folders to the internal maps.
     */
    void FolderManager::buildFromAssets()
    {
        clear();
        std::unordered_set<std::string> allPaths;

        for (const auto& ref : assets::AssetCatalog::getInstance().getAssets()) {
            if (const auto assetData = ref.lock()) {
                const std::string& folderPath = assetData->getMetadata().location.getPath();
                if (folderPath.empty() || folderPath.front() == '_') continue;
                addPathAndParents(folderPath, allPaths);
            }
        }

        buildMapsFromPaths(allPaths);
    }

    /**
     * @brief Retrieves the children of a specified folder path.
     *
     * This method searches for the children of the given folder path in the `m_children` map.
     * For each child path found, it retrieves the corresponding name from the `m_pathToName` map
     * and adds it to the result as a pair of path and name.
     *
     * @param path The folder path for which to retrieve the children.
     * @return A vector of pairs, where each pair contains a child path and its corresponding name.
     */
    std::vector<std::pair<std::string, std::string>> FolderManager::getChildren(const std::string& path) const
    {
        std::vector<std::pair<std::string, std::string>> result;

        if (const auto it = m_children.find(path); it != m_children.end()) {
            result.reserve(it->second.size());
            for (const std::string& childPath : it->second) {
                auto nameIt = m_pathToName.find(childPath);
                if (nameIt != m_pathToName.end()) {
                    result.emplace_back(childPath, nameIt->second);
                }
            }
        }

        return result;
    }

    /**
     * @brief Retrieves the name of a folder given its path.
     *
     * This method checks if the folder path exists in the `m_pathToName` map.
     * If it does, it returns the corresponding name. Otherwise, it extracts
     * the name from the path using the `extractNameFromPath` method.
     *
     * @param path The folder path for which to retrieve the name.
     * @return The name of the folder.
     */
    std::string FolderManager::getName(const std::string& path) const
    {
        if (const auto it = m_pathToName.find(path); it != m_pathToName.end()) return it->second;
        return extractNameFromPath(path);
    }

    /**
     * @brief Checks if a folder path exists in the folder manager.
     *
     * This method verifies whether the specified folder path is present
     * in the `m_pathToName` map.
     *
     * @param path The folder path to check.
     * @return `true` if the folder path exists, `false` otherwise.
     */
    bool FolderManager::exists(const std::string& path) const
    {
        return m_pathToName.contains(path);
    }

    /**
     * @brief Retrieves all assets located in a specified folder.
     *
     * This method collects all assets whose paths match the specified folder path
     * or are located within its subfolders. It ensures that the folder exists
     * before attempting to retrieve the assets.
     *
     * @param folderPath The path of the folder from which to retrieve assets.
     * @return A vector of `GenericAssetRef` objects representing the assets in the folder.
     */
    std::vector<assets::GenericAssetRef> FolderManager::getFolderAssets(const std::string& folderPath) const
    {
        std::vector<assets::GenericAssetRef> assets;
        if (folderPath.empty() || !exists(folderPath)) return assets;

        for (const auto& ref : assets::AssetCatalog::getInstance().getAssets()) {
            if (const auto assetData = ref.lock()) {
                const std::string& assetPath = assetData->getMetadata().location.getPath();
                if (assetPath.starts_with(folderPath + "/") || assetPath == folderPath) {
                    assets.push_back(ref);
                }
            }
        }

        return assets;
    }

    /**
     * @brief Creates a new folder under a specified parent folder.
     *
     * This method checks if the parent folder exists and if the new folder name is valid.
     * If the conditions are met, it adds the new folder to the folder manager,
     * updates the parent-child relationships, and sorts the parent's children.
     *
     * @param parentPath The path of the parent folder where the new folder will be created.
     * @param folderName The name of the new folder to create.
     * @return `true` if the folder was successfully created, `false` otherwise.
     */
    bool FolderManager::createFolder(const std::string& parentPath, const std::string& folderName)
    {
        if (!exists(parentPath)) return false;

        const std::string newFolderPath = parentPath.empty() ? folderName : parentPath + "/" + folderName;
        if (exists(newFolderPath)) return false;

        if (!isNameValid(folderName)) return false;

        // Add the new folder to the internal maps
        m_pathToName[newFolderPath] = folderName;
        m_children[newFolderPath]   = {};
        m_children[parentPath].push_back(newFolderPath);

        std::ranges::sort(m_children[parentPath]);

        return true;
    }

    /**
     * @brief Deletes a folder and all its contents recursively.
     *
     * This method removes the specified folder and all its child folders from the folder hierarchy.
     * It ensures that all references to the folder are removed, including its entry in the parent folder's
     * children list and the internal maps `m_pathToName` and `m_children`.
     *
     * @param folderPath The path of the folder to delete.
     * @return `true` if the folder was successfully deleted, `false` otherwise.
     */
    bool FolderManager::deleteFolder(const std::string& folderPath)
    {
        if (folderPath.empty() || !exists(folderPath)) return false;

        // Recursively delete all folder inside first
        for (const auto childrenCopy = m_children[folderPath]; const std::string& childPath : childrenCopy)
            deleteFolder(childPath);

        // Remove from parent's children list
        const std::string parentPath = getParentPath(folderPath);
        if (const auto parentIt = m_children.find(parentPath); parentIt != m_children.end()) {
            auto& parentChildren = parentIt->second;
            std::erase(parentChildren, folderPath);
        }

        // Remove the folder from the internal maps
        m_pathToName.erase(folderPath);
        m_children.erase(folderPath);
        return true;
    }

    /**
     * @brief Validates a name based on specific criteria.
     *
     * This method checks if the name is valid according to the following rules:
     * - It must not be empty.
     * - It must not start with an underscore ('_').
     * - It must not contain slashes ('/').
     * - It must only contain alphanumeric characters, dots ('.'), hyphens ('-'), and underscores ('_').
     *
     * @param name The name of the item to validate.
     * @return `true` if the name is valid, `false` otherwise.
     */
    bool FolderManager::isNameValid(const std::string& name)
    {
        const bool containsWrongCharacter = std::ranges::any_of(
            name, [](const char c) { return !std::isalnum(c) && c != '.' && c != '-' && c != '_' && c != ' '; });

        return !(name.empty() || name.front() == '_' || name.find('/') != std::string::npos) && !containsWrongCharacter;
    }

    /**
     * @brief Renames a folder and updates all its children paths accordingly.
     *
     * This method renames a folder by updating its path and the paths of all its children
     * in the folder hierarchy. It ensures that the new name is valid and that no folder
     * with the same name already exists in the same parent directory. If the folder path
     * does not change, only the display name is updated.
     *
     * @param folderPath The current path of the folder to rename.
     * @param newName The new name for the folder.
     * @return `true` if the folder was successfully renamed, `false` otherwise.
     */
    bool FolderManager::renameFolder(const std::string& folderPath, const std::string& newName)
    {
        if (newName.empty() || folderPath.empty() || !exists(folderPath)) return false;

        if (!isNameValid(newName)) return false;

        const std::string parentPath    = getParentPath(folderPath);
        const std::string newFolderPath = folderPath.find_last_of('/') == std::string::npos ?
                                              newName :
                                              folderPath.substr(0, folderPath.find_last_of('/')) + "/" + newName;

        if (newFolderPath != folderPath && exists(newFolderPath)) return false;

        // If the path doesn't change, just update the display name
        if (newFolderPath == folderPath) {
            m_pathToName[folderPath] = newName;
            return true;
        }

        // Get all children paths that need to be updated
        std::vector<std::string> toUpdate;
        for (const auto& key : m_pathToName | std::views::keys) {
            if (key == folderPath || key.starts_with(folderPath + "/")) {
                toUpdate.push_back(key);
            }
            std::cout << std::endl;
        }

        // Update paths and names
        for (const auto& oldPath : toUpdate) {
            std::string newPath   = newFolderPath + oldPath.substr(folderPath.size());
            m_pathToName[newPath] = (oldPath == folderPath) ? newName : extractNameFromPath(newPath);
            m_children[newPath]   = std::move(m_children[oldPath]);
            m_children[parentPath].push_back(newPath);
        }

        // Clean up old paths
        for (const auto& oldPath : toUpdate) {
            m_pathToName.erase(oldPath);
            m_children.erase(oldPath);
            std::erase(m_children[parentPath], oldPath);
        }

        return true;
    }

    bool FolderManager::moveFolder(const std::string& currentFolderPath, const std::string& path)
    {
        std::cout << "======================================================\nMoving folder '" << currentFolderPath
                  << "' to '" << path << "'\n";

        // if current folder is not empty or already existing or not same as path and if path exists
        if (currentFolderPath.empty() || !exists(currentFolderPath) || !exists(path) || path == currentFolderPath)
            return false;

        const std::string parentPath = getParentPath(currentFolderPath);
        const std::string folderName = extractNameFromPath(currentFolderPath);

        const std::string newFolderPath = path.empty() ? folderName : path + "/" + folderName;

        //if new path already (same destination or folder with same name exists at this destination
        if (exists(newFolderPath)) return false;

        // update name of children
        std::vector<std::string> toUpdate;
        for (const auto& children_path : m_children | std::views::keys) {
            if (children_path == currentFolderPath || children_path.find(currentFolderPath) != std::string::npos) {
                toUpdate.push_back(children_path);
                const std::string newPath   = newFolderPath + children_path.substr(currentFolderPath.size());
                for (const auto & child : m_children[children_path]) {
                    if (child.find(currentFolderPath) != std::string::npos) {
                        std::string newChildPath   = newFolderPath + child.substr(currentFolderPath.size());
                        m_children[newPath].push_back(newChildPath);
                    }
                }
            }
        }
        std::erase(m_children[parentPath], currentFolderPath);
        m_children[path].push_back(newFolderPath);

        // get all assets
        const assets::AssetCatalog &assetCatalog    = assets::AssetCatalog::getInstance();
        std::vector<assets::GenericAssetRef> assets = assetCatalog.getAssets();

        // update path of asset inside
        for (const auto &ref : assets) {
            const auto d = ref.lock();
            if (!d) continue;
            std::string assetPath = d->getMetadata().location.getPath();
            if (assetPath == currentFolderPath || assetPath.starts_with(currentFolderPath + "/")) {
                std::string newAssetPath = newFolderPath + assetPath.substr(currentFolderPath.size());
                d->getMetadata().location.setPath(newAssetPath);
            }
        }

        for (const auto& oldPath : toUpdate) {
            m_children.erase(oldPath);
        }

        // update path in pathToName
        for (const auto& key : m_pathToName | std::views::keys) {
            if (key.find(currentFolderPath) != std::string::npos) {
                toUpdate.push_back(key);
                // std::cout << "key [" << key << "] new folder path [" << newFolderPath << "] m_pathToName[key] [" << m_pathToName[key] << "]" << std::endl;
                m_pathToName[newFolderPath] = m_pathToName[key];
            }
        }

        for (const auto& oldPath : toUpdate) {
            m_pathToName.erase(oldPath);
        }

        // for (const auto& child : m_children | std::views::keys) {
        //     std::cout << "Child: [" << child << "]\n";
        //     for (const auto& val : m_children[child]) {
        //         std::cout << "   - " << val << "\n";
        //     }
        // }
        //
        // std::cout << std::endl;
        //
        // for (const auto& key : m_pathToName | std::views::keys) {
        //     std::cout << "Key: [" << key << "]\n";
        //     std::cout << "   - " << m_pathToName[key] << "\n";
        // }
        return true;
    }

    /**
     * @brief Retrieves all folder paths managed by the FolderManager.
     *
     * This method collects all folder paths stored in the `m_pathToName` map,
     * sorts them in lexicographical order, and returns them as a vector.
     *
     * @return A sorted vector containing all folder paths.
     */
    std::vector<std::string> FolderManager::getAllPaths() const
    {
        std::vector<std::string> paths;
        paths.reserve(m_pathToName.size());
        for (const auto& path : m_pathToName | std::views::keys) {
            paths.push_back(path);
        }
        std::ranges::sort(paths);
        return paths;
    }

    /**
     * @brief Retrieves the number of direct children for a given folder path.
     *
     * This method checks if the specified folder path exists in the `m_children` map.
     * If it does, it returns the size of the vector containing the folder's children.
     * If the folder path does not exist, it returns 0.
     *
     * @param path The folder path for which to count the children.
     * @return The number of direct children of the specified folder path.
     */
    size_t FolderManager::getChildCount(const std::string& path) const
    {
        if (const auto it = m_children.find(path); it != m_children.end()) {
            return it->second.size();
        }
        return 0;
    }

    /**
     * @brief Calculates the total size of a folder and its contents on disk.
     *
     * This method recursively traverses the specified folder and sums the sizes
     * of all regular files within it. The total size is returned in megabytes (Mo).
     *
     * @param folderPath The path of the folder to calculate the size for.
     * @return The total size of the folder in megabytes, or 0 if an error occurs.
     */
    float FolderManager::getFolderSize(const std::string& folderPath)
    {
        namespace fs    = std::filesystem;
        float totalSize = 0.0f;
        try {
            for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
                if (is_regular_file(entry.path())) {
                    totalSize += static_cast<float>(fs::file_size(entry.path()));
                }
            }
        } catch (const std::exception&) {
            return 0.0f;
        }
        return totalSize / (1024.0f * 1024.0f); // Return size in Mo
    }

    /**
     * @brief Clears all folder data and resets the folder manager to its initial state.
     *
     * This method removes all entries from the internal maps `m_pathToName` and `m_children`,
     * effectively clearing the folder hierarchy. After clearing, it reinitializes the root
     * folder with the name "Assets" and an empty list of children.
     */
    void FolderManager::clear()
    {
        m_pathToName.clear(); // Remove all folder paths and names.
        m_children.clear();   // Remove all parent-child relationships.

        // Reinitialize the root folder
        m_pathToName[""] = "Assets";
        m_children[""]   = {};
    }

    /**
     * @brief Adds a full path and all its parent paths to a set of paths.
     *
     * This method breaks down a full folder path into its individual segments
     * and adds each segment (as well as the cumulative path up to that segment)
     * to the `allPaths` set. This ensures that all parent folders of a given path
     * are also included in the folder hierarchy.
     *
     * @param fullPath The full path to add (e.g., "folder/subfolder/item").
     * @param allPaths A reference to a set of strings where the paths will be added.
     */
    void FolderManager::addPathAndParents(const std::string& fullPath, std::unordered_set<std::string>& allPaths)
    {
        if (fullPath.empty()) return;

        std::string currentPath;
        std::stringstream ss(fullPath);
        std::string part;

        while (std::getline(ss, part, '/')) {
            if (!part.empty()) {
                if (!currentPath.empty()) {
                    currentPath += "/";
                }
                currentPath += part;
                allPaths.insert(currentPath);
            }
        }
    }

    /**
     * @brief Builds internal maps for folder paths and their relationships.
     *
     * This method populates the `m_pathToName` and `m_children` maps based on the provided set of folder paths.
     * It first initializes the mappings for each path, then establishes parent-child relationships,
     * and finally sorts the children of each parent folder.
     *
     * @param allPaths A set of folder paths to process and build the maps from.
     */
    void FolderManager::buildMapsFromPaths(const std::unordered_set<std::string>& allPaths)
    {
        // Build path->name mapping
        for (const std::string& path : allPaths) {
            m_pathToName[path] = extractNameFromPath(path);
            m_children[path]   = {}; // Initialize empty children vector
        }

        // Build parent->children relationships
        for (const std::string& path : allPaths) {
            std::string parentPath = getParentPath(path);
            m_children[parentPath].push_back(path);
        }

        // Sort all children vectors
        for (auto& [parent, children] : m_children) std::ranges::sort(children);
    }

    /**
     * @brief Extracts the name of a folder from its path.
     *
     * This method retrieves the last segment of a folder path, which represents the folder's name.
     * If the path is empty, it returns "Assets" as the default name.
     *
     * @param path The folder path to extract the name from.
     * @return The name of the folder.
     */
    std::string FolderManager::extractNameFromPath(const std::string& path)
    {
        if (path.empty()) return "Assets";

        const size_t lastSlash = path.find_last_of('/');
        return (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);
    }

    /**
     * @brief Retrieves the parent path of a given folder path.
     *
     * This method determines the parent folder path by removing the last segment of the given path.
     * If the path is empty or has no parent, it returns an empty string.
     *
     * @param path The folder path to retrieve the parent path from.
     * @return The parent path of the folder.
     */
    std::string FolderManager::getParentPath(const std::string& path)
    {
        if (path.empty()) return ""; // Root has no parent

        const size_t lastSlash = path.find_last_of('/');
        return (lastSlash == std::string::npos) ? "" : path.substr(0, lastSlash);
    }
} // namespace nexo::editor
