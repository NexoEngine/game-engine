//// Projection.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/04/2025
//  Description: Source file for the math projection utils
//
///////////////////////////////////////////////////////////////////////////////

#include "Projection.hpp"

namespace nexo::math {

    glm::vec3 projectRayToWorld(const float x, const float y,
                                    const glm::mat4 &viewProjectionMatrix,
                                    const glm::vec3 &cameraPosition,
                                    const unsigned int width, const unsigned int height
    ) {
        // Convert to NDC
        const float ndcX = (2.0f * x) / static_cast<float>(width) - 1.0f;
        const float ndcY = 1.0f - (2.0f * y) / static_cast<float>(height);

        const glm::mat4 inverseViewProj = glm::inverse(viewProjectionMatrix);

        // Points in NDC space at near and far planes
        glm::vec4 nearPoint = inverseViewProj * glm::vec4(ndcX, ndcY, -1.0f, 1.0f);

        nearPoint /= nearPoint.w;

        const glm::vec3 rayDir = glm::normalize(glm::vec3(nearPoint) - cameraPosition);

        return rayDir;
    }
}
