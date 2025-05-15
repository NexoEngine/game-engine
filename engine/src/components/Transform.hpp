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

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace nexo::components {

    struct TransformComponent final {
        struct Memento {
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;
        };

        void restore(const Memento &memento)
        {
            pos = memento.position;
            quat = memento.rotation;
            size = memento.scale;
        }

        [[nodiscard]] Memento save() const
        {
            return {pos, quat, size};
        }

        glm::vec3 pos;
        glm::vec3 size = glm::vec3(1.0f);
        glm::quat quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    };

    struct LocalTransformComponent {
        struct Memento {
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;
        };

        void restore(const Memento &memento)
        {
            position = memento.position;
            rotation = memento.rotation;
            scale = memento.scale;
        }

        [[nodiscard]] Memento save() const
        {
            return {position, rotation, scale};
        }


        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    };

}
