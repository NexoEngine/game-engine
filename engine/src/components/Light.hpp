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

constexpr unsigned int MAX_DIRECTIONAL_LIGHTS = 8;
constexpr unsigned int MAX_POINT_LIGHTS = 8;
constexpr unsigned int MAX_SPOT_LIGHTS = 8;

namespace nexo::components {
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT,
    };

    struct Light {
        Light() = default;

        Light(const LightType lightType, const glm::vec4 &lightColor) : type(lightType),
                                                                        color(lightColor)
        {};

        ~Light() = default;

        LightType type = LightType::DIRECTIONAL;
        glm::vec4 color{};
    };

    struct DirectionalLight : Light {
        explicit DirectionalLight(const glm::vec3 &lightDirection,
                                  const glm::vec4 &lightColor = {1.0f, 1.0f, 1.0f, 1.0f}) : Light(LightType::DIRECTIONAL, lightColor),
                                                                        direction(lightDirection) {};

        glm::vec3 direction{};
    };

    struct PointLight : Light {
        explicit PointLight(glm::vec3 lightPos, const glm::vec4 &lightColor = {1.0f, 1.0f, 1.0f, 1.0f}, float linear = 0.09f, float quadratic = 0.032f) : Light(LightType::POINT, lightColor),
                                                         pos(lightPos), linear(linear), quadratic(quadratic) {};

        glm::vec3 pos{};
        float constant = 1.0f;
        float linear;
        float quadratic;
    };

    struct SpotLight : Light {
    	explicit SpotLight(glm::vec3 lightPos, glm::vec3 lightDir, glm::vec4 lightColor, float cutOff, float outerCutoff, float linear = 0.0014f, float quadractic = 0.000007f) : Light(LightType::SPOT, lightColor),
     						pos(lightPos), direction(lightDir), cutOff(cutOff), outerCutoff(outerCutoff), linear(linear), quadratic(quadractic) {}

     	glm::vec3 pos{};
      	glm::vec3 direction{};
       	float cutOff;
       	float outerCutoff;

        float constant = 1.0f;
        float linear;
        float quadratic;
    };
}
