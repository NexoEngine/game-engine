/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** LightHandling
*/

#pragma once

#include "game_engine/core/Light.hpp"
#include "game_engine/ecs/components/Shader.hpp"
#include "raylib.h"

namespace engine {
    engine::core::LightId createLight(engine::core::LightType type, Vector3 position, Vector3 target, Color color, Shader lightingShader = ecs::components::shader::defaultLightingShader);
    void setLightEnabled(engine::core::LightId lightID, bool enabled, Shader lightingShader = ecs::components::shader::defaultLightingShader);
    void setLightColor(engine::core::LightId lightID, Color newColor, Shader lightingShader = ecs::components::shader::defaultLightingShader);
}