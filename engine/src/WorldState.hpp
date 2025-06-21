//// WorldState.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        21/06/2025
//  Description: Header file for the WorldState class,
//      which manages the state of the world in the game engine
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Application.hpp"

namespace nexo {

    struct WorldState {
        struct WorldTime {
            double deltaTime = 0.0; // Time since last update
            double totalTime = 0.0; // Total time since the start of the world
        } time;

        struct WorldStats {
            int frameCount = 0; // Number of frames rendered
        } stats;
    };

} // namespace nexo::scripting