//// Matrix.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/11/2024
//  Description: Source file for the matrixes utilitary functions
//
///////////////////////////////////////////////////////////////////////////////
#include "Matrix.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace nexo::math {
    void decomposeTransformEuler(const glm::mat4 &mat, glm::vec3 &outTranslation, glm::vec3 &outRotation, glm::vec3 &outScale)
    {
        // Extract translation
        outTranslation = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);

        // Extract scale (length of each column)
        outScale.x = glm::length(glm::vec3(mat[0][0], mat[0][1], mat[0][2]));
        outScale.y = glm::length(glm::vec3(mat[1][0], mat[1][1], mat[1][2]));
        outScale.z = glm::length(glm::vec3(mat[2][0], mat[2][1], mat[2][2]));

        // Normalize rows to remove scale
        glm::mat3 rotationMatrix = glm::mat3(mat);
        if (outScale.x != 0) rotationMatrix[0] /= outScale.x;
        if (outScale.y != 0) rotationMatrix[1] /= outScale.y;
        if (outScale.z != 0) rotationMatrix[2] /= outScale.z;

        // Check for negative scale using the cross product of the first two rows
        glm::vec3 crossProduct = glm::cross(glm::vec3(rotationMatrix[0]), glm::vec3(rotationMatrix[1]));
        if (glm::dot(crossProduct, glm::vec3(rotationMatrix[2])) < 0) {
            outScale.x *= -1;
            rotationMatrix[0] = -rotationMatrix[0];
        }

        // Extract Euler angles (rotation)
        outRotation.x = atan2(rotationMatrix[1][2], rotationMatrix[2][2]); // Rotation around X-axis
        float c2 = sqrt(rotationMatrix[0][0] * rotationMatrix[0][0] + rotationMatrix[0][1] * rotationMatrix[0][1]);
        outRotation.y = atan2(-rotationMatrix[0][2], c2);                  // Rotation around Y-axis
        outRotation.z = atan2(rotationMatrix[0][1], rotationMatrix[0][0]); // Rotation around Z-axis
    }
}