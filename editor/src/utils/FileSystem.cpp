//// FileSystem.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/04/2025
//  Description: Source file for the file system utils functions
//
///////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h>
#endif

#include <iomanip>
#include <sstream>
#include "FileSystem.hpp"
#include "Logger.hpp"

namespace nexo::editor::utils {
    void openFolder(const std::string &folderPath)
    {
        #ifdef _WIN32
            ShellExecuteA(nullptr, "open", folderPath.c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
        #else
            std::stringstream ss;
            ss << "xdg-open " << std::quoted(folderPath);
            const int status = std::system(ss.str().c_str());
            if (status == -1)
                LOG(NEXO_ERROR, "Failed to open folder '{}'", folderPath);
            else if (status != 0)
                LOG(NEXO_WARN, "Opening folder '{}' returned exit code {}", folderPath, status);
        #endif
    }
}
