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

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <array>

constexpr unsigned int MAX_POINT_LIGHTS = 8;
constexpr unsigned int MAX_SPOT_LIGHTS = 8;

namespace nexo::components {

    struct AmbientLightComponent {
        glm::vec3 color{};
    };

    struct DirectionalLightComponent {
    	DirectionalLightComponent() = default;
        explicit DirectionalLightComponent(const glm::vec3 &lightDirection,
                                  		   const glm::vec3 &lightColor = {1.0f, 1.0f, 1.0f}) :
        	direction(lightDirection), color(lightColor) {};

        glm::vec3 direction{};
        glm::vec3 color{};
    };

    struct PointLightComponent {
    	PointLightComponent() = default;
        explicit PointLightComponent(const glm::vec3 lightPos,
		        					const glm::vec3 &lightColor = {1.0f, 1.0f, 1.0f},
							        const float linear = 0.09f,
							        const float quadratic = 0.032f) :
            pos(lightPos), color(lightColor),
            linear(linear), quadratic(quadratic) {};

        glm::vec3 pos{};
        glm::vec3 color{};
        float maxDistance = 50.0f;
        float constant = 1.0f;
        float linear{};
        float quadratic{};
    };

    struct SpotLightComponent {
    	SpotLightComponent() = default;
    	explicit SpotLightComponent(glm::vec3 lightPos,
     							   glm::vec3 lightDir,
             					   glm::vec3 lightColor,
                   				   float cutOff,
                          		   float outerCutoff,
                          		   float linear = 0.0014f,
                          		   float quadractic = 0.0007f) :
     		pos(lightPos), color(lightColor),
       		direction(lightDir), cutOff(cutOff),
         	outerCutoff(outerCutoff), linear(linear),
          	quadratic(quadractic) {}

     	glm::vec3 pos{};
      	glm::vec3 color{};
      	glm::vec3 direction{};
        float maxDistance = 325.0f;
       	float cutOff{};
       	float outerCutoff{};

        float constant = 1.0f;
        float linear{};
        float quadratic{};
    };

    struct LightContext {
        glm::vec3 ambientLight;
        std::array<PointLightComponent, MAX_POINT_LIGHTS> pointLights;
        unsigned int pointLightCount = 0;
        std::array<SpotLightComponent, MAX_SPOT_LIGHTS> spotLights;
        unsigned int spotLightCount = 0;
        DirectionalLightComponent dirLight;
    };
}
