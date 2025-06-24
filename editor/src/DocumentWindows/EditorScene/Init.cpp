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

namespace nexo::editor
{
    void EditorScene::setup()
    {
        setupWindow();
        setupScene();
        setupShortcuts();
    }

    void EditorScene::setupScene()
    {
        auto& app = getApp();

        m_sceneId = static_cast<int>(app.getSceneManager().createScene(m_windowName));
        renderer::NxFramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            renderer::NxFrameBufferTextureFormats::RGBA8, renderer::NxFrameBufferTextureFormats::RED_INTEGER,
            renderer::NxFrameBufferTextureFormats::Depth
        };
        framebufferSpecs.width = static_cast<unsigned int>(m_contentSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(m_contentSize.y);
        const auto renderTarget = renderer::NxFramebuffer::create(framebufferSpecs);
        m_editorCamera = CameraFactory::createPerspectiveCamera({0.0f, 36.0f, 25.0f},
                                                                static_cast<unsigned int>(m_contentSize.x),
                                                                static_cast<unsigned int>(m_contentSize.y),
                                                                renderTarget);
        auto& cameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(m_editorCamera);
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
        Application::m_coordinator->addComponent<components::PerspectiveCameraController>(
            static_cast<ecs::Entity>(m_editorCamera), controller);
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

        scene.addEntity(LightFactory::createAmbientLight({0.45f, 0.45f, 0.45f}));
        // const ecs::Entity pointLight = LightFactory::createPointLight({2.0f, 5.0f, 0.0f});
        // addPropsTo(pointLight, utils::PropsType::POINT_LIGHT);
        // scene.addEntity(pointLight);
        // scene.addEntity(LightFactory::createDirectionalLight({0.2f, -1.0f, -0.3f}));

        // Helper function to create and add an entity
        auto createAndAddEntity = [&](const glm::vec3& pos, const glm::vec3& size, const glm::vec3& rotation,
                                      const glm::vec4& color, system::ShapeType shapeType, JPH::EMotionType motionType)
        {
            ecs::Entity entity;
            std::cout << "Creating entity of type: " << static_cast<int>(shapeType) << std::endl;
            switch (shapeType)
            {
                case system::ShapeType::Box:
                    entity = EntityFactory3D::createCube(pos, size, rotation, color);
                    break;
                case system::ShapeType::Sphere:
                    entity = EntityFactory3D::createSphere(pos, size, rotation, color);
                    break;
                case system::ShapeType::Cylinder:
                    entity = EntityFactory3D::createCylinder(pos, size, rotation, color);
                    break;
                case system::ShapeType::Tetrahedron:
                    entity = EntityFactory3D::createTetrahedron(pos, size, rotation, color);
                    break;
                case system::ShapeType::Pyramid:
                    entity = EntityFactory3D::createPyramid(pos, size, rotation, color);
                    break;
                default:
                    throw std::runtime_error("Unsupported shape type for entity creation.");
            }
            app.getPhysicsSystem()->createBodyFromShape(entity, app.m_coordinator->getComponent<components::TransformComponent>(entity), shapeType, motionType);
            scene.addEntity(entity);
            return entity;
        };

        // Balls
        for (int i = 0; i < 30; ++i)
        {
            float x = -3.0f + static_cast<float>(i % 5) * 1.5f;
            float z = static_cast<float>((i % 2 == 0) ? 1 : -1) * 0.5f;
            glm::vec3 pos = {x, 42.0f + static_cast<float>(i), z};
            glm::vec4 color = {
                1.0f, static_cast<float>(rand()) / RAND_MAX, static_cast<float>(rand()) / RAND_MAX, 1.0f
            };
            createAndAddEntity(pos, {0.5f, 0.5f, 0.5f}, {0, 0, 0}, color, system::ShapeType::Sphere,
                               JPH::EMotionType::Dynamic);
        }

        // Background
        createAndAddEntity({-0.0f, 30.0f, -3.0f}, {40.0f, 60.0f, 0.5f}, {0, 0, 0}, {0.2f, 0.2f, 0.2f, 1.0f},
                           system::ShapeType::Box, JPH::EMotionType::Static);

        // Funnel
        createAndAddEntity({-6.0f, 40.0f, 0.0f}, {10.0f, 0.5f, 6.0f}, {0, 0, -45.0f}, {0.3f, 0.3f, 0.3f, 0.1f},
                           system::ShapeType::Box, JPH::EMotionType::Static);
        createAndAddEntity({6.0f, 40.0f, 0.0f}, {10.0f, 0.5f, 6.0f}, {0, 0, 45.0f}, {0.3f, 0.3f, 0.3f, 0.1f},
                           system::ShapeType::Box, JPH::EMotionType::Static);

        // Spinner
        createAndAddEntity({0.0f, 35.0f, 0.0f}, {0.5f, 3.0f, 6.0f}, {0, 0, 0}, {1.0f, 0.0f, 0.0f, 1.0f},
                           system::ShapeType::Box, JPH::EMotionType::Static);

        // Stairs
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> stairs = {
            {{3.0f, 31.5f, 0.0f}, {5.0f, 0.5f, 6.0f}, {0, 0, -15.0f}},
            {{11.0f, 28.5f, 0.0f}, {8.0f, 0.5f, 6.0f}, {0, 0, 20.0f}},
            {{3.0f, 25.5f, 0.0f}, {5.0f, 0.5f, 6.0f}, {0, 0, -15.0f}},
            {{10.0f, 22.5f, 0.0f}, {12.0f, 0.5f, 6.0f}, {0, 0, 20.0f}}
        };
        for (const auto& [pos, size, rotation] : stairs)
        {
            createAndAddEntity(pos, size, rotation, {0.3f, 0.3f, 0.3f, 0.1f}, system::ShapeType::Box,
                               JPH::EMotionType::Static);
        }

        // Tunnel
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> tunnels = {
            {{-6.0f, 29.0f, 0.0f}, {3.0f, 11.0f, 6.0f}, {0, 0, 0}},
            {{-1.0f, 28.5f, 0.0f}, {3.0f, 8.0f, 6.0f}, {0, 0, 0}},
            {{-5.0f, 21.0f, 0.0f}, {9.0f, 0.5f, 6.0f}, {0, 0, -25.0f}}
        };
        for (const auto& [pos, size, rotation] : tunnels)
        {
            createAndAddEntity(pos, size, rotation, {0.3f, 0.3f, 0.3f, 0.1f}, system::ShapeType::Box,
                               JPH::EMotionType::Static);
        }

        // Dominos
        createAndAddEntity({-9.0f, 14.0f, 0.0f}, {21.0f, 0.5f, 6.0f}, {0, 0, 0.0f}, {0.3f, 0.3f, 0.3f, 0.1f},
                           system::ShapeType::Box, JPH::EMotionType::Static);
        createAndAddEntity({9.5f, 14.0f, 0.0f}, {13.0f, 0.5f, 6.0f}, {0, 0, 0.0f}, {0.3f, 0.3f, 0.3f, 0.1f},
                           system::ShapeType::Box, JPH::EMotionType::Static);

        for (int i = 0; i < 22; ++i)
        {
            if (i == 13) continue;
            float x = -18.4f + static_cast<float>(i) * 1.6f;
            glm::vec3 pos = {x, 15.0f, 0.0f};
            createAndAddEntity(pos, {0.25f, 3.0f, 3.0f}, {0, 0, 0}, {0.6f, 0.6f, 0.6f, 1.0f},
                               system::ShapeType::Box, JPH::EMotionType::Dynamic);
        }

        const float spacing = 4.0f;
        const int totalRows = 20;
        const float startY = -11.0f;
        const float startX = -16.0f;
        const int cols = 8;

        // Fakir
        for (int row = 0; row < totalRows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                float offsetX = (row % 2 == 0) ? 0.0f : spacing / 2.0f;
                glm::vec3 pos = {col * spacing + startX + offsetX, startY + row * 1.2f, 0.0f};
                glm::vec4 color = {
                    static_cast<float>(rand()) / RAND_MAX, 1.0f, static_cast<float>(rand()) / RAND_MAX, 1.0f
                };
                createAndAddEntity(pos, {0.25f, 3.0f, 0.25f}, {90.0f, 0, 0}, color, system::ShapeType::Cylinder, JPH::EMotionType::Static);
                // ecs::Entity peg = EntityFactory3D::createCylinder(pos, {0.35f, 6.0f, 0.35f}, {90.0f, 0, 0}, color);
                // app.getPhysicsSystem()->createBodyFromShape(peg,
                //                                             app.m_coordinator->getComponent<
                //                                                 components::TransformComponent>(peg),
                //                                             system::ShapeType::Cylinder,
                //                                             JPH::EMotionType::Static);
                // scene.addEntity(peg);
            }
        }
    }


    void EditorScene::setupWindow()
    {
        m_contentSize = ImVec2(1280, 720);
    }

    void EditorScene::setCamera(const ecs::Entity cameraId)
    {
        auto& oldCameraComponent = Application::m_coordinator->getComponent<
            components::CameraComponent>(m_activeCamera);
        oldCameraComponent.active = false;
        oldCameraComponent.render = false;
        m_activeCamera = static_cast<int>(cameraId);
        auto& newCameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(cameraId);
        newCameraComponent.resize(static_cast<unsigned int>(m_contentSize.x),
                                  static_cast<unsigned int>(m_contentSize.y));
    }
}
