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

namespace nexo::editor {

    static bool isNameValid(std::string_view folderName)
    {
        std::cout << "Checking folder name validity: '" << folderName << "'" << std::endl;
        return !(folderName.empty() || folderName.front() == '_' || folderName.find('/') != std::string::npos);
    }

    FolderManager::FolderManager()
    {
        m_pathToName[""] = "Assets";
        m_children[""] = {};
    }

    void FolderManager::buildFromAssets()
    {
        clear();
        std::unordered_set<std::string> allPaths;

        for (const auto& ref : assets::AssetCatalog::getInstance().getAssets()) {
            if (const auto assetData = ref.lock()) {
                const std::string& folderPath = assetData->getMetadata().location.getPath();
                if (folderPath.empty() || folderPath.front() == '_')
                    continue;
                addPathAndParents(folderPath, allPaths);
            }
        }

        buildMapsFromPaths(allPaths);
    }

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

    std::string FolderManager::getName(const std::string& path) const
    {
        if (const auto it = m_pathToName.find(path); it != m_pathToName.end())
            return it->second;
        return extractNameFromPath(path);
    }

    bool FolderManager::exists(const std::string& path) const
    {
        return m_pathToName.contains(path);
    }

    std::vector<assets::GenericAssetRef> FolderManager::getFolderAssets(const std::string& folderPath) const
    {
        std::vector<assets::GenericAssetRef> assets;
        if (folderPath.empty() || !exists(folderPath))
            return assets;

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

    bool FolderManager::createFolder(const std::string& parentPath, const std::string& folderName)
    {
        if (!exists(parentPath)) return false;

        const std::string newFolderPath = parentPath.empty() ? folderName : parentPath + "/" + folderName;
        if (exists(newFolderPath))
            return false;

        if (!isNameValid(folderName))
            return false;

        m_pathToName[newFolderPath] = folderName;
        m_children[newFolderPath] = {};
        m_children[parentPath].push_back(newFolderPath);

        std::ranges::sort(m_children[parentPath]);

        return true;
    }

    bool FolderManager::deleteFolder(const std::string& folderPath)
    {
        if (folderPath.empty()) // || !exists(folderPath)) TODO: fix exists check
            return false;

        // Recursively delete all folder inside first
        for (const auto childrenCopy = m_children[folderPath]; const std::string& childPath : childrenCopy)
            deleteFolder(childPath);

        // Remove from parent's children list
        const std::string parentPath = getParentPath(folderPath);
        if (const auto parentIt = m_children.find(parentPath); parentIt != m_children.end()) {
            auto& parentChildren = parentIt->second;
            std::erase(parentChildren, folderPath);
        }

        m_pathToName.erase(folderPath);
        m_children.erase(folderPath);
        return true;
    }

    bool FolderManager::renameFolder(const std::string& folderPath, const std::string& newName)
    {
        if (newName.empty() || folderPath.empty())// || !exists(folderPath))
            return false;

        if (!isNameValid(newName)) return false;

        const std::string parentPath = getParentPath(folderPath);
        const std::string newFolderPath = parentPath.empty() ? newName : parentPath + "/" + newName;

        if (newFolderPath != folderPath && exists(newFolderPath))
            return false;

        // If the path doesn't change, just update the display name
        if (newFolderPath == folderPath) {
            m_pathToName[folderPath] = newName;
            return true;
        }

        // Get all children paths that need to be updated
        std::vector<std::string> toUpdate;
        for (const auto& [path, _] : m_pathToName) {
            if (path == folderPath || path.starts_with(folderPath + "/")) {
                toUpdate.push_back(path);
            }
        }

        // Update paths and names
        for (const auto& oldPath : toUpdate) {
            std::string newPath = newFolderPath + oldPath.substr(folderPath.size());
            m_pathToName[newPath] = (oldPath == folderPath) ? newName : extractNameFromPath(newPath);
            m_children[newPath] = std::move(m_children[oldPath]);
        }

        // Clean up old paths
        for (const auto& oldPath : toUpdate) {
            m_pathToName.erase(oldPath);
            m_children.erase(oldPath);
        }

        //  Update the parent's children list
        auto& siblings = m_children[parentPath];
        std::ranges::replace(siblings, folderPath, newFolderPath);

        return true;
    }

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

    size_t FolderManager::getChildCount(const std::string& path) const
    {
        if (const auto it = m_children.find(path); it != m_children.end()) {
            return it->second.size();
        }
        return 0;
    }

    void FolderManager::clear()
    {
        m_pathToName.clear();
        m_children.clear();

        m_pathToName[""] = "Assets";
        m_children[""] = {};
    }

    void FolderManager::addPathAndParents(const std::string& fullPath, std::unordered_set<std::string>& allPaths) const
    {
        if (fullPath.empty()) return;

        std::string currentPath;
        std::stringstream ss(fullPath);
        std::string part;

        while (std::getline(ss, part, '/')) {
            if (!part.empty()) {
                currentPath = currentPath.empty() ? part : currentPath + "/" + part;
                allPaths.insert(currentPath);
            }
        }
    }

    void FolderManager::buildMapsFromPaths(const std::unordered_set<std::string>& allPaths)
    {
        // Build path->name mapping
        for (const std::string& path : allPaths) {
            m_pathToName[path] = extractNameFromPath(path);
            m_children[path] = {}; // Initialize empty children vector
        }

        // Build parent->children relationships
        for (const std::string& path : allPaths) {
            std::string parentPath = getParentPath(path);
            m_children[parentPath].push_back(path);
        }

        // Sort all children vectors
        for (auto& [parent, children] : m_children)
            std::ranges::sort(children);
    }

    std::string FolderManager::extractNameFromPath(const std::string& path) const
    {
        if (path.empty()) return "Assets";

        const size_t lastSlash = path.find_last_of('/');
        return (lastSlash == std::string::npos) ? path : path.substr(lastSlash + 1);
    }

    std::string FolderManager::getParentPath(const std::string& path) const
    {
        if (path.empty())
            return ""; // Root has no parent

        const size_t lastSlash = path.find_last_of('/');
        return (lastSlash == std::string::npos) ? "" : path.substr(0, lastSlash);
    }
}
