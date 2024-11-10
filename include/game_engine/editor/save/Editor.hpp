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

namespace engine::editor {
    class EditorSaveFolder : public save::SaveFolder {
        public:
        EditorSaveFolder()
        {
            setPath(".");

            save::EmptySaveFile entities;

            SaveFolder engineSaveFolder = {
                {"entities", {
                    {"entities.json", entities}
                }},
                {"scenes", {
                    {"mainMenu", {
                        {"entities.json", save::EmptySaveFile()},
                        {"config.json", save::EmptySaveFile()},
                        {"assets.nbin", save::EmptySaveFile()},
                    }},
                    {"level1", {
                        {"entities.json", save::EmptySaveFile()},
                        {"config.json", save::EmptySaveFile()},
                        {"assets.nbin", save::EmptySaveFile()},
                    }}
                }},
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
                    "only entities are loaded for now.")}
            };

            addFolder(".nexo", engineSaveFolder);
        }
    };
} // namespace engine::editor
