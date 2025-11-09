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
#include <components/Video.hpp>
#include <iomanip>
#include <math/Light.hpp>
#include <random>
#include <sstream>

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
        scene.addEntity(LightFactory::createAmbientLight({1.0f, 1.0f, 1.0f}));

        m_sceneUuid = app.getSceneManager().getScene(m_sceneId).getUuid();
        if (m_defaultScene) {
            loadDefaultEntities();
            // physicScene(glm::vec3{-60.0f, 0.0f, 0.0f});
            // videoScene(glm::vec3{-15.0f, 0.0f, 0.0f});
            // lightsScene(glm::vec3{50.0f, 0.0f, 0.0f});
            // forestScene({100.0f, 1.0f, 0.0f});
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

    ecs::Entity FindEntityByName(const char* name)
    {
        if (!name) {
            LOG(NEXO_ERROR, "NxFindEntityByName: name is null");
            return static_cast<ecs::Entity>(-1);
        }

        auto& app            = Application::getInstance();
        auto& coordinator    = *Application::m_coordinator;
        const auto& scene    = app.getSceneManager().getScene(0);
        const auto& entities = scene.getEntities();

        for (const auto entity : entities) {
            if (coordinator.entityHasComponent<components::NameComponent>(entity)) {
                const auto& nameComponent = coordinator.getComponent<components::NameComponent>(entity);
                if (nameComponent.name == name) {
                    return entity;
                }
            }
        }

        LOG(NEXO_WARN, "NxFindEntityByName: Entity with name '{}' not found", name);
        return static_cast<ecs::Entity>(-1);
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
            EntityFactory3D::createModel(corkBoardAssetRef, {9.63f + offset.x, 5.69f + offset.y, 0.0f + offset.z},
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

        // Get dominos pos
        std::vector<glm::vec3> dominoPositions = {
            {8.21f, 4.04f, -3.74}, {8.21f, 4.04f, -3.64}, {8.16f, 4.04f, -3.54}, {8.09f, 4.04f, -3.48},
            {8.00f, 4.04f, -3.43}, {7.90f, 4.05f, -3.40}, {7.81f, 4.05f, -3.38}, {7.71f, 4.05f, -3.36},
            {7.61f, 4.05f, -3.35}, {7.51f, 4.05f, -3.33}, {7.41f, 4.05f, -3.33}, {7.31f, 4.05f, -3.32},
            {7.21f, 4.05f, -3.31}, {7.11f, 4.05f, -3.31}, {7.01f, 4.05f, -3.30}, {6.91f, 4.05f, -3.30},
            {6.81f, 4.05f, -3.30}, {6.71f, 4.05f, -3.30}, {6.61f, 4.05f, -3.30}, {6.51f, 4.05f, -3.30},
            {6.40f, 4.05f, -3.30}, {6.30f, 4.05f, -3.30}, {6.20f, 4.05f, -3.30}, {6.11f, 4.05f, -3.26},
            {6.02f, 4.05f, -3.22}, {5.92f, 4.05f, -3.19}, {5.83f, 4.05f, -3.14}, {5.75f, 4.05f, -3.09},
            {5.67f, 4.06f, -3.03}, {5.64f, 4.06f, -2.94}, {5.70f, 4.06f, -2.86}, {5.79f, 4.06f, -2.80},
            {5.88f, 4.06f, -2.76}, {5.97f, 4.06f, -2.72}, {6.06f, 4.06f, -2.67}, {6.15f, 4.06f, -2.63},
            {6.24f, 4.07f, -2.58}, {6.33f, 4.07f, -2.53}, {6.40f, 4.07f, -2.46}, {6.46f, 4.07f, -2.39},
            {6.48f, 4.07f, -2.29}, {6.44f, 4.07f, -2.20}, {6.39f, 4.07f, -2.11}, {6.31f, 4.07f, -2.05},
            {6.24f, 4.07f, -1.98}, {6.17f, 4.07f, -1.91}, {6.11f, 4.07f, -1.83}, {6.05f, 4.07f, -1.74},
            {5.99f, 4.07f, -1.66}, {5.95f, 4.07f, -1.57}, {5.90f, 4.07f, -1.48}, {5.87f, 4.07f, -1.39},
            {5.85f, 4.07f, -1.29}, {5.90f, 4.07f, -1.20}, {5.99f, 4.06f, -1.18}, {6.09f, 4.06f, -1.18},
            {6.19f, 4.06f, -1.20}, {6.29f, 4.06f, -1.23}, {6.38f, 4.06f, -1.26}, {6.48f, 4.06f, -1.29},
            {6.57f, 4.06f, -1.33}, {6.66f, 4.06f, -1.37}, {6.75f, 4.05f, -1.41}, {6.84f, 4.05f, -1.46},
            {6.93f, 4.05f, -1.50}, {7.02f, 4.05f, -1.55}, {7.11f, 4.05f, -1.60}, {7.20f, 4.05f, -1.65},
            {7.29f, 4.05f, -1.69}, {7.39f, 4.05f, -1.70}, {7.49f, 4.05f, -1.70}, {7.59f, 4.05f, -1.71},
            {7.69f, 4.05f, -1.72}, {7.79f, 4.05f, -1.73}, {7.89f, 4.05f, -1.74}, {7.99f, 4.05f, -1.74},
            {8.09f, 4.05f, -1.74}, {8.19f, 4.05f, -1.73}, {8.29f, 4.05f, -1.72}, {8.38f, 4.05f, -1.69},
            {8.44f, 4.04f, -1.62}, {8.43f, 4.04f, -1.52}, {8.38f, 4.04f, -1.44}, {8.32f, 4.04f, -1.36},
            {8.25f, 4.04f, -1.29}, {8.19f, 4.04f, -1.21}, {8.12f, 4.04f, -1.14}, {8.05f, 4.04f, -1.06},
            {7.98f, 4.04f, -0.99}, {7.92f, 4.04f, -0.91}, {7.87f, 4.04f, -0.82}, {7.83f, 4.04f, -0.73},
            {7.83f, 4.04f, -0.63}, {7.85f, 4.04f, -0.54}, {7.93f, 4.04f, -0.48}, {8.01f, 4.04f, -0.42},
            {8.11f, 4.04f, -0.39}, {8.21f, 4.04f, -0.36}, {8.30f, 4.04f, -0.34}, {8.40f, 4.04f, -0.31},
            {8.49f, 4.04f, -0.28}, {8.58f, 4.04f, -0.23}, {8.66f, 4.04f, -0.17}, {8.72f, 4.04f, -0.09},
            {8.78f, 4.04f, -0.00}, {8.82f, 4.04f, 0.09},  {8.85f, 4.04f, 0.18},  {8.87f, 4.04f, 0.28},
            {8.88f, 4.05f, 0.38},  {8.88f, 4.05f, 0.48},  {8.87f, 4.05f, 0.58},  {8.85f, 4.05f, 0.68},
            {8.82f, 4.05f, 0.77},  {8.77f, 4.05f, 0.86},  {8.72f, 4.05f, 0.95},  {8.66f, 4.05f, 1.03},
            {8.59f, 4.05f, 1.10},  {8.52f, 4.05f, 1.17},  {8.44f, 4.05f, 1.23},  {8.35f, 4.05f, 1.29},
            {8.27f, 4.06f, 1.33},  {8.17f, 4.06f, 1.37},  {8.08f, 4.06f, 1.41},  {7.99f, 4.06f, 1.45},
            {7.89f, 4.06f, 1.47},  {7.79f, 4.06f, 1.49},  {7.69f, 4.06f, 1.51},  {7.60f, 4.06f, 1.54},
            {7.50f, 4.06f, 1.57},  {7.40f, 4.06f, 1.60},  {7.30f, 4.06f, 1.61},  {7.20f, 4.06f, 1.61},
            {7.10f, 4.06f, 1.61},  {7.00f, 4.07f, 1.61},  {6.90f, 4.07f, 1.61},  {6.80f, 4.07f, 1.61},
            {6.70f, 4.07f, 1.60},  {6.60f, 4.07f, 1.61},  {6.50f, 4.07f, 1.61},  {6.40f, 4.07f, 1.63},
            {6.30f, 4.07f, 1.65},  {6.21f, 4.07f, 1.68},  {6.12f, 4.07f, 1.72},  {6.03f, 4.07f, 1.77},
            {5.96f, 4.07f, 1.84},  {5.89f, 4.07f, 1.91},  {5.83f, 4.07f, 1.99},  {5.77f, 4.07f, 2.08},
            {5.73f, 4.07f, 2.17},  {5.68f, 4.07f, 2.26},  {5.66f, 4.07f, 2.35},  {5.63f, 4.07f, 2.45},
            {5.62f, 4.07f, 2.55},  {5.62f, 4.07f, 2.65},  {5.62f, 4.07f, 2.75},  {5.65f, 4.07f, 2.85},
            {5.68f, 4.07f, 2.94},  {5.72f, 4.07f, 3.04},  {5.77f, 4.07f, 3.12},  {5.84f, 4.07f, 3.19},
            {5.91f, 4.07f, 3.26},  {6.00f, 4.07f, 3.32},  {6.09f, 4.07f, 3.36},  {6.18f, 4.07f, 3.39},
            {6.28f, 4.07f, 3.41},  {6.38f, 4.07f, 3.43},  {6.48f, 4.07f, 3.44},  {6.58f, 4.07f, 3.46},
            {6.68f, 4.07f, 3.45},  {6.78f, 4.07f, 3.44},  {6.88f, 4.07f, 3.43},  {6.97f, 4.07f, 3.39},
            {7.06f, 4.07f, 3.35},  {7.14f, 4.07f, 3.29},  {7.21f, 4.07f, 3.22},  {7.28f, 4.07f, 3.14},
            {7.33f, 4.07f, 3.06},  {7.39f, 4.07f, 2.97},  {7.43f, 4.07f, 2.88},  {7.47f, 4.07f, 2.79},
            {7.51f, 4.07f, 2.70},  {7.54f, 4.07f, 2.60},  {7.58f, 4.07f, 2.51},  {7.62f, 4.07f, 2.42},
            {7.66f, 4.07f, 2.32},  {7.70f, 4.07f, 2.23},  {7.74f, 4.07f, 2.14},  {7.79f, 4.07f, 2.06},
            {7.85f, 4.07f, 1.97},  {7.92f, 4.07f, 1.90},  {7.99f, 4.07f, 1.83},  {8.08f, 4.07f, 1.78},
            {8.17f, 4.07f, 1.73},  {8.26f, 4.07f, 1.70},  {8.36f, 4.07f, 1.68},  {8.46f, 4.07f, 1.67},
            {8.56f, 4.07f, 1.66},  {8.66f, 4.07f, 1.64},  {8.76f, 4.07f, 1.63}};

        for (auto pos : dominoPositions) {
            pos += offset;
            const assets::AssetLocation dominoModel("my_package::domino@Demo");
            const auto dominoAssetRef = catalog.getAsset(dominoModel).as<assets::Model>();
            const auto domino =
                EntityFactory3D::createModel(dominoAssetRef, pos, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
            const JPH::BodyID bodyId = app.getPhysicsSystem()->createBodyFromShape(
            domino, Application::m_coordinator->getComponent<components::TransformComponent>(domino),system::ShapeType::Box,
            JPH::EMotionType::Dynamic);
            if (bodyId.IsInvalid()) {
                THROW_EXCEPTION(InvalidBodyId, domino);
            }
            scene.addEntity(domino);
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

    void EditorScene::physicScene(const glm::vec3& offset) const
    {
        // Background
        createEntityWithPhysic({0.0f + offset.x, 40.0f + offset.y, -2.5f + offset.z}, {44.0f, 80.0f, 0.5f}, {0, 0, 0},
                               {0.91f, 0.91f, 0.91f, 1.0f}, system::ShapeType::Box, JPH::EMotionType::Static);

        // Funnel left and right
        createEntityWithPhysic({-6.0f + offset.x, 70.0f + offset.y, 0.0f + offset.z}, {10.0f, 0.5f, 4.0f},
                               {0, 0, -45.0f}, {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box,
                               JPH::EMotionType::Static);
        createEntityWithPhysic({6.0f + offset.x, 70.0f + offset.y, 0.0f + offset.z}, {10.0f, 0.5f, 4.0f}, {0, 0, 45.0f},
                               {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box, JPH::EMotionType::Static);

        // Stairs right
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec4>> stairs = {
            {{3.0f, 61.5f, 0.0f}, {5.0f, 0.5f, 4.0f}, {0, 0, -15.0f}, {0.0f, 0.28f, 0.47f, 1.0f}},
            {{11.0f, 58.5f, 0.0f}, {8.0f, 0.5f, 4.0f}, {0.0f, 0.0f, 20.0f}, {0.0f, 0.28f, 0.47f, 1.0f}},
            {{3.0f, 55.5f, 0.0f}, {5.0f, 0.5f, 4.0f}, {0.0f, 0.0f, -15.0f}, {0.0f, 0.28f, 0.47f, 1.0f}},
            {{10.0f, 52.5f, 0.0f}, {12.0f, 0.5f, 4.0f}, {0.0f, 0.0f, 20.0f}, {0.0f, 0.28f, 0.47f, 1.0f}}};
        for (const auto& [pos, size, rotation, color] : stairs) {
            createEntityWithPhysic(pos + offset, size, rotation, color, system::ShapeType::Box,
                                   JPH::EMotionType::Static);
        }

        // Tunnel left
        std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3>> tunnels = {
            {{-6.0f, 59.0f, 0.0f}, {3.0f, 11.0f, 4.0f}, {0, 0, 0}},
            {{-1.0f, 58.5f, 0.0f}, {3.0f, 8.0f, 4.0f}, {0, 0, 0}},
            {{-5.0f, 51.0f, 0.0f}, {9.0f, 0.5f, 4.0f}, {0, 0, -25.0f}}};
        for (const auto& [pos, size, rotation] : tunnels) {
            createEntityWithPhysic(pos + offset, size, rotation, {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box,
                                   JPH::EMotionType::Static);
        }
        // Dominos left and right
        createEntityWithPhysic({-9.0f + offset.x, 44.0f + offset.y, 0.0f + offset.z}, {20.9f, 0.5f, 4.0f}, {0, 0, 0.0f},
                               {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box, JPH::EMotionType::Static);
        createEntityWithPhysic({11.15f + offset.x, 44.0f + offset.y, 0.0f + offset.z}, {15.5f, 0.5f, 4.0f},
                               {0, 0, 0.0f}, {0.0f, 0.28f, 0.47f, 1.0f}, system::ShapeType::Box,
                               JPH::EMotionType::Static);

        for (int i = 0; i < 24; ++i) {
            if (i == 13) continue;
            const float x              = -18.4f + static_cast<float>(i) * 1.6f;
            glm::vec3 pos              = {x, 45.5f, 0.0f};
            const float gradientFactor = static_cast<float>(i) / 24.0f;
            glm::vec4 color =
                mix(glm::vec4(0.0f, 0.77f, 0.95f, 1.0f), glm::vec4(0.83f, 0.14f, 0.67f, 1.0f), gradientFactor);
            createEntityWithPhysic(pos + offset, {0.25f, 3.0f, 3.0f}, {0, 0, 0}, color, system::ShapeType::Box,
                                   JPH::EMotionType::Dynamic);
        }

        // Fakir
        constexpr int totalRows = 20;
        constexpr float startX  = -14.0f;

        for (int row = 0; row < totalRows; ++row) {
            constexpr int cols = 10;
            for (int col = 0; col < cols; ++col) {
                constexpr float startY     = 14.0f;
                constexpr float spacing    = 3.0f;
                const float offsetX        = (row % 2 == 0) ? 0.0f : spacing / 2.0f;
                glm::vec3 pos              = {static_cast<float>(col) * spacing + startX + offsetX,
                                              startY + static_cast<float>(row) * 1.2f, 0.0f};
                constexpr auto maxFactor   = static_cast<float>(totalRows * cols);
                const float gradientFactor = static_cast<float>((row + 1) * (col + 1)) / maxFactor;
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

        const auto videoBillboard = EntityFactory3D::createBillboard(
            {0.0f + offset.x, 5.0f + offset.y, 1.0f + offset.z}, {5.3f, 3.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
        components::VideoComponent videoComponent;
        videoComponent.path      = nexo::Path::resolvePathRelativeToExe("../resources/videos/_Warmup.mp4").string();
        videoComponent.keyframes = {
            {0.0f, 0.1f, components::KeyframeType::NORMAL},
            {0.01f, 1.25f, components::KeyframeType::TRANSITION},
            {1.25f, 16.15f, components::KeyframeType::LOOP},
            {16.15f, 17.00f, components::KeyframeType::TRANSITION},
            {17.00f, 37.23f, components::KeyframeType::LOOP},
            {37.23f, 38.07f, components::KeyframeType::TRANSITION},
            {38.07f, 82.27f, components::KeyframeType::LOOP},
            {82.27f, 84.25f, components::KeyframeType::TRANSITION},
            {84.25f, 85.00f, components::KeyframeType::NORMAL},
            {85.00f, 87.03f, components::KeyframeType::TRANSITION},
            {87.03f, 95.15f, components::KeyframeType::LOOP},
            {95.15f, 97.15f, components::KeyframeType::TRANSITION},
            {97.15f, 109.13f, components::KeyframeType::LOOP},
            {109.13f, 111.07f, components::KeyframeType::TRANSITION},
            {111.07f, 140.04f, components::KeyframeType::LOOP},
            {138.07f, 140.04f, components::KeyframeType::TRANSITION},
            {142.01f, 149.15f, components::KeyframeType::LOOP},
            {149.15f, 151.09f, components::KeyframeType::TRANSITION},
            {151.09f, 161.53f, components::KeyframeType::LOOP},
            {161.53f, 164.06f, components::KeyframeType::TRANSITION},
            {164.06f, 195.14f, components::KeyframeType::LOOP},
            {195.14f, 197.19f, components::KeyframeType::TRANSITION},
            {197.19f, 214.12f, components::KeyframeType::LOOP},
            {214.12f, 216.18f, components::KeyframeType::TRANSITION},
            {216.18f, 243.81f, components::KeyframeType::LOOP},
            {243.81f, 245.81f, components::KeyframeType::TRANSITION},
            {245.81f, 262.00f, components::KeyframeType::NORMAL},

        };

        Application::m_coordinator->addComponent(videoBillboard, videoComponent);

        scene.addEntity(videoBillboard);
    }

    void EditorScene::forestScene(const glm::vec3& offset) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        const auto floor =
            EntityFactory3D::createCube({0.0f + offset.x, 0.0f + offset.y, 0.0f + offset.z}, {20.0f, 1.0f, 20.0f},
                                        {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
        scene.addEntity(floor);

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
