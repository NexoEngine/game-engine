//// Projection.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/04/2025
//  Description: Header file for the math projection utils
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>

namespace nexo::math {
    glm::vec3 projectRayToWorld(float x, float y,
                                    const glm::mat4 &viewProjectionMatrix,
                                    const glm::vec3 &cameraPosition,
                                    unsigned int width, unsigned int height
    );
}
