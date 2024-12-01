//// EntityFactory2D.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/11/2024
//  Description: Header file for the 2D entity factory
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>

#include "ecs/Entity.hpp"

namespace nexo {

    class EntityFactory2D {
        public:
            static ecs::Entity createQuad(glm::vec3 pos, glm::vec2 size, float rotation, glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f});
    };
}