//// Scenes.hpp ///////////////////////////////////////////////////////////////
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

    class ScenesSaveFolder : public SaveFolder {
    public:
        void save() override
        {
            SaveFolder::save();
            auto sceneIDs = engine::getSceneManager().getSceneIDs();
            for (const auto &sceneID : sceneIDs) {
                _saveScene(sceneID);
            }
        }

        void load() override
        {
            SaveFolder::load();
        }
    private:

        void _saveSceneIndex()
        {
            json sceneIndex;
            for (const auto &sceneID : engine::getSceneManager().getSceneIDs()) {
                sceneIndex.push_back({"scene" + std::to_string(sceneID), sceneID});
            }
        }

        void _saveScene(ecs::SceneID sceneID)
        {
            std::filesystem::path scenePath = getPath() / ("scene" + std::to_string(sceneID));
            std::filesystem::create_directory(scenePath);

            json entitiesData;
            auto sceneEntities = engine::getSceneManager().getSceneEntities(sceneID);
            for (const auto &entity : sceneEntities) {
                entitiesData.push_back({
                    {"entityId", entity},
                    {"data", engine::saveEntityComponents(entity)}
                });
            }
            std::ofstream entitiesFile(scenePath / "entities.json");
            entitiesFile << entitiesData.dump(4) << std::endl;
        }
        


    };

} // namespace engine::editor
