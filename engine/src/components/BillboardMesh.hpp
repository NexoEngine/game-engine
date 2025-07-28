//// BillboardMesh.hpp ////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/05/2025
//  Description: Header file for the billboard mesh component, we have
//               to handle it separately because it has to be oriented
//               facing the camera
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "VertexArray.hpp"
#include <glm/glm.hpp>

namespace nexo::components {
    enum class BillboardType {
        FULL,       // Always faces camera completely (like particles)
        AXIS_Y,     // Rotates only around Y axis (like trees)
        AXIS_CUSTOM // Rotates around a custom axis
    };

    struct BillboardComponent {
        BillboardType type = BillboardType::FULL;
        glm::vec3 axis = {0.0f, 1.0f, 0.0f}; // For AXIS_CUSTOM type
        std::shared_ptr<renderer::NxVertexArray> vao;
    };
}
