//// Cameras.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: SaveFile to save ECS cameras
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <GameEngine.hpp>

#include "game_engine/save/filesystem/SaveFolder.hpp"

#include <imgui.h>
#include <SaveHandling.hpp>
#include <SceneHandling.hpp>

namespace engine::save {

    class CamerasSaveFile : public SaveFile {
    private:
        void _saveImpl(std::ofstream& outFile) const override {

        }

        void _loadImpl(std::ifstream& inFile) override
        {

        }
    };

} // namespace engine::editor
