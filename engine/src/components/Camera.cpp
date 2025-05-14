//// Camera.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/05/2025
//  Description: Source file for the camera component
//
///////////////////////////////////////////////////////////////////////////////

#include "Camera.hpp"

namespace nexo::components {
    [[nodiscard]] glm::mat4 CameraComponent::getProjectionMatrix() const
    {
        if (type == CameraType::PERSPECTIVE) {
            return glm::perspective(glm::radians(fov), static_cast<float>(width) / static_cast<float>(height), nearPlane, farPlane);
        }
        return glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, nearPlane, farPlane);
    }

    [[nodiscard]] glm::mat4 CameraComponent::getViewMatrix(const TransformComponent &transf) const
    {
        const glm::vec3 forward = transf.quat * glm::vec3(0.0f, 0.0f, -1.0f);
        const glm::vec3 upVec = transf.quat * glm::vec3(0.0f, 1.0f, 0.0f);
        return glm::lookAt(transf.pos, transf.pos + forward, upVec);
    }

    void CameraComponent::resize(const unsigned int newWidth, const unsigned int newHeight)
    {
        width = newWidth;
        height = newHeight;
        resizing = true;
        if (m_renderTarget) {
            m_renderTarget->resize(newWidth, newHeight);
        }
        pipeline.resize(newWidth, newHeight);
    }


    void CameraComponent::restore(const CameraComponent::Memento& memento)
    {
        if (width != memento.width || height != memento.height) {
            width = memento.width;
            height = memento.height;
            resize(width, height);
        }
        viewportLocked = memento.viewportLocked;
        fov = memento.fov;
        nearPlane = memento.nearPlane;
        farPlane = memento.farPlane;
        type = memento.type;
        clearColor = memento.clearColor;
        main = memento.main;
        m_renderTarget = memento.renderTarget;
    }

    [[nodiscard]] CameraComponent::Memento CameraComponent::save() const
    {
        return {
            width,
            height,
            viewportLocked,
            fov,
            nearPlane,
            farPlane,
            type,
            clearColor,
            main,
            m_renderTarget
        };
    }
}
