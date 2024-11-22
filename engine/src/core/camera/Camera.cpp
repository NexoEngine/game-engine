//// Camera.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the camera class
//
///////////////////////////////////////////////////////////////////////////////

#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

nexo::camera::CameraId nexo::camera::Camera::nextCameraId = 0;

namespace nexo::camera {

    void Camera::recalculateViewMatrix()
    {
        const glm::mat4 rotationMatrix = glm::toMat4(m_rotation);
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
                                    rotationMatrix;

        m_viewMatrix = glm::inverse(transform);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    void Camera::rotate(const glm::vec3 &deltaRotation)
    {
        const auto deltaQuat = glm::quat(glm::radians(deltaRotation));
        m_rotation = glm::normalize(deltaQuat * m_rotation);
        recalculateViewMatrix();
    }

}
