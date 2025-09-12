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

#include <components/Video.hpp>
#include "CameraFactory.hpp"
#include "EditorScene.hpp"
#include "EntityFactory3D.hpp"
#include "LightFactory.hpp"
#include "Path.hpp"
#include "RenderPass.hpp"
#include "assets/AssetImporter.hpp"
#include "renderPasses/GridPass.hpp"
#include "renderPasses/MaskPass.hpp"
#include "renderPasses/OutlinePass.hpp"
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
            CameraFactory::createPerspectiveCamera({0.0f, 36.0f, 25.0f}, static_cast<unsigned int>(m_contentSize.x),
                                                                 static_cast<unsigned int>(m_contentSize.y), renderTarget));
        auto& cameraComponent  = Application::m_coordinator->getComponent<components::CameraComponent>(m_editorCamera);
        cameraComponent.render = true;
        auto maskPass          = std::make_shared<renderer::MaskPass>(static_cast<unsigned int>(m_contentSize.x),
                                                             static_cast<unsigned int>(m_contentSize.y));
        auto outlinePass       = std::make_shared<renderer::OutlinePass>();
        auto gridPass          = std::make_shared<renderer::GridPass>();

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
        app.getSceneManager().getScene(m_sceneId).addEntity(static_cast<ecs::Entity>(m_editorCamera));
        const components::PerspectiveCameraController controller;
        Application::m_coordinator->addComponent<components::PerspectiveCameraController>(
            static_cast<ecs::Entity>(m_editorCamera), controller);
        constexpr components::EditorCameraTag editorCameraTag;
        Application::m_coordinator->addComponent(m_editorCamera, editorCameraTag);
        m_activeCamera = m_editorCamera;

        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);
        scene.addEntity(LightFactory::createAmbientLight({1.0f, 1.0f, 1.0f}));
        scene.addEntity(LightFactory::createDirectionalLight({0.0f, -0.8f, 0.0f}));

        m_sceneUuid = app.getSceneManager().getScene(m_sceneId).getUuid();
        if (m_defaultScene) {
            // loadDefaultEntities();
            physicScene(glm::vec3{-60.0f, 0.0f, 0.0f});
            lightsScene(glm::vec3{50.0f, 0.0f, 0.0f});
            // videoScene(glm::vec3{-15.0f, 0.0f, 0.0f});
            forestScene({100.0f, 1.0f, 0.0f});
        }
    }

    void EditorScene::lightsScene(const glm::vec3& offset) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        // Lights
        constexpr int numLights             = 5;
        constexpr glm::vec3 center          = {0.0f, 5.0f, 0.0f};
        const std::vector<glm::vec4> colors = {
            {0.0f, 0.0f, 1.0f, 1.0f}, // Blue
            {1.0f, 0.0f, 1.0f, 1.0f}, // Magenta
            {1.0f, 0.5f, 0.0f, 1.0f}, // Orange
            {0.0f, 1.0f, 0.0f, 1.0f}, // Green
            {1.0f, 1.0f, 0.0f, 1.0f}  // Yellow
        };

        for (int i = 0; i < numLights; ++i) {
            constexpr float radius   = 10.0f;
            const float angle        = glm::radians(360.0f / numLights * static_cast<float>(i));
            const glm::vec3 position = center + glm::vec3(radius * cos(angle), 5.0f, radius * sin(angle));
            const auto light =
                LightFactory::createPointLight(position + offset, colors[i % colors.size()], 0.01f, 0.0010f);
            utils::addPropsTo(light, utils::PropsType::POINT_LIGHT);
            scene.addEntity(light);
        }

        const auto base = EntityFactory3D::createCube( {0.0f + offset.x, 0.0f + offset.y, 0.0f + offset.z},
                                                 {25.0f, 0.7f, 25.0f}, {0.0f, 0.0f, 0.0f}, {0.15f, 0.15f, 0.15f, 1.0f});
        scene.addEntity(base);

        const auto& catalog = nexo::assets::AssetCatalog::getInstance();

        const assets::AssetLocation rubixCubeModel("my_package::RubixCube@Models");
        const auto rubixCubeAssetRef = catalog.getAsset(rubixCubeModel).as<assets::Model>();
        auto rubixCube = EntityFactory3D::createModel(rubixCubeAssetRef, {4.1f + offset.x, 2.8f + offset.y, -4.7f + offset.z},
                                                  {10.0f, 10.0f, 10.0f}, {180.0f, 0.0f, 0.0f});
        scene.addEntity(rubixCube);

        const assets::AssetLocation planeModel("my_package::Plane@Models");
        const auto planeAssetRef = catalog.getAsset(planeModel).as<assets::Model>();
        auto plane = EntityFactory3D::createModel(planeAssetRef, {-5.0f + offset.x, 0.5f + offset.y, -5.0f + offset.z},
                                                  {1.0f, 1.0f, 1.0f}, {0.0f, 45.0f, 0.0f});
        scene.addEntity(plane);

        const assets::AssetLocation cupModel("my_package::Cup@Models");
        const auto cupAssetRef = catalog.getAsset(cupModel).as<assets::Model>();
        auto cup = EntityFactory3D::createModel(cupAssetRef, {7.0f + offset.x, 0.3f + offset.y, 1.6f + offset.z},
                                                  {14.0f, 14.0f, 14.0f}, {0.0f, -105.0f, 0.0f});
        scene.addEntity(cup);

        const assets::AssetLocation earthModel("my_package::Earth@Models");
        const auto earthAssetRef = catalog.getAsset(earthModel).as<assets::Model>();
        auto earth = EntityFactory3D::createModel(earthAssetRef, {-0.4f + offset.x, 2.1f + offset.y, 7.0f + offset.z},
                                                  {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f});
        scene.addEntity(earth);

        const assets::AssetLocation plantModel("my_package::Plant@Models");
        const auto plantAssetRef = catalog.getAsset(plantModel).as<assets::Model>();
        auto plant = EntityFactory3D::createModel(plantAssetRef, {-4.9f + offset.x, 0.38f + offset.y, 3.8f + offset.z},
                                                  {5.0f, 5.0f, 5.0f}, {0.0f, 0.0f, 0.0f});
        scene.addEntity(plant);

        const assets::AssetLocation swordModel("my_package::Sword@Models");
        const auto swordAssetRef = catalog.getAsset(swordModel).as<assets::Model>();
        auto sword = EntityFactory3D::createModel(swordAssetRef, {-0.0f + offset.x, 0.48f + offset.y, 0.0f + offset.z},
                                                  {2.77f, 2.77f, 2.77f}, {0.0f, 0.0f, 0.0f});
        scene.addEntity(sword);
    }

    void EditorScene::physicScene(const glm::vec3& offset) const
    {
        auto& app = getApp();

        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

        // Balls
        for (int i = 0; i < 50; ++i) {
            float x         = -3.0f + static_cast<float>(i % 5) * 1.5f;
            float z         = static_cast<float>((i % 2 == 0) ? 1 : -1) * 0.5f;
            glm::vec3 pos   = {x, 62.0f + static_cast<float>(i), z};
            glm::vec4 color = {1.0f, dis(gen), dis(gen), 1.0f};
            createEntityWithPhysic(pos + offset, {0.4f, 0.4f, 0.4f}, {0, 0, 0}, color, system::ShapeType::Sphere,
                                   JPH::EMotionType::Dynamic);
        }

        // Background
        createEntityWithPhysic({0.0f + offset.x, 40.0f + offset.y, -2.5f + offset.z}, {44.0f, 80.0f, 0.5f}, {0, 0, 0},
                               {0.91f, 0.91f, 0.91f, 1.0f}, system::ShapeType::Box, JPH::EMotionType::Static);

        // Funnel
        createEntityWithPhysic({-6.0f + offset.x, 70.0f + offset.y, 0.0f + offset.z}, {10.0f, 0.5f, 4.0f},
                               {0, 0, -45.0f}, {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box,
                               JPH::EMotionType::Static);
        createEntityWithPhysic({6.0f + offset.x, 70.0f + offset.y, 0.0f + offset.z}, {10.0f, 0.5f, 4.0f}, {0, 0, 45.0f},
                               {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box, JPH::EMotionType::Static);

        // Spinner
        // createEntityWithPhysic({0.0f, 65.0f, 0.0f}, {0.5f, 3.0f, 4.0f}, {0, 0, 5.0f}, {0.0f, 1.0f, 0.0f, 1.0f},
        //                        system::ShapeType::Box, JPH::EMotionType::Static);

        // Stairs
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec4>> stairs = {
            {{3.0f, 61.5f, 0.0f}, {5.0f, 0.5f, 4.0f}, {0, 0, -15.0f}, {0.0f, 0.28f, 0.47f, 1.0f}},
            {{11.0f, 58.5f, 0.0f}, {8.0f, 0.5f, 4.0f}, {0.0f, 0.0f, 20.0f}, {0.0f, 0.28f, 0.47f, 1.0f}},
            {{3.0f, 55.5f, 0.0f}, {5.0f, 0.5f, 4.0f}, {0.0f, 0.0f, -15.0f}, {0.0f, 0.28f, 0.47f, 1.0f}},
            {{10.0f, 52.5f, 0.0f}, {12.0f, 0.5f, 4.0f}, {0.0f, 0.0f, 20.0f}, {0.0f, 0.28f, 0.47f, 1.0f}}};
        for (const auto& [pos, size, rotation, color] : stairs) {
            createEntityWithPhysic(pos + offset, size, rotation, color, system::ShapeType::Box,
                                   JPH::EMotionType::Static);
        }

        // Tunnel
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> tunnels = {
            {{-6.0f, 59.0f, 0.0f}, {3.0f, 11.0f, 4.0f}, {0, 0, 0}},
            {{-1.0f, 58.5f, 0.0f}, {3.0f, 8.0f, 4.0f}, {0, 0, 0}},
            {{-5.0f, 51.0f, 0.0f}, {9.0f, 0.5f, 4.0f}, {0, 0, -25.0f}}};
        for (const auto& [pos, size, rotation] : tunnels) {
            createEntityWithPhysic(pos + offset, size, rotation, {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box,
                                   JPH::EMotionType::Static);
        }

        // Dominos
        createEntityWithPhysic({-9.0f + offset.x, 44.0f + offset.y, 0.0f + offset.z}, {20.9f, 0.5f, 4.0f}, {0, 0, 0.0f},
                               {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box, JPH::EMotionType::Static);
        createEntityWithPhysic({11.15f + offset.x, 44.0f + offset.y, 0.0f + offset.z}, {15.5f, 0.5f, 4.0f},
                               {0, 0, 0.0f}, {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box,
                               JPH::EMotionType::Static);

        for (int i = 0; i < 24; ++i) {
            if (i == 13) continue;
            float x              = -18.4f + static_cast<float>(i) * 1.6f;
            glm::vec3 pos        = {x, 45.5f, 0.0f};
            float gradientFactor = static_cast<float>(i) / 24.0f;
            glm::vec4 color =
                mix(glm::vec4(0.0f, 0.77f, 0.95f, 1.0f), glm::vec4(0.83f, 0.14f, 0.67f, 1.0f), gradientFactor);
            createEntityWithPhysic(pos + offset, {0.25f, 3.0f, 3.0f}, {0, 0, 0}, color, system::ShapeType::Box,
                                   JPH::EMotionType::Dynamic);
        }

        // Spinner
        createEntityWithPhysic({2.5f + offset.x, 41.0f + offset.y, 0.0f + offset.z}, {0.5f, 3.0f, 4.0f}, {0, 0, 0},
                               {0.0f, 1.0f, 0.0f, 1.0f}, system::ShapeType::Box, JPH::EMotionType::Static);

        // Fakir
        constexpr int totalRows = 20;
        constexpr float startX  = -14.0f;

        for (int row = 0; row < totalRows; ++row) {
            constexpr int cols = 10;
            for (int col = 0; col < cols; ++col) {
                constexpr float startY  = 14.0f;
                constexpr float spacing = 3.0f;
                float offsetX           = (row % 2 == 0) ? 0.0f : spacing / 2.0f;
                glm::vec3 pos           = {static_cast<float>(col) * spacing + startX + offsetX,
                                           startY + static_cast<float>(row) * 1.2f, 0.0f};
                auto maxFactor          = static_cast<float>(totalRows * cols);
                float gradientFactor    = static_cast<float>((row + 1) * (col + 1)) / maxFactor;
                glm::vec4 color =
                    mix(glm::vec4(0.0f, 0.77f, 0.95f, 1.0f), glm::vec4(0.83f, 0.14f, 0.67f, 1.0f), gradientFactor);
                createEntityWithPhysic(pos + offset, {0.4f, 6.0f, 0.4f}, {90.0f, 0, 0}, color,
                                       system::ShapeType::Cylinder, JPH::EMotionType::Static);
            }
        }
    }

    void EditorScene::videoScene(const glm::vec3& offset) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        auto videoBillboard =
            EntityFactory3D::createBillboard({0.0f + offset.x, 5.0f + offset.y, 1.0f + offset.z}, {5.3f, 3.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
        components::VideoComponent videoComponent;
        videoComponent.path = nexo::Path::resolvePathRelativeToExe("../resources/videos/test.mp4").string();

        Application::m_coordinator->addComponent(videoBillboard, videoComponent);

        scene.addEntity(videoBillboard);
    }

    void EditorScene::forestScene(const glm::vec3& offset) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        auto floor = EntityFactory3D::createCube({0.0f + offset.x, 0.0f + offset.y, 0.0f + offset.z},
                                                 {20.0f, 1.0f, 20.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
        components::VideoComponent videoComponent;
        videoComponent.path = nexo::Path::resolvePathRelativeToExe("../resources/videos/test.mp4").string();

        const auto& catalog = nexo::assets::AssetCatalog::getInstance();
        const assets::AssetLocation grassTexture("my_package::grass@Textures");
        const auto grassAssetRef = catalog.getAsset(grassTexture).as<assets::Texture>();
        auto &materialComp = app.m_coordinator->getComponent<components::MaterialComponent>(floor);
        const auto materialAssetRef = materialComp.material;
        const auto materialAsset = materialAssetRef.lock();
        materialAsset->getData()->albedoTexture = grassAssetRef;

        const assets::AssetLocation frogModel("my_package::Frog@Models");
        const auto frogAssetRef = catalog.getAsset(frogModel).as<assets::Model>();
        auto frog = EntityFactory3D::createModel(frogAssetRef, {0.0f + offset.x, 1.0f + offset.y, 0.0f + offset.z},
                                                 {0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 0.0f});

        const assets::AssetLocation treeModel("my_package::Tree@Models");
        const auto treeAssetRef = catalog.getAsset(treeModel).as<assets::Model>();
        auto tree = EntityFactory3D::createModel(treeAssetRef, {5.0f + offset.x, 0.5f + offset.y, -3.0f + offset.z},
                                                 {2.44f, 2.44f, 2.44f}, {0.0f, 0.0f, 0.0f});

        const assets::AssetLocation benchModel("my_package::Bench@Models");
        const auto benchAssetRef = catalog.getAsset(benchModel).as<assets::Model>();
        auto bench = EntityFactory3D::createModel(benchAssetRef, {-6.0f + offset.x, 2.0f + offset.y, -6.5f + offset.z},
                                                  {1.0f, 1.0f, 1.0f}, {0.0f, 180.0f, 0.0f});

        const assets::AssetLocation logModel("my_package::Log@Models");
        const auto logAssetRef = catalog.getAsset(logModel).as<assets::Model>();
        auto log = EntityFactory3D::createModel(logAssetRef, {-5.0f + offset.x, 0.5f + offset.y, 5.0f + offset.z},
                                                  {2.3f, 2.3f, 2.3f}, {0.0f, -40.0f, 0.0f});

        scene.addEntity(floor);
        scene.addEntity(frog);
        scene.addEntity(tree);
        scene.addEntity(bench);
        scene.addEntity(log);
    }

    void EditorScene::createEntityWithPhysic(const glm::vec3& pos, const glm::vec3& size, const glm::vec3& rotation,
                                             const glm::vec4& color, system::ShapeType shapeType,
                                             const JPH::EMotionType motionType) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        ecs::Entity entity;
        LOG(NEXO_DEV, "Creating entity of type: {}", static_cast<int>(shapeType));
        switch (shapeType) {
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
        const JPH::BodyID bodyId = app.getPhysicsSystem()->createBodyFromShape(
            entity, Application::m_coordinator->getComponent<components::TransformComponent>(entity), shapeType,
            motionType);
        if (bodyId.IsInvalid()) {
            LOG(NEXO_ERROR, "Failed to create physics body for entity {}", entity);
        }
        scene.addEntity(entity);
    }

    void EditorScene::loadDefaultEntities() const
    {}

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
