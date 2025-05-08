//// FileSystem.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/04/2025
//  Description: Source file for the file system utils functions
//
///////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
    #define NOMINMAX
    #include <windows.h>
#endif

#include "FileSystem.hpp"
#include <iomanip>



namespace nexo::editor::utils {
    void openFolder(const std::string &folderPath)
    {
        #ifdef _WIN32
            ShellExecuteA(nullptr, "open", folderPath.c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
        #else
            const std::string command = "xdg-open " + std::quoted(folderPath);
            std::system(command.c_str());
        #endif
    }
}
