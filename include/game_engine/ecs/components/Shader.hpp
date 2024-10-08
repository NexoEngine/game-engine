/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** Shader
*/

#pragma once

#include "my_raylib.h"

namespace ecs {
    namespace components {
        namespace shader {
            enum ShaderType {
                SHADER_HDR,
                SHADER_BLOOM,
                SHADER_DOF,
                SHADER_MOTION_BLUR,
                SHADER_SSAO,
                SHADER_GAMMA_CORRECTION,
                SHADER_COUNT
            };

            #define DEFAULT_LIGHT_VS "src/game_engine/ressources/shaders/light.vs"
            #define DEFAULT_LIGHT_FS "src/game_engine/ressources/shaders/light.fs"

            inline Shader defaultLightingShader;
        }
    }
}
