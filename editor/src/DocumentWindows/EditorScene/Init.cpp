//// Init.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the init functions of the editor scene
//
///////////////////////////////////////////////////////////////////////////////

#include "EditorScene.hpp"
#include "CameraFactory.hpp"
#include "LightFactory.hpp"
#include "EntityFactory3D.hpp"
#include "utils/EditorProps.hpp"

namespace nexo::editor {

    void EditorScene::setup()
    {
        setupWindow();
        setupScene();
        setupShortcuts();
    }

    void EditorScene::setupScene()
    {
        auto &app = getApp();

        m_sceneId = static_cast<int>(app.getSceneManager().createScene(m_windowName));
        renderer::NxFramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            renderer::NxFrameBufferTextureFormats::RGBA8, renderer::NxFrameBufferTextureFormats::RED_INTEGER, renderer::NxFrameBufferTextureFormats::Depth
        };
        framebufferSpecs.width = static_cast<unsigned int>(m_contentSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(m_contentSize.y);
        const auto renderTarget = renderer::NxFramebuffer::create(framebufferSpecs);
        m_editorCamera = CameraFactory::createPerspectiveCamera({0.0f, 4.0f, 10.0f}, static_cast<unsigned int>(m_contentSize.x), static_cast<unsigned int>(m_contentSize.y), renderTarget);
        auto &cameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(m_editorCamera);
        cameraComponent.render = true;
        app.getSceneManager().getScene(m_sceneId).addEntity(static_cast<ecs::Entity>(m_editorCamera));
        components::PerspectiveCameraController controller;
        Application::m_coordinator->addComponent<components::PerspectiveCameraController>(static_cast<ecs::Entity>(m_editorCamera), controller);
        components::EditorCameraTag editorCameraTag;
        Application::m_coordinator->addComponent(m_editorCamera, editorCameraTag);
        m_activeCamera = m_editorCamera;

        m_sceneUuid = app.getSceneManager().getScene(m_sceneId).getUuid();
        if (m_defaultScene)
            loadDefaultEntities();
    }

    void EditorScene::loadDefaultEntities() const
    {
        auto &app = getApp();
        scene::Scene &scene = app.getSceneManager().getScene(m_sceneId);
        const ecs::Entity ambientLight = LightFactory::createAmbientLight({0.5f, 0.5f, 0.5f});
        scene.addEntity(ambientLight);
        const ecs::Entity pointLight = LightFactory::createPointLight({2.0f, 5.0f, 0.0f});
        utils::addPropsTo(pointLight, utils::PropsType::POINT_LIGHT);
        scene.addEntity(pointLight);
        const ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.2f, -1.0f, -0.3f});
        scene.addEntity(directionalLight);
        const ecs::Entity spotLight = LightFactory::createSpotLight({-2.0f, 5.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
        utils::addPropsTo(spotLight, utils::PropsType::SPOT_LIGHT);
        scene.addEntity(spotLight);
        const ecs::Entity basicCube = EntityFactory3D::createCube({0.0f, 0.25f, 0.0f}, {20.0f, 0.5f, 20.0f},
                                                               {0.0f, 0.0f, 0.0f}, {0.05f * 1.7, 0.09f * 1.35, 0.13f * 1.45, 1.0f});
        app.getSceneManager().getScene(m_sceneId).addEntity(basicCube);

        const ecs::Entity ground = EntityFactory3D::createCube(
            {0.0f, 0.25f, 0.0f}, // position
            {20.0f, 0.5f, 20.0f}, // size
            {0.0f, 0.0f, 0.0f},
            {0.2f, 0.2f, 0.2f, 1.0f} // color
        );
        app.getPhysicsSystem()->createStaticBody(ground, app.m_coordinator->getComponent<components::TransformComponent>(ground));
        scene.addEntity(ground);

        const ecs::Entity fallingCube = EntityFactory3D::createCube(
            {0.0f, 5.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.2f, 0.2f, 1.0f}
        );
        app.getPhysicsSystem()->createDynamicBody(fallingCube, app.m_coordinator->getComponent<components::TransformComponent>(fallingCube));
        scene.addEntity(fallingCube);

        const ecs::Entity fallingCube2 = EntityFactory3D::createCube(
            {0.5f, 7.0f, 0.0f},
            {1.0f, 1.0f, 1.0f},
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.2f, 0.2f, 1.0f}
        );
        app.getPhysicsSystem()->createDynamicBody(fallingCube2, app.m_coordinator->getComponent<components::TransformComponent>(fallingCube2));
        scene.addEntity(fallingCube2);
    }

    void EditorScene::setupWindow()
    {
        m_contentSize = ImVec2(1280, 720);
    }

    void EditorScene::setCamera(ecs::Entity cameraId)
    {
        auto &app = getApp();
        auto &oldCameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);
        oldCameraComponent.active = false;
        oldCameraComponent.render = false;
        m_activeCamera = cameraId;
        auto &newCameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(cameraId);
        newCameraComponent.resize(m_contentSize.x, m_contentSize.y);
    }
}
