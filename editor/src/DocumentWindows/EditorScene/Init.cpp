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

#include <components/Name.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "CameraFactory.hpp"
#include "EditorScene.hpp"
#include "EntityFactory3D.hpp"
#include "LightFactory.hpp"
#include "Path.hpp"
#include "DemoScenes.hpp"
#include "RenderPass.hpp"
#include "renderPasses/GridPass.hpp"
#include "renderPasses/MaskPass.hpp"
#include "renderPasses/OutlinePass.hpp"
#include "renderPasses/ShadowPass.hpp"
#include "renderPasses/PointShadowPass.hpp"
#include "utils/EditorProps.hpp"

namespace nexo::editor {
    void EditorScene::setup()
    {
        setupWindow();
        setupScene();
        setupShortcuts();
    }

    void EditorScene::setupDemoCamera() const
    {
        auto& app = getApp();

        renderer::NxFramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {renderer::NxFrameBufferTextureFormats::RGBA8,
                                        renderer::NxFrameBufferTextureFormats::RED_INTEGER,
                                        renderer::NxFrameBufferTextureFormats::Depth};
        framebufferSpecs.width       = static_cast<unsigned int>(m_contentSize.x);
        framebufferSpecs.height      = static_cast<unsigned int>(m_contentSize.y);
        const auto renderTarget      = renderer::NxFramebuffer::create(framebufferSpecs);
        const ecs::Entity demoCamera =
            CameraFactory::createPerspectiveCamera({6.47f, 4.97f, -0.04f}, static_cast<unsigned int>(m_contentSize.x),
                                                                 static_cast<unsigned int>(m_contentSize.y), renderTarget);
        auto& cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(demoCamera);
        auto& transformComponent =
            Application::m_coordinator->getComponent<components::TransformComponent>(demoCamera);
        transformComponent.quat = glm::quat(glm::radians(glm::vec3{151.70f, -89.40f, -180.0f}));
        cameraComponent.render  = true;

        // Add editor camera to the scene
        app.getSceneManager().getScene(m_sceneId).addEntity(demoCamera);
        const components::PerspectiveCameraController controller;
        Application::m_coordinator->addComponent<components::PerspectiveCameraController>(
            demoCamera, controller);
        utils::addPropsTo(demoCamera, utils::PropsType::CAMERA);
        components::NameComponent nameComp;
        nameComp.name = "Demo_camera";
        Application::m_coordinator->addComponent<components::NameComponent>(demoCamera, nameComp);
    }

    void EditorScene::setupScene()
    {
        auto& app = getApp();

        m_sceneId = static_cast<int>(app.getSceneManager().createScene(m_windowName));
        renderer::NxFramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {renderer::NxFrameBufferTextureFormats::RGBA8,
                                        renderer::NxFrameBufferTextureFormats::RED_INTEGER,
                                        renderer::NxFrameBufferTextureFormats::Depth};
        framebufferSpecs.width       = static_cast<unsigned int>(m_contentSize.x);
        framebufferSpecs.height      = static_cast<unsigned int>(m_contentSize.y);
        const auto renderTarget      = renderer::NxFramebuffer::create(framebufferSpecs);
        m_editorCamera               = static_cast<int>(
            CameraFactory::createPerspectiveCamera({-8.51f, 11.26f, 0.0f}, static_cast<unsigned int>(m_contentSize.x),
                                                                 static_cast<unsigned int>(m_contentSize.y), renderTarget));
        auto& cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(m_editorCamera);
        auto& transformComponent =
            Application::m_coordinator->getComponent<components::TransformComponent>(m_editorCamera);
        transformComponent.quat = glm::quat(glm::radians(glm::vec3{154.30f, -89.8f, 180.0f}));
        cameraComponent.render  = true;
        auto maskPass           = std::make_shared<renderer::MaskPass>(static_cast<unsigned int>(m_contentSize.x),
                                                             static_cast<unsigned int>(m_contentSize.y));
        auto outlinePass        = std::make_shared<renderer::OutlinePass>();
        auto gridPass           = std::make_shared<renderer::GridPass>();
        auto shadowPass         = std::make_shared<renderer::ShadowPass>(2048);
        auto pointShadowPass = std::make_shared<renderer::PointShadowPass>();

        const renderer::PassId forwardId = cameraComponent.pipeline.getFinalOutputPass();
        const renderer::PassId maskId    = cameraComponent.pipeline.addRenderPass(std::move(maskPass));
        const renderer::PassId outlineId = cameraComponent.pipeline.addRenderPass(std::move(outlinePass));
        const renderer::PassId gridId    = cameraComponent.pipeline.addRenderPass(std::move(gridPass));

        // Set up prerequisites
        cameraComponent.pipeline.addPrerequisite(outlineId, maskId);
        cameraComponent.pipeline.addPrerequisite(outlineId, forwardId);
        cameraComponent.pipeline.addPrerequisite(gridId, outlineId);

        // Set up effects
        cameraComponent.pipeline.addEffect(forwardId, outlineId);
        cameraComponent.pipeline.addEffect(maskId, outlineId);
        cameraComponent.pipeline.addEffect(outlineId, gridId);


        // Set the final output pass explicitly
        cameraComponent.pipeline.setFinalOutputPass(gridId);

        // Add editor camera to the scene
        app.getSceneManager().getScene(m_sceneId).addEntity(static_cast<ecs::Entity>(m_editorCamera));
        const components::PerspectiveCameraController controller;
        Application::m_coordinator->addComponent<components::PerspectiveCameraController>(
            static_cast<ecs::Entity>(m_editorCamera), controller);
        constexpr components::EditorCameraTag editorCameraTag;
        Application::m_coordinator->addComponent(m_editorCamera, editorCameraTag);
        m_activeCamera = m_editorCamera;

        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);
        scene.addEntity(LightFactory::createAmbientLight({0.02f, 0.02f, 0.02f}));
        setupDemoCamera();

        m_sceneUuid = app.getSceneManager().getScene(m_sceneId).getUuid();
        if (m_defaultScene) {
            // loadDefaultEntities();
            fullScene(m_sceneId);
            dominoScene(m_sceneId, {7.5f, 4.07f, -0.5f});
            lightsScene(m_sceneId, glm::vec3{0.0f, 0.0f, 0.0f});
            // chambouleScene(m_sceneId, glm::vec3{0.0f, 0.0f, 0.0f});
            // physicScene(m_sceneId, glm::vec3{-60.0f, 0.0f, 0.0f});
            // videoScene(m_sceneId, glm::vec3{0.0f, 0.0f, 0.0f});
            // forestScene(m_sceneId, glm::vec3{100.0f, 1.0f, 0.0f});
        }
    }

    void EditorScene::setupWindow()
    {
        m_contentSize = ImVec2(1280, 720);
    }

    void EditorScene::setCamera(const ecs::Entity cameraId)
    {
        auto& oldCameraComponent =
            Application::m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);
        oldCameraComponent.active = false;
        oldCameraComponent.render = false;
        m_activeCamera            = static_cast<int>(cameraId);
        auto& newCameraComponent  = Application::m_coordinator->getComponent<components::CameraComponent>(cameraId);
        newCameraComponent.resize(static_cast<unsigned int>(m_contentSize.x),
                                  static_cast<unsigned int>(m_contentSize.y));
    }

} // namespace nexo::editor
