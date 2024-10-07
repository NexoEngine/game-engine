/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** LightHandling
*/

#include "game_engine/LightHandling.hpp"
#include "game_engine/GameEngine.hpp"

namespace engine {
    engine::core::LightId createLight(engine::core::LightType type, Vector3 position, Vector3 target, Color color, Shader lightingShader)
    {
        return Engine::getInstance()->createLight(type, position, target, color, lightingShader);
    }

    void setLightEnabled(engine::core::LightId lightID, bool enabled, Shader lightingShader)
    {
        Engine::getInstance()->setLightEnabled(lightID, enabled, lightingShader);
    }

    void setLightColor(engine::core::LightId lightID, Color newColor, Shader lightingShader)
    {
        Engine::getInstance()->setLightColor(lightID, newColor, lightingShader);
    }
}