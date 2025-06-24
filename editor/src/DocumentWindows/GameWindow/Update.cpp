//// Update.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Cardonne
//  Date:        2025-06-24
//  Description: Update implementation for GameWindow
//
///////////////////////////////////////////////////////////////////////////////

#include "GameWindow.hpp"
#include "Application.hpp"
#include "ecs/Coordinator.hpp"
#include "components/Camera.hpp"
#include "core/scene/SceneManager.hpp"
#include <loguru.hpp>

namespace nexo::editor
{

    void GameWindow::update()
    {
        if (!m_opened)
        {
            // the window will just close
            return;
        }

        // When paused, we still render the scene but don't update game logic
        // TODO: When the engine supports pausing game systems, implement it here
        // For now, the pause state is tracked but doesn't affect the actual game update
        
        // The actual scene update and rendering happens through the render systems
        // which are managed by the Application class
    }

    void GameWindow::shutdown()
    {
        auto &coordinator = *Application::m_coordinator;
        auto &app = getApp();
        auto &sceneManager = app.getSceneManager();

        // Clean up the game camera
        if (m_gameCamera != 0)
        {
            try
            {
                // Disable rendering
                auto &cameraComp = coordinator.getComponent<components::CameraComponent>(m_gameCamera);
                cameraComp.render = false;
                cameraComp.active = false;

                // Remove from scene
                sceneManager.getScene(m_sceneId).removeEntity(m_gameCamera);

                // Destroy the entity
                coordinator.destroyEntity(m_gameCamera);

                DLOG_F(INFO, "Destroyed game camera entity %u", m_gameCamera);
            }
            catch (...)
            {
                // Camera was already destroyed
                DLOG_F(WARNING, "Failed to properly clean up game camera");
            }

            m_gameCamera = 0;
        }
    }

}