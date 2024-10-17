/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** LightHandling
*/

#include "game_engine/LightHandling.hpp"
#include "game_engine/GameEngine.hpp"

namespace engine {

    engine::core::LightId Engine::createLight(engine::core::LightType type, Vector3 position, Vector3 target, Color color, Shader lightingShader)
    {
        if (engine::core::lights.size() >= MAX_LIGHTS) {
            std::cout << "Max number of lights reached!" << std::endl;
            return -1;
        }

        engine::core::Light newLight;
        newLight.id = engine::core::nextLightID;
        newLight.enabled = 1;
        newLight.type = type;
        newLight.position = position;
        newLight.target = target;
        newLight.color = color;

        engine::core::lights.push_back(newLight);

        newLight.enabledLoc = GetShaderLocation(lightingShader, TextFormat("lights[%i].enabled", engine::core::nextLightID));
        newLight.typeLoc = GetShaderLocation(lightingShader, TextFormat("lights[%i].type", engine::core::nextLightID));
        newLight.positionLoc = GetShaderLocation(lightingShader, TextFormat("lights[%i].position", engine::core::nextLightID));
        newLight.targetLoc = GetShaderLocation(lightingShader, TextFormat("lights[%i].target", engine::core::nextLightID));
        newLight.colorLoc = GetShaderLocation(lightingShader, TextFormat("lights[%i].color", engine::core::nextLightID));

        int enabled = (newLight.enabled) ? 1 : 0;
        int lightType = (newLight.type == engine::core::DIRECTIONAL) ? 0 : 1;
        SetShaderValue(lightingShader, newLight.enabledLoc, &enabled, SHADER_UNIFORM_INT);
        SetShaderValue(lightingShader, newLight.typeLoc, &lightType, SHADER_UNIFORM_INT);

        float pos[3] = { newLight.position.x, newLight.position.y, newLight.position.z };
        SetShaderValue(lightingShader, newLight.positionLoc, pos, SHADER_UNIFORM_VEC3);

        float tar[3] = { newLight.target.x, newLight.target.y, newLight.target.z };
        SetShaderValue(lightingShader, newLight.targetLoc, tar, SHADER_UNIFORM_VEC3);

        float col[4] = { (float)newLight.color.r/(float)255, (float)newLight.color.g/(float)255, 
                        (float)newLight.color.b/(float)255, (float)newLight.color.a/(float)255 };
        SetShaderValue(lightingShader, newLight.colorLoc, col, SHADER_UNIFORM_VEC4);

        engine::core::nextLightID++;
        return newLight.id; 
    }

    engine::core::LightId createLight(engine::core::LightType type, Vector3 position, Vector3 target, Color color, Shader lightingShader)
    {
        return Engine::getInstance()->createLight(type, position, target, color, lightingShader);
    }

    void Engine::setLightEnabled(engine::core::LightId lightID, bool enabled, Shader lightingShader)
    {
        for (engine::core::Light& light : engine::core::lights) {
            if (light.id == lightID) {
                light.enabled = enabled ? 1 : 0;
                for (int i = 0; i < engine::core::lights.size(); i++) {
                    std::string enabledStr = "lights[" + std::to_string(i) + "].enabled";
                    std::string typeStr = "lights[" + std::to_string(i) + "].type";
                    std::string posStr = "lights[" + std::to_string(i) + "].position";
                    std::string colorStr = "lights[" + std::to_string(i) + "].color";

                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, enabledStr.c_str()), &engine::core::lights[i].enabled, SHADER_UNIFORM_INT);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, typeStr.c_str()), &engine::core::lights[i].type, SHADER_UNIFORM_INT);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, posStr.c_str()), &engine::core::lights[i].position, SHADER_UNIFORM_VEC3);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, colorStr.c_str()), &engine::core::lights[i].color, SHADER_UNIFORM_VEC4);
                }
                break;
            }
        }
    }

    void setLightEnabled(engine::core::LightId lightID, bool enabled, Shader lightingShader)
    {
        Engine::getInstance()->setLightEnabled(lightID, enabled, lightingShader);
    }

    void Engine::setLightColor(engine::core::LightId lightID, Color newColor, Shader lightingShader)
    {
        for (engine::core::Light& light : engine::core::lights) {
            if (light.id == lightID) {
                light.color = newColor;
                for (int i = 0; i < engine::core::lights.size(); i++) {
                    std::string enabledStr = "lights[" + std::to_string(i) + "].enabled";
                    std::string typeStr = "lights[" + std::to_string(i) + "].type";
                    std::string posStr = "lights[" + std::to_string(i) + "].position";
                    std::string colorStr = "lights[" + std::to_string(i) + "].color";

                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, enabledStr.c_str()), &engine::core::lights[i].enabled, SHADER_UNIFORM_INT);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, typeStr.c_str()), &engine::core::lights[i].type, SHADER_UNIFORM_INT);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, posStr.c_str()), &engine::core::lights[i].position, SHADER_UNIFORM_VEC3);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, colorStr.c_str()), &engine::core::lights[i].color, SHADER_UNIFORM_VEC4);
                }
                break;
            }
        }
    }

    void setLightColor(engine::core::LightId lightID, Color newColor, Shader lightingShader)
    {
        Engine::getInstance()->setLightColor(lightID, newColor, lightingShader);
    }
}