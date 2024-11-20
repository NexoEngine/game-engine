//// PerspectiveCamera.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Source file for the perspective camera class
//
///////////////////////////////////////////////////////////////////////////////
#include "PerspectiveCamera.hpp"
#include "Logger.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace nexo::camera {

    PerspectiveCamera::PerspectiveCamera(const float fovY, const float aspectRatio, const float nearClip, const float farClip)
        : m_fovY(fovY), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip)
    {
        LOG(NEXO_DEBUG, "PerspectiveCamera: new perspective camera created");
        setProjection(fovY, aspectRatio, nearClip, farClip);
        m_viewMatrix = glm::mat4(1.0f);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        m_mode = CameraMode::PERSPECTIVE;
        m_id = nextCameraId++;
    }

    void PerspectiveCamera::setProjection(const float fovY, const float aspectRatio, const float nearClip, const float farClip)
    {
        m_fovY = fovY;
        m_aspectRatio = aspectRatio;
        m_nearClip = nearClip;
        m_farClip = farClip;

        m_projectionMatrix = glm::perspective(fovY, aspectRatio, nearClip, farClip);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    void PerspectiveCamera::setAspectRatio(const float aspectRatio)
    {
        m_aspectRatio = aspectRatio;
        setProjection(m_fovY, m_aspectRatio, m_nearClip, m_farClip);
    }

}