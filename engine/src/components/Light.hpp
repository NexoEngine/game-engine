//// Light.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/11/2024
//  Description: Header file for the light
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#define MAX_DIRECTIONAL_LIGHTS 8
#define MAX_POINT_LIGHTS 8

namespace nexo::components {
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT,
    };

    struct Light {
        Light() = default;

        Light(const LightType lightType, const glm::vec4 &lightColor, const float lightIntensity) : type(lightType),
                                                                                                    color(lightColor),
                                                                                                    intensity(
                                                                                                        lightIntensity)
        {};

        ~Light() = default;

        LightType type = LightType::DIRECTIONAL;
        glm::vec4 color{};
        float intensity{};
    };

    struct DirectionalLight : Light {
        explicit DirectionalLight(const glm::vec3 &lightDirection,
                                  const glm::vec4 &lightColor = {1.0f, 1.0f, 1.0f, 1.0f},
                                  const float &lightIntensity = 0.5f) : Light(LightType::DIRECTIONAL, lightColor,
                                                                              lightIntensity),
                                                                        direction(lightDirection) {};

        glm::vec3 direction{};
    };

    struct PointLight : Light {
        explicit PointLight(glm::vec3 lightPos, const glm::vec4 &lightColor = {1.0f, 1.0f, 1.0f, 1.0f},
                   const float &lightIntensity = 0.5f) : Light(LightType::POINT, lightColor, lightIntensity),
                                                         pos(lightPos) {};

        glm::vec3 pos{};
    };
}
