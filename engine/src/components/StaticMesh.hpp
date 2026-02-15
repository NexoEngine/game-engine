//// StaticMesh.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/05/2025
//  Description: Header file for the static mesh component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/Attributes.hpp"
#include "renderer/VertexArray.hpp"

#include <glm/glm.hpp>

namespace nexo::components {

    struct StaticMeshComponent {
        std::shared_ptr<renderer::NxVertexArray> vao;

        renderer::RequiredAttributes meshAttributes;

        bool hasBounds = false;
        glm::vec3 localMin = {0.0f, 0.0f, 0.0f};
        glm::vec3 localMax = {0.0f, 0.0f, 0.0f};

        struct Memento {
            std::shared_ptr<renderer::NxVertexArray> vao;
            bool hasBounds;
            glm::vec3 localMin;
            glm::vec3 localMax;
        };

        void restore(const Memento &memento)
        {
            vao = memento.vao;
            hasBounds = memento.hasBounds;
            localMin = memento.localMin;
            localMax = memento.localMax;
        }

        [[nodiscard]] Memento save() const
        {
            return {vao, hasBounds, localMin, localMax};
        }
    };

}
