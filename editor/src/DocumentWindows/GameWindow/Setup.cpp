//// Setup.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
#include "components/SceneComponents.hpp"
#include "components/Name.hpp"
#include "core/scene/SceneManager.hpp"
#include "renderer/Framebuffer.hpp"
#include "CameraFactory.hpp"
#include <loguru.hpp>

namespace nexo::editor
{

    void GameWindow::setup()
    {
        DLOG_F(INFO, "Setting up GameWindow for scene %u", m_sceneId);

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

        auto &coordinator = *Application::m_coordinator;
        auto &cameraComponent = coordinator.getComponent<components::CameraComponent>(m_gameCamera);
        cameraComponent.render = true;
        cameraComponent.active = true;

        // Add the camera to the scene
        auto &app = getApp();
        app.getSceneManager().getScene(m_sceneId).addEntity(m_gameCamera);

        // Tag the camera with the scene
        components::SceneTag sceneTag{m_sceneId};
        coordinator.addComponent(m_gameCamera, sceneTag);

        components::NameComponent nameComponent{"Render Camera"};
        coordinator.addComponent(m_gameCamera, nameComponent);

        DLOG_F(INFO, "Created game camera %u for scene %u", m_gameCamera, m_sceneId);

        m_firstFrame = true;
    }

}