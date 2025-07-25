//// Path.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/07/2025
//  Description: Source file for the path utilities
//
///////////////////////////////////////////////////////////////////////////////

#include "Path.hpp"

namespace nexo {

    const std::filesystem::path& Path::getExecutablePath()
    {
        if (!m_executablePathCached.empty() && !m_executableRootPathCached.empty())
            return m_executablePathCached;
        const boost::dll::fs::path path = boost::dll::program_location();
        m_executablePathCached = path.c_str();
        m_executableRootPathCached = m_executablePathCached.parent_path();
        return m_executablePathCached;
    }

    std::filesystem::path Path::resolvePathRelativeToExe(const std::filesystem::path& path)
    {
        if (m_executableRootPathCached.empty())
            getExecutablePath();
        return (m_executableRootPathCached / path).lexically_normal();
    }

    void Path::resetCache()
    {
        m_executablePathCached.clear();
        m_executableRootPathCached.clear();
    }

    std::string normalizePathAndRemovePrefixSlash(const std::string &rawPath)
    {
        namespace fs = std::filesystem;
        fs::path p = fs::path(rawPath).lexically_normal();

        std::string s = p.generic_string();

        if (s == "/" || s.empty())
            return {};

        size_t start = s.find_first_not_of('/');
        size_t end   = s.find_last_not_of('/');
        return s.substr(start, end - start + 1);
    }
}
