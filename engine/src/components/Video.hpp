//// Name.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/06/2025
//  Description: Header file for the name component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "assets/Assets/Material/Material.hpp"

namespace nexo::components {
    struct VideoComponent {
        std::string path;
        std::vector<assets::AssetRef<assets::Material>> frames;
        float frameRate          = 30.0f; // Frames per second
        size_t currentFrameIndex = 0;
        size_t nbFrame;

        [[nodiscard]] bool loadVideoFrames(const std::string& videoPath);
    };
} // namespace nexo::components
