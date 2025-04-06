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

    ImGuiID findWindowDockIDFromConfig(const std::string& windowName)
    {
        std::string configPath = Path::resolvePathRelativeToExe(
            "../config/default-layout.ini").string();
        std::ifstream configFile(configPath);
        if (!configFile.is_open()) {
            std::cout << "Could not open config file: " << configPath << std::endl;
            return 0;
        }

        std::string line;
        bool inWindowSection = false;
        ImGuiID dockId = 0;

        std::string windowHeader = "[Window][" + windowName + "]";

        while (std::getline(configFile, line)) {
            // Found the window section we're looking for
            if (line == windowHeader) {
                inWindowSection = true;
                continue;
            }

            if (inWindowSection) {
                // If we hit a new section, stop searching
                if (!line.empty() && line[0] == '[')
                    break;

                std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

                if (std::smatch match; std::regex_search(line, match, dockIdRegex) && match.size() > 1) {
                    std::string hexDockId = match[1];
                    std::stringstream ss;
                    ss << std::hex << hexDockId;
                    ss >> dockId;
                    break;
                }
            }
        }

        configFile.close();
        return dockId;
    }
}
