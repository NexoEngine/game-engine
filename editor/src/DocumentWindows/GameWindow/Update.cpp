//// Update.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Cardonne
//  Date:        2025-06-24
//  Description: Update implementation for GameWindow
//
///////////////////////////////////////////////////////////////////////////////

#include "GameWindow.hpp"
#include "Application.hpp"
#include "ecs/Coordinator.hpp"
#include "ecs/Definitions.hpp"
#include "components/Camera.hpp"
#include "core/scene/SceneManager.hpp"
#include "Logger.hpp"

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
        if (m_gameCamera != ecs::INVALID_ENTITY)
        {
            // Try to get the camera component
            const auto cameraCompOpt = coordinator.tryGetComponent<components::CameraComponent>(m_gameCamera);
            if (cameraCompOpt)
            {
                // Disable rendering
                auto &cameraComp = cameraCompOpt->get();
                cameraComp.render = false;
                cameraComp.active = false;

                // Remove from scene
                sceneManager.getScene(m_sceneId).removeEntity(m_gameCamera);

                // Destroy the entity
                coordinator.destroyEntity(m_gameCamera);

                LOG(NEXO_INFO, "Destroyed game camera entity {}", m_gameCamera);
            }
            else
            {
                // Camera was already destroyed
                LOG(NEXO_WARN, "Failed to properly clean up game camera");
            }

            m_gameCamera = ecs::INVALID_ENTITY;
        }
    }

}