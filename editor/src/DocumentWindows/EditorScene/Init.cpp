//// Init.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the init functions of the editor scene
//
///////////////////////////////////////////////////////////////////////////////

#include <components/Video.hpp>
#include <math/Light.hpp>
#include <random>

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
        framebufferSpecs.attachments = renderer::NxFrameBufferAttachmentsSpecifications(
            {renderer::NxFrameBufferTextureSpecifications(renderer::NxFrameBufferTextureFormats::RGBA8),
             renderer::NxFrameBufferTextureSpecifications(renderer::NxFrameBufferTextureFormats::RED_INTEGER),
             renderer::NxFrameBufferTextureSpecifications(renderer::NxFrameBufferTextureFormats::Depth)});

        framebufferSpecs.width  = static_cast<unsigned int>(m_contentSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(m_contentSize.y);

        const auto renderTarget = renderer::NxFramebuffer::create(framebufferSpecs);
        m_editorCamera          = static_cast<int>(
            CameraFactory::createPerspectiveCamera({-14.51f, 7.41f, 2.46f}, static_cast<unsigned int>(m_contentSize.x),
                                                            static_cast<unsigned int>(m_contentSize.y), renderTarget));
        auto& cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(m_editorCamera);
        auto& transformComponent =
            Application::m_coordinator->getComponent<components::TransformComponent>(m_editorCamera);

        transformComponent.quat = glm::quat(glm::radians(glm::vec3{-56.90f, 18.90f, 0.0f}));
        cameraComponent.render  = true;
        auto maskPass           = std::make_shared<renderer::MaskPass>(static_cast<unsigned int>(m_contentSize.x),
                                                             static_cast<unsigned int>(m_contentSize.y));
        auto outlinePass        = std::make_shared<renderer::OutlinePass>();
        auto gridPass           = std::make_shared<renderer::GridPass>();

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

        m_sceneUuid = app.getSceneManager().getScene(m_sceneId).getUuid();
        if (m_defaultScene) {
            loadDefaultEntities(); // Load default entities for testing purposes
            physicScene(glm::vec3{-60.0f, 0.0f, 0.0f});
            videoScene(glm::vec3{-15.0f, 0.0f, 0.0f});
            lightsScene(glm::vec3{50.0f, 0.0f, 0.0f});
            forestScene({100.0f, 1.0f, 0.0f});
        }
    }

    void EditorScene::createEntityWithPhysic(const glm::vec3& pos, const glm::vec3& size, const glm::vec3& rotation,
                                             const glm::vec4& color, system::ShapeType shapeType,
                                             const JPH::EMotionType motionType) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        ecs::Entity entity;
        LOG(NEXO_DEV, "Creating entity of type: {}", static_cast<int>(shapeType));

        using enum nexo::system::ShapeType;
        switch (shapeType) {
            case Box:
                entity = EntityFactory3D::createCube(pos, size, rotation, color);
                break;
            case Sphere:
                entity = EntityFactory3D::createSphere(pos, size, rotation, color, 1);
                break;
            case Cylinder:
                entity = EntityFactory3D::createCylinder(pos, size, rotation, color, 8);
                break;
            case Tetrahedron:
                entity = EntityFactory3D::createTetrahedron(pos, size, rotation, color);
                break;
            case Pyramid:
                entity = EntityFactory3D::createPyramid(pos, size, rotation, color);
                break;
            default:
                THROW_EXCEPTION(UnsupportedEntityShapeType);
        }

        const JPH::BodyID bodyId = app.getPhysicsSystem()->createBodyFromShape(
            entity, Application::m_coordinator->getComponent<components::TransformComponent>(entity), shapeType,
            motionType);
        if (bodyId.IsInvalid()) {
            THROW_EXCEPTION(InvalidBodyId, entity);
        }
        scene.addEntity(entity);
    }

    void EditorScene::addModelToScene(const std::string& modelPath, const glm::vec3& position, const glm::vec3& scale,
                                      const glm::vec3& rotation) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);
        const auto& catalog = nexo::assets::AssetCatalog::getInstance();

        const assets::AssetLocation model(modelPath);
        const auto modelAssetRef = catalog.getAsset(model).as<assets::Model>();
        const ecs::Entity entity = EntityFactory3D::createModel(modelAssetRef, position, scale, rotation);

        scene.addEntity(entity);
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

    void EditorScene::loadDefaultEntities(const glm::vec3& offset) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);
        const auto& catalog = nexo::assets::AssetCatalog::getInstance();

        const assets::AssetLocation fullRoomModel("my_package::full_room@Demo");
        const auto fullRoomAssetRef = catalog.getAsset(fullRoomModel).as<assets::Model>();
        const auto fullRoom         = EntityFactory3D::createModel(
            fullRoomAssetRef, {0.0 + offset.x, 0.0 + offset.y, 0.0 + offset.z}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
        scene.addEntity(fullRoom);

        const auto backWall =
            EntityFactory3D::createCube({10.0f + offset.x, 7.0f + offset.y, 0.0f + offset.z}, {0.5f, 14.0f, 20.0f},
                                        {0.0f, 0.0f, 0.0f}, {0.29f, 0.41f, 0.45f, 1.0f});
        scene.addEntity(backWall);

        const auto leftWall =
            EntityFactory3D::createCube({0.0f + offset.x, 7.0f + offset.y, 10.0f + offset.z}, {20.0f, 14.0f, 0.5f},
                                        {0.0f, 0.0f, 0.0f}, {0.15f, 0.21f, 0.25f, 1.0f});
        scene.addEntity(leftWall);

        const auto rightWall =
            EntityFactory3D::createCube({0.0f + offset.x, 7.0f + offset.y, -10.0f + offset.z}, {20.0f, 14.0f, 0.5f},
                                        {0.0f, 0.0f, 0.0f}, {0.15f, 0.21f, 0.25f, 1.0f});
        scene.addEntity(rightWall);

        const auto [linear, quad] = math::computeAttenuationFromDistance(70.0f);
        const auto pointLightTop  = LightFactory::createPointLight({0.0f + offset.x, 14.0f + offset.y, 0.0f + offset.z},
                                                                   {1, 1, 1}, linear, quad);
        utils::addPropsTo(pointLightTop, utils::PropsType::POINT_LIGHT);
        scene.addEntity(pointLightTop);

        const auto [linear2, quad2] = math::computeAttenuationFromDistance(13.0f);
        const auto deskSpotLight = LightFactory::createSpotLight({7.94f + offset.x, 5.20f + offset.y, 2.75f + offset.z},
                                                                 {-0.30f, -0.30f, -0.30f}, {1.0f, 0.90f, 0.46f},
                                                                 linear2, quad2, 33.50f, 43.0f);
        utils::addPropsTo(deskSpotLight, utils::PropsType::SPOT_LIGHT);
        scene.addEntity(deskSpotLight);

        const auto [linear3, quad3] = math::computeAttenuationFromDistance(100.0f);
        const auto standSpotLightTop =
            LightFactory::createSpotLight({8.48f + offset.x, 7.10f + offset.y, -8.06f + offset.z}, {0.0f, 1.0f, 0.0f},
                                          {1.0f, 1.0f, 0.64f}, linear3, quad3, 55.50f, 75.5f);
        utils::addPropsTo(standSpotLightTop, utils::PropsType::SPOT_LIGHT);
        scene.addEntity(standSpotLightTop);

        const auto standSpotLightBottom =
            LightFactory::createSpotLight({8.48f + offset.x, 6.5f + offset.y, -8.06f + offset.z}, {0.0f, -1.0f, 0.0f},
                                          {1.0f, 1.0f, 0.64f}, linear3, quad3, 16.50f, 43.5f);
        utils::addPropsTo(standSpotLightBottom, utils::PropsType::SPOT_LIGHT);
        scene.addEntity(standSpotLightBottom);

        const auto [linear4, quad4] = math::computeAttenuationFromDistance(50.0f);
        const auto garlandPos       = std::vector<glm::vec3>{{9.16f + offset.x, 5.35f + offset.y, -1.68f + offset.z},
                                                             {9.16f + offset.x, 5.10f + offset.y, -1.16f + offset.z},
                                                             {9.16f + offset.x, 4.95f + offset.y, -0.62f + offset.z},
                                                             {9.16f + offset.x, 4.91f + offset.y, -0.06 + offset.z},
                                                             {9.16f + offset.x, 4.97f + offset.y, 0.50f + offset.z},
                                                             {9.16f + offset.x, 5.12f + offset.y, 1.04f + offset.z},
                                                             {9.16f + offset.x, 5.36f + offset.y, 1.55f + offset.z}};
        const auto garlandColors =
            std::vector<glm::vec3>{{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f},
                                   {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}};
        for (int i = 0; i < garlandPos.size(); ++i) {
            const auto garlandSpotLight = LightFactory::createSpotLight(garlandPos[i], {1.0f, -0.6f, 0.0f},
                                                                        garlandColors[i], linear4, quad4, 20.0f, 30.0f);
            utils::addPropsTo(garlandSpotLight, utils::PropsType::SPOT_LIGHT);
            scene.addEntity(garlandSpotLight);
        }

        const assets::AssetLocation corkBoardModel("my_package::cork_board@Demo");
        const auto corkBoardAssetRef = catalog.getAsset(corkBoardModel).as<assets::Model>();
        const auto corkBoard =
            EntityFactory3D::createModel(corkBoardAssetRef, {9.63f + offset.x, 5.59f + offset.y, 0.0f + offset.z},
                                         {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
        scene.addEntity(corkBoard);

        const assets::AssetLocation frameModel("my_package::frame@Demo");
        const auto frameAssetRef = catalog.getAsset(frameModel).as<assets::Model>();
        const auto frame =
            EntityFactory3D::createModel(frameAssetRef, {9.7f + offset.x, 5.69f + offset.y, 3.96f + offset.z},
                                         {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
        scene.addEntity(frame);

        const assets::AssetLocation duckModel("my_package::duck@Demo");
        const auto duckAssetRef = catalog.getAsset(duckModel).as<assets::Model>();
        const auto duck =
            EntityFactory3D::createModel(duckAssetRef, {7.09f + offset.x, 3.88f + offset.y, -1.07f + offset.z},
                                         {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
        scene.addEntity(duck);

        const assets::AssetLocation laptopModel("my_package::laptop@Demo");
        const auto laptopAssetRef = catalog.getAsset(laptopModel).as<assets::Model>();
        const auto laptop =
            EntityFactory3D::createModel(laptopAssetRef, {7.05f + offset.x, 4.0f + offset.y, 0.0f + offset.z},
                                         {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
        scene.addEntity(laptop);

        const assets::AssetLocation bretzelModel("my_package::bretzel@Demo");
        const auto bretzelAssetRef = catalog.getAsset(bretzelModel).as<assets::Model>();
        const auto bretzel =
            EntityFactory3D::createModel(bretzelAssetRef, {7.4f + offset.x, 4.04f + offset.y, -2.98f + offset.z},
                                         {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
        scene.addEntity(bretzel);
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

        const auto base =
            EntityFactory3D::createCube({0.0f + offset.x, 0.0f + offset.y, 0.0f + offset.z}, {25.0f, 0.7f, 25.0f},
                                        {0.0f, 0.0f, 0.0f}, {0.15f, 0.15f, 0.15f, 1.0f});
        scene.addEntity(base);

        const auto& catalog = nexo::assets::AssetCatalog::getInstance();

        const assets::AssetLocation rubixCubeModel("my_package::RubixCube@Models");
        const auto rubixCubeAssetRef = catalog.getAsset(rubixCubeModel).as<assets::Model>();
        auto rubixCube =
            EntityFactory3D::createModel(rubixCubeAssetRef, {4.1f + offset.x, 2.8f + offset.y, -4.7f + offset.z},
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

    void EditorScene::videoScene(const glm::vec3& offset) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        const auto videoBillboard = EntityFactory3D::createBillboard(
            {0.0f + offset.x, 5.0f + offset.y, 1.0f + offset.z}, {5.3f, 3.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
        components::VideoComponent videoComponent;
        videoComponent.path = nexo::Path::resolvePathRelativeToExe("../resources/videos/WarmUp.mp4").string();
        videoComponent.keyframes = {
            {0.0f, 0.1f, components::KeyframeType::NORMAL},
            {0.1f, 1.25f, components::KeyframeType::TRANSITION},
            {1.25f, 16.15f, components::KeyframeType::LOOP},
            {16.15f, 17.0f, components::KeyframeType::TRANSITION},
            {17.0f, 37.23f, components::KeyframeType::LOOP},
            {37.23f, 38.07f, components::KeyframeType::TRANSITION},
            {38.07f, 82.27f, components::KeyframeType::LOOP},
            {82.27, 84.25f, components::KeyframeType::TRANSITION},
            {84.25f, 85.0f, components::KeyframeType::LOOP},
            {85.0f, 87.03f, components::KeyframeType::TRANSITION},
            {87.03f, 95.18f, components::KeyframeType::LOOP},
            {95.18f, 97.18f, components::KeyframeType::TRANSITION},
            {97.18f, 99.17f, components::KeyframeType::TRANSITION},
            {99.17f, 128.11f, components::KeyframeType::LOOP},
            {128.11f, 130.08f, components::KeyframeType::TRANSITION},
            {130.08f, 137.22f, components::KeyframeType::LOOP},
            {137.22f, 139.16f, components::KeyframeType::TRANSITION},
            {139.16f, 150.00f, components::KeyframeType::LOOP},
            {150.00f, 152.13f, components::KeyframeType::TRANSITION},
            {152.13f, 183.21f, components::KeyframeType::LOOP},
            {183.21f, 185.26f, components::KeyframeType::TRANSITION},
            {185.26f, 202.19f, components::KeyframeType::LOOP},
            {202.19f, 204.25f, components::KeyframeType::TRANSITION},
            {204.25f, 232.05f, components::KeyframeType::LOOP},
        };

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
        auto& materialComp       = nexo::Application::m_coordinator->getComponent<components::MaterialComponent>(floor);
        const auto materialAssetRef             = materialComp.material;
        const auto materialAsset                = materialAssetRef.lock();
        materialAsset->getData()->albedoTexture = grassAssetRef;

        addModelToScene("my_package::Frog@Models", {0.0f + offset.x, 1.0f + offset.y, 0.0f + offset.z},
                        {0.5f, 0.5f, 0.5f});

        addModelToScene("my_package::Tree@Models", {5.0f + offset.x, 0.5f + offset.y, -3.0f + offset.z},
                        {2.44f, 2.44f, 2.44f});

        addModelToScene("my_package::Bench@Models", {-6.0f + offset.x, 2.0f + offset.y, -6.5f + offset.z});

        addModelToScene("my_package::Log@Models", {-5.0f + offset.x, 0.5f + offset.y, 5.0f + offset.z},
                        {2.3f, 2.3f, 2.3f});
    }

} // namespace nexo::editor
