//// OrthographicCameraController.cpp /////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the orthographic camera controller class
//
///////////////////////////////////////////////////////////////////////////////

#include "OrthographicCameraController.hpp"
#include "core/event/Input.hpp"
#include "core/event/KeyCodes.hpp"
#include "Application.hpp"
#include "Logger.hpp"

namespace nexo::camera {

    OrthographicCameraController::OrthographicCameraController(const float aspectRatio, const bool rotation)
        : m_aspectRatio(aspectRatio),
        m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel),
        m_rotationActivated(rotation)
    {
        LOG(NEXO_DEBUG, "OrthographicCameraController: new orthographic camera controller created");
        Application::getInstance().getEventManager()->registerListener<event::EventWindowResize>(this);
        Application::getInstance().getEventManager()->registerListener<event::EventMouseScroll>(this);
    }

    void OrthographicCameraController::onUpdate(Timestep ts)
    {
        const float time = ts;

        // Calculate the camera's direction vectors based on rotation
        const float radians = glm::radians(m_cameraRotation);
        const glm::vec3 rightDirection = {cos(radians), sin(radians), 0.0f};
        const glm::vec3 upDirection = {-rightDirection.y, rightDirection.x, 0.0f};

        if (event::isKeyPressed(NEXO_KEY_Z))
            m_cameraPosition -= upDirection * m_cameraTranslationSpeed * time; // Move up
        if (event::isKeyPressed(NEXO_KEY_S))
            m_cameraPosition += upDirection * m_cameraTranslationSpeed * time; // Move down
        if (event::isKeyPressed(NEXO_KEY_Q))
            m_cameraPosition += rightDirection * m_cameraTranslationSpeed * time; // Move left
        if (event::isKeyPressed(NEXO_KEY_D))
            m_cameraPosition -= rightDirection * m_cameraTranslationSpeed * time; // Move right

        if (m_rotationActivated)
        {
            if (event::isKeyPressed(NEXO_KEY_E))
                m_cameraRotation += m_cameraRotationSpeed * time;
            if (event::isKeyPressed(NEXO_KEY_A))
                m_cameraRotation -= m_cameraRotationSpeed * time;
        }

        m_camera.setPosition(m_cameraPosition);
        m_camera.setRotation({0.0f, 0.0f, m_cameraRotation});

        m_cameraTranslationSpeed = m_zoomLevel;
    }

    void OrthographicCameraController::handleEvent(event::EventMouseScroll &event)
    {
        if (!zoomOn)
            return;

        m_zoomLevel -= event.y * 0.5f;
        m_zoomLevel = std::max(m_zoomLevel, 0.25f);
        m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
        event.consumed = true;
    }

    void OrthographicCameraController::handleEvent(event::EventWindowResize &event)
    {
        m_aspectRatio = static_cast<float>(event.width) / static_cast<float>(event.height);
        m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
        event.consumed = true;
    }
}
