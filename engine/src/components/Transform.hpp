//// Transform.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the transform related components
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/Definitions.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

namespace nexo::components {

    struct TransformComponent final {
        struct Memento {
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;

            glm::mat4 localMatrix;
            glm::vec3 localCenter;

            std::vector<ecs::Entity> children;
        };

        void restore(const Memento &memento);
        [[nodiscard]] Memento save() const;

        void addChild(ecs::Entity childEntity);
        void removeChild(ecs::Entity childEntity);

        glm::vec3 pos;
        glm::vec3 size = glm::vec3(1.0f);
        glm::quat quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

        glm::mat4 worldMatrix = glm::mat4(1.0f);
        glm::mat4 localMatrix = glm::mat4(1.0f);

        glm::vec3 localCenter = {0.0f, 0.0f, 0.0f};

        std::vector<ecs::Entity> children{};
    };
}
