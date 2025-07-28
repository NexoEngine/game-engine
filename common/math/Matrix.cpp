//// Matrix.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/11/2024
//  Description: Source file for the matrixes utilitary functions
//
///////////////////////////////////////////////////////////////////////////////
#include "Matrix.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace nexo::math {
    void decomposeTransformEuler(const glm::mat4 &mat, glm::vec3 &outTranslation, glm::vec3 &outRotation,
                                 glm::vec3 &outScale)
    {
        // Extract translation
        outTranslation = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);

        // Extract scale (length of each column)
        outScale.x = glm::length(glm::vec3(mat[0][0], mat[0][1], mat[0][2]));
        outScale.y = glm::length(glm::vec3(mat[1][0], mat[1][1], mat[1][2]));
        outScale.z = glm::length(glm::vec3(mat[2][0], mat[2][1], mat[2][2]));

        // Normalize rows to remove scale
        auto rotationMatrix = glm::mat3(mat);
        if (outScale.x != 0) rotationMatrix[0] /= outScale.x;
        if (outScale.y != 0) rotationMatrix[1] /= outScale.y;
        if (outScale.z != 0) rotationMatrix[2] /= outScale.z;

        // Check for negative scale using the cross product of the first two rows
        if (const glm::vec3 crossProduct = glm::cross(glm::vec3(rotationMatrix[0]), glm::vec3(rotationMatrix[1]));
            glm::dot(crossProduct, glm::vec3(rotationMatrix[2])) < 0)
        {
            outScale.x *= -1;
            rotationMatrix[0] = -rotationMatrix[0];
        }

        // Extract Euler angles (rotation)
        outRotation.x = static_cast<float>(atan2(rotationMatrix[1][2], rotationMatrix[2][2])); // Rotation around X-axis
        const auto c2 = static_cast<float>(sqrt(
            rotationMatrix[0][0] * rotationMatrix[0][0] + rotationMatrix[0][1] * rotationMatrix[0][1]));
        outRotation.y = static_cast<float>(atan2(-rotationMatrix[0][2], c2)); // Rotation around Y-axis
        outRotation.z = static_cast<float>(atan2(rotationMatrix[0][1], rotationMatrix[0][0])); // Rotation around Z-axis
    }

    void decomposeTransformQuat(const glm::mat4 &mat, glm::vec3 &outTranslation, glm::quat &outRotation, glm::vec3 &outScale)
    {
        // Extract translation
        outTranslation = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);

        // Extract scale (length of each column vector)
        outScale.x = glm::length(glm::vec3(mat[0][0], mat[0][1], mat[0][2]));
        outScale.y = glm::length(glm::vec3(mat[1][0], mat[1][1], mat[1][2]));
        outScale.z = glm::length(glm::vec3(mat[2][0], mat[2][1], mat[2][2]));

        // Extract rotation matrix and remove scaling by normalizing the basis vectors.
        glm::mat3 rotationMatrix = mat;
        if (outScale.x != 0) rotationMatrix[0] /= outScale.x;
        if (outScale.y != 0) rotationMatrix[1] /= outScale.y;
        if (outScale.z != 0) rotationMatrix[2] /= outScale.z;

        // Check for negative scale using the cross product of the first two rows.
        // If the dot product between the cross product and the third row is negative,
        // we have a negative scale, so we fix it.
        if (const glm::vec3 crossProduct = glm::cross(glm::vec3(rotationMatrix[0]), glm::vec3(rotationMatrix[1]));
            glm::dot(crossProduct, glm::vec3(rotationMatrix[2])) < 0)
        {
            outScale.x *= -1;
            rotationMatrix[0] = -rotationMatrix[0];
        }

        outRotation = glm::normalize(glm::quat_cast(rotationMatrix));
    }
}
