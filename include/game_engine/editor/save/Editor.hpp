//// Editor.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Definition of engine folder wrapper.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "game_engine/save/filesystem/SaveFolder.hpp"
#include "game_engine/save/filesystem/SaveFile.hpp"

#include "game_engine/editor/save/EditorLayout.hpp"
#include "game_engine/editor/save/Scenes.hpp"
#include "game_engine/editor/save/Entities.hpp"
#include "game_engine/editor/save/Cameras.hpp"

namespace engine::editor {
    class EditorSaveFolder : public save::SaveFolder {
        public:
        EditorSaveFolder()
        {
            setPath(".");


            save::ScenesSaveFolder scenesSaveFolder;
            SaveFolder engineSaveFolder = {
				{"objects", {
					{"entities.json", save::EntitiesSaveFile()},
                    {"cameras.json", save::CamerasSaveFile()},
				}},
                {"scenes", scenesSaveFolder},
                {"assets", {
                    {"models.nbin", save::EmptySaveFile()},
                    {"textures.nbin", save::EmptySaveFile()},
                    {"sounds.nbin", save::EmptySaveFile()},
                    {"fonts.nbin", save::EmptySaveFile()},
                    {"shaders.nbin", save::EmptySaveFile()},
                }},
                {"config.json", save::EmptySaveFile()},
                {"project.json", save::EmptySaveFile()},
                {"editor_layout.ini", save::EditorLayoutSaveFile()},
                {"README.md", save::PlaceholderSaveFile(
                    "Most of these folders and files are placeholders, "
                    "only scenes and entities are loaded for now.")}
            };

            addFolder(".nexo", engineSaveFolder);
        }
    };
} // namespace engine::editor
