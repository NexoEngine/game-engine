//// OrthographicCamera.cpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the orthographic camera class
//
///////////////////////////////////////////////////////////////////////////////
#include "OrthographicCamera.hpp"
#include "Logger.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace nexo::camera {

    OrthographicCamera::OrthographicCamera(const float left, const float right, const float bottom, const float top)
    {
        LOG(NEXO_DEBUG, "OrthographicCamera: new orthographic camera created");
        m_projectionMatrix = glm::ortho(left, right, bottom, top);
        m_viewMatrix = glm::mat4(1.0f);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        m_mode = CameraMode::ORTHOGRAPHIC;
        m_id = nextCameraId++;
    }

    void OrthographicCamera::setProjection(const float left, const float right, const float bottom, const float top)
    {
        m_projectionMatrix = glm::ortho(left, right, bottom, top);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

}
