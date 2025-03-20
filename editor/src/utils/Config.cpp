//// Config.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/03/2025
//  Description: Source file for the config utils
//
///////////////////////////////////////////////////////////////////////////////

#include "Config.hpp"
#include "Path.hpp"

#include <fstream>
#include <string>
#include <regex>
#include <iostream>

namespace nexo::editor {


// Function to find a window's dock ID from the ImGui config file
ImGuiID findWindowDockIDFromConfig(const std::string& windowName)
{
    // 1. Open the config file (usually imgui.ini or your custom path)
    std::string configPath = Path::resolvePathRelativeToExe(
        "../config/default-layout.ini").string(); // Adjust if you use a different path
    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        std::cout << "Could not open config file: " << configPath << std::endl;
        return 0;
    }

    std::string line;
    bool inWindowSection = false;
    ImGuiID dockId = 0;

    // 2. Find the window section and extract the DockId
    std::string windowHeader = "[Window][" + windowName + "]";

    while (std::getline(configFile, line)) {
        // Found the window section we're looking for
        if (line == windowHeader) {
            inWindowSection = true;
            continue;
        }

        std::cout << "Line " << line << std::endl;

        // If we're in the target window section, look for DockId
        if (inWindowSection) {
            // If we hit a new section, stop searching
            if (line.size() > 0 && line[0] == '[') {
                break;
            }


            // Look for DockId=0x... line using regex
            std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");
            std::smatch match;

            if (std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                // Extract the hex dock ID and convert to decimal
                std::string hexDockId = match[1];
                std::stringstream ss;
                ss << std::hex << hexDockId;
                ss >> dockId;
                break;
            }
        }
    }

    configFile.close();

    if (dockId != 0) {
        std::cout << "Found dock ID for window '" << windowName << "': " << dockId << std::endl;
    } else {
        std::cout << "No dock ID found for window '" << windowName << "' in config file" << std::endl;
    }

    return dockId;
}
}
