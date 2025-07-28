//// Light.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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

#include "ecs/Definitions.hpp"

constexpr unsigned int MAX_POINT_LIGHTS = 10;
constexpr unsigned int MAX_SPOT_LIGHTS = 10;

namespace nexo::components {

    struct AmbientLightComponent {
        glm::vec3 color{};

        struct Memento {
            glm::vec3 color;
        };

        void restore(const Memento& memento)
        {
            color = memento.color;
        }

        [[nodiscard]] Memento save() const
        {
            return {color};
        }


    };

    struct DirectionalLightComponent {
    	DirectionalLightComponent() = default;
        explicit DirectionalLightComponent(const glm::vec3 &lightDirection,
                                  		   const glm::vec3 &lightColor = {1.0f, 1.0f, 1.0f}) :
        	direction(lightDirection), color(lightColor) {};

        glm::vec3 direction{};
        glm::vec3 color{};

        struct Memento {
            glm::vec3 direction;
            glm::vec3 color;
        };

        void restore(const Memento& memento)
        {
            direction = memento.direction;
            color = memento.color;
        }

        [[nodiscard]] Memento save() const
        {
            return {direction, color};
        }
    };

    struct PointLightComponent {
        glm::vec3 color{};
        float linear{};
        float quadratic{};
        float maxDistance = 50.0f;
        float constant = 1.0f;

        struct Memento {
            glm::vec3 color{};
            float linear{};
            float quadratic{};
            float maxDistance;
            float constant;
        };

        void restore(const Memento& memento)
        {
            color = memento.color;
            linear = memento.linear;
            quadratic = memento.quadratic;
            maxDistance = memento.maxDistance;
            constant = memento.constant;
        }

        [[nodiscard]] Memento save() const
        {
            return {color, linear, quadratic, maxDistance, constant};
        }
    };

    struct SpotLightComponent {
        glm::vec3 direction{};
        glm::vec3 color{};
        float cutOff{};
        float outerCutoff{};
        float linear{};
        float quadratic{};
        float maxDistance = 325.0f;
        float constant = 1.0f;

        struct Memento {
            glm::vec3 direction{};
            glm::vec3 color{};
            float cutOff{};
            float outerCutoff{};
            float linear{};
            float quadratic{};
            float maxDistance;
            float constant;
        };

        void restore(const Memento& memento)
        {
            direction = memento.direction;
            color = memento.color;
            cutOff = memento.cutOff;
            outerCutoff = memento.outerCutoff;
            linear = memento.linear;
            quadratic = memento.quadratic;
            maxDistance = memento.maxDistance;
            constant = memento.constant;
        }

        [[nodiscard]] Memento save() const
        {
            return {direction, color, cutOff, outerCutoff, linear, quadratic, maxDistance, constant};
        }
    };

    struct LightContext {
        glm::vec3 ambientLight;
        std::array<ecs::Entity, MAX_POINT_LIGHTS> pointLights;
        unsigned int pointLightCount = 0;
        std::array<ecs::Entity, MAX_SPOT_LIGHTS> spotLights;
        unsigned int spotLightCount = 0;
        DirectionalLightComponent dirLight;
    };
}
