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
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <sstream>

#include "assets/AssetCatalog.hpp"

namespace nexo::editor {

    class FolderManager {
        private:
            std::unordered_map<std::string, std::string> m_pathToName;           // path -> display name
            std::unordered_map<std::string, std::vector<std::string>> m_children; // path -> direct children paths

        public:
            FolderManager();

            void buildFromAssets();

            std::vector<std::pair<std::string, std::string>> getChildren(const std::string& path) const;

            std::string getName(const std::string& path) const;

            bool exists(const std::string& path) const;

            bool createFolder(const std::string& parentPath, const std::string& folderName);

            bool deleteFolder(const std::string& folderPath);

            bool renameFolder(const std::string& folderPath, const std::string& newName);

            std::vector<std::string> getAllPaths() const;

            size_t getChildCount(const std::string& path) const;

        private:
            void clear();

            void addPathAndParents(const std::string& fullPath, std::unordered_set<std::string>& allPaths);

            void buildMapsFromPaths(const std::unordered_set<std::string>& allPaths);

            std::string extractNameFromPath(const std::string& path) const;

            std::string getParentPath(const std::string& path) const;
    };

} // namespace nexo::editor
