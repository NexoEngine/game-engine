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

namespace nexo::camera {

    PerspectiveCameraController::PerspectiveCameraController(const float aspectRatio, const float fovY)
        : m_aspectRatio(aspectRatio),
          m_fovY(fovY),
          m_camera(fovY, aspectRatio, 0.1f, 100.0f)
    {
        LOG(NEXO_DEBUG, "PerspectiveCameraController: new perspective camera controller created");
        Application::getInstance().getEventManager()->registerListener<event::EventWindowResize>(this);
        Application::getInstance().getEventManager()->registerListener<event::EventMouseScroll>(this);
    }

    void PerspectiveCameraController::onUpdate(const Timestep ts)
    {
        const float time = ts;

        // Camera movement
        if (event::isKeyPressed(NEXO_KEY_Z))
            m_cameraPosition.z -= m_cameraTranslationSpeed * time;
        if (event::isKeyPressed(NEXO_KEY_S))
            m_cameraPosition.z += m_cameraTranslationSpeed * time;
        if (event::isKeyPressed(NEXO_KEY_D))
            m_cameraPosition.x += m_cameraTranslationSpeed * time;
        if (event::isKeyPressed(NEXO_KEY_A))
            m_cameraPosition.x -= m_cameraTranslationSpeed * time;
        if (event::isKeyPressed(NEXO_KEY_SPACE))
            m_cameraPosition.y += m_cameraTranslationSpeed * time;
        if (event::isKeyPressed(NEXO_KEY_TAB))
            m_cameraPosition.y -= m_cameraTranslationSpeed * time;

        // Camera rotation
        if (event::isKeyPressed(NEXO_KEY_UP))
            m_cameraRotation.x -= m_cameraRotationSpeed * time;
        if (event::isKeyPressed(NEXO_KEY_DOWN))
            m_cameraRotation.x += m_cameraRotationSpeed * time;
        if (event::isKeyPressed(NEXO_KEY_LEFT))
            m_cameraRotation.y -= m_cameraRotationSpeed * time;
        if (event::isKeyPressed(NEXO_KEY_RIGHT))
            m_cameraRotation.y += m_cameraRotationSpeed * time;

        m_camera.setPosition(m_cameraPosition);
        m_camera.setRotation(m_cameraRotation.x);

        m_cameraTranslationSpeed = m_zoomLevel;
    }

    void PerspectiveCameraController::handleEvent(event::EventMouseScroll &event)
    {
        if (!zoomOn)
            return;

        m_zoomLevel -= event.y * 0.1f;
        m_zoomLevel = std::max(m_zoomLevel, 0.25f);
        m_fovY = 45.0f / m_zoomLevel;

        m_camera.setProjection(m_fovY, m_aspectRatio, 0.1f, 100.0f);
        event.consumed = true;
    }

    void PerspectiveCameraController::handleEvent(event::EventWindowResize &event)
    {
        m_aspectRatio = static_cast<float>(event.width) / static_cast<float>(event.height);
        m_camera.setProjection(m_fovY, m_aspectRatio, 0.1f, 100.0f);
        event.consumed = true;
    }

}

