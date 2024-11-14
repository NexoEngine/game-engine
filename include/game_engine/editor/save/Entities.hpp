//// Entities.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: SaveFile to save ECS entities
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <GameEngine.hpp>

#include "game_engine/save/filesystem/SaveFolder.hpp"

#include <imgui.h>
#include <SaveHandling.hpp>
#include <SceneHandling.hpp>

namespace engine::save {

    class EntitiesSaveFile : public SaveFile {
    private:
        void _saveImpl(std::ofstream& outFile) const override {
            json componentsJson = json::array();
            auto allEntities = engine::getSceneManager().getAllEntities();
            for (const auto& entity : allEntities) {
                componentsJson.push_back({
                    {"entityId", entity},
                    {"data", engine::saveEntityComponents(entity)}
                    });
            }
            outFile << componentsJson.dump(4);
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
