//// Setup.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Cardonne
//  Date:        2025-06-24
//  Description: Setup implementation for GameWindow
//
///////////////////////////////////////////////////////////////////////////////

#include "GameWindow.hpp"
#include "Application.hpp"
#include "ecs/Coordinator.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"
#include "core/scene/SceneManager.hpp"
#include "renderer/Framebuffer.hpp"
#include "CameraFactory.hpp"
#include "Logger.hpp"

namespace nexo::editor
{

    void GameWindow::setup()
    {
        LOG(NEXO_INFO, "Setting up GameWindow for scene {}", m_sceneId);

        auto &coordinator = *Application::m_coordinator;
        auto &app = getApp();
        auto &scene = app.getSceneManager().getScene(m_sceneId);
        
        // Check if a main camera already exists in the scene
        bool mainCameraFound = false;
        for (const auto &entity : scene.getEntities()) {
            if (coordinator.entityHasComponent<components::CameraComponent>(entity)) {
                auto &camera = coordinator.getComponent<components::CameraComponent>(entity);
                if (camera.main) {
                    // Found an existing main camera, use it with its existing render target
                    m_gameCamera = entity;
                    camera.render = true;
                    camera.active = true;
                    mainCameraFound = true;
                    LOG(NEXO_INFO, "Using existing main camera {} for scene {}", m_gameCamera, m_sceneId);
                    break;
                }
            }
        }
        
        // Only create a new camera if no main camera exists
        if (!mainCameraFound) {
            // Create render target specs for new camera
            renderer::NxFramebufferSpecs framebufferSpecs;
            framebufferSpecs.attachments = {
                renderer::NxFrameBufferTextureFormats::RGBA8,
                renderer::NxFrameBufferTextureFormats::RED_INTEGER, // Required by render system
                renderer::NxFrameBufferTextureFormats::Depth};
            framebufferSpecs.width = 1280; // Default size, will be resized
            framebufferSpecs.height = 720;
            const auto renderTarget = renderer::NxFramebuffer::create(framebufferSpecs);
            
            // Create a render camera
            m_gameCamera = CameraFactory::createPerspectiveCamera(
                {0.0f, 0.0f, 6.0f}, // Default position
                framebufferSpecs.width,
                framebufferSpecs.height,
                renderTarget);

            auto &cameraComponent = coordinator.getComponent<components::CameraComponent>(m_gameCamera);
            cameraComponent.render = true;
            cameraComponent.active = true;

            // Add the camera to the scene
            scene.addEntity(m_gameCamera);

            LOG(NEXO_INFO, "Created new game camera {} for scene {}", m_gameCamera, m_sceneId);
        }
    }

}