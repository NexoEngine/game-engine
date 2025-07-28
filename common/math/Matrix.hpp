//// Matrix.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/11/2024
//  Description: Header file for the matrixes utilitary functions
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>

namespace nexo::math {
	/**
     * @brief Decomposes a 4x4 transformation matrix into translation, Euler rotation, and scale components.
     *
     * This function extracts the translation vector, calculates the scale by measuring the length of
     * the matrix columns, and removes the scale from the rotation part. Then it computes Euler angles
     * (in radians) representing the rotation.
     *
     * @param mat The transformation matrix to decompose.
     * @param[out] outTranslation The extracted translation vector.
     * @param[out] outRotation The extracted rotation as Euler angles (radians).
     * @param[out] outScale The extracted scale vector.
     */
    void decomposeTransformEuler(const glm::mat4 &mat, glm::vec3 &outTranslation, glm::vec3 &outRotation, glm::vec3 &outScale);

    /**
     * @brief Decomposes a 4x4 transformation matrix into translation, quaternion rotation, and scale components.
     *
     * This function extracts the translation vector and scale from the transformation matrix.
     * The rotation part is isolated by normalizing the matrix basis vectors, then converted into
     * a normalized quaternion.
     *
     * @param mat The transformation matrix to decompose.
     * @param[out] outTranslation The extracted translation vector.
     * @param[out] outRotation The extracted rotation as a quaternion.
     * @param[out] outScale The extracted scale vector.
     */
    void decomposeTransformQuat(const glm::mat4 &mat, glm::vec3 &outTranslation, glm::quat &outRotation, glm::vec3 &outScale);
}
