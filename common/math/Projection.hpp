//// Projection.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/04/2025
//  Description: Header file for the math projection utils
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>

namespace nexo::math {
    const glm::vec3 projectRayToWorld(float x, float y,
                                    const glm::mat4 &viewProjectionMatrix,
                                    const glm::vec3 &cameraPosition,
                                    unsigned int width, unsigned int height
    );
}
