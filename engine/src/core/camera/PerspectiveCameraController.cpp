//// PerspectiveCameraController.cpp //////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Source file for the perspective camera controller
//
///////////////////////////////////////////////////////////////////////////////

#include "PerspectiveCameraController.hpp"
#include "core/event/Input.hpp"
#include "core/event/KeyCodes.hpp"
#include "Application.hpp"
#include "Logger.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

namespace nexo::camera {

    PerspectiveCameraController::PerspectiveCameraController(const float aspectRatio, const float fovY)
        : m_aspectRatio(aspectRatio),
          m_fovY(fovY),
          m_camera(fovY, aspectRatio, 0.1f, 100.0f)
    {
        LOG(NEXO_DEBUG, "PerspectiveCameraController: new perspective camera controller created");
        Application::getInstance().getEventManager()->registerListener<event::EventWindowResize>(this);
        Application::getInstance().getEventManager()->registerListener<event::EventMouseScroll>(this);
        Application::getInstance().getEventManager()->registerListener<event::EventMouseMove>(this);
        m_lastMousePosition = event::getMousePosition();
    }

    void PerspectiveCameraController::onUpdate(const Timestep ts)
    {
        const float time = ts;

        if (event::isKeyPressed(NEXO_KEY_Z))
            m_cameraPosition += m_cameraFront * m_cameraTranslationSpeed * time; // Forward
        if (event::isKeyPressed(NEXO_KEY_S))
            m_cameraPosition -= m_cameraFront * m_cameraTranslationSpeed * time; // Backward
        if (event::isKeyPressed(NEXO_KEY_Q))
            m_cameraPosition -= m_cameraRight * m_cameraTranslationSpeed * time; // Left
        if (event::isKeyPressed(NEXO_KEY_D))
            m_cameraPosition += m_cameraRight * m_cameraTranslationSpeed * time; // Right
        if (event::isKeyPressed(NEXO_KEY_SPACE))
            m_cameraPosition += m_cameraUp * m_cameraTranslationSpeed * time;    // Up
        if (event::isKeyPressed(NEXO_KEY_TAB))
            m_cameraPosition -= m_cameraUp * m_cameraTranslationSpeed * time;    // Down

        m_camera.update(m_cameraPosition, m_cameraYaw, m_cameraPitch);
    }

    void PerspectiveCameraController::handleEvent(event::EventMouseScroll &event)
    {
        if (!zoomOn)
            return;

        constexpr float zoomSpeed = 0.5f;
        m_cameraPosition += m_cameraFront * event.y * zoomSpeed;

        m_camera.setPosition(m_cameraPosition);

        event.consumed = true;
    }

    void PerspectiveCameraController::handleEvent(event::EventWindowResize &event)
    {
        m_aspectRatio = static_cast<float>(event.width) / static_cast<float>(event.height);
        m_camera.setProjection(m_fovY, m_aspectRatio, 0.1f, 100.0f);
        event.consumed = true;
    }

    void PerspectiveCameraController::handleEvent(event::EventMouseMove &event)
    {
        glm::vec2 currentMousePosition(event.x, event.y);
        glm::vec2 mouseDelta = (currentMousePosition - m_lastMousePosition) * m_mouseSensitivity;
        m_lastMousePosition = currentMousePosition;

        m_cameraYaw += mouseDelta.x;
        m_cameraPitch -= mouseDelta.y;

        m_cameraPitch = glm::clamp(m_cameraPitch, -89.0f, 89.0f);

        glm::vec3 front;
        front.x = cos(glm::radians(m_cameraYaw)) * cos(glm::radians(m_cameraPitch));
        front.y = sin(glm::radians(m_cameraPitch));
        front.z = sin(glm::radians(m_cameraYaw)) * cos(glm::radians(m_cameraPitch));
        m_cameraFront = glm::normalize(front);

        m_cameraRight = glm::normalize(glm::cross(m_cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
        m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
    }

}


