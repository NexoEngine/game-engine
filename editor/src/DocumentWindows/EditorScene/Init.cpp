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
        m_editorCamera = static_cast<int>(CameraFactory::createPerspectiveCamera({0.0f, 3.0f, -2.0f}, static_cast<unsigned int>(m_contentSize.x), static_cast<unsigned int>(m_contentSize.y), renderTarget));
        auto &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(m_editorCamera);
        cameraComponent.render = true;
        auto maskPass = std::make_shared<renderer::MaskPass>(m_contentSize.x, m_contentSize.y);
        auto outlinePass = std::make_shared<renderer::OutlinePass>(m_contentSize.x, m_contentSize.y);
        auto gridPass = std::make_shared<renderer::GridPass>(m_contentSize.x, m_contentSize.y);

        renderer::PassId forwardId = cameraComponent.pipeline.getFinalOutputPass();
        renderer::PassId maskId = cameraComponent.pipeline.addRenderPass(std::move(maskPass));
        renderer::PassId outlineId = cameraComponent.pipeline.addRenderPass(std::move(outlinePass));
        renderer::PassId gridId = cameraComponent.pipeline.addRenderPass(std::move(gridPass));
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

        assets::AssetImporter importer;
    }

    void EditorScene::loadDefaultEntities() const
    {
        auto &app = getApp();
        scene::Scene &scene = app.getSceneManager().getScene(m_sceneId);
        const ecs::Entity ambientLight = LightFactory::createAmbientLight({0.5f, 0.5f, 0.5f});
        scene.addEntity(ambientLight);
        // const ecs::Entity pointLight = LightFactory::createPointLight({2.0f, 5.0f, 0.0f});
        // utils::addPropsTo(pointLight, utils::PropsType::POINT_LIGHT);
        // scene.addEntity(pointLight);
        const ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.2f, -1.0f, -0.3f});
        scene.addEntity(directionalLight);
        // const ecs::Entity spotLight = LightFactory::createSpotLight({-2.0f, 5.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
        // utils::addPropsTo(spotLight, utils::PropsType::SPOT_LIGHT);
        // scene.addEntity(spotLight);
        // const ecs::Entity basicCube = EntityFactory3D::createCube({0.0f, 0.25f, 0.0f}, {20.0f, 0.5f, 20.0f},
        //                                                        {0.0f, 0.0f, 0.0f}, {0.05f * 1.7, 0.09f * 1.35, 0.13f * 1.45, 1.0f});
        // app.getSceneManager().getScene(m_sceneId).addEntity(basicCube);
        const ecs::Entity basicSphere = EntityFactory3D::createSphere({0.0f, 0.0f, -5.0f}, {1.0f, 1.0f, 1.0f},
                                                                                {0.0f, 0.0f, 0.0f}, {
                                                                                    0.05f * 1.5, 0.09f * 1.15,
                                                                                    0.13f * 1.25, 1.0f
                                                                                });
        app.getSceneManager().getScene(m_sceneId).addEntity(basicSphere);

        assets::AssetImporter importer;

        lightsScene(m_sceneId);

        // 9mn
        std::filesystem::path path9mn = Path::resolvePathRelativeToExe("../resources/models/9mn/scene.gltf");
        assets::ImporterFileInput fileInput9mn{path9mn};
        auto assetRef9mn = importer.importAsset<assets::Model>(assets::AssetLocation("my_package::9mn@DefaultScene/"), fileInput9mn);

        const ecs::Entity Model9mn = EntityFactory3D::createModel(assetRef9mn, {3.0f, 2.0f, 0.0f}, {0.01f, 0.01f, 0.01f},
                                                               {0.0f, 0.0f, 0.0f});
        app.getSceneManager().getScene(m_sceneId).addEntity(Model9mn);

        // SmilingFace
        const std::filesystem::path pathSmilingFace = Path::resolvePathRelativeToExe("../resources/models/SmilingFace/SmilingFace.gltf");
        assets::ImporterFileInput fileInputSmilingFace{pathSmilingFace};
        const auto assetRefModelSmilingFace = importer.importAsset<assets::Model>(
            assets::AssetLocation("my_package::SmilingFace@DefaultScene/"), fileInputSmilingFace
        );
        const ecs::Entity ModelSmilingFace = EntityFactory3D::createModel(assetRefModelSmilingFace, {-3.0f, 2.0f, 0.0f},
                                                                 {2.0f, 2.0f, 2.0f},
                                                                 {0.0f, 0.0f, 0.0f});

        app.getSceneManager().getScene(m_sceneId).addEntity(ModelSmilingFace);
        // Avocado
        const std::filesystem::path pathAvocado = Path::resolvePathRelativeToExe("../resources/models/Avocado/Avocado.gltf");
        assets::ImporterFileInput fileInputAvocado{pathAvocado};
        const auto assetRefModelAvocado = importer.importAsset<assets::Model>(
            assets::AssetLocation("my_package::Avocado@DefaultScene/"), fileInputAvocado
        );
        const ecs::Entity ModelAvocado = EntityFactory3D::createModel(assetRefModelAvocado, {0.0f, 2.0f, 0.0f},
                                                                 {50.0f, 50.0f, 50.0f}, {0.0f, 0.0f, 0.0f});

        // DiscoBall
        // const std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/DiscoBall/discoball.fbx");
        // assets::ImporterFileInput fileInput{path};
        // const auto assetRefModel = importer.importAsset<assets::Model>(
        //     assets::AssetLocation("my_package::discoball@DefaultScene/"), fileInput
        // );
        // const ecs::Entity Model = EntityFactory3D::createModel(assetRefModel, {0.0f, 0.0f, 0.0f},
        //                                                          {1.0f, 1.0f, 1.0f},
        //                                                          {0.0f, 0.0f, 0.0f});

        app.getSceneManager().getScene(m_sceneId).addEntity(ModelAvocado);
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
