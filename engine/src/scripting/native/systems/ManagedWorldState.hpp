//// WorldState.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        21/06/2025
//  Description: Header file for the WorldState class,
//      which manages the state of the world in the game engine
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Application.hpp"

#include "scripting/native/ManagedTypedef.hpp"

namespace nexo::scripting {

    /**
     * @brief Represents the state of the world in the game engine.
     * This structure is 1 to 1 equivalent to the WorldState class in the C# managed code.
     * @warning If you modify this structure, ensure that the C# managed code is updated accordingly.
     */
    struct ManagedWorldState {
        struct WorldTime {
            Double deltaTime = 0.0; // Time since last update
            Double totalTime = 0.0; // Total time since the start of the world
        } time;

        struct WorldStats {
            UInt64 frameCount = 0; // Number of frames rendered
        } stats;

        void update(const WorldState& worldState) {
            // Update the world state with the current time and stats
            time.deltaTime = worldState.time.deltaTime;
            time.totalTime = worldState.time.totalTime;
            stats.frameCount = worldState.stats.frameCount;
        }
    };

} // namespace nexo::scripting