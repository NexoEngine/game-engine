//// MainScene.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source for the main scene document window
//
///////////////////////////////////////////////////////////////////////////////

#include "MainScene.hpp"

#include <Path.hpp>

#include "EntityFactory2D.hpp"
#include "EntityFactory3D.hpp"
#include "Nexo.hpp"
#include "math/Matrix.hpp"
#include "core/camera/PerspectiveCameraController.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

#include "SceneViewManager.hpp"
#include "Primitive.hpp"

namespace nexo::editor {

    MainScene::MainScene(std::string sceneName, const bool defaultScene) : m_sceneName(std::move(sceneName)),
                                                                           m_defaultScene(defaultScene) {}

    MainScene::~MainScene()
    = default;

    void MainScene::setup()
    {
        setupImguizmo();
        setupWindow();
        setupFramebuffer();
        setupScene();
    }

    void MainScene::setupScene()
    {
        auto &app = getApp();
        _sceneID = app.createScene(m_sceneName);
        const scene::LayerId layerId = app.addNewLayer(_sceneID, "Layer 1");
        m_camera = std::make_shared<camera::PerspectiveCameraController>(_viewSize.x / _viewSize.y);
        //m_camera = std::make_shared<camera::OrthographicCameraController>(_viewSize.x / _viewSize.y, true);
        app.attachCamera(_sceneID, m_camera, layerId);
        if (m_defaultScene)
            loadDefaultEntities(layerId);
    }

    void MainScene::setupImguizmo() const
    {
        ImGuizmo::SetOrthographic(true);
    }

    float randomColor()
    {
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(rng);
    }

