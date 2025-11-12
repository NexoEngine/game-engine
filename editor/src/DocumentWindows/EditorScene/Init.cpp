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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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

    void EditorScene::setupDemoCamera()
    {
        auto& app = getApp();

        renderer::NxFramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {renderer::NxFrameBufferTextureFormats::RGBA8,
                                        renderer::NxFrameBufferTextureFormats::RED_INTEGER,
                                        renderer::NxFrameBufferTextureFormats::Depth};
        framebufferSpecs.width       = static_cast<unsigned int>(m_contentSize.x);
        framebufferSpecs.height      = static_cast<unsigned int>(m_contentSize.y);
        const auto renderTarget      = renderer::NxFramebuffer::create(framebufferSpecs);
        ecs::Entity demoCamera =
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
            fullScene();
            dominoScene({7.5f, 4.07f, -0.5f});
            lightsScene(glm::vec3{0.0f, 0.0f, 0.0f});
            // chambouleScene(glm::vec3{0.0f, 0.0f, 0.0f});
            // physicScene(glm::vec3{-60.0f, 0.0f, 0.0f});
            //videoScene(glm::vec3{0.0f, 0.0f, 0.0f});
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

        LOG(NEXO_WARN, "FindEntityByName: Entity with name '{}' not found", name);
        return static_cast<ecs::Entity>(-1);
    }

    void EditorScene::loadDefaultEntities(const glm::vec3& offset) const
    {
    }

    void EditorScene::fullScene(const glm::vec3& offset) const
    {
        auto& app           = getApp();

        //// Import and add to scene models
        addModelToScene("my_package::full_room@Demo", {0.0f + offset.x, 0.0f + offset.y, 0.0f + offset.z});
        addModelToScene("my_package::desk@Demo", {7.44f + offset.x, 1.93f + offset.y, 0.0f + offset.z});
        addModelToScene("my_package::shelf@Demo", {8.86f + offset.x, 5.60f + offset.y, 0.0f + offset.z});

        //// add physics body to some models
        ecs::Entity desk = FindEntityByName("desk");
        if (Application::m_coordinator->entityHasComponent<components::ParentComponent>(desk)) {
            desk = Application::m_coordinator->getComponent<components::ParentComponent>(desk).parent;
        }
        auto rootCompDesk = Application::m_coordinator->getComponent<components::RootComponent>(desk);
        auto modelDesk = rootCompDesk.modelRef.lock();
        auto boundingBoxDesk         = modelDesk->rootBounds;
        auto transformCompDesk =
            Application::m_coordinator->getComponent<components::TransformComponent>(desk);
        const JPH::BodyID bodyIdDesk = app.getPhysicsSystem()->createBodyFromBounds(desk, transformCompDesk, boundingBoxDesk,
                                                                                JPH::EMotionType::Static);
        if (bodyIdDesk.IsInvalid()) {
            THROW_EXCEPTION(InvalidBodyId, desk);
        }

        ecs::Entity shelf = FindEntityByName("shelf");
        if (Application::m_coordinator->entityHasComponent<components::ParentComponent>(shelf)) {
            shelf = Application::m_coordinator->getComponent<components::ParentComponent>(shelf).parent;
        }
        auto rootCompShelf = Application::m_coordinator->getComponent<components::RootComponent>(shelf);
        auto modelShelf = rootCompShelf.modelRef.lock();
        auto boundingBoxShelf         = modelShelf->rootBounds;
        auto transformCompShelf =
            Application::m_coordinator->getComponent<components::TransformComponent>(shelf);
        const JPH::BodyID bodyIdShelf = app.getPhysicsSystem()->createBodyFromBounds(shelf, transformCompShelf, boundingBoxShelf,
                                                                                JPH::EMotionType::Static);
        if (bodyIdShelf.IsInvalid()) {
            THROW_EXCEPTION(InvalidBodyId, shelf);
        }

        auto &catalog = assets::AssetCatalog::getInstance();

        std::array<assets::AssetRef<assets::Texture>, 10> polaTextures = {
            nullptr,
            catalog.getAsset(assets::AssetLocation("my_package::mehdy@Textures/Picture")).as<assets::Texture>(),
            catalog.getAsset(assets::AssetLocation("my_package::baz@Textures/Picture")).as<assets::Texture>(),
            catalog.getAsset(assets::AssetLocation("my_package::fdlm@Textures/Picture")).as<assets::Texture>(),
            catalog.getAsset(assets::AssetLocation("my_package::guillaume@Textures/Picture")).as<assets::Texture>(),
            catalog.getAsset(assets::AssetLocation("my_package::guizmo@Textures/Picture")).as<assets::Texture>(),
            catalog.getAsset(assets::AssetLocation("my_package::jean@Textures/Picture")).as<assets::Texture>(),
            catalog.getAsset(assets::AssetLocation("my_package::kevin@Textures/Picture")).as<assets::Texture>(),
            catalog.getAsset(assets::AssetLocation("my_package::kaben@Textures/Picture")).as<assets::Texture>(),
            catalog.getAsset(assets::AssetLocation("my_package::leo@Textures/Picture")).as<assets::Texture>(),
        };

        //// add pictures in frames
        for (unsigned int i = 1; i <= 9; ++i) {
            std::string frameName = std::format("pola{}-1", i);
            ecs::Entity frameEntity = FindEntityByName(frameName.c_str());
            auto &materialComponent = Application::m_coordinator->getComponent<components::MaterialComponent>(frameEntity);
            auto matRef = materialComponent.material.lock();
            auto &material = matRef->getData();
            material->albedoTexture = polaTextures[i];
        }

        std::string frameName = "frame_pic-1";
        ecs::Entity frameEntity = FindEntityByName(frameName.c_str());
        auto &materialComponentFrame = Application::m_coordinator->getComponent<components::MaterialComponent>(frameEntity);
        auto matRefFrame = materialComponentFrame.material.lock();
        auto &materialFrame = matRefFrame->getData();
        auto cathedralAssetRef = catalog.getAsset(assets::AssetLocation("my_package::cathedral@Textures/Picture")).as<assets::Texture>();
        materialFrame->albedoTexture = cathedralAssetRef;

        std::string deskFrame = "desk_frame_pic-1";
        ecs::Entity deskFrameEntity = FindEntityByName(deskFrame.c_str());
        auto &materialComponentDeskFrame = Application::m_coordinator->getComponent<components::MaterialComponent>(deskFrameEntity);
        auto matRefDeskFrame = materialComponentDeskFrame.material.lock();
        auto &materialDeskFrame = matRefDeskFrame->getData();
        auto thomasGuigzAssetRef = catalog.getAsset(assets::AssetLocation("my_package::Thomas_Guigs@Textures/Picture")).as<assets::Texture>();
        materialDeskFrame->albedoTexture = thomasGuigzAssetRef;
    }

    void EditorScene::lightsScene(const glm::vec3& offset) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

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

        // multicolored points lights (garland)
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
    }

    void EditorScene::scriptedLightsOnModelsScene(const glm::vec3& offset) const
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

    void EditorScene::fakirGameScene(const glm::vec3& offset) const
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

    void EditorScene::dominoScene(const glm::vec3& offset) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);
        const auto& catalog = nexo::assets::AssetCatalog::getInstance();

        createEntityWithPhysic({9.25f, 5.78f, 1.56f}, {0.25f, 0.14f, 0.25f}, {0, 0, 0},
                       {0.8f, 0.5f, 0.1f, 1.0f}, system::ShapeType::Pyramid, JPH::EMotionType::Static);
        createEntityWithPhysic({9.06f, 5.85f, 1.6f}, {0.08f, 0.08f, 0.08f}, {0, 0, 0},
                               {0.80f, 0.8f, 0.8f, 1.0f}, system::ShapeType::Sphere, JPH::EMotionType::Dynamic);

        //// Get dominos pos
        std::vector<glm::vec3> dominosPositions = {
            {0.89f, 0.00f, -3.25f},  {0.87f, 0.00f, -3.13f},  {0.81f, 0.00f, -3.03f},  {0.70f, 0.00f, -2.98f},
            {0.59f, 0.00f, -2.96f},  {0.47f, 0.00f, -2.96f},  {0.35f, 0.00f, -2.98f},  {0.23f, 0.00f, -3.00f},
            {0.12f, 0.00f, -3.03f},  {0.00f, 0.00f, -3.07f},  {-0.11f, 0.00f, -3.11f}, {-0.22f, 0.00f, -3.15f},
            {-0.34f, 0.00f, -3.19f}, {-0.45f, 0.00f, -3.22f}, {-0.57f, 0.00f, -3.23f}, {-0.69f, 0.00f, -3.22f},
            {-0.80f, 0.00f, -3.19f}, {-0.91f, 0.00f, -3.14f}, {-1.01f, 0.00f, -3.08f}, {-1.10f, 0.00f, -3.00f},
            {-1.18f, 0.00f, -2.91f}, {-1.25f, 0.00f, -2.81f}, {-1.30f, 0.00f, -2.70f}, {-1.33f, 0.00f, -2.59f},
            {-1.35f, 0.00f, -2.47f}, {-1.35f, 0.00f, -2.35f}, {-1.34f, 0.00f, -2.23f}, {-1.30f, 0.00f, -2.12f},
            {-1.26f, 0.00f, -2.01f}, {-1.19f, 0.00f, -1.91f}, {-1.12f, 0.00f, -1.81f}, {-1.03f, 0.00f, -1.73f},
            {-0.93f, 0.00f, -1.67f}, {-0.83f, 0.00f, -1.61f}, {-0.72f, 0.00f, -1.56f}, {-0.61f, 0.00f, -1.51f},
            {-0.50f, 0.00f, -1.48f}, {-0.38f, 0.00f, -1.44f}, {-0.27f, 0.00f, -1.41f}, {-0.15f, 0.00f, -1.38f},
            {-0.03f, 0.00f, -1.35f}, {0.08f, 0.00f, -1.32f},  {0.20f, 0.00f, -1.29f},  {0.31f, 0.00f, -1.25f},
            {0.42f, 0.00f, -1.21f},  {0.53f, 0.00f, -1.16f},  {0.63f, 0.00f, -1.10f},  {0.73f, 0.00f, -1.03f},
            {0.81f, 0.00f, -0.94f},  {0.88f, 0.00f, -0.85f},  {0.94f, 0.00f, -0.74f},  {0.99f, 0.00f, -0.64f},
            {1.04f, 0.00f, -0.52f},  {1.07f, 0.00f, -0.41f},  {1.11f, 0.00f, -0.30f},  {1.13f, 0.00f, -0.18f},
            {1.16f, 0.00f, -0.06f},  {1.18f, 0.00f, 0.06f},   {1.19f, 0.00f, 0.17f},   {1.20f, 0.00f, 0.29f},
            {1.20f, 0.00f, 0.41f},   {1.21f, 0.00f, 0.53f},   {1.20f, 0.00f, 0.65f},   {1.19f, 0.00f, 0.77f},
            {1.18f, 0.00f, 0.89f},   {1.16f, 0.00f, 1.01f},   {1.13f, 0.00f, 1.12f},   {1.10f, 0.00f, 1.24f},
            {1.05f, 0.00f, 1.35f},   {1.00f, 0.00f, 1.46f},   {0.94f, 0.00f, 1.55f},   {0.86f, 0.00f, 1.65f},
            {0.77f, 0.00f, 1.72f},   {0.66f, 0.00f, 1.78f},   {0.56f, 0.00f, 1.83f},   {0.44f, 0.00f, 1.87f},
            {0.33f, 0.00f, 1.89f},   {0.21f, 0.00f, 1.91f},   {0.09f, 0.00f, 1.92f},   {-0.03f, 0.00f, 1.93f},
            {-0.15f, 0.00f, 1.93f},  {-0.27f, 0.00f, 1.94f},  {-0.39f, 0.00f, 1.94f},  {-0.51f, 0.00f, 1.95f},
            {-0.63f, 0.00f, 1.96f},  {-0.75f, 0.00f, 1.98f},  {-0.86f, 0.00f, 2.01f},  {-0.98f, 0.00f, 2.04f},
            {-1.09f, 0.00f, 2.09f},  {-1.19f, 0.00f, 2.15f},  {-1.28f, 0.00f, 2.22f},  {-1.36f, 0.00f, 2.31f},
            {-1.43f, 0.00f, 2.40f},  {-1.49f, 0.00f, 2.51f},  {-1.52f, 0.00f, 2.62f},  {-1.54f, 0.00f, 2.74f},
            {-1.54f, 0.00f, 2.86f},  {-1.53f, 0.00f, 2.98f},  {-1.50f, 0.00f, 3.09f},  {-1.46f, 0.00f, 3.21f},
            {-1.41f, 0.00f, 3.31f},  {-1.34f, 0.00f, 3.41f},  {-1.26f, 0.00f, 3.50f},  {-1.17f, 0.00f, 3.58f},
            {-1.07f, 0.00f, 3.63f},  {-0.96f, 0.00f, 3.68f},  {-0.84f, 0.00f, 3.70f},  {-0.72f, 0.00f, 3.72f},
            {-0.61f, 0.00f, 3.70f},  {-0.49f, 0.00f, 3.69f},  {-0.38f, 0.00f, 3.64f},  {-0.27f, 0.00f, 3.60f},
            {-0.17f, 0.00f, 3.53f},  {-0.08f, 0.00f, 3.45f},  {-0.01f, 0.00f, 3.36f},  {0.03f, 0.00f, 3.25f},
            {0.07f, 0.00f, 3.13f},   {0.10f, 0.00f, 3.02f},   {0.11f, 0.00f, 2.90f},   {0.13f, 0.00f, 2.78f},
            {0.15f, 0.00f, 2.66f},   {0.17f, 0.00f, 2.54f},   {0.20f, 0.00f, 2.43f},   {0.24f, 0.00f, 2.32f},
            {0.30f, 0.00f, 2.21f},   {0.38f, 0.00f, 2.13f},   {0.49f, 0.00f, 2.08f},   {0.60f, 0.00f, 2.05f}};

        std::vector<glm::vec3> dominosRotation = {
            {0.0727f, 0.0332f, 0.8405f},     {0.0000f, -9.4016f, 0.0000f},   {0.0291f, -21.6951f, 0.8302f},
            {0.1246f, -38.1306f, 0.7617f},   {1.0381f, 36.4383f, 0.6043f},   {0.9172f, 23.9981f, 0.3586f},
            {0.8743f, 15.8196f, 0.2182f},    {0.8566f, 10.8646f, 0.1194f},   {0.8493f, 7.9136f, 0.1003f},
            {0.8483f, 7.4345f, 0.0828f},     {0.8495f, 8.1157f, 0.0969f},    {0.8505f, 9.8723f, 0.1262f},
            {0.8674f, 14.1957f, 0.1645f},    {0.8964f, 20.3253f, 0.2958f},   {0.9510f, 28.5394f, 0.4157f},
            {1.0577f, 37.2697f, 0.5775f},    {0.0267f, -38.1087f, 0.8241f},  {-0.0091f, -30.2687f, 0.8402f},
            {0.0309f, -21.9459f, 0.8294f},   {-0.0106f, -17.7086f, 0.8359f}, {0.0240f, -8.2565f, 0.8374f},
            {0.0907f, -3.0771f, 0.8324f},    {0.0290f, 7.8809f, 0.8448f},    {-0.0116f, 14.2850f, 0.8312f},
            {0.0611f, 25.6295f, 0.8671f},    {-0.0125f, 32.2510f, 0.8319f},  {0.0731f, 40.7691f, 0.8896f},
            {-1.0810f, -38.9626f, 0.6900f},  {-0.9779f, -31.2953f, 0.5343f}, {-0.9215f, -24.1329f, 0.4209f},
            {-0.8884f, -19.1666f, 0.2829f},  {-0.8588f, -11.4828f, 0.2262f}, {-0.8414f, -2.6407f, 0.0491f},
            {-0.8373f, 4.8189f, -0.0479f},   {-0.8627f, 12.9370f, -0.1459f}, {-0.8859f, 18.9356f, -0.2985f},
            {-0.9526f, 27.9024f, -0.3906f},  {-1.0761f, 38.7212f, -0.6829f}, {-1.1869f, 44.8683f, -0.7643f},
            {-0.0282f, -34.8947f, -0.8247f}, {0.0092f, -29.5323f, -0.8431f}, {-0.0594f, -21.4475f, -0.8192f},
            {-0.0082f, -15.8607f, -0.8384f}, {0.0042f, -12.9955f, -0.8405f}, {-0.0268f, -8.2092f, -0.8372f},
            {-0.0032f, -5.8879f, -0.8402f},  {-0.0282f, -4.1591f, -0.8390f}, {-0.0103f, -3.6382f, -0.8401f},
            {-0.0158f, -3.5443f, -0.8400f},  {-0.0039f, -4.3453f, -0.8403f}, {-0.0141f, -6.0198f, -0.8395f},
            {-0.0156f, -8.2250f, -0.8389f},  {0.0051f, -12.0413f, -0.8414f}, {-0.0345f, -16.7032f, -0.8314f},
            {-0.0072f, -22.7727f, -0.8344f}, {0.0100f, -29.7364f, -0.8452f}, {-0.0688f, -37.6155f, -0.7991f},
            {-1.1602f, 43.9598f, -0.7955f},  {-1.0401f, 36.0274f, -0.5959f}, {-0.9547f, 28.3296f, -0.3771f},
            {-0.9118f, 22.8067f, -0.3561f},  {-0.8832f, 17.7559f, -0.2159f}, {-0.8644f, 13.6084f, -0.2098f},
            {-0.8543f, 10.0485f, -0.1066f},  {-0.8467f, 6.9596f, -0.1047f},  {-0.8434f, 4.1787f, -0.0216f},
            {-0.8411f, 1.6994f, -0.0209f},   {-0.8412f, -0.6668f, 0.0509f},  {-0.8419f, -2.8334f, 0.0514f},
            {-0.8443f, -4.9849f, 0.1182f},   {-0.8474f, -7.0598f, 0.1190f},  {-0.8505f, -9.1331f, 0.1191f},
            {-0.8578f, -11.3211f, 0.1873f},  {0.0000f, -0.0000f, 0.0000f},   {-0.8750f, -16.0017f, 0.2629f},
            {-0.8858f, -18.5562f, 0.2658f},  {-0.9053f, -21.6954f, 0.3558f}, {-0.9314f, -25.7155f, 0.4079f},
            {-0.9668f, -29.5624f, 0.4872f},  {-1.0211f, -34.6167f, 0.5862f}, {-1.1328f, -42.2359f, 0.7703f},
            {0.0630f, 39.9460f, 0.8817f},    {0.0135f, 31.7931f, 0.8460f},   {0.0146f, 26.0571f, 0.8475f},
            {0.0546f, 21.3585f, 0.8610f},    {0.0046f, 18.1617f, 0.8422f},   {0.0335f, 16.0524f, 0.8505f},
            {0.0109f, 14.5519f, 0.8436f},    {0.0260f, 13.9992f, 0.8474f},   {0.0218f, 13.7308f, 0.8462f},
            {0.0233f, 14.3669f, 0.8468f},    {0.0000f, 15.1841f, 0.0000f},   {0.0244f, 17.0475f, 0.8482f},
            {0.0558f, 19.0501f, 0.8591f},    {0.0319f, 22.5121f, 0.8533f},   {0.0296f, 26.2563f, 0.8515f},
            {0.0561f, 31.7312f, 0.8704f},    {-0.0116f, 35.7215f, 0.8322f},  {0.0762f, 43.7010f, 0.8943f},
            {-1.0390f, -36.0589f, 0.6071f},  {-0.9672f, -29.6538f, 0.5620f}, {-0.8922f, -19.6375f, 0.3026f},
            {-0.8566f, -12.9521f, 0.2009f},  {-0.8431f, -4.1944f, 0.1081f},  {-0.8423f, 3.6343f, -0.0398f},
            {-0.8502f, 9.2160f, -0.1464f},   {-0.8889f, 19.7654f, -0.2787f}, {-0.9454f, 27.1491f, -0.3763f},
            {-1.0463f, 36.5278f, -0.5755f},  {-1.1866f, 44.8713f, -0.8098f}, {-0.0168f, -36.7760f, -0.8306f},
            {0.0000f, -28.3306f, -0.8402f},  {0.0133f, -19.6426f, -0.8439f}, {0.0121f, -13.0521f, -0.8415f},
            {-0.0342f, -0.8927f, -0.8366f},  {-0.0001f, 7.9104f, -0.8355f},  {-0.0496f, 16.8690f, -0.8552f},
            {0.0037f, 18.4928f, -0.8391f},   {-0.0178f, 18.1408f, -0.8466f}, {-0.0265f, 16.5291f, -0.8487f},
            {0.0031f, 14.5694f, -0.8396f},   {0.0009f, 11.8922f, -0.8403f},  {-0.0002f, 8.2837f, -0.8406f},
            {0.0029f, 3.0904f, -0.8403f},    {-0.0110f, -4.9085f, -0.8386f}, {-0.0192f, -17.8808f, -0.8279f},
            {-0.0799f, -42.4392f, -0.7868f}, {-0.8735f, 17.1091f, -0.2649f}};

        std::ranges::reverse(dominosRotation);

        std::vector<std::pair<int, int>> verticalRange = {{0, 1}, {18, 29}, {47, 71}, {89, 101}, {112, 124}};
        for (int i = 0; i < dominosPositions.size(); ++i) {
            glm::vec3 pos       = dominosPositions[i];
            const glm::vec3 rot = {0.0f, -dominosRotation[i].y, 0.0f};
            pos += offset;
            const assets::AssetLocation dominoVModel("my_package::dominoV@Demo");
            const assets::AssetLocation dominoHModel("my_package::dominoH@Demo");
            assets::AssetRef<assets::Model> dominoAssetRef;
            if (std::ranges::any_of(verticalRange, [i](const std::pair<int, int>& range) {
                    return i >= range.first && i < range.second;
                })) {
                dominoAssetRef = catalog.getAsset(dominoVModel).as<assets::Model>();
            } else {
                dominoAssetRef = catalog.getAsset(dominoHModel).as<assets::Model>();
            }
            const auto domino        = EntityFactory3D::createModel(dominoAssetRef, pos, {0.01f, 0.01f, 0.01f}, rot);
            auto rootComp = Application::m_coordinator->getComponent<components::RootComponent>(domino);
            auto model = rootComp.modelRef.lock();
            auto boundingBox         = model->rootBounds;
            auto transformComp =
                Application::m_coordinator->getComponent<components::TransformComponent>(domino);
            const JPH::BodyID bodyId = app.getPhysicsSystem()->createBodyFromBounds(domino, transformComp, boundingBox,
                                                                                    JPH::EMotionType::Dynamic);
            if (bodyId.IsInvalid()) {
                THROW_EXCEPTION(InvalidBodyId, domino);
            }
            scene.addEntity(domino);
        }
    }

    void EditorScene::videoScene(const glm::vec3& offset) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        const auto videoBillboard = EntityFactory3D::createBillboard(
            {7.39f + offset.x, 4.44f + offset.y, -0.02f + offset.z}, {1.48f, 0.88f, 13.11f}, {1.0f, 1.0f, 1.0f, 1.0f});
        auto &transform = Application::m_coordinator->getComponent<components::TransformComponent>(videoBillboard);
        transform.quat = glm::quat(glm::radians(glm::vec3(-151.69f, -38.03f, 138.55f)));
        components::VideoComponent videoComponent;
        videoComponent.path      = nexo::Path::resolvePathRelativeToExe("../resources/videos/test.mp4").string();
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

    void EditorScene::chambouleScene(const glm::vec3& offset) const
    {
        auto& app           = getApp();
        scene::Scene& scene = app.getSceneManager().getScene(m_sceneId);

        createEntityWithPhysic({0.0f + offset.x, 0.0f + offset.y, 0.0f + offset.z}, {30.0f, 0.5f, 30.0f},
                               {0.0f, 0.0f, 0.0f}, {0.3f, 0.3f, 0.3f, 1.0f}, system::ShapeType::Box,
                               JPH::EMotionType::Static);

        createEntityWithPhysic({2.0f + offset.x, 6.0f + offset.y, 0.0f + offset.z}, {8.0f, 0.5f, 6.0f},
                               {0.0f, 0.0f, 25.0f}, {0.8f, 0.3f, 0.2f, 1.0f}, system::ShapeType::Box,
                               JPH::EMotionType::Static);

        createEntityWithPhysic({2.0f + offset.x, 15.0f + offset.y, 0.0f + offset.z}, {1.0f, 1.0f, 1.0f},
                               {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, system::ShapeType::Sphere,
                               JPH::EMotionType::Dynamic);

        const glm::vec3 pyramidBase = {-8.0f + offset.x, 0.5f + offset.y, 0.0f + offset.z};
        constexpr float cubeSize    = 1.0f;
        constexpr float spacing     = 0.05f;

        const std::vector<glm::vec4> levelColors = {
            {0.2f, 0.6f, 0.9f, 1.0f},
            {0.3f, 0.7f, 0.4f, 1.0f},
            {0.9f, 0.7f, 0.2f, 1.0f},
            {0.9f, 0.3f, 0.3f, 1.0f}
        };

        for (int i = 0; i < 4; ++i) {
            const float zPos = pyramidBase.z + (static_cast<float>(i) - 1.5f) * (cubeSize + spacing);
            createEntityWithPhysic({pyramidBase.x, pyramidBase.y, zPos}, {cubeSize, cubeSize, cubeSize},
                                   {0.0f, 0.0f, 0.0f}, levelColors[0], system::ShapeType::Box,
                                   JPH::EMotionType::Dynamic);
        }

        for (int i = 0; i < 3; ++i) {
            const float zPos = pyramidBase.z + (static_cast<float>(i) - 1.0f) * (cubeSize + spacing);
            const float yPos = pyramidBase.y + (cubeSize + spacing);
            createEntityWithPhysic({pyramidBase.x, yPos, zPos}, {cubeSize, cubeSize, cubeSize}, {0.0f, 0.0f, 0.0f},
                                   levelColors[1], system::ShapeType::Box, JPH::EMotionType::Dynamic);
        }

        for (int i = 0; i < 2; ++i) {
            const float zPos = pyramidBase.z + (static_cast<float>(i) - 0.5f) * (cubeSize + spacing);
            const float yPos = pyramidBase.y + 2.0f * (cubeSize + spacing);
            createEntityWithPhysic({pyramidBase.x, yPos, zPos}, {cubeSize, cubeSize, cubeSize}, {0.0f, 0.0f, 0.0f},
                                   levelColors[2], system::ShapeType::Box, JPH::EMotionType::Dynamic);
        }

        const float topYPos = pyramidBase.y + 3.0f * (cubeSize + spacing);
        createEntityWithPhysic({pyramidBase.x, topYPos, pyramidBase.z}, {cubeSize, cubeSize, cubeSize},
                               {0.0f, 0.0f, 0.0f}, levelColors[3], system::ShapeType::Box,
                               JPH::EMotionType::Dynamic);

        const auto [linear, quad] = math::computeAttenuationFromDistance(50.0f);
        const auto pointLight     = LightFactory::createPointLight({0.0f + offset.x, 10.0f + offset.y, 5.0f + offset.z},
                                                                {1.0f, 1.0f, 1.0f}, linear, quad);
        scene.addEntity(pointLight);
    }

} // namespace nexo::editor
