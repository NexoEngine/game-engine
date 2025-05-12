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

    const std::vector<std::string> findAllEditorScenes()
    {
        std::string configPath = Path::resolvePathRelativeToExe(
            "../config/default-layout.ini").string();
        std::ifstream configFile(configPath);

        std::vector<std::string> sceneWindows;

        if (!configFile.is_open()) {
            std::cout << "Could not open config file: " << configPath << std::endl;
            return sceneWindows;
        }

        std::string line;
        std::regex windowRegex("\\[Window\\]\\[(###Default Scene\\d+)\\]");

        while (std::getline(configFile, line)) {
            std::smatch match;
            if (std::regex_search(line, match, windowRegex) && match.size() > 1) {
                // match[1] contains the window name (e.g., "Default Scene0")
                sceneWindows.push_back(match[1].str());
            }
        }

        configFile.close();
        return sceneWindows;
    }

    void setAllWindowDockIDsFromConfig(WindowRegistry& registry)
    {
        std::string configPath = Path::resolvePathRelativeToExe(
            "../config/default-layout.ini").string();
        std::ifstream configFile(configPath);

        if (!configFile.is_open()) {
            std::cout << "Could not open config file: " << configPath << std::endl;
            return;
        }

        std::string line;
        std::string currentWindowName;
        bool inWindowSection = false;
        bool isHashedWindow = false;

        std::regex windowHeaderRegex("\\[Window\\]\\[(.+)\\]");
        std::regex dockIdRegex("DockId=(0x[0-9a-fA-F]+)");

        while (std::getline(configFile, line)) {
            // Check if this line is a window header
            std::smatch windowMatch;
            if (std::regex_search(line, windowMatch, windowHeaderRegex) && windowMatch.size() > 1) {
                currentWindowName = windowMatch[1].str();
                inWindowSection = true;

                // Check if the window name starts with ###
                isHashedWindow = (currentWindowName.size() >= 3 &&
                                 currentWindowName.substr(0, 3) == "###");

                continue;
            }

            // If we're in a window section and it's a hashed window, look for DockId
            if (inWindowSection && isHashedWindow) {
                // If we hit a new section, reset state
                if (!line.empty() && line[0] == '[') {
                    inWindowSection = false;
                    isHashedWindow = false;
                    continue;
                }

                std::smatch dockMatch;
                if (std::regex_search(line, dockMatch, dockIdRegex) && dockMatch.size() > 1) {
                    std::string hexDockId = dockMatch[1];
                    ImGuiID dockId = 0;
                    std::stringstream ss;
                    ss << std::hex << hexDockId;
                    ss >> dockId;

                    // Set the dock ID for this window in the registry
                    if (dockId != 0) {
                        std::cout << "Setting dock id " << dockId << " for hashed window "
                                  << currentWindowName << std::endl;
                        registry.setDockId(currentWindowName, dockId);
                    }
                }
            } else if (inWindowSection && !isHashedWindow && !line.empty() && line[0] == '[') {
                // Reset state when we hit a new section
                inWindowSection = false;
                isHashedWindow = false;
            }
        }

        configFile.close();
    }
}
