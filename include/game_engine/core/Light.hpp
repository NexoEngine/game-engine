/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** Light
*/

#pragma once

#include "my_raylib.h"

#include <vector>

namespace engine {
    namespace core {

        #define MAX_LIGHTS 8

        using LightId = int;

        enum LightType {
            DIRECTIONAL,
            POINT
        };

        struct Light {
            LightId id;
            bool enabled;
            LightType type;
            Vector3 position;
            Vector3 target;
            Color color;

            // Shader locations
            int enabledLoc;
            int typeLoc;
            int positionLoc;
            int targetLoc;
            int colorLoc;
            int attenuationLoc;
        };

        inline std::vector<Light> lights;
        inline int nextLightID = 0;

        
    }
}