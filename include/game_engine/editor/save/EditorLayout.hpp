//// EditorLayout.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: SaveFile to save IMGUI layout.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "game_engine/save/filesystem/SaveFile.hpp"

#include <imgui.h>

namespace engine::save {

    class EditorLayoutSaveFile : public SaveFile {
    private:
        void _saveImpl(std::ofstream& outFile) const override {
            outFile << ImGui::SaveIniSettingsToMemory() << std::endl;
        }

        void _loadImpl(std::ifstream& inFile) override
        {
            if (!inFile.is_open()) {
                ImGui::LoadIniSettingsFromDisk("imgui.ini");
                return;
            }
            std::stringstream ss;
            ss << inFile.rdbuf();
            std::string iniString = ss.str();
            ImGui::LoadIniSettingsFromMemory(iniString.c_str(), iniString.size());
            LOG_F(INFO, "Editor layout loaded successfully");
        }
    };

} // namespace engine::editor
