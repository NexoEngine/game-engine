//// Path.hpp /////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        23/11/2024
//  Description: Utils for handling paths
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <filesystem>
#include <boost/dll/runtime_symbol_info.hpp>

namespace nexo {

    /**
     * @brief Utils for handling paths
     */
    class Path {
    public:
        /**
         * @brief Get the path to the executable (e.g.: nexoEditor)
         * @return The path to the executable
         */
        static const std::filesystem::path& getExecutablePath();

        /**
        * @brief Resolve a path relative to the executable
        * @param path The path to resolve
        * @return The resolved path
        *
        * @note Example: if assets is a folder in the same directory as the executable, you can use: resolvePathRelativeToExe("assets")
        * @example ../editor/src/Editor.cpp
        */
        static std::filesystem::path resolvePathRelativeToExe(const std::filesystem::path& path);

        /**
         * @brief Reset the cached paths
         */
        static void resetCache();

        private:
            Path() = default;

            inline static std::filesystem::path m_executablePathCached;
            inline static std::filesystem::path m_executableRootPathCached;
    };

    std::string normalizePath(const std::string &rawPath);
} // namespace nexo
