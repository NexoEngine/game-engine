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
#include "components/SceneComponents.hpp"

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

    void lightsScene(const unsigned int sceneId)
    {
        auto& app = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(sceneId);

        // Lights
        constexpr int numLights = 5;
        constexpr glm::vec3 center = {0.0f, 5.0f, 0.0f};
        const std::vector<glm::vec4> colors = {
            {0.0f, 0.0f, 1.0f, 1.0f}, // Blue
            {1.0f, 0.0f, 1.0f, 1.0f}, // Magenta
            {1.0f, 0.5f, 0.0f, 1.0f}, // Orange
            {0.0f, 1.0f, 0.0f, 1.0f}, // Green
            {1.0f, 1.0f, 0.0f, 1.0f} // Yellow
        };

        for (int i = 0; i < numLights; ++i)
        {
            constexpr float radius = 30.0f;
            const float angle = glm::radians(360.0f / numLights * i);
            const glm::vec3 position = center + glm::vec3(radius * cos(angle), 0.0f, radius * sin(angle));
            const auto light = LightFactory::createPointLight(position, colors[i % colors.size()], 0.01, 0.0010);
            scene.addEntity(light);
        }
    }

    void EditorScene::loadDefaultEntities() const
    {
        auto& app = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

        scene.addEntity(LightFactory::createAmbientLight({1.0f, 1.0f, 1.0f}));
        // const ecs::Entity pointLight = LightFactory::createPointLight({2.0f, 5.0f, 0.0f});
        // addPropsTo(pointLight, utils::PropsType::POINT_LIGHT);
        // scene.addEntity(pointLight);
        // scene.addEntity(LightFactory::createDirectionalLight({0.2f, -1.0f, -0.3f}));

        // Lights
        // constexpr int nb_lights = 5;
        // for (int i = 0; i < nb_lights; ++i)
        // {
        //     constexpr float startY_light = 60.0f;
        //     constexpr float light_spacing = 70.0f / nb_lights;
        //     scene.addEntity(LightFactory::createPointLight({-21.0f, startY_light + static_cast<float>(i) * light_spacing, 0.0f},
        //                                                     {1.0f, 1.0f, 1.0f}));
        //     scene.addEntity(LightFactory::createPointLight({21.0f, startY_light + static_cast<float>(i) * light_spacing, 0.0f},
        //                                         {1.0f, 1.0f, 1.0f}));
        //
        // }

        // Helper function to create and add an entity
        auto createAndAddEntity = [&](const glm::vec3& pos, const glm::vec3& size, const glm::vec3& rotation,
                                      const glm::vec4& color, system::ShapeType shapeType, JPH::EMotionType motionType)
        {
            ecs::Entity entity;
            LOG(NEXO_DEV, "Creating entity of type: {}", static_cast<int>(shapeType));
            switch (shapeType)
            {
                case system::ShapeType::Box:
                    entity = EntityFactory3D::createCube(pos, size, rotation, color);
                    break;
                case system::ShapeType::Sphere:
                    entity = EntityFactory3D::createSphere(pos, size, rotation, color, 1);
                    break;
                case system::ShapeType::Cylinder:
                    entity = EntityFactory3D::createCylinder(pos, size, rotation, color, 8);
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
            JPH::BodyID bodyId = app.getPhysicsSystem()->createBodyFromShape(entity, app.m_coordinator->getComponent<components::TransformComponent>(entity), shapeType, motionType);
            if (bodyId.IsInvalid()) {
                LOG(NEXO_ERROR, "Failed to create physics body for entity {}", entity);
            }
            scene.addEntity(entity);
            return entity;
        };

        // Balls

        for (int i = 0; i < 50; ++i)
        {
            float x = -3.0f + static_cast<float>(i % 5) * 1.5f;
            float z = static_cast<float>((i % 2 == 0) ? 1 : -1) * 0.5f;
            glm::vec3 pos = {x, 62.0f + static_cast<float>(i), z};
            glm::vec4 color = {
                1.0f, dis(gen), dis(gen), 1.0f
            };
            createAndAddEntity(pos, {0.4f, 0.4f, 0.4f}, {0, 0, 0}, color, system::ShapeType::Sphere,
                               JPH::EMotionType::Dynamic);
        }
        lightsScene(m_sceneId);

        // 9mn
        assets::AssetImporter importer;
        std::filesystem::path path9mn = Path::resolvePathRelativeToExe("../resources/models/9mn/scene.gltf");
        assets::ImporterFileInput fileInput9mn{path9mn};
        auto assetRef9mn = importer.importAsset<assets::Model>(assets::AssetLocation("my_package::9mn@DefaultScene/"), fileInput9mn);

        // Background
        createAndAddEntity({0.0f, 40.0f, -2.5f}, {44.0f, 80.0f, 0.5f}, {0, 0, 0}, {0.91f, 0.91f, 0.91f, 1.0f},
                           system::ShapeType::Box, JPH::EMotionType::Static);

        // Funnel
        createAndAddEntity({-6.0f, 70.0f, 0.0f}, {10.0f, 0.5f, 4.0f}, {0, 0, -45.0f}, {0.0f, 0.28f, 0.47f, 1.0f},
                           system::ShapeType::Box, JPH::EMotionType::Static);
        createAndAddEntity({6.0f, 70.0f, 0.0f}, {10.0f, 0.5f, 4.0f}, {0, 0, 45.0f}, {0.0f, 0.28f, 0.47f, 1.0f},
                           system::ShapeType::Box, JPH::EMotionType::Static);

        // Spinner
        createAndAddEntity({0.0f, 65.0f, 0.0f}, {0.5f, 3.0f, 4.0f}, {0, 0, 5.0f}, {0.0f, 1.0f, 0.0f, 1.0f},
                           system::ShapeType::Box, JPH::EMotionType::Static);

        // Stairs
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec4>> stairs = {
            {{3.0f, 61.5f, 0.0f}, {5.0f, 0.5f, 4.0f}, {0, 0, -15.0f}, {0.0f, 0.28f, 0.47f, 1.0f}},
            {{11.0f, 58.5f, 0.0f}, {8.0f, 0.5f, 4.0f}, {0.0f,  0.0f,  20.0f}, {0.0f, 0.28f, 0.47f, 1.0f}},
            {{3.0f, 55.5f, 0.0f}, {5.0f, 0.5f, 4.0f}, {0.0f,  0.0f,  -15.0f}, {0.0f, 0.28f, 0.47f, 1.0f}},
            {{10.0f, 52.5f, 0.0f}, {12.0f, 0.5f, 4.0f}, {0.0f,  0.0f,  20.0f}, {0.0f, 0.28f, 0.47f, 1.0f}}
        };
        for (const auto& [pos, size, rotation, color] : stairs)
        {
            createAndAddEntity(pos, size, rotation, color, system::ShapeType::Box, JPH::EMotionType::Static);
        }

        // Tunnel
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> tunnels = {
            {{-6.0f, 59.0f, 0.0f}, {3.0f, 11.0f, 4.0f}, {0, 0, 0}},
            {{-1.0f, 58.5f, 0.0f}, {3.0f, 8.0f, 4.0f}, {0, 0, 0}},
            {{-5.0f, 51.0f, 0.0f}, {9.0f, 0.5f, 4.0f}, {0, 0, -25.0f}}
        };
        for (int i = 0; const auto& [pos, size, rotation] : tunnels)
        {
            createAndAddEntity(pos, size, rotation, {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box, JPH::EMotionType::Static);
        }

        // Dominos
        createAndAddEntity({-9.0f, 44.0f, 0.0f}, {20.9f, 0.5f, 4.0f}, {0, 0, 0.0f}, {0.0f, 0.28f, 0.47f, 1.0f},
                           system::ShapeType::Box, JPH::EMotionType::Static);
        createAndAddEntity({11.15f, 44.0f, 0.0f}, {15.5f, 0.5f, 4.0f}, {0, 0, 0.0f}, {0.0f, 0.28f, 0.47f, 1.0f},
                           system::ShapeType::Box, JPH::EMotionType::Static);

        for (int i = 0; i < 24; ++i)
        {
            if (i == 13) continue;
            float x = -18.4f + static_cast<float>(i) * 1.6f;
            glm::vec3 pos = {x, 45.5f, 0.0f};
            float gradientFactor = static_cast<float>(i) / 24.0f;
            glm::vec4 color = mix(glm::vec4(0.0f, 0.77f, 0.95f, 1.0f), glm::vec4(0.83f, 0.14f, 0.67f, 1.0f), gradientFactor);
            createAndAddEntity(pos, {0.25f, 3.0f, 3.0f}, {0, 0, 0}, color, system::ShapeType::Box, JPH::EMotionType::Dynamic);
        }

        // Spinner
        createAndAddEntity({2.5f, 41.0f, 0.0f}, {0.5f, 3.0f, 4.0f}, {0, 0, 0}, {0.0f, 1.0f, 0.0f, 1.0f},
                           system::ShapeType::Box, JPH::EMotionType::Static);

        // Fakir
        constexpr int totalRows = 20;
        constexpr float startX = -14.0f;

        for (int row = 0; row < totalRows; ++row)
        {
            constexpr int cols = 10;
            for (int col = 0; col < cols; ++col)
            {
                constexpr float startY = 14.0f;
                constexpr float spacing = 3.0f;
                float offsetX = (row % 2 == 0) ? 0.0f : spacing / 2.0f;
                glm::vec3 pos = {col * spacing + startX + offsetX, startY + row * 1.2f, 0.0f};
                auto maxFactor = static_cast<float>(totalRows * cols);
                float gradientFactor = static_cast<float>((row + 1) * (col + 1)) / maxFactor;
                glm::vec4 color = mix(glm::vec4(0.0f, 0.77f, 0.95f, 1.0f), glm::vec4(0.83f, 0.14f, 0.67f, 1.0f), gradientFactor);
                createAndAddEntity(pos, {0.4f, 6.0f, 0.4f}, {90.0f, 0, 0}, color, system::ShapeType::Cylinder, JPH::EMotionType::Static);
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
