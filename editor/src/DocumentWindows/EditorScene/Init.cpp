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
#include "RenderPass.hpp"
#include "utils/EditorProps.hpp"
#include "renderPasses/GridPass.hpp"
#include "renderPasses/MaskPass.hpp"
#include "renderPasses/OutlinePass.hpp"
#include "assets/AssetImporter.hpp"
#include "Path.hpp"

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
        m_editorCamera = CameraFactory::createPerspectiveCamera({0.0f, 6.0f, 20.0f}, static_cast<unsigned int>(m_contentSize.x), static_cast<unsigned int>(m_contentSize.y), renderTarget);
        auto &cameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(m_editorCamera);
        cameraComponent.render = true;
        auto maskPass = std::make_shared<renderer::MaskPass>(m_contentSize.x, m_contentSize.y);
        auto outlinePass = std::make_shared<renderer::OutlinePass>(m_contentSize.x, m_contentSize.y);
        auto gridPass = std::make_shared<renderer::GridPass>(m_contentSize.x, m_contentSize.y);

        const renderer::PassId forwardId = cameraComponent.pipeline.getFinalOutputPass();
        const renderer::PassId maskId = cameraComponent.pipeline.addRenderPass(std::move(maskPass));
        const renderer::PassId outlineId = cameraComponent.pipeline.addRenderPass(std::move(outlinePass));
        const renderer::PassId gridId = cameraComponent.pipeline.addRenderPass(std::move(gridPass));
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
        app.getSceneManager().getScene(m_sceneId).addEntity(static_cast<ecs::Entity>(m_editorCamera));
        const components::PerspectiveCameraController controller;
        Application::m_coordinator->addComponent<components::PerspectiveCameraController>(static_cast<ecs::Entity>(m_editorCamera), controller);
        constexpr components::EditorCameraTag editorCameraTag;
        Application::m_coordinator->addComponent(m_editorCamera, editorCameraTag);
        m_activeCamera = m_editorCamera;

        m_sceneUuid = app.getSceneManager().getScene(m_sceneId).getUuid();
        if (m_defaultScene)
            loadDefaultEntities();
    }

    void EditorScene::loadDefaultEntities() const
    {
        auto& app = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        scene.addEntity(LightFactory::createAmbientLight({0.5f, 0.5f, 0.5f}));
        const ecs::Entity pointLight = LightFactory::createPointLight({2.0f, 5.0f, 0.0f});
        utils::addPropsTo(pointLight, utils::PropsType::POINT_LIGHT);
        scene.addEntity(pointLight);
        scene.addEntity(LightFactory::createDirectionalLight({0.2f, -1.0f, -0.3f}));

        // Ground
        // const ecs::Entity ground = EntityFactory3D::createCube({0.0f, 0.25f, 0.0f}, {20.0f, 0.5f, 20.0f}, {0, 0, 0}, {0.2f, 0.2f, 0.2f, 1.0f});
        // app.getPhysicsSystem()->createBodyFromShape(ground, app.m_coordinator->getComponent<components::TransformComponent>(ground), system::ShapeType::Box, JPH::EMotionType::Static);
        // scene.addEntity(ground);

        auto& coord = *app.m_coordinator;

        {
            ecs::Entity wallLeft = EntityFactory3D::createCube({-4.45f, 2.5f, 0.0f}, {0.5f, 20.0f, 10.0f}, {0, 0, 0}, {0.3f, 0.3f, 0.3f, 1.0f});
            app.getPhysicsSystem()->createBodyFromShape(wallLeft, coord.getComponent<components::TransformComponent>(wallLeft), system::ShapeType::Box, JPH::EMotionType::Static);
            scene.addEntity(wallLeft);
        }

        {
            ecs::Entity wallRight = EntityFactory3D::createCube({4.7f, 2.5f, 0.0f}, {0.5f, 20.0f, 10.0f}, {0, 0, 0}, {0.3f, 0.3f, 0.3f, 1.0f});
            app.getPhysicsSystem()->createBodyFromShape(wallRight, coord.getComponent<components::TransformComponent>(wallRight), system::ShapeType::Box, JPH::EMotionType::Static);
            scene.addEntity(wallRight);
        }

        {
            ecs::Entity wallBack = EntityFactory3D::createCube({0.0f, 2.5f, -1.6f}, {10.0f, 20.0f, 0.5f}, {0, 0, 0}, {0.3f, 0.3f, 0.3f, 0.1f});
            app.getPhysicsSystem()->createBodyFromShape(wallBack, coord.getComponent<components::TransformComponent>(wallBack), system::ShapeType::Box, JPH::EMotionType::Static);
            scene.addEntity(wallBack);
        }

        // Mur avant
        // {
        //     ecs::Entity wallFront = EntityFactory3D::createCube({0.0f, 5.0f, 2.5f}, {10.0f, 10.0f, 0.5f}, {0, 0, 0}, {1.0f, 0.0f, 0.0f, 0.0f});
        //     app.getPhysicsSystem()->createBodyFromShape(wallFront, coord.getComponent<components::TransformComponent>(wallFront), system::ShapeType::Box, JPH::EMotionType::Static);
        //     scene.addEntity(wallFront);
        // }

        const float spacing = 1.5f;
        const int cols = 6;
        const float startY = 0.0f;
        const int totalRows = 10;

        for (int row = 0; row < totalRows; ++row) {
            for (int col = 0; col < cols; ++col) {
                float offsetX = (row % 2 == 0) ? 0.0f : spacing / 2.0f;
                glm::vec3 pos = {
                    col * spacing - 4.0f + offsetX,
                    startY + row * 1.2f,
                    0.0f
                };

                ecs::Entity peg = EntityFactory3D::createCylinder(
                    pos,
                    {0.2f, 2.5f, 0.2f},
                    {glm::radians(90.0f), 0, 0},
                    {0.6f, 0.6f, 0.6f, 1.0f},
                    12
                );
                app.getPhysicsSystem()->createBodyFromShape(
                    peg,
                    app.m_coordinator->getComponent<components::TransformComponent>(peg),
                    system::ShapeType::Cylinder,
                    JPH::EMotionType::Static
                );
                scene.addEntity(peg);
            }
        }

        for (int i = 0; i < 300; ++i) {
            float x = -3.0f + static_cast<float>(i % 5) * 1.5f;
            float z = static_cast<float>((i % 2 == 0) ? 1 : -1) * 0.5f;
            glm::vec3 pos = {x, 10.0f + static_cast<float>(i), z};

            ecs::Entity ball = EntityFactory3D::createSphere(pos, {0.3f, 0.3f, 0.3f}, {0, 0, 0}, {1.0f, 0.2f, 0.2f, 1.0f}, 3);
            app.getPhysicsSystem()->createBodyFromShape(ball, app.m_coordinator->getComponent<components::TransformComponent>(ball), system::ShapeType::Sphere, JPH::EMotionType::Dynamic);
            scene.addEntity(ball);
        }
    }


    void EditorScene::setupWindow()
    {
        m_contentSize = ImVec2(1280, 720);
    }

    void EditorScene::setCamera(const ecs::Entity cameraId)
    {
        auto &oldCameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);
        oldCameraComponent.active = false;
        oldCameraComponent.render = false;
        m_activeCamera = static_cast<int>(cameraId);
        auto &newCameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(cameraId);
        newCameraComponent.resize(static_cast<unsigned int>(m_contentSize.x), static_cast<unsigned int>(m_contentSize.y));
    }
}
