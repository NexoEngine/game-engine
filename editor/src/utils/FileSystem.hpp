//// FileSystem.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/04/2025
//  Description: Header file for utils function to handle files
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace nexo::editor::utils {
    void openFolder(const std::string &folderPath);
}
