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
//  Description: SaveFile to save ECS scenes
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <GameEngine.hpp>

#include "game_engine/save/filesystem/SaveFolder.hpp"

#include <imgui.h>
#include "SaveHandling.hpp"
#include "SceneHandling.hpp"

namespace engine::save {

    class ScenesSaveFolder : public SaveFolder {
    public:
        const char *INDEX_FILE_MAGIC = "nindx";
		const char* ENTITIES_FILE_MAGIC = "nentt";

        void save() override
        {
            SaveFolder::save();
            auto sceneIDs = engine::getSceneManager().getSceneIDs();
            _saveSceneIndex(sceneIDs);
            for (const auto &sceneID : sceneIDs) {
                _saveScene(sceneID);
            }
        }

        void load() override
        {
            SaveFolder::load();

        }
    private:

        void _saveSceneIndex(const std::vector<ecs::SceneID>& sceneIDs)
        {
            json sceneIndices;
            for (const auto &sceneID : sceneIDs) {
                sceneIndices.push_back({"scene" + std::to_string(sceneID), sceneID});
            }
            json sceneIndex = createJsonWithMagic(
                INDEX_FILE_MAGIC,
                sceneIndices
            );
            std::ofstream sceneIndexFile(getPath() / "scene_index.bin", std::ios::binary);
            json::to_msgpack(sceneIndex, sceneIndexFile);
        }

        void _saveScene(ecs::SceneID sceneID)
        {
            std::filesystem::path scenePath = getPath() / ("scene" + std::to_string(sceneID));
            std::filesystem::create_directory(scenePath);

            _saveEntities(sceneID, scenePath);

        }

        void _saveEntities(ecs::SceneID sceneID, std::filesystem::path& scenePath)
        {
            json entitiesData = createJsonWithMagic(
				ENTITIES_FILE_MAGIC,
                engine::getSceneManager().getSceneEntities(sceneID)
			);
            std::ofstream entitiesFile(scenePath / "entities.bin", std::ios::binary);
			json::to_msgpack(entitiesData, entitiesFile);
        }

		void _saveCameras(ecs::SceneID sceneID, std::filesystem::path& scenePath)
		{
			/*json camerasData = engine::getSceneManager().getSceneCameras(sceneID);
			std::ofstream camerasFile(scenePath / "cameras.bin", std::ios::binary);*/
        }
    };

} // namespace engine::editor