    void MainScene::loadDefaultEntities(const scene::LayerId defaultLayerId) const
    {
        auto &app = getApp();
        // const ecs::Entity basicQuad = EntityFactory2D::createQuad({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 45.0f);
        // app.addEntityToScene(basicQuad, _sceneID, static_cast<int>(defaultLayerId));
        app.setAmbientLightValue(_sceneID, glm::vec3(0.5f));
        const ecs::Entity basicCube = EntityFactory3D::createCube({0.0f, -5.0f, -5.0f}, {20.0f, 0.5f, 20.0f},
                                                               {0.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f, 1.0f});
        app.addEntityToScene(basicCube, _sceneID, static_cast<int>(defaultLayerId));

        // const ecs::Entity gunModel = EntityFactory3D::createModel(Path::resolvePathRelativeToExe("../assets/models/9mn/scene.gltf").string(), {0.0f, 0.0f, -2.0f}, {0.01f, 0.01f, 0.01f}, {0.0f, 0.0f, 0.0f});
        // app.addEntityToScene(gunModel, _sceneID, static_cast<int>(defaultLayerId));
        glm::vec3 lightPos1 = {1.2f, 5.0f, 0.1f};
        glm::vec4 color1 = {1.0f, 1.0f, 1.0f, 1.0f};
        auto pointLight1 = std::make_shared<components::PointLight>(lightPos1, color1);
        app.addLightToScene(_sceneID, pointLight1);

        glm::vec3 lightPos2 = {-1.2f, 5.0f, -0.1f};
        glm::vec4 color2 = {0.0f, 1.0f, 0.0f, 1.0f};
        auto pointLight2 = std::make_shared<components::PointLight>(lightPos2, color2);
        app.addLightToScene(_sceneID, pointLight2);

        glm::vec3 lightDirection = {0.2f, -1.0f, -0.3f};
        glm::vec4 colorDir = {1.0f, 1.0f, 1.0f, 1.0f};
        auto directionalLight = std::make_shared<components::DirectionalLight>(lightDirection, colorDir);
        app.addLightToScene(_sceneID, directionalLight);

        glm::vec3 spotDir = {0.0f, -1.0f, 0.0f};
        glm::vec3 spotPos = {0.0f, 0.5f, -2.0f};
        glm::vec4 colorSpot = {0.0f, 0.0f, 1.0f, 1.0f};
        auto spotLight = std::make_shared<components::SpotLight>(spotPos, spotDir, colorSpot, glm::cos(glm::radians(40.5f)), glm::cos(glm::radians(45.5f)));
        app.addLightToScene(_sceneID, spotLight);

        const int numCubes = 10;
        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> distX(-5.0f, 5.0f);   // Spread on X
        std::uniform_real_distribution<float> distY(-5.0f, 5.0f);   // Spread on Y
        std::uniform_real_distribution<float> distZ(-10.0f, -2.0f); // In front of camera (negative Z)
        std::uniform_real_distribution<float> distRot(0.0f, 360.0f);  // Full rotation range
        std::uniform_real_distribution<float> distSize(0.5f, 2.0f);   // Vary cube size


        std::shared_ptr<renderer::Texture2D> containerDiffuseTexture = renderer::Texture2D::create(Path::resolvePathRelativeToExe(
            "../assets/textures/container2.png").string());
        std::shared_ptr<renderer::Texture2D> containerSpecularTexture = renderer::Texture2D::create(Path::resolvePathRelativeToExe(
            "../assets/textures/container2_specular.png").string());

        for (int i = 0; i < numCubes; ++i)
        {
            glm::vec3 pos(distX(rng), distY(rng), distZ(rng));
            float cubeSize = distSize(rng);
            glm::vec3 size(cubeSize, cubeSize, cubeSize);
            glm::vec3 rotation(distRot(rng), distRot(rng), distRot(rng));
            components::Material material{};
            material.albedoTexture = containerDiffuseTexture;
            material.metallicMap = containerSpecularTexture;

            // Create a cube at the generated position with the given size, rotation, and color.
            ecs::Entity cube = EntityFactory3D::createCube(pos, size, rotation, material);
            //app.addEntityToScene(cube, _sceneID, static_cast<int>(defaultLayerId));
        }


        // glm::vec3 lightPos2 = {0.0f, 2.0f, 1.0f};
        // glm::vec4 color2 = {randomColor(), randomColor(), randomColor(), 1.0f};
        // auto pointLight2 = std::make_shared<components::PointLight>(lightPos2, color2);
        // app.addLightToScene(_sceneID, pointLight2);
        //
        // glm::vec3 lightPos3 = {1.0f, 2.0f, 0.0f};
        // glm::vec4 color3 = {randomColor(), randomColor(), randomColor(), 1.0f};
        // auto pointLight3 = std::make_shared<components::PointLight>(lightPos3, color3);
        // app.addLightToScene(_sceneID, pointLight3);

        // glm::vec3 lightPos4 = {-1.0f, 2.0f, 0.0f};
        // glm::vec4 color4 = {1.0f, 1.0f, 1.0f, 1.0f};
        // auto pointLight4 = std::make_shared<components::PointLight>(lightPos4, color4);
        // app.addLightToScene(_sceneID, pointLight4);
        //app.addEntityToScene(basicQuad, _sceneID, static_cast<int>(defaultLayerId));
    }

    void MainScene::setupWindow()
    {
        constexpr auto pos = ImVec2(118, 24);
        constexpr auto size = ImVec2(1280, 720);
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        _viewSize = size;
    }

    void MainScene::shutdown()
    {}

    void MainScene::setupFramebuffer()
    {
        renderer::FramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            renderer::FrameBufferTextureFormats::RGBA8, renderer::FrameBufferTextureFormats::RED_INTEGER, renderer::FrameBufferTextureFormats::Depth
        };
        framebufferSpecs.width = static_cast<unsigned int>(_viewSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(_viewSize.y);
        m_framebuffer = renderer::Framebuffer::create(framebufferSpecs);
    }

    void MainScene::handleKeyEvents()
    {}

    void MainScene::renderToolbar()
    {
        // static bool chose_primitive = false;
        // ImVec2 buttonSize = ImVec2(40, 40);
        constexpr float padding = 0.0f;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(padding, padding));
        ImGui::SetCursorScreenPos(ImVec2(_viewPosition.x + 10, _viewPosition.y + 10));
        if (ImGui::Button("Orthographic"))
        {
            // Switch to orthographic camera mode
            //engine::camera::setMode(CameraMode::ORTHOGRAPHIC);
        }

