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
        scene.addEntity(LightFactory::createAmbientLight({1.0f, 1.0f, 1.0f}));
        scene.addEntity(LightFactory::createDirectionalLight({0.0f, -0.8f, 0.0f}));

        m_sceneUuid = app.getSceneManager().getScene(m_sceneId).getUuid();
        if (m_defaultScene) {
            loadDefaultEntities(); // Load default entities for testing purposes
            physicScene(glm::vec3{-60.0f, 0.0f, 0.0f});
            videoScene(glm::vec3{-15.0f, 0.0f, 0.0f});
            lightsScene(glm::vec3{50.0f, 0.0f, 0.0f});
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
