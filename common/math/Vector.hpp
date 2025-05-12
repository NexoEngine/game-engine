//// Vector.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/03/2025
//  Description: Header file for the vectors utilities functions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace nexo::math {
	/**
     * @brief Extracts camera orientation components from a rotation vector
     *
     * Converts Euler angles (rotation) into three orthogonal vectors that define
     * the camera's orientation in 3D space.
     *
     * @param[in] rotation The camera's rotation as Euler angles in degrees (pitch, yaw, roll)
     * @param[out] front The forward direction vector of the camera
     * @param[out] right The right direction vector of the camera
     * @param[out] up The up direction vector of the camera
     */
	void extractCameraComponents(const glm::vec3 &rotation, glm::vec3 &front, glm::vec3 &right, glm::vec3 &up);

	/**
	* @brief Converts a quaternion to Euler angles in degrees
	*
	* This function converts a unit quaternion to Euler angles (pitch, yaw, roll)
	*
	* @param[in] q The input quaternion to convert (assumed to be normalized)
	* @return glm::vec3 Euler angles in degrees in XYZ order (pitch, yaw, roll)
	*
	* @note This implementation follows a specific conversion order that matches
	*       the project's requirements. It converts radians to degrees before returning.
	* @note Special handling is implemented for the singularity case (gimbal lock)
	*       when sinp approaches ±1.
	*/
	glm::vec3 customQuatToEuler(const glm::quat &q);

	bool isPosInBounds(const glm::vec2 pos, const glm::vec2 &min, const glm::vec2 &max);
}