        ImGui::SameLine();
        if (ImGui::Button("Perspective"))
        {
            // Switch to perspective camera mode
            //engine::camera::setMode(CameraMode::PERSPECTIVE);
        }

        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_HAND_POINTER, buttonSize))
        // {
        //     // Set cursor to normal mode
        //     //engine::input::setCursorMode(CursorMode::NORMAL);
        // }
        //
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_HAND, buttonSize))
        // {
        //     // Set cursor to pan mode
        //     //engine::input::setCursorMode(CursorMode::PAN);
        // }
        //
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_ARROWS_SPIN, buttonSize))
        // {
        //     // Set cursor to rotate mode
        //     //engine::input::setCursorMode(CursorMode::ROTATE);
        // }
        //
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_CUBE, buttonSize))
        // {
        //     ImGui::OpenPopup("add_primitive");
        // }

        ImGui::SameLine();
        if (ImGui::BeginPopup("add_primitive"))
        {
            // const char *primitives_names[] = {
            //     "  Cube", "  Plan", "  Sphere",\
            //     "  Cylinder", "  Cone", "  Polygon", "  Torus", "  Knot",\
            //     "  Hemisphere"
            // };
            // PrimitiveFunction add_primitive_fct[] = {
            //     &Main3DScene::add_cube,
            //     &Main3DScene::add_plan,
            //     &Main3DScene::add_sphere,
            //     &Main3DScene::add_cylinder,
            //     &Main3DScene::add_cone,
            //     &Main3DScene::add_polygon,
            //     &Main3DScene::add_torus,
            //     &Main3DScene::add_knot,
            //     &Main3DScene::add_hemisphere
            // };
            ImGui::SeparatorText(" Add primitive ");
            // for (int i = 0; i < IM_ARRAYSIZE(primitives_names); i++)
            // {
            //     if (ImGui::Selectable(primitives_names[i], &chose_primitive))
            //     {
            //         (this->*add_primitive_fct[i])();
            //     }
            // }

            ImGui::EndPopup();
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(100);
        if (ImGui::DragInt("Target FPS", &_targetFPS, 1, 1, 120))
        {
            // Set target FPS
        }
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();
    }

    void MainScene::renderGizmo() const
    {
        const auto &coord = nexo::Application::m_coordinator;
        if (const auto &viewManager = SceneViewManager::getInstance();
            m_sceneManagerBridge->getSelectionType() != SelectionType::ENTITY ||
            viewManager->getSelectedScene() != static_cast<int>(_sceneID))
            return;
        auto &selectionData = m_sceneManagerBridge->getData();
        if (!std::holds_alternative<EntityProperties>(selectionData))
            return;
        const ecs::Entity entity = std::get<EntityProperties>(selectionData).entity;
        ImGuizmo::SetOrthographic(m_camera->getMode() == camera::CameraMode::ORTHOGRAPHIC);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetID(static_cast<int>(entity));
        ImGuizmo::SetRect(_viewPosition.x, _viewPosition.y, _viewSize.x, _viewSize.y);
        glm::mat4 viewMatrix = m_camera->getViewMatrix();
        glm::mat4 projectionMatrix = m_camera->getProjectionMatrix();
        auto transf = coord->tryGetComponent<components::TransformComponent>(entity);
        if (!transf)
            return;
        glm::quat rotationQuat = glm::quat(glm::radians(transf->get().rotation));
		glm::mat4 rotationMat = glm::toMat4(rotationQuat);
        glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), transf->get().pos) *
                                    rotationMat *
                                    glm::scale(glm::mat4(1.0f), {transf->get().size.x, transf->get().size.y, transf->get().size.z});
        ImGuizmo::Enable(true);
        ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
                             _currentGizmoOperation,
                             ImGuizmo::MODE::WORLD,
                             glm::value_ptr(transformMatrix));

        glm::vec3 translation(0);
        glm::vec3 rotation(0);
        glm::vec3 scale(0);

        math::decomposeTransformEuler(transformMatrix, translation, rotation, scale);

        if (ImGuizmo::IsUsing())
        {
            transf->get().pos = translation;
            transf->get().rotation = glm::degrees(rotation);
            transf->get().size = scale;
        }
    }

    void MainScene::renderView()
    {
        auto viewPortOffset = ImGui::GetCursorPos();

        // Resize handling
        if (ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            _viewSize.x != viewportPanelSize.x || _viewSize.y != viewportPanelSize.y)
        {
            m_framebuffer->resize(static_cast<unsigned int>(viewportPanelSize.x),
                                  static_cast<unsigned int>(viewportPanelSize.y));
            _viewSize.x = viewportPanelSize.x;
            _viewSize.y = viewportPanelSize.y;
            const auto event = std::make_shared<event::EventWindowResize>(viewportPanelSize.x, viewportPanelSize.y);
            getApp().getEventManager()->emitEvent<event::EventWindowResize>(event);
            m_sceneManagerBridge->setSceneActiveStatus(_sceneID, false);
        }

        // Render framebuffer
        const unsigned int textureId = m_framebuffer->getColorAttachmentId(0);
        ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)), _viewSize, ImVec2(0, 1), ImVec2(1, 0));
        m_framebuffer->clearAttachment<int>(1, -1); // ?

        auto windowSize = ImGui::GetWindowSize();
        auto minBounds = ImGui::GetWindowPos();

        minBounds.x += viewPortOffset.x;
        minBounds.y += viewPortOffset.y;

        ImVec2 maxBounds = {minBounds.x + windowSize.x, minBounds.y + windowSize.y};
        m_viewportBounds[0] = minBounds;
        m_viewportBounds[1] = maxBounds;
    }

    void MainScene::setHiddenLayerStatus(const bool status) const
    {
        for (const auto hiddenLayer: m_hiddenLayers)
            m_sceneManagerBridge->setLayerRenderStatus(_sceneID, hiddenLayer, status);
    }

    void MainScene::show()
    {
        setHiddenLayerStatus(false);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(480, 270), ImVec2(1920, 1080));

        if (ImGui::Begin(m_sceneName.c_str(), &m_opened, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
        {
            _viewPosition = ImGui::GetCursorScreenPos();

            m_focused = ImGui::IsWindowFocused();
            m_sceneManagerBridge->setSceneActiveStatus(_sceneID, m_focused);
            m_camera->zoomOn = m_focused;
            if (m_focused)
            {
                if (const auto &viewManager = SceneViewManager::getInstance();
                    viewManager->getSelectedScene() != static_cast<int>(_sceneID))
                {
                    viewManager->setSelectedScene(_sceneID);
                    m_sceneManagerBridge->unselectEntity();
                }
            }

            renderView();
            renderGizmo();
        }
        ImGui::End();
        ImGui::PopStyleVar();
        setHiddenLayerStatus(true);
    }

    void MainScene::update()
    {
        setHiddenLayerStatus(false);
        if (!m_opened)
            return;
        handleKeyEvents();

        m_framebuffer->bind();
        renderer::RenderCommand::setClearColor({0.0f, 0.0f, 0.0f, 1.0f});
        renderer::RenderCommand::clear();
        m_framebuffer->clearAttachment<int>(1, -1);
        runEngine(_sceneID, RenderingType::FRAMEBUFFER);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing())
        {
            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_viewportBounds[0].x;
            my -= m_viewportBounds[0].y;

            // Flip the y-coordinate to match opengl texture format (maybe make it modular in some way)
            my = _viewSize.y - my;

            if (mx >= 0 && my >= 0 && mx < _viewSize.x && my < _viewSize.y)
            {
                int data = m_framebuffer->getPixel<int>(1, mx, my);
                if (data != -1)
                {
                	std::cout << "Clicked on entity with ID: " << data << std::endl;
                    const auto &viewManager = SceneViewManager::getInstance();
                    m_sceneManagerBridge->setSelectedEntity(data);
                    viewManager->setSelectedScene(_sceneID);
                    m_sceneManagerBridge->setSelectionType(SelectionType::ENTITY);
                }
                else
                {
                    m_sceneManagerBridge->unselectEntity();
                }
            }
        }
        m_framebuffer->unbind();
        setHiddenLayerStatus(true);

    }

    void MainScene::addDefaultCameraToLayer(const scene::LayerId id) const
    {
        auto &layerStack = m_sceneManagerBridge->getSceneLayers(_sceneID);
        const std::shared_ptr<layer::Layer> layer = layerStack[id];
        const auto newCamera = std::make_shared<camera::OrthographicCameraController>(_viewSize.x / _viewSize.y, true);
        layer->attachCamera(newCamera);
    }

}
