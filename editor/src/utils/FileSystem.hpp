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

namespace nexo::editor::utils {

    /**
     * @brief Opens a file explorer window showing a specified folder.
     *
     * Uses platform-specific mechanisms to open the operating system's file explorer
     * at the specified folder location:
     * - On Windows: Uses ShellExecuteA to open Windows Explorer
     * - On Linux/Unix: Uses xdg-open via system command
     *
     * This function is intended for user interaction purposes, such as revealing
     * exported files, log directories, or other locations that the user may need
     * to access directly.
     *
     * @param folderPath The absolute path to the folder to open
     *
     * @note This function does not check if the path exists or is accessible.
     *       It simply passes the request to the operating system which will handle
     *       any error conditions according to its standard behavior.
     *
     * @note On non-Windows platforms, this executes a system command, which may have
     *       security implications if folderPath contains untrusted input.
     */
    void openFolder(const std::string &folderPath);
}
